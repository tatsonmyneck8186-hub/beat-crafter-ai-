#include "UI/LastBellHUD.h"
#include "Blueprint/UserWidget.h"

ALastBellHUD::ALastBellHUD()
{
}

void ALastBellHUD::BeginPlay()
{
    Super::BeginPlay();
}

void ALastBellHUD::ShowGameHUD()
{
    if (!HUDWidgetClass) return;

    if (!ActiveHUDWidget)
    {
        ActiveHUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
    }
    if (ActiveHUDWidget)
    {
        ActiveHUDWidget->AddToViewport();
    }
}

void ALastBellHUD::HideGameHUD()
{
    if (ActiveHUDWidget)
    {
        ActiveHUDWidget->RemoveFromParent();
    }
}

void ALastBellHUD::ShowKnockdownCounter(int32 Count)
{
    if (!KnockdownCountWidgetClass) return;

    if (!KnockdownWidget)
    {
        KnockdownWidget = CreateWidget<UUserWidget>(
            GetOwningPlayerController(), KnockdownCountWidgetClass);
        KnockdownWidget->AddToViewport(10);
    }
}

void ALastBellHUD::HideKnockdownCounter()
{
    if (KnockdownWidget)
    {
        KnockdownWidget->RemoveFromParent();
        KnockdownWidget = nullptr;
    }
}
