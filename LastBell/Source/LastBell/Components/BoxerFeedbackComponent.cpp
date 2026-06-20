#include "Components/BoxerFeedbackComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformTime.h"
#include "GameFramework/PlayerController.h"

UBoxerFeedbackComponent::UBoxerFeedbackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

void UBoxerFeedbackComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBoxerFeedbackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TickFOV(DeltaTime);
    if (bInSlowMo) TickSlowMo();

    // Clear the per-frame FOV request so next frame returns to base
    // unless SetFOVOffset is called again
    if (!bFOVOffsetRequested)
    {
        DesiredFOVOffset = 0.f;
    }
    bFOVOffsetRequested = false;
}

void UBoxerFeedbackComponent::SetCamera(UCameraComponent* InCamera)
{
    TargetCamera = InCamera;
    if (TargetCamera)
    {
        BaseFOV = TargetCamera->FieldOfView;
    }
}

void UBoxerFeedbackComponent::SetFOVOffset(float Delta)
{
    DesiredFOVOffset = Delta;
    bFOVOffsetRequested = true;
}

void UBoxerFeedbackComponent::ClearFOVOffset()
{
    DesiredFOVOffset = 0.f;
    bFOVOffsetRequested = false;
}

void UBoxerFeedbackComponent::TriggerScreenFlash(float Intensity, float Duration)
{
    OnScreenFlash.Broadcast(Intensity, Duration);
}

void UBoxerFeedbackComponent::TriggerKOSlowMotion(float TargetDilation,
    float RampInDuration, float HoldDuration, float RampOutDuration)
{
    bInSlowMo = true;
    SlowMoStartRealTime  = FPlatformTime::Seconds();
    SlowMoTargetDilation = FMath::Clamp(TargetDilation, 0.02f, 1.f);
    SlowMoRampInDuration  = RampInDuration;
    SlowMoHoldDuration    = HoldDuration;
    SlowMoRampOutDuration = RampOutDuration;
    SlowMoTotalDuration   = RampInDuration + HoldDuration + RampOutDuration;
}

void UBoxerFeedbackComponent::TriggerRumble(float Intensity, float Duration)
{
    UWorld* World = GetWorld();
    if (!World) return;
    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return;
    PC->PlayDynamicForceFeedback(Intensity, Duration, true, true, true, true);
}

void UBoxerFeedbackComponent::TickFOV(float DeltaTime)
{
    if (!TargetCamera) return;

    float TargetFOV = BaseFOV + DesiredFOVOffset;
    float NewFOV = FMath::FInterpTo(TargetCamera->FieldOfView,
        TargetFOV, DeltaTime, FOVLerpSpeed);
    TargetCamera->SetFieldOfView(NewFOV);
    CurrentFOVOffset = NewFOV - BaseFOV;
}

void UBoxerFeedbackComponent::TickSlowMo()
{
    UWorld* World = GetWorld();
    if (!World) return;

    double ElapsedReal = FPlatformTime::Seconds() - SlowMoStartRealTime;

    if (ElapsedReal >= SlowMoTotalDuration)
    {
        // Done — restore fully
        UGameplayStatics::SetGlobalTimeDilation(World, 1.f);
        bInSlowMo = false;
        return;
    }

    float Dilation;
    if (ElapsedReal < SlowMoRampInDuration)
    {
        // Ramp in: 1.0 → target
        float T = static_cast<float>(ElapsedReal) / SlowMoRampInDuration;
        Dilation = FMath::Lerp(1.f, SlowMoTargetDilation, T);
    }
    else if (ElapsedReal < SlowMoRampInDuration + SlowMoHoldDuration)
    {
        Dilation = SlowMoTargetDilation;
    }
    else
    {
        // Ramp out: target → 1.0
        float T = static_cast<float>(ElapsedReal - SlowMoRampInDuration - SlowMoHoldDuration)
            / SlowMoRampOutDuration;
        Dilation = FMath::Lerp(SlowMoTargetDilation, 1.f, T);
    }

    UGameplayStatics::SetGlobalTimeDilation(World,
        FMath::Clamp(Dilation, 0.02f, 1.f));
}
