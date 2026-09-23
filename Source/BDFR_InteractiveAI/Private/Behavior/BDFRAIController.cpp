#include "Behavior/BDFRAIController.h"

#include "Acoustics/BDFRAcousticExposureComponent.h"
#include "Awareness/BDFRAwarenessComponent.h"
#include "Core/BDFRAISettings.h"
#include "Difficulty/BDFRDifficultyComponent.h"
#include "GameFramework/Pawn.h"
#include "IndirectVisual/BDFRIndirectVisualPerceptionComponent.h"
#include "Social/BDFRStressComponent.h"
#include "Tracking/BDFRFootprintTrackingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABDFRAIController::ABDFRAIController()
{
    AwarenessComponent = CreateDefaultSubobject<UBDFRAwarenessComponent>(TEXT("BDFRAwareness"));
    StressComponent = CreateDefaultSubobject<UBDFRStressComponent>(TEXT("BDFRStress"));
    DifficultyComponent = CreateDefaultSubobject<UBDFRDifficultyComponent>(TEXT("BDFRDifficulty"));
    IndirectVisualPerceptionComponent =
        CreateDefaultSubobject<UBDFRIndirectVisualPerceptionComponent>(TEXT("BDFRIndirectVisual"));
    FootprintTrackingComponent =
        CreateDefaultSubobject<UBDFRFootprintTrackingComponent>(TEXT("BDFRFootprintTracking"));

    BDFRPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("BDFRPerception"));
    SetPerceptionComponent(*BDFRPerceptionComponent);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("BDFRSightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("BDFRHearingConfig"));
    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("BDFRDamageConfig"));

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();

    SightConfig->SightRadius = Settings->DefaultSightRadius;
    SightConfig->LoseSightRadius = Settings->DefaultLoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = Settings->DefaultPeripheralVisionHalfAngle;
    SightConfig->SetMaxAge(Settings->DefaultSightMaxAge);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    HearingConfig->HearingRange = Settings->DefaultHearingRange;
    HearingConfig->SetMaxAge(Settings->DefaultHearingMaxAge);
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    BDFRPerceptionComponent->ConfigureSense(*SightConfig);
    BDFRPerceptionComponent->ConfigureSense(*HearingConfig);
    BDFRPerceptionComponent->ConfigureSense(*DamageConfig);
    BDFRPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABDFRAIController::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(BDFRPerceptionComponent))
    {
        BDFRPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
            this,
            &ThisClass::HandleTargetPerceptionUpdated);
    }

    if (IsValid(DifficultyComponent))
    {
        DifficultyComponent->OnDifficultyChanged.AddDynamic(
            this,
            &ThisClass::HandleDifficultyChanged);
    }

    ApplyDifficultyToPerception();
}

void ABDFRAIController::HandleDifficultyChanged(
    const EBDFRDifficultyTier PreviousTier,
    const EBDFRDifficultyTier NewTier)
{
    ApplyDifficultyToPerception();

    if (!IsValid(DifficultyComponent)
        || !DifficultyComponent->ShouldPersistHuntAfterConfirmedTarget())
    {
        CancelPersistentHunt();
    }
}

UBDFRAcousticExposureComponent* ABDFRAIController::GetAcousticExposureComponent() const
{
    const APawn* ControlledPawn = GetPawn();
    return IsValid(ControlledPawn)
        ? ControlledPawn->FindComponentByClass<UBDFRAcousticExposureComponent>()
        : nullptr;
}

void ABDFRAIController::HandleTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus)
{
    if (!IsValid(SourceActor))
    {
        return;
    }

    const FAISenseID HearingSenseId = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

    if (Stimulus.Type == HearingSenseId && Stimulus.WasSuccessfullySensed())
    {
        if (GetCurrentHearingSensitivity() <= KINDA_SMALL_NUMBER)
        {
            return;
        }

        if (IsDistressStimulus(Stimulus))
        {
            HandleDistressStimulus(SourceActor, Stimulus);
            return;
        }

        if (IsAcousticStimulus(Stimulus))
        {
            HandleAcousticStimulus(SourceActor, Stimulus);
        }
    }

    if (!IsValid(AwarenessComponent) || !BDFR_ShouldProcessPerceivedActor(SourceActor))
    {
        return;
    }

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    const FAISenseID SightSenseId = UAISense::GetSenseID(UAISense_Sight::StaticClass());
    const FAISenseID DamageSenseId = UAISense::GetSenseID(UAISense_Damage::StaticClass());

    if (Stimulus.Type == SightSenseId)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            AwarenessComponent->AddAwareness(
                SourceActor,
                Settings->SightAwarenessGain * GetAwarenessGainMultiplier(),
                Stimulus.StimulusLocation,
                false,
                true);
            RefreshPersistentHuntState(SourceActor);
        }
        else
        {
            AwarenessComponent->SetTargetLineOfSight(SourceActor, false);
        }

        return;
    }

    if (Stimulus.Type == HearingSenseId && Stimulus.WasSuccessfullySensed())
    {
        const float EffectiveHearingStrength =
            FMath::Clamp(Stimulus.Strength, 0.1f, 1.0f)
            * GetCurrentHearingSensitivity();

        if (EffectiveHearingStrength > KINDA_SMALL_NUMBER)
        {
            AwarenessComponent->AddAwareness(
                SourceActor,
                Settings->HearingAwarenessGain * EffectiveHearingStrength * GetAwarenessGainMultiplier(),
                Stimulus.StimulusLocation,
                false,
                false);
            RefreshPersistentHuntState(SourceActor);
        }

        return;
    }

    if (Stimulus.Type == DamageSenseId && Stimulus.WasSuccessfullySensed())
    {
        AwarenessComponent->AddAwareness(
            SourceActor,
            Settings->DamageAwarenessGain * GetAwarenessGainMultiplier(),
            Stimulus.StimulusLocation,
            true,
            false);
        RefreshPersistentHuntState(SourceActor);
    }
}

bool ABDFRAIController::BDFR_ShouldProcessPerceivedActor_Implementation(AActor* SourceActor) const
{
    return IsValid(SourceActor) && SourceActor != GetPawn();
}

bool ABDFRAIController::BDFR_ShouldRespondToDistress_Implementation(AActor* SourceActor) const
{
    return IsValid(SourceActor) && SourceActor != GetPawn();
}

void ABDFRAIController::ClearPendingAssistance()
{
    PendingAssistanceTarget = nullptr;
    PendingAssistanceLocation = FVector::ZeroVector;
    PendingAssistanceUrgency = 0.0f;
}

void ABDFRAIController::NotifyPersistentHuntTargetNeutralized(AActor* NeutralizedTarget)
{
    if (NeutralizedTarget == PersistentHuntTarget)
    {
        CancelPersistentHunt();
    }
}

void ABDFRAIController::CancelPersistentHunt()
{
    bPersistentHuntActive = false;
    PersistentHuntTarget = nullptr;
}

void ABDFRAIController::RefreshPersistentHuntState(AActor* SourceActor)
{
    if (!IsValid(SourceActor)
        || !IsValid(DifficultyComponent)
        || !DifficultyComponent->ShouldPersistHuntAfterConfirmedTarget()
        || !IsValid(AwarenessComponent))
    {
        return;
    }

    const FBDFRAwarenessSnapshot Snapshot = AwarenessComponent->GetSnapshot();

    if (Snapshot.bHasConfirmedTarget
        && AwarenessComponent->GetCurrentTarget() == SourceActor)
    {
        bPersistentHuntActive = true;
        PersistentHuntTarget = SourceActor;
    }
}

void ABDFRAIController::ApplyDifficultyToPerception()
{
    if (!IsValid(DifficultyComponent) || !IsValid(BDFRPerceptionComponent))
    {
        return;
    }

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    const FBDFRDifficultyProfile Profile = DifficultyComponent->GetDifficultyProfile();

    if (IsValid(SightConfig))
    {
        SightConfig->SightRadius = Settings->DefaultSightRadius * Profile.SightRadiusMultiplier;
        SightConfig->LoseSightRadius = Settings->DefaultLoseSightRadius * Profile.SightRadiusMultiplier;
        BDFRPerceptionComponent->ConfigureSense(*SightConfig);
    }

    if (IsValid(HearingConfig))
    {
        HearingConfig->HearingRange =
            Settings->DefaultHearingRange
            * Profile.HearingSensitivityMultiplier
            * BDFR_GetSpeciesHearingMultiplier();
        BDFRPerceptionComponent->ConfigureSense(*HearingConfig);
    }

    BDFRPerceptionComponent->RequestStimuliListenerUpdate();
}

float ABDFRAIController::GetCurrentHearingSensitivity() const
{
    const UBDFRAcousticExposureComponent* ExposureComponent = GetAcousticExposureComponent();
    const float ExposureSensitivity = IsValid(ExposureComponent)
        ? ExposureComponent->GetHearingSensitivity()
        : 1.0f;

    const float DifficultySensitivity = IsValid(DifficultyComponent)
        ? DifficultyComponent->GetDifficultyProfile().HearingSensitivityMultiplier
        : 1.0f;

    return FMath::Clamp(
        ExposureSensitivity
        * DifficultySensitivity
        * BDFR_GetSpeciesHearingMultiplier(),
        0.0f,
        3.0f);
}

float ABDFRAIController::GetAwarenessGainMultiplier() const
{
    return IsValid(DifficultyComponent)
        ? DifficultyComponent->GetDifficultyProfile().AwarenessGainMultiplier
        : 1.0f;
}

float ABDFRAIController::GetStressGainMultiplier() const
{
    return IsValid(DifficultyComponent)
        ? DifficultyComponent->GetStressGainMultiplier()
        : 1.0f;
}

bool ABDFRAIController::IsDistressStimulus(const FAIStimulus& Stimulus) const
{
    const FAISenseID HearingSenseId = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

    return Stimulus.Type == HearingSenseId
        && Stimulus.WasSuccessfullySensed()
        && Stimulus.Tag.ToString().StartsWith(TEXT("BDFR.Distress."));
}

bool ABDFRAIController::IsAcousticStimulus(const FAIStimulus& Stimulus) const
{
    const FAISenseID HearingSenseId = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

    return Stimulus.Type == HearingSenseId
        && Stimulus.WasSuccessfullySensed()
        && Stimulus.Tag.ToString().StartsWith(TEXT("BDFR.Acoustic."));
}

void ABDFRAIController::HandleDistressStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
    const float Urgency =
        FMath::Clamp(Stimulus.Strength, 0.0f, 1.0f)
        * GetCurrentHearingSensitivity();

    if (Urgency <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    if (IsValid(StressComponent))
    {
        StressComponent->AddStress(
            GetDistressStressAmount(Stimulus.Tag, Urgency) * GetStressGainMultiplier());
    }

    OnDistressPerceived.Broadcast(
        SourceActor,
        Stimulus.Tag,
        Stimulus.StimulusLocation,
        Urgency);

    if (!BDFR_ShouldRespondToDistress(SourceActor))
    {
        return;
    }

    if (!IsValid(PendingAssistanceTarget) || Urgency >= PendingAssistanceUrgency)
    {
        PendingAssistanceTarget = SourceActor;
        PendingAssistanceLocation = Stimulus.StimulusLocation;
        PendingAssistanceUrgency = Urgency;
    }
}

void ABDFRAIController::HandleAcousticStimulus(AActor* SourceActor, const FAIStimulus& Stimulus)
{
    const float EffectiveStrength =
        FMath::Clamp(Stimulus.Strength, 0.0f, 1.0f)
        * GetCurrentHearingSensitivity();

    if (EffectiveStrength <= KINDA_SMALL_NUMBER)
    {
        return;
    }

    LastHeardAcousticLocation = Stimulus.StimulusLocation;
    LastHeardAcousticTag = Stimulus.Tag;
    LastHeardAcousticStrength = EffectiveStrength;
    LastHeardAcousticTimeSeconds = GetWorld() != nullptr
        ? GetWorld()->GetTimeSeconds()
        : 0.0f;

    const FString AcousticTag = Stimulus.Tag.ToString();

    if (IsValid(StressComponent))
    {
        if (AcousticTag.Contains(TEXT("Explosion")))
        {
            StressComponent->AddStress(0.30f * EffectiveStrength * GetStressGainMultiplier());
        }
        else if (AcousticTag.Contains(TEXT("Gunshot")))
        {
            StressComponent->AddStress(0.12f * EffectiveStrength * GetStressGainMultiplier());
        }
    }

    OnAcousticEventPerceived.Broadcast(
        SourceActor,
        Stimulus.Tag,
        Stimulus.StimulusLocation,
        EffectiveStrength);
}

float ABDFRAIController::GetDistressStressAmount(const FName DistressTag, const float Urgency)
{
    const FString Tag = DistressTag.ToString();

    float BaseStress = 0.12f;

    if (Tag == TEXT("BDFR.Distress.AllyDown"))
    {
        BaseStress = 0.35f;
    }
    else if (Tag == TEXT("BDFR.Distress.BleedingOut"))
    {
        BaseStress = 0.30f;
    }
    else if (Tag == TEXT("BDFR.Distress.Help"))
    {
        BaseStress = 0.20f;
    }
    else if (Tag == TEXT("BDFR.Distress.Panic"))
    {
        BaseStress = 0.25f;
    }

    return FMath::Clamp(BaseStress * FMath::Max(0.25f, Urgency), 0.0f, 1.0f);
}
