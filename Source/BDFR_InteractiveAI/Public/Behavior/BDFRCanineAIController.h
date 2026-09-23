#pragma once

#include "CoreMinimal.h"
#include "Behavior/BDFRAIController.h"
#include "BDFRCanineAIController.generated.h"

class UBDFRCanineTrackingComponent;

UCLASS(Blueprintable)
class BDFR_INTERACTIVEAI_API ABDFRCanineAIController : public ABDFRAIController
{
    GENERATED_BODY()

public:
    ABDFRCanineAIController();

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    UBDFRCanineTrackingComponent* GetCanineTrackingComponent() const
    {
        return CanineTrackingComponent;
    }

protected:
    virtual float BDFR_GetSpeciesHearingMultiplier() const override
    {
        return CanineHearingMultiplier;
    }

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Canine")
    TObjectPtr<UBDFRCanineTrackingComponent> CanineTrackingComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine", meta = (ClampMin = "1.0"))
    float CanineHearingMultiplier = 1.75f;
};
