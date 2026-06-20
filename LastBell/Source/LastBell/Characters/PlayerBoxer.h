#pragma once

#include "CoreMinimal.h"
#include "Characters/BoxerCharacter.h"
#include "InputActionValue.h"
#include "PlayerBoxer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class LASTBELL_API APlayerBoxer : public ABoxerCharacter
{
    GENERATED_BODY()

public:
    APlayerBoxer();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
    virtual void Tick(float DeltaTime) override;

    // ─── Camera ─────────────────────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    // ─── Enhanced Input ──────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_MoveHorizontal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Jab;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Hook;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Uppercut;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Duck;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Dodge;

private:
    // Input handlers
    void OnMoveHorizontal(const FInputActionValue& Value);
    void OnJab(const FInputActionValue& Value);
    void OnHook(const FInputActionValue& Value);
    void OnUppercut(const FInputActionValue& Value);
    void OnDuckPressed(const FInputActionValue& Value);
    void OnDuckReleased(const FInputActionValue& Value);
    void OnDodge(const FInputActionValue& Value);

    float HorizontalInput = 0.f;
};
