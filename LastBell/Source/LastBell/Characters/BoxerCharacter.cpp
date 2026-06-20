#include "Characters/BoxerCharacter.h"
#include "Components/BoxerStatsComponent.h"
#include "Components/CombatComponent.h"
#include "Data/FighterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ABoxerCharacter::ABoxerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Stats
    StatsComponent = CreateDefaultSubobject<UBoxerStatsComponent>(TEXT("StatsComponent"));

    // Combat
    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

    // Character movement - restrict to X axis for arcade style
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    if (Movement)
    {
        Movement->bConstrainToPlane = false;
        Movement->GravityScale = 1.f;
        Movement->MaxWalkSpeed = 380.f;
        Movement->BrakingDecelerationWalking = 2048.f;
        Movement->bOrientRotationToMovement = false;
    }

    // Disable automatic rotation from controller
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void ABoxerCharacter::BeginPlay()
{
    Super::BeginPlay();

    CombatComponent->SetStatsComponent(StatsComponent);

    if (FighterData)
    {
        LoadFighterData(FighterData);
    }

    // Bind stat events
    StatsComponent->OnKnockdown.AddDynamic(this, &ABoxerCharacter::HandleKnockdown);
    StatsComponent->OnKO.AddDynamic(this, &ABoxerCharacter::HandleKO);
}

void ABoxerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bBoxerEnabled)
    {
        UpdateFacingDirection();
        ClampToRingBounds();
    }
}

void ABoxerCharacter::LoadFighterData(UFighterDataAsset* Data)
{
    if (!Data) return;
    FighterData = Data;

    StatsComponent->InitializeStats(Data->Stats);
    CombatComponent->SetAttackData(EBoxingMove::Jab, Data->JabData);
    CombatComponent->SetAttackData(EBoxingMove::Hook, Data->HookData);
    CombatComponent->SetAttackData(EBoxingMove::Uppercut, Data->UppercutData);

    if (Data->FighterMesh && GetMesh())
    {
        GetMesh()->SetSkeletalMesh(Data->FighterMesh);
    }
    if (Data->AnimInstanceClass && GetMesh())
    {
        GetMesh()->SetAnimInstanceClass(Data->AnimInstanceClass);
    }

    // Apply fighter colors via dynamic material
    if (GetMesh())
    {
        UMaterialInterface* Mat = GetMesh()->GetMaterial(0);
        if (Mat)
        {
            UMaterialInstanceDynamic* DynMat =
                UMaterialInstanceDynamic::Create(Mat, this);
            DynMat->SetVectorParameterValue(TEXT("PrimaryColor"), Data->PrimaryColor);
            DynMat->SetVectorParameterValue(TEXT("SecondaryColor"), Data->SecondaryColor);
            GetMesh()->SetMaterial(0, DynMat);
        }
    }
}

// ─── IBoxerInterface ─────────────────────────────────────────────────────────────────

void ABoxerCharacter::ExecuteAttack_Implementation(EBoxingMove Move)
{
    if (!bBoxerEnabled || bIsKnockedDown) return;
    if (CombatComponent->TryAttack(Move))
    {
        OnAttackStarted(Move);
        CurrentState = EBoxerState::Attacking;
    }
}

void ABoxerCharacter::ReceiveHit_Implementation(const FAttackData& AttackData, AActor* Attacker)
{
    if (bIsKnockedDown) return;
    CombatComponent->ReceiveHit(AttackData, Attacker);
    OnHitReceived(AttackData.MoveType);
    CurrentState = EBoxerState::HitStun;
}

void ABoxerCharacter::ExecuteDodge_Implementation()
{
    if (!bBoxerEnabled || bIsKnockedDown) return;
    CombatComponent->StartDodge();
    CurrentState = EBoxerState::Dodging;
    OnDodgeStarted();
}

void ABoxerCharacter::ExecuteDuck_Implementation()
{
    if (!bBoxerEnabled || bIsKnockedDown) return;
    CombatComponent->StartDuck();
    CurrentState = EBoxerState::Ducking;
}

void ABoxerCharacter::StopDuck_Implementation()
{
    CombatComponent->StopDuck();
    if (CurrentState == EBoxerState::Ducking)
    {
        CurrentState = EBoxerState::Idle;
    }
}

EBoxerState ABoxerCharacter::GetBoxerState_Implementation() const
{
    return CurrentState;
}

float ABoxerCharacter::GetHealthPercent_Implementation() const
{
    return StatsComponent->GetHealthPercent();
}

float ABoxerCharacter::GetStaminaPercent_Implementation() const
{
    return StatsComponent->GetStaminaPercent();
}

float ABoxerCharacter::GetKOMeterPercent_Implementation() const
{
    return StatsComponent->GetKOMeterPercent();
}

bool ABoxerCharacter::IsAttacking_Implementation() const
{
    return CombatComponent->IsAttacking();
}

bool ABoxerCharacter::IsDucking_Implementation() const
{
    return CombatComponent->IsDucking();
}

void ABoxerCharacter::SetOpponent_Implementation(AActor* Opponent)
{
    OpponentRef = Opponent;
    CombatComponent->SetOpponent(Opponent);
}

AActor* ABoxerCharacter::GetOpponent_Implementation() const
{
    return OpponentRef;
}

// ─── State ───────────────────────────────────────────────────────────────────

void ABoxerCharacter::SetBoxerEnabled(bool bEnabled)
{
    bBoxerEnabled = bEnabled;
    if (!bEnabled)
    {
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ABoxerCharacter::TriggerKnockdown()
{
    bIsKnockedDown = true;
    bBoxerEnabled = false;
    CurrentState = EBoxerState::KnockedDown;
    GetCharacterMovement()->StopMovementImmediately();
    OnKnockdownTriggered();
}

void ABoxerCharacter::RecoverFromKnockdown()
{
    bIsKnockedDown = false;
    bBoxerEnabled = true;
    CurrentState = EBoxerState::Idle;
    StatsComponent->ResetStats();
}

void ABoxerCharacter::UpdateFacingDirection()
{
    if (!OpponentRef) return;
    FVector ToOpponent = OpponentRef->GetActorLocation() - GetActorLocation();
    ToOpponent.Z = 0.f;
    if (ToOpponent.IsNearlyZero()) return;

    FRotator Target = ToOpponent.ToOrientationRotator();
    SetActorRotation(FMath::RInterpTo(
        GetActorRotation(), Target,
        GetWorld()->GetDeltaSeconds(), 20.f));
}

void ABoxerCharacter::ClampToRingBounds()
{
    FVector Loc = GetActorLocation();
    float HalfW = RingHalfWidth;
    Loc.Y = FMath::Clamp(Loc.Y, RingCenter.Y - HalfW, RingCenter.Y + HalfW);
    SetActorLocation(Loc);
}

void ABoxerCharacter::HandleKnockdown()
{
    TriggerKnockdown();
}

void ABoxerCharacter::HandleKO()
{
    bIsKO = true;
    CurrentState = EBoxerState::KO;
    bBoxerEnabled = false;
    OnKOTriggered();
}
