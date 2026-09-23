#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tracking/BDFRTrackingTypes.h"
#include "BDFRTrackingWorldSubsystem.generated.h"

UCLASS()
class BDFR_INTERACTIVEAI_API UBDFRTrackingWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void AddTrackSample(const FBDFRTrackSample& Sample);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    bool FindBestTrackSample(
        FVector ObserverLocation,
        float MaxDistance,
        float MaxAgeSeconds,
        bool bAllowFootprints,
        bool bAllowScent,
        AActor* SourceFilter,
        FBDFRTrackSample& OutSample) const;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    bool FindNewerTrackSample(
        AActor* SourceActor,
        EBDFRTrackType TrackType,
        int32 AfterSequence,
        FVector ObserverLocation,
        float MaxDistance,
        FBDFRTrackSample& OutSample) const;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void RemoveTracksForActor(AActor* SourceActor);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Tracking")
    void PruneExpiredTracks(float MaxRetainedAgeSeconds = 300.0f);

private:
    UPROPERTY(Transient)
    TArray<FBDFRTrackSample> Samples;
};
