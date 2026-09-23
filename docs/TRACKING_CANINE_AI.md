# Footprint Tracking and Canine AI

BDFR provides two related but separate tracking systems:

1. **Human footprint tracking** for higher AI difficulty tiers.
2. **Canine scent tracking** with enhanced hearing.

## Difficulty gating

Human footprint tracking is enabled by default for:

- Sergeant
- Commando
- SAS

It is disabled for:

- Recruit
- Private

Initial tracking tuning:

| Tier | Footprint Tracking | Sensitivity | Max Footprint Age |
|---|:---:|---:|---:|
| Recruit | No | 0.50x | 20 s |
| Private | No | 0.75x | 35 s |
| Sergeant | Yes | 1.00x | 75 s |
| Commando | Yes | 1.30x | 120 s |
| SAS | Yes | 1.60x | 180 s |

The age values are gameplay defaults and can be changed in Project Settings.

## Track emission

Characters that can be tracked use:

`UBDFRTrackEmitterComponent`

It emits two independent trail types:

```text
Footprint
Scent
```

Footprints are emitted by movement distance.

Scent samples are emitted by time interval.

Default behavior:

```text
move ~90 cm
    -> footprint sample

every ~1 second
    -> scent sample
```

Each sample stores:

- source actor,
- world location,
- movement direction,
- time,
- strength,
- sequence number,
- surface type.

Footprint locations are projected onto the ground with a visibility trace.

## Surface response

BDFR includes generic gameplay surface categories:

```text
Default
Dirt
Mud
Snow
Sand
Grass
Concrete
Metal
Water
```

Initial footprint multipliers:

| Surface | Footprint |
|---|---:|
| Mud | 1.00 |
| Snow | 1.00 |
| Sand | 0.90 |
| Dirt | 0.80 |
| Grass | 0.45 |
| Concrete | 0.18 |
| Metal | 0.08 |
| Water | 0.00 |

Initial scent multipliers:

| Surface | Scent |
|---|---:|
| Grass | 1.00 |
| Dirt | 0.95 |
| Mud | 0.90 |
| Sand | 0.80 |
| Snow | 0.75 |
| Concrete | 0.70 |
| Metal | 0.65 |
| Water | 0.25 |

These values are intentionally gameplay-oriented rather than a physical odor simulation.

Projects can switch the current surface category through:

`SetSurfaceType()`

This can later be driven by Unreal Physical Materials.

## Human footprint tracking

Every `ABDFRAIController` owns:

`UBDFRFootprintTrackingComponent`

The component checks the current BDFR difficulty profile.

When enabled, it:

1. searches for nearby recent footprint samples,
2. checks field of view,
3. checks visibility of the footprint point,
4. selects a valid footprint,
5. raises awareness toward the source actor,
6. stores the source and next track location,
7. searches for a newer sequence sample from the same source.

Exposed state:

```text
HasActiveFootprintTrail()
GetTrackedActor()
GetNextTrackLocation()
GetLastDetectedSample()
```

Suggested Behavior Tree flow:

```text
Footprint found
    |
    v
Investigate Track
    |
    v
Move to NextTrackLocation
    |
    v
Find newer footprint
    |
    +--> found -> continue
    |
    +--> lost -> Search / scan area
```

A footprint never confirms the target by itself.

## Canine AI

BDFR provides:

`ABDFRCanineAIController`

and:

`UBDFRCanineTrackingComponent`

The canine controller inherits the normal BDFR perception stack and adds:

- scent tracking,
- high hearing sensitivity,
- compatibility with normal Sight and Damage senses.

Default canine hearing multiplier:

```text
1.75x
```

This multiplier applies both to hearing range and effective hearing strength.

### Scent acquisition

The canine tracker:

1. looks for recent scent samples around the dog,
2. acquires a source actor,
3. stores the current scent sample,
4. searches for newer scent sequence samples,
5. exposes the next scent location for navigation,
6. raises awareness from scent without requiring line of sight.

Default values:

```text
Scent acquisition radius: 30 m
Maximum scent age:         240 s
Trail step radius:          35 m
Scan interval:             0.20 s
```

Exposed state:

```text
HasScentTarget()
GetScentTarget()
GetNextScentLocation()
GetLastScentSample()
```

Suggested canine Behavior Tree:

```text
Hear suspicious sound
        |
        v
Search nearby scent
        |
        v
Scent found
        |
        v
Follow scent samples
        |
        +--> newer scent -> keep tracking
        |
        +--> target seen -> chase / alert handler
        |
        +--> scent lost -> search spiral / return to handler
```

## ProjectIGI integration

`AIGIPlayerCharacter` now owns:

`UBDFRTrackEmitterComponent`

so the player automatically produces footprint and scent samples.

ProjectIGI also includes:

`AIGIDogAIController : ABDFRCanineAIController`

It filters hostile perception to `AIGIPlayerCharacter`.

A future dog Pawn/Character can use `AIGIDogAIController` as its controller without coupling BDFR to a specific dog mesh, skeleton, or locomotion system.

## Visual footprints

BDFR tracking works even without visible decals.

`UBDFRTrackEmitterComponent::OnTrackSampleEmitted` is exposed so a host project can optionally spawn:

- mud footprint decals,
- snow depressions,
- wet footprints,
- blood footprints,
- dust disturbances.

This visual layer is separate from AI tracking state.

## Future extensions

- Unreal Physical Material -> BDFR surface automatic mapping,
- rain washing out scent,
- wind direction affecting scent spread,
- blood-trail tracking,
- crawling / limping trail signatures,
- dog handler commands,
- dog barking / alert signaling,
- scent contamination and crossed trails,
- water breaking or reducing trails,
- squad sharing of discovered tracks.
