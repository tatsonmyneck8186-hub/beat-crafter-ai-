#include "Characters/PlayerBoxer.h"
#include "Components/BoxerStatsComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerBoxer::APlayerBoxer()
{
    // Camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 520.f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
    // Angle down from behind the player looking toward the opponent
    CameraBoom->SetRelativeRotation(FRotator(-12.f, 0.f, 0.f));
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritRoll = false;
    CameraBoom->bDoCollisionTest = false;

    // Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->FieldOfView = 75.f;
}

void APlayerBoxer::BeginPlay()
{
    Super::BeginPlay();

    // Register Enhanced Input mapping context
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void APlayerBoxer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Apply horizontal movement
    if (bBoxerEnabled && FMath::Abs(HorizontalInput) > 0.01f)
    {
        // Move perpendicular to facing direction (strafe around ring)
        FVector Right = GetActorRightVector();
        AddMovementInput(Right, HorizontalInput);
    }
}

void APlayerBoxer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IA_MoveHorizontal)
            EIC->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered,
                this, &APlayerBoxer::OnMoveHorizontal);

        if (IA_Jab)
            EIC->BindAction(IA_Jab, ETriggerEvent::Started,
                this, &APlayerBoxer::OnJab);

        if (IA_Hook)
            EIC->BindAction(IA_Hook, ETriggerEvent::Started,
                this, &APlayerBoxer::OnHook);

        if (IA_Uppercut)
            EIC->BindAction(IA_Uppercut, ETriggerEvent::Started,
                this, &APlayerBoxer::OnUppercut);

        if (IA_Duck)
        {
            EIC->BindAction(IA_Duck, ETriggerEvent::Started,
                this, &APlayerBoxer::OnDuckPressed);
            EIC->BindAction(IA_Duck, ETriggerEvent::Completed,
                this, &APlayerBoxer::OnDuckReleased);
        }

        if (IA_Dodge)
            EIC->BindAction(IA_Dodge, ETriggerEvent::Started,
                this, &APlayerBoxer::OnDodge);
    }
}

void APlayerBoxer::OnMoveHorizontal(const FInputActionValue& Value)
{
    HorizontalInput = Value.Get<float>();
}

void APlayerBoxer::OnJab(const FInputActionValue& Value)
{
    ExecuteAttack_Implementation(EBoxingMove::Jab);
}

void APlayerBoxer::OnHook(const FInputActionValue& Value)
{
    ExecuteAttack_Implementation(EBoxingMove::Hook);
}

void APlayerBoxer::OnUppercut(const FInputActionValue& Value)
{
    ExecuteAttack_Implementation(EBoxingMove::Uppercut);
}

void APlayerBoxer::OnDuckPressed(const FInputActionValue& Value)
{
    ExecuteDuck_Implementation();
}

void APlayerBoxer::OnDuckReleased(const FInputActionValue& Value)
{
    StopDuck_Implementation();
}

void APlayerBoxer::OnDodge(const FInputActionValue& Value)
{
    ExecuteDodge_Implementation();
}
