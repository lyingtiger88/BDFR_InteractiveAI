#pragma once

#include "CoreMinimal.h"
#include "BDFRDifficultyTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRDifficultyTier : uint8
{
    Recruit  UMETA(DisplayName = "Recruit"),
    Private  UMETA(DisplayName = "Private"),
    Sergeant UMETA(DisplayName = "Sergeant"),
    Commando UMETA(DisplayName = "Commando"),
    SAS      UMETA(DisplayName = "SAS")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRDifficultyProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty")
    EBDFRDifficultyTier Tier = EBDFRDifficultyTier::Private;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float SightRadiusMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float HearingSensitivityMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float AwarenessGainMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.0"))
    float ReactionDelaySeconds = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.0"))
    float SearchPersistenceSeconds = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float StressResistance = 0.45f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AssistanceBravery = 0.50f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SurrenderResistance = 0.50f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float AcousticRecoveryMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float CombatAccuracyMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty", meta = (ClampMin = "0.1"))
    float CoverDecisionMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty")
    bool bPersistentHuntAfterConfirmedTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Indirect Visual")
    bool bCanDetectShadows = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Indirect Visual")
    bool bCanDetectReflections = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Indirect Visual", meta = (ClampMin = "0.1"))
    float IndirectVisualAwarenessMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Tracking")
    bool bCanTrackFootprints = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Tracking", meta = (ClampMin = "0.1"))
    float FootprintTrackingSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Difficulty|Tracking", meta = (ClampMin = "1.0"))
    float MaxFootprintAgeSeconds = 60.0f;
};
