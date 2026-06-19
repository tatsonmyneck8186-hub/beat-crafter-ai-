#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteDodge.generated.h"

UCLASS()
class LASTBELL_API UBTTask_ExecuteDodge : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_ExecuteDodge();

    virtual EBTNodeResult::Type ExecuteTask(
        UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Dodge")
    float DodgeProbability = 0.7f;
};
