#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health/BDFRHealthTypes.h"
#include "BDFRStressComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBDFROnStressChanged, float, Stress, EBDFRStressState, StressState);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRStressComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRStressComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Stress")
    void AddStress(float Amount);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Stress")
    void ReduceStress(float Amount);

    UFUNCTION(BlueprintPure, Category = "BDFR|Stress")
    float GetStress() const { return Stress; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Stress")
    EBDFRStressState GetStressState() const;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Stress")
    FBDFROnStressChanged OnStressChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Stress", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Stress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Stress", meta = (ClampMin = "0.0"))
    float StressDecayPerSecond = 0.025f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Stress|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ConcernedThreshold = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Stress|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float StressedThreshold = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Stress|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float PanickedThreshold = 0.85f;

private:
    void SetStress(float NewStress);
};
