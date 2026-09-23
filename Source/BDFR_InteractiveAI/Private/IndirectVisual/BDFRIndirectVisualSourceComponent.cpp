#include "IndirectVisual/BDFRIndirectVisualSourceComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "IndirectVisual/BDFRIndirectVisualWorldSubsystem.h"

UBDFRIndirectVisualSourceComponent::UBDFRIndirectVisualSourceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBDFRIndirectVisualSourceComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UBDFRIndirectVisualWorldSubsystem* Subsystem =
            World->GetSubsystem<UBDFRIndirectVisualWorldSubsystem>())
        {
            Subsystem->RegisterSource(this);
        }
    }
}

void UBDFRIndirectVisualSourceComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (UBDFRIndirectVisualWorldSubsystem* Subsystem =
            World->GetSubsystem<UBDFRIndirectVisualWorldSubsystem>())
        {
            Subsystem->UnregisterSource(this);
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UBDFRIndirectVisualSourceComponent::SetShadowCue(
    const FVector InShadowLocation,
    const float InStrength)
{
    ShadowCueLocation = InShadowLocation;
    ShadowCueStrength = FMath::Clamp(InStrength * BaseShadowStrength, 0.0f, 1.0f);
    bShadowCueValid = ShadowCueStrength > KINDA_SMALL_NUMBER;
}

void UBDFRIndirectVisualSourceComponent::ClearShadowCue()
{
    bShadowCueValid = false;
    ShadowCueLocation = FVector::ZeroVector;
    ShadowCueStrength = 0.0f;
}

bool UBDFRIndirectVisualSourceComponent::EstimateShadowCueFromLightDirection(
    FVector LightTravelDirection,
    const float TraceDistance)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();

    if (!IsValid(Owner) || !IsValid(World) || LightTravelDirection.IsNearlyZero())
    {
        ClearShadowCue();
        return false;
    }

    LightTravelDirection.Normalize();

    const FVector Start = Owner->GetActorLocation();
    const FVector End = Start + LightTravelDirection * FMath::Max(100.0f, TraceDistance);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(BDFRShadowProjection), false, Owner);

    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params);

    if (!bHit)
    {
        ClearShadowCue();
        return false;
    }

    const float TravelFraction =
        FMath::Clamp(Hit.Distance / FMath::Max(TraceDistance, 1.0f), 0.0f, 1.0f);
    const float Strength = FMath::Lerp(1.0f, 0.35f, TravelFraction);

    SetShadowCue(Hit.ImpactPoint + Hit.ImpactNormal * 2.0f, Strength);
    return true;
}
