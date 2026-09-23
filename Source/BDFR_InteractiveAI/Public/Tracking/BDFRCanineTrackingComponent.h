#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tracking/BDFRTrackingTypes.h"
#include "BDFRCanineTrackingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnScentTrackDetected,
    FBDFRTrackSample, Sample);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRCanineTrackingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRCanineTrackingComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    bool HasScentTarget() const { return IsValid(ScentTarget); }

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    AActor* GetScentTarget() const { return ScentTarget; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    FVector GetNextScentLocation() const { return NextScentLocation; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    FBDFRTrackSample GetLastScentSample() const { return LastScentSample; }

    UFUNCTION(BlueprintCallable, Category = "BDFR|Canine")
    void ClearScentTarget();

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Canine")
    FBDFROnScentTrackDetected OnScentTrackDetected;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "0.05"))
    float ScanIntervalSeconds = 0.20f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "100.0"))
    float ScentAcquisitionRadius = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "1.0"))
    float MaxScentAgeSeconds = 240.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "100.0"))
    float ScentTrailStepRadius = 3500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinimumScentStrength = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AwarenessGainFromScent = 0.10f;

private:
    float AccumulatedTime = 0.0f;

    UPROPERTY(Transient)
    TObjectPtr<AActor> ScentTarget;

    UPROPERTY(Transient)
    FVector NextScentLocation = FVector::ZeroVector;

    UPROPERTY(Transient)
    FBDFRTrackSample LastScentSample;

    void ScanScentTrail();
    void AcceptScentSample(const FBDFRTrackSample& Sample);
};
