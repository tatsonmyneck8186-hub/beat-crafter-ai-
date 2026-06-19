#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Core/BoxingTypes.h"
#include "BTTask_ExecuteAttack.generated.h"

UCLASS()
class LASTBELL_API UBTTask_ExecuteAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_ExecuteAttack();

    virtual EBTNodeResult::Type ExecuteTask(
        UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Attack")
    EBoxingMove AttackMove = EBoxingMove::Jab;

    // If true, pick randomly from all moves
    UPROPERTY(EditAnywhere, Category = "Attack")
    bool bPickRandomMove = false;

    // Weight distribution for random picks [Jab, Hook, Uppercut]
    UPROPERTY(EditAnywhere, Category = "Attack", meta = (EditCondition = "bPickRandomMove"))
    FVector MoveWeights = FVector(0.5f, 0.3f, 0.2f);
};
