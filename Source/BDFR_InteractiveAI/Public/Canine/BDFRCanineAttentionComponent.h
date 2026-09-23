#pragma once

#include "CoreMinimal.h"
#include "Canine/BDFRCanineTypes.h"
#include "Components/ActorComponent.h"
#include "BDFRCanineAttentionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FBDFROnCanineAttentionUpdated,
    FBDFRCanineAttentionSnapshot, Snapshot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FBDFROnCanineAttentionStateChanged,
    EBDFRCanineAttentionState, PreviousState,
    EBDFRCanineAttentionState, NewState);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRCanineAttentionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRCanineAttentionComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure, Category = "BDFR|Canine")
    FBDFRCanineAttentionSnapshot GetAttentionSnapshot() const { return Snapshot; }

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Canine")
    FBDFROnCanineAttentionUpdated OnCanineAttentionUpdated;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Canine")
    FBDFROnCanineAttentionStateChanged OnCanineAttentionStateChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.1"))
    float AcousticAttentionHoldSeconds = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.1"))
    float EarInterpSpeed = 14.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.1"))
    float HeadInterpSpeed = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0", ClampMax = "120.0"))
    float MaxEarYawDegrees = 85.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0", ClampMax = "90.0"))
    float MaxEarPitchDegrees = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0", ClampMax = "120.0"))
    float MaxHeadYawDegrees = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0", ClampMax = "90.0"))
    float MaxHeadPitchDegrees = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0"))
    float EnvironmentalScanYawDegrees = 32.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.05"))
    float EnvironmentalScanFrequency = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Canine|Attention", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BarkOnStrongSoundThreshold = 0.80f;

private:
    UPROPERTY(Transient)
    FBDFRCanineAttentionSnapshot Snapshot;

    EBDFRCanineAttentionState PreviousState = EBDFRCanineAttentionState::Relaxed;
    bool bEngageAnimationSent = false;
    float LastBarkTimeSeconds = -1000.0f;

    void UpdateAttention(float DeltaTime);
    void ResolveAttentionTarget(
        EBDFRCanineAttentionState& OutState,
        FVector& OutLocation,
        float& OutAlertness,
        AActor*& OutEngageTarget) const;

    void UpdateAimOffsets(const FVector& FocusLocation, bool bHasFocus, float DeltaTime);
    void PushPresentation(AActor* EngageTarget);
};
