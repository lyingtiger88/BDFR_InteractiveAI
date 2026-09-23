#include "Health/BDFRInjuryResponseComponent.h"

#include "Engine/World.h"
#include "Health/BDFRHealthComponent.h"
#include "Social/BDFRDistressComponent.h"

UBDFRInjuryResponseComponent::UBDFRInjuryResponseComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBDFRInjuryResponseComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return;
    }

    HealthComponent = Owner->FindComponentByClass<UBDFRHealthComponent>();
    DistressComponent = Owner->FindComponentByClass<UBDFRDistressComponent>();

    if (IsValid(HealthComponent))
    {
        HealthComponent->OnWounded.AddDynamic(this, &ThisClass::HandleWounded);
        HealthComponent->OnHealthStateChanged.AddDynamic(this, &ThisClass::HandleHealthStateChanged);
        HealthComponent->OnBleedingChanged.AddDynamic(this, &ThisClass::HandleBleedingChanged);
    }
}

void UBDFRInjuryResponseComponent::HandleWounded(
    AActor* DamageCauser,
    const float DamageAmount,
    const EBDFRHealthState NewState)
{
    if (!IsValid(DistressComponent))
    {
        return;
    }

    if (bAutoPainShout && CanEmit(LastPainShoutTime, PainShoutCooldown))
    {
        const float Urgency =
            NewState == EBDFRHealthState::Critical || NewState == EBDFRHealthState::Incapacitated
                ? 0.9f
                : 0.5f;

        DistressComponent->EmitPainShout(Urgency);
        LastPainShoutTime = GetWorld()->GetTimeSeconds();
    }

    if (NewState == EBDFRHealthState::Critical || NewState == EBDFRHealthState::Incapacitated)
    {
        TryRequestHelp(NewState == EBDFRHealthState::Incapacitated ? 1.0f : 0.85f);
    }
}

void UBDFRInjuryResponseComponent::HandleHealthStateChanged(
    const EBDFRHealthState PreviousState,
    const EBDFRHealthState NewState)
{
    if (!IsValid(DistressComponent))
    {
        return;
    }

    if (NewState == EBDFRHealthState::Incapacitated)
    {
        DistressComponent->EmitBleedingOut(1.0f);
    }
}

void UBDFRInjuryResponseComponent::HandleBleedingChanged(const bool bBleeding)
{
    if (bBleeding)
    {
        TryRequestHelp(0.8f);
    }
}

bool UBDFRInjuryResponseComponent::CanEmit(const float LastTime, const float Cooldown) const
{
    return GetWorld() != nullptr && GetWorld()->GetTimeSeconds() - LastTime >= Cooldown;
}

void UBDFRInjuryResponseComponent::TryRequestHelp(const float Urgency)
{
    if (!bAutoRequestHelp || !IsValid(DistressComponent) || !CanEmit(LastHelpRequestTime, HelpRequestCooldown))
    {
        return;
    }

    DistressComponent->RequestHelp(Urgency);
    LastHelpRequestTime = GetWorld()->GetTimeSeconds();
}
