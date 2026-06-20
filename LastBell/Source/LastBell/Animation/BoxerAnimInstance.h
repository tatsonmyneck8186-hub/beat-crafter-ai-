#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/BoxingTypes.h"
#include "BoxerAnimInstance.generated.h"

UCLASS()
class LASTBELL_API UBoxerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // ─── Animation State Properties (read by AnimGraph) ──────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    EBoxerState BoxerState = EBoxerState::Idle;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    EBoxingMove CurrentAttack = EBoxingMove::None;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    float MoveSpeed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    float HorizontalMovement = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    bool bIsDucking = false;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    bool bIsDodging = false;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    bool bIsKnockedDown = false;

    UPROPERTY(BlueprintReadOnly, Category = "Anim")
    bool bIsKO = false;

private:
    UPROPERTY()
    TObjectPtr<class ABoxerCharacter> OwnerBoxer;
};
