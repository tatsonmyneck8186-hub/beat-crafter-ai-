#include "Environment/CrowdActor.h"
#include "Components/InstancedStaticMeshComponent.h"

ACrowdActor::ACrowdActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CrowdMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CrowdMeshes"));
    RootComponent = CrowdMeshes;
    CrowdMeshes->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACrowdActor::BeginPlay()
{
    Super::BeginPlay();
    SpawnCrowd();
}

void ACrowdActor::SpawnCrowd()
{
    CrowdMeshes->ClearInstances();

    // Arrange crowd in concentric rings around the boxing ring
    int32 PerRow = FMath::Max(8, CrowdCount / 4);

    for (int32 i = 0; i < CrowdCount; ++i)
    {
        float Angle = (float(i) / CrowdCount) * 360.f;
        float Radius = ArenaRadius + FMath::RandRange(-60.f, 60.f);
        float X = FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius;
        float Y = FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius;
        float Z = CrowdHeight + FMath::RandRange(0.f, 30.f);

        // Face toward center
        FRotator Rot(0.f, Angle + 180.f, 0.f);
        float Scale = FMath::RandRange(0.9f, 1.1f);

        FTransform T(
            Rot,
            FVector(X, Y, Z),
            FVector(Scale));
        CrowdMeshes->AddInstance(T);
    }
}

void ACrowdActor::AnimateCheering()
{
    // Randomize instance transforms slightly to simulate crowd motion
    int32 Count = CrowdMeshes->GetInstanceCount();
    for (int32 i = 0; i < Count; ++i)
    {
        FTransform T;
        CrowdMeshes->GetInstanceTransform(i, T, true);
        T.SetScale3D(T.GetScale3D() * FMath::RandRange(1.f, 1.08f));
        CrowdMeshes->UpdateInstanceTransform(i, T, true, false);
    }
    CrowdMeshes->MarkRenderStateDirty();
}
