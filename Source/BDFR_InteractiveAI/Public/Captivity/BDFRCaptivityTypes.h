#pragma once

#include "CoreMinimal.h"
#include "BDFRCaptivityTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRCaptivityState : uint8
{
    Free          UMETA(DisplayName = "Free"),
    Surrendering  UMETA(DisplayName = "Surrendering"),
    Restrained    UMETA(DisplayName = "Restrained"),
    Hostage       UMETA(DisplayName = "Hostage"),
    Interrogating UMETA(DisplayName = "Interrogating"),
    Escaping      UMETA(DisplayName = "Escaping"),
    Released      UMETA(DisplayName = "Released")
};

UENUM(BlueprintType)
enum class EBDFRInterrogationApproach : uint8
{
    Question          UMETA(DisplayName = "Question"),
    PresentEvidence   UMETA(DisplayName = "Present Evidence"),
    Bargain           UMETA(DisplayName = "Bargain"),
    Intimidate        UMETA(DisplayName = "Intimidate"),
    CoercivePressure  UMETA(DisplayName = "Coercive Pressure")
};

UENUM(BlueprintType)
enum class EBDFRInterrogationOutcome : uint8
{
    NoResponse        UMETA(DisplayName = "No Response"),
    Refusal           UMETA(DisplayName = "Refusal"),
    PartialDisclosure UMETA(DisplayName = "Partial Disclosure"),
    FullDisclosure    UMETA(DisplayName = "Full Disclosure"),
    FalseStatement    UMETA(DisplayName = "False Statement"),
    Breakdown         UMETA(DisplayName = "Breakdown")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRCaptivitySnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Captivity")
    EBDFRCaptivityState State = EBDFRCaptivityState::Free;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Captivity")
    TObjectPtr<AActor> Captor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Captivity")
    bool bRestrained = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Captivity")
    bool bCanEscape = false;
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRInterrogationSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Resistance = 0.60f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Rapport = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EvidencePressure = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float PsychologicalStrain = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float InformationReliability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Interrogation")
    int32 DisclosureLevel = 0;
};
