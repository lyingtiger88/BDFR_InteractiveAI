#include "Core/BDFRAISettings.h"

UBDFRAISettings::UBDFRAISettings()
{
    RecruitDifficulty.Tier = EBDFRDifficultyTier::Recruit;
    RecruitDifficulty.SightRadiusMultiplier = 0.75f;
    RecruitDifficulty.HearingSensitivityMultiplier = 0.75f;
    RecruitDifficulty.AwarenessGainMultiplier = 0.65f;
    RecruitDifficulty.ReactionDelaySeconds = 1.10f;
    RecruitDifficulty.SearchPersistenceSeconds = 10.0f;
    RecruitDifficulty.StressResistance = 0.20f;
    RecruitDifficulty.AssistanceBravery = 0.30f;
    RecruitDifficulty.SurrenderResistance = 0.25f;
    RecruitDifficulty.AcousticRecoveryMultiplier = 0.80f;
    RecruitDifficulty.CombatAccuracyMultiplier = 0.70f;
    RecruitDifficulty.CoverDecisionMultiplier = 0.70f;
    RecruitDifficulty.bPersistentHuntAfterConfirmedTarget = false;

    PrivateDifficulty.Tier = EBDFRDifficultyTier::Private;
    PrivateDifficulty.SightRadiusMultiplier = 1.00f;
    PrivateDifficulty.HearingSensitivityMultiplier = 1.00f;
    PrivateDifficulty.AwarenessGainMultiplier = 1.00f;
    PrivateDifficulty.ReactionDelaySeconds = 0.55f;
    PrivateDifficulty.SearchPersistenceSeconds = 18.0f;
    PrivateDifficulty.StressResistance = 0.45f;
    PrivateDifficulty.AssistanceBravery = 0.50f;
    PrivateDifficulty.SurrenderResistance = 0.50f;
    PrivateDifficulty.AcousticRecoveryMultiplier = 1.00f;
    PrivateDifficulty.CombatAccuracyMultiplier = 1.00f;
    PrivateDifficulty.CoverDecisionMultiplier = 1.00f;
    PrivateDifficulty.bPersistentHuntAfterConfirmedTarget = false;

    SergeantDifficulty.Tier = EBDFRDifficultyTier::Sergeant;
    SergeantDifficulty.SightRadiusMultiplier = 1.15f;
    SergeantDifficulty.HearingSensitivityMultiplier = 1.10f;
    SergeantDifficulty.AwarenessGainMultiplier = 1.20f;
    SergeantDifficulty.ReactionDelaySeconds = 0.30f;
    SergeantDifficulty.SearchPersistenceSeconds = 28.0f;
    SergeantDifficulty.StressResistance = 0.70f;
    SergeantDifficulty.AssistanceBravery = 0.72f;
    SergeantDifficulty.SurrenderResistance = 0.75f;
    SergeantDifficulty.AcousticRecoveryMultiplier = 1.20f;
    SergeantDifficulty.CombatAccuracyMultiplier = 1.18f;
    SergeantDifficulty.CoverDecisionMultiplier = 1.20f;
    SergeantDifficulty.bPersistentHuntAfterConfirmedTarget = false;

    CommandoDifficulty.Tier = EBDFRDifficultyTier::Commando;
    CommandoDifficulty.SightRadiusMultiplier = 1.30f;
    CommandoDifficulty.HearingSensitivityMultiplier = 1.20f;
    CommandoDifficulty.AwarenessGainMultiplier = 1.40f;
    CommandoDifficulty.ReactionDelaySeconds = 0.16f;
    CommandoDifficulty.SearchPersistenceSeconds = 40.0f;
    CommandoDifficulty.StressResistance = 0.88f;
    CommandoDifficulty.AssistanceBravery = 0.88f;
    CommandoDifficulty.SurrenderResistance = 0.92f;
    CommandoDifficulty.AcousticRecoveryMultiplier = 1.40f;
    CommandoDifficulty.CombatAccuracyMultiplier = 1.32f;
    CommandoDifficulty.CoverDecisionMultiplier = 1.35f;
    CommandoDifficulty.bPersistentHuntAfterConfirmedTarget = false;

    SASDifficulty.Tier = EBDFRDifficultyTier::SAS;
    SASDifficulty.SightRadiusMultiplier = 1.45f;
    SASDifficulty.HearingSensitivityMultiplier = 1.35f;
    SASDifficulty.AwarenessGainMultiplier = 1.65f;
    SASDifficulty.ReactionDelaySeconds = 0.08f;
    SASDifficulty.SearchPersistenceSeconds = 120.0f;
    SASDifficulty.StressResistance = 0.97f;
    SASDifficulty.AssistanceBravery = 0.95f;
    SASDifficulty.SurrenderResistance = 1.00f;
    SASDifficulty.AcousticRecoveryMultiplier = 1.60f;
    SASDifficulty.CombatAccuracyMultiplier = 1.45f;
    SASDifficulty.CoverDecisionMultiplier = 1.50f;
    SASDifficulty.bPersistentHuntAfterConfirmedTarget = true;
}
