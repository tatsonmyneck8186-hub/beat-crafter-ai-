#include "Camera/BoxingCameraShake.h"
#include "Camera/CameraShakeSourceComponent.h"

UBoxingCameraShake_Light::UBoxingCameraShake_Light()
{
    OscillationDuration = 0.18f;
    OscillationBlendInTime = 0.01f;
    OscillationBlendOutTime = 0.1f;
}
