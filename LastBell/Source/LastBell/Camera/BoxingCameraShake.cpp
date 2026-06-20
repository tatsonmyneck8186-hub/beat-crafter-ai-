#include "Camera/BoxingCameraShake.h"
#include "Camera/PlayerCameraManager.h"

// Camera shake classes register themselves so Blueprints can subclass them
// and configure oscillation patterns in the BP's Details panel.
// The C++ constructor sets sensible defaults that Blueprint overrides can tweak.

UBoxingCameraShake_Light::UBoxingCameraShake_Light()
{
    OscillationDuration = 0.18f;
    OscillationBlendInTime = 0.01f;
    OscillationBlendOutTime = 0.10f;
}

UBoxingCameraShake_Heavy::UBoxingCameraShake_Heavy()
{
    OscillationDuration = 0.28f;
    OscillationBlendInTime = 0.01f;
    OscillationBlendOutTime = 0.15f;
}

UBoxingCameraShake_KO::UBoxingCameraShake_KO()
{
    OscillationDuration = 0.60f;
    OscillationBlendInTime = 0.02f;
    OscillationBlendOutTime = 0.30f;
}
