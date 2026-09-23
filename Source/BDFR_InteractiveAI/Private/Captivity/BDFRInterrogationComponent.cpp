#include "Captivity/BDFRInterrogationComponent.h"

#include "Core/BDFRAISettings.h"

UBDFRInterrogationComponent::UBDFRInterrogationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

EBDFRInterrogationOutcome UBDFRInterrogationComponent::ApplyApproach(
    const EBDFRInterrogationApproach Approach,
    const float Intensity)
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();
    if (!Settings->bEnableInterrogationSystem)
    {
        return EBDFRInterrogationOutcome::NoResponse;
    }

    if (Approach == EBDFRInterrogationApproach::CoercivePressure
        && !Settings->bEnableCoerciveInterrogation)
    {
        return EBDFRInterrogationOutcome::NoResponse;
    }

    const float I = FMath::Clamp(Intensity, 0.0f, 1.0f);

    switch (Approach)
    {
        case EBDFRInterrogationApproach::Question:
            Rapport = FMath::Clamp(Rapport + 0.08f * I, 0.0f, 1.0f);
            PsychologicalStrain = FMath::Clamp(PsychologicalStrain + 0.03f * I, 0.0f, 1.0f);
            break;

        case EBDFRInterrogationApproach::PresentEvidence:
            EvidencePressure = FMath::Clamp(EvidencePressure + 0.22f * I, 0.0f, 1.0f);
            PsychologicalStrain = FMath::Clamp(PsychologicalStrain + 0.06f * I, 0.0f, 1.0f);
            break;

        case EBDFRInterrogationApproach::Bargain:
            Rapport = FMath::Clamp(Rapport + 0.18f * I, 0.0f, 1.0f);
            Fear = FMath::Clamp(Fear - 0.04f * I, 0.0f, 1.0f);
            break;

        case EBDFRInterrogationApproach::Intimidate:
            Fear = FMath::Clamp(Fear + 0.20f * I, 0.0f, 1.0f);
            Rapport = FMath::Clamp(Rapport - 0.08f * I, 0.0f, 1.0f);
            PsychologicalStrain = FMath::Clamp(PsychologicalStrain + 0.12f * I, 0.0f, 1.0f);
            break;

        case EBDFRInterrogationApproach::CoercivePressure:
            Fear = FMath::Clamp(Fear + 0.30f * I, 0.0f, 1.0f);
            Rapport = FMath::Clamp(Rapport - 0.18f * I, 0.0f, 1.0f);
            PsychologicalStrain = FMath::Clamp(PsychologicalStrain + 0.28f * I, 0.0f, 1.0f);
            InformationReliability = FMath::Clamp(InformationReliability - 0.22f * I, 0.0f, 1.0f);
            break;
    }

    const float ComplianceScore = FMath::Clamp(
        (Rapport * 0.35f)
        + (EvidencePressure * 0.35f)
        + (Fear * 0.25f)
        + (PsychologicalStrain * 0.10f)
        - (Resistance * 0.45f),
        0.0f,
        1.0f);

    const float FalseStatementRisk = FMath::Clamp(
        (1.0f - InformationReliability)
        + PsychologicalStrain * 0.35f
        + (Approach == EBDFRInterrogationApproach::CoercivePressure ? 0.25f * I : 0.0f),
        0.0f,
        1.0f);

    const EBDFRInterrogationOutcome Outcome =
        ResolveOutcome(Approach, I, ComplianceScore, FalseStatementRisk);

    if (Outcome == EBDFRInterrogationOutcome::PartialDisclosure)
    {
        DisclosureLevel = FMath::Min(DisclosureLevel + 1, MaxDisclosureLevel);
    }
    else if (Outcome == EBDFRInterrogationOutcome::FullDisclosure)
    {
        DisclosureLevel = MaxDisclosureLevel;
    }

    OnInterrogationOutcome.Broadcast(Approach, Outcome, DisclosureLevel);
    return Outcome;
}

void UBDFRInterrogationComponent::AddEvidencePressure(const float Amount)
{
    EvidencePressure = FMath::Clamp(EvidencePressure + Amount, 0.0f, 1.0f);
}

void UBDFRInterrogationComponent::SetResistance(const float NewResistance)
{
    Resistance = FMath::Clamp(NewResistance, 0.0f, 1.0f);
}

FBDFRInterrogationSnapshot UBDFRInterrogationComponent::GetSnapshot() const
{
    FBDFRInterrogationSnapshot Snapshot;
    Snapshot.Resistance = Resistance;
    Snapshot.Fear = Fear;
    Snapshot.Rapport = Rapport;
    Snapshot.EvidencePressure = EvidencePressure;
    Snapshot.PsychologicalStrain = PsychologicalStrain;
    Snapshot.InformationReliability = InformationReliability;
    Snapshot.DisclosureLevel = DisclosureLevel;
    return Snapshot;
}

EBDFRInterrogationOutcome UBDFRInterrogationComponent::ResolveOutcome(
    const EBDFRInterrogationApproach Approach,
    const float Intensity,
    const float ComplianceScore,
    const float FalseStatementRisk) const
{
    // These are intentionally fictional gameplay tuning thresholds, not real-world interrogation guidance.
    if (PsychologicalStrain >= 0.95f)
    {
        return FalseStatementRisk >= 0.45f
            ? EBDFRInterrogationOutcome::FalseStatement
            : EBDFRInterrogationOutcome::Breakdown;
    }

    if (FalseStatementRisk >= 0.70f && ComplianceScore >= 0.45f)
    {
        return EBDFRInterrogationOutcome::FalseStatement;
    }

    if (ComplianceScore >= 0.80f)
    {
        return EBDFRInterrogationOutcome::FullDisclosure;
    }

    if (ComplianceScore >= 0.55f)
    {
        return EBDFRInterrogationOutcome::PartialDisclosure;
    }

    if (ComplianceScore < 0.20f)
    {
        return EBDFRInterrogationOutcome::Refusal;
    }

    return EBDFRInterrogationOutcome::NoResponse;
}
