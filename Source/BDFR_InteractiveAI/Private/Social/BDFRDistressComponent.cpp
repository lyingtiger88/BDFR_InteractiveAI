#include "Social/BDFRDistressComponent.h"

#include "GameFramework/Actor.h"
#include "Perception/AISense_Hearing.h"

UBDFRDistressComponent::UBDFRDistressComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBDFRDistressComponent::EmitPainShout(const float Urgency)
{
    EmitDistress(EBDFRDistressType::PainShout, Urgency);
}

void UBDFRDistressComponent::RequestHelp(const float Urgency)
{
    EmitDistress(EBDFRDistressType::HelpRequest, Urgency);
}

void UBDFRDistressComponent::EmitAllyDown(const float Urgency)
{
    EmitDistress(EBDFRDistressType::AllyDown, Urgency);
}

void UBDFRDistressComponent::EmitBleedingOut(const float Urgency)
{
    EmitDistress(EBDFRDistressType::BleedingOut, Urgency);
}

void UBDFRDistressComponent::EmitDistress(
    const EBDFRDistressType DistressType,
    const float Urgency,
    const float RadiusOverride)
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return;
    }

    const float ClampedUrgency = FMath::Clamp(Urgency, 0.0f, 1.0f);
    const float Radius = RadiusOverride > 0.0f ? RadiusOverride : GetDefaultRadius(DistressType);

    UAISense_Hearing::ReportNoiseEvent(
        Owner,
        Owner->GetActorLocation(),
        FMath::Max(0.1f, ClampedUrgency),
        Owner,
        Radius,
        GetNoiseTag(DistressType));

    OnDistressEmitted.Broadcast(DistressType, ClampedUrgency, Radius);
}

FName UBDFRDistressComponent::GetNoiseTag(const EBDFRDistressType DistressType)
{
    switch (DistressType)
    {
        case EBDFRDistressType::PainShout:
            return TEXT("BDFR.Distress.Pain");
        case EBDFRDistressType::HelpRequest:
            return TEXT("BDFR.Distress.Help");
        case EBDFRDistressType::AllyDown:
            return TEXT("BDFR.Distress.AllyDown");
        case EBDFRDistressType::BleedingOut:
            return TEXT("BDFR.Distress.BleedingOut");
        case EBDFRDistressType::Panic:
            return TEXT("BDFR.Distress.Panic");
        default:
            return TEXT("BDFR.Distress.Unknown");
    }
}

float UBDFRDistressComponent::GetDefaultRadius(const EBDFRDistressType DistressType) const
{
    switch (DistressType)
    {
        case EBDFRDistressType::PainShout:
            return DefaultPainRadius;
        case EBDFRDistressType::HelpRequest:
            return DefaultHelpRadius;
        case EBDFRDistressType::AllyDown:
        case EBDFRDistressType::BleedingOut:
        case EBDFRDistressType::Panic:
            return DefaultEmergencyRadius;
        default:
            return DefaultHelpRadius;
    }
}
