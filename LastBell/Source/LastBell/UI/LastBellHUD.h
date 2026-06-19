#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/BoxingTypes.h"
#include "LastBellHUD.generated.h"

class UBoxingHUDWidget;
class UUserWidget;
class ABoxerCharacter;

UCLASS()
class LASTBELL_API ALastBellHUD : public AHUD
{
    GENERATED_BODY()

public:
    ALastBellHUD();

protected:
    virtual void BeginPlay() override;

public:
    // ─── Widget Classes (set in BP_HUD subclass) ───────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> CharSelectWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> WinScreenWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> LoseScreenWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<UUserWidget> KnockdownCountWidgetClass;

    // ─── Active Widgets ───────────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UUserWidget> ActiveHUDWidget;

    // ─── HUD Controls ─────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideGameHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowKnockdownCounter(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideKnockdownCounter();

    // ─── HUD Update ──────────────────────────────────────────────────────────

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdatePlayerHealthBar(float HealthPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdatePlayerStaminaBar(float StaminaPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdatePlayerKOMeter(float KOPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdateOpponentHealthBar(float HealthPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdateOpponentStaminaBar(float StaminaPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdateOpponentKOMeter(float KOPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdateRoundNumber(int32 Round);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void UpdateRoundTimer(float TimeRemaining);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Update")
    void SetOpponentName(const FText& Name);

private:
    UPROPERTY()
    TObjectPtr<UUserWidget> KnockdownWidget;
};
