#include "Behavior/BDFRCanineAIController.h"

#include "Tracking/BDFRCanineTrackingComponent.h"

ABDFRCanineAIController::ABDFRCanineAIController()
{
    CanineTrackingComponent =
        CreateDefaultSubobject<UBDFRCanineTrackingComponent>(TEXT("BDFRCanineTracking"));
}
