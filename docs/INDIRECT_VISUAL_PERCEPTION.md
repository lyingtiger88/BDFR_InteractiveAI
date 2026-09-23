# Indirect Visual Perception: Shadows and Reflections

BDFR supports indirect visual clues for high-difficulty AI without requiring AI Perception to read rendered pixels.

## Difficulty gating

Indirect visual perception is disabled by default for:

- Recruit
- Private
- Sergeant

It is enabled for:

- Commando
- SAS

SAS uses a higher indirect-visual awareness multiplier than Commando.

A shadow or reflection does **not** directly confirm the player as a threat. It raises awareness and provides a clue location. Direct visual confirmation is still required for a confirmed threat unless another gameplay system confirms the target.

## Architecture

```text
Player / detectable actor
    |
    +-- BDFRIndirectVisualSourceComponent
            |
            +-- Shadow Cue
            |
            +-- Reflection eligibility

Reflective level surface
    |
    +-- BDFRReflectiveSurfaceComponent
            |
            +-- Mirror / CalmWater / PolishedMetal / Custom
            +-- finite surface bounds
            +-- reflection plane normal
            +-- reflection strength

Enemy AI Controller
    |
    +-- BDFRIndirectVisualPerceptionComponent
            |
            +-- difficulty gate
            +-- direct-LOS rejection
            +-- shadow visibility test
            +-- specular reflection-path test
            +-- awareness gain
```

## Shadow detection

`UBDFRIndirectVisualSourceComponent` can automatically estimate a shadow cue from the dominant Directional Light.

The component:

1. finds the strongest visible Directional Light,
2. reads its light direction,
3. projects a visibility trace from the actor along the light travel direction,
4. stores the approximate surface point as the shadow clue,
5. periodically updates the clue while the actor moves.

This is a gameplay approximation, not a Shadow Map readback.

For custom lighting setups, automatic estimation can be disabled and the project can call:

```text
SetShadowCue(Location, Strength)
```

or:

```text
EstimateShadowCueFromLightDirection(Direction)
```

directly.

## Reflection detection

`UBDFRReflectiveSurfaceComponent` is a bounded reflective gameplay surface.

For each candidate viewer/source pair it:

1. reflects the source position mathematically across the surface plane,
2. traces from the viewer toward the virtual reflected source,
3. calculates the point where that ray intersects the reflective plane,
4. rejects the clue if the reflection point is outside the component bounds,
5. rejects blocked viewer-to-surface paths,
6. optionally rejects blocked source-to-surface paths,
7. applies distance and surface-facing attenuation.

The reflective surface component should be aligned with the visible reflective surface.

The component's local surface normal defaults to local +Z. Rotate the component so local +Z points out of the reflective plane, or override `LocalSurfaceNormal`.

## Recommended material setups

The values below are starting points for visual consistency. The BDFR gameplay detector does not inspect material shader values directly; the designer explicitly marks gameplay-relevant reflective surfaces with `BDFRReflectiveSurfaceComponent`.

### Mirror

Recommended visual setup:

```text
Shading Model: Default Lit
Blend Mode: Opaque
Metallic: 1.0
Roughness: 0.00 - 0.03
Base Color: 0.90 - 1.00 neutral
Normal strength: very low
```

Recommended reflection method:

- Planar Reflection for important flat mirrors requiring reliable, accurate dynamic reflections.
- Lumen Reflections can be used when the project already relies on Lumen and the quality/performance tradeoff is acceptable.

Recommended BDFR gameplay setup:

```text
Surface Type: Mirror
Reflection Strength: 0.90 - 1.00
Max Detection Path: 5000 - 8000 cm
```

### Calm water

Recommended visual setup:

```text
Shading Model: SingleLayerWater
Blend Mode: Opaque or Masked
Roughness: ~0.03 - 0.12 for calm water
Normal distortion: low to moderate
```

Use stronger reflection clues only for calm or slowly moving water. Choppy water should use lower BDFR reflection strength.

Recommended BDFR gameplay setup:

```text
Surface Type: CalmWater
Reflection Strength: 0.55 - 0.85
Max Detection Path: 4000 - 7000 cm
```

### Polished metal

Recommended visual setup:

```text
Shading Model: Default Lit
Blend Mode: Opaque
Metallic: 1.0
Roughness: 0.08 - 0.20
Normal detail: subtle
```

Recommended BDFR gameplay setup:

```text
Surface Type: PolishedMetal
Reflection Strength: 0.30 - 0.65
Max Detection Path: 2500 - 5000 cm
```

Brushed, dirty, oxidized, scratched, or highly rough metal should normally have much weaker reflection clues or no gameplay reflective component at all.

## Level-design examples

### Player hidden behind a doorway

```text
Player body: no direct LOS
Player shadow: extends into corridor
Commando/SAS sees shadow cue
Awareness rises
AI turns toward clue / investigates
```

### Player beside a mirror

```text
Player body: behind corner
Mirror: visible to enemy
Specular reflection path valid
Reflection cue detected
Awareness rises
AI checks reflected direction
```

### Player near calm water

```text
Player body: partially hidden by cover
Water surface: visible
Valid source -> water -> viewer reflection path
Reflection cue detected
```

## Behavior Tree integration

The later Blackboard sync should expose:

```text
LastIndirectCueType
LastIndirectCueLocation
LastIndirectCueStrength
HasIndirectVisualCue
```

Suggested behavior:

```text
Indirect cue detected
    |
    v
Suspicious / Investigate
    |
    +-- turn toward cue
    +-- move carefully
    +-- scan nearby cover/corners
    +-- do not immediately fire
```

For SAS, an indirect cue may accelerate search behavior but should not start Persistent Hunt until the target becomes a confirmed threat.

## Performance

The system avoids GPU readback and does not sample reflection or shadow render targets.

It uses:

- registered source components,
- registered reflective-surface components,
- periodic CPU geometry checks,
- visibility traces,
- per-source cue cooldowns.

Keep reflective gameplay components limited to surfaces that matter tactically rather than marking every shiny prop in the level.
