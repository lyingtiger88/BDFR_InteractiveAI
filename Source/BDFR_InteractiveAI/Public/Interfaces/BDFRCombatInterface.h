#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BDFRCombatInterface.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRCombatInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRCombatInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Combat")
    bool BDFR_CanEngageTarget(AActor* TargetActor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Combat")
    void BDFR_SetCombatTarget(AActor* TargetActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Combat")
    void BDFR_RequestAttack(AActor* TargetActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Combat")
    void BDFR_StopAttack();
};
