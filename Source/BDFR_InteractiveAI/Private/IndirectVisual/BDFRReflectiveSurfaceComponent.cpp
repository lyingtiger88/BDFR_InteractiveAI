#include "IndirectVisual/BDFRReflectiveSurfaceComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "IndirectVisual/BDFRIndirectVisualWorldSubsystem.h"

UBDFRReflectiveSurfaceComponent::UBDFRReflectiveSurfaceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    SetGenerateOverlapEvents(false);
}

void UBDFRReflectiveSurfaceComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UBDFRIndirectVisualWorldSubsystem* Subsystem =
            World->GetSubsystem<UBDFRIndirectVisualWorldSubsystem>())
        {
            Subsystem->RegisterReflectiveSurface(this);
        }
    }
}

void UBDFRReflectiveSurfaceComponent::EndPlay(
    const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (UBDFRIndirectVisualWorldSubsystem* Subsystem =
            World->GetSubsystem<UBDFRIndirectVisualWorldSubsystem>())
        {
            Subsystem->UnregisterReflectiveSurface(this);
        }
    }

    Super::EndPlay(EndPlayReason);
}

bool UBDFRReflectiveSurfaceComponent::TryCalculateReflectionCue(
    const FVector ViewerLocation,
    AActor* SourceActor,
    FVector& OutCueLocation,
    float& OutStrength) const
{
    OutCueLocation = FVector::ZeroVector;
    OutStrength = 0.0f;

    if (!IsValid(SourceActor) || ReflectionStrength <= KINDA_SMALL_NUMBER)
    {
        return false;
    }

    FVector SurfaceNormal = GetComponentTransform().TransformVectorNoScale(LocalSurfaceNormal);
    if (SurfaceNormal.IsNearlyZero())
    {
        return false;
    }
    SurfaceNormal.Normalize();

    const FVector PlanePoint = GetComponentLocation();
    const FVector SourceLocation = SourceActor->GetActorLocation();

    const float SourcePlaneDistance =
        FVector::DotProduct(SourceLocation - PlanePoint, SurfaceNormal);
    const FVector MirroredSource =
        SourceLocation - 2.0f * SourcePlaneDistance * SurfaceNormal;

    const FVector Ray = MirroredSource - ViewerLocation;
    const float Denominator = FVector::DotProduct(Ray, SurfaceNormal);

    if (FMath::IsNearlyZero(Denominator))
    {
        return false;
    }

    const float T =
        FVector::DotProduct(PlanePoint - ViewerLocation, SurfaceNormal)
        / Denominator;

    if (T <= 0.0f || T >= 1.0f)
    {
        return false;
    }

    const FVector ReflectionPoint = ViewerLocation + Ray * T;
    if (!IsPointInsideSurfaceBounds(ReflectionPoint))
    {
        return false;
    }

    const float TotalPathLength =
        FVector::Distance(ViewerLocation, ReflectionPoint)
        + FVector::Distance(SourceLocation, ReflectionPoint);

    if (TotalPathLength > MaxDetectionPathLength)
    {
        return false;
    }

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    FCollisionQueryParams ViewerParams(SCENE_QUERY_STAT(BDFRReflectionViewer), false);
    ViewerParams.AddIgnoredActor(SourceActor);

    FHitResult ViewerHit;
    const bool bViewerBlocked = World->LineTraceSingleByChannel(
        ViewerHit,
        ViewerLocation,
        ReflectionPoint,
        ECC_Visibility,
        ViewerParams);

    if (bViewerBlocked && ViewerHit.Component.Get() != this)
    {
        return false;
    }

    if (bRequireClearSourceToSurfacePath)
    {
        FCollisionQueryParams SourceParams(SCENE_QUERY_STAT(BDFRReflectionSource), false, SourceActor);

        FHitResult SourceHit;
        const bool bSourceBlocked = World->LineTraceSingleByChannel(
            SourceHit,
            SourceLocation,
            ReflectionPoint,
            ECC_Visibility,
            SourceParams);

        if (bSourceBlocked && SourceHit.Component.Get() != this)
        {
            return false;
        }
    }

    const float DistanceFactor =
        1.0f - FMath::Clamp(TotalPathLength / MaxDetectionPathLength, 0.0f, 1.0f);

    const FVector ToViewer = (ViewerLocation - ReflectionPoint).GetSafeNormal();
    const float FacingFactor =
        FMath::Clamp(FVector::DotProduct(ToViewer, SurfaceNormal), 0.0f, 1.0f);

    OutCueLocation = ReflectionPoint;
    OutStrength = FMath::Clamp(
        ReflectionStrength * DistanceFactor * FMath::Sqrt(FacingFactor),
        0.0f,
        1.0f);

    return OutStrength > KINDA_SMALL_NUMBER;
}

bool UBDFRReflectiveSurfaceComponent::IsPointInsideSurfaceBounds(
    const FVector& WorldPoint) const
{
    const FVector LocalPoint = GetComponentTransform().InverseTransformPosition(WorldPoint);
    const FVector Extent = GetUnscaledBoxExtent();

    return FMath::Abs(LocalPoint.X) <= Extent.X
        && FMath::Abs(LocalPoint.Y) <= Extent.Y
        && FMath::Abs(LocalPoint.Z) <= Extent.Z;
}
