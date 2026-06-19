#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LastBellPlayerController.generated.h"

UCLASS()
class LASTBELL_API ALastBellPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ALastBellPlayerController();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowHUD(bool bShow);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowWinScreen();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowLoseScreen();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void ReturnToMainMenu();

    // Called from Game Mode when match state changes
    UFUNCTION(BlueprintImplementableEvent, Category = "Match")
    void OnMatchStateChanged(EMatchState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "Match")
    void OnKnockdownCountChanged(int32 Count);
};
