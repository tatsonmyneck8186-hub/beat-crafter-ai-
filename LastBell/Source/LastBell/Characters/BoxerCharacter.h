#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/BoxingTypes.h"
#include "Core/IBoxerInterface.h"
#include "BoxerCharacter.generated.h"

class UBoxerStatsComponent;
class UCombatComponent;
class UFighterDataAsset;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class LASTBELL_API ABoxerCharacter : public ACharacter, public IBoxerInterface
{
    GENERATED_BODY()

public:
    ABoxerCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ─── Components ───────────────────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBoxerStatsComponent> StatsComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCombatComponent> CombatComponent;

    // ─── Fighter Data ────────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fighter")
    TObjectPtr<UFighterDataAsset> FighterData;

    UFUNCTION(BlueprintCallable, Category = "Fighter")
    void LoadFighterData(UFighterDataAsset* Data);

    UFUNCTION(BlueprintPure, Category = "Fighter")
    UFighterDataAsset* GetFighterData() const { return FighterData; }

    // ─── IBoxerInterface ─────────────────────────────────────────────────────────

    virtual void ExecuteAttack_Implementation(EBoxingMove Move) override;
    virtual void ReceiveHit_Implementation(const FAttackData& AttackData, AActor* Attacker) override;
    virtual void ExecuteDodge_Implementation() override;
    virtual void ExecuteDuck_Implementation() override;
    virtual void StopDuck_Implementation() override;
    virtual EBoxerState GetBoxerState_Implementation() const override;
    virtual float GetHealthPercent_Implementation() const override;
    virtual float GetStaminaPercent_Implementation() const override;
    virtual float GetKOMeterPercent_Implementation() const override;
    virtual bool IsAttacking_Implementation() const override;
    virtual bool IsDucking_Implementation() const override;
    virtual void SetOpponent_Implementation(AActor* Opponent) override;
    virtual AActor* GetOpponent_Implementation() const override;

    // ─── State ───────────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "Boxer")
    EBoxerState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "Boxer")
    void SetBoxerEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Boxer")
    bool IsBoxerEnabled() const { return bBoxerEnabled; }

    UFUNCTION(BlueprintCallable, Category = "Boxer")
    void TriggerKnockdown();

    UFUNCTION(BlueprintCallable, Category = "Boxer")
    void RecoverFromKnockdown();

    // ─── Events (override in Blueprint) ───────────────────────────────────────

    UFUNCTION(BlueprintImplementableEvent, Category = "Boxer|Events")
    void OnAttackStarted(EBoxingMove Move);

    UFUNCTION(BlueprintImplementableEvent, Category = "Boxer|Events")
    void OnHitReceived(EBoxingMove AttackType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Boxer|Events")
    void OnKnockdownTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Boxer|Events")
    void OnKOTriggered();

    UFUNCTION(BlueprintImplementableEvent, Category = "Boxer|Events")
    void OnDodgeStarted();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Boxer")
    EBoxerState CurrentState = EBoxerState::Idle;

    UPROPERTY()
    TObjectPtr<AActor> OpponentRef;

    bool bBoxerEnabled = true;
    bool bIsKnockedDown = false;
    bool bIsKO = false;

    // Ring bounds enforcement
    float RingHalfWidth = 300.f;
    FVector RingCenter = FVector::ZeroVector;

    void UpdateFacingDirection();
    void ClampToRingBounds();

    UFUNCTION()
    void HandleKnockdown();

    UFUNCTION()
    void HandleKO();
};
