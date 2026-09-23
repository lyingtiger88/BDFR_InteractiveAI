#include "Difficulty/BDFRDifficultyComponent.h"

#include "Core/BDFRAISettings.h"
#include "Difficulty/BDFRDifficultyUnlockSubsystem.h"
#include "Engine/GameInstance.h"

UBDFRDifficultyComponent::UBDFRDifficultyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBDFRDifficultyComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bUseProjectDefaultDifficulty)
    {
        const EBDFRDifficultyTier ProjectDefault =
            GetDefault<UBDFRAISettings>()->DefaultDifficultyTier;

        DifficultyTier = CanSelectDifficultyTier(ProjectDefault)
            ? ProjectDefault
            : EBDFRDifficultyTier::Commando;
    }
    else if (!CanSelectDifficultyTier(DifficultyTier))
    {
        DifficultyTier = EBDFRDifficultyTier::Commando;
    }
}

void UBDFRDifficultyComponent::SetDifficultyTier(const EBDFRDifficultyTier NewTier)
{
    if (!CanSelectDifficultyTier(NewTier))
    {
        return;
    }

    if (DifficultyTier == NewTier && !bUseProjectDefaultDifficulty)
    {
        return;
    }

    const EBDFRDifficultyTier PreviousTier = DifficultyTier;
    bUseProjectDefaultDifficulty = false;
    DifficultyTier = NewTier;
    OnDifficultyChanged.Broadcast(PreviousTier, DifficultyTier);
}

bool UBDFRDifficultyComponent::CanSelectDifficultyTier(
    const EBDFRDifficultyTier Tier) const
{
    if (Tier != EBDFRDifficultyTier::SAS)
    {
        return true;
    }

    const UWorld* World = GetWorld();
    const UGameInstance* GameInstance = IsValid(World) ? World->GetGameInstance() : nullptr;
    const UBDFRDifficultyUnlockSubsystem* UnlockSubsystem =
        IsValid(GameInstance)
            ? GameInstance->GetSubsystem<UBDFRDifficultyUnlockSubsystem>()
            : nullptr;

    return IsValid(UnlockSubsystem) && UnlockSubsystem->IsDifficultyUnlocked(Tier);
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

        case EBDFRDifficultyTier::SAS:
            return Settings->SASDifficulty;

        case EBDFRDifficultyTier::Private:
        default:
            return Settings->PrivateDifficulty;
    }
}

float UBDFRDifficultyComponent::GetStressGainMultiplier() const
{
    return 1.0f - FMath::Clamp(GetDifficultyProfile().StressResistance, 0.0f, 1.0f);
}
