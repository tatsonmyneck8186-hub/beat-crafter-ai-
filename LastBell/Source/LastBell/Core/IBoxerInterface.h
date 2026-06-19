#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/BoxingTypes.h"
#include "IBoxerInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UBoxerInterface : public UInterface
{
    GENERATED_BODY()
};

class LASTBELL_API IBoxerInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Combat")
    void ExecuteAttack(EBoxingMove Move);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Combat")
    void ReceiveHit(const FAttackData& AttackData, AActor* Attacker);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Combat")
    void ExecuteDodge();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Combat")
    void ExecuteDuck();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Combat")
    void StopDuck();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    EBoxerState GetBoxerState() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    float GetKOMeterPercent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    bool IsAttacking() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|State")
    bool IsDucking() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Setup")
    void SetOpponent(AActor* Opponent);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boxing|Setup")
    AActor* GetOpponent() const;
};
