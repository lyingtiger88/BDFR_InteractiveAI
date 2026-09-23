#include "Tracking/BDFRCanineTrackingComponent.h"

#include "AIController.h"
#include "Awareness/BDFRAwarenessComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Tracking/BDFRTrackingWorldSubsystem.h"

UBDFRCanineTrackingComponent::UBDFRCanineTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRCanineTrackingComponent::TickComponent(
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
    ScanScentTrail();
}

void UBDFRCanineTrackingComponent::ClearScentTarget()
{
    ScentTarget = nullptr;
    NextScentLocation = FVector::ZeroVector;
    LastScentSample = FBDFRTrackSample();
}

void UBDFRCanineTrackingComponent::ScanScentTrail()
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    APawn* Pawn = IsValid(Controller) ? Controller->GetPawn() : nullptr;
    UWorld* World = GetWorld();

    if (!IsValid(Controller) || !IsValid(Pawn) || !IsValid(World))
    {
        return;
    }

    UBDFRTrackingWorldSubsystem* Tracking =
        World->GetSubsystem<UBDFRTrackingWorldSubsystem>();
    if (!IsValid(Tracking))
    {
        return;
    }

    FBDFRTrackSample Candidate;

    if (IsValid(ScentTarget))
    {
        if (Tracking->FindNewerTrackSample(
            ScentTarget,
            EBDFRTrackType::Scent,
            LastScentSample.Sequence,
            Pawn->GetActorLocation(),
            ScentTrailStepRadius,
            Candidate)
            && Candidate.Strength >= MinimumScentStrength)
        {
            AcceptScentSample(Candidate);
            return;
        }
    }

    if (Tracking->FindBestTrackSample(
        Pawn->GetActorLocation(),
        ScentAcquisitionRadius,
        MaxScentAgeSeconds,
        false,
        true,
        nullptr,
        Candidate)
        && Candidate.Strength >= MinimumScentStrength)
    {
        AcceptScentSample(Candidate);
    }
}

void UBDFRCanineTrackingComponent::AcceptScentSample(const FBDFRTrackSample& Sample)
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!IsValid(Controller) || !IsValid(Sample.SourceActor))
    {
        return;
    }

    UBDFRAwarenessComponent* Awareness =
        Controller->FindComponentByClass<UBDFRAwarenessComponent>();

    ScentTarget = Sample.SourceActor;
    LastScentSample = Sample;
    NextScentLocation = Sample.Location + Sample.Direction.GetSafeNormal2D() * 180.0f;

    if (IsValid(Awareness))
    {
        Awareness->AddAwareness(
            Sample.SourceActor,
            FMath::Clamp(AwarenessGainFromScent * Sample.Strength, 0.0f, 1.0f),
            Sample.Location,
            false,
            false);
    }

    OnScentTrackDetected.Broadcast(Sample);
}
