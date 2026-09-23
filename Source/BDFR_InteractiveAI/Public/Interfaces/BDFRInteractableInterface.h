#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BDFRInteractableInterface.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRInteractableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Interaction")
    bool BDFR_CanInteract(AActor* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Interaction")
    FVector BDFR_GetInteractionLocation(AActor* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Interaction")
    void BDFR_BeginInteraction(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Interaction")
    void BDFR_EndInteraction(AActor* Interactor);
};
