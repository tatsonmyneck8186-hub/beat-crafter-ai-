#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/BoxingTypes.h"
#include "LastBellGameMode.generated.h"

class ABoxerCharacter;
class AAIBoxer;
class APlayerBoxer;
class UFighterDataAsset;
class ULastBellGameSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, EMatchState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundChanged, int32, RoundNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerTick, float, RemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKnockdownCount, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEnd, FRoundResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchEnd, bool, bPlayerWon);

UCLASS()
class LASTBELL_API ALastBellGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALastBellGameMode();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ─── Settings ─────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
    TObjectPtr<ULastBellGameSettings> GameSettings;

    // ─── Match Control ──────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Match")
    void StartMatch(UFighterDataAsset* OpponentData);

    UFUNCTION(BlueprintCallable, Category = "Match")
    void StartRound();

    UFUNCTION(BlueprintCallable, Category = "Match")
    void EndRound(bool bPlayerWonRound, bool bKnockout);

    UFUNCTION(BlueprintCallable, Category = "Match")
    void SetMatchState(EMatchState NewState);

    UFUNCTION(BlueprintPure, Category = "Match")
    EMatchState GetMatchState() const { return CurrentMatchState; }

    UFUNCTION(BlueprintPure, Category = "Match")
    int32 GetCurrentRound() const { return CurrentRound; }

    UFUNCTION(BlueprintPure, Category = "Match")
    float GetRoundTimeRemaining() const { return RoundTimeRemaining; }

    UFUNCTION(BlueprintPure, Category = "Match")
    int32 GetPlayerRoundsWon() const { return PlayerRoundsWon; }

    UFUNCTION(BlueprintPure, Category = "Match")
    int32 GetOpponentRoundsWon() const { return OpponentRoundsWon; }

    // ─── Knockdown ─────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Match")
    void TriggerKnockdown(ABoxerCharacter* KnockedBoxer);

    // ─── Fighter References ────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Match")
    TObjectPtr<APlayerBoxer> PlayerFighter;

    UPROPERTY(BlueprintReadOnly, Category = "Match")
    TObjectPtr<AAIBoxer> OpponentFighter;

    // ─── Events ────────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnMatchStateChanged OnMatchStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnRoundChanged OnRoundChanged;

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnTimerTick OnTimerTick;

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnKnockdownCount OnKnockdownCount;

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnRoundEnd OnRoundEnd;

    UPROPERTY(BlueprintAssignable, Category = "Match|Events")
    FOnMatchEnd OnMatchEnd;

private:
    EMatchState CurrentMatchState = EMatchState::PreMatch;
    int32 CurrentRound = 0;
    float RoundTimeRemaining = 120.f;
    int32 PlayerRoundsWon = 0;
    int32 OpponentRoundsWon = 0;

    int32 KnockdownCount = 0;
    ABoxerCharacter* KnockedDownFighter = nullptr;

    bool bTimerRunning = false;

    // Default values used if GameSettings asset is not assigned
    int32 TotalRounds = 3;
    float RoundDuration = 120.f;
    float BetweenRoundDuration = 5.f;
    float KnockdownCountInterval = 1.f;
    float MaxKnockdownCount = 10.f;

    void TickRoundTimer(float DeltaTime);
    void AdvanceKnockdownCount();
    void HandleRoundTimeout();
    void FinalizeMatch();
    void SetFightersEnabled(bool bEnabled);

    FTimerHandle RoundStartHandle;
    FTimerHandle BetweenRoundsHandle;
    FTimerHandle KnockdownCountHandle;
    FTimerHandle PostMatchHandle;
};
