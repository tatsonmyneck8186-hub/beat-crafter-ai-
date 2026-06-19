#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BoxingTypes.h"
#include "BoxerFeedbackComponent.generated.h"

class UCameraComponent;

// Broadcasts to Blueprint for screen-space effects that can't be done in C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenFlash, float, Intensity, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboCount, int32, Count);

// Drives all player-perceivable feedback: FOV animation, screen flash, KO slow-motion.
// Attach to the player boxer. AI boxer ignores FOV/flash but KO slow-mo is global.
UCLASS(ClassGroup=(Boxing), meta=(BlueprintSpawnableComponent))
class LASTBELL_API UBoxerFeedbackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBoxerFeedbackComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // ─── Setup ────────────────────────────────────────────────────────────────

    // Call from BeginPlay once camera is known
    void SetCamera(UCameraComponent* InCamera);

    // ─── FOV ───────────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category="Feedback|FOV")
    float BaseFOV = 75.f;

    UPROPERTY(EditDefaultsOnly, Category="Feedback|FOV")
    float FOVLerpSpeed = 14.f;

    // Set FOV offset from base (call each frame you want a different FOV)
    void SetFOVOffset(float Delta);

    // Returns to BaseFOV
    void ClearFOVOffset();

    // ─── Screen Flash ──────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category="Feedback|Flash")
    void TriggerScreenFlash(float Intensity, float Duration);

    UPROPERTY(BlueprintAssignable, Category="Feedback|Flash")
    FOnScreenFlash OnScreenFlash;

    // ─── KO Slow-Motion ─────────────────────────────────────────────────────────

    // Smoothly ramp to target dilation, hold, then restore.
    UFUNCTION(BlueprintCallable, Category="Feedback|SlowMo")
    void TriggerKOSlowMotion(float TargetDilation = 0.1f,
        float RampInDuration = 0.25f,
        float HoldDuration = 0.6f,
        float RampOutDuration = 0.4f);

    UFUNCTION(BlueprintPure, Category="Feedback|SlowMo")
    bool IsInSlowMotion() const { return bInSlowMo; }

    // ─── Controller Vibration ───────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category="Feedback|Rumble")
    void TriggerRumble(float Intensity, float Duration);

private:
    UPROPERTY()
    TObjectPtr<UCameraComponent> TargetCamera;

    // FOV state
    float CurrentFOVOffset = 0.f;
    float DesiredFOVOffset = 0.f;
    bool bFOVOffsetRequested = false;

    // Slow-mo state (tracked in real time, not game time)
    bool bInSlowMo = false;
    double SlowMoStartRealTime = 0.0;
    float SlowMoTargetDilation  = 0.1f;
    float SlowMoRampInDuration  = 0.25f;
    float SlowMoHoldDuration    = 0.6f;
    float SlowMoRampOutDuration = 0.4f;
    float SlowMoTotalDuration   = 1.25f;

    void TickFOV(float DeltaTime);
    void TickSlowMo();
};
