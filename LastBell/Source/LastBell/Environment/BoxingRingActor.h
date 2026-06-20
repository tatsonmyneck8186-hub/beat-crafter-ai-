#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxingRingActor.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class USpotLightComponent;

UCLASS()
class LASTBELL_API ABoxingRingActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxingRingActor();

protected:
    virtual void BeginPlay() override;

public:
    // ─── Structural Mesh ───────────────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> FloorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> RopeNorth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> RopeSouth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> RopeEast;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> RopeWest;

    // Corner posts
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> PostNE;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> PostNW;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> PostSE;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ring")
    TObjectPtr<UStaticMeshComponent> PostSW;

    // ─── Lighting ─────────────────────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
    TObjectPtr<USpotLightComponent> SpotLight1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
    TObjectPtr<USpotLightComponent> SpotLight2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
    TObjectPtr<USpotLightComponent> SpotLight3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
    TObjectPtr<USpotLightComponent> SpotLight4;

    // ─── Config ─────────────────────────────────────────────────────────────────

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
    float RingSize = 600.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
    float RopeHeight = 120.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
    FLinearColor CanvasColor = FLinearColor(0.9f, 0.85f, 0.7f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
    FLinearColor RopeColor = FLinearColor(0.8f, 0.1f, 0.1f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    float SpotLightIntensity = 8000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    FLinearColor SpotLightColor = FLinearColor(1.f, 0.95f, 0.85f);

private:
    void BuildRing();
    void BuildLighting();
    UStaticMeshComponent* CreateBoxMesh(
        const FString& Name, const FVector& Location,
        const FVector& Scale, const FLinearColor& Color);
};
