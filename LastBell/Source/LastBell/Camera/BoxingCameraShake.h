#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "BoxingCameraShake.generated.h"

// Light hit shake (jabs)
UCLASS()
class LASTBELL_API UBoxingCameraShake_Light : public UCameraShakeBase
{
    GENERATED_BODY()

public:
    UBoxingCameraShake_Light();
};

// Heavy hit shake (hooks, uppercuts)
UCLASS()
class LASTBELL_API UBoxingCameraShake_Heavy : public UCameraShakeBase
{
    GENERATED_BODY()
};

// KO shake
UCLASS()
class LASTBELL_API UBoxingCameraShake_KO : public UCameraShakeBase
{
    GENERATED_BODY()
};
