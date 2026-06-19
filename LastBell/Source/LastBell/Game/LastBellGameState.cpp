#include "Game/LastBellGameState.h"

ALastBellGameState::ALastBellGameState()
{
}

void ALastBellGameState::SetMatchState(EMatchState NewState)
{
    CurrentMatchState = NewState;
}

void ALastBellGameState::AddRoundResult(const FRoundResult& Result)
{
    RoundHistory.Add(Result);
}
