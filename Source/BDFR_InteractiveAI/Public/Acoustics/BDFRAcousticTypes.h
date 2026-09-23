#pragma once

#include "CoreMinimal.h"
#include "BDFRAcousticTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRAcousticEventType : uint8
{
    Gunshot           UMETA(DisplayName = "Gunshot"),
    SuppressedGunshot UMETA(DisplayName = "Suppressed Gunshot"),
    Explosion         UMETA(DisplayName = "Explosion"),
    BlastWave         UMETA(DisplayName = "Blast Wave"),
    Impact            UMETA(DisplayName = "Impact")
};

UENUM(BlueprintType)
enum class EBDFRHearingState : uint8
{
    Normal   UMETA(DisplayName = "Normal"),
    Ringing  UMETA(DisplayName = "Ringing"),
    Impaired UMETA(DisplayName = "Impaired"),
    Deafened UMETA(DisplayName = "Temporarily Deafened")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRAcousticExposureSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    float Exposure = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    float HearingSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    EBDFRHearingState HearingState = EBDFRHearingState::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    FVector LastExposureLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    EBDFRAcousticEventType LastEventType = EBDFRAcousticEventType::Impact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Acoustics")
    bool bHearingSenseDisabled = false;
};
