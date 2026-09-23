#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IndirectVisual/BDFRIndirectVisualTypes.h"
#include "BDFRIndirectVisualPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnIndirectVisualCueDetected,
    FBDFRIndirectVisualCue, Cue);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRIndirectVisualPerceptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRIndirectVisualPerceptionComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual")
    FBDFRIndirectVisualCue GetLastCue() const { return LastCue; }

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Indirect Visual")
    FBDFROnIndirectVisualCueDetected OnIndirectVisualCueDetected;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.05"))
    float ScanIntervalSeconds = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0"))
    float ShadowDetectionRadius = 3500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0"))
    float ReflectionDetectionRadius = 6000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ShadowBaseAwarenessGain = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ReflectionBaseAwarenessGain = 0.10f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinimumCueStrength = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.1"))
    float PerSourceCueCooldownSeconds = 1.0f;

private:
    UPROPERTY(Transient)
    FBDFRIndirectVisualCue LastCue;

    float AccumulatedTime = 0.0f;
    TMap<TWeakObjectPtr<AActor>, float> LastCueTimeBySource;

    void ScanForIndirectVisualCues();
    bool IsSourceDirectlyVisible(AActor* SourceActor, const FVector& ViewerLocation) const;
    bool IsPointVisible(const FVector& ViewerLocation, const FVector& Point, AActor* IgnoreActor) const;
    void HandleDetectedCue(const FBDFRIndirectVisualCue& Cue, float BaseAwarenessGain);
};
