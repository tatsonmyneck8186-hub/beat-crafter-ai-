#include "Game/LastBellPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ALastBellPlayerController::ALastBellPlayerController()
{
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
}

void ALastBellPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(FInputModeGameOnly());
}

void ALastBellPlayerController::ShowHUD(bool bShow)
{
    if (GetHUD())
    {
        GetHUD()->SetActorHiddenInGame(!bShow);
    }
}

void ALastBellPlayerController::ShowWinScreen()
{
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}

void ALastBellPlayerController::ShowLoseScreen()
{
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}

void ALastBellPlayerController::ReturnToMainMenu()
{
    UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/LastBell/Maps/L_MainMenu")));
}
