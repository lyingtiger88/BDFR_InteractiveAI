#include "Awareness/BDFRAwarenessComponent.h"

#include "Core/BDFRAISettings.h"
#include "GameFramework/Actor.h"

UBDFRAwarenessComponent::UBDFRAwarenessComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRAwarenessComponent::BeginPlay()
{
    Super::BeginPlay();
    Snapshot.AwarenessLevel = GetAwarenessLevel();
}

void UBDFRAwarenessComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Snapshot.bHasLineOfSight || Snapshot.Awareness <= 0.0f)
    {
        return;
    }

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    const float PreviousAwareness = Snapshot.Awareness;
    const EBDFRAwarenessLevel PreviousLevel = Snapshot.AwarenessLevel;

    SetAwareness(Snapshot.Awareness - Settings->AwarenessDecayPerSecond * DeltaTime);

    if (Snapshot.Awareness <= KINDA_SMALL_NUMBER && !Snapshot.bHasConfirmedTarget)
    {
        CurrentTarget = nullptr;
        Snapshot.LastKnownLocation = FVector::ZeroVector;
    }

    BroadcastIfChanged(PreviousAwareness, PreviousLevel);
}

void UBDFRAwarenessComponent::AddAwareness(
    AActor* SourceActor,
    const float Amount,
    const FVector& StimulusLocation,
    const bool bConfirmTarget,
    const bool bHasLineOfSight)
{
    if (!IsValid(SourceActor) || Amount < 0.0f)
    {
        return;
    }

    const float PreviousAwareness = Snapshot.Awareness;
    const EBDFRAwarenessLevel PreviousLevel = Snapshot.AwarenessLevel;

    CurrentTarget = SourceActor;
    Snapshot.LastKnownLocation = StimulusLocation;
    Snapshot.bHasLineOfSight = bHasLineOfSight;
    Snapshot.bHasConfirmedTarget = Snapshot.bHasConfirmedTarget || bConfirmTarget;

    SetAwareness(Snapshot.Awareness + Amount);

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (Snapshot.Awareness >= Settings->ConfirmedThreatThreshold)
    {
        Snapshot.bHasConfirmedTarget = true;
    }

    Snapshot.AwarenessLevel = GetAwarenessLevel();
    BroadcastIfChanged(PreviousAwareness, PreviousLevel);
}

void UBDFRAwarenessComponent::SetTargetLineOfSight(AActor* SourceActor, const bool bHasLineOfSight)
{
    if (SourceActor != CurrentTarget)
    {
        return;
    }

    const float PreviousAwareness = Snapshot.Awareness;
    const EBDFRAwarenessLevel PreviousLevel = Snapshot.AwarenessLevel;

    Snapshot.bHasLineOfSight = bHasLineOfSight;

    if (IsValid(SourceActor))
    {
        Snapshot.LastKnownLocation = SourceActor->GetActorLocation();
    }

    BroadcastIfChanged(PreviousAwareness, PreviousLevel);
}

void UBDFRAwarenessComponent::ForgetTarget()
{
    const float PreviousAwareness = Snapshot.Awareness;
    const EBDFRAwarenessLevel PreviousLevel = Snapshot.AwarenessLevel;

    CurrentTarget = nullptr;
    Snapshot = FBDFRAwarenessSnapshot();

    BroadcastIfChanged(PreviousAwareness, PreviousLevel);
}

EBDFRAwarenessLevel UBDFRAwarenessComponent::GetAwarenessLevel() const
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();

    if (Snapshot.bHasConfirmedTarget || Snapshot.Awareness >= Settings->ConfirmedThreatThreshold)
    {
        return EBDFRAwarenessLevel::ConfirmedThreat;
    }

    if (Snapshot.Awareness >= Settings->AlertThreshold)
    {
        return EBDFRAwarenessLevel::Alerted;
    }

    if (Snapshot.Awareness >= Settings->InvestigateThreshold)
    {
        return EBDFRAwarenessLevel::Investigating;
    }

    if (Snapshot.Awareness >= Settings->SuspiciousThreshold)
    {
        return EBDFRAwarenessLevel::Suspicious;
    }

    return EBDFRAwarenessLevel::Unaware;
}

void UBDFRAwarenessComponent::SetAwareness(const float NewAwareness)
{
    Snapshot.Awareness = FMath::Clamp(NewAwareness, 0.0f, 1.0f);
    Snapshot.AwarenessLevel = GetAwarenessLevel();
}

void UBDFRAwarenessComponent::BroadcastIfChanged(
    const float PreviousAwareness,
    const EBDFRAwarenessLevel PreviousLevel)
{
    if (!FMath::IsNearlyEqual(PreviousAwareness, Snapshot.Awareness) || PreviousLevel != Snapshot.AwarenessLevel)
    {
        OnAwarenessChanged.Broadcast(CurrentTarget, Snapshot.Awareness, Snapshot.AwarenessLevel);
    }
}
