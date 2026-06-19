#pragma once

#include "CoreMinimal.h"
#include "BoxingTypes.generated.h"

// ─── Enums ───────────────────────────────────────────────────────────────────

UENUM(BlueprintType)
enum class EBoxingMove : uint8
{
    None        UMETA(DisplayName = "None"),
    Jab         UMETA(DisplayName = "Jab"),
    Hook        UMETA(DisplayName = "Hook"),
    Uppercut    UMETA(DisplayName = "Uppercut"),
    Duck        UMETA(DisplayName = "Duck"),
    Dodge       UMETA(DisplayName = "Dodge"),
};

UENUM(BlueprintType)
enum class EFighterType : uint8
{
    Player          UMETA(DisplayName = "Player"),
    Rookie          UMETA(DisplayName = "Rookie"),
    CounterBoxer    UMETA(DisplayName = "Counter Boxer"),
    Heavyweight     UMETA(DisplayName = "Heavyweight"),
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
    PreMatch        UMETA(DisplayName = "Pre-Match"),
    RoundStart      UMETA(DisplayName = "Round Start"),
    RoundActive     UMETA(DisplayName = "Round Active"),
    KnockDown       UMETA(DisplayName = "Knock Down"),
    BetweenRounds   UMETA(DisplayName = "Between Rounds"),
    MatchWin        UMETA(DisplayName = "Match Win"),
    MatchLose       UMETA(DisplayName = "Match Lose"),
};

UENUM(BlueprintType)
enum class EBoxerState : uint8
{
    Idle            UMETA(DisplayName = "Idle"),
    MovingLeft      UMETA(DisplayName = "Moving Left"),
    MovingRight     UMETA(DisplayName = "Moving Right"),
    Ducking         UMETA(DisplayName = "Ducking"),
    Dodging         UMETA(DisplayName = "Dodging"),
    Attacking       UMETA(DisplayName = "Attacking"),
    HitStun         UMETA(DisplayName = "Hit Stun"),
    KnockedDown     UMETA(DisplayName = "Knocked Down"),
    KO              UMETA(DisplayName = "KO"),
};

UENUM(BlueprintType)
enum class EHitResult : uint8
{
    Miss        UMETA(DisplayName = "Miss"),
    Hit         UMETA(DisplayName = "Hit"),
    Blocked     UMETA(DisplayName = "Blocked"),
    Dodged      UMETA(DisplayName = "Dodged"),
    Counter     UMETA(DisplayName = "Counter"),
};

// ─── Structs ──────────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    EBoxingMove MoveType = EBoxingMove::None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float BaseDamage = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float StaminaCost = 10.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float KOMeterGain = 5.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float HitStunDuration = 0.12f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float HitPauseDuration = 0.06f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float AttackRange = 160.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float HitboxActiveStart = 0.10f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float HitboxActiveEnd = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float TotalDuration = 0.45f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float CameraShakeIntensity = 1.f;
};

USTRUCT(BlueprintType)
struct FFighterStats
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxHealth = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxStamina = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float StaminaRegenRate = 18.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float StaminaRegenDelay = 0.8f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxKOMeter = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float KOMeterDecayRate = 3.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MoveSpeed = 380.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DamageMultiplier = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DefenseMultiplier = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DodgeDistance = 80.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DodgeDuration = 0.25f;
};

USTRUCT(BlueprintType)
struct FAIPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float AttackInterval = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float AttackIntervalVariance = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float DodgeReactionTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float CounterWindowDuration = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float AggressionLevel = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float DodgeProbability = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float PreferredRange = 140.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    bool bIsCounterFighter = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
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
