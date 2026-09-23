#include "Tracking/BDFRFootprintTrackingComponent.h"

#include "AIController.h"
#include "Awareness/BDFRAwarenessComponent.h"
#include "Difficulty/BDFRDifficultyComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Tracking/BDFRTrackingWorldSubsystem.h"

UBDFRFootprintTrackingComponent::UBDFRFootprintTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRFootprintTrackingComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AccumulatedTime += DeltaTime;
    if (AccumulatedTime < ScanIntervalSeconds)
    {
        return;
    }

    AccumulatedTime = 0.0f;
    ScanForFootprints();
}

void UBDFRFootprintTrackingComponent::ClearFootprintTrail()
{
    TrackedActor = nullptr;
    NextTrackLocation = FVector::ZeroVector;
    LastDetectedSample = FBDFRTrackSample();
}

void UBDFRFootprintTrackingComponent::ScanForFootprints()
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    APawn* Pawn = IsValid(Controller) ? Controller->GetPawn() : nullptr;
    UWorld* World = GetWorld();

    if (!IsValid(Controller) || !IsValid(Pawn) || !IsValid(World))
    {
        return;
    }

    UBDFRDifficultyComponent* Difficulty =
        Controller->FindComponentByClass<UBDFRDifficultyComponent>();
    UBDFRAwarenessComponent* Awareness =
        Controller->FindComponentByClass<UBDFRAwarenessComponent>();

    if (!IsValid(Difficulty) || !IsValid(Awareness))
    {
        return;
    }

    const FBDFRDifficultyProfile Profile = Difficulty->GetDifficultyProfile();
    if (!Profile.bCanTrackFootprints)
    {
        ClearFootprintTrail();
        return;
    }

    UBDFRTrackingWorldSubsystem* Tracking =
        World->GetSubsystem<UBDFRTrackingWorldSubsystem>();
    if (!IsValid(Tracking))
    {
        return;
    }

    FVector ViewerLocation;
    FRotator ViewerRotation;
    Pawn->GetActorEyesViewPoint(ViewerLocation, ViewerRotation);
    const FVector ViewerForward = ViewerRotation.Vector();

    FBDFRTrackSample Candidate;

    if (IsValid(TrackedActor))
    {
        if (Tracking->FindNewerTrackSample(
            TrackedActor,
            EBDFRTrackType::Footprint,
            LastDetectedSample.Sequence,
            Pawn->GetActorLocation(),
            TrailStepSearchRadius * Profile.FootprintTrackingSensitivity,
            Candidate)
            && Candidate.Strength >= MinimumTrackStrength
            && IsSampleVisibleAndInView(Candidate, ViewerLocation, ViewerForward))
        {
            AcceptSample(Candidate);
            return;
        }
    }

    if (Tracking->FindBestTrackSample(
        Pawn->GetActorLocation(),
        BaseDetectionRadius * Profile.FootprintTrackingSensitivity,
        Profile.MaxFootprintAgeSeconds,
        true,
        false,
        nullptr,
        Candidate)
        && Candidate.Strength >= MinimumTrackStrength
        && IsSampleVisibleAndInView(Candidate, ViewerLocation, ViewerForward))
    {
        AcceptSample(Candidate);
    }
}

bool UBDFRFootprintTrackingComponent::IsSampleVisibleAndInView(
    const FBDFRTrackSample& Sample,
    const FVector& ViewerLocation,
    const FVector& ViewerForward) const
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    const FVector ToSample = (Sample.Location - ViewerLocation).GetSafeNormal();
    const float MinimumDot = FMath::Cos(FMath::DegreesToRadians(DetectionHalfAngleDegrees));

    if (FVector::DotProduct(ViewerForward, ToSample) < MinimumDot)
    {
        return false;
    }

    FCollisionQueryParams Params(SCENE_QUERY_STAT(BDFRFootprintLOS), false);
    Params.AddIgnoredActor(GetOwner());
    if (const AAIController* Controller = Cast<AAIController>(GetOwner()))
    {
        Params.AddIgnoredActor(Controller->GetPawn());
    }

    FHitResult Hit;
    return !World->LineTraceSingleByChannel(
        Hit,
        ViewerLocation,
        Sample.Location + FVector(0.0f, 0.0f, 8.0f),
        ECC_Visibility,
        Params);
}

void UBDFRFootprintTrackingComponent::AcceptSample(const FBDFRTrackSample& Sample)
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!IsValid(Controller) || !IsValid(Sample.SourceActor))
    {
        return;
    }

    UBDFRDifficultyComponent* Difficulty =
        Controller->FindComponentByClass<UBDFRDifficultyComponent>();
    UBDFRAwarenessComponent* Awareness =
        Controller->FindComponentByClass<UBDFRAwarenessComponent>();

    if (!IsValid(Difficulty) || !IsValid(Awareness))
    {
        return;
    }

    const FBDFRDifficultyProfile Profile = Difficulty->GetDifficultyProfile();

    TrackedActor = Sample.SourceActor;
    LastDetectedSample = Sample;
    NextTrackLocation = Sample.Location + Sample.Direction.GetSafeNormal2D() * 120.0f;

    Awareness->AddAwareness(
        Sample.SourceActor,
        FMath::Clamp(
            AwarenessGainOnTrack
            * Sample.Strength
            * Profile.FootprintTrackingSensitivity,
            0.0f,
            1.0f),
        Sample.Location,
        false,
        false);

    OnFootprintTrackDetected.Broadcast(Sample);
}
