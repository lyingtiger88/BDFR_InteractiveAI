#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Difficulty/BDFRDifficultyTypes.h"
#include "BDFRDifficultyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FBDFROnDifficultyChanged,
    EBDFRDifficultyTier, PreviousTier,
    EBDFRDifficultyTier, NewTier);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRDifficultyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRDifficultyComponent();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Difficulty")
    void SetDifficultyTier(EBDFRDifficultyTier NewTier);

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    EBDFRDifficultyTier GetDifficultyTier() const { return DifficultyTier; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    FBDFRDifficultyProfile GetDifficultyProfile() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    float GetStressGainMultiplier() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    float GetReactionDelaySeconds() const { return GetDifficultyProfile().ReactionDelaySeconds; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    float GetSearchPersistenceSeconds() const { return GetDifficultyProfile().SearchPersistenceSeconds; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    float GetCombatAccuracyMultiplier() const { return GetDifficultyProfile().CombatAccuracyMultiplier; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Difficulty")
    float GetSurrenderResistance() const { return GetDifficultyProfile().SurrenderResistance; }

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Difficulty")
    FBDFROnDifficultyChanged OnDifficultyChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Difficulty")
    EBDFRDifficultyTier DifficultyTier = EBDFRDifficultyTier::Private;
};
