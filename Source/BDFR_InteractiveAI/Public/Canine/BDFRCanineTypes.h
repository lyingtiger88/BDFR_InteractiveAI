#pragma once

#include "CoreMinimal.h"
#include "BDFRCanineTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRCanineAttentionState : uint8
{
    Relaxed      UMETA(DisplayName = "Relaxed"),
    Scanning     UMETA(DisplayName = "Scanning"),
    Listening    UMETA(DisplayName = "Listening"),
    Tracking     UMETA(DisplayName = "Tracking"),
    Alerted      UMETA(DisplayName = "Alerted"),
    Engaging     UMETA(DisplayName = "Engaging")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRCanineAttentionSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    EBDFRCanineAttentionState State = EBDFRCanineAttentionState::Relaxed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    FVector FocusLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    bool bHasFocus = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    float EarYawDegrees = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    float EarPitchDegrees = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    float HeadYawDegrees = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    float HeadPitchDegrees = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Alertness = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Canine")
    bool bRequestEngageAnimation = false;
};
