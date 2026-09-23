#pragma once

#include "CoreMinimal.h"
#include "Captivity/BDFRCaptivityTypes.h"
#include "UObject/Interface.h"
#include "BDFRCaptivityPresentationInterface.generated.h"

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRCaptivityPresentationInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRCaptivityPresentationInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Captivity")
    void BDFR_OnCaptivityStatePresentation(EBDFRCaptivityState NewState);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Interrogation")
    void BDFR_OnInterrogationPresentation(
        EBDFRInterrogationApproach Approach,
        EBDFRInterrogationOutcome Outcome);
};
