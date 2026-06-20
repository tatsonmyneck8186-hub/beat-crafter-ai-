#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BoxingTypes.h"
#include "ComboComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboUpdated, int32, CurrentCombo, int32, BestCombo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboReset);

// Tracks consecutive hits within a window. Drives combo display on HUD.
UCLASS(ClassGroup=(Boxing), meta=(BlueprintSpawnableComponent))
class LASTBELL_API UComboComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UComboComponent();

public:
    UPROPERTY(EditDefaultsOnly, Category="Combo")
    float ComboWindowDuration = 2.2f; // Reset if no hit lands within this time

    UPROPERTY(BlueprintReadOnly, Category="Combo")
    int32 CurrentCombo = 0;

    UPROPERTY(BlueprintReadOnly, Category="Combo")
    int32 BestComboThisMatch = 0;

    UFUNCTION(BlueprintCallable, Category="Combo")
    void RegisterHit(EBoxingMove Move);

    UFUNCTION(BlueprintCallable, Category="Combo")
    void BreakCombo(); // Call when player takes a hit

    UFUNCTION(BlueprintCallable, Category="Combo")
    void ResetForNewMatch();

    UPROPERTY(BlueprintAssignable, Category="Combo")
    FOnComboUpdated OnComboUpdated;

    UPROPERTY(BlueprintAssignable, Category="Combo")
    FOnComboReset OnComboReset;

private:
    float LastHitTime = -99.f;

    void CheckWindowExpiry();
};
