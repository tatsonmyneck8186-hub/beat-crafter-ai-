#include "Components/CombatComponent.h"
#include "Components/BoxerStatsComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Default attack data
    FAttackData JabDefault;
    JabDefault.MoveType = EBoxingMove::Jab;
    JabDefault.BaseDamage = 8.f;
    JabDefault.StaminaCost = 8.f;
    JabDefault.KOMeterGain = 4.f;
    JabDefault.HitStunDuration = 0.1f;
    JabDefault.HitPauseDuration = 0.05f;
    JabDefault.AttackRange = 160.f;
    JabDefault.HitboxActiveStart = 0.08f;
    JabDefault.HitboxActiveEnd = 0.22f;
    JabDefault.TotalDuration = 0.40f;
    JabDefault.CameraShakeIntensity = 0.6f;
    AttackDataMap.Add(EBoxingMove::Jab, JabDefault);

    FAttackData HookDefault;
    HookDefault.MoveType = EBoxingMove::Hook;
    HookDefault.BaseDamage = 14.f;
    HookDefault.StaminaCost = 14.f;
    HookDefault.KOMeterGain = 8.f;
    HookDefault.HitStunDuration = 0.16f;
    HookDefault.HitPauseDuration = 0.07f;
    HookDefault.AttackRange = 155.f;
    HookDefault.HitboxActiveStart = 0.12f;
    HookDefault.HitboxActiveEnd = 0.28f;
    HookDefault.TotalDuration = 0.55f;
    HookDefault.CameraShakeIntensity = 0.9f;
    AttackDataMap.Add(EBoxingMove::Hook, HookDefault);

    FAttackData UppercutDefault;
    UppercutDefault.MoveType = EBoxingMove::Uppercut;
    UppercutDefault.BaseDamage = 20.f;
    UppercutDefault.StaminaCost = 20.f;
    UppercutDefault.KOMeterGain = 14.f;
    UppercutDefault.HitStunDuration = 0.22f;
    UppercutDefault.HitPauseDuration = 0.10f;
    UppercutDefault.AttackRange = 145.f;
    UppercutDefault.HitboxActiveStart = 0.18f;
    UppercutDefault.HitboxActiveEnd = 0.35f;
    UppercutDefault.TotalDuration = 0.70f;
    UppercutDefault.CameraShakeIntensity = 1.4f;
    AttackDataMap.Add(EBoxingMove::Uppercut, UppercutDefault);
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsAttacking)    TickAttack(DeltaTime);
    if (bInHitStun)      TickHitStun(DeltaTime);
    if (bIsDodging)      TickDodge(DeltaTime);
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
    // Duck evades jab/hook
    if (bIsDucking && AttackData.MoveType != EBoxingMove::Uppercut) return;
    // Active dodge evades all
    if (bIsDodging) return;

    if (StatsComp)
    {
        float Damage = AttackData.BaseDamage * (Attacker ?
            1.f : 1.f); // attacker damage mult applied by caller
        StatsComp->ApplyDamage(Damage);
        StatsComp->AddKOMeter(AttackData.KOMeterGain);
    }

    // Enter hitstun
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
    DodgeDuration = 0.28f;
}

void UCombatComponent::StartDuck()
{
    bIsDucking = true;
}

void UCombatComponent::StopDuck()
{
    bIsDucking = false;
}

bool UCombatComponent::CanAct() const
{
    return !bIsAttacking && !bInHitStun && !bIsDodging;
}

void UCombatComponent::TickAttack(float DeltaTime)
{
    AttackElapsed += DeltaTime;

    // Activate hitbox window
    if (!bHitboxActive && AttackElapsed >= CurrentAttackData.HitboxActiveStart
        && AttackElapsed < CurrentAttackData.HitboxActiveEnd)
    {
        bHitboxActive = true;
    }
    else if (bHitboxActive && AttackElapsed >= CurrentAttackData.HitboxActiveEnd)
    {
        bHitboxActive = false;
    }

    // Check for hit during active window
    if (bHitboxActive && !bHitRegisteredThisAttack)
    {
        CheckHit();
    }

    // End attack
    if (AttackElapsed >= CurrentAttackData.TotalDuration)
    {
        EndAttack();
    }
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

void UCombatComponent::CheckHit()
{
    if (!OpponentActor || !GetOwner()) return;

    float Distance = FVector::Dist(
        GetOwner()->GetActorLocation(),
        OpponentActor->GetActorLocation());

    if (Distance > CurrentAttackData.AttackRange) return;

    // Check opponent is facing us (in front)
    FVector ToOpponent = (OpponentActor->GetActorLocation()
        - GetOwner()->GetActorLocation()).GetSafeNormal();
    float DotToForward = FVector::DotProduct(GetOwner()->GetActorForwardVector(), ToOpponent);
    if (DotToForward < 0.3f) return;

    bHitRegisteredThisAttack = true;

    // Apply hit to opponent through interface
    if (OpponentActor->Implements<UBoxerInterface>())
    {
        IBoxerInterface::Execute_ReceiveHit(OpponentActor,
            CurrentAttackData, GetOwner());
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

void UCombatComponent::TriggerHitPause(float Duration)
{
    if (Duration <= 0.f) return;
    UWorld* World = GetWorld();
    if (!World) return;

    // Slow time briefly
    UGameplayStatics::SetGlobalTimeDilation(World, 0.05f);

    FTimerHandle PauseHandle;
    World->GetTimerManager().SetTimer(PauseHandle, [World]()
    {
        if (IsValid(World))
        {
            UGameplayStatics::SetGlobalTimeDilation(World, 1.f);
        }
    }, Duration, false);
}

void UCombatComponent::TriggerCameraShake(float Intensity)
{
    if (!LightHitCameraShake) return;
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return;

    TSubclassOf<UCameraShakeBase> ShakeClass =
        (Intensity >= 1.2f && HeavyHitCameraShake) ? HeavyHitCameraShake : LightHitCameraShake;

    PC->ClientStartCameraShake(ShakeClass, Intensity);
}

void UCombatComponent::SpawnHitEffect(const FVector& Location)
{
    if (!HitParticle) return;
    UWorld* World = GetWorld();
    if (!World) return;
    UGameplayStatics::SpawnEmitterAtLocation(World, HitParticle, Location);
}
