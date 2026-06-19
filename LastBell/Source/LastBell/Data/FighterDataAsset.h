#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/BoxingTypes.h"
#include "FighterDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LASTBELL_API UFighterDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // ─── Identity ─────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    FText FighterName = FText::FromString("Unknown Fighter");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    FText FighterNickname;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    FText FighterBio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    EFighterType FighterType = EFighterType::Player;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    TObjectPtr<UTexture2D> Portrait;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    FLinearColor PrimaryColor = FLinearColor(0.8f, 0.1f, 0.1f, 1.f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Identity")
    FLinearColor SecondaryColor = FLinearColor(0.1f, 0.1f, 0.8f, 1.f);

    // ─── Stats ────────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Stats")
    FFighterStats Stats;

    // ─── Attacks ──────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Attacks")
    FAttackData JabData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Attacks")
    FAttackData HookData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Attacks")
    FAttackData UppercutData;

    // ─── AI Personality ───────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|AI")
    FAIPersonality AIPersonality;

    // ─── Mesh ─────────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Visuals")
    TObjectPtr<USkeletalMesh> FighterMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Visuals")
    TSubclassOf<UAnimInstance> AnimInstanceClass;

    // ─── Audio ────────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Audio")
    TObjectPtr<USoundBase> JabSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Audio")
    TObjectPtr<USoundBase> HookSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Audio")
    TObjectPtr<USoundBase> UppercutSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Audio")
    TObjectPtr<USoundBase> HitSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter|Audio")
    TObjectPtr<USoundBase> KOSound;

    // ─── Helpers ──────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "Fighter")
    const FAttackData& GetAttackData(EBoxingMove Move) const;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("FighterData", GetFName());
    }
};
