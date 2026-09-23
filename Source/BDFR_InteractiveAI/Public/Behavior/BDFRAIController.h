#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BDFRAIController.generated.h"

class UBDFRAcousticExposureComponent;
class UBDFRAwarenessComponent;
class UBDFRStressComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
    FBDFROnDistressPerceived,
    AActor*, SourceActor,
    FName, DistressTag,
    FVector, Location,
    float, Urgency);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
    FBDFROnAcousticEventPerceived,
    AActor*, SourceActor,
    FName, AcousticTag,
    FVector, Location,
    float, EffectiveStrength);

UCLASS(Blueprintable)
class BDFR_INTERACTIVEAI_API ABDFRAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABDFRAIController();

    UFUNCTION(BlueprintPure, Category = "BDFR|AI")
    UBDFRAwarenessComponent* GetAwarenessComponent() const { return AwarenessComponent; }

    UFUNCTION(BlueprintPure, Category = "BDFR|AI")
    UBDFRStressComponent* GetStressComponent() const { return StressComponent; }

    UFUNCTION(BlueprintPure, Category = "BDFR|AI")
    UAIPerceptionComponent* GetBDFRPerceptionComponent() const { return BDFRPerceptionComponent; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    UBDFRAcousticExposureComponent* GetAcousticExposureComponent() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    FVector GetLastHeardAcousticLocation() const { return LastHeardAcousticLocation; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    FName GetLastHeardAcousticTag() const { return LastHeardAcousticTag; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    float GetLastHeardAcousticStrength() const { return LastHeardAcousticStrength; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Assistance")
    AActor* GetPendingAssistanceTarget() const { return PendingAssistanceTarget; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Assistance")
    FVector GetPendingAssistanceLocation() const { return PendingAssistanceLocation; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Assistance")
    float GetPendingAssistanceUrgency() const { return PendingAssistanceUrgency; }

    UFUNCTION(BlueprintCallable, Category = "BDFR|Assistance")
    void ClearPendingAssistance();

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Social")
    FBDFROnDistressPerceived OnDistressPerceived;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Acoustics")
    FBDFROnAcousticEventPerceived OnAcousticEventPerceived;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus);

    UFUNCTION(BlueprintNativeEvent, Category = "BDFR|AI")
    bool BDFR_ShouldProcessPerceivedActor(AActor* SourceActor) const;

    virtual bool BDFR_ShouldProcessPerceivedActor_Implementation(AActor* SourceActor) const;

    UFUNCTION(BlueprintNativeEvent, Category = "BDFR|Social")
    bool BDFR_ShouldRespondToDistress(AActor* SourceActor) const;

    virtual bool BDFR_ShouldRespondToDistress_Implementation(AActor* SourceActor) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|AI")
    TObjectPtr<UBDFRAwarenessComponent> AwarenessComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Social")
    TObjectPtr<UBDFRStressComponent> StressComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|AI")
    TObjectPtr<UAIPerceptionComponent> BDFRPerceptionComponent;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Damage> DamageConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Assistance")
    TObjectPtr<AActor> PendingAssistanceTarget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Assistance")
    FVector PendingAssistanceLocation = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Assistance")
    float PendingAssistanceUrgency = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics")
    FVector LastHeardAcousticLocation = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics")
    FName LastHeardAcousticTag = NAME_None;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics")
    float LastHeardAcousticStrength = 0.0f;

private:
    float GetCurrentHearingSensitivity() const;
    bool IsDistressStimulus(const FAIStimulus& Stimulus) const;
    bool IsAcousticStimulus(const FAIStimulus& Stimulus) const;
    void HandleDistressStimulus(AActor* SourceActor, const FAIStimulus& Stimulus);
    void HandleAcousticStimulus(AActor* SourceActor, const FAIStimulus& Stimulus);
    static float GetDistressStressAmount(FName DistressTag, float Urgency);
};
