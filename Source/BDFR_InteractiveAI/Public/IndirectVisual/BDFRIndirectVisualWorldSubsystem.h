#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BDFRIndirectVisualWorldSubsystem.generated.h"

class UBDFRIndirectVisualSourceComponent;
class UBDFRReflectiveSurfaceComponent;

UCLASS()
class BDFR_INTERACTIVEAI_API UBDFRIndirectVisualWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    void RegisterSource(UBDFRIndirectVisualSourceComponent* Source);
    void UnregisterSource(UBDFRIndirectVisualSourceComponent* Source);

    void RegisterReflectiveSurface(UBDFRReflectiveSurfaceComponent* Surface);
    void UnregisterReflectiveSurface(UBDFRReflectiveSurfaceComponent* Surface);

    const TArray<TWeakObjectPtr<UBDFRIndirectVisualSourceComponent>>& GetSources() const { return Sources; }
    const TArray<TWeakObjectPtr<UBDFRReflectiveSurfaceComponent>>& GetReflectiveSurfaces() const { return ReflectiveSurfaces; }

private:
    TArray<TWeakObjectPtr<UBDFRIndirectVisualSourceComponent>> Sources;
    TArray<TWeakObjectPtr<UBDFRReflectiveSurfaceComponent>> ReflectiveSurfaces;
};
