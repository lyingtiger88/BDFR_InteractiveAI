#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "IndirectVisual/BDFRIndirectVisualTypes.h"
#include "BDFRReflectiveSurfaceComponent.generated.h"

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRReflectiveSurfaceComponent : public UBoxComponent
{
    GENERATED_BODY()

public:
    UBDFRReflectiveSurfaceComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Indirect Visual|Reflection")
    bool TryCalculateReflectionCue(
        FVector ViewerLocation,
        AActor* SourceActor,
        FVector& OutCueLocation,
        float& OutStrength) const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual|Reflection")
    EBDFRReflectiveSurfaceType GetSurfaceType() const { return SurfaceType; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Reflection")
    EBDFRReflectiveSurfaceType SurfaceType = EBDFRReflectiveSurfaceType::Mirror;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Reflection", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ReflectionStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Reflection", meta = (ClampMin = "100.0"))
    float MaxDetectionPathLength = 7000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Reflection")
    FVector LocalSurfaceNormal = FVector::UpVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Reflection")
    bool bRequireClearSourceToSurfacePath = true;

private:
    bool IsPointInsideSurfaceBounds(const FVector& WorldPoint) const;
};
