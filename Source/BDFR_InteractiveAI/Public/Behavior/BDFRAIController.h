#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BDFRAIController.generated.h"

class UBDFRAwarenessComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;

UCLASS(Blueprintable)
class BDFR_INTERACTIVEAI_API ABDFRAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABDFRAIController();

    UFUNCTION(BlueprintPure, Category = "BDFR|AI")
    UBDFRAwarenessComponent* GetAwarenessComponent() const { return AwarenessComponent; }

    UFUNCTION(BlueprintPure, Category = "BDFR|AI")
    UAIPerceptionComponent* GetBDFRPerceptionComponent() const { return BDFRPerceptionComponent; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus);

    UFUNCTION(BlueprintNativeEvent, Category = "BDFR|AI")
    bool BDFR_ShouldProcessPerceivedActor(AActor* SourceActor) const;

    virtual bool BDFR_ShouldProcessPerceivedActor_Implementation(AActor* SourceActor) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|AI")
    TObjectPtr<UBDFRAwarenessComponent> AwarenessComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|AI")
    TObjectPtr<UAIPerceptionComponent> BDFRPerceptionComponent;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Damage> DamageConfig;
};
