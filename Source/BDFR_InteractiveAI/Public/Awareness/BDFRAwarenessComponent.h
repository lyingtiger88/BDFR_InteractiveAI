#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/BDFRAITypes.h"
#include "BDFRAwarenessComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FBDFROnAwarenessChanged,
    AActor*, TargetActor,
    float, Awareness,
    EBDFRAwarenessLevel, AwarenessLevel);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRAwarenessComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRAwarenessComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Awareness")
    void AddAwareness(AActor* SourceActor, float Amount, const FVector& StimulusLocation, bool bConfirmTarget, bool bHasLineOfSight);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Awareness")
    void SetTargetLineOfSight(AActor* SourceActor, bool bHasLineOfSight);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Awareness")
    void ForgetTarget();

    UFUNCTION(BlueprintPure, Category = "BDFR|Awareness")
    AActor* GetCurrentTarget() const { return CurrentTarget; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Awareness")
    FBDFRAwarenessSnapshot GetSnapshot() const { return Snapshot; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Awareness")
    EBDFRAwarenessLevel GetAwarenessLevel() const;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Awareness")
    FBDFROnAwarenessChanged OnAwarenessChanged;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(Transient)
    TObjectPtr<AActor> CurrentTarget;

    UPROPERTY(Transient)
    FBDFRAwarenessSnapshot Snapshot;

    void SetAwareness(float NewAwareness);
    void BroadcastIfChanged(float PreviousAwareness, EBDFRAwarenessLevel PreviousLevel);
};
