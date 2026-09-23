# Architecture

## Boundary

BDFR Interactive AI owns **decision-making and reusable AI behavior**. It must not own a project's locomotion implementation, weapon implementation, mission rules, or character-specific animation graph.

```text
Game Project
├── Character / Pawn
├── Locomotion implementation
├── Weapon / combat implementation
├── Mission-specific behavior
└── BDFR adapters
        │
        ▼
BDFR Interactive AI
├── Perception
├── Awareness
├── Patrol / Observation
├── Investigation / Search
├── Cover
├── Interaction
├── Behavior Tree / Blackboard
└── EQS
```

## ALS integration rule

BDFR must never include ALS headers or link against the ALS module.

A project that uses ALS implements `IBDFRLocomotionInterface` and translates generic BDFR requests into ALS operations.

Example:

```text
BDFR Request
    Set Desired Gait
            │
            ▼
IBDFRLocomotionInterface
            │
            ▼
Project-specific ALS Adapter
            │
            ▼
AAlsCharacter::SetDesiredGait(...)
```

This prevents the AI framework from becoming coupled to a specific animation/locomotion solution.

## Initial AI state model

```text
Idle
 └─ Patrol
     └─ Observe
         └─ Suspicious
             └─ Investigate
                 └─ Search
                     └─ Alerted
                         ├─ Combat
                         └─ TakeCover
```

Transitions are not restricted to the diagram above. The state enum is a shared vocabulary; Behavior Trees, components, and project code may drive the actual transitions.

## Awareness

Awareness is continuous rather than binary:

```text
0.00                                      1.00
 |------------------------------------------|
 Unaware     Suspicious    Investigate   Threat
```

Perception stimuli add confidence. Loss of information can decay confidence over time. A confirmed target can therefore remain remembered after line of sight is lost.

## Next implementation blocks

1. Awareness component
2. AI perception bridge
3. Patrol route + patrol point
4. Behavior Tree services/tasks
5. Investigation/search memory
6. Cover query and reservation
7. Interaction/Smart Object bridge
8. Squad knowledge
