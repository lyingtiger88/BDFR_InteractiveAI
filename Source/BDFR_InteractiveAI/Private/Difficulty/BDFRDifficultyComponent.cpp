#include "Difficulty/BDFRDifficultyComponent.h"

#include "Core/BDFRAISettings.h"

UBDFRDifficultyComponent::UBDFRDifficultyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBDFRDifficultyComponent::SetDifficultyTier(const EBDFRDifficultyTier NewTier)
{
    if (DifficultyTier == NewTier)
    {
        return;
    }

    const EBDFRDifficultyTier PreviousTier = DifficultyTier;
    DifficultyTier = NewTier;
    OnDifficultyChanged.Broadcast(PreviousTier, DifficultyTier);
}

FBDFRDifficultyProfile UBDFRDifficultyComponent::GetDifficultyProfile() const
{
    const UBDFRAISettings* Settings = GetDefault<UBDFRAISettings>();

    switch (DifficultyTier)
    {
        case EBDFRDifficultyTier::Recruit:
            return Settings->RecruitDifficulty;

        case EBDFRDifficultyTier::Sergeant:
            return Settings->SergeantDifficulty;

        case EBDFRDifficultyTier::Commando:
            return Settings->CommandoDifficulty;

        case EBDFRDifficultyTier::Private:
        default:
            return Settings->PrivateDifficulty;
    }
}

float UBDFRDifficultyComponent::GetStressGainMultiplier() const
{
    return 1.0f - FMath::Clamp(GetDifficultyProfile().StressResistance, 0.0f, 1.0f);
}
