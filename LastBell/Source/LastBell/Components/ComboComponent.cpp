#include "Components/ComboComponent.h"
#include "Engine/World.h"

UComboComponent::UComboComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UComboComponent::RegisterHit(EBoxingMove Move)
{
    UWorld* World = GetWorld();
    float Now = World ? World->GetTimeSeconds() : 0.f;

    // If the window expired, reset before incrementing
    if (CurrentCombo > 0 && (Now - LastHitTime) > ComboWindowDuration)
    {
        BreakCombo();
    }

    LastHitTime = Now;
    CurrentCombo++;
    BestComboThisMatch = FMath::Max(BestComboThisMatch, CurrentCombo);
    OnComboUpdated.Broadcast(CurrentCombo, BestComboThisMatch);
}

void UComboComponent::BreakCombo()
{
    if (CurrentCombo == 0) return;
    CurrentCombo = 0;
    OnComboReset.Broadcast();
}

void UComboComponent::ResetForNewMatch()
{
    CurrentCombo = 0;
    BestComboThisMatch = 0;
    LastHitTime = -99.f;
}
