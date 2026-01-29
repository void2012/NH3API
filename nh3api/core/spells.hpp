//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "creatures.hpp" // TCreatureType, others

// Spell school mask /
// Маска школы магии.
NH3API_FLAG_ENUM TSpellSchool : uint32_t
{
    const_invalid_school = 0,
    eSchoolAir           = 1,             // Школа магии воздуха
    eSchoolFire          = 2,             // Школа магии огня
    eSchoolWater         = 4,             // Школа магии воды
    eSchoolEarth         = 8,             // Школа магии земли
    eSchoolAll           = 15,            // Все школы магии
    kNumSpellSchools     = 4,             // Кол-во школ магии

    SPELL_SCHOOL_NONE  = const_invalid_school,
    SPELL_SCHOOL_AIR   = eSchoolAir,      // Школа магии воздуха
    SPELL_SCHOOL_FIRE  = eSchoolFire,     // Школа магии огня
    SPELL_SCHOOL_WATER = eSchoolWater,    // Школа магии воды
    SPELL_SCHOOL_EARTH = eSchoolEarth,    // Школа магии земли
    SPELL_SCHOOL_ALL   = eSchoolAll,      // Все школы магии
    MAX_SPELL_SCHOOLS  = kNumSpellSchools // Кол-во школ магии

};

// Spell type /
// Тип заклинания.
enum SpellID : int32_t
{
    SPELL_SUMMON_BOAT    = 0,                          // Вызвать Корабль
    SPELL_SCUTTLE_BOAT   = 1,                          // Затопить Корабль
    SPELL_VISIONS        = 2,                          // Видения
    SPELL_VIEW_EARTH     = 3,                          // Земное Око
    SPELL_DISGUISE       = 4,                          // Маскировка
    SPELL_VIEW_AIR       = 5,                          // Небесное Око
    SPELL_FLY            = 6,                          // Полёт
    SPELL_WATER_WALK     = 7,                          // Хождение По Воде
    SPELL_DIMENSION_DOOR = 8,                          // Дверь Измерений
    SPELL_TOWN_PORTAL    = 9,                          // Городской Портал

    SPELL_QUICKSAND             = 10,                  // Зыбучие Пески
    SPELL_LAND_MINE             = 11,                  // Минное Поле
    SPELL_FORCE_FIELD           = 12,                  // Силовое Поле
    SPELL_FIRE_WALL             = 13,                  // Огненная Стена
    SPELL_EARTHQUAKE            = 14,                  // Землетрясение
    SPELL_MAGIC_ARROW           = 15,                  // Волшебная Стрела
    SPELL_ICE_BOLT              = 16,                  // Ледяная Молния
    SPELL_LIGHTNING_BOLT        = 17,                  // Молния
    SPELL_IMPLOSION             = 18,                  // Взрыв
    SPELL_CHAIN_LIGHTNING       = 19,                  // Цепная Молния
    SPELL_FROST_RING            = 20,                  // Кольцо Холода
    SPELL_FIREBALL              = 21,                  // Огненный Шар
    SPELL_INFERNO               = 22,                  // Инферно
    SPELL_METEOR_SHOWER         = 23,                  // Камнепад
    SPELL_DEATH_RIPPLE          = 24,                  // Волна Смерти
    SPELL_DESTROY_UNDEAD        = 25,                  // Упокоить Нежить
    SPELL_ARMAGEDDON            = 26,                  // Армагеддон
    SPELL_SHIELD                = 27,                  // Щит
    SPELL_AIR_SHIELD            = 28,                  // Воздушный Щит
    SPELL_FIRE_SHIELD           = 29,                  // Огненный Щит
    SPELL_PROTECTION_FROM_AIR   = 30,                  // Оберег от заклинаний Магии Воздуха
    SPELL_PROTECTION_FROM_FIRE  = 31,                  // Оберег от заклинаний Магии Огня
    SPELL_PROTECTION_FROM_WATER = 32,                  // Оберег от заклинаний Магии Воды
    SPELL_PROTECTION_FROM_EARTH = 33,                  // Оберег от заклинаний Магии Земли
    SPELL_ANTI_MAGIC            = 34,                  // Антимагия
    SPELL_DISPEL                = 35,                  // Снятие Заклинаний
    SPELL_MAGIC_MIRROR          = 36,                  // Волшебное Зеркало
    SPELL_CURE                  = 37,                  // Лечение
    SPELL_RESURRECTION          = 38,                  // Воскрешение
    SPELL_ANIMATE_DEAD          = 39,                  // Поднять Нежить
    SPELL_SACRIFICE             = 40,                  // Жертва
    SPELL_BLESS                 = 41,                  // Благословение
    SPELL_CURSE                 = 42,                  // Проклятье
    SPELL_BLOODLUST             = 43,                  // Жажда Крови
    SPELL_PRECISION             = 44,                  // Точность
    SPELL_WEAKNESS              = 45,                  // Слабость
    SPELL_STONE_SKIN            = 46,                  // Каменная Кожа
    SPELL_DISRUPTING_RAY        = 47,                  // Разрушающий Луч
    SPELL_PRAYER                = 48,                  // Молитва
    SPELL_MIRTH                 = 49,                  // Радость
    SPELL_SORROW                = 50,                  // Печаль
    SPELL_FORTUNE               = 51,                  // Удача
    SPELL_MISFORTUNE            = 52,                  // Неудача
    SPELL_HASTE                 = 53,                  // Ускорение
    SPELL_SLOW                  = 54,                  // Медлительность
    SPELL_SLAYER                = 55,                  // Палач
    SPELL_FRENZY                = 56,                  // Бешенство
    SPELL_TITANS_LIGHTNING_BOLT = 57,                  // Гром Титана
    SPELL_COUNTERSTRIKE         = 58,                  // Котрудар
    SPELL_BERSERK               = 59,                  // Берсерк
    SPELL_HYPNOTIZE             = 60,                  // Гипноз
    SPELL_FORGETFULNESS         = 61,                  // Забывчивость
    SPELL_BLIND                 = 62,                  // Слепота
    SPELL_TELEPORT              = 63,                  // Телепорт
    SPELL_REMOVE_OBSTACLE       = 64,                  // Убрать Преграду
    SPELL_CLONE                 = 65,                  // Клон
    SPELL_FIRE_ELEMENTAL        = 66,                  // Вызов Элементаля Огня
    SPELL_EARTH_ELEMENTAL       = 67,                  // Вызов Элементаля Земли
    SPELL_WATER_ELEMENTAL       = 68,                  // Вызов Элементаля Воды
    SPELL_AIR_ELEMENTAL         = 69,                  // Вызов Элементаля Воздуха

    SPELL_STONE                 = 70,                  // Окаменение
    SPELL_POISON                = 71,                  // Яд
    SPELL_BIND                  = 72,                  // Оковы
    SPELL_DESEASE               = 73,                  // Болезнь
    SPELL_PARALYZE              = 74,                  // Паралич
    SPELL_AGE                   = 75,                  // Старость
    SPELL_DEATH_CLOUD           = 76,                  // Облако Смерти
    SPELL_THUNDERBOLT           = 77,                  // Удар Молнии
    SPELL_DISPEL_HELPFUL_SPELLS = 78,                  // Снятие Полезных Чар
    SPELL_DEATH_STARE           = 79,                  // Смертельный Взгляд
    SPELL_ACID_BREATH           = 80,                  // Кислотное Дыхание

    SPELL_NONE                 = -1,                   // Пусто. (Используется для проверок)
    SPELL_INVALID              = SPELL_NONE,           // Пусто. (Используется для проверок)
    INVALID_SPELL              = SPELL_NONE,           // Пусто. (Используется для проверок)
    MAX_SPELLS                 = 81,                   // Кол-во заклинаний в игре
    MAX_BOOK_SPELLS            = 70,                   // Кол-во заклинаний, доступные герою для изучения.
    MAX_COMBAT_SPELLS          = 71,                   // Кол-во боевых заклинаний
    MAX_CREATURE_SPELLS        = 10,                   // Кол-во заклинаний, кастуемых существами
    MAX_ADVENTURE_SPELLS       = 10,                   // Кол-во заклинаний на карте приключений
    MAX_SPELLS_BOOK            = MAX_BOOK_SPELLS,      // Кол-во заклинаний, доступные герою для изучения.
    MAX_SPELLS_COMBAT          = MAX_COMBAT_SPELLS,    // Кол-во боевых заклинаний
    MAX_SPELLS_CREATURE        = MAX_CREATURE_SPELLS,  // Кол-во заклинаний, кастуемых существами
    MAX_SPELLS_ADVENTURE       = MAX_ADVENTURE_SPELLS, // Кол-во заклинаний на карте приключений
    MAX_SPELL_LEVELS           = 5,                    // Максимальное количество уровней заклинаний
    FIRST_ADVENTURE_SPELL      = 0,                    // Тип первого заклинания на карте приключений
    FIRST_COMBAT_SPELL         = 10,                   // Тип первого боевого заклинания
    FIRST_CREATURE_SPELL       = 70,                   // Тип первого заклинания существа
    LAST_ADVENTURE_SPELL       = 9,                    // Тип последнего заклинания на карте приключений
    LAST_COMBAT_SPELL          = 80,                   // Тип последнего боевого заклинания(включая заклинания существ)
    LAST_CREATURE_SPELL        = 80,                   // Тип последнего заклинания существа
    LAST_CASTABLE_COMBAT_SPELL = 69,                   // Тип последнего заклинания, которое может вызвать герой на поле боя
    FIRST_BOOK_SPELL           = SPELL_SUMMON_BOAT,
    LAST_BOOK_SPELL            = SPELL_AIR_ELEMENTAL,
    FIRST_SPELL                = SPELL_SUMMON_BOAT,
    LAST_SPELL                 = SPELL_ACID_BREATH
};

template<>
struct nh3api::enum_limits<SpellID>
    : nh3api::enum_limits_base<SpellID, FIRST_ADVENTURE_SPELL, LAST_COMBAT_SPELL>
{ static inline constexpr bool is_specialized = true; };

// Combat Spell Effect IDs /
// ID Эффектов заклинаний в бою.
enum TSpellEffectID : int32_t
{
    eSpellEffectNone                  = -1, //
    eSpellEffectPrayer                = 0, //
    eSpellEffectLightning_Bolt        = 1, //
    eSpellEffectAirShield             = 2, //
    eSpellEffectBacklash              = 3, //
    eSpellEffectAnimateDead           = 4, //
    eSpellEffectAntiMagic             = 5, //
    eSpellEffectBlind                 = 6, //
    eSpellEffectCounterstroke         = 7, //
    eSpellEffectDeathRipple           = 8, //
    eSpellEffectFireblast             = 9, //
    eSpellEffectDecay                 = 10, //
    eSpellEffectFireShield            = 11, //
    eSpellEffectFirestorm             = 12, //
    eSpellEffectDisruptiveRay_Ray     = 13, //
    eSpellEffectDisruptiveRay_Burst   = 14, //
    eSpellEffectFear                  = 15, //
    eSpellEffectMeteorShower          = 16, //
    eSpellEffectFrenzy                = 17, //
    eSpellEffectFortune               = 18, //
    eSpellEffectMuckAndMire           = 19, //
    eSpellEffectMirth                 = 20, //
    eSpellEffectHypnotize             = 21, //
    eSpellEffectProtectionFromAir     = 22, //
    eSpellEffectProtectionFromWater   = 23, //
    eSpellEffectProtectionFromFire    = 24, //
    eSpellEffectPrecision             = 25, //
    eSpellEffectProtectionFromEarth   = 26, //
    eSpellEffectShield                = 27, //
    eSpellEffectSlayer                = 28, //
    eSpellEffectSacredBreath          = 29, //
    eSpellEffectSorrow                = 30, //
    eSpellEffectTailWind              = 31, //
    eSpellEffectForcefield_2          = 32, //
    eSpellEffectForcefield_3          = 33, //
    eSpellEffectRemoveObstacle        = 34, //
    eSpellEffectBerserk               = 35, //
    eSpellEffectBless                 = 36, //
    eSpellEffectChainLightning_Bolt   = 37, //
    eSpellEffectChainLightning_Dust   = 38, //
    eSpellEffectCure                  = 39, //
    eSpellEffectCurse                 = 40, //
    eSpellEffectDispel                = 41, //
    eSpellEffectForgetfulness         = 42, //
    eSpellEffectFirewall_2            = 43, //
    eSpellEffectFirewall_3            = 44, //
    eSpellEffectFrostRing             = 45, //
    eSpellEffectIceRay_Burst          = 46, //
    eSpellEffectLandMine              = 47, //
    eSpellEffectMisfortune            = 48, //
    eSpellEffectLightning_Dust        = 49, //
    eSpellEffectResurrection          = 50, //
    eSpellEffectSacrifice_Slay        = 51, //
    eSpellEffectSacrifice_Resurrect   = 52, //
    eSpellEffectSpontaneousCombustion = 53, //
    eSpellEffectToughSkin             = 54, //
    eSpellEffectQuicksand             = 55, //
    eSpellEffectWeakness              = 56, //
    eSpellEffectLandMineExplosion     = 57, //
    eSpellEffectDispelQuicksand       = 58, //
    eSpellEffectDispelLandMine        = 59, //
    eSpellEffectDispelForcefield_2    = 60, //
    eSpellEffectDispelForcefield_3    = 61, //
    eSpellEffectDispelFirewall_2      = 62, //
    eSpellEffectDispelFirewall_3      = 63, //
    eSpellEffectMagicBolt_Burst       = 64, //
    eSpellEffectFirewall_1            = 65, //
    eSpellEffectDispelFirewall_1      = 66, //
    eSpellEffectPoison                = 67, //
    eSpellEffectBind                  = 68, //
    eSpellEffectDisease               = 69, //
    eSpellEffectParalyze              = 70, //
    eSpellEffectAge                   = 71, //
    eSpellEffectDeathCloud            = 72, //
    eSpellEffectDeathBlow             = 73, //
    eSpellEffectDrainLife             = 74, //
    eSpellEffectMagicChannel_Suck     = 75, //
    eSpellEffectMagicChannel_Spew     = 76, //
    eSpellEffectMagicDrain            = 77, //
    eSpellEffectMagicResistance       = 78, //
    eSpellEffectRegenerate            = 79, //
    eSpellEffectDeathStare            = 80, //
    eSpellEffectPoof                  = 81, //

    SPELL_EFFECT_PRAYER                = 0, //
    SPELL_EFFECT_LIGHTNING_BOLT        = 1, //
    SPELL_EFFECT_AIRSHIELD             = 2, //
    SPELL_EFFECT_BACKLASH              = 3, //
    SPELL_EFFECT_ANIMATEDEAD           = 4, //
    SPELL_EFFECT_ANTIMAGIC             = 5, //
    SPELL_EFFECT_BLIND                 = 6, //
    SPELL_EFFECT_COUNTERSTROKE         = 7, //
    SPELL_EFFECT_DEATHRIPPLE           = 8, //
    SPELL_EFFECT_FIREBLAST             = 9, //
    SPELL_EFFECT_DECAY                 = 10, //
    SPELL_EFFECT_FIRESHIELD            = 11, //
    SPELL_EFFECT_FIRESTORM             = 12, //
    SPELL_EFFECT_DISRUPTIVERAY_RAY     = 13, //
    SPELL_EFFECT_DISRUPTIVERAY_BURST   = 14, //
    SPELL_EFFECT_FEAR                  = 15, //
    SPELL_EFFECT_METEORSHOWER          = 16, //
    SPELL_EFFECT_FRENZY                = 17, //
    SPELL_EFFECT_FORTUNE               = 18, //
    SPELL_EFFECT_MUCKANDMIRE           = 19, //
    SPELL_EFFECT_MIRTH                 = 20, //
    SPELL_EFFECT_HYPNOTIZE             = 21, //
    SPELL_EFFECT_PROTECTIONFROMAIR     = 22, //
    SPELL_EFFECT_PROTECTIONFROMWATER   = 23, //
    SPELL_EFFECT_PROTECTIONFROMFIRE    = 24, //
    SPELL_EFFECT_PRECISION             = 25, //
    SPELL_EFFECT_PROTECTIONFROMEARTH   = 26, //
    SPELL_EFFECT_SHIELD                = 27, //
    SPELL_EFFECT_SLAYER                = 28, //
    SPELL_EFFECT_SACREDBREATH          = 29, //
    SPELL_EFFECT_SORROW                = 30, //
    SPELL_EFFECT_TAILWIND              = 31, //
    SPELL_EFFECT_FORCEFIELD_2          = 32, //
    SPELL_EFFECT_FORCEFIELD_3          = 33, //
    SPELL_EFFECT_REMOVEOBSTACLE        = 34, //
    SPELL_EFFECT_BERSERK               = 35, //
    SPELL_EFFECT_BLESS                 = 36, //
    SPELL_EFFECT_CHAINLIGHTNING_BOLT   = 37, //
    SPELL_EFFECT_CHAINLIGHTNING_DUST   = 38, //
    SPELL_EFFECT_CURE                  = 39, //
    SPELL_EFFECT_CURSE                 = 40, //
    SPELL_EFFECT_DISPEL                = 41, //
    SPELL_EFFECT_FORGETFULNESS         = 42, //
    SPELL_EFFECT_FIREWALL_2            = 43, //
    SPELL_EFFECT_FIREWALL_3            = 44, //
    SPELL_EFFECT_FROSTRING             = 45, //
    SPELL_EFFECT_ICERAY_BURST          = 46, //
    SPELL_EFFECT_LANDMINE              = 47, //
    SPELL_EFFECT_MISFORTUNE            = 48, //
    SPELL_EFFECT_LIGHTNING_DUST        = 49, //
    SPELL_EFFECT_RESURRECTION          = 50, //
    SPELL_EFFECT_SACRIFICE_SLAY        = 51, //
    SPELL_EFFECT_SACRIFICE_RESURRECT   = 52, //
    SPELL_EFFECT_SPONTANEOUSCOMBUSTION = 53, //
    SPELL_EFFECT_TOUGHSKIN             = 54, //
    SPELL_EFFECT_QUICKSAND             = 55, //
    SPELL_EFFECT_WEAKNESS              = 56, //
    SPELL_EFFECT_LANDMINEEXPLOSION     = 57, //
    SPELL_EFFECT_DISPELQUICKSAND       = 58, //
    SPELL_EFFECT_DISPELLANDMINE        = 59, //
    SPELL_EFFECT_DISPELFORCEFIELD_2    = 60, //
    SPELL_EFFECT_DISPELFORCEFIELD_3    = 61, //
    SPELL_EFFECT_DISPELFIREWALL_2      = 62, //
    SPELL_EFFECT_DISPELFIREWALL_3      = 63, //
    SPELL_EFFECT_MAGICBOLT_BURST       = 64, //
    SPELL_EFFECT_FIREWALL_1            = 65, //
    SPELL_EFFECT_DISPELFIREWALL_1      = 66, //
    SPELL_EFFECT_POISON                = 67, //
    SPELL_EFFECT_BIND                  = 68, //
    SPELL_EFFECT_DISEASE               = 69, //
    SPELL_EFFECT_PARALYZE              = 70, //
    SPELL_EFFECT_AGE                   = 71, //
    SPELL_EFFECT_DEATHCLOUD            = 72, //
    SPELL_EFFECT_DEATHBLOW             = 73, //
    SPELL_EFFECT_DRAINLIFE             = 74, //
    SPELL_EFFECT_MAGICCHANNEL_SUCK     = 75, //
    SPELL_EFFECT_MAGICCHANNEL_SPEW     = 76, //
    SPELL_EFFECT_MAGICDRAIN            = 77, //
    SPELL_EFFECT_MAGICRESISTANCE       = 78, //
    SPELL_EFFECT_REGENERATE            = 79, //
    SPELL_EFFECT_DEATHSTARE            = 80, //
    SPELL_EFFECT_POOF                  = 81, //
    MAX_SPELL_EFFECTS                  = 82,
    kNumSpellEffects                   = 82
};

template<>
struct nh3api::enum_limits<TSpellEffectID>
    : nh3api::enum_limits_base<TSpellEffectID, SPELL_EFFECT_PRAYER, SPELL_EFFECT_POOF>
{ static inline constexpr bool is_specialized = true; };

enum FSpellEffectFlags : uint32_t
{
    SPELL_EFFECT_FLAG_BELOW       = 0b000000000,
    SPELL_EFFECT_FLAG_MIDDLE      = 0b000000001,
    SPELL_EFFECT_FLAG_ABOVE       = 0b000000010,
    SPELL_EFFECT_FLAG_AHEAD       = 0b000000011,
    SPELL_EFFECT_FLAG_LEFT_BELOW  = 0b000000100,
    SPELL_EFFECT_FLAG_UNSPECIFIED = 0b000001111,
    SPELL_EFFECT_FLAG_TRANSPARENT = 0b100000000,
};

// Spell flags /
// Флаги заклинаний
NH3API_FLAG_ENUM FSpellFlags : uint32_t
{
    SF_BATTLE_SPELL          = 0b000000000000000000001,      // Боевое заклинание
    SF_MAP_SPELL             = 0b000000000000000000010,      // Заклинание карты приключений
    SF_TIME_SCALE            = 0b000000000000000000100,      //
    SF_CREATURE_SPELL        = 0b000000000000000001000,      // Заклинание существа
    SF_SINGLE_TARGET         = 0b000000000000000010000,     // Заклинание, которое направлено на одну цель
    SF_SINGLE_SHOOTING_STACK = 0b000000000000000100000,     //
    SF_EXPERT_MASS_VERSION   = 0b000000000000001000000,     // Массовая(Экспертная ступень) версия заклинания
    SF_TARGET_ANYWHERE       = 0b000000000000010000000,     // Массовое заклинание
    SF_REMOVE_OBSTACLE       = 0b000000000000100000000,    // Убрать препятствие
    SF_DAMAGE_SPELL          = 0b000000000001000000000,    // Заклинание, наносящее урон
    SF_MIND_SPELL            = 0b000000000010000000000,    // Заклинание разума
    SF_FRIENDLY_MASS         = 0b000000000100000000000,    // Заклинание для собственных существ
    SF_NOT_AT_WAR_MACHINE    = 0b000000001000000000000,   // Заклинание, направленное НЕ на боевые машины
    SF_SPELL_FROM_ARTIFACT   = 0b000000010000000000000,   // Заклинание, полученное с помощью артефакта.
    SF_DEFENSIVE             = 0b000000100000000000000,   //
    SF_AI_DAMAGE_SPELL       = 0b000001000000000000000,   // ИИ: Заклинание, наносящее урон
    SF_AI_AREA_EFFECT        = 0b000010000000000000000,  // ИИ: Заклинание, действующее по площади
    SF_AI_MASS_DAMAGE_SPELL  = 0b000100000000000000000,  // ИИ: Массовое заклинание
    SF_AI_NON_DAMAGE_SPELL   = 0b001000000000000000000,  // ИИ: Любое другое заклинание кроме того, что наносят урон
    SF_AI_CREATURES          = 0b01000000000000000000,  // ИИ: Заклинание, действующее на существ
    SF_AI_ADVENTURE_MAP      = 0b100000000000000000000, // ИИ: Заклинание карты приключений
};

#pragma pack(push, 4)
// Spell traits /
// Свойства заклинаний.
// size = 0x88 = 136, align = 4
struct TSpellTraits
{
    // Target type /
    // Тип цели.
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t m_karma;

    // Cast sample name /
    // Название звукового файла заклинания.
    // offset: +0x4 = +4,  size = 0x4 = 4
    const char* m_sample;

    // Spell effect /
    // Эффект заклинания.
    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t m_effect;

    // Spell flags /
    // Флаги заклинания.
    // offset: +0xC = +12,  size = 0x4 = 4
    FSpellFlags m_flags;

    // Spell name /
    // Название заклинания.
    // offset: +0x10 = +16,  size = 0x4 = 4
    const char* m_name;

    // Abbreviated name /
    // Сокращённое название(зачастую то же, что и обычное).
    // offset: +0x14 = +20,  size = 0x4 = 4
    const char* m_abbreviated_name;

    // Spell level /
    // Уровень заклинания.
    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t m_level;

    // Spell school /
    // Школа заклинаний
    // offset: +0x1C = +28,  size = 0x4 = 4
    TSpellSchool m_school;

    // Mana cost for each level of spell /
    // Стоимость заклинания в мане для каждого из уровней мастерства школы магии.
    // offset: +0x20 = +32,  size = 0x10 = 16
    std::array<int32_t, 4> m_manaCost;

    // Spell power factor /
    // Множитель силы.
    // offset: +0x30 = +48,  size = 0x4 = 4
    int32_t m_power_factor;

    // Spell mastery bonus /
    // Бонус мастерств для каждого из уровней мастерства школы магии.
    // offset: +0x34 = +52,  size = 0x10 = 16
    std::array<int32_t, 4> m_mastery_bonus;

    // Chance of getting spell in each town type /
    // %, Шанс получения заклинания в каждой фракции.
    // offset: +0x44 = +68,  size = 0x24 = 36
    std::array<int32_t, 9> m_townGetsItChance;

    // AI value of spell for each level /
    // Ценность заклинания для ИИ для каждого из уровней мастерства школы магии.
    // offset: +0x68 = +104,  size = 0x10 = 16
    std::array<int32_t, 4> m_AI_value;

    // Description for each level /
    // Описание заклинания для каждого из уровней мастерства школы магии.
    // offset: +0x78 = +120,  size = 0x10 = 16
    std::array<const char*, 4> m_description;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x88, TSpellTraits);

[[nodiscard]] inline TCreatureType get_elemental_type(SpellID spell)
{ return FASTCALL_1(TCreatureType, 0x5A9670, spell); }

[[nodiscard]] inline int32_t modify_spell_damage(int32_t damage, SpellID spell, TCreatureType creature)
{ return FASTCALL_3(int32_t, 0x44B180, damage, spell, creature); }

// AI value of spell /
// Ценность заклинания для ИИ.
[[nodiscard]] inline int32_t ValueOfSpell(const hero* current_hero, SpellID spell)
{ return (current_hero) ? THISCALL_2(int32_t, 0x527F80, current_hero, spell) : 0; }

// Spell traits array /
// Таблица свойств заклинаний.
inline std::array<TSpellTraits, MAX_SPELLS>& akSpellTraits = get_global_var_ref(0x6854A0, std::array<TSpellTraits, MAX_SPELLS>);
