#include "Captivity/BDFRCaptivityComponent.h"

#include "Core/BDFRAISettings.h"
#include "GameFramework/Actor.h"

UBDFRCaptivityComponent::UBDFRCaptivityComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UBDFRCaptivityComponent::BeginSurrender(AActor* InCaptor)
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (!Settings->bEnableCaptivitySystem || !bCanBeCaptured || State == EBDFRCaptivityState::Released)
    {
        return false;
    }

    Captor = InCaptor;
    bRestrained = false;
    SetState(EBDFRCaptivityState::Surrendering);
    return true;
}

bool UBDFRCaptivityComponent::Restrain(AActor* InCaptor)
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (!Settings->bEnableCaptivitySystem || !bCanBeCaptured)
    {
        return false;
    }

    Captor = InCaptor;
    bRestrained = true;
    SetState(EBDFRCaptivityState::Restrained);
    return true;
}

bool UBDFRCaptivityComponent::SetAsHostage(AActor* InCaptor)
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (!Settings->bEnableCaptivitySystem || !bRestrained)
    {
        return false;
    }

    Captor = InCaptor;
    SetState(EBDFRCaptivityState::Hostage);
    return true;
}

bool UBDFRCaptivityComponent::BeginInterrogation(AActor* InCaptor)
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (!Settings->bEnableInterrogationSystem || !bRestrained)
    {
        return false;
    }

    Captor = InCaptor;
    SetState(EBDFRCaptivityState::Interrogating);
    return true;
}

void UBDFRCaptivityComponent::Release()
{
    Captor = nullptr;
    bRestrained = false;
    SetState(EBDFRCaptivityState::Released);
}

bool UBDFRCaptivityComponent::BeginEscape()
{
    if (!bCanAttemptEscape || !IsCaptive())
    {
        return false;
    }

    SetState(EBDFRCaptivityState::Escaping);
    return true;
}

bool UBDFRCaptivityComponent::IsCaptive() const
{
    return State == EBDFRCaptivityState::Surrendering
        || State == EBDFRCaptivityState::Restrained
        || State == EBDFRCaptivityState::Hostage
        || State == EBDFRCaptivityState::Interrogating;
}

FBDFRCaptivitySnapshot UBDFRCaptivityComponent::GetSnapshot() const
{
    FBDFRCaptivitySnapshot Snapshot;
    Snapshot.State = State;
    Snapshot.Captor = Captor;
    Snapshot.bRestrained = bRestrained;
    Snapshot.bCanEscape = bCanAttemptEscape && IsCaptive();
    return Snapshot;
}

void UBDFRCaptivityComponent::SetState(const EBDFRCaptivityState NewState)
{
    if (State == NewState)
    {
        return;
    }

    const EBDFRCaptivityState PreviousState = State;
    State = NewState;
    OnCaptivityStateChanged.Broadcast(PreviousState, State);
}
