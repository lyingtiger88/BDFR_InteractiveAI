#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health/BDFRHealthTypes.h"
#include "BDFRHealthComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBDFROnHealthChanged, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBDFROnHealthStateChanged, EBDFRHealthState, PreviousState, EBDFRHealthState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBDFROnWounded, AActor*, DamageCauser, float, DamageAmount, EBDFRHealthState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBDFROnBleedingChanged, bool, bBleeding);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBDFROnDeath, AActor*, DamageCauser);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRHealthComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Health")
    void ApplyHealthDamage(float DamageAmount, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Health")
    void RestoreHealth(float Amount);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Health")
    void StartBleeding(float InBleedRatePerSecond);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Health")
    void StopBleeding();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Health")
    void Stabilize(float RestoredHealth = 0.0f);

    UFUNCTION(BlueprintPure, Category = "BDFR|Health")
    FBDFRHealthSnapshot GetHealthSnapshot() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Health")
    EBDFRHealthState GetHealthState() const { return HealthState; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Health")
    float GetHealthNormalized() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Health")
    bool IsBleeding() const { return bBleeding; }

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Health")
    FBDFROnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Health")
    FBDFROnHealthStateChanged OnHealthStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Health")
    FBDFROnWounded OnWounded;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Health")
    FBDFROnBleedingChanged OnBleedingChanged;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Health")
    FBDFROnDeath OnDeath;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Health", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Health")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Health|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float WoundedThreshold = 0.65f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Health|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float CriticalThreshold = 0.30f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Health|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float IncapacitatedThreshold = 0.08f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Health")
    EBDFRHealthState HealthState = EBDFRHealthState::Healthy;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Health")
    bool bBleeding = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Health")
    float BleedRatePerSecond = 0.0f;

private:
    TWeakObjectPtr<AActor> LastDamageCauser;

    void SetHealth(float NewHealth, AActor* DamageCauser);
    void RecalculateHealthState(AActor* DamageCauser, float DamageAmount);
};
