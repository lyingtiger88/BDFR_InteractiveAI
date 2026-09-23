#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Acoustics/BDFRAcousticTypes.h"
#include "BDFRAcousticEventLibrary.generated.h"

UCLASS()
class BDFR_INTERACTIVEAI_API UBDFRAcousticEventLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "BDFR|Acoustics", meta = (WorldContext = "WorldContextObject"))
    static void ReportGunshot(
        UObject* WorldContextObject,
        AActor* InstigatorActor,
        FVector Location,
        bool bSuppressed = false,
        float HearingRadius = 6000.0f,
        float ExposureRadius = 900.0f,
        float PeakExposure = 0.35f);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Acoustics", meta = (WorldContext = "WorldContextObject"))
    static void ReportExplosion(
        UObject* WorldContextObject,
        AActor* InstigatorActor,
        FVector Location,
        float HearingRadius = 12000.0f,
        float BlastExposureRadius = 3500.0f,
        float PeakExposure = 1.0f,
        bool bSimulateBlastTravelTime = true,
        float PropagationSpeedCmPerSecond = 34300.0f);

    // Lightweight semantic AI-hearing event for movement, gear, handling, impacts, etc.
    // Unlike ReportGunshot / ReportExplosion this does not apply acoustic exposure.
    UFUNCTION(BlueprintCallable, Category = "BDFR|Acoustics", meta = (WorldContext = "WorldContextObject"))
    static void ReportAcousticEvent(
        UObject* WorldContextObject,
        AActor* InstigatorActor,
        FVector Location,
        float Loudness,
        float HearingRadius,
        FName EventTag);

private:
    static void ReportAcousticNoise(
        UObject* WorldContextObject,
        AActor* InstigatorActor,
        FVector Location,
        float Loudness,
        float HearingRadius,
        FName Tag);

    static void ApplyRadialExposure(
        UObject* WorldContextObject,
        FVector Location,
        float Radius,
        float PeakExposure,
        EBDFRAcousticEventType EventType,
        bool bUseTravelTime,
        float PropagationSpeedCmPerSecond);
};
