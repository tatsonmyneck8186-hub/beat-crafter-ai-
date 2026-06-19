#include "Components/BoxerAudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"

UBoxerAudioComponent::UBoxerAudioComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f; // Check breathing 10x/sec
}

void UBoxerAudioComponent::BeginPlay()
{
    Super::BeginPlay();

    if (HeavyBreathingLoop && GetOwner())
    {
        BreathingAudioComp = UGameplayStatics::SpawnSoundAttached(
            HeavyBreathingLoop,
            GetOwner()->GetRootComponent());
        if (BreathingAudioComp)
        {
            BreathingAudioComp->SetVolumeMultiplier(0.f);
            BreathingAudioComp->bAutoDestroy = false;
        }
    }
}

void UBoxerAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Breathing is updated externally via UpdateBreathing()
}

void UBoxerAudioComponent::PlayPunchImpact(EBoxingMove Move, EHitZone Zone,
    float DamageFraction)
{
    if (!CanPlayImpact()) return;

    USoundBase* PrimarySound = nullptr;
    bool bIsHeavy = false;

    switch (Move)
    {
        case EBoxingMove::Jab:
            PrimarySound = (Zone == EHitZone::Head) ? JabHeadSound : JabBodySound;
            break;
        case EBoxingMove::Hook:
            PrimarySound = (Zone == EHitZone::Head) ? HookHeadSound : HookBodySound;
            bIsHeavy = true;
            break;
        case EBoxingMove::Uppercut:
            PrimarySound = UppercutHeadSound;
            bIsHeavy = true;
            break;
        default: break;
    }

    PlaySoundAtOwner(PrimarySound);
    PlaySoundAtOwner(ImpactFleshLayerSound, 0.7f);
    if (bIsHeavy || DamageFraction > 0.7f)
    {
        PlaySoundAtOwner(HeavyImpactLayerSound, FMath::Lerp(0.4f, 1.f, DamageFraction));
    }

    LastImpactSoundTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
}

void UBoxerAudioComponent::PlayPunchMiss(EBoxingMove Move)
{
    PlaySoundAtOwner(SwishMissSound, 0.5f);
}

void UBoxerAudioComponent::PlayHitReaction()
{
    PlaySoundAtOwner(HitReactionGruntSound);
}

void UBoxerAudioComponent::PlayStagger()
{
    PlaySoundAtOwner(StaggerSound);
}

void UBoxerAudioComponent::PlayVictory()
{
    PlaySoundAtOwner(VictorySound);
}

void UBoxerAudioComponent::PlayDefeat()
{
    PlaySoundAtOwner(DefeatSound);
}

void UBoxerAudioComponent::UpdateBreathing(float StaminaPercent)
{
    if (!BreathingAudioComp) return;

    // Volume ramps from 0 to 1 as stamina drops from threshold to 0
    float Alpha = 0.f;
    if (StaminaPercent < BreathingStaminaThreshold)
    {
        Alpha = 1.f - (StaminaPercent / BreathingStaminaThreshold);
    }
    BreathingAudioComp->SetVolumeMultiplier(Alpha);
}

void UBoxerAudioComponent::PlaySoundAtOwner(USoundBase* Sound, float VolumeScale)
{
    if (!Sound || !GetOwner()) return;
    UGameplayStatics::SpawnSoundAtLocation(
        GetOwner(), Sound,
        GetOwner()->GetActorLocation(),
        FRotator::ZeroRotator,
        VolumeScale);
}

bool UBoxerAudioComponent::CanPlayImpact() const
{
    UWorld* World = GetWorld();
    if (!World) return false;
    return (World->GetTimeSeconds() - LastImpactSoundTime) >= MinImpactInterval;
}
