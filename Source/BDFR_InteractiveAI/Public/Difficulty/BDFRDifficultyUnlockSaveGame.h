#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BDFRDifficultyUnlockSaveGame.generated.h"

UCLASS()
class BDFR_INTERACTIVEAI_API UBDFRDifficultyUnlockSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
    bool bSASUnlocked = false;
};
