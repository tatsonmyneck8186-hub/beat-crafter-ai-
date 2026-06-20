#pragma once

#include "CoreMinimal.h"
#include "Characters/BoxerCharacter.h"
#include "AIBoxer.generated.h"

UCLASS()
class LASTBELL_API AAIBoxer : public ABoxerCharacter
{
    GENERATED_BODY()

public:
    AAIBoxer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ─── AI Access ────────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AI|Boxing")
    void AIExecuteAttack(EBoxingMove Move);

    UFUNCTION(BlueprintCallable, Category = "AI|Boxing")
    void AIExecuteDodge();

    UFUNCTION(BlueprintCallable, Category = "AI|Boxing")
    void AIMoveTowardPlayer(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "AI|Boxing")
    void AIMoveAwayFromPlayer(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "AI|Boxing")
    float GetDistanceToOpponent() const;

    UFUNCTION(BlueprintPure, Category = "AI|Boxing")
    bool IsOpponentAttacking() const;

    UFUNCTION(BlueprintPure, Category = "AI|Boxing")
    bool IsInAttackRange() const;

    // Preferred engagement range from FighterData personality
    UPROPERTY(BlueprintReadOnly, Category = "AI")
    float PreferredRange = 140.f;
};
