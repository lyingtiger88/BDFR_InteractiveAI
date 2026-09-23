# Canine Attention and Animation Presentation

BDFR separates canine decision logic from the dog skeleton and animation assets.

## Attention states

```text
Relaxed
Scanning
Listening
Tracking
Alerted
Engaging
```

`UBDFRCanineAttentionComponent` is owned by `ABDFRCanineAIController`.

It continuously evaluates:

- recent acoustic events,
- scent tracking,
- awareness state,
- confirmed threats,
- idle environmental scanning.

## Directional ear response

When the dog hears a recent sound:

```text
Sound Location
    ↓
Local direction relative to dog
    ↓
EarYaw / EarPitch
    ↓
HeadYaw / HeadPitch
```

The ear interpolation is intentionally faster than the head interpolation.

Default tuning:

```text
Ear interp speed:  14
Head interp speed:  6
Max ear yaw:       85°
Max ear pitch:     40°
Max head yaw:      60°
Max head pitch:    30°
```

This allows the ears to orient toward a sound before the full head follows.

## Environmental attention

When there is no active sound, scent target, or confirmed threat, the dog enters `Scanning`.

A slow procedural scan target moves around the forward direction so the dog does not remain visually frozen.

The host Animation Blueprint can combine this with:

- subtle ear twitch,
- breathing,
- small head turns,
- sniffing,
- idle weight shifting.

## Tracking

When scent is active:

```text
State = Tracking
FocusLocation = NextScentLocation
```

The same ear/head output can be used to keep the dog focused on the next scent direction.

A dog animation set can layer:

- nose-down tracking pose,
- sniff loop,
- short pauses,
- head sweep,
- gait changes.

## Alerted

When awareness rises but the target is not confirmed:

```text
State = Alerted
FocusLocation = LastKnownLocation
```

Suggested animation:

- ears forward,
- head elevated,
- body tension,
- low growl,
- brief pause before moving.

## Engage

When BDFR awareness marks a target as confirmed:

```text
Confirmed Threat
    ↓
State = Engaging
    ↓
BDFR_PlayCanineEngage(Target)
```

The Engage presentation event is fired once when entering the state.

The host project can map it to:

- lunge montage,
- bite attack montage,
- chase transition,
- growl/bark,
- handler release command.

BDFR does not contain a hard-coded dog skeleton or attack montage.

## Presentation interface

A canine pawn may implement:

`IBDFRCaninePresentationInterface`

Methods:

```text
BDFR_UpdateCanineAttention(Snapshot)
BDFR_PlayCanineEngage(TargetActor)
BDFR_PlayCanineBark(ReasonTag)
```

## Attention snapshot

The snapshot contains:

```text
AttentionState
FocusLocation
HasFocus
EarYawDegrees
EarPitchDegrees
HeadYawDegrees
HeadPitchDegrees
Alertness
RequestEngageAnimation
```

## Recommended Anim Blueprint setup

Create Anim Blueprint variables matching the snapshot:

```text
CanineAttentionState
EarYaw
EarPitch
HeadYaw
HeadPitch
Alertness
```

Recommended AnimGraph:

```text
Base Locomotion
      |
      +--> Layered Blend / Additive Attention
              |
              +--> Transform Modify Bone: Ear_L
              +--> Transform Modify Bone: Ear_R
              +--> Transform Modify Bone: Head / Neck
      |
      +--> Montage Slot: FullBody / Engage
```

If the dog skeleton has separate left/right ear chains, the project may apply the same BDFR ear direction with small per-ear offsets for a more natural result.

## Bark response

Strong acoustic events can request a bark.

Default threshold:

```text
Effective sound strength >= 0.80
```

A cooldown prevents bark spam.

Example reason tag:

```text
BDFR.Canine.Bark.StrongSound
```

## ProjectIGI integration

ProjectIGI contains:

```text
AIGIDogCharacter
AIGIDogAIController
```

`AIGIDogCharacter` implements the canine presentation interface and stores the current BDFR attention snapshot.

Blueprint events:

```text
IGI_OnCanineAttentionUpdated
IGI_OnCanineEngageRequested
IGI_OnCanineBarkRequested
```

This allows a future dog Blueprint and Anim Blueprint to use any compatible mesh, skeleton and animation pack without modifying BDFR core code.
