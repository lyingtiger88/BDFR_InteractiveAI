#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDFRIndirectVisualSourceComponent.generated.h"

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRIndirectVisualSourceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRIndirectVisualSourceComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Indirect Visual|Shadow")
    void SetShadowCue(FVector InShadowLocation, float InStrength = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Indirect Visual|Shadow")
    void ClearShadowCue();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Indirect Visual|Shadow")
    bool EstimateShadowCueFromLightDirection(
        FVector LightTravelDirection,
        float TraceDistance = 5000.0f);

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual|Shadow")
    bool HasValidShadowCue() const { return bShadowCueValid && bCanProduceShadowCue; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual|Shadow")
    FVector GetShadowCueLocation() const { return ShadowCueLocation; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual|Shadow")
    float GetShadowCueStrength() const { return ShadowCueStrength; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Indirect Visual")
    bool CanProduceReflectionCue() const { return bCanProduceReflectionCue; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual")
    bool bCanProduceShadowCue = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual")
    bool bCanProduceReflectionCue = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Indirect Visual|Shadow", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseShadowStrength = 0.75f;

private:
    UPROPERTY(Transient)
    bool bShadowCueValid = false;

    UPROPERTY(Transient)
    FVector ShadowCueLocation = FVector::ZeroVector;

    UPROPERTY(Transient)
    float ShadowCueStrength = 0.0f;
};
