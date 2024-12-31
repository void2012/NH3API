//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/intrin.hpp"
#include "nh3api_std/array.hpp"

// Secondary skill mastery /
// Ступень вторичного
enum TSkillMastery : int32_t
{
    eMasteryInvalid  = -1, // Пустое (используется для проверок)
    eMasteryNone     = 0,  // Нет навыка
    eMasteryBasic    = 1,  // Базовый (1 ступень)
    eMasteryAdvanced = 2,  // Продвинутый (2 ступень)
    eMasteryExpert   = 3,  // Эксперт (3 ступень)
    kNumMasteries    = 4,  // Количество возможных ступеней навыков
};

// Hero's secondary skill /
// Вторичный навык героя.
enum TSecondarySkill : int32_t
{
    SKILL_PATHFINDING  = 0,  // Поиск пути
    SKILL_ARCHERY      = 1,  // Стрелок
    SKILL_LOGISTICS    = 2,  // Логистика
    SKILL_SCOUTING     = 3,  // Разведка
    SKILL_DIPLOMACY    = 4,  // Дипломатия
    SKILL_NAVIGATION   = 5,  // Навигация
    SKILL_LEADERSHIP   = 6,  // Лидерство
    SKILL_WISDOM       = 7,  // Мудрость
    SKILL_MYSTICISM    = 8,  // Мистицизм
    SKILL_LUCK         = 9,  // Удача
    SKILL_BALLISTICS   = 10, // Баллистика
    SKILL_EAGLE_EYE    = 11, // Орлиный глаз
    SKILL_NECROMANCY   = 12, // Некромантия
    SKILL_ESTATES      = 13, // Казначей
    SKILL_FIRE_MAGIC   = 14, // Магия Огня
    SKILL_AIR_MAGIC    = 15, // Магия Воздуха
    SKILL_WATER_MAGIC  = 16, // Магия Воды
    SKILL_EARTH_MAGIC  = 17, // Магия Земли
    SKILL_SCHOLAR      = 18, // Книжник
    SKILL_TACTICS      = 19, // Тактика
    SKILL_ARTILLERY    = 20, // Артиллерия
    SKILL_LEARNING     = 21, // Обучение
    SKILL_OFFENSE      = 22, // Нападение
    SKILL_ARMORER      = 23, // Доспехи
    SKILL_INTELLIGENCE = 24, // Интеллект
    SKILL_SORCERY      = 25, // Волшебство
    SKILL_RESISTANCE   = 26, // Сопротивление
    SKILL_FIRST_AID    = 27, // Первая помощь

    SKILL_NONE           = -1,
    MAX_SECONDARY_SKILLS = 28, // Количество вторичных навыков в игре
    MAX_SKILLS_PER_HERO  = 8,  // Максимальное количество вторичных навыков героя

};

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
};

#pragma pack(push, 4)
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
    std::array<const char*, kNumMasteries - 1> desc;
};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
// Secondary Skill traits table. Filled by game from "SSTRAITS.TXT" /
// Таблица свойств вторичных навыков. Заполняется игрой из файла "SSTRAITS.TXT"
std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>& akSSkillTraits 
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698D88, std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>));

NH3API_INLINE_OR_EXTERN
// Luck secondary skill bonus /
// Бонус навыка "Удача"
const std::array<int32_t, kNumMasteries>& luck_bonus
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E998, std::array<int32_t, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Leadership secondary skill bonus /
// Бонус навыка "Лидерство"
const std::array<int32_t, kNumMasteries>& leadership_bonus
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9A8, std::array<int32_t, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Leadership secondary skill factor /
// Множитель Некромантии
const std::array<float, kNumMasteries>& SSNecromancyFactor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9B8, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Mysticism secondary skill bonus /
// Бонус навыка "Мистицизм"
const std::array<int32_t, kNumMasteries>& mysticism_bonus
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9C8, std::array<int32_t, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Scouting secondary skill bonus /
// Бонус навыка "Разведка"
const std::array<int32_t, kNumMasteries>& SSScoutingRange
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9D8, std::array<int32_t, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Archery secondary skill factor /
// Множитель навыка "Стрельба"
const std::array<float, kNumMasteries>& SSArcheryMod
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9E8, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Offense secondary skill factor /
// Множитель навыка "Нападение"
const std::array<float, kNumMasteries>& offense_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63E9F8, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Defense secondary skill factor /
// Множитель навыка "Доспехи"
const std::array<float, kNumMasteries>& defense_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA08, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Estates secondary skill bonus /
// Бонус навыка "Казначей"
const std::array<int32_t, kNumMasteries>& estates_bonus
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA18, std::array<int32_t, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Eagle eye skill chance /
// Шанс навыка "Орлиный взор"
const std::array<float, kNumMasteries>& SSEagleEyeChance
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA28, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Diplomacy secondary skill factor /
// Множитель навыка "Дипломатия"
const std::array<float, kNumMasteries>& SSDiplomacyFactor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA38, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Diplomacy secondary skill factor /
// Множитель навыка "Сопротивление"
const std::array<float, kNumMasteries>& resistance_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA48, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Diplomacy secondary skill factor /
// Множитель навыка "Обучение"
const std::array<float, kNumMasteries>& learning_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA58, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Logitics secondary skill factor /
// Множитель навыка "Логистика"
const std::array<float, kNumMasteries>& logistics_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA68, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Sorcery secondary skill factor /
// Множитель навыка "Волшебство"
const std::array<float, kNumMasteries>& sorcery_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA78, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// Intelligence secondary skill factor /
// Множитель навыка "Интеллект"
const std::array<float, kNumMasteries>& intelligence_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA88, std::array<float, kNumMasteries>));

NH3API_INLINE_OR_EXTERN
// First Aid secondary skill factor /
// Множитель навыка "Первая помощь"
const std::array<float, kNumMasteries>& firstaid_factor
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x63EA98, std::array<float, kNumMasteries>));