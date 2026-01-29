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

#include <array>

#include "nh3api_std/enum_limits.hpp"
#include "nh3api_std/intrin.hpp"

// Secondary skill mastery /
// Ступень вторичного навыка.
enum TSkillMastery : int32_t
{
    eMasteryInvalid  = -1,                     // Пустое (используется для проверок)
    eMasteryNone     = 0,                      // Нет навыка
    eMasteryBasic    = 1,                      // Базовый (1 ступень)
    eMasteryAdvanced = 2,                      // Продвинутый (2 ступень)
    eMasteryExpert   = 3,                      // Эксперт (3 ступень)
    kNumMasteries    = 4,                      // Количество возможных ступеней навыков

    SKILL_MASTERY_INVALID  = eMasteryInvalid,  // Пустое (используется для проверок)
    SKILL_MASTERY_NONE     = eMasteryNone,     // Нет навыка
    SKILL_MASTERY_BASIC    = eMasteryBasic,    // Базовый (1 ступень)
    SKILL_MASTERY_ADVANCED = eMasteryAdvanced, // Продвинутый (2 ступень)
    SKILL_MASTERY_EXPERT   = eMasteryExpert,   // Эксперт (3 ступень)
    MAX_SKILL_MASTERIES    = kNumMasteries,    // Количество возможных ступеней навыков
    FIRST_SKILL_MASTERY    = SKILL_MASTERY_NONE,
    LAST_SKILL_MASTERY     = SKILL_MASTERY_EXPERT
};

template<>
struct nh3api::enum_limits<TSkillMastery>
    : nh3api::enum_limits_base<TSkillMastery, FIRST_SKILL_MASTERY, LAST_SKILL_MASTERY>
{ static inline constexpr bool is_specialized = true; };

// Hero's secondary skill /
// Вторичный навык героя.
enum TSecondarySkill : int32_t
{
    SKILL_PATHFINDING  = 0,                      // Поиск пути
    SKILL_ARCHERY      = 1,                      // Стрельба
    SKILL_LOGISTICS    = 2,                      // Логистика
    SKILL_SCOUTING     = 3,                      // Разведка
    SKILL_DIPLOMACY    = 4,                      // Дипломатия
    SKILL_NAVIGATION   = 5,                      // Навигация
    SKILL_LEADERSHIP   = 6,                      // Лидерство
    SKILL_WISDOM       = 7,                      // Мудрость
    SKILL_MYSTICISM    = 8,                      // Мистицизм
    SKILL_LUCK         = 9,                      // Удача
    SKILL_BALLISTICS   = 10,                     // Баллистика
    SKILL_EAGLE_EYE    = 11,                     // Орлиный глаз
    SKILL_NECROMANCY   = 12,                     // Некромантия
    SKILL_ESTATES      = 13,                     // Казначей
    SKILL_FIRE_MAGIC   = 14,                     // Магия Огня
    SKILL_AIR_MAGIC    = 15,                     // Магия Воздуха
    SKILL_WATER_MAGIC  = 16,                     // Магия Воды
    SKILL_EARTH_MAGIC  = 17,                     // Магия Земли
    SKILL_SCHOLAR      = 18,                     // Книжник
    SKILL_TACTICS      = 19,                     // Тактика
    SKILL_ARTILLERY    = 20,                     // Артиллерия
    SKILL_LEARNING     = 21,                     // Обучение
    SKILL_OFFENSE      = 22,                     // Нападение
    SKILL_ARMORER      = 23,                     // Доспехи
    SKILL_INTELLIGENCE = 24,                     // Интеллект
    SKILL_SORCERY      = 25,                     // Волшебство
    SKILL_RESISTANCE   = 26,                     // Сопротивление
    SKILL_FIRST_AID    = 27,                     // Первая помощь

    SKILL_NONE            = -1,
    MAX_SECONDARY_SKILLS  = 28,                  // Количество вторичных навыков в игре
    MAX_SKILLS_PER_HERO   = 8,                   // Максимальное количество вторичных навыков героя
    MAX_HERO_SKILLS       = MAX_SKILLS_PER_HERO, // Максимальное количество вторичных навыков героя
    FIRST_SECONDARY_SKILL = SKILL_PATHFINDING,
    LAST_SECONDARY_SKILL  = SKILL_FIRST_AID
};

template<>
struct nh3api::enum_limits<TSecondarySkill>
    : nh3api::enum_limits_base<TSecondarySkill, SKILL_PATHFINDING, SKILL_FIRST_AID>
{ static inline constexpr bool is_specialized = true; };

// Hero's primary skill /
// Первичный навык героя.
enum TPrimarySkill : int32_t
{
    ePriSkillAttack       = 0,  // Атака
    ePriSkillDefense      = 1,  // Защита
    ePriSkillPower        = 2,  // Сила магии
    ePriSkillKnowledge    = 3,  // Знания
    kNumPrimarySkills     = 4,  // Количество первичных навыков в игре
    kMaxPrimarySkillLevel = 99, // Максимальный уровень первичного навыка в игре

    FIRST_PRIMARY_SKILL     = ePriSkillAttack,
    LAST_PRIMARY_SKILL      = ePriSkillKnowledge,
    PRIMARY_SKILL_ATTACK    = ePriSkillAttack,
    PRIMARY_SKILL_DEFENSE   = ePriSkillDefense,
    PRIMARY_SKILL_POWER     = ePriSkillPower,
    PRIMARY_SKILL_KNOWLEDGE = ePriSkillKnowledge,
    MAX_PRIMARY_SKILLS      = kNumPrimarySkills,
    MAX_PRIMARY_SKILL_LEVEL = kMaxPrimarySkillLevel
};

// Secondary skill traits /
// Свойства вторичных навыков.
// size = 0x10 = 16, align = 4
struct TSSkillTraits
{
    // Skill name /
    // Имя навыка.
    // offset: +0x0 = +0,  size: 0x4 = 4
    const char* name;

    // Skill description for three skill masteries /
    // Описание навыка для каждой из ступеней.
    // offset: +0x4 = +4,  size: 0xC = 12
    std::array<const char*, 3> desc;

};

// Secondary Skill Data in Pandora's box and Seer's Hut /
// Данные вторичного навыка вознаграждения в ящике пандоры и хижине провидца.
// size = 0x8 = 8, align = 4
struct SecondarySkillData
{
    // Secondary skill /
    // Вторичный навык.
    // offset: +0x0 = +0,  size = 0x4 = 4
    TSecondarySkill type;

    // Secondary skill mastery /
    // Ступень вторичного навыка.
    // offset: +0x4 = +4,  size = 0x4 = 4
    TSkillMastery level;

};

// Secondary Skill traits table. Filled by game from "SSTRAITS.TXT" /
// Таблица свойств вторичных навыков. Заполняется игрой из файла "SSTRAITS.TXT"
inline std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>& akSSkillTraits
= get_global_var_ref(0x698D88, std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>);

// Luck secondary skill bonus /
// Бонус навыка "Удача"
inline const std::array<int32_t, MAX_SKILL_MASTERIES>& luck_bonus
= get_global_var_ref(0x63E998, std::array<int32_t, MAX_SKILL_MASTERIES>);

// Leadership secondary skill bonus /
// Бонус навыка "Лидерство"
inline const std::array<int32_t, MAX_SKILL_MASTERIES>& leadership_bonus
= get_global_var_ref(0x63E9A8, std::array<int32_t, MAX_SKILL_MASTERIES>);

// Leadership secondary skill factor /
// Множитель Некромантии
inline const std::array<float, MAX_SKILL_MASTERIES>& SSNecromancyFactor
= get_global_var_ref(0x63E9B8, std::array<float, MAX_SKILL_MASTERIES>);

// Mysticism secondary skill bonus /
// Бонус навыка "Мистицизм"
inline const std::array<int32_t, MAX_SKILL_MASTERIES>& mysticism_bonus
= get_global_var_ref(0x63E9C8, std::array<int32_t, MAX_SKILL_MASTERIES>);

// Scouting secondary skill bonus /
// Бонус навыка "Разведка"
inline const std::array<int32_t, MAX_SKILL_MASTERIES>& SSScoutingRange
= get_global_var_ref(0x63E9D8, std::array<int32_t, MAX_SKILL_MASTERIES>);

// Archery secondary skill factor /
// Множитель навыка "Стрельба"
inline const std::array<float, MAX_SKILL_MASTERIES>& SSArcheryMod
= get_global_var_ref(0x63E9E8, std::array<float, MAX_SKILL_MASTERIES>);

// Offense secondary skill factor /
// Множитель навыка "Нападение"
inline const std::array<float, MAX_SKILL_MASTERIES>& offense_factor
= get_global_var_ref(0x63E9F8, std::array<float, MAX_SKILL_MASTERIES>);

// Defense secondary skill factor /
// Множитель навыка "Доспехи"
inline const std::array<float, MAX_SKILL_MASTERIES>& defense_factor
= get_global_var_ref(0x63EA08, std::array<float, MAX_SKILL_MASTERIES>);

// Estates secondary skill bonus /
// Бонус навыка "Казначей"
inline const std::array<int32_t, MAX_SKILL_MASTERIES>& estates_bonus
= get_global_var_ref(0x63EA18, std::array<int32_t, MAX_SKILL_MASTERIES>);

// Eagle eye skill chance /
// Шанс навыка "Орлиный взор"
inline const std::array<float, MAX_SKILL_MASTERIES>& SSEagleEyeChance
= get_global_var_ref(0x63EA28, std::array<float, MAX_SKILL_MASTERIES>);

// Diplomacy secondary skill factor /
// Множитель навыка "Дипломатия"
inline const std::array<float, MAX_SKILL_MASTERIES>& SSDiplomacyFactor
= get_global_var_ref(0x63EA38, std::array<float, MAX_SKILL_MASTERIES>);

// Diplomacy secondary skill factor /
// Множитель навыка "Сопротивление"
inline const std::array<float, MAX_SKILL_MASTERIES>& resistance_factor
= get_global_var_ref(0x63EA48, std::array<float, MAX_SKILL_MASTERIES>);

// Diplomacy secondary skill factor /
// Множитель навыка "Обучение"
inline const std::array<float, MAX_SKILL_MASTERIES>& learning_factor
= get_global_var_ref(0x63EA58, std::array<float, MAX_SKILL_MASTERIES>);

// Logitics secondary skill factor /
// Множитель навыка "Логистика"
inline const std::array<float, MAX_SKILL_MASTERIES>& logistics_factor
= get_global_var_ref(0x63EA68, std::array<float, MAX_SKILL_MASTERIES>);

// Sorcery secondary skill factor /
// Множитель навыка "Волшебство"
inline const std::array<float, MAX_SKILL_MASTERIES>& sorcery_factor
= get_global_var_ref(0x63EA78, std::array<float, MAX_SKILL_MASTERIES>);

// Intelligence secondary skill factor /
// Множитель навыка "Интеллект"
inline const std::array<float, MAX_SKILL_MASTERIES>& intelligence_factor
= get_global_var_ref(0x63EA88, std::array<float, MAX_SKILL_MASTERIES>);

// First Aid secondary skill factor /
// Множитель навыка "Первая помощь"
inline const std::array<float, MAX_SKILL_MASTERIES>& firstaid_factor
= get_global_var_ref(0x63EA98, std::array<float, MAX_SKILL_MASTERIES>);
