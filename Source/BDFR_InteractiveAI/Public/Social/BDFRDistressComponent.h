#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health/BDFRHealthTypes.h"
#include "BDFRDistressComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FBDFROnDistressEmitted,
    EBDFRDistressType, DistressType,
    float, Urgency,
    float, Radius);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRDistressComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRDistressComponent();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Distress")
    void EmitPainShout(float Urgency = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Distress")
    void RequestHelp(float Urgency = 0.8f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Distress")
    void EmitAllyDown(float Urgency = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Distress")
    void EmitBleedingOut(float Urgency = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Distress")
    void EmitDistress(EBDFRDistressType DistressType, float Urgency, float RadiusOverride = 0.0f);

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Distress")
    FBDFROnDistressEmitted OnDistressEmitted;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Distress", meta = (ClampMin = "0.0"))
    float DefaultPainRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Distress", meta = (ClampMin = "0.0"))
    float DefaultHelpRadius = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Distress", meta = (ClampMin = "0.0"))
    float DefaultEmergencyRadius = 2400.0f;

private:
    static FName GetNoiseTag(EBDFRDistressType DistressType);
    float GetDefaultRadius(EBDFRDistressType DistressType) const;
};
