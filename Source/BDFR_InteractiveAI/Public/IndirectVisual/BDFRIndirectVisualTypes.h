#pragma once

#include "CoreMinimal.h"
#include "BDFRIndirectVisualTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRIndirectVisualCueType : uint8
{
    Shadow     UMETA(DisplayName = "Shadow"),
    Reflection UMETA(DisplayName = "Reflection")
};

UENUM(BlueprintType)
enum class EBDFRReflectiveSurfaceType : uint8
{
    Mirror        UMETA(DisplayName = "Mirror"),
    CalmWater     UMETA(DisplayName = "Calm Water"),
    PolishedMetal UMETA(DisplayName = "Polished Metal"),
    Custom        UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRIndirectVisualCue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Indirect Visual")
    EBDFRIndirectVisualCueType CueType = EBDFRIndirectVisualCueType::Shadow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Indirect Visual")
    TObjectPtr<AActor> SourceActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Indirect Visual")
    FVector CueLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Indirect Visual", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Strength = 0.0f;
};
