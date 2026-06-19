#include "Environment/BoxingRingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ABoxingRingActor::ABoxingRingActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Floor
    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
    RootComponent = FloorMesh;

    // Ropes
    RopeNorth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeNorth"));
    RopeNorth->SetupAttachment(RootComponent);

    RopeSouth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeSouth"));
    RopeSouth->SetupAttachment(RootComponent);

    RopeEast = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeEast"));
    RopeEast->SetupAttachment(RootComponent);

    RopeWest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeWest"));
    RopeWest->SetupAttachment(RootComponent);

    // Corner posts
    PostNE = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostNE"));
    PostNE->SetupAttachment(RootComponent);
    PostNW = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostNW"));
    PostNW->SetupAttachment(RootComponent);
    PostSE = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostSE"));
    PostSE->SetupAttachment(RootComponent);
    PostSW = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostSW"));
    PostSW->SetupAttachment(RootComponent);

    // Spot lights
    SpotLight1 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight1"));
    SpotLight1->SetupAttachment(RootComponent);
    SpotLight2 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight2"));
    SpotLight2->SetupAttachment(RootComponent);
    SpotLight3 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight3"));
    SpotLight3->SetupAttachment(RootComponent);
    SpotLight4 = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight4"));
    SpotLight4->SetupAttachment(RootComponent);
}

void ABoxingRingActor::BeginPlay()
{
    Super::BeginPlay();
    BuildRing();
    BuildLighting();
}

void ABoxingRingActor::BuildRing()
{
    float Half = RingSize * 0.5f;
    float Thickness = 15.f;
    float PostW = 20.f;

    // Floor scale (1uu = 1cm in UE, so RingSize in cm)
    FloorMesh->SetRelativeScale3D(
        FVector(RingSize / 100.f, RingSize / 100.f, 0.1f));

    // Apply canvas color
    if (UMaterialInterface* Mat = FloorMesh->GetMaterial(0))
    {
        UMaterialInstanceDynamic* DynMat =
            UMaterialInstanceDynamic::Create(Mat, this);
        DynMat->SetVectorParameterValue(TEXT("Color"), CanvasColor);
        FloorMesh->SetMaterial(0, DynMat);
    }

    // Rope positions
    RopeNorth->SetRelativeLocation(FVector(Half, 0.f, RopeHeight));
    RopeNorth->SetRelativeScale3D(FVector(Thickness / 100.f, RingSize / 100.f, Thickness / 100.f));

    RopeSouth->SetRelativeLocation(FVector(-Half, 0.f, RopeHeight));
    RopeSouth->SetRelativeScale3D(FVector(Thickness / 100.f, RingSize / 100.f, Thickness / 100.f));

    RopeEast->SetRelativeLocation(FVector(0.f, Half, RopeHeight));
    RopeEast->SetRelativeScale3D(FVector(RingSize / 100.f, Thickness / 100.f, Thickness / 100.f));

    RopeWest->SetRelativeLocation(FVector(0.f, -Half, RopeHeight));
    RopeWest->SetRelativeScale3D(FVector(RingSize / 100.f, Thickness / 100.f, Thickness / 100.f));

    // Corner posts
    PostNE->SetRelativeLocation(FVector(Half, Half, RopeHeight * 0.5f));
    PostNE->SetRelativeScale3D(FVector(PostW / 100.f, PostW / 100.f, RopeHeight / 100.f));
    PostNW->SetRelativeLocation(FVector(Half, -Half, RopeHeight * 0.5f));
    PostNW->SetRelativeScale3D(FVector(PostW / 100.f, PostW / 100.f, RopeHeight / 100.f));
    PostSE->SetRelativeLocation(FVector(-Half, Half, RopeHeight * 0.5f));
    PostSE->SetRelativeScale3D(FVector(PostW / 100.f, PostW / 100.f, RopeHeight / 100.f));
    PostSW->SetRelativeLocation(FVector(-Half, -Half, RopeHeight * 0.5f));
    PostSW->SetRelativeScale3D(FVector(PostW / 100.f, PostW / 100.f, RopeHeight / 100.f));
}

void ABoxingRingActor::BuildLighting()
{
    float LightHeight = 600.f;
    float Half = RingSize * 0.3f;

    auto ConfigSpot = [&](USpotLightComponent* Light, FVector Offset)
    {
        Light->SetRelativeLocation(FOffset + FVector(0.f, 0.f, LightHeight));
        Light->SetRelativeRotation(FRotator(-85.f, 0.f, 0.f));
        Light->SetIntensity(SpotLightIntensity);
        Light->SetLightColor(SpotLightColor);
        Light->InnerConeAngle = 20.f;
        Light->OuterConeAngle = 40.f;
        Light->AttenuationRadius = 1200.f;
        Light->bUseInverseSquaredFalloff = true;
        Light->CastShadows = true;
    };

    // Fix: lambda capture needs correct syntax
    SpotLight1->SetRelativeLocation(FVector(-Half, -Half, LightHeight));
    SpotLight1->SetRelativeRotation(FRotator(-85.f, 45.f, 0.f));
    SpotLight1->SetIntensity(SpotLightIntensity);
    SpotLight1->SetLightColor(SpotLightColor);
    SpotLight1->InnerConeAngle = 22.f;
    SpotLight1->OuterConeAngle = 42.f;
    SpotLight1->AttenuationRadius = 1200.f;

    SpotLight2->SetRelativeLocation(FVector(Half, -Half, LightHeight));
    SpotLight2->SetRelativeRotation(FRotator(-85.f, 135.f, 0.f));
    SpotLight2->SetIntensity(SpotLightIntensity);
    SpotLight2->SetLightColor(SpotLightColor);
    SpotLight2->InnerConeAngle = 22.f;
    SpotLight2->OuterConeAngle = 42.f;
    SpotLight2->AttenuationRadius = 1200.f;

    SpotLight3->SetRelativeLocation(FVector(Half, Half, LightHeight));
    SpotLight3->SetRelativeRotation(FRotator(-85.f, 225.f, 0.f));
    SpotLight3->SetIntensity(SpotLightIntensity);
    SpotLight3->SetLightColor(SpotLightColor);
    SpotLight3->InnerConeAngle = 22.f;
    SpotLight3->OuterConeAngle = 42.f;
    SpotLight3->AttenuationRadius = 1200.f;

    SpotLight4->SetRelativeLocation(FVector(-Half, Half, LightHeight));
    SpotLight4->SetRelativeRotation(FRotator(-85.f, 315.f, 0.f));
    SpotLight4->SetIntensity(SpotLightIntensity);
    SpotLight4->SetLightColor(SpotLightColor);
    SpotLight4->InnerConeAngle = 22.f;
    SpotLight4->OuterConeAngle = 42.f;
    SpotLight4->AttenuationRadius = 1200.f;
}
