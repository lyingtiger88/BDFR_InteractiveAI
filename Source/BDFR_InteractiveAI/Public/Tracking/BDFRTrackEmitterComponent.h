#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tracking/BDFRTrackingTypes.h"
#include "BDFRTrackEmitterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnTrackSampleEmitted,
    FBDFRTrackSample, Sample);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRTrackEmitterComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRTrackEmitterComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void SetSurfaceType(EBDFRTrackSurfaceType NewSurfaceType);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void SetTrackingEmissionEnabled(bool bEnabled) { bTrackingEmissionEnabled = bEnabled; }

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void EmitFootprintNow(float StrengthOverride = -1.0f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void EmitScentNow(float StrengthOverride = -1.0f);

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Tracking")
    FBDFROnTrackSampleEmitted OnTrackSampleEmitted;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking")
    bool bTrackingEmissionEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking|Footprints", meta = (ClampMin = "10.0"))
    float FootprintSpacingCm = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking|Scent", meta = (ClampMin = "0.1"))
    float ScentIntervalSeconds = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking|Footprints", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseFootprintStrength = 0.75f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking|Scent", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseScentStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking")
    EBDFRTrackSurfaceType SurfaceType = EBDFRTrackSurfaceType::Default;

private:
    FVector LastFootprintLocation = FVector::ZeroVector;
    float ScentAccumulator = 0.0f;
    int32 FootprintSequence = 0;
    int32 ScentSequence = 0;

    float GetSurfaceFootprintMultiplier() const;
    float GetSurfaceScentMultiplier() const;
    void AddSample(EBDFRTrackType TrackType, float Strength, int32 Sequence);
};
