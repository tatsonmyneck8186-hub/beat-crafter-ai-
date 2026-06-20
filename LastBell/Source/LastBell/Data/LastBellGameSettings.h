#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LastBellGameSettings.generated.h"

class UFighterDataAsset;

UCLASS(BlueprintType, Blueprintable)
class LASTBELL_API ULastBellGameSettings : public UDataAsset
{
    GENERATED_BODY()

public:
    // ─── Match Settings ───────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    int32 TotalRounds = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    float RoundDuration = 120.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    float BetweenRoundDuration = 5.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    float RoundStartDelay = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    float KnockdownCountDuration = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match")
    float KnockdownCountInterval = 1.f;

    // ─── Ring Bounds ──────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ring")
    float RingHalfWidth = 280.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ring")
    float MinFighterDistance = 80.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ring")
    float MaxFighterDistance = 320.f;

    // ─── Effects ─────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    float HitPauseTimeDilationScale = 0.05f;

    // ─── Audio ────────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> BellSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> CrowdCheerSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> CrowdBoooSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> KOAnnounceSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
    TObjectPtr<USoundBase> BackgroundMusic;

    // ─── Particle Effects ─────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UParticleSystem> HitSparksEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UParticleSystem> HardHitEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UParticleSystem> KOEffect;

    // ─── Fighter References ───────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighters")
    TArray<TObjectPtr<UFighterDataAsset>> AvailableOpponents;
};
