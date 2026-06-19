#include "AI/BTService_UpdateCombatState.h"
#include "Characters/AIBoxer.h"
#include "Components/BoxerStatsComponent.h"
#include "Core/IBoxerInterface.h"
#include "AI/BoxingAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_UpdateCombatState::UBTService_UpdateCombatState()
{
    NodeName = TEXT("Update Combat State");
    Interval = 0.1f;
    RandomDeviation = 0.02f;
}

void UBTService_UpdateCombatState::TickNode(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller) return;

    AAIBoxer* Boxer = Cast<AAIBoxer>(Controller->GetPawn());
    if (!Boxer) return;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    AActor* Player = Cast<AActor>(BB->GetValueAsObject(BBKeys::Player));
    if (!Player) return;

    // Distance
    float Dist = Boxer->GetDistanceToOpponent();
    BB->SetValueAsFloat(BBKeys::DistToPlayer, Dist);

    // Is player attacking?
    bool bPlayerAtk = false;
    if (Player->Implements<UBoxerInterface>())
    {
        bPlayerAtk = IBoxerInterface::Execute_IsAttacking(Player);
    }
    BB->SetValueAsBool(BBKeys::bPlayerAttacking, bPlayerAtk);

    // Health
    float HP = Boxer->StatsComponent->GetHealthPercent();
    BB->SetValueAsFloat(BBKeys::HealthPercent, HP);

    // Can attack
    bool bCanAct = Boxer->GetCurrentState() != EBoxerState::Attacking
        && Boxer->GetCurrentState() != EBoxerState::HitStun
        && Boxer->GetCurrentState() != EBoxerState::KnockedDown
        && Boxer->GetCurrentState() != EBoxerState::KO;
    BB->SetValueAsBool(BBKeys::bCanAttack, bCanAct);

    BB->SetValueAsBool(BBKeys::bShouldRetreat, HP < 0.2f);
}
