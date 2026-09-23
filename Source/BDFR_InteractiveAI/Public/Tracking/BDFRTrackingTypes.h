#pragma once

#include "CoreMinimal.h"
#include "BDFRTrackingTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRTrackType : uint8
{
    Footprint UMETA(DisplayName = "Footprint"),
    Scent     UMETA(DisplayName = "Scent")
};

UENUM(BlueprintType)
enum class EBDFRTrackSurfaceType : uint8
{
    Default  UMETA(DisplayName = "Default"),
    Dirt     UMETA(DisplayName = "Dirt"),
    Mud      UMETA(DisplayName = "Mud"),
    Snow     UMETA(DisplayName = "Snow"),
    Sand     UMETA(DisplayName = "Sand"),
    Grass    UMETA(DisplayName = "Grass"),
    Concrete UMETA(DisplayName = "Concrete"),
    Metal    UMETA(DisplayName = "Metal"),
    Water    UMETA(DisplayName = "Water")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRTrackSample
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    EBDFRTrackType TrackType = EBDFRTrackType::Footprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    EBDFRTrackSurfaceType SurfaceType = EBDFRTrackSurfaceType::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    TObjectPtr<AActor> SourceActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    FVector Direction = FVector::ForwardVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    float TimeSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Strength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Tracking")
    int32 Sequence = 0;
};
