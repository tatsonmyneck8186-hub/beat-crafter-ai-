#include "Components/CombatComponent.h"
#include "Components/BoxerStatsComponent.h"
#include "Core/IBoxerInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "HAL/PlatformTime.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Jab
    FAttackData Jab;
    Jab.MoveType = EBoxingMove::Jab;
    Jab.BaseDamage = 8.f;  Jab.StaminaCost = 8.f;  Jab.KOMeterGain = 4.f;
    Jab.HitStunDuration = 0.10f; Jab.HitPauseDuration = 0.05f;
    Jab.AttackRange = 160.f;
    Jab.HitboxActiveStart = 0.08f; Jab.HitboxActiveEnd = 0.22f;
    Jab.TotalDuration = 0.40f; Jab.CameraShakeIntensity = 0.6f;
    AttackDataMap.Add(EBoxingMove::Jab, Jab);

    // Hook
    FAttackData Hook;
    Hook.MoveType = EBoxingMove::Hook;
    Hook.BaseDamage = 14.f; Hook.StaminaCost = 14.f; Hook.KOMeterGain = 8.f;
    Hook.HitStunDuration = 0.16f; Hook.HitPauseDuration = 0.07f;
    Hook.AttackRange = 155.f;
    Hook.HitboxActiveStart = 0.12f; Hook.HitboxActiveEnd = 0.28f;
    Hook.TotalDuration = 0.55f; Hook.CameraShakeIntensity = 0.9f;
    AttackDataMap.Add(EBoxingMove::Hook, Hook);

    // Uppercut
    FAttackData Uppercut;
    Uppercut.MoveType = EBoxingMove::Uppercut;
    Uppercut.BaseDamage = 20.f; Uppercut.StaminaCost = 20.f; Uppercut.KOMeterGain = 14.f;
    Uppercut.HitStunDuration = 0.22f; Uppercut.HitPauseDuration = 0.10f;
    Uppercut.AttackRange = 145.f;
    Uppercut.HitboxActiveStart = 0.18f; Uppercut.HitboxActiveEnd = 0.35f;
    Uppercut.TotalDuration = 0.70f; Uppercut.CameraShakeIntensity = 1.4f;
    AttackDataMap.Add(EBoxingMove::Uppercut, Uppercut);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Hit-pause restore: use real time so dilation doesn't extend the freeze
    if (bInHitPause) TickHitPause();

    if (bIsAttacking) TickAttack(DeltaTime);
    if (bInHitStun)   TickHitStun(DeltaTime);
    if (bIsDodging)   TickDodge(DeltaTime);
}

void UCombatComponent::SetStatsComponent(UBoxerStatsComponent* InStats)
{
    StatsComp = InStats;
}

void UCombatComponent::SetOpponent(AActor* InOpponent)
{
    OpponentActor = InOpponent;
}

void UCombatComponent::SetAttackData(EBoxingMove Move, const FAttackData& Data)
{
    AttackDataMap.Add(Move, Data);
}

bool UCombatComponent::TryAttack(EBoxingMove Move)
{
    if (!CanAct()) return false;
    if (Move == EBoxingMove::None) return false;

    const FAttackData* Data = AttackDataMap.Find(Move);
    if (!Data) return false;

    if (StatsComp && !StatsComp->ConsumeStamina(Data->StaminaCost)) return false;

    CurrentAttack = Move;
    CurrentAttackData = *Data;
    bIsAttacking = true;
    bHitboxActive = false;
    bHitRegisteredThisAttack = false;
    AttackElapsed = 0.f;

    OnAttackExecuted.Broadcast(Move);
    OnAttackStateChanged.Broadcast(true);
    return true;
}

void UCombatComponent::ReceiveHit(const FAttackData& AttackData, AActor* Attacker)
{
    if (bIsDucking && AttackData.MoveType != EBoxingMove::Uppercut) return;
    if (bIsDodging) return;

    if (StatsComp)
    {
        StatsComp->ApplyDamage(AttackData.BaseDamage);
        StatsComp->AddKOMeter(AttackData.KOMeterGain);
    }

    bInHitStun = true;
    bIsAttacking = false;
    HitStunDuration = AttackData.HitStunDuration;
    HitStunElapsed = 0.f;

    TriggerHitPause(AttackData.HitPauseDuration);
    TriggerCameraShake(AttackData.CameraShakeIntensity);

    if (GetOwner())
    {
        SpawnHitEffect(GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 80.f));
    }
}

void UCombatComponent::StartDodge()
{
    if (!CanAct()) return;
    bIsDodging = true;
    DodgeElapsed = 0.f;
}

void UCombatComponent::StartDuck() { bIsDucking = true; }
void UCombatComponent::StopDuck()  { bIsDucking = false; }

bool UCombatComponent::CanAct() const
{
    return !bIsAttacking && !bInHitStun && !bIsDodging;
}

void UCombatComponent::TickAttack(float DeltaTime)
{
    AttackElapsed += DeltaTime;

    if (!bHitboxActive
        && AttackElapsed >= CurrentAttackData.HitboxActiveStart
        && AttackElapsed < CurrentAttackData.HitboxActiveEnd)
    {
        bHitboxActive = true;
    }
    else if (bHitboxActive && AttackElapsed >= CurrentAttackData.HitboxActiveEnd)
    {
        bHitboxActive = false;
    }

    if (bHitboxActive && !bHitRegisteredThisAttack) CheckHit();
    if (AttackElapsed >= CurrentAttackData.TotalDuration) EndAttack();
}

void UCombatComponent::TickHitStun(float DeltaTime)
{
    HitStunElapsed += DeltaTime;
    if (HitStunElapsed >= HitStunDuration)
    {
        bInHitStun = false;
        HitStunElapsed = 0.f;
    }
}

void UCombatComponent::TickDodge(float DeltaTime)
{
    DodgeElapsed += DeltaTime;
    if (DodgeElapsed >= DodgeDuration)
    {
        bIsDodging = false;
        DodgeElapsed = 0.f;
    }
}

void UCombatComponent::TickHitPause()
{
    // Restore time dilation using real (wall-clock) time,
    // immune to the dilation we just applied.
    double Now = FPlatformTime::Seconds();
    if (Now - HitPauseStartRealTime >= HitPauseRealDuration)
    {
        bInHitPause = false;
        UWorld* World = GetWorld();
        if (World)
        {
            UGameplayStatics::SetGlobalTimeDilation(World, 1.f);
        }
    }
}

void UCombatComponent::CheckHit()
{
    if (!OpponentActor || !GetOwner()) return;

    float Distance = FVector::Dist(
        GetOwner()->GetActorLocation(),
        OpponentActor->GetActorLocation());

    if (Distance > CurrentAttackData.AttackRange) return;

    FVector ToOpponent = (OpponentActor->GetActorLocation()
        - GetOwner()->GetActorLocation()).GetSafeNormal();
    if (FVector::DotProduct(GetOwner()->GetActorForwardVector(), ToOpponent) < 0.25f)
        return;

    bHitRegisteredThisAttack = true;

    if (OpponentActor->Implements<UBoxerInterface>())
    {
        IBoxerInterface::Execute_ReceiveHit(
            OpponentActor, CurrentAttackData, GetOwner());
    }
    OnHitLanded.Broadcast(CurrentAttack, EHitResult::Hit);
}

void UCombatComponent::EndAttack()
{
    bIsAttacking = false;
    bHitboxActive = false;
    CurrentAttack = EBoxingMove::None;
    AttackElapsed = 0.f;
    OnAttackStateChanged.Broadcast(false);
}

void UCombatComponent::TriggerHitPause(float RealDuration)
{
    if (RealDuration <= 0.f) return;
    UWorld* World = GetWorld();
    if (!World) return;

    UGameplayStatics::SetGlobalTimeDilation(World, 0.05f);
    bInHitPause = true;
    HitPauseStartRealTime = FPlatformTime::Seconds();
    HitPauseRealDuration = RealDuration;
}

void UCombatComponent::TriggerCameraShake(float Intensity)
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return;

    TSubclassOf<UCameraShakeBase> ShakeClass =
        (Intensity >= 1.2f && HeavyHitCameraShake)
        ? HeavyHitCameraShake : LightHitCameraShake;

    if (ShakeClass)
    {
        PC->ClientStartCameraShake(ShakeClass, Intensity);
    }
}

void UCombatComponent::SpawnHitEffect(const FVector& Location)
{
    if (!HitParticle) return;
    UWorld* World = GetWorld();
    if (!World) return;
    UGameplayStatics::SpawnEmitterAtLocation(World, HitParticle, Location);
}
