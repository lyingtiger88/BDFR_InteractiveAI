# ProjectIGI Remake integration

BDFR Interactive AI is intentionally independent from ALS Refactored.

ProjectIGI owns the adapters; BDFR owns reusable AI/tracking systems.

## Dependency layout

```text
ProjectIGI_Remake/
├── Plugins/
│   ├── ALS/                    -> ALS Refactored submodule
│   └── BDFR_InteractiveAI/     -> BDFR plugin submodule
└── Source/ProjectIGI_Remake/
    ├── IGIPlayerCharacter
    ├── Tracking/
    │   └── IGITrackingSurfaceComponent
    └── AI/
        ├── IGIEnemyAIController
        └── IGIDogAIController
```

The game module depends on both `ALS` and `BDFR_InteractiveAI`. BDFR itself does not include ALS
headers and can be reused by projects with different character frameworks.

## Player tracking integration

`AIGIPlayerCharacter : AAlsCharacter` owns:

```text
UBDFRTrackEmitterComponent
UIGITrackingSurfaceComponent
```

The emitter produces logical:

- footprint samples by movement distance,
- scent samples by time interval.

The ProjectIGI surface component performs a downward Physical Material trace and maps the project's
physical surfaces to `EBDFRTrackSurfaceType`.

ProjectIGI surface convention:

| Physical Surface | BDFR surface |
|---|---|
| SurfaceType1 / Dirt | Dirt |
| SurfaceType2 / Mud | Mud |
| SurfaceType3 / Snow | Snow |
| SurfaceType4 / Sand | Sand |
| SurfaceType5 / Grass | Grass |
| SurfaceType6 / Concrete | Concrete |
| SurfaceType7 / Metal | Metal |
| SurfaceType8 / Water | Water |

This keeps BDFR independent from any particular set of project Physical Material assets.

## Human enemy integration

ProjectIGI provides:

`AIGIEnemyAIController : ABDFRAIController`

The base BDFR controller already owns:

- awareness,
- difficulty,
- Sight / Hearing / Damage perception,
- indirect visual perception,
- footprint tracking.

The ProjectIGI controller filters hostile perception to `AIGIPlayerCharacter`.

Footprint tracking remains difficulty-gated by `FBDFRDifficultyProfile::bCanTrackFootprints`.

## Canine integration

ProjectIGI provides:

`AIGIDogAIController : ABDFRCanineAIController`

The BDFR canine controller adds:

- `UBDFRCanineTrackingComponent`,
- `UBDFRCanineAttentionComponent`,
- species hearing multiplier.

The ProjectIGI subclass uses the same player-target filter as human enemies.

A dog Pawn/Character can therefore remain project-owned while BDFR supplies scent acquisition,
trail following state, awareness integration, and attention data.

## Track flow

```text
AIGIPlayerCharacter
        |
        | footprint / scent sample
        v
UBDFRTrackingWorldSubsystem
        |
        +--------------------+
        |                    |
        v                    v
Human footprint         Canine scent
tracker                 tracker
        |                    |
        v                    v
next trail location     next scent location
        |                    |
        +---------+----------+
                  |
                  v
        Project Behavior Tree / State Tree
```

BDFR stores track samples as lightweight structs rather than spawning one Actor per track point.

## Behavior integration

Human AI can query:

```text
GetFootprintTrackingComponent()
HasActiveFootprintTrail()
GetTrackedActor()
GetNextTrackLocation()
GetLastDetectedSample()
```

Canine AI can query:

```text
GetCanineTrackingComponent()
HasScentTarget()
GetScentTarget()
GetNextScentLocation()
GetLastScentSample()
```

The host project's Behavior Tree/State Tree should decide when to move to these locations, search,
give up, alert allies, or transition to direct pursuit.

## ALS adapter responsibilities

For future ALS-driven enemy Characters, keep the adapter in ProjectIGI.

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

Do not add ALS as a BDFR dependency.

## Visible footprints

The logical track system does not require decals.

Projects can subscribe to:

`UBDFRTrackEmitterComponent::OnTrackSampleEmitted`

and render mud/snow/wet/blood footprint decals or Niagara effects. The presentation layer should not
become the authoritative AI tracking state.

## Current ProjectIGI source

The integration is implemented in the ProjectIGI repository:

- `Source/ProjectIGI_Remake/IGIPlayerCharacter.*`
- `Source/ProjectIGI_Remake/Tracking/IGITrackingSurfaceComponent.*`
- `Source/ProjectIGI_Remake/AI/IGIEnemyAIController.*`
- `Source/ProjectIGI_Remake/AI/IGIDogAIController.*`
- `docs/TRACKING_SCENT.md`
