#include "Game/LastBellGameMode.h"
#include "Characters/PlayerBoxer.h"
#include "Characters/AIBoxer.h"
#include "Components/BoxerStatsComponent.h"
#include "AI/BoxingAIController.h"
#include "Data/FighterDataAsset.h"
#include "Data/LastBellGameSettings.h"
#include "Game/LastBellGameState.h"
#include "Game/LastBellPlayerController.h"
#include "Core/IBoxerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ALastBellGameMode::ALastBellGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    GameStateClass = ALastBellGameState::StaticClass();
    PlayerControllerClass = ALastBellPlayerController::StaticClass();
    DefaultPawnClass = APlayerBoxer::StaticClass();
}

void ALastBellGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Load settings
    if (GameSettings)
    {
        TotalRounds = GameSettings->TotalRounds;
        RoundDuration = GameSettings->RoundDuration;
        BetweenRoundDuration = GameSettings->BetweenRoundDuration;
        KnockdownCountInterval = GameSettings->KnockdownCountInterval;
        MaxKnockdownCount = GameSettings->KnockdownCountDuration;
    }

    // Find fighters in the world
    for (TActorIterator<APlayerBoxer> It(GetWorld()); It; ++It)
    {
        PlayerFighter = *It;
        break;
    }
    for (TActorIterator<AAIBoxer> It(GetWorld()); It; ++It)
    {
        OpponentFighter = *It;
        break;
    }

    // Set opponents
    if (PlayerFighter && OpponentFighter)
    {
        IBoxerInterface::Execute_SetOpponent(PlayerFighter, OpponentFighter);
        IBoxerInterface::Execute_SetOpponent(OpponentFighter, PlayerFighter);
    }

    SetMatchState(EMatchState::PreMatch);
}

void ALastBellGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bTimerRunning && CurrentMatchState == EMatchState::RoundActive)
    {
        TickRoundTimer(DeltaTime);
    }
}

void ALastBellGameMode::StartMatch(UFighterDataAsset* OpponentData)
{
    if (OpponentData && OpponentFighter)
    {
        OpponentFighter->LoadFighterData(OpponentData);
    }

    CurrentRound = 0;
    PlayerRoundsWon = 0;
    OpponentRoundsWon = 0;

    SetMatchState(EMatchState::PreMatch);

    // Brief delay then start round 1
    GetWorldTimerManager().SetTimer(RoundStartHandle, this,
        &ALastBellGameMode::StartRound, 2.f, false);
}

void ALastBellGameMode::StartRound()
{
    CurrentRound++;
    RoundTimeRemaining = RoundDuration;
    KnockdownCount = 0;
    KnockedDownFighter = nullptr;

    // Reset fighter health
    if (PlayerFighter) PlayerFighter->StatsComponent->ResetStats();
    if (OpponentFighter) OpponentFighter->StatsComponent->ResetStats();

    SetFightersEnabled(true);
    SetMatchState(EMatchState::RoundActive);
    bTimerRunning = true;
    OnRoundChanged.Broadcast(CurrentRound);

    // Start AI
    if (OpponentFighter)
    {
        if (ABoxingAIController* AIC =
            Cast<ABoxingAIController>(OpponentFighter->GetController()))
        {
            AIC->StartAI(PlayerFighter);
        }
    }
}

void ALastBellGameMode::EndRound(bool bPlayerWonRound, bool bKnockout)
{
    bTimerRunning = false;
    SetFightersEnabled(false);

    // Stop AI
    if (OpponentFighter)
    {
        if (ABoxingAIController* AIC =
            Cast<ABoxingAIController>(OpponentFighter->GetController()))
        {
            AIC->StopAI();
        }
    }

    FRoundResult Result;
    Result.RoundNumber = CurrentRound;
    Result.bPlayerWon = bPlayerWonRound;
    Result.bKnockout = bKnockout;
    if (PlayerFighter)
        Result.PlayerHealthRemaining = PlayerFighter->StatsComponent->GetHealth();
    if (OpponentFighter)
        Result.OpponentHealthRemaining = OpponentFighter->StatsComponent->GetHealth();

    if (bPlayerWonRound) PlayerRoundsWon++;
    else OpponentRoundsWon++;

    OnRoundEnd.Broadcast(Result);

    int32 RoundsNeeded = (TotalRounds / 2) + 1;

    if (PlayerRoundsWon >= RoundsNeeded || OpponentRoundsWon >= RoundsNeeded
        || CurrentRound >= TotalRounds)
    {
        SetMatchState(EMatchState::MatchEnd);
        GetWorldTimerManager().SetTimer(PostMatchHandle, [this]()
        {
            FinalizeMatch();
        }, 2.f, false);
    }
    else
    {
        SetMatchState(EMatchState::BetweenRounds);
        GetWorldTimerManager().SetTimer(BetweenRoundsHandle,
            this, &ALastBellGameMode::StartRound,
            BetweenRoundDuration, false);
    }
}

void ALastBellGameMode::SetMatchState(EMatchState NewState)
{
    CurrentMatchState = NewState;
    OnMatchStateChanged.Broadcast(NewState);

    if (ALastBellGameState* GS = GetGameState<ALastBellGameState>())
    {
        GS->SetMatchState(NewState);
    }
}

void ALastBellGameMode::TriggerKnockdown(ABoxerCharacter* KnockedBoxer)
{
    if (!KnockedBoxer) return;

    KnockedDownFighter = KnockedBoxer;
    KnockdownCount = 0;
    bTimerRunning = false;
    SetMatchState(EMatchState::KnockDown);
    KnockedBoxer->TriggerKnockdown();

    // Start count
    GetWorldTimerManager().SetTimer(
        KnockdownCountHandle,
        this, &ALastBellGameMode::AdvanceKnockdownCount,
        KnockdownCountInterval, true);
}

void ALastBellGameMode::AdvanceKnockdownCount()
{
    KnockdownCount++;
    OnKnockdownCount.Broadcast(KnockdownCount);

    bool bFighterIsPlayer = (KnockedDownFighter == PlayerFighter);

    if (KnockdownCount >= MaxKnockdownCount)
    {
        GetWorldTimerManager().ClearTimer(KnockdownCountHandle);
        // TKO
        EndRound(!bFighterIsPlayer, true);
    }
}

void ALastBellGameMode::HandleRoundTimeout()
{
    // Judge by health remaining
    float PlayerHP = PlayerFighter ? PlayerFighter->StatsComponent->GetHealthPercent() : 0.f;
    float OpponentHP = OpponentFighter ? OpponentFighter->StatsComponent->GetHealthPercent() : 0.f;
    EndRound(PlayerHP >= OpponentHP, false);
}

void ALastBellGameMode::FinalizeMatch()
{
    bool bPlayerWon = PlayerRoundsWon > OpponentRoundsWon;
    SetMatchState(bPlayerWon ? EMatchState::MatchWin : EMatchState::MatchLose);
    OnMatchEnd.Broadcast(bPlayerWon);

    // Load appropriate end screen
    FString LevelName = bPlayerWon ?
        TEXT("/Game/LastBell/Maps/L_WinScreen") :
        TEXT("/Game/LastBell/Maps/L_LoseScreen");
    UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void ALastBellGameMode::SetFightersEnabled(bool bEnabled)
{
    if (PlayerFighter) PlayerFighter->SetBoxerEnabled(bEnabled);
    if (OpponentFighter) OpponentFighter->SetBoxerEnabled(bEnabled);
}

void ALastBellGameMode::TickRoundTimer(float DeltaTime)
{
    RoundTimeRemaining -= DeltaTime;
    OnTimerTick.Broadcast(FMath::Max(0.f, RoundTimeRemaining));

    if (RoundTimeRemaining <= 0.f)
    {
        bTimerRunning = false;
        HandleRoundTimeout();
    }
}
