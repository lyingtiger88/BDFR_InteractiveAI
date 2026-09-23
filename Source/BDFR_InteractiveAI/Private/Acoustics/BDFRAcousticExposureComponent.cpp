#include "Acoustics/BDFRAcousticExposureComponent.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"

UBDFRAcousticExposureComponent::UBDFRAcousticExposureComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRAcousticExposureComponent::BeginPlay()
{
    Super::BeginPlay();
    CachedState = GetHearingState();
    RefreshStateAndSense();
}

void UBDFRAcousticExposureComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Exposure > 0.0f && ExposureDecayPerSecond > 0.0f)
    {
        SetExposure(Exposure - ExposureDecayPerSecond * DeltaTime);
    }
}

void UBDFRAcousticExposureComponent::ApplyExposure(
    const float Amount,
    const FVector SourceLocation,
    const EBDFRAcousticEventType EventType)
{
    if (Amount <= 0.0f)
    {
        return;
    }

    LastExposureLocation = SourceLocation;
    LastEventType = EventType;

    SetExposure(Exposure + Amount);
    OnAcousticExposure.Broadcast(EventType, Amount, SourceLocation);
}

void UBDFRAcousticExposureComponent::ClearExposure()
{
    SetExposure(0.0f);
}

float UBDFRAcousticExposureComponent::GetHearingSensitivity() const
{
    if (GetHearingState() == EBDFRHearingState::Deafened)
    {
        return 0.0f;
    }

    const float Scaled = 1.0f - Exposure * 0.85f;
    return FMath::Clamp(Scaled, MinimumImpairedSensitivity, 1.0f);
}

EBDFRHearingState UBDFRAcousticExposureComponent::GetHearingState() const
{
    if (Exposure >= DeafenedThreshold)
    {
        return EBDFRHearingState::Deafened;
    }

    if (Exposure >= ImpairedThreshold)
    {
        return EBDFRHearingState::Impaired;
    }

    if (Exposure >= RingingThreshold)
    {
        return EBDFRHearingState::Ringing;
    }

    return EBDFRHearingState::Normal;
}

FBDFRAcousticExposureSnapshot UBDFRAcousticExposureComponent::GetSnapshot() const
{
    FBDFRAcousticExposureSnapshot Snapshot;
    Snapshot.Exposure = Exposure;
    Snapshot.HearingSensitivity = GetHearingSensitivity();
    Snapshot.HearingState = GetHearingState();
    Snapshot.LastExposureLocation = LastExposureLocation;
    Snapshot.LastEventType = LastEventType;
    Snapshot.bHearingSenseDisabled = bHearingSenseDisabled;
    return Snapshot;
}

void UBDFRAcousticExposureComponent::SetExposure(const float NewExposure)
{
    Exposure = FMath::Clamp(NewExposure, 0.0f, 1.0f);
    RefreshStateAndSense();
}

void UBDFRAcousticExposureComponent::RefreshStateAndSense()
{
    const EBDFRHearingState NewState = GetHearingState();
    const bool bShouldEnableHearing = NewState != EBDFRHearingState::Deafened;

    if (bShouldEnableHearing == bHearingSenseDisabled)
    {
        SetHearingSenseEnabled(bShouldEnableHearing);
    }

    if (NewState != CachedState)
    {
        const EBDFRHearingState PreviousState = CachedState;
        CachedState = NewState;
        OnHearingStateChanged.Broadcast(PreviousState, NewState, Exposure);
    }
}

void UBDFRAcousticExposureComponent::SetHearingSenseEnabled(const bool bEnabled)
{
    APawn* PawnOwner = Cast<APawn>(GetOwner());
    if (!IsValid(PawnOwner))
    {
        return;
    }

    AAIController* AIController = Cast<AAIController>(PawnOwner->GetController());
    if (!IsValid(AIController))
    {
        return;
    }

    UAIPerceptionComponent* Perception = AIController->GetAIPerceptionComponent();
    if (!IsValid(Perception))
    {
        return;
    }

    Perception->SetSenseEnabled(UAISense_Hearing::StaticClass(), bEnabled);
    bHearingSenseDisabled = !bEnabled;
}
