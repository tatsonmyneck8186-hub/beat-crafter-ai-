#include "Characters/AIBoxer.h"
#include "Components/BoxerStatsComponent.h"
#include "Components/CombatComponent.h"
#include "Data/FighterDataAsset.h"
#include "Core/IBoxerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIBoxer::AAIBoxer()
{
    PrimaryActorTick.bCanEverTick = true;
    // AI doesn't need a camera
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAIBoxer::BeginPlay()
{
    Super::BeginPlay();

    if (FighterData)
    {
        PreferredRange = FighterData->AIPersonality.PreferredRange;
    }
}

void AAIBoxer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAIBoxer::AIExecuteAttack(EBoxingMove Move)
{
    ExecuteAttack_Implementation(Move);
}

void AAIBoxer::AIExecuteDodge()
{
    ExecuteDodge_Implementation();
}

void AAIBoxer::AIMoveTowardPlayer(float DeltaTime)
{
    if (!OpponentRef || !bBoxerEnabled) return;

    FVector ToOpponent = (OpponentRef->GetActorLocation()
        - GetActorLocation()).GetSafeNormal();
    ToOpponent.Z = 0.f;

    float Speed = StatsComponent->GetBaseStats().MoveSpeed;
    AddMovementInput(ToOpponent, 1.f);
}

void AAIBoxer::AIMoveAwayFromPlayer(float DeltaTime)
{
    if (!OpponentRef || !bBoxerEnabled) return;

    FVector AwayFromOpponent = (GetActorLocation()
        - OpponentRef->GetActorLocation()).GetSafeNormal();
    AwayFromOpponent.Z = 0.f;

    AddMovementInput(AwayFromOpponent, 1.f);
}

float AAIBoxer::GetDistanceToOpponent() const
{
    if (!OpponentRef) return TNumericLimits<float>::Max();
    return FVector::Dist(GetActorLocation(), OpponentRef->GetActorLocation());
}

bool AAIBoxer::IsOpponentAttacking() const
{
    if (!OpponentRef) return false;
    if (OpponentRef->Implements<UBoxerInterface>())
    {
        return IBoxerInterface::Execute_IsAttacking(OpponentRef);
    }
    return false;
}

bool AAIBoxer::IsInAttackRange() const
{
    return GetDistanceToOpponent() <= PreferredRange + 30.f;
}
