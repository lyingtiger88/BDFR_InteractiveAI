#include "Tracking/BDFRTrackingWorldSubsystem.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

void UBDFRTrackingWorldSubsystem::AddTrackSample(const FBDFRTrackSample& Sample)
{
    if (!IsValid(Sample.SourceActor))
    {
        return;
    }

    Samples.Add(Sample);

    if (Samples.Num() > 4096)
    {
        PruneExpiredTracks(300.0f);
    }
}

bool UBDFRTrackingWorldSubsystem::FindBestTrackSample(
    const FVector ObserverLocation,
    const float MaxDistance,
    const float MaxAgeSeconds,
    const bool bAllowFootprints,
    const bool bAllowScent,
    AActor* SourceFilter,
    FBDFRTrackSample& OutSample) const
{
    const UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    const float Now = World->GetTimeSeconds();
    float BestScore = -1.0f;
    bool bFound = false;

    for (const FBDFRTrackSample& Sample : Samples)
    {
        if (!IsValid(Sample.SourceActor))
        {
            continue;
        }

        if (IsValid(SourceFilter) && Sample.SourceActor != SourceFilter)
        {
            continue;
        }

        if ((Sample.TrackType == EBDFRTrackType::Footprint && !bAllowFootprints)
            || (Sample.TrackType == EBDFRTrackType::Scent && !bAllowScent))
        {
            continue;
        }

        const float Age = FMath::Max(0.0f, Now - Sample.TimeSeconds);
        if (Age > MaxAgeSeconds)
        {
            continue;
        }

        const float Distance = FVector::Distance(ObserverLocation, Sample.Location);
        if (Distance > MaxDistance)
        {
            continue;
        }

        const float AgeFactor = 1.0f - FMath::Clamp(Age / FMath::Max(MaxAgeSeconds, 1.0f), 0.0f, 1.0f);
        const float DistanceFactor = 1.0f - FMath::Clamp(Distance / FMath::Max(MaxDistance, 1.0f), 0.0f, 1.0f);
        const float Score = Sample.Strength * (0.65f * AgeFactor + 0.35f * DistanceFactor);

        if (Score > BestScore)
        {
            BestScore = Score;
            OutSample = Sample;
            bFound = true;
        }
    }

    return bFound;
}

bool UBDFRTrackingWorldSubsystem::FindNewerTrackSample(
    AActor* SourceActor,
    const EBDFRTrackType TrackType,
    const int32 AfterSequence,
    const FVector ObserverLocation,
    const float MaxDistance,
    FBDFRTrackSample& OutSample) const
{
    if (!IsValid(SourceActor))
    {
        return false;
    }

    int32 BestSequence = MAX_int32;
    bool bFound = false;

    for (const FBDFRTrackSample& Sample : Samples)
    {
        if (Sample.SourceActor != SourceActor
            || Sample.TrackType != TrackType
            || Sample.Sequence <= AfterSequence
            || Sample.Sequence >= BestSequence)
        {
            continue;
        }

        if (FVector::Distance(ObserverLocation, Sample.Location) > MaxDistance)
        {
            continue;
        }

        BestSequence = Sample.Sequence;
        OutSample = Sample;
        bFound = true;
    }

    return bFound;
}

void UBDFRTrackingWorldSubsystem::RemoveTracksForActor(AActor* SourceActor)
{
    Samples.RemoveAll(
        [SourceActor](const FBDFRTrackSample& Sample)
        {
            return !IsValid(Sample.SourceActor) || Sample.SourceActor == SourceActor;
        });
}

void UBDFRTrackingWorldSubsystem::PruneExpiredTracks(const float MaxRetainedAgeSeconds)
{
    const UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    const float Now = World->GetTimeSeconds();
    Samples.RemoveAll(
        [Now, MaxRetainedAgeSeconds](const FBDFRTrackSample& Sample)
        {
            return !IsValid(Sample.SourceActor)
                || Now - Sample.TimeSeconds > MaxRetainedAgeSeconds;
        });
}
