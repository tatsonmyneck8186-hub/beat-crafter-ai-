#include "UI/BoxingUserWidget.h"
#include "Game/LastBellGameMode.h"
#include "Game/LastBellGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ALastBellGameMode* UBoxingUserWidget::GetLastBellGameMode() const
{
    return Cast<ALastBellGameMode>(UGameplayStatics::GetGameMode(this));
}

ULastBellGameInstance* UBoxingUserWidget::GetLastBellGameInstance() const
{
    return Cast<ULastBellGameInstance>(GetGameInstance());
}

void UBoxingUserWidget::GoToMainMenu()
{
    UGameplayStatics::OpenLevel(this,
        FName(TEXT("/Game/LastBell/Maps/L_MainMenu")));
}

void UBoxingUserWidget::GoToCharacterSelect()
{
    UGameplayStatics::OpenLevel(this,
        FName(TEXT("/Game/LastBell/Maps/L_CharacterSelect")));
}

void UBoxingUserWidget::StartFight()
{
    if (ULastBellGameInstance* GI = GetLastBellGameInstance())
    {
        GI->StartFight();
    }
}

void UBoxingUserWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr,
        EQuitPreference::Quit, false);
}
