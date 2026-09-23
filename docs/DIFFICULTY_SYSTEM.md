# Difficulty System

BDFR uses military-themed **gameplay difficulty profiles**. The names are labels for game AI skill levels, not a claim that they form one real British Army rank ladder.

## Standard difficulty menu

The four standard tiers are:

1. **Recruit** — Easy
2. **Private** — Normal
3. **Sergeant** — Hard
4. **Commando** — Elite

`Commando` is an elite gameplay label, not a British Army rank.

## Hidden fifth tier: SAS

A fifth challenge profile, **SAS**, exists but is hidden from the normal difficulty menu.

It becomes available only after the player completes the campaign at least once on **Commando**.

The unlock flow is:

```text
Complete campaign on Commando
        ↓
NotifyCampaignCompleted(Commando)
        ↓
SAS unlock saved permanently
        ↓
SAS appears in difficulty menu
```

The unlock is persisted in a dedicated BDFR SaveGame slot:

```text
BDFR_DifficultyUnlocks
```

The game UI should populate its menu through:

```text
GetUnlockedMenuDifficultyTiers()
```

Before unlock, the function returns only the four standard tiers. After unlock, it returns all five.

## SAS Persistent Hunt

SAS has a unique behavior rule:

```text
Player presence confirmed
        ↓
Persistent Hunt activated
        ↓
Lose line of sight
        ↓
Search continues
        ↓
NO return to Patrol
        ↓
Target neutralized OR explicit mission/system cancel
        ↓
Persistent Hunt ends
```

The AIController exposes:

```text
IsPersistentHuntActive()
GetPersistentHuntTarget()
NotifyPersistentHuntTargetNeutralized()
CancelPersistentHunt()
```

The later Behavior Tree must use `IsPersistentHuntActive` as a hard decorator/priority rule so a SAS enemy cannot time out back into Patrol after confirming the player.

## Difficulty profile fields

Each tier can tune:

- Sight Radius Multiplier
- Hearing Sensitivity Multiplier
- Awareness Gain Multiplier
- Reaction Delay
- Search Persistence
- Stress Resistance
- Assistance Bravery
- Surrender Resistance
- Acoustic Recovery Multiplier
- Combat Accuracy Multiplier
- Cover Decision Multiplier
- Persistent Hunt After Confirmed Target

## Initial tuning

| Tier | Sight | Hearing | Awareness | Reaction | Search | Stress Resistance | Accuracy | Persistent Hunt |
|---|---:|---:|---:|---:|---:|---:|---:|:---:|
| Recruit | 0.75x | 0.75x | 0.65x | 1.10 s | 10 s | 0.20 | 0.70x | No |
| Private | 1.00x | 1.00x | 1.00x | 0.55 s | 18 s | 0.45 | 1.00x | No |
| Sergeant | 1.15x | 1.10x | 1.20x | 0.30 s | 28 s | 0.70 | 1.18x | No |
| Commando | 1.30x | 1.20x | 1.40x | 0.16 s | 40 s | 0.88 | 1.32x | No |
| SAS | 1.45x | 1.35x | 1.65x | 0.08 s | 120 s fallback | 0.97 | 1.45x | **Yes** |

These are gameplay tuning defaults, not real-world measurements.

## Global and per-AI difficulty

The project may define one global difficulty in Project Settings.

Individual AI may override the global setting.

Example:

```text
Global: Private

Guard            -> Private
Rookie           -> Recruit
Squad Leader     -> Sergeant
Elite Guard      -> Commando
Unlocked Hunter  -> SAS
```

SAS selection is rejected while the unlock is unavailable, even if code attempts to select it.

## ProjectIGI integration

At campaign completion, ProjectIGI should call:

```text
DifficultyUnlockSubsystem
    -> NotifyCampaignCompleted(CurrentDifficulty)
```

When `CurrentDifficulty == Commando`, SAS becomes permanently available.

The actual main-menu widget remains owned by ProjectIGI; BDFR provides the authoritative list of unlocked difficulty tiers.
