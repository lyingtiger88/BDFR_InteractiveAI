#include "IndirectVisual/BDFRIndirectVisualPerceptionComponent.h"

#include "AIController.h"
#include "Awareness/BDFRAwarenessComponent.h"
#include "Difficulty/BDFRDifficultyComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "IndirectVisual/BDFRIndirectVisualSourceComponent.h"
#include "IndirectVisual/BDFRIndirectVisualWorldSubsystem.h"
#include "IndirectVisual/BDFRReflectiveSurfaceComponent.h"

UBDFRIndirectVisualPerceptionComponent::UBDFRIndirectVisualPerceptionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UBDFRIndirectVisualPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBDFRIndirectVisualPerceptionComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AccumulatedTime += DeltaTime;
    if (AccumulatedTime < ScanIntervalSeconds)
    {
        return;
    }

    AccumulatedTime = 0.0f;
    ScanForIndirectVisualCues();
}

void UBDFRIndirectVisualPerceptionComponent::ScanForIndirectVisualCues()
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    APawn* Pawn = IsValid(Controller) ? Controller->GetPawn() : nullptr;
    UWorld* World = GetWorld();

    if (!IsValid(Controller) || !IsValid(Pawn) || !IsValid(World))
    {
        return;
    }

    UBDFRDifficultyComponent* Difficulty =
        Controller->FindComponentByClass<UBDFRDifficultyComponent>();
    UBDFRAwarenessComponent* Awareness =
        Controller->FindComponentByClass<UBDFRAwarenessComponent>();

    if (!IsValid(Difficulty) || !IsValid(Awareness))
    {
        return;
    }

    const FBDFRDifficultyProfile Profile = Difficulty->GetDifficultyProfile();
    if (!Profile.bCanDetectShadows && !Profile.bCanDetectReflections)
    {
        return;
    }

    UBDFRIndirectVisualWorldSubsystem* Subsystem =
        World->GetSubsystem<UBDFRIndirectVisualWorldSubsystem>();
    if (!IsValid(Subsystem))
    {
        return;
    }

    FVector ViewerLocation;
    FRotator ViewerRotation;
    Pawn->GetActorEyesViewPoint(ViewerLocation, ViewerRotation);

    const float Now = World->GetTimeSeconds();

    for (const TWeakObjectPtr<UBDFRIndirectVisualSourceComponent>& WeakSource : Subsystem->GetSources())
    {
        UBDFRIndirectVisualSourceComponent* Source = WeakSource.Get();
        AActor* SourceActor = IsValid(Source) ? Source->GetOwner() : nullptr;

        if (!IsValid(SourceActor) || SourceActor == Pawn)
        {
            continue;
        }

        const float* LastCueTime = LastCueTimeBySource.Find(SourceActor);
        if (LastCueTime && Now - *LastCueTime < PerSourceCueCooldownSeconds)
        {
            continue;
        }

        if (IsSourceDirectlyVisible(SourceActor, ViewerLocation))
        {
            continue;
        }

        bool bDetectedAnyCue = false;

        if (Profile.bCanDetectShadows && Source->HasValidShadowCue())
        {
            const FVector CueLocation = Source->GetShadowCueLocation();
            const float Distance = FVector::Distance(ViewerLocation, CueLocation);

            if (Distance <= ShadowDetectionRadius
                && IsPointVisible(ViewerLocation, CueLocation, SourceActor))
            {
                FBDFRIndirectVisualCue Cue;
                Cue.CueType = EBDFRIndirectVisualCueType::Shadow;
                Cue.SourceActor = SourceActor;
                Cue.CueLocation = CueLocation;
                Cue.Strength = Source->GetShadowCueStrength();

                if (Cue.Strength >= MinimumCueStrength)
                {
                    HandleDetectedCue(Cue, ShadowBaseAwarenessGain);
                    bDetectedAnyCue = true;
                }
            }
        }

        if (!bDetectedAnyCue
            && Profile.bCanDetectReflections
            && Source->CanProduceReflectionCue())
        {
            for (const TWeakObjectPtr<UBDFRReflectiveSurfaceComponent>& WeakSurface :
                Subsystem->GetReflectiveSurfaces())
            {
                UBDFRReflectiveSurfaceComponent* Surface = WeakSurface.Get();
                if (!IsValid(Surface))
                {
                    continue;
                }

                if (FVector::Distance(ViewerLocation, Surface->GetComponentLocation())
                    > ReflectionDetectionRadius)
                {
                    continue;
                }

                FVector CueLocation;
                float CueStrength = 0.0f;

                if (!Surface->TryCalculateReflectionCue(
                    ViewerLocation,
                    SourceActor,
                    CueLocation,
                    CueStrength))
                {
                    continue;
                }

                if (CueStrength < MinimumCueStrength)
                {
                    continue;
                }

                FBDFRIndirectVisualCue Cue;
                Cue.CueType = EBDFRIndirectVisualCueType::Reflection;
                Cue.SourceActor = SourceActor;
                Cue.CueLocation = CueLocation;
                Cue.Strength = CueStrength;

                HandleDetectedCue(Cue, ReflectionBaseAwarenessGain);
                bDetectedAnyCue = true;
                break;
            }
        }

        if (bDetectedAnyCue)
        {
            LastCueTimeBySource.Add(SourceActor, Now);
        }
    }
}

bool UBDFRIndirectVisualPerceptionComponent::IsSourceDirectlyVisible(
    AActor* SourceActor,
    const FVector& ViewerLocation) const
{
    if (!IsValid(SourceActor))
    {
        return false;
    }

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    const FVector TargetLocation = SourceActor->GetActorLocation();
    FCollisionQueryParams Params(SCENE_QUERY_STAT(BDFRIndirectDirectLOS), false);
    Params.AddIgnoredActor(GetOwner());

    FHitResult Hit;
    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        ViewerLocation,
        TargetLocation,
        ECC_Visibility,
        Params);

    return !bHit || Hit.GetActor() == SourceActor;
}

bool UBDFRIndirectVisualPerceptionComponent::IsPointVisible(
    const FVector& ViewerLocation,
    const FVector& Point,
    AActor* IgnoreActor) const
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    FCollisionQueryParams Params(SCENE_QUERY_STAT(BDFRIndirectCueLOS), false);
    Params.AddIgnoredActor(GetOwner());
    if (IsValid(IgnoreActor))
    {
        Params.AddIgnoredActor(IgnoreActor);
    }

    FHitResult Hit;
    return !World->LineTraceSingleByChannel(
        Hit,
        ViewerLocation,
        Point,
        ECC_Visibility,
        Params);
}

void UBDFRIndirectVisualPerceptionComponent::HandleDetectedCue(
    const FBDFRIndirectVisualCue& Cue,
    const float BaseAwarenessGain)
{
    AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!IsValid(Controller) || !IsValid(Cue.SourceActor))
    {
        return;
    }

    UBDFRDifficultyComponent* Difficulty =
        Controller->FindComponentByClass<UBDFRDifficultyComponent>();
    UBDFRAwarenessComponent* Awareness =
        Controller->FindComponentByClass<UBDFRAwarenessComponent>();

    if (!IsValid(Difficulty) || !IsValid(Awareness))
    {
        return;
    }

    const FBDFRDifficultyProfile Profile = Difficulty->GetDifficultyProfile();

    const float AwarenessGain = FMath::Clamp(
        BaseAwarenessGain
        * Cue.Strength
        * Profile.IndirectVisualAwarenessMultiplier,
        0.0f,
        1.0f);

    Awareness->AddAwareness(
        Cue.SourceActor,
        AwarenessGain,
        Cue.CueLocation,
        false,
        false);

    LastCue = Cue;
    OnIndirectVisualCueDetected.Broadcast(Cue);
}
