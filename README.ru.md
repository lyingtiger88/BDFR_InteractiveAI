# BDFR Interactive AI

<p align="center">
  <a href="README.md">English</a> •
  <a href="README.fa.md">فارسی</a> •
  <a href="README.ar.md">العربية</a> •
  <a href="README.ru.md">Русский</a>
</p>

Переиспользуемый фреймворк **тактического ИИ для Unreal Engine 5**.

## Цель

BDFR Interactive AI разрабатывается как независимый от конкретного проекта Unreal Engine-плагин для поведения противников и NPC, включая:

- патрулирование и наблюдение,
- зрительное и слуховое восприятие угроз,
- нарастание подозрения и уровни тревоги,
- расследование и поиск,
- выбор и использование укрытий,
- здоровье, ранения и кровотечение,
- крики о помощи и психологическое влияние на ближайших персонажей,
- оценку состояния союзников и оказание помощи,
- звуки выстрелов и взрывов и временное ухудшение слуха,
- опциональные системы плена, заложников и допроса,
- военные игровые профили сложности и скрытый режим `SAS` с Persistent Hunt,
- непрямое визуальное обнаружение по теням и отражениям для Commando и SAS,
- контекстные взаимодействия,
- интеграцию с `Behavior Tree` и `Blackboard`,
- тактические запросы через `EQS`,
- опциональные адаптеры для Locomotion и Combat систем проекта.

## Принципы архитектуры

- **Нет прямой зависимости от ALS.** ALS, Motion Matching, Lyra и пользовательские системы перемещения подключаются через Interfaces и Adapters.
- **Независимое ядро.** Специфичная для игры логика оружия, миссий, анимации и персонажей остаётся за пределами плагина.
- **Компонентная архитектура.** Perception, Awareness, Cover, Health и Interaction разделены на независимые подсистемы.
- **Data-driven настройка.** Settings и Gameplay Tags предпочтительнее жёстко заданных зависимостей.
- **C++ с поддержкой Blueprint.** Основная логика реализуется в C++, при этом ключевые точки расширения доступны в Blueprint.

## Текущее состояние

- [x] Каркас Unreal-плагина и Runtime Module
- [x] Базовая модель состояний AI
- [x] Модель Awareness
- [x] Developer Settings
- [x] Универсальный Locomotion Interface
- [x] Универсальный Combat Interface
- [x] Универсальный Interaction Interface
- [x] Base AI Controller
- [x] AI Perception: Sight
- [x] AI Perception: Hearing
- [x] AI Perception: Damage
- [x] Накопление и спад Awareness
- [x] Последняя известная позиция цели
- [x] Память Confirmed Threat
- [x] Состояния здоровья и кровотечение
- [x] Автоматические крики боли и просьбы о помощи
- [x] Психологический Stress для ближайших AI
- [x] Цель и приоритет помощи союзнику
- [x] Assistance Interface
- [x] Акустические события выстрелов и взрывов
- [x] Состояния Ringing / Impaired / Deafened
- [x] Временное отключение и восстановление AI Hearing
- [x] Blast Exposure с задержкой по расстоянию
- [x] Опциональные состояния Surrender / Restrained / Hostage
- [x] Опциональная модель Interrogation
- [x] Модель False Statement и Information Reliability
- [x] Captivity Presentation Interface
- [x] Gameplay Tags
- [x] Профили Recruit / Private / Sergeant / Commando
- [x] Постоянная разблокировка скрытого SAS после прохождения на Commando
- [x] Persistent Hunt в SAS после подтверждённого обнаружения игрока
- [x] Обнаружение теней в Commando / SAS
- [x] Обнаружение отражений в зеркалах, спокойной воде и полированном металле
- [ ] Синхронизация Blackboard
- [ ] Behavior Tree Tasks / Services
- [ ] Patrol Route
- [ ] Investigation / Search
- [ ] Cover / EQS
- [ ] Smart Object Integration
- [ ] Squad Knowledge

## Архитектура

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

Дополнительная документация:

- `docs/ARCHITECTURE.md`
- `docs/HEALTH_DISTRESS_SOCIAL.md`
- `docs/ACOUSTIC_HEARING.md`
- `docs/CAPTIVITY_INTERROGATION.md`
- `docs/DIFFICULTY_SYSTEM.md`
- `docs/INDIRECT_VISUAL_PERCEPTION.md`

## Первая интеграция

Первым целевым проектом является `ProjectIGI_Remake`, работающий на **Unreal Engine 5.8** с **ALS Refactored**.

Сам плагин не зависит напрямую от ALS. ProjectIGI предоставляет собственный Adapter, переводящий запросы BDFR в Gait, Stance, Aiming и Look Behavior ALS.

Подробнее:

`docs/PROJECT_IGI_INTEGRATION.md`

## Статус

Версия: **0.1.0 / Foundation**

Следующий этап разработки — синхронизация Blackboard и реализация Patrol / Observation.

## Лицензия

Финальная лицензия проекта пока не выбрана.
