# Health, Distress and Social Response

This subsystem makes injury a gameplay/AI event rather than only an HP reduction.

## Physical health

`UBDFRHealthComponent` tracks:

- health / max health,
- Healthy / Wounded / Critical / Incapacitated / Dead,
- bleeding state,
- bleed rate per second,
- stabilization,
- health state events.

Bleeding is orthogonal to health state. An actor may be Wounded + Bleeding or Critical + Bleeding.

## Injury reaction

`UBDFRInjuryResponseComponent` observes the health component and can automatically:

- emit a pain shout after damage,
- request help when critical,
- request help when bleeding starts,
- emit an emergency BleedingOut signal when incapacitated.

Pain/help signals use cooldowns so repeated damage does not create audio/event spam.

## Distress propagation

`UBDFRDistressComponent` reports distress through Unreal AI Hearing using semantic tags:

```text
BDFR.Distress.Pain
BDFR.Distress.Help
BDFR.Distress.AllyDown
BDFR.Distress.BleedingOut
BDFR.Distress.Panic
```

Each distress signal has urgency and an audible radius.

This lets nearby AI perceive the event without direct references between every NPC.

## Psychological stress

Every `ABDFRAIController` owns a `UBDFRStressComponent`.

Nearby distress can raise stress:

```text
Calm
  -> Concerned
      -> Stressed
          -> Panicked
```

Stress decays over time. Later combat logic can use stress to alter:

- accuracy,
- reaction time,
- willingness to leave cover,
- suppression response,
- voice lines,
- retreat/panic decisions.

## Assistance

When an eligible distress signal is heard, the controller stores:

- PendingAssistanceTarget,
- PendingAssistanceLocation,
- PendingAssistanceUrgency.

The project can decide who counts as an ally by overriding:

`BDFR_ShouldRespondToDistress()`.

The casualty exposes assistance through `IBDFRAssistanceInterface`:

- CanReceiveAssistance,
- GetAssistanceLocation,
- BeginAssistance,
- CompleteAssistance.

The Behavior Tree layer should resolve the actual movement and priority:

```text
Distress heard
    |
    v
Is this an ally?
    |
    v
Store assistance request
    |
    +--> Combat danger too high -> keep cover / fight
    |
    +--> Safe enough -> RespondToDistress
                         |
                         v
                    Move near ally
                         |
                         v
                     AssessAlly
                         |
             +-----------+-----------+
             |                       |
          Bleeding                 Minor
             |                       |
          Assist                  Reassure
             |
          Stabilize
```

## ProjectIGI initial behavior

`AIGIEnemyCharacter` owns:

- BDFR Health,
- BDFR Distress,
- BDFR Injury Response.

It implements `IBDFRAssistanceInterface`.

`AIGIEnemyAIController` treats other `AIGIEnemyCharacter` actors as valid distress sources while player perception remains hostile-target perception.

## Planned extensions

- hit-location injury severity,
- weapon/damage-type driven bleeding,
- limping / reduced gait,
- arm injury and aim penalties,
- medic role and medical inventory,
- drag / carry wounded allies,
- casualty inspection duration,
- squad morale propagation,
- death / ally-down witness reactions,
- bravery/personality tuning,
- suppression integration.
