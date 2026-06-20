#include "AI/BTTask_MoveTowardPlayer.h"
#include "Characters/AIBoxer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BoxingAIController.h"

UBTTask_MoveTowardPlayer::UBTTask_MoveTowardPlayer()
{
    NodeName = TEXT("Move Toward/Away Player");
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveTowardPlayer::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    AAIBoxer* Boxer = Cast<AAIBoxer>(Controller->GetPawn());
    if (!Boxer) return EBTNodeResult::Failed;

    if (bMoveAway)
    {
        Boxer->AIMoveAwayFromPlayer(MoveDuration);
    }
    else
    {
        Boxer->AIMoveTowardPlayer(MoveDuration);
    }

    return EBTNodeResult::Succeeded;
}
