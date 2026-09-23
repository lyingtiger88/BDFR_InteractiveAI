# BDFR Interactive AI

<p align="center">
  <a href="README.md">English</a> •
  <a href="README.fa.md">فارسی</a> •
  <a href="README.ar.md">العربية</a> •
  <a href="README.ru.md">Русский</a>
</p>

چارچوبی قابل‌استفاده‌مجدد برای **هوش مصنوعی تاکتیکی در Unreal Engine 5**.

## هدف

BDFR Interactive AI به‌عنوان یک پلاگین مستقل از پروژه برای Unreal Engine طراحی شده تا رفتار دشمن‌ها و NPCها را مدیریت کند، از جمله:

- گشت‌زنی و دیدبانی،
- ادراک دیداری، شنیداری و تشخیص تهدید،
- افزایش تدریجی شک و وضعیت هشدار،
- بررسی موقعیت و جست‌وجو،
- انتخاب و استفاده از کاور،
- سلامت، جراحت و خونریزی،
- فریاد کمک و تأثیر روانی روی نیروهای اطراف،
- ارزیابی وضعیت هم‌تیمی و کمک‌رسانی،
- تشخیص صدای شلیک و انفجار و اختلال موقت شنوایی،
- سیستم‌های اختیاری اسارت، گروگان‌گیری و بازجویی،
- درجه‌های سختی نظامی‌محور و حالت مخفی `SAS` با Persistent Hunt،
- تعاملات زمینه‌ای،
- یکپارچه‌سازی با `Behavior Tree` و `Blackboard`،
- تصمیم‌گیری تاکتیکی مبتنی بر `EQS`،
- Adapterهای اختیاری برای اتصال به سیستم‌های Locomotion و Combat پروژه میزبان.

## اصول طراحی

- **بدون وابستگی مستقیم به ALS.** سیستم‌هایی مثل ALS، Motion Matching، Lyra یا Locomotion سفارشی از طریق Interface و Adapter متصل می‌شوند.
- **هسته مستقل از پروژه.** منطق اختصاصی اسلحه، مأموریت، انیمیشن و کاراکتر بیرون از پلاگین باقی می‌ماند.
- **معماری Component محور.** Perception، Awareness، Cover، Health و Interaction به‌صورت زیرسیستم‌های مستقل طراحی شده‌اند.
- **تنظیمات Data-driven.** به‌جای وابستگی‌های Hard-coded از Settings و Gameplay Tags استفاده می‌شود.
- **C++ سازگار با Blueprint.** هسته سیستم در C++ پیاده‌سازی شده و نقاط توسعه تمیز برای Blueprint ارائه می‌دهد.

## وضعیت فعلی پیاده‌سازی

- [x] اسکلت پلاگین و Runtime Module
- [x] مدل State اصلی AI
- [x] مدل Awareness
- [x] Developer Settings
- [x] Interface عمومی Locomotion
- [x] Interface عمومی Combat
- [x] Interface عمومی Interaction
- [x] Base AI Controller
- [x] AI Perception: Sight
- [x] AI Perception: Hearing
- [x] AI Perception: Damage
- [x] افزایش و کاهش تدریجی Awareness
- [x] ذخیره آخرین موقعیت شناخته‌شده هدف
- [x] حافظه Confirmed Threat
- [x] وضعیت‌های سلامت و خونریزی
- [x] فریاد درد و درخواست کمک خودکار
- [x] Stress روانی برای AIهای نزدیک
- [x] هدف و اولویت کمک‌رسانی به هم‌تیمی
- [x] Assistance Interface
- [x] رویدادهای صوتی شلیک و انفجار
- [x] حالت‌های Ringing / Impaired / Deafened
- [x] غیرفعال و فعال شدن موقت AI Hearing
- [x] Blast Exposure با تأخیر متناسب با فاصله
- [x] وضعیت‌های اختیاری Surrender / Restrained / Hostage
- [x] مدل اختیاری Interrogation
- [x] مدل False Statement و Information Reliability
- [x] Captivity Presentation Interface
- [x] Gameplay Tags
- [x] درجه‌های سختی Recruit / Private / Sergeant / Commando
- [x] Unlock دائمی درجه مخفی SAS پس از اتمام بازی روی Commando
- [x] Persistent Hunt در SAS پس از شناسایی قطعی بازیکن
- [ ] همگام‌سازی Blackboard
- [ ] Behavior Tree Tasks / Services
- [ ] Patrol Route
- [ ] Investigation / Search
- [ ] Cover / EQS
- [ ] Smart Object Integration
- [ ] Squad Knowledge

## معماری

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

مستندات تکمیلی:

- `docs/ARCHITECTURE.md`
- `docs/HEALTH_DISTRESS_SOCIAL.md`
- `docs/ACOUSTIC_HEARING.md`
- `docs/CAPTIVITY_INTERROGATION.md`
- `docs/DIFFICULTY_SYSTEM.md`

## هدف اولیه برای یکپارچه‌سازی

اولین پروژه‌ای که BDFR با آن یکپارچه می‌شود `ProjectIGI_Remake` است که روی **Unreal Engine 5.8** و **ALS Refactored** اجرا می‌شود.

خود پلاگین هیچ وابستگی مستقیمی به ALS ندارد. در ProjectIGI یک Adapter اختصاصی درخواست‌های BDFR را به Gait، Stance، Aiming و Look Behavior در ALS تبدیل می‌کند.

برای جزئیات بیشتر:

`docs/PROJECT_IGI_INTEGRATION.md`

## وضعیت

نسخه: **0.1.0 / Foundation**

مرحله بعدی توسعه شامل همگام‌سازی Blackboard و پیاده‌سازی Patrol / Observation است.

## مجوز

هنوز License نهایی پروژه انتخاب نشده است.
