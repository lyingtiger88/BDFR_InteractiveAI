#include "Acoustics/BDFRAcousticEventLibrary.h"

#include "Acoustics/BDFRAcousticExposureComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Perception/AISense_Hearing.h"
#include "TimerManager.h"

void UBDFRAcousticEventLibrary::ReportGunshot(
    UObject* WorldContextObject,
    AActor* InstigatorActor,
    const FVector Location,
    const bool bSuppressed,
    const float HearingRadius,
    const float ExposureRadius,
    const float PeakExposure)
{
    const float Loudness = bSuppressed ? 0.35f : 1.0f;
    const FName Tag = bSuppressed
        ? TEXT("BDFR.Acoustic.Gunshot.Suppressed")
        : TEXT("BDFR.Acoustic.Gunshot");

    ReportAcousticNoise(
        WorldContextObject,
        InstigatorActor,
        Location,
        Loudness,
        HearingRadius,
        Tag);

    ApplyRadialExposure(
        WorldContextObject,
        Location,
        ExposureRadius,
        bSuppressed ? PeakExposure * 0.35f : PeakExposure,
        bSuppressed ? EBDFRAcousticEventType::SuppressedGunshot : EBDFRAcousticEventType::Gunshot,
        false,
        0.0f);
}

void UBDFRAcousticEventLibrary::ReportExplosion(
    UObject* WorldContextObject,
    AActor* InstigatorActor,
    const FVector Location,
    const float HearingRadius,
    const float BlastExposureRadius,
    const float PeakExposure,
    const bool bSimulateBlastTravelTime,
    const float PropagationSpeedCmPerSecond)
{
    ReportAcousticNoise(
        WorldContextObject,
        InstigatorActor,
        Location,
        1.0f,
        HearingRadius,
        TEXT("BDFR.Acoustic.Explosion"));

    ApplyRadialExposure(
        WorldContextObject,
        Location,
        BlastExposureRadius,
        PeakExposure,
        EBDFRAcousticEventType::BlastWave,
        bSimulateBlastTravelTime,
        PropagationSpeedCmPerSecond);
}

void UBDFRAcousticEventLibrary::ReportAcousticNoise(
    UObject* WorldContextObject,
    AActor* InstigatorActor,
    const FVector Location,
    const float Loudness,
    const float HearingRadius,
    const FName Tag)
{
    const float SafeLoudness = FMath::Max(0.05f, Loudness);
    const float CompensatedMaxRange =
        HearingRadius > 0.0f ? HearingRadius / SafeLoudness : 0.0f;

    UAISense_Hearing::ReportNoiseEvent(
        WorldContextObject,
        Location,
        SafeLoudness,
        InstigatorActor,
        CompensatedMaxRange,
        Tag);
}

void UBDFRAcousticEventLibrary::ApplyRadialExposure(
    UObject* WorldContextObject,
    const FVector Location,
    const float Radius,
    const float PeakExposure,
    const EBDFRAcousticEventType EventType,
    const bool bUseTravelTime,
    const float PropagationSpeedCmPerSecond)
{
    if (!IsValid(WorldContextObject) || Radius <= 0.0f || PeakExposure <= 0.0f)
    {
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    TArray<FOverlapResult> Overlaps;
    FCollisionObjectQueryParams ObjectQuery;
    ObjectQuery.AddObjectTypesToQuery(ECC_Pawn);

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BDFRAcousticExposure), false);

    const bool bAnyOverlap = World->OverlapMultiByObjectType(
        Overlaps,
        Location,
        FQuat::Identity,
        ObjectQuery,
        FCollisionShape::MakeSphere(Radius),
        QueryParams);

    if (!bAnyOverlap)
    {
        return;
    }

    TSet<TObjectPtr<AActor>> ProcessedActors;

    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* Actor = Result.GetActor();
        if (!IsValid(Actor) || ProcessedActors.Contains(Actor))
        {
            continue;
        }

        ProcessedActors.Add(Actor);

        UBDFRAcousticExposureComponent* ExposureComponent =
            Actor->FindComponentByClass<UBDFRAcousticExposureComponent>();
        if (!IsValid(ExposureComponent))
        {
            continue;
        }

        const float Distance = FVector::Distance(Location, Actor->GetActorLocation());
        const float NormalizedDistance = FMath::Clamp(Distance / Radius, 0.0f, 1.0f);
        const float Falloff = FMath::Square(1.0f - NormalizedDistance);
        const float EffectiveExposure = FMath::Clamp(PeakExposure * Falloff, 0.0f, 1.0f);

        if (EffectiveExposure <= KINDA_SMALL_NUMBER)
        {
            continue;
        }

        const float Delay =
            bUseTravelTime && PropagationSpeedCmPerSecond > KINDA_SMALL_NUMBER
                ? Distance / PropagationSpeedCmPerSecond
                : 0.0f;

        if (Delay <= KINDA_SMALL_NUMBER)
        {
            ExposureComponent->ApplyExposure(EffectiveExposure, Location, EventType);
            continue;
        }

        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
            ExposureComponent,
            &UBDFRAcousticExposureComponent::ApplyExposure,
            EffectiveExposure,
            Location,
            EventType);

        World->GetTimerManager().SetTimer(
            TimerHandle,
            TimerDelegate,
            Delay,
            false);
    }
}
