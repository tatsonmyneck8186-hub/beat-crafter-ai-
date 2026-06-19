#include "AI/BoxingAIController.h"
#include "Characters/AIBoxer.h"
#include "Components/BoxerStatsComponent.h"
#include "Core/IBoxerInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ABoxingAIController::ABoxingAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABoxingAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ABoxingAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledBoxer = Cast<AAIBoxer>(InPawn);
}

void ABoxingAIController::StartAI(AActor* PlayerTarget)
{
    if (!BehaviorTree || !Blackboard) return;

    UseBlackboard(BehaviorTree->BlackboardAsset, Blackboard);
    RunBehaviorTree(BehaviorTree);

    Blackboard->SetValueAsObject(BBKeys::Player, PlayerTarget);
    if (ControlledBoxer)
    {
        Blackboard->SetValueAsObject(BBKeys::Self, ControlledBoxer);
    }

    // Update blackboard frequently
    GetWorldTimerManager().SetTimer(
        BlackboardUpdateHandle,
        this, &ABoxingAIController::UpdateBlackboard,
        0.1f, true);
}

void ABoxingAIController::StopAI()
{
    GetWorldTimerManager().ClearTimer(BlackboardUpdateHandle);
    StopMovement();

    if (UBehaviorTreeComponent* BTComp =
        Cast<UBehaviorTreeComponent>(BrainComponent))
    {
        BTComp->StopTree();
    }
}

void ABoxingAIController::UpdateBlackboard()
{
    if (!Blackboard || !ControlledBoxer) return;

    AActor* Player = Cast<AActor>(
        Blackboard->GetValueAsObject(BBKeys::Player));

    if (Player)
    {
        float Dist = FVector::Dist(
            ControlledBoxer->GetActorLocation(),
            Player->GetActorLocation());
        Blackboard->SetValueAsFloat(BBKeys::DistToPlayer, Dist);

        bool bPlayerAttacking = false;
        if (Player->Implements<UBoxerInterface>())
        {
            bPlayerAttacking = IBoxerInterface::Execute_IsAttacking(Player);
        }
        Blackboard->SetValueAsBool(BBKeys::bPlayerAttacking, bPlayerAttacking);

        float HP = ControlledBoxer->StatsComponent->GetHealthPercent();
        Blackboard->SetValueAsFloat(BBKeys::HealthPercent, HP);

        bool bCanAct = !ControlledBoxer->GetCurrentState() ==
            EBoxerState::Attacking &&
            !ControlledBoxer->GetCurrentState() == EBoxerState::HitStun;
        Blackboard->SetValueAsBool(BBKeys::bCanAttack, bCanAct);

        // Retreat when low health
        Blackboard->SetValueAsBool(BBKeys::bShouldRetreat, HP < 0.2f);
    }
}
