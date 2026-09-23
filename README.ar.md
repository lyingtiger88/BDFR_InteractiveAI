# BDFR Interactive AI

<p align="center">
  <a href="README.md">English</a> •
  <a href="README.fa.md">فارسی</a> •
  <a href="README.ar.md">العربية</a> •
  <a href="README.ru.md">Русский</a>
</p>

إطار عمل قابل لإعادة الاستخدام لبناء **ذكاء اصطناعي تكتيكي في Unreal Engine 5**.

## الهدف

تم تصميم BDFR Interactive AI كإضافة مستقلة عن المشروع لإدارة سلوك الأعداء وNPCs، بما في ذلك:

- الدوريات والمراقبة،
- الإدراك البصري والسمعي واكتشاف التهديدات،
- تصاعد الشك وحالات التأهب،
- التحقيق والبحث،
- اختيار الغطاء واستخدامه،
- الصحة والإصابات والنزيف،
- نداءات الاستغاثة والتأثير النفسي على الشخصيات القريبة،
- تقييم حالة الحلفاء والاستجابة لطلبات المساعدة،
- أصوات إطلاق النار والانفجارات وضعف السمع المؤقت،
- أنظمة اختيارية للأسر والرهائن والاستجواب،
- مستويات صعوبة بطابع عسكري مع وضع `SAS` مخفي وPersistent Hunt،
- التفاعل السياقي،
- التكامل مع `Behavior Tree` و`Blackboard`،
- استعلامات تكتيكية باستخدام `EQS`،
- Adapters اختيارية لأنظمة Locomotion وCombat في المشروع المضيف.

## مبادئ التصميم

- **لا توجد تبعية مباشرة لـ ALS.** يمكن ربط ALS أو Motion Matching أو Lyra أو أي نظام حركة مخصص عبر Interfaces وAdapters.
- **نواة مستقلة عن المشروع.** تبقى أنظمة الأسلحة والمهام والأنيميشن والمنطق الخاص بالشخصيات خارج الإضافة.
- **بنية تعتمد على Components.** يتم فصل Perception وAwareness وCover وHealth وInteraction إلى أنظمة مستقلة.
- **إعدادات Data-driven.** يتم تفضيل Settings وGameplay Tags على الافتراضات Hard-coded.
- **C++ متوافق مع Blueprint.** توجد النواة في C++ مع نقاط توسعة واضحة للـBlueprint.

## الأساس الحالي

- [x] هيكل الإضافة وRuntime Module
- [x] نموذج حالات AI
- [x] نموذج Awareness
- [x] Developer Settings
- [x] واجهة Locomotion عامة
- [x] واجهة Combat عامة
- [x] واجهة Interaction عامة
- [x] Base AI Controller
- [x] AI Perception: Sight
- [x] AI Perception: Hearing
- [x] AI Perception: Damage
- [x] تراكم وانخفاض Awareness
- [x] آخر موقع معروف للهدف
- [x] ذاكرة Confirmed Threat
- [x] حالات الصحة والنزيف
- [x] صرخات الألم وطلبات المساعدة التلقائية
- [x] Stress نفسي للـAI القريب
- [x] هدف وأولوية مساعدة الحلفاء
- [x] Assistance Interface
- [x] أحداث صوتية لإطلاق النار والانفجارات
- [x] حالات Ringing / Impaired / Deafened
- [x] تعطيل واستعادة AI Hearing مؤقتًا
- [x] Blast Exposure مع تأخير حسب المسافة
- [x] حالات اختيارية Surrender / Restrained / Hostage
- [x] نموذج Interrogation اختياري
- [x] نموذج False Statement وInformation Reliability
- [x] Captivity Presentation Interface
- [x] Gameplay Tags
- [x] مستويات Recruit / Private / Sergeant / Commando
- [x] فتح دائم لوضع SAS المخفي بعد إنهاء اللعبة على Commando
- [x] Persistent Hunt في SAS بعد تأكيد وجود اللاعب
- [ ] مزامنة Blackboard
- [ ] Behavior Tree Tasks / Services
- [ ] Patrol Route
- [ ] Investigation / Search
- [ ] Cover / EQS
- [ ] Smart Object Integration
- [ ] Squad Knowledge

## البنية

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

الوثائق الإضافية:

- `docs/ARCHITECTURE.md`
- `docs/HEALTH_DISTRESS_SOCIAL.md`
- `docs/ACOUSTIC_HEARING.md`
- `docs/CAPTIVITY_INTERROGATION.md`
- `docs/DIFFICULTY_SYSTEM.md`

## هدف التكامل الأولي

المشروع الأول المستهدف للتكامل هو `ProjectIGI_Remake`، ويعمل على **Unreal Engine 5.8** مع **ALS Refactored**.

الإضافة نفسها لا تعتمد مباشرة على ALS. يوفر ProjectIGI Adapter يحول طلبات BDFR إلى Gait وStance وAiming وLook Behavior داخل ALS.

للمزيد:

`docs/PROJECT_IGI_INTEGRATION.md`

## الحالة

الإصدار: **0.1.0 / Foundation**

المرحلة التالية هي مزامنة Blackboard ثم تنفيذ Patrol / Observation.

## الترخيص

لم يتم اختيار الترخيص النهائي للمشروع بعد.
