#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BoxingTypes.h"
#include "CombatComponent.generated.h"

class UBoxerStatsComponent;
class UCameraShakeBase;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackExecuted, EBoxingMove, Move);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitLanded, EBoxingMove, Move, EHitResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStateChanged, bool, bIsAttacking);

UCLASS(ClassGroup=(Boxing), meta=(BlueprintSpawnableComponent))
class LASTBELL_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // ─── Setup ────────────────────────────────────────────────────────────────

    void SetStatsComponent(UBoxerStatsComponent* InStats);
    void SetOpponent(AActor* InOpponent);
    void SetFighterData(const TArray<FAttackData>& Attacks);
    void SetAttackData(EBoxingMove Move, const FAttackData& Data);

    // ─── Actions ───────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool TryAttack(EBoxingMove Move);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ReceiveHit(const FAttackData& AttackData, AActor* Attacker);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartDodge();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartDuck();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopDuck();

    // ─── State Queries ─────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsAttacking() const { return bIsAttacking; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsDucking() const { return bIsDucking; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsDodging() const { return bIsDodging; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsInHitStun() const { return bInHitStun; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool CanAct() const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    EBoxingMove GetCurrentAttack() const { return CurrentAttack; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsHitboxActive() const { return bHitboxActive; }

    // ─── Events ────────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnAttackExecuted OnAttackExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnHitLanded OnHitLanded;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
    FOnAttackStateChanged OnAttackStateChanged;

    // ─── Visual Config ──────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Effects")
    TSubclassOf<UCameraShakeBase> LightHitCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Effects")
    TSubclassOf<UCameraShakeBase> HeavyHitCameraShake;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Effects")
    TObjectPtr<UParticleSystem> HitParticle;

    UPROPERTY(EditDefaultsOnly, Category = "Combat|Effects")
    TObjectPtr<UParticleSystem> KOParticle;

private:
    // Attack state
    bool bIsAttacking = false;
    bool bIsDucking = false;
    bool bIsDodging = false;
    bool bInHitStun = false;
    bool bHitboxActive = false;
    bool bHitRegisteredThisAttack = false;

    EBoxingMove CurrentAttack = EBoxingMove::None;
    FAttackData CurrentAttackData;

    float AttackElapsed = 0.f;
    float HitStunElapsed = 0.f;
    float DodgeElapsed = 0.f;
    float HitStunDuration = 0.f;
    float DodgeDuration = 0.3f;

    // Per-move data
    TMap<EBoxingMove, FAttackData> AttackDataMap;

    UPROPERTY()
    TObjectPtr<UBoxerStatsComponent> StatsComp;

    UPROPERTY()
    TObjectPtr<AActor> OpponentActor;

    void TickAttack(float DeltaTime);
    void TickHitStun(float DeltaTime);
    void TickDodge(float DeltaTime);
    void CheckHit();
    void EndAttack();
    void TriggerHitPause(float Duration);
    void TriggerCameraShake(float Intensity);
    void SpawnHitEffect(const FVector& Location);
};
