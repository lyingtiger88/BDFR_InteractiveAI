#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "BDFRLocomotionInterface.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class BDFR_INTERACTIVEAI_API UBDFRLocomotionInterface : public UInterface
{
    GENERATED_BODY()
};

class BDFR_INTERACTIVEAI_API IBDFRLocomotionInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Locomotion")
    void BDFR_SetDesiredGait(FGameplayTag GaitTag);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Locomotion")
    void BDFR_SetDesiredStance(FGameplayTag StanceTag);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Locomotion")
    void BDFR_SetAiming(bool bAiming);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Locomotion")
    void BDFR_SetLookTarget(AActor* TargetActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BDFR|Locomotion")
    void BDFR_ClearLookTarget();
};
