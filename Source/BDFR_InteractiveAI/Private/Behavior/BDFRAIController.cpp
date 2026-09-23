#include "Behavior/BDFRAIController.h"

#include "Awareness/BDFRAwarenessComponent.h"
#include "Core/BDFRAISettings.h"
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
}

void ABDFRAIController::HandleTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus)
{
    if (!IsValid(SourceActor) || !IsValid(AwarenessComponent) || !BDFR_ShouldProcessPerceivedActor(SourceActor))
    {
        return;
    }

    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    const FAISenseID SightSenseId = UAISense::GetSenseID(UAISense_Sight::StaticClass());
    const FAISenseID HearingSenseId = UAISense::GetSenseID(UAISense_Hearing::StaticClass());
    const FAISenseID DamageSenseId = UAISense::GetSenseID(UAISense_Damage::StaticClass());

    if (Stimulus.Type == SightSenseId)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            AwarenessComponent->AddAwareness(
                SourceActor,
                Settings->SightAwarenessGain,
                Stimulus.StimulusLocation,
                false,
                true);
        }
        else
        {
            AwarenessComponent->SetTargetLineOfSight(SourceActor, false);
        }

        return;
    }

    if (Stimulus.Type == HearingSenseId && Stimulus.WasSuccessfullySensed())
    {
        AwarenessComponent->AddAwareness(
            SourceActor,
            Settings->HearingAwarenessGain,
            Stimulus.StimulusLocation,
            false,
            false);
        return;
    }

    if (Stimulus.Type == DamageSenseId && Stimulus.WasSuccessfullySensed())
    {
        AwarenessComponent->AddAwareness(
            SourceActor,
            Settings->DamageAwarenessGain,
            Stimulus.StimulusLocation,
            true,
            false);
    }
}

bool ABDFRAIController::BDFR_ShouldProcessPerceivedActor_Implementation(AActor* SourceActor) const
{
    return IsValid(SourceActor) && SourceActor != GetPawn();
}
