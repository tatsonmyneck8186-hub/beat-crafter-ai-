#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Core/BoxingTypes.h"
#include "BoxingAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;
class AAIBoxer;

// Blackboard key names
namespace BBKeys
{
    static const FName Player          = TEXT("PlayerActor");
    static const FName Self            = TEXT("SelfActor");
    static const FName DistToPlayer    = TEXT("DistanceToPlayer");
    static const FName bCanAttack      = TEXT("bCanAttack");
    static const FName bPlayerAttacking= TEXT("bPlayerAttacking");
    static const FName AttackMove      = TEXT("AttackMove");
    static const FName HealthPercent   = TEXT("HealthPercent");
    static const FName bShouldRetreat  = TEXT("bShouldRetreat");
}

UCLASS()
class LASTBELL_API ABoxingAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABoxingAIController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    // ─── Behavior Tree ─────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UFUNCTION(BlueprintCallable, Category = "AI")
    void StartAI(AActor* PlayerTarget);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void StopAI();

    UFUNCTION(BlueprintCallable, Category = "AI")
    void UpdateBlackboard();

private:
    UPROPERTY()
    TObjectPtr<AAIBoxer> ControlledBoxer;

    FTimerHandle BlackboardUpdateHandle;
};
