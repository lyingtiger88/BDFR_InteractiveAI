#include "Difficulty/BDFRDifficultyUnlockSubsystem.h"

#include "Difficulty/BDFRDifficultyUnlockSaveGame.h"
#include "Kismet/GameplayStatics.h"

FString UBDFRDifficultyUnlockSubsystem::GetSaveSlotName()
{
    return TEXT("BDFR_DifficultyUnlocks");
}

void UBDFRDifficultyUnlockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadUnlockState();
}

void UBDFRDifficultyUnlockSubsystem::NotifyCampaignCompleted(
    const EBDFRDifficultyTier CompletedTier)
{
    if (CompletedTier != EBDFRDifficultyTier::Commando || bSASUnlocked)
    {
        return;
    }

    bSASUnlocked = true;
    SaveUnlockState();
    OnDifficultyUnlocked.Broadcast(EBDFRDifficultyTier::SAS);
}

bool UBDFRDifficultyUnlockSubsystem::IsDifficultyUnlocked(
    const EBDFRDifficultyTier Tier) const
{
    return Tier != EBDFRDifficultyTier::SAS || bSASUnlocked;
}

TArray<EBDFRDifficultyTier> UBDFRDifficultyUnlockSubsystem::GetUnlockedMenuDifficultyTiers() const
{
    TArray<EBDFRDifficultyTier> Tiers;
    Tiers.Reserve(bSASUnlocked ? 5 : 4);

    Tiers.Add(EBDFRDifficultyTier::Recruit);
    Tiers.Add(EBDFRDifficultyTier::Private);
    Tiers.Add(EBDFRDifficultyTier::Sergeant);
    Tiers.Add(EBDFRDifficultyTier::Commando);

    if (bSASUnlocked)
    {
        Tiers.Add(EBDFRDifficultyTier::SAS);
    }

    return Tiers;
}

void UBDFRDifficultyUnlockSubsystem::ResetDifficultyUnlocks()
{
    bSASUnlocked = false;
    SaveUnlockState();
}

void UBDFRDifficultyUnlockSubsystem::LoadUnlockState()
{
    bSASUnlocked = false;

    if (!UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(), SaveUserIndex))
    {
        return;
    }

    UBDFRDifficultyUnlockSaveGame* SaveGame =
        Cast<UBDFRDifficultyUnlockSaveGame>(
            UGameplayStatics::LoadGameFromSlot(GetSaveSlotName(), SaveUserIndex));

    if (IsValid(SaveGame))
    {
        bSASUnlocked = SaveGame->bSASUnlocked;
    }
}

void UBDFRDifficultyUnlockSubsystem::SaveUnlockState() const
{
    UBDFRDifficultyUnlockSaveGame* SaveGame =
        Cast<UBDFRDifficultyUnlockSaveGame>(
            UGameplayStatics::CreateSaveGameObject(
                UBDFRDifficultyUnlockSaveGame::StaticClass()));

    if (!IsValid(SaveGame))
    {
        return;
    }

    SaveGame->bSASUnlocked = bSASUnlocked;
    UGameplayStatics::SaveGameToSlot(SaveGame, GetSaveSlotName(), SaveUserIndex);
}
