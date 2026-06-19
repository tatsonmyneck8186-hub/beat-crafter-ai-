#include "Animation/BoxerAnimInstance.h"
#include "Characters/BoxerCharacter.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBoxerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerBoxer)
    {
        OwnerBoxer = Cast<ABoxerCharacter>(TryGetPawnOwner());
        if (!OwnerBoxer) return;
    }

    BoxerState = OwnerBoxer->GetCurrentState();
    bIsAttacking = OwnerBoxer->CombatComponent->IsAttacking();
    bIsDucking   = OwnerBoxer->CombatComponent->IsDucking();
    bIsDodging   = OwnerBoxer->CombatComponent->IsDodging();
    bIsKnockedDown = (BoxerState == EBoxerState::KnockedDown);
    bIsKO          = (BoxerState == EBoxerState::KO);
    CurrentAttack  = OwnerBoxer->CombatComponent->GetCurrentAttack();

    if (UCharacterMovementComponent* Move =
        OwnerBoxer->GetCharacterMovement())
    {
        MoveSpeed = Move->Velocity.Size();
        // Horizontal relative to character forward
        FVector LocalVel = OwnerBoxer->GetActorTransform()
            .InverseTransformVector(Move->Velocity);
        HorizontalMovement = LocalVel.Y;
    }
}
