#pragma once
#include "CoreMinimal.h"
#include "BoxingTypes.generated.h"

//--------------------------------------------------------------------
// Enums
//--------------------------------------------------------------------

UENUM(BlueprintType)
enum class EBoxingMove : uint8
{
    None     UMETA(DisplayName="None"),
    Jab      UMETA(DisplayName="Jab"),
    Hook     UMETA(DisplayName="Hook"),
    Uppercut UMETA(DisplayName="Uppercut"),
    Duck     UMETA(DisplayName="Duck"),
    Dodge    UMETA(DisplayName="Dodge"),
};

// Which part of the body is targeted by an attack
UENUM(BlueprintType)
enum class EHitZone : uint8
{
    Head UMETA(DisplayName="Head"),
    Body UMETA(DisplayName="Body"),
};

// Three-phase commit structure for every punch
UENUM(BlueprintType)
enum class EAttackPhase : uint8
{
    None     UMETA(DisplayName="None"),
    Startup  UMETA(DisplayName="Startup"),   // Wind-up — no hitbox, can be interrupted
    Active   UMETA(DisplayName="Active"),    // Hitbox live
    Recovery UMETA(DisplayName="Recovery"),  // End-lag — fully committed, vulnerable
};

UENUM(BlueprintType)
enum class EFighterType : uint8
{
    Player       UMETA(DisplayName="Player"),
    Rookie       UMETA(DisplayName="Rookie"),
    CounterBoxer UMETA(DisplayName="Counter Boxer"),
    Heavyweight  UMETA(DisplayName="Heavyweight"),
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
    PreMatch      UMETA(DisplayName="Pre-Match"),
    RoundStart    UMETA(DisplayName="Round Start"),
    RoundActive   UMETA(DisplayName="Round Active"),
    KnockDown     UMETA(DisplayName="Knock Down"),
    BetweenRounds UMETA(DisplayName="Between Rounds"),
    MatchWin      UMETA(DisplayName="Match Win"),
    MatchLose     UMETA(DisplayName="Match Lose"),
};

UENUM(BlueprintType)
enum class EBoxerState : uint8
{
    Idle        UMETA(DisplayName="Idle"),
    MovingLeft  UMETA(DisplayName="Moving Left"),
    MovingRight UMETA(DisplayName="Moving Right"),
    Ducking     UMETA(DisplayName="Ducking"),
    Dodging     UMETA(DisplayName="Dodging"),
    Attacking   UMETA(DisplayName="Attacking"),
    HitStun     UMETA(DisplayName="Hit Stun"),
    KnockedDown UMETA(DisplayName="Knocked Down"),
    KO          UMETA(DisplayName="KO"),
    Victory     UMETA(DisplayName="Victory"),
    Defeat      UMETA(DisplayName="Defeat"),
};

UENUM(BlueprintType)
enum class EHitResult : uint8
{
    Miss    UMETA(DisplayName="Miss"),
    Hit     UMETA(DisplayName="Hit"),
    Blocked UMETA(DisplayName="Blocked"),
    Dodged  UMETA(DisplayName="Dodged"),
    Counter UMETA(DisplayName="Counter"),
};

// AI emotional/tactical state
UENUM(BlueprintType)
enum class EAggressionState : uint8
{
    Passive    UMETA(DisplayName="Passive"),    // Backing off, recovering
    Normal     UMETA(DisplayName="Normal"),     // Standard pressure
    Aggressive UMETA(DisplayName="Aggressive"), // Pressing, shorter gaps
    Desperate  UMETA(DisplayName="Desperate"),  // Low health comeback mode
};

//--------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
    EBoxingMove MoveType = EBoxingMove::None;

    // Damage
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Damage")
    float BaseDamage = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Damage")
    float StaminaCost = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Damage")
    float KOMeterGain = 5.f;

    // Hit response
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|HitResponse")
    float HitStunDuration = 0.12f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|HitResponse")
    float HitPauseDuration = 0.06f;

    // Three-phase timing (seconds from attack start)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Timing")
    float HitboxActiveStart = 0.10f; // Startup ends here

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Timing")
    float HitboxActiveEnd = 0.25f;   // Active ends here

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Timing")
    float TotalDuration = 0.45f;     // Recovery ends here

    // Range
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Range")
    float AttackRange = 160.f;

    // Knockback — applied to opponent on hit
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Knockback")
    float KnockbackForce = 200.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Knockback")
    float KnockbackVerticalRatio = 0.12f;

    // Camera — FOV nudge during punch phases
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Camera")
    float StartupFOVDelta = -2.f;   // Zoom in during wind-up

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Camera")
    float ActiveFOVDelta = 4.f;     // Push out for drama on impact

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Camera")
    float CameraShakeIntensity = 1.f;

    // Screen flash for heavy hits
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Effects")
    bool bCausesScreenFlash = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Effects")
    float ScreenFlashIntensity = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Effects")
    float ScreenFlashDuration = 0.08f;

    // Audio zone
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Audio")
    EHitZone TargetZone = EHitZone::Head;
};

USTRUCT(BlueprintType)
struct FFighterStats
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float MaxHealth = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float MaxStamina = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float StaminaRegenRate = 18.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float StaminaRegenDelay = 0.8f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float MaxKOMeter = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float KOMeterDecayRate = 3.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float MoveSpeed = 380.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float DamageMultiplier = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float DefenseMultiplier = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float DodgeDistance = 80.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats")
    float DodgeDuration = 0.25f;
};

USTRUCT(BlueprintType)
struct FAIPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float AttackInterval = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float AttackIntervalVariance = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float DodgeReactionTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float CounterWindowDuration = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float AggressionLevel = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float DodgeProbability = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float PreferredRange = 140.f;

    // Feinting
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float FakePunchProbability = 0.15f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float FakePunchCooldown = 6.f;

    // Fatigue
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float FatigueAttackSlowdown = 0.5f; // At max fatigue attacks take this much longer

    // Comeback
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float ComebackHealthThreshold = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float ComebackAggressionBoost = 1.8f;

    // Defensive pause after taking a hit
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI|Polish")
    float DefensivePhaseDuration = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    bool bIsCounterFighter = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    bool bIsHeavyweight = false;
};

USTRUCT(BlueprintType)
struct FRoundResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 RoundNumber = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bPlayerWon = false;

    UPROPERTY(BlueprintReadOnly)
    bool bKnockout = false;

    UPROPERTY(BlueprintReadOnly)
    float PlayerHealthRemaining = 100.f;

    UPROPERTY(BlueprintReadOnly)
    float OpponentHealthRemaining = 100.f;
};
