#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Captivity/BDFRCaptivityTypes.h"
#include "BDFRCaptivityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FBDFROnCaptivityStateChanged,
    EBDFRCaptivityState, PreviousState,
    EBDFRCaptivityState, NewState);

UCLASS(ClassGroup = (BDFR), meta = (BlueprintSpawnableComponent))
class BDFR_INTERACTIVEAI_API UBDFRCaptivityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBDFRCaptivityComponent();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    bool BeginSurrender(AActor* InCaptor);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    bool Restrain(AActor* InCaptor);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    bool SetAsHostage(AActor* InCaptor);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    bool BeginInterrogation(AActor* InCaptor);

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    void Release();

    UFUNCTION(BlueprintCallable, Category = "BDFR|Captivity")
    bool BeginEscape();

    UFUNCTION(BlueprintPure, Category = "BDFR|Captivity")
    bool IsCaptive() const;

    UFUNCTION(BlueprintPure, Category = "BDFR|Captivity")
    bool IsRestrained() const { return bRestrained; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Captivity")
    EBDFRCaptivityState GetCaptivityState() const { return State; }

    UFUNCTION(BlueprintPure, Category = "BDFR|Captivity")
    FBDFRCaptivitySnapshot GetSnapshot() const;

    UPROPERTY(BlueprintAssignable, Category = "BDFR|Captivity")
    FBDFROnCaptivityStateChanged OnCaptivityStateChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Captivity")
    bool bCanBeCaptured = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BDFR|Captivity")
    bool bCanAttemptEscape = true;

private:
    UPROPERTY(Transient)
    EBDFRCaptivityState State = EBDFRCaptivityState::Free;

    UPROPERTY(Transient)
    TObjectPtr<AActor> Captor;

    UPROPERTY(Transient)
    bool bRestrained = false;

    void SetState(EBDFRCaptivityState NewState);
};
