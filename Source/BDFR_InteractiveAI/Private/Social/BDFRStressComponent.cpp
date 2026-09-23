#include "Social/BDFRStressComponent.h"

UBDFRStressComponent::UBDFRStressComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRStressComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Stress > 0.0f && StressDecayPerSecond > 0.0f)
    {
        SetStress(Stress - StressDecayPerSecond * DeltaTime);
    }
}

void UBDFRStressComponent::AddStress(const float Amount)
{
    if (Amount > 0.0f)
    {
        SetStress(Stress + Amount);
    }
}

void UBDFRStressComponent::ReduceStress(const float Amount)
{
    if (Amount > 0.0f)
    {
        SetStress(Stress - Amount);
    }
}

EBDFRStressState UBDFRStressComponent::GetStressState() const
{
    if (Stress >= PanickedThreshold)
    {
        return EBDFRStressState::Panicked;
    }
    if (Stress >= StressedThreshold)
    {
        return EBDFRStressState::Stressed;
    }
    if (Stress >= ConcernedThreshold)
    {
        return EBDFRStressState::Concerned;
    }
    return EBDFRStressState::Calm;
}

void UBDFRStressComponent::SetStress(const float NewStress)
{
    const float Previous = Stress;
    const EBDFRStressState PreviousState = GetStressState();

    Stress = FMath::Clamp(NewStress, 0.0f, 1.0f);
    const EBDFRStressState NewState = GetStressState();

    if (!FMath::IsNearlyEqual(Previous, Stress) || PreviousState != NewState)
    {
        OnStressChanged.Broadcast(Stress, NewState);
    }
}
