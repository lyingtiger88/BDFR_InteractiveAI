# ProjectIGI Remake integration

BDFR Interactive AI is intentionally independent from ALS Refactored.

## Target dependency layout

```text
ProjectIGI_Remake/
├── Plugins/
│   ├── ALS/                    -> ALS Refactored submodule
│   └── BDFR_InteractiveAI/     -> BDFR plugin submodule
└── Source/ProjectIGI_Remake/
    ├── IGIPlayerCharacter
    ├── IGIEnemyCharacter
    └── IGIBDFRLocomotionAdapter
```

## Recommended enemy inheritance

For the first ProjectIGI enemy implementation:

```text
AAlsCharacter
    └── AIGIEnemyCharacter
            ├── implements IBDFRLocomotionInterface
            └── AIControllerClass = ABDFRAIController (or an IGI subclass)
```

The enemy character remains project-owned. BDFR does not subclass ALS.

## ALS adapter responsibilities

`AIGIEnemyCharacter` (or a dedicated component on it) should implement
`IBDFRLocomotionInterface` and translate generic BDFR requests.

Suggested mappings:

```text
BDFR gait request
  Walking  -> AlsGaitTags::Walking
  Running  -> AlsGaitTags::Running
  Sprint   -> AlsGaitTags::Sprinting

BDFR stance request
  Standing -> AlsStanceTags::Standing
  Crouched -> AlsStanceTags::Crouching

BDFR aiming request
  true/false -> SetDesiredAiming(...)

BDFR look target
  -> AIController SetFocus / ClearFocus
  -> ALS consumes controller/look rotation through the project adapter
```

Do not put ALS headers into the BDFR plugin.

## Perception flow

```text
UAIPerceptionComponent
        │
        ├── Sight
        ├── Hearing
        └── Damage
        │
        ▼
ABDFRAIController
        │
        ▼
UBDFRAwarenessComponent
        │
        ├── Awareness 0..1
        ├── Awareness Level
        ├── Last Known Location
        ├── Has Line Of Sight
        └── Confirmed Target
        │
        ▼
Behavior Tree / Blackboard (next layer)
```

## Project-specific target filtering

`ABDFRAIController::BDFR_ShouldProcessPerceivedActor` is a BlueprintNativeEvent.

ProjectIGI should override it to reject friendly actors and non-target actors using the project's team/faction rules. The BDFR base implementation only rejects the controlled pawn itself.

## Next ProjectIGI integration step

1. Add BDFR as a Git submodule under `Plugins/BDFR_InteractiveAI`.
2. Enable the plugin.
3. Create `AIGIEnemyCharacter : AAlsCharacter`.
4. Implement `IBDFRLocomotionInterface`.
5. Create `AIGIEnemyAIController : ABDFRAIController`.
6. Add faction filtering.
7. Add Blackboard + Behavior Tree for Patrol / Observe / Investigate / Search.
