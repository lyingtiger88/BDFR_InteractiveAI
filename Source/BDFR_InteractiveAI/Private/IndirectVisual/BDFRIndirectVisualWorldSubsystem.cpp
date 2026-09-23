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
    Sources.Remove(Source);
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
    ReflectiveSurfaces.Remove(Surface);
}
