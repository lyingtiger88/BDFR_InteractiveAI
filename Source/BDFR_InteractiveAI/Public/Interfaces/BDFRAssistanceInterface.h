#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BDFRAssistanceInterface.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRAssistanceInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRAssistanceInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Assistance")
    bool BDFR_CanReceiveAssistance(AActor* Helper) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Assistance")
    FVector BDFR_GetAssistanceLocation(AActor* Helper) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Assistance")
    void BDFR_BeginAssistance(AActor* Helper);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Assistance")
    void BDFR_CompleteAssistance(AActor* Helper);
};
