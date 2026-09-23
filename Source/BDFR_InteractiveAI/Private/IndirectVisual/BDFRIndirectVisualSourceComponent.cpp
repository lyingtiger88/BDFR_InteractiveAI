#include "IndirectVisual/BDFRIndirectVisualSourceComponent.h"

#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "IndirectVisual/BDFRIndirectVisualWorldSubsystem.h"

UBDFRIndirectVisualSourceComponent::UBDFRIndirectVisualSourceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
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

    if (bAutoEstimateShadowFromDirectionalLight)
    {
        CachedDirectionalLight = FindDominantDirectionalLight();
        UpdateAutomaticShadowCue();
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

void UBDFRIndirectVisualSourceComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bAutoEstimateShadowFromDirectionalLight || !bCanProduceShadowCue)
    {
        return;
    }

    AutoShadowAccumulatedTime += DeltaTime;
    if (AutoShadowAccumulatedTime < AutoShadowUpdateInterval)
    {
        return;
    }

    AutoShadowAccumulatedTime = 0.0f;
    UpdateAutomaticShadowCue();
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

    SetShadowCue(Hit.ImpactPoint + Hit.ImpactNormal * 3.0f, Strength);
    return true;
}

ADirectionalLight* UBDFRIndirectVisualSourceComponent::FindDominantDirectionalLight() const
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return nullptr;
    }

    ADirectionalLight* BestLight = nullptr;
    float BestIntensity = -1.0f;

    for (TActorIterator<ADirectionalLight> It(World); It; ++It)
    {
        ADirectionalLight* Light = *It;
        UDirectionalLightComponent* LightComponent =
            IsValid(Light) ? Light->GetComponent() : nullptr;

        if (!IsValid(LightComponent)
            || !LightComponent->IsVisible()
            || LightComponent->Intensity <= BestIntensity)
        {
            continue;
        }

        BestIntensity = LightComponent->Intensity;
        BestLight = Light;
    }

    return BestLight;
}

void UBDFRIndirectVisualSourceComponent::UpdateAutomaticShadowCue()
{
    if (!IsValid(CachedDirectionalLight))
    {
        CachedDirectionalLight = FindDominantDirectionalLight();
    }

    UDirectionalLightComponent* LightComponent =
        IsValid(CachedDirectionalLight)
            ? CachedDirectionalLight->GetComponent()
            : nullptr;

    if (!IsValid(LightComponent))
    {
        ClearShadowCue();
        return;
    }

    EstimateShadowCueFromLightDirection(
        LightComponent->GetDirection(),
        AutoShadowTraceDistance);
}
