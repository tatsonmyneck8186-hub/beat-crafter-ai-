#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/BoxingTypes.h"
#include "LastBellGameInstance.generated.h"

class UFighterDataAsset;

UCLASS()
class LASTBELL_API ULastBellGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    ULastBellGameInstance();

    // ─── Session State (persists between levels) ─────────────────────────────────

    UPROPERTY(BlueprintReadWrite, Category = "Session")
    TObjectPtr<UFighterDataAsset> SelectedOpponent;

    UPROPERTY(BlueprintReadWrite, Category = "Session")
    int32 SelectedPlayerSkin = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Session")
    TArray<FRoundResult> LastMatchResults;

    UPROPERTY(BlueprintReadWrite, Category = "Session")
    bool bLastMatchWon = false;

    UFUNCTION(BlueprintCallable, Category = "Session")
    void SetSelectedOpponent(UFighterDataAsset* Opponent);

    UFUNCTION(BlueprintPure, Category = "Session")
    UFighterDataAsset* GetSelectedOpponent() const { return SelectedOpponent; }

    UFUNCTION(BlueprintCallable, Category = "Session")
    void StartFight();

    UFUNCTION(BlueprintCallable, Category = "Session")
    void RecordMatchResult(bool bWon, const TArray<FRoundResult>& Results);

    // ─── Statistics ───────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 TotalWins = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 TotalLosses = 0;
};
