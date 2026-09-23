# Acoustic Events and Temporary Hearing Impairment

BDFR separates **hearing a sound** from **being physically exposed to a loud acoustic/blast event**.

## Event sources

Use `UBDFRAcousticEventLibrary` from C++ or Blueprint.

### Gunshot

`ReportGunshot(...)`

A gunshot:

- emits an AI Hearing event,
- stores a semantic tag,
- applies short-range acoustic exposure,
- can accumulate with repeated nearby shots.

Tags:

```text
BDFR.Acoustic.Gunshot
BDFR.Acoustic.Gunshot.Suppressed
```

### Explosion

`ReportExplosion(...)`

An explosion:

- emits a long-range AI Hearing event,
- applies a radial blast/acoustic exposure,
- uses distance-squared falloff,
- can delay physical exposure by distance / propagation speed.

Tag:

```text
BDFR.Acoustic.Explosion
```

The default propagation speed is 34300 cm/s (343 m/s).

## Hearing exposure states

`UBDFRAcousticExposureComponent` tracks cumulative exposure:

```text
Normal
  -> Ringing
      -> Impaired
          -> Temporarily Deafened
```

Exposure decays over time.

Default thresholds:

```text
Ringing   >= 0.25
Impaired  >= 0.50
Deafened  >= 0.85
```

Repeated gunfire can therefore build exposure even if one shot is not enough to deafen the actor.

## Perception effects

While hearing is partially impaired, perceived hearing strength is scaled by the current hearing sensitivity.

This affects:

- gunshot awareness,
- explosion awareness,
- distress/help calls,
- psychological stress from heard events.

When state becomes `Deafened`, BDFR disables `UAISense_Hearing` on the AI Perception component.

As exposure decays below the deafened threshold, Hearing is enabled again automatically.

## Stress effects

Heard acoustic events also affect the BDFR stress system.

Initial behavior:

- gunshots add moderate stress,
- explosions add stronger stress,
- hearing impairment can reduce later heard-event influence.

## Gameplay hooks

`OnHearingStateChanged` and `OnAcousticExposure` can be used by the project for:

- tinnitus/ringing audio,
- muffled voice lines,
- head-shake or disorientation animation,
- temporary aim/reaction penalties,
- combat barks such as "I can't hear!",
- HUD/audio post-processing for player-controlled characters.

## ProjectIGI integration

`AIGIEnemyCharacter` owns a `UBDFRAcousticExposureComponent`.

Weapon code should call:

```text
ReportGunshot
```

for each actual shot.

Explosion/grenade code should call:

```text
ReportExplosion
```

at detonation.

## Native AI Hearing limitation

Unreal's built-in AI Hearing processes Report Noise Event stimuli without per-listener speed-of-sound travel simulation.

BDFR currently simulates travel delay for the **physical blast/acoustic exposure**. A future custom BDFR AI Sense can provide true per-listener delayed sound arrival if required.
