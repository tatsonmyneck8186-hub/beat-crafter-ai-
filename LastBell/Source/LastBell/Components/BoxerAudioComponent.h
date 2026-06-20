#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BoxingTypes.h"
#include "BoxerAudioComponent.generated.h"

class UAudioComponent;

// Layered audio manager for a single boxer.
// Handles impact sound layering, stamina breathing, coach voice lines.
UCLASS(ClassGroup=(Boxing), meta=(BlueprintSpawnableComponent))
class LASTBELL_API UBoxerAudioComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBoxerAudioComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // ─── Sound Assets (assign in FighterDataAsset or BP) ─────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> JabHeadSound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> JabBodySound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> HookHeadSound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> HookBodySound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> UppercutHeadSound;

    // Extra layer played on every impact (flesh thud)
    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> ImpactFleshLayerSound;

    // Extra layer for heavy hits (bone crack / reverb)
    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> HeavyImpactLayerSound;

    // Plays when a punch misses
    UPROPERTY(EditDefaultsOnly, Category="Audio|Impacts")
    TObjectPtr<USoundBase> SwishMissSound;

    // ─── Breathing ───────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category="Audio|Breathing")
    TObjectPtr<USoundBase> HeavyBreathingLoop;

    // Stamina threshold below which heavy breathing kicks in
    UPROPERTY(EditDefaultsOnly, Category="Audio|Breathing")
    float BreathingStaminaThreshold = 0.35f;

    // ─── Reaction / Misc ─────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category="Audio|Reactions")
    TObjectPtr<USoundBase> HitReactionGruntSound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Reactions")
    TObjectPtr<USoundBase> StaggerSound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Reactions")
    TObjectPtr<USoundBase> VictorySound;

    UPROPERTY(EditDefaultsOnly, Category="Audio|Reactions")
    TObjectPtr<USoundBase> DefeatSound;

    // ─── API ─────────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayPunchImpact(EBoxingMove Move, EHitZone Zone, float DamageFraction);

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayPunchMiss(EBoxingMove Move);

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayHitReaction();

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayStagger();

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayVictory();

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayDefeat();

    // Called from stats component each frame with current stamina percent
    void UpdateBreathing(float StaminaPercent);

private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> BreathingAudioComp;

    float LastImpactSoundTime = -99.f;
    static constexpr float MinImpactInterval = 0.04f;

    void PlaySoundAtOwner(USoundBase* Sound, float VolumeScale = 1.f);
    bool CanPlayImpact() const;
};
