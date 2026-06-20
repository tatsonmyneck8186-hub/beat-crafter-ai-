#include "Game/LastBellGameInstance.h"
#include "Kismet/GameplayStatics.h"

ULastBellGameInstance::ULastBellGameInstance()
{
}

void ULastBellGameInstance::SetSelectedOpponent(UFighterDataAsset* Opponent)
{
    SelectedOpponent = Opponent;
}

void ULastBellGameInstance::StartFight()
{
    UGameplayStatics::OpenLevel(this,
        FName(TEXT("/Game/LastBell/Maps/L_FightArena")));
}

void ULastBellGameInstance::RecordMatchResult(bool bWon,
    const TArray<FRoundResult>& Results)
{
    bLastMatchWon = bWon;
    LastMatchResults = Results;

    if (bWon) TotalWins++;
    else TotalLosses++;
}
