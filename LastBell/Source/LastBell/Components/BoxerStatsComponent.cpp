#include "Components/BoxerStatsComponent.h"

UBoxerStatsComponent::UBoxerStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBoxerStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    // Default stats used until InitializeStats is called
    CurrentHealth = BaseStats.MaxHealth;
    CurrentStamina = BaseStats.MaxStamina;
    CurrentKOMeter = 0.f;
}

void UBoxerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TickStaminaRegen(DeltaTime);
    TickKOMeterDecay(DeltaTime);
}

void UBoxerStatsComponent::InitializeStats(const FFighterStats& InStats)
{
    BaseStats = InStats;
    CurrentHealth = BaseStats.MaxHealth;
    CurrentStamina = BaseStats.MaxStamina;
    CurrentKOMeter = 0.f;
    OnHealthChanged.Broadcast(CurrentHealth, BaseStats.MaxHealth);
    OnStaminaChanged.Broadcast(CurrentStamina, BaseStats.MaxStamina);
    OnKOMeterChanged.Broadcast(CurrentKOMeter, BaseStats.MaxKOMeter);
}

void UBoxerStatsComponent::ResetStats()
{
    CurrentHealth = BaseStats.MaxHealth;
    CurrentStamina = BaseStats.MaxStamina;
    CurrentKOMeter = 0.f;
    OnHealthChanged.Broadcast(CurrentHealth, BaseStats.MaxHealth);
    OnStaminaChanged.Broadcast(CurrentStamina, BaseStats.MaxStamina);
    OnKOMeterChanged.Broadcast(CurrentKOMeter, BaseStats.MaxKOMeter);
}

void UBoxerStatsComponent::ApplyDamage(float Damage)
{
    if (Damage <= 0.f) return;

    float ActualDamage = Damage / FMath::Max(BaseStats.DefenseMultiplier, 0.1f);
    CurrentHealth = FMath::Max(0.f, CurrentHealth - ActualDamage);
    OnHealthChanged.Broadcast(CurrentHealth, BaseStats.MaxHealth);

    if (CurrentHealth <= 0.f)
    {
        OnKO.Broadcast();
    }
}

bool UBoxerStatsComponent::ConsumeStamina(float Amount)
{
    if (CurrentStamina < Amount) return false;

    CurrentStamina = FMath::Max(0.f, CurrentStamina - Amount);
    TimeSinceLastStaminaUse = 0.f;
    bStaminaRegenActive = false;
    OnStaminaChanged.Broadcast(CurrentStamina, BaseStats.MaxStamina);
    return true;
}

void UBoxerStatsComponent::AddKOMeter(float Amount)
{
    if (Amount <= 0.f) return;

    bool bWasFull = IsKOMeterFull();
    CurrentKOMeter = FMath::Min(BaseStats.MaxKOMeter, CurrentKOMeter + Amount);
    OnKOMeterChanged.Broadcast(CurrentKOMeter, BaseStats.MaxKOMeter);

    if (!bWasFull && IsKOMeterFull())
    {
        OnKnockdown.Broadcast();
    }
}

float UBoxerStatsComponent::GetHealthPercent() const
{
    return BaseStats.MaxHealth > 0.f ? CurrentHealth / BaseStats.MaxHealth : 0.f;
}

float UBoxerStatsComponent::GetStaminaPercent() const
{
    return BaseStats.MaxStamina > 0.f ? CurrentStamina / BaseStats.MaxStamina : 0.f;
}

float UBoxerStatsComponent::GetKOMeterPercent() const
{
    return BaseStats.MaxKOMeter > 0.f ? CurrentKOMeter / BaseStats.MaxKOMeter : 0.f;
}

void UBoxerStatsComponent::TickStaminaRegen(float DeltaTime)
{
    TimeSinceLastStaminaUse += DeltaTime;

    if (!bStaminaRegenActive && TimeSinceLastStaminaUse >= BaseStats.StaminaRegenDelay)
    {
        bStaminaRegenActive = true;
    }

    if (bStaminaRegenActive && CurrentStamina < BaseStats.MaxStamina)
    {
        CurrentStamina = FMath::Min(BaseStats.MaxStamina,
            CurrentStamina + BaseStats.StaminaRegenRate * DeltaTime);
        OnStaminaChanged.Broadcast(CurrentStamina, BaseStats.MaxStamina);
    }
}

void UBoxerStatsComponent::TickKOMeterDecay(float DeltaTime)
{
    if (CurrentKOMeter > 0.f && !IsKOMeterFull())
    {
        CurrentKOMeter = FMath::Max(0.f,
            CurrentKOMeter - BaseStats.KOMeterDecayRate * DeltaTime);
        OnKOMeterChanged.Broadcast(CurrentKOMeter, BaseStats.MaxKOMeter);
    }
}
