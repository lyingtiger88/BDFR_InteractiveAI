#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tracking/BDFRTrackingTypes.h"
#include "BDFRFootprintTrackingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnFootprintTrackDetected,
    FBDFRTrackSample, Sample);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRFootprintTrackingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRFootprintTrackingComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure, Category = "BDFR|Tracking")
    bool HasActiveFootprintTrail() const { return IsValid(TrackedActor); }

    UFUNCTION(BlueprintPure, Category = "BDFR|Tracking")
    AActor* GetTrackedActor() const { return TrackedActor; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Tracking")
    FVector GetNextTrackLocation() const { return NextTrackLocation; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Tracking")
    FBDFRTrackSample GetLastDetectedSample() const { return LastDetectedSample; }

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void ClearFootprintTrail();

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Tracking")
    FBDFROnFootprintTrackDetected OnFootprintTrackDetected;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "0.05"))
    float ScanIntervalSeconds = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "50.0"))
    float BaseDetectionRadius = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "0.0", ClampMax = "180.0"))
    float DetectionHalfAngleDegrees = 75.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinimumTrackStrength = 0.12f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "50.0"))
    float TrailStepSearchRadius = 1600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Tracking", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AwarenessGainOnTrack = 0.07f;

private:
    float AccumulatedTime = 0.0f;

    UPROPERTY(Transient)
    TObjectPtr<AActor> TrackedActor;

    UPROPERTY(Transient)
    FVector NextTrackLocation = FVector::ZeroVector;

    UPROPERTY(Transient)
    FBDFRTrackSample LastDetectedSample;

    void ScanForFootprints();
    bool IsSampleVisibleAndInView(const FBDFRTrackSample& Sample, const FVector& ViewerLocation, const FVector& ViewerForward) const;
    void AcceptSample(const FBDFRTrackSample& Sample);
};
