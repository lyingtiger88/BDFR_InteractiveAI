#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health/BDFRHealthTypes.h"
#include "BDFRInjuryResponseComponent.generated.h"

class AActor;
class UBDFRDistressComponent;
class UBDFRHealthComponent;

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRInjuryResponseComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRInjuryResponseComponent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Injury Response")
    bool bAutoPainShout = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Injury Response")
    bool bAutoRequestHelp = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Injury Response", meta = (ClampMin = "0.0"))
    float PainShoutCooldown = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Injury Response", meta = (ClampMin = "0.0"))
    float HelpRequestCooldown = 5.0f;

private:
    UPROPERTY(Transient)
    TObjectPtr<UBDFRHealthComponent> HealthComponent;

    UPROPERTY(Transient)
    TObjectPtr<UBDFRDistressComponent> DistressComponent;

    float LastPainShoutTime = -1000.0f;
    float LastHelpRequestTime = -1000.0f;

    UFUNCTION()
    void HandleWounded(AActor* DamageCauser, float DamageAmount, EBDFRHealthState NewState);

    UFUNCTION()
    void HandleHealthStateChanged(EBDFRHealthState PreviousState, EBDFRHealthState NewState);

    UFUNCTION()
    void HandleBleedingChanged(bool bBleeding);

    bool CanEmit(float LastTime, float Cooldown) const;
    void TryRequestHelp(float Urgency);
};
