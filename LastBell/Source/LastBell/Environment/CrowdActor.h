#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowdActor.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class LASTBELL_API ACrowdActor : public AActor
{
    GENERATED_BODY()

public:
    ACrowdActor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crowd")
    TObjectPtr<UInstancedStaticMeshComponent> CrowdMeshes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    int32 CrowdCount = 120;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    float ArenaRadius = 1200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    float CrowdHeight = 0.f;

    UFUNCTION(BlueprintCallable, Category = "Crowd")
    void SpawnCrowd();

    UFUNCTION(BlueprintCallable, Category = "Crowd")
    void AnimateCheering();
};
