#pragma once

#include "CoreMinimal.h"
#include "Canine/BDFRCanineTypes.h"
#include "UObject/Interface.h"
#include "BDFRCaninePresentationInterface.generated.h"

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRCaninePresentationInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRCaninePresentationInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Canine|Presentation")
    void BDFR_UpdateCanineAttention(const FBDFRCanineAttentionSnapshot& Snapshot);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Canine|Presentation")
    void BDFR_PlayCanineEngage(AActor* TargetActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Canine|Presentation")
    void BDFR_PlayCanineBark(FName ReasonTag);
};
