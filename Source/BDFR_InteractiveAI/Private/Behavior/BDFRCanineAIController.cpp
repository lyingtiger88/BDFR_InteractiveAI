#include "Behavior/BDFRCanineAIController.h"

#include "Canine/BDFRCanineAttentionComponent.h"
#include "Tracking/BDFRCanineTrackingComponent.h"

ABDFRCanineAIController::ABDFRCanineAIController()
{
    CanineTrackingComponent =
        CreateDefaultSubobject<UBDFRCanineTrackingComponent>(TEXT("BDFRCanineTracking"));
    CanineAttentionComponent =
        CreateDefaultSubobject<UBDFRCanineAttentionComponent>(TEXT("BDFRCanineAttention"));
}
