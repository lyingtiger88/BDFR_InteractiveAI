#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BDFRAISettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "BDFR Interactive AI"))
class BDFR_INTERACTIVEAI_API UBDFRAISettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Optional Systems")
    bool bEnableCaptivitySystem = true;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Optional Systems")
    bool bEnableInterrogationSystem = true;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Optional Systems")
    bool bEnableCoerciveInterrogation = false;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Awareness", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SuspiciousThreshold = 0.25f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Awareness", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float InvestigateThreshold = 0.50f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Awareness", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AlertThreshold = 0.75f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Awareness", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ConfirmedThreatThreshold = 1.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Awareness", meta = (ClampMin = "0.0"))
    float AwarenessDecayPerSecond = 0.08f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Sight", meta = (ClampMin = "0.0"))
    float DefaultSightRadius = 2500.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Sight", meta = (ClampMin = "0.0"))
    float DefaultLoseSightRadius = 3200.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Sight", meta = (ClampMin = "0.0", ClampMax = "180.0"))
    float DefaultPeripheralVisionHalfAngle = 70.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Sight", meta = (ClampMin = "0.0"))
    float DefaultSightMaxAge = 4.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Hearing", meta = (ClampMin = "0.0"))
    float DefaultHearingRange = 15000.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Hearing", meta = (ClampMin = "0.0"))
    float DefaultHearingMaxAge = 5.0f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Awareness Gain", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SightAwarenessGain = 0.60f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Awareness Gain", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HearingAwarenessGain = 0.25f;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Perception|Awareness Gain", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DamageAwarenessGain = 1.0f;
};
