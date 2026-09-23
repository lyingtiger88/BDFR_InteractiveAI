#pragma once

#include "CoreMinimal.h"
#include "Difficulty/BDFRDifficultyTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BDFRDifficultyUnlockSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnDifficultyUnlocked,
    EBDFRDifficultyTier, UnlockedTier);

UCLASS()
class BDFR_INTERACTIVEAI_API UBDFRDifficultyUnlockSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Difficulty")
    void NotifyCampaignCompleted(EBDFRDifficultyTier CompletedTier);

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    bool IsDifficultyUnlocked(EBDFRDifficultyTier Tier) const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    TArray<EBDFRDifficultyTier> GetUnlockedMenuDifficultyTiers() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    bool IsSASUnlocked() const { return bSASUnlocked; }

    UFUNCTION(BlueprintCallable, Category = "BDFR|Difficulty|Debug")
    void ResetDifficultyUnlocks();

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Difficulty")
    FBDFROnDifficultyUnlocked OnDifficultyUnlocked;

private:
    static constexpr const TCHAR* SaveSlotName = TEXT("BDFR_DifficultyUnlocks");
    static constexpr int32 SaveUserIndex = 0;

    bool bSASUnlocked = false;

    void LoadUnlockState();
    void SaveUnlockState() const;
};
