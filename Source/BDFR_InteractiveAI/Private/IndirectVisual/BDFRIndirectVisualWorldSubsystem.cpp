#include "IndirectVisual/BDFRIndirectVisualWorldSubsystem.h"

#include "IndirectVisual/BDFRIndirectVisualSourceComponent.h"
#include "IndirectVisual/BDFRReflectiveSurfaceComponent.h"

void UBDFRIndirectVisualWorldSubsystem::RegisterSource(
    UBDFRIndirectVisualSourceComponent* Source)
{
    if (IsValid(Source))
    {
        Sources.AddUnique(Source);
    }
}

void UBDFRIndirectVisualWorldSubsystem::UnregisterSource(
    UBDFRIndirectVisualSourceComponent* Source)
{
    Sources.RemoveAll(
        [Source](const TWeakObjectPtr<UBDFRIndirectVisualSourceComponent>& Entry)
        {
            return !Entry.IsValid() || Entry.Get() == Source;
        });
}

void UBDFRIndirectVisualWorldSubsystem::RegisterReflectiveSurface(
    UBDFRReflectiveSurfaceComponent* Surface)
{
    if (IsValid(Surface))
    {
        ReflectiveSurfaces.AddUnique(Surface);
    }
}

void UBDFRIndirectVisualWorldSubsystem::UnregisterReflectiveSurface(
    UBDFRReflectiveSurfaceComponent* Surface)
{
    ReflectiveSurfaces.RemoveAll(
        [Surface](const TWeakObjectPtr<UBDFRReflectiveSurfaceComponent>& Entry)
        {
            return !Entry.IsValid() || Entry.Get() == Surface;
        });
}
