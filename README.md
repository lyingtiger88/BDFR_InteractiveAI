# BDFR Interactive AI

Reusable tactical AI framework for Unreal Engine 5.

## Goal

BDFR Interactive AI is designed as a project-independent Unreal Engine plugin for enemy and NPC behaviors such as:

- patrol and observation,
- sight / hearing / threat perception,
- suspicion and alert escalation,
- investigation and search,
- cover selection and cover behavior,
- contextual interaction,
- Behavior Tree / Blackboard integration,
- EQS-based tactical queries,
- optional project adapters for locomotion and combat systems.

## Design principles

- **No direct ALS dependency.** ALS, Motion Matching, Lyra, or custom locomotion should connect through interfaces/adapters.
- **Project-independent core.** Game-specific weapon, mission, animation, and character code stays outside the plugin.
- **Component-driven behavior.** Perception, awareness, cover, and interaction are isolated systems.
- **Data-driven tuning.** Gameplay tags and settings are preferred over hard-coded project assumptions.
- **Blueprint-friendly C++.** Core systems live in C++ and expose clean Blueprint extension points.

## Planned architecture

```text
BDFR_InteractiveAI
├── Core
├── Perception
├── Awareness
├── Patrol
├── Search
├── Cover
├── Interaction
├── Behavior
└── Interfaces
```

## Initial integration target

The first integration target is `ProjectIGI_Remake`, running Unreal Engine 5.8 with ALS Refactored.

The plugin itself must remain independent from ALS. ProjectIGI will provide a thin locomotion adapter that translates BDFR requests into ALS gait, stance, aiming, and rotation/look behavior.

## Development roadmap

### Phase 1 — Foundation
- [x] Plugin/module skeleton
- [x] Core AI state types
- [x] Locomotion interface
- [x] Combat interface
- [x] Interaction interface
- [ ] Developer settings
- [ ] Gameplay tags

### Phase 2 — Perception & awareness
- [ ] AI Perception component
- [ ] Sight
- [ ] Hearing
- [ ] Damage awareness
- [ ] Threat memory
- [ ] Suspicion / alert model

### Phase 3 — Patrol & observation
- [ ] Patrol route
- [ ] Patrol points
- [ ] Observation points
- [ ] Wait / scan behavior

### Phase 4 — Investigation & search
- [ ] Last known position
- [ ] Investigation locations
- [ ] Search patterns
- [ ] Lost target memory

### Phase 5 — Cover
- [ ] Cover candidates
- [ ] EQS cover query
- [ ] Occupancy/reservation
- [ ] Enter / leave / re-evaluate cover
- [ ] Peek hooks

### Phase 6 — Interaction
- [ ] Generic interactable interface
- [ ] Contextual interaction tasks
- [ ] Smart Object integration

### Phase 7 — Tactical coordination
- [ ] Shared alerts
- [ ] Squad knowledge
- [ ] Roles
- [ ] Flanking / suppression hooks

## License

License has not yet been selected.
