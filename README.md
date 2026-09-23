# BDFR Interactive AI

Reusable tactical AI framework for Unreal Engine 5.

## Goal

BDFR Interactive AI is designed as a project-independent Unreal Engine plugin for enemy and NPC behaviors such as:

- patrol and observation,
- sight / hearing / threat perception,
- suspicion and alert escalation,
- investigation and search,
- cover selection and cover behavior,
- health / injury / bleeding,
- distress calls and nearby psychological stress,
- ally assessment and assistance requests,
- gunshot / explosion acoustic events and temporary hearing impairment,
- contextual interaction,
- Behavior Tree / Blackboard integration,
- EQS-based tactical queries,
- optional project adapters for locomotion and combat systems.

## Design principles

- **No direct ALS dependency.** ALS, Motion Matching, Lyra, or custom locomotion connect through interfaces/adapters.
- **Project-independent core.** Game-specific weapon, mission, animation, and character code stays outside the plugin.
- **Component-driven behavior.** Perception, awareness, cover, and interaction are isolated systems.
- **Data-driven tuning.** Settings and tags are preferred over hard-coded project assumptions.
- **Blueprint-friendly C++.** Core systems live in C++ and expose Blueprint extension points.

## Current foundation

- [x] Unreal plugin/module skeleton
- [x] Core AI state model
- [x] Awareness level model
- [x] Developer settings
- [x] Generic locomotion interface
- [x] Generic combat interface
- [x] Generic interaction interface
- [x] Base AI Controller
- [x] AI Perception: sight
- [x] AI Perception: hearing
- [x] AI Perception: damage
- [x] Awareness accumulation / decay
- [x] Last known target location
- [x] Confirmed threat memory
- [x] Health states and bleeding
- [x] Automatic pain/help distress signals
- [x] Nearby AI stress response
- [x] Pending ally assistance target/urgency
- [x] Assistance interface
- [x] Gunshot and explosion acoustic events
- [x] Cumulative ringing / impaired / deafened hearing states
- [x] Temporary AI Hearing disable/recovery
- [x] Distance-delayed blast exposure
- [ ] Gameplay tags
- [ ] Blackboard sync
- [ ] Behavior Tree tasks/services
- [ ] Patrol route
- [ ] Investigation/search
- [ ] Cover/EQS
- [ ] Smart Object integration
- [ ] Squad knowledge

## Architecture

```text
BDFR_InteractiveAI
├── Core
├── Perception (via base AIController)
├── Awareness
├── Patrol
├── Search
├── Cover
├── Interaction
├── Behavior
└── Interfaces
```

See `docs/ARCHITECTURE.md`, `docs/HEALTH_DISTRESS_SOCIAL.md`, and `docs/ACOUSTIC_HEARING.md`.

## Initial integration target

The first integration target is `ProjectIGI_Remake`, running Unreal Engine 5.8 with ALS Refactored.

The plugin itself remains independent from ALS. ProjectIGI supplies the adapter translating BDFR requests into ALS gait, stance, aiming, and look behavior.

See `docs/PROJECT_IGI_INTEGRATION.md`.

## Status

Version: **0.1.0 / foundation**

The next implementation block is Blackboard synchronization plus Patrol/Observation.

## License

License has not yet been selected.
