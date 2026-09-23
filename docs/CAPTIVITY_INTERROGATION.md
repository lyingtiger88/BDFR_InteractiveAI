# Captivity, Hostage and Interrogation

This subsystem is optional and project-controlled.

## Optional switches

Project Settings -> BDFR Interactive AI:

- Enable Captivity System
- Enable Interrogation System
- Enable Coercive Interrogation

Coercive interrogation is disabled by default.

## Captivity state model

```text
Free
  -> Surrendering
      -> Restrained
          -> Hostage
          -> Interrogating
          -> Escaping
          -> Released
```

`UBDFRCaptivityComponent` owns the captive state, captor reference, restraint state, and escape eligibility.

Per-NPC configuration can disable capture entirely.

## Interrogation model

`UBDFRInterrogationComponent` uses fictional gameplay variables:

- Resistance
- Fear
- Rapport
- Evidence Pressure
- Psychological Strain
- Information Reliability
- Disclosure Level

Available approaches:

```text
Question
Present Evidence
Bargain
Intimidate
Coercive Pressure
```

The coercive option is deliberately abstract. BDFR does not implement real-world torture methods.

## Information reliability

Disclosure and truth reliability are separate.

High pressure may increase compliance while reducing information reliability. Outcomes include:

```text
No Response
Refusal
Partial Disclosure
Full Disclosure
False Statement
Breakdown
```

This prevents the game from treating coercion as a guaranteed truth mechanic.

All thresholds are fictional game-balance values rather than real-world interrogation guidance.

## Presentation / animation

`IBDFRCaptivityPresentationInterface` separates gameplay logic from animation.

A host project can map state changes to:

- surrender animation,
- hands-up pose,
- kneeling,
- restrained locomotion,
- hostage positioning,
- interrogation montages,
- release / escape animation.

BDFR itself has no ALS dependency.

## ProjectIGI

`AIGIEnemyCharacter` owns:

- `UBDFRCaptivityComponent`
- `UBDFRInterrogationComponent`

The components are passive until gameplay logic invokes them.

Planned ProjectIGI integration:

1. surrender conditions from morale/threat state,
2. player interaction to restrain,
3. hostage positioning / movement,
4. nearby ally response to captured teammate,
5. interrogation UI/dialogue,
6. information payloads tied to mission/gameplay data,
7. optional escape attempts,
8. Behavior Tree overrides while captive.

## Future extensions

- hostage negotiation,
- captor/hostage movement constraints,
- rescue behavior by allies,
- prisoner escort,
- guard-prisoner interaction,
- reputation / mission consequences,
- witness reactions,
- dialogue trees,
- data-driven information packages,
- non-lethal surrender conditions.
