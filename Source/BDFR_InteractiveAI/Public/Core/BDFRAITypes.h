#pragma once

#include "CoreMinimal.h"
#include "BDFRAITypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRAIState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Patrol      UMETA(DisplayName = "Patrol"),
    Observe     UMETA(DisplayName = "Observe"),
    Suspicious  UMETA(DisplayName = "Suspicious"),
    Investigate UMETA(DisplayName = "Investigate"),
    Search      UMETA(DisplayName = "Search"),
    Alerted     UMETA(DisplayName = "Alerted"),
    Combat      UMETA(DisplayName = "Combat"),
    TakeCover   UMETA(DisplayName = "Take Cover"),
    Interact    UMETA(DisplayName = "Interact"),
    Disabled    UMETA(DisplayName = "Disabled"),
    Dead        UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EBDFRAwarenessLevel : uint8
{
    Unaware         UMETA(DisplayName = "Unaware"),
    Suspicious      UMETA(DisplayName = "Suspicious"),
    Investigating   UMETA(DisplayName = "Investigating"),
    Alerted         UMETA(DisplayName = "Alerted"),
    ConfirmedThreat UMETA(DisplayName = "Confirmed Threat")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRAwarenessSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Awareness")
    float Awareness = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Awareness")
    EBDFRAwarenessLevel AwarenessLevel = EBDFRAwarenessLevel::Unaware;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Awareness")
    FVector LastKnownLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Awareness")
    bool bHasConfirmedTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Awareness")
    bool bHasLineOfSight = false;
};
