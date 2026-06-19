#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "BoxingCameraShake.generated.h"

// Assign these classes to CombatComponent.LightHitCameraShake / HeavyHitCameraShake
// in the BP_PlayerBoxer / BP_AIBoxer Details panel.
// They use the Perlin Noise pattern asset set in the Blueprint.
// Alternatively subclass UCameraModifier for full control.

UCLASS(Blueprintable)
class LASTBELL_API UBoxingCameraShake_Light : public UCameraShakeBase
{
    GENERATED_BODY()
public:
    UBoxingCameraShake_Light();
};

UCLASS(Blueprintable)
class LASTBELL_API UBoxingCameraShake_Heavy : public UCameraShakeBase
{
    GENERATED_BODY()
public:
    UBoxingCameraShake_Heavy();
};

UCLASS(Blueprintable)
class LASTBELL_API UBoxingCameraShake_KO : public UCameraShakeBase
{
    GENERATED_BODY()
public:
    UBoxingCameraShake_KO();
};
