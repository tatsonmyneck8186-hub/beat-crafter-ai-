#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/BoxingTypes.h"
#include "LastBellGameState.generated.h"

UCLASS()
class LASTBELL_API ALastBellGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ALastBellGameState();

    UFUNCTION(BlueprintCallable, Category = "Match")
    void SetMatchState(EMatchState NewState);

    UFUNCTION(BlueprintPure, Category = "Match")
    EMatchState GetMatchState() const { return CurrentMatchState; }

    UPROPERTY(BlueprintReadOnly, Category = "Match")
    EMatchState CurrentMatchState = EMatchState::PreMatch;

    UPROPERTY(BlueprintReadOnly, Category = "Match")
    TArray<FRoundResult> RoundHistory;

    UFUNCTION(BlueprintCallable, Category = "Match")
    void AddRoundResult(const FRoundResult& Result);
};
