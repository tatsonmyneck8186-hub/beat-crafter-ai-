#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BoxingTypes.h"
#include "BoxerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKOMeterChanged, float, NewKOMeter, float, MaxKOMeter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKnockdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKO);

UCLASS(ClassGroup=(Boxing), meta=(BlueprintSpawnableComponent))
class LASTBELL_API UBoxerStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBoxerStatsComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // ─── Initialization ───────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void InitializeStats(const FFighterStats& InStats);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ResetStats();

    // ─── Health ────────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Stats|Health")
    void ApplyDamage(float Damage);

    UFUNCTION(BlueprintPure, Category = "Stats|Health")
    float GetHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Stats|Health")
    float GetMaxHealth() const { return BaseStats.MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Stats|Health")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintPure, Category = "Stats|Health")
    bool IsAlive() const { return CurrentHealth > 0.f; }

    // ─── Stamina ───────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Stats|Stamina")
    bool ConsumeStamina(float Amount);

    UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
    float GetStamina() const { return CurrentStamina; }

    UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
    float GetMaxStamina() const { return BaseStats.MaxStamina; }

    UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
    bool HasStaminaFor(float Amount) const { return CurrentStamina >= Amount; }

    // ─── KO Meter ──────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Stats|KOMeter")
    void AddKOMeter(float Amount);

    UFUNCTION(BlueprintPure, Category = "Stats|KOMeter")
    float GetKOMeter() const { return CurrentKOMeter; }

    UFUNCTION(BlueprintPure, Category = "Stats|KOMeter")
    float GetMaxKOMeter() const { return BaseStats.MaxKOMeter; }

    UFUNCTION(BlueprintPure, Category = "Stats|KOMeter")
    float GetKOMeterPercent() const;

    UFUNCTION(BlueprintPure, Category = "Stats|KOMeter")
    bool IsKOMeterFull() const { return CurrentKOMeter >= BaseStats.MaxKOMeter; }

    // ─── Multipliers ───────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetDamageMultiplier() const { return BaseStats.DamageMultiplier; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    float GetDefenseMultiplier() const { return BaseStats.DefenseMultiplier; }

    UFUNCTION(BlueprintPure, Category = "Stats")
    const FFighterStats& GetBaseStats() const { return BaseStats; }

    // ─── Events ────────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnStaminaChanged OnStaminaChanged;

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnKOMeterChanged OnKOMeterChanged;

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnKnockdown OnKnockdown;

    UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
    FOnKO OnKO;

private:
    UPROPERTY()
    FFighterStats BaseStats;

    UPROPERTY(VisibleInstanceOnly, Category = "Stats|Debug")
    float CurrentHealth = 100.f;

    UPROPERTY(VisibleInstanceOnly, Category = "Stats|Debug")
    float CurrentStamina = 100.f;

    UPROPERTY(VisibleInstanceOnly, Category = "Stats|Debug")
    float CurrentKOMeter = 0.f;

    float TimeSinceLastStaminaUse = 0.f;
    bool bStaminaRegenActive = true;

    void TickStaminaRegen(float DeltaTime);
    void TickKOMeterDecay(float DeltaTime);
};
