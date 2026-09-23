#include "Tracking/BDFRTrackEmitterComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Tracking/BDFRTrackingWorldSubsystem.h"

UBDFRTrackEmitterComponent::UBDFRTrackEmitterComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRTrackEmitterComponent::BeginPlay()
{
    Super::BeginPlay();

    if (const AActor* Owner = GetOwner())
    {
        LastFootprintLocation = Owner->GetActorLocation();
    }
}

void UBDFRTrackEmitterComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    if (!bTrackingEmissionEnabled || !IsValid(Owner))
    {
        return;
    }

    const FVector CurrentLocation = Owner->GetActorLocation();
    const float MovedDistance = FVector::Dist2D(CurrentLocation, LastFootprintLocation);

    if (MovedDistance >= FootprintSpacingCm)
    {
        EmitFootprintNow();
        LastFootprintLocation = CurrentLocation;
    }

    ScentAccumulator += DeltaTime;
    if (ScentAccumulator >= ScentIntervalSeconds)
    {
        ScentAccumulator = 0.0f;
        EmitScentNow();
    }
}

void UBDFRTrackEmitterComponent::SetSurfaceType(const EBDFRTrackSurfaceType NewSurfaceType)
{
    SurfaceType = NewSurfaceType;
}

void UBDFRTrackEmitterComponent::EmitFootprintNow(const float StrengthOverride)
{
    const float Strength = StrengthOverride >= 0.0f
        ? StrengthOverride
        : BaseFootprintStrength * GetSurfaceFootprintMultiplier();

    if (Strength > KINDA_SMALL_NUMBER)
    {
        AddSample(EBDFRTrackType::Footprint, FMath::Clamp(Strength, 0.0f, 1.0f), ++FootprintSequence);
    }
}

void UBDFRTrackEmitterComponent::EmitScentNow(const float StrengthOverride)
{
    const float Strength = StrengthOverride >= 0.0f
        ? StrengthOverride
        : BaseScentStrength * GetSurfaceScentMultiplier();

    if (Strength > KINDA_SMALL_NUMBER)
    {
        AddSample(EBDFRTrackType::Scent, FMath::Clamp(Strength, 0.0f, 1.0f), ++ScentSequence);
    }
}

float UBDFRTrackEmitterComponent::GetSurfaceFootprintMultiplier() const
{
    switch (SurfaceType)
    {
        case EBDFRTrackSurfaceType::Mud:      return 1.00f;
        case EBDFRTrackSurfaceType::Snow:     return 1.00f;
        case EBDFRTrackSurfaceType::Sand:     return 0.90f;
        case EBDFRTrackSurfaceType::Dirt:     return 0.80f;
        case EBDFRTrackSurfaceType::Grass:    return 0.45f;
        case EBDFRTrackSurfaceType::Concrete: return 0.18f;
        case EBDFRTrackSurfaceType::Metal:    return 0.08f;
        case EBDFRTrackSurfaceType::Water:    return 0.00f;
        default:                              return 0.35f;
    }
}

float UBDFRTrackEmitterComponent::GetSurfaceScentMultiplier() const
{
    switch (SurfaceType)
    {
        case EBDFRTrackSurfaceType::Water:    return 0.25f;
        case EBDFRTrackSurfaceType::Metal:    return 0.65f;
        case EBDFRTrackSurfaceType::Concrete: return 0.70f;
        case EBDFRTrackSurfaceType::Grass:    return 1.00f;
        case EBDFRTrackSurfaceType::Dirt:     return 0.95f;
        case EBDFRTrackSurfaceType::Mud:      return 0.90f;
        case EBDFRTrackSurfaceType::Snow:     return 0.75f;
        case EBDFRTrackSurfaceType::Sand:     return 0.80f;
        default:                              return 0.80f;
    }
}

void UBDFRTrackEmitterComponent::AddSample(
    const EBDFRTrackType TrackType,
    const float Strength,
    const int32 Sequence)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!IsValid(Owner) || !IsValid(World))
    {
        return;
    }

    UBDFRTrackingWorldSubsystem* Subsystem =
        World->GetSubsystem<UBDFRTrackingWorldSubsystem>();
    if (!IsValid(Subsystem))
    {
        return;
    }

    FBDFRTrackSample Sample;
    Sample.TrackType = TrackType;
    Sample.SurfaceType = SurfaceType;
    Sample.SourceActor = Owner;
    Sample.Location = Owner->GetActorLocation();
    Sample.Direction = Owner->GetActorForwardVector();
    Sample.TimeSeconds = World->GetTimeSeconds();
    Sample.Strength = Strength;
    Sample.Sequence = Sequence;

    Subsystem->AddTrackSample(Sample);
}
