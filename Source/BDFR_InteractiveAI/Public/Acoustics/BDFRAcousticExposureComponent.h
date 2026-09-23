#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Acoustics/BDFRAcousticTypes.h"
#include "BDFRAcousticExposureComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FBDFROnHearingStateChanged,
    EBDFRHearingState, PreviousState,
    EBDFRHearingState, NewState,
    float, Exposure);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FBDFROnAcousticExposure,
    EBDFRAcousticEventType, EventType,
    float, ExposureAmount,
    FVector, SourceLocation);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRAcousticExposureComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRAcousticExposureComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "BDFR|Acoustics")
    void ApplyExposure(float Amount, FVector SourceLocation, EBDFRAcousticEventType EventType);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Acoustics")
    void ClearExposure();

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    float GetExposure() const { return Exposure; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    float GetHearingSensitivity() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    EBDFRHearingState GetHearingState() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    bool IsTemporarilyDeafened() const { return GetHearingState() == EBDFRHearingState::Deafened; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Acoustics")
    FBDFRAcousticExposureSnapshot GetSnapshot() const;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Acoustics")
    FBDFROnHearingStateChanged OnHearingStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Acoustics")
    FBDFROnAcousticExposure OnAcousticExposure;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics", meta = (ClampMin = "0.0"))
    float ExposureDecayPerSecond = 0.10f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float RingingThreshold = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ImpairedThreshold = 0.50f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics|Thresholds", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DeafenedThreshold = 0.85f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Acoustics", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinimumImpairedSensitivity = 0.15f;

private:
    UPROPERTY(Transient)
    float Exposure = 0.0f;

    UPROPERTY(Transient)
    FVector LastExposureLocation = FVector::ZeroVector;

    UPROPERTY(Transient)
    EBDFRAcousticEventType LastEventType = EBDFRAcousticEventType::Impact;

    UPROPERTY(Transient)
    bool bHearingSenseDisabled = false;

    EBDFRHearingState CachedState = EBDFRHearingState::Normal;

    void SetExposure(float NewExposure);
    void RefreshStateAndSense();
    void SetHearingSenseEnabled(bool bEnabled);
};
