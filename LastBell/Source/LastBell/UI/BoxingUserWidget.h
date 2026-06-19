#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/BoxingTypes.h"
#include "BoxingUserWidget.generated.h"

class ALastBellGameMode;
class ULastBellGameInstance;

// Base class for all LAST BELL Blueprint UI widgets.
// Provides convenient access to game systems.
UCLASS(Abstract)
class LASTBELL_API UBoxingUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // ─── Helpers ───────────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "LastBell")
    ALastBellGameMode* GetLastBellGameMode() const;

    UFUNCTION(BlueprintPure, Category = "LastBell")
    ULastBellGameInstance* GetLastBellGameInstance() const;

    // ─── Navigation ──────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void GoToMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void GoToCharacterSelect();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void StartFight();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void QuitGame();
};
