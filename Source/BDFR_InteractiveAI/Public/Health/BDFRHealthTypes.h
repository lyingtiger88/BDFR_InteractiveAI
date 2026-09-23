#pragma once

#include "CoreMinimal.h"
#include "BDFRHealthTypes.generated.h"

UENUM(BlueprintType)
enum class EBDFRHealthState : uint8
{
    Healthy       UMETA(DisplayName = "Healthy"),
    Wounded       UMETA(DisplayName = "Wounded"),
    Critical      UMETA(DisplayName = "Critical"),
    Incapacitated UMETA(DisplayName = "Incapacitated"),
    Dead          UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EBDFRStressState : uint8
{
    Calm       UMETA(DisplayName = "Calm"),
    Concerned  UMETA(DisplayName = "Concerned"),
    Stressed   UMETA(DisplayName = "Stressed"),
    Panicked   UMETA(DisplayName = "Panicked")
};

UENUM(BlueprintType)
enum class EBDFRDistressType : uint8
{
    PainShout   UMETA(DisplayName = "Pain Shout"),
    HelpRequest UMETA(DisplayName = "Help Request"),
    AllyDown    UMETA(DisplayName = "Ally Down"),
    BleedingOut UMETA(DisplayName = "Bleeding Out"),
    Panic       UMETA(DisplayName = "Panic")
};

USTRUCT(BlueprintType)
struct BDFR_INTERACTIVEAI_API FBDFRHealthSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    EBDFRHealthState HealthState = EBDFRHealthState::Healthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    bool bBleeding = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    float BleedRatePerSecond = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    bool bCanMove = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BDFR|Health")
    bool bCanFight = true;
};
