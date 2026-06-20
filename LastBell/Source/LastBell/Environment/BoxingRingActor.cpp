#include "Environment/BoxingRingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"

ABoxingRingActor::ABoxingRingActor()
{
    PrimaryActorTick.bCanEverTick = false;

    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
    RootComponent = FloorMesh;

    RopeNorth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeNorth"));
    RopeNorth->SetupAttachment(RootComponent);
    RopeSouth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeSouth"));
    RopeSouth->SetupAttachment(RootComponent);
    RopeEast = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeEast"));
    RopeEast->SetupAttachment(RootComponent);
    RopeWest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeWest"));
    RopeWest->SetupAttachment(RootComponent);

    PostNE = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostNE"));
    PostNE->SetupAttachment(RootComponent);
    PostNW = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostNW"));
    PostNW->SetupAttachment(RootComponent);
    PostSE = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostSE"));
    PostSE->SetupAttachment(RootComponent);
    PostSW = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostSW"));
    PostSW->SetupAttachment(RootComponent);

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

    FloorMesh->SetRelativeScale3D(
        FVector(RingSize / 100.f, RingSize / 100.f, 0.1f));

    if (UMaterialInterface* Mat = FloorMesh->GetMaterial(0))
    {
        UMaterialInstanceDynamic* DynMat =
            UMaterialInstanceDynamic::Create(Mat, this);
        DynMat->SetVectorParameterValue(TEXT("Color"), CanvasColor);
        FloorMesh->SetMaterial(0, DynMat);
    }

    // Rope placement
    RopeNorth->SetRelativeLocation(FVector(Half, 0.f, RopeHeight));
    RopeNorth->SetRelativeScale3D(FVector(Thickness / 100.f, RingSize / 100.f, Thickness / 100.f));
    RopeSouth->SetRelativeLocation(FVector(-Half, 0.f, RopeHeight));
    RopeSouth->SetRelativeScale3D(FVector(Thickness / 100.f, RingSize / 100.f, Thickness / 100.f));
    RopeEast->SetRelativeLocation(FVector(0.f, Half, RopeHeight));
    RopeEast->SetRelativeScale3D(FVector(RingSize / 100.f, Thickness / 100.f, Thickness / 100.f));
    RopeWest->SetRelativeLocation(FVector(0.f, -Half, RopeHeight));
    RopeWest->SetRelativeScale3D(FVector(RingSize / 100.f, Thickness / 100.f, Thickness / 100.f));

    // Corner posts
    auto PlacePost = [&](UStaticMeshComponent* Post, float PX, float PY)
    {
        Post->SetRelativeLocation(FVector(PX, PY, RopeHeight * 0.5f));
        Post->SetRelativeScale3D(FVector(PostW / 100.f, PostW / 100.f, (RopeHeight + 20.f) / 100.f));
    };
    PlacePost(PostNE,  Half,  Half);
    PlacePost(PostNW,  Half, -Half);
    PlacePost(PostSE, -Half,  Half);
    PlacePost(PostSW, -Half, -Half);
}

void ABoxingRingActor::BuildLighting()
{
    float LightHeight = 700.f;
    float Half = RingSize * 0.4f;

    auto ConfigSpot = [&](USpotLightComponent* Light,
        float LX, float LY, float Yaw)
    {
        Light->SetRelativeLocation(FVector(LX, LY, LightHeight));
        Light->SetRelativeRotation(FRotator(-80.f, Yaw, 0.f));
        Light->SetIntensity(SpotLightIntensity);
        Light->SetLightColor(SpotLightColor);
        Light->InnerConeAngle = 22.f;
        Light->OuterConeAngle = 44.f;
        Light->AttenuationRadius = 1400.f;
        Light->bUseInverseSquaredFalloff = false;
        Light->CastShadows = true;
    };

    ConfigSpot(SpotLight1, -Half, -Half, 45.f);
    ConfigSpot(SpotLight2,  Half, -Half, 135.f);
    ConfigSpot(SpotLight3,  Half,  Half, 225.f);
    ConfigSpot(SpotLight4, -Half,  Half, 315.f);
}
