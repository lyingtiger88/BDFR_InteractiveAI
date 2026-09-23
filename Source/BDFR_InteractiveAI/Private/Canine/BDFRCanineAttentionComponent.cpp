#include "Canine/BDFRCanineAttentionComponent.h"

#include "AIController.h"
#include "Awareness/BDFRAwarenessComponent.h"
#include "Behavior/BDFRCanineAIController.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/BDFRCaninePresentationInterface.h"
#include "Tracking/BDFRCanineTrackingComponent.h"

UBDFRCanineAttentionComponent::UBDFRCanineAttentionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRCanineAttentionComponent::BeginPlay()
{
    Super::BeginPlay();
    PreviousState = Snapshot.State;
}

void UBDFRCanineAttentionComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateAttention(DeltaTime);
}

void UBDFRCanineAttentionComponent::UpdateAttention(const float DeltaTime)
{
    EBDFRCanineAttentionState NewState = EBDFRCanineAttentionState::Relaxed;
    FVector FocusLocation = FVector::ZeroVector;
    float Alertness = 0.0f;
    AActor* EngageTarget = nullptr;

    ResolveAttentionTarget(NewState, FocusLocation, Alertness, EngageTarget);

    Snapshot.State = NewState;
    Snapshot.FocusLocation = FocusLocation;
    Snapshot.bHasFocus = !FocusLocation.IsNearlyZero();
    Snapshot.Alertness = FMath::Clamp(Alertness, 0.0f, 1.0f);
    Snapshot.bRequestEngageAnimation = NewState == EBDFRCanineAttentionState::Engaging;

    UpdateAimOffsets(FocusLocation, Snapshot.bHasFocus, DeltaTime);

    if (PreviousState != NewState)
    {
        OnCanineAttentionStateChanged.Broadcast(PreviousState, NewState);
        PreviousState = NewState;

        if (NewState != EBDFRCanineAttentionState::Engaging)
        {
            bEngageAnimationSent = false;
        }
    }

    PushPresentation(EngageTarget);
    OnCanineAttentionUpdated.Broadcast(Snapshot);
}

void UBDFRCanineAttentionComponent::ResolveAttentionTarget(
    EBDFRCanineAttentionState& OutState,
    FVector& OutLocation,
    float& OutAlertness,
    AActor*& OutEngageTarget) const
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    ABDFRCanineAIController* Controller =
        IsValid(Pawn) ? Cast<ABDFRCanineAIController>(Pawn->GetController()) : nullptr;

    if (!IsValid(Pawn) || !IsValid(Controller))
    {
        return;
    }

    UBDFRAwarenessComponent* Awareness = Controller->GetAwarenessComponent();
    UBDFRCanineTrackingComponent* Tracking = Controller->GetCanineTrackingComponent();

    if (IsValid(Awareness))
    {
        const FBDFRAwarenessSnapshot AwarenessSnapshot = Awareness->GetSnapshot();
        AActor* CurrentTarget = Awareness->GetCurrentTarget();

        if (AwarenessSnapshot.bHasConfirmedTarget && IsValid(CurrentTarget))
        {
            OutState = EBDFRCanineAttentionState::Engaging;
            OutLocation = CurrentTarget->GetActorLocation();
            OutAlertness = 1.0f;
            OutEngageTarget = CurrentTarget;
            return;
        }

        if (AwarenessSnapshot.AwarenessLevel == EBDFRAwarenessLevel::Alerted
            || AwarenessSnapshot.AwarenessLevel == EBDFRAwarenessLevel::Investigating)
        {
            OutState = EBDFRCanineAttentionState::Alerted;
            OutLocation = AwarenessSnapshot.LastKnownLocation;
            OutAlertness = FMath::Max(0.65f, AwarenessSnapshot.Awareness);
        }
    }

    const UWorld* World = GetWorld();
    const float Now = IsValid(World) ? World->GetTimeSeconds() : 0.0f;
    const float AcousticAge = Now - Controller->GetLastHeardAcousticTimeSeconds();

    if (AcousticAge >= 0.0f && AcousticAge <= AcousticAttentionHoldSeconds)
    {
        OutState = EBDFRCanineAttentionState::Listening;
        OutLocation = Controller->GetLastHeardAcousticLocation();
        OutAlertness = FMath::Max(
            OutAlertness,
            FMath::Clamp(Controller->GetLastHeardAcousticStrength(), 0.0f, 1.0f));
        return;
    }

    if (IsValid(Tracking) && Tracking->HasScentTarget())
    {
        OutState = EBDFRCanineAttentionState::Tracking;
        OutLocation = Tracking->GetNextScentLocation();
        OutAlertness = FMath::Max(OutAlertness, 0.55f);
        return;
    }

    if (OutState == EBDFRCanineAttentionState::Alerted)
    {
        return;
    }

    OutState = EBDFRCanineAttentionState::Scanning;
    const float Phase = Now * TWO_PI * EnvironmentalScanFrequency;
    const float ScanYaw = FMath::Sin(Phase) * EnvironmentalScanYawDegrees;
    OutLocation =
        Pawn->GetActorLocation()
        + Pawn->GetActorForwardVector().RotateAngleAxis(ScanYaw, FVector::UpVector) * 500.0f
        + FVector(0.0f, 0.0f, 60.0f);
    OutAlertness = 0.15f;
}

void UBDFRCanineAttentionComponent::UpdateAimOffsets(
    const FVector& FocusLocation,
    const bool bHasFocus,
    const float DeltaTime)
{
    APawn* Pawn = Cast<APawn>(GetOwner());

    float TargetYaw = 0.0f;
    float TargetPitch = 0.0f;

    if (IsValid(Pawn) && bHasFocus)
    {
        const FVector ToFocus = FocusLocation - Pawn->GetActorLocation();
        const FRotator WorldLookRotation = ToFocus.Rotation();
        const FRotator LocalDelta =
            (WorldLookRotation - Pawn->GetActorRotation()).GetNormalized();

        TargetYaw = LocalDelta.Yaw;
        TargetPitch = LocalDelta.Pitch;
    }

    const float EarYawTarget = FMath::Clamp(TargetYaw, -MaxEarYawDegrees, MaxEarYawDegrees);
    const float EarPitchTarget = FMath::Clamp(TargetPitch, -MaxEarPitchDegrees, MaxEarPitchDegrees);
    const float HeadYawTarget = FMath::Clamp(TargetYaw, -MaxHeadYawDegrees, MaxHeadYawDegrees);
    const float HeadPitchTarget = FMath::Clamp(TargetPitch, -MaxHeadPitchDegrees, MaxHeadPitchDegrees);

    Snapshot.EarYawDegrees = FMath::FInterpTo(
        Snapshot.EarYawDegrees,
        EarYawTarget,
        DeltaTime,
        EarInterpSpeed);

    Snapshot.EarPitchDegrees = FMath::FInterpTo(
        Snapshot.EarPitchDegrees,
        EarPitchTarget,
        DeltaTime,
        EarInterpSpeed);

    Snapshot.HeadYawDegrees = FMath::FInterpTo(
        Snapshot.HeadYawDegrees,
        HeadYawTarget,
        DeltaTime,
        HeadInterpSpeed);

    Snapshot.HeadPitchDegrees = FMath::FInterpTo(
        Snapshot.HeadPitchDegrees,
        HeadPitchTarget,
        DeltaTime,
        HeadInterpSpeed);
}

void UBDFRCanineAttentionComponent::PushPresentation(AActor* EngageTarget)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return;
    }

    if (Owner->GetClass()->ImplementsInterface(UBDFRCaninePresentationInterface::StaticClass()))
    {
        IBDFRCaninePresentationInterface::Execute_BDFR_UpdateCanineAttention(
            Owner,
            Snapshot);

        if (Snapshot.State == EBDFRCanineAttentionState::Engaging
            && IsValid(EngageTarget)
            && !bEngageAnimationSent)
        {
            IBDFRCaninePresentationInterface::Execute_BDFR_PlayCanineEngage(
                Owner,
                EngageTarget);
            bEngageAnimationSent = true;
        }

        APawn* Pawn = Cast<APawn>(Owner);
        ABDFRCanineAIController* Controller =
            IsValid(Pawn) ? Cast<ABDFRCanineAIController>(Pawn->GetController()) : nullptr;

        const UWorld* World = GetWorld();
        const float Now = IsValid(World) ? World->GetTimeSeconds() : 0.0f;

        if (IsValid(Controller)
            && Snapshot.State == EBDFRCanineAttentionState::Listening
            && Controller->GetLastHeardAcousticStrength() >= BarkOnStrongSoundThreshold
            && Now - LastBarkTimeSeconds >= 4.0f)
        {
            IBDFRCaninePresentationInterface::Execute_BDFR_PlayCanineBark(
                Owner,
                TEXT("BDFR.Canine.Bark.StrongSound"));
            LastBarkTimeSeconds = Now;
        }
    }
}
