#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Captivity/BDFRCaptivityTypes.h"
#include "BDFRInterrogationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FBDFROnInterrogationOutcome,
    EBDFRInterrogationApproach, Approach,
    EBDFRInterrogationOutcome, Outcome,
    int32, DisclosureLevel);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRInterrogationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRInterrogationComponent();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Interrogation")
    EBDFRInterrogationOutcome ApplyApproach(EBDFRInterrogationApproach Approach, float Intensity = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Interrogation")
    void AddEvidencePressure(float Amount);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Interrogation")
    void SetResistance(float NewResistance);

    UFUNCTION(BlueprintPure, Category = "BDFR|Interrogation")
    FBDFRInterrogationSnapshot GetSnapshot() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Interrogation")
    bool HasFullyDisclosed() const { return DisclosureLevel >= MaxDisclosureLevel; }

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Interrogation")
    FBDFROnInterrogationOutcome OnInterrogationOutcome;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Resistance = 0.60f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Rapport = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EvidencePressure = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float PsychologicalStrain = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Interrogation", meta = (ClampMin = "1", ClampMax = "10"))
    int32 MaxDisclosureLevel = 3;

private:
    UPROPERTY(Transient)
    int32 DisclosureLevel = 0;

    UPROPERTY(Transient)
    float InformationReliability = 1.0f;

    EBDFRInterrogationOutcome ResolveOutcome(
        EBDFRInterrogationApproach Approach,
        float Intensity,
        float ComplianceScore,
        float FalseStatementRisk) const;
};
