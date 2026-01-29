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

#include "nh3api_std/exe_string.hpp" // exe_string
#include "terrain.hpp"               // EMagicTerrain, TTerrainType, TTownType, kNumTowns

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// Creature type /
// Тип существа.
enum TCreatureType : int32_t
{

    // Creatures of Castle /
    // Существа Замка.

    CREATURE_PIKEMAN             =  0, // Копейщик
    CREATURE_HALBERDIER          =  1, // Алебардщик
    CREATURE_ARCHER              =  2, // Стрелок
    CREATURE_MARKSMAN            =  3, // Лучник
    CREATURE_GRIFFIN             =  4, // Грифон
    CREATURE_ROYAL_GRIFFIN       =  5, // Королевский грифон
    CREATURE_SWORDSMAN           =  6, // Мечник
    CREATURE_CRUSADER            =  7, // Крестоносец
    CREATURE_MONK                =  8, // Монах
    CREATURE_ZEALOT              =  9, // Фанатик
    CREATURE_CAVALIER            = 10, // Всадник
    CREATURE_CHAMPION            = 11, // Чемпион
    CREATURE_ANGEL               = 12, // Ангел
    CREATURE_ARCHANGEL           = 13, // Архангел

    // Creatures of Rampart /
    // Существа Оплота.

    CREATURE_CENTAUR             = 14, // Кентавр
    CREATURE_CENTAUR_CAPTAIN     = 15, // Капитан кентавров
    CREATURE_DWARF               = 16, // Гном
    CREATURE_BATTLE_DWARF        = 17, // Боевой гном
    CREATURE_WOOD_ELF            = 18, // Лесной эльф
    CREATURE_GRAND_ELF           = 19, // Высокий эльф
    CREATURE_PEGASUS             = 20, // Пегас
    CREATURE_SILVER_PEGASUS      = 21, // Серебряный пегас
    CREATURE_DENDROID_GUARD      = 22, // Дендроид-страж
    CREATURE_DENDROID_SOLDIER    = 23, // Дендроид-воин
    CREATURE_UNICORN             = 24, // Единорог
    CREATURE_WAR_UNICORN         = 25, // Боевой единорог
    CREATURE_GREEN_DRAGON        = 26, // Зеленый дракон
    CREATURE_GOLD_DRAGON         = 27, // Золотой дракон

    // Creatures of Tower /
    // Существа Башни.

    CREATURE_GREMLIN             = 28, // Гремлин
    CREATURE_MASTER_GREMLIN      = 29, // Мастер-гремлин
    CREATURE_STONE_GARGOYLE      = 30, // Каменная горгулья
    CREATURE_OBSIDIAN_GARGOYLE   = 31, // Обсидиановая горгулья
    CREATURE_STONE_GOLEM         = 32, // Каменный голем
    CREATURE_IRON_GOLEM          = 33, // Железный голем
    CREATURE_MAGE                = 34, // Маг
    CREATURE_ARCH_MAGE           = 35, // Архимаг
    CREATURE_GENIE               = 36, // Джинн
    CREATURE_MASTER_GENIE        = 37, // Владыка джиннов
    CREATURE_NAGA                = 38, // Нага
    CREATURE_NAGA_QUEEN          = 39, // Королева нага
    CREATURE_GIANT               = 40, // Гигант
    CREATURE_TITAN               = 41, // Титан

    // Creatures of Inferno /
    // Существа Инферно.

    CREATURE_IMP                 = 42, // Бес
    CREATURE_FAMILIAR            = 43, // Чёрт
    CREATURE_GOG                 = 44, // Гог
    CREATURE_MAGOG               = 45, // Магог
    CREATURE_HELL_HOUND          = 46, // Гончая ада
    CREATURE_CERBERUS            = 47, // Цербер
    CREATURE_DEMON               = 48, // Демон
    CREATURE_HORNED_DEMON        = 49, // Рогатый демон
    CREATURE_PIT_FIEND           = 50, // Демон бездны
    CREATURE_PIT_LORD            = 51, // Владыка бездны
    CREATURE_EFREETI             = 52, // Ифрит
    CREATURE_EFREET_SULTAN       = 53, // Ифрит-султан
    CREATURE_DEVIL               = 54, // Дьявол
    CREATURE_ARCH_DEVIL          = 55, // Архидьявол

    // Creatures of Necropolis /
    // Существа Некрополиса.

    CREATURE_SKELETON            = 56, // Скелет
    CREATURE_SKELETON_WARRIOR    = 57, // Скелет-воин
    CREATURE_WALKING_DEAD        = 58, // Ходячий мертвец
    CREATURE_ZOMBIE              = 59, // Зомби
    CREATURE_WIGHT               = 60, // Призрак
    CREATURE_WRAITH              = 61, // Привидение
    CREATURE_VAMPIRE             = 62, // Вампир
    CREATURE_VAMPIRE_LORD        = 63, // Лорд Вампиров
    CREATURE_LICH                = 64, // Лич
    CREATURE_POWER_LICH          = 65, // Могучий лич
    CREATURE_BLACK_KNIGHT        = 66, // Черный рыцарь
    CREATURE_DREAD_KNIGHT        = 67, // Зловещий рыцарь
    CREATURE_BONE_DRAGON         = 68, // Костяной дракон
    CREATURE_GHOST_DRAGON        = 69, // Призрачный дракон

    // Creatures of Dungeon /
    // Существа Темницы.

    CREATURE_TROGLODYTE          = 70, // Троглодит
    CREATURE_INFERNAL_TROGLODYTE = 71, // Адский троглодит
    CREATURE_HARPY               = 72, // Гарпия
    CREATURE_HARPY_HAG           = 73, // Гарпия-ведьма
    CREATURE_BEHOLDER            = 74, // Бехолдер
    CREATURE_EVIL_EYE            = 75, // Злобоглаз
    CREATURE_MEDUSA              = 76, // Медуза
    CREATURE_MEDUSA_QUEEN        = 77, // Королева медуз
    CREATURE_MINOTAUR            = 78, // Минотавр
    CREATURE_MINOTAUR_KING       = 79, // Королевский минотавр
    CREATURE_MANTICORE           = 80, // Мантикора
    CREATURE_SCORPICORE          = 81, // Скорпикора
    CREATURE_RED_DRAGON          = 82, // Красный дракон
    CREATURE_BLACK_DRAGON        = 83, // Черный дракон

    // Creatures of Stronghold /
    // Существа Цитадели.

    CREATURE_GOBLIN              = 84, // Гоблин
    CREATURE_HOBGOBLIN           = 85, // Хобгоблин
    CREATURE_WOLF_RIDER          = 86, // Наездник на волке
    CREATURE_WOLF_RAIDER         = 87, // Разбойник на волке
    CREATURE_ORC                 = 88, // Орк
    CREATURE_ORC_CHIEFTAIN       = 89, // Вождь орков
    CREATURE_OGRE                = 90, // Огр
    CREATURE_OGRE_MAGE           = 91, // Огр-маг
    CREATURE_ROC                 = 92, // Рух
    CREATURE_THUNDERBIRD         = 93, // Птица грома
    CREATURE_CYCLOPS             = 94, // Циклоп
    CREATURE_CYCLOPS_KING        = 95, // Королевский циклоп
    CREATURE_BEHEMOTH            = 96, // Чудище
    CREATURE_ANCIENT_BEHEMOTH    = 97, // Древнее чудище

    // Creatures of Fortress /
    // Существа Крепости.

    CREATURE_GNOLL               = 98, // Гнолл
    CREATURE_GNOLL_MARAUDER      = 99, // Гнолл-мародёр
    CREATURE_LIZARDMAN           = 100,// Ящер
    CREATURE_LIZARD_WARRIOR      = 101,// Ящер-воин
    CREATURE_GORGON              = 102,// Горгона
    CREATURE_MIGHTY_GORGON       = 103,// Могучая горгона
    CREATURE_SERPENT_FLY         = 104,// Летучий змей
    CREATURE_DRAGON_FLY          = 105,// Летучий змий
    CREATURE_BASILISK            = 106,// Василиск
    CREATURE_GREATER_BASILISK    = 107,// Великий василиск
    CREATURE_WYVERN              = 108,// Виверна
    CREATURE_WYVERN_MONARCH      = 109,// Виверна-монарх
    CREATURE_HYDRA               = 110,// Гидра
    CREATURE_CHAOS_HYDRA         = 111,// Гидра хаоса

    CREATURE_AIR_ELEMENTAL       = 112,// Воздушный элементал(ь)
    CREATURE_EARTH_ELEMENTAL     = 113,// Земляной элементал(ь)
    CREATURE_FIRE_ELEMENTAL      = 114,// Огненный элементал(ь)
    CREATURE_WATER_ELEMENTAL     = 115,// Водный элементал(ь)

    CREATURE_GOLD_GOLEM          = 116,// Золотой голем
    CREATURE_DIAMOND_GOLEM       = 117,// Алмазный голем

    CREATURE_PIXIE               = 118,// Маленькая фея
    CREATURE_SPRITE              = 119,// Фея

    CREATURE_PSYCHIC_ELEMENTAL   = 120,// Психический элементал(ь)
    CREATURE_MAGIC_ELEMENTAL     = 121,// Магический элементал(ь)

    CREATURE_122                 = 122,// Не используется.
    CREATURE_ICE_ELEMENTAL       = 123,// Элементал(ь) льда
    CREATURE_124                 = 124,// Не используется.
    CREATURE_MAGMA_ELEMENTAL     = 125,// Элементал(ь) магмы
    CREATURE_126                 = 126,// Не используется.
    CREATURE_STORM_ELEMENTAL     = 127,// Элементал(ь) шторма
    CREATURE_128                 = 128,// Не используется.
    CREATURE_ENERGY_ELEMENTAL    = 129,// Элементал(ь) энергии

    CREATURE_FIREBIRD            = 130,// Огненная птица
    CREATURE_PHOENIX             = 131,// Феникс

    // Neutral creatures /
    // Нейтральные существа.

    CREATURE_AZURE_DRAGON        = 132,// Лазурный дракон
    CREATURE_CRYSTAL_DRAGON      = 133,// Кристальный дракон
    CREATURE_FAERIE_DRAGON       = 134,// Сказочный дракон
    CREATURE_RUST_DRAGON         = 135,// Ржавый дракон
    CREATURE_ENCHANTER           = 136,// Колдун
    CREATURE_SHARPSHOOTER        = 137,// Снайпер
    CREATURE_HALFLING            = 138,// Полурослик
    CREATURE_PEASANT             = 139,// Крестьянин
    CREATURE_BOAR                = 140,// Наездник на кабане
    CREATURE_MUMMY               = 141,// Мумия
    CREATURE_NOMAD               = 142,// Кочевник
    CREATURE_ROGUE               = 143,// Разбойник
    CREATURE_TROLL               = 144,// Тролль

    CREATURE_CATAPULT            = 145, // Катапульта (на поле боя)
    CREATURE_BALLISTA            = 146, // Баллиста (на поле боя)
    CREATURE_FIRST_AID_TENT      = 147, // Палатка первой помощи (на поле боя)
    CREATURE_AMMO_CART           = 148, // Тележка с боеприпасами (на поле боя)
    CREATURE_ARROW_TOWER         = 149, // Стрелковая башня (на поле боя)
    CREATURE_NONE                = -1,  // Пустой (используется для проверок)

    CREATURE_ROE_CATAPULT       = 118, // ID Катапульты на поле боя в RoE
    CREATURE_ROE_BALLISTA       = 119, // ID Баллисты на поле боя в RoE
    CREATURE_ROE_FIRST_AID_TENT = 120, // ID Палатки первой помощи на поле боя в RoE
    CREATURE_ROE_AMMO_CART      = 121, // ID Тележки с боеприпасами на поле боя в RoE

    MAX_CREATURES_ROE  = 118, // Количество существ в RoE
    MAX_CREATURES_AB   = 145, // Количество существ в AB
    MAX_CREATURES_SOD  = MAX_CREATURES_AB, // Количество существ в SoD
    MAX_CREATURES      = MAX_CREATURES_SOD, // Количество существ
    MAX_COMBAT_CREATURES_ROE = 122, // Количество существ на поле боя в RoE
    MAX_COMBAT_CREATURES_AB  = 150, // Количество существ на поле боя в AB
    MAX_COMBAT_CREATURES_SOD = MAX_COMBAT_CREATURES_AB, // Количество существ на поле боя в SoD
    MAX_COMBAT_CREATURES     = MAX_COMBAT_CREATURES_SOD // Количество существ на поле боя
};

template<>
struct nh3api::enum_limits<TCreatureType>
    : nh3api::enum_limits_base<TCreatureType, CREATURE_PIKEMAN, CREATURE_TROLL>
{ static inline constexpr bool is_specialized = true; };

// Creature sprite sequences ID during combat /
// Индексы последовательностей кадров анимации существ на поле боя.
enum creature_seqid : int32_t
{
    cs_walk       = 0, //
    cs_fidget     = 1, //
    cs_wait       = 2, //
    cs_wince      = 3, //
    cs_defend     = 4, //
    cs_death      = 5, //
    cs_specdeath  = 6, //
    cs_turn_rf    = 7, //
    cs_turn_fr    = 8, //
    cs_turn_lf    = 9, //
    cs_turn_fl    = 10, //
    cs_attack_ur  = 11, //
    cs_attack_r   = 12, //
    cs_attack_dr  = 13, //
    cs_range_ur   = 14, //
    cs_range_r    = 15, //
    cs_range_dr   = 16, //
    cs_special_ur = 17, //
    cs_special_r  = 18, //
    cs_special_dr = 19, //
    cs_prewalk    = 20, //
    cs_postwalk   = 21, //
    cs_max        = 22  //
};

template<>
struct nh3api::enum_limits<creature_seqid>
    : nh3api::enum_limits_base<creature_seqid, cs_walk, cs_postwalk>
{ static inline constexpr bool is_specialized = true; };

// Creature flags /
// Флаги существа.
NH3API_FLAG_ENUM creature_flags : uint32_t
{
    // Const flag: Occupies two hexes. /
    // Существо занимает две клетки.
    CF_DOUBLE_WIDE           = 0b00000000000000000000000000000001,

    // Const flag: Can fly. /
    // Существо умеет летать.
    CF_FLYING_ARMY           = 0b00000000000000000000000000000010,

    // Const flag: Can shoot. /
    // Существо умеет стрелять.
    CF_SHOOTING_ARMY         = 0b00000000000000000000000000000100,

    // Const flag: has extended attack(more than 1 hex). /
    // Существо умеет атаковать больше чем на одну клетку(напр.,  дыхание дракона).
    CF_HAS_EXTENDED_ATTACK   = 0b00000000000000000000000000001000,

    // Const flag: Alive. /
    // Существо - живое(скелеты, зомби, элементали и другие НЕ живые)
    CF_ALIVE                 = 0b00000000000000000000000000010000,

    // Const flag: Catapult. /
    // Катапульта.
    CF_CATAPULT              = 0b00000000000000000000000000100000,

    // Const flag: Siege Weapon. /
    // Орудие осады города
    CF_SIEGE_WEAPON          = 0b00000000000000000000000001000000,

    // Const flag: Dragons,  Behemoth,  Hydra,  Phoenix /
    // Драконы,  Чудище,  Гидра,  Феникс.
    CF_KING_1                = 0b00000000000000000000000010000000,

    // Const flag: Angel,  Devil /
    // Ангел,  Дьявол.
    CF_KING_2                = 0b00000000000000000000000100000000,

    // Const flag: Titan /
    // Титан.
    CF_KING_3                = 0b00000000000000000000001000000000,

    // Const flag: Immune to Mind Spells(Berserk, Blind, Forgetfulness, Frenzy, Hypnotize, Sorrow). /
    // Иммунитет к заклинаниям разума.(Берсерк, Слепота, Забывчивость, Бешенство, Гипноз, Печаль).
    CF_IMMUNE_TO_MIND_SPELLS = 0b00000000000000000000010000000000,

    // Const flag: Shoots rays. /
    // Стреляет лучом(напр.,  созерцатели и маги).
    CF_SHOOTS_RAY            = 0b00000000000000000000100000000000,

    // Const flag: No melee penalty for a shooter. /
    // Нет штрафа в рукопашной(актуально для стрелков).
    CF_NO_MELEE_PENALTY      = 0b00000000000000000001000000000000,

    // Const flag: Unused. Can be exploited for your own new flag /
    // Неиспользуемый флаг. Может быть использован как ваш новый флаг.
    CF_UNUSED                = 0b00000000000000000010000000000000,

    // Const flag: Immune to fire spells. /
    // Иммунитет к заклинаниям огня.
    CF_IMMUNE_TO_FIRE_SPELLS = 0b00000000000000000100000000000000,

    // Const flag: Double attack. /
    // Двойная атака.
    CF_TWO_ATTACKS           = 0b00000000000000001000000000000000,
    CF_FREE_ATTACK           = 0b00000000000000010000000000000000,

    // Const flag: No morale dependency. /
    // Не зависит от боевого духа.
    CF_NO_MORALE             = 0b00000000000000100000000000000000,

    // Const flag: Undead. /
    // Мертвец
    CF_UNDEAD                = 0b00000000000001000000000000000000,

    // Const flag: Multi Headed. /
    // Несколько голов.
    CF_MULTI_HEADED          = 0b00000000000010000000000000000000,

    // Const flag: Can throw a fireball. /
    // Атака фаерболлом.
    CF_FIREBALL_ATTACK       = 0b00000000000100000000000000000000,

    // Combat flag: Immobilized. /
    // Флаг битвы: Не может двигаться.
    CF_IMMOBILIZED           = 0b00000000001000000000000000000000,

    // Combat flag: Summoned. /
    // Флаг битвы: Вызвано заклинанием.
    CF_SUMMONED              = 0b00000000010000000000000000000000,

    // Combat flag: Clone. /
    // Флаг битвы: Клон.
    CF_CLONE                 = 0b00000000100000000000000000000000,

    // Combat flag: Getting morale. /
    // Флаг битвы: Моралится.
    CF_MORALE                = 0b00000001000000000000000000000000,

    // Combat flag: Waiting. /
    // Флаг битвы: Ждёт.
    CF_WAITING               = 0b00000010000000000000000000000000,

    // Combat flag: Ended turn. /
    // Флаг битвы: Ход завершён.
    CF_DONE                  = 0b00000100000000000000000000000000,

    // Combat flag: Defending. /
    // Флаг битвы: В оборонительной стойке.
    CF_DEFENDING             = 0b00001000000000000000000000000000,

    // Combat flag: Sacrificed. /
    // Флаг битвы: Было жертвой заклинания "Жертва".
    CF_SACRIFICED            = 0b00010000000000000000000000000000,

    // Combat flag: Drawn red. /
    // Флаг битвы: Закрашено красным.
    CF_RED_COLORING          = 0b00100000000000000000000000000000,

    // Combat flag: Drawn grey. /
    // Флаг битвы: Закрашено серым.
    CF_GREY_COLORING         = 0b01000000000000000000000000000000,

    // Combat flag: Dragon. /
    // Флаг битвы: Дракон.
    CF_DRAGON                = 0b10000000000000000000000000000000

};

#pragma pack(push, 4)
// Creature traits /
// Свойства существ.
// size = 0x74 = 116, align = 4
struct TCreatureTypeTraits
{
    // Creature alignment (= -1 for neutral creatures) /
    // Фракция,  к которому принадлежит существо(= -1 для нейтральных существ).
    // offset: +0x0 = +0,  size: 0x4 = 4
    TTownType townType;

    // Creature level (= 0 for creature of level 1,  = 1 for creature of level 2 and so on) /
    // Уровень существа. (= 0 для существа 1 уровня,  = 1 для существа 2 уровня и т.д.).
    // offset: +0x4 = +4,  size: 0x4 = 4
    int32_t level;

    // short creature name. Used to find the corresponding sound file. /
    // Краткое наименование существа(названи(я)е соответсвующего звукового файла).
    // offset: +0x8 = +8,  size: 0x4 = 4
    const char* cSamplePrefix;

    // Creature sprite file name(.def) /
    // Название файла спрайта существа(.def).
    // offset: +0xC = +12,  size: 0x4 = 4
    const char* m_sprite_name;

    // Creature attributes,  stored in 4 bytes. Use army::Is(...) method to check the flag(s). /
    // Флаги существа,  хранимые в 4 байтах. Используйте метод army::Is(...) чтобы проверить флаг(и).
    // offset: +0x10 = +16,  size: 0x4 = 4
    creature_flags flags;

    // Creature's singular name /
    // Имя существа в единиственном числе.
    // offset: +0x14 = +20,  size: 0x4 = 4
    const char* m_name;

    // Creatures' plural name /
    // Имя существ в множественном числе.
    // offset: +0x18 = +24,  size: 0x4 = 4
    const char* m_plural_name;

    // Special ability description text /
    // Описание уникальной способности существа.
    // offset: +0x1C = +28,  size: 0x4 = 4
    const char* special_ability;

    // Creature cost  /
    // Стоимость покупки существа.
    // offset: +0x20 = +32,  size: 0x1C = 28
    std::array<int32_t, 7> cost;

    // fight_value.
    // offset: +0x3C = +60,  size: 0x4 = 4
    int32_t baseFightValue;

    // AI_value.
    // offset: +0x40 = +64,  size: 0x4 = 4
    int32_t AI_value;

    // Base creature growth in town per week /
    // Начальный прирост существа в городе за каждую неделю.
    // offset: +0x44 = +68,  size: 0x4 = 4
    int32_t growthRate;

    // Horde growth bonus per week /
    // Бонус к приросту от орды за неделю.
    // offset: +0x48 = +72,  size: 0x4 = 4
    int32_t horde_growth_rate;

    // Creature base hit points /
    // Начальные очки здоровья существа.
    // offset: +0x4C = +76,  size: 0x4 = 4
    int32_t hitPoints;

    // Creature base speed /
    // Начальные очки скорости существа.
    // offset: +0x50 = +80,  size: 0x4 = 4
    int32_t speed;

    // Creature base attack skill /
    // Начальные очки атаки существа.
    // offset: +0x54 = +84,  size: 0x4 = 4
    int32_t attackSkill;

    // Creature base defense skill /
    // Начальные очки защиты существа.
    // offset: +0x58 = +88,  size: 0x4 = 4
    int32_t defenseSkill;

    // Creature base damage low bound /
    // Начальная нижняя граница разброса урона существа.
    // offset: +0x5C = +92,  size: 0x4 = 4
    int32_t damageLowBound;

    // Creature base damage high bound /
    // Начальная верхняя граница разброса урона существа.
    // offset: +0x60 = +96,  size: 0x4 = 4
    int32_t damageHighBound;

    // Shooter maximum number of shots /
    // Количество выстрелов у стрелка.
    // offset: +0x64 = +100,  size: 0x4 = 4
    int32_t numShots;

    // Has spell? /
    // Есть заклинание?
    // offset: +0x68 = +104,  size: 0x4 = 4
    int32_t hasSpell;

    // Wandering creature random amount low bound /
    // Нижняя граница из случайного диапазона количества существа на карте.
    // offset: +0x6C = +108,  size: 0x4 = 4
    int32_t wanderingLow;

    // Wandering creature random amount high bound /
    // Верхняя граница из случайного диапазона количества существа на карте.
    // offset: +0x70 = +112,  size: 0x4 = 4
    int32_t wanderingHigh;

};

// Creature frame info /
// Информация об анимации существа.
// size = 0x54 = 84, align = 4
struct SMonFrameInfo
{
    // offset: +0x0 = +0,  size = 0xC = 12
    std::array<int16_t, 6> iMissileOffset;

    // offset: +0xC = +12,  size = 0x30 = 48
    std::array<float, 12>  fArrowAngle;

    // offset: +0x3C = +60,  size = 0x4 = 4
    int32_t iExtraNumTroopsXOffset;

    // offset: +0x40 = +64,  size = 0x4 = 4
    int32_t iAttackFrames;

    // offset: +0x44 = +68,  size = 0x4 = 4
    int32_t iFidgetFrequency;

    // offset: +0x48 = +72,  size = 0x4 = 4
    int32_t iWalkCycleTime;

    // offset: +0x4C = +76,  size = 0x4 = 4
    int32_t iAttackStartCycleTime;

    // offset: +0x50 = +80,  size = 0x4 = 4
    int32_t iFlightPixelSpan;

};

// All creature traits /
// Свойства всех существ.
inline std::array<TCreatureTypeTraits, MAX_COMBAT_CREATURES>& akCreatureTypeTraits
= get_global_var_ref(0x6703B8, std::array<TCreatureTypeTraits, MAX_COMBAT_CREATURES>);

// Creature types in each town dwelling gDwellingType[towntype][levels][upgraded] /
// Существа в жилищах каждого типа городов gDwellingType[город][уровни][улучшенное]
inline std::array<std::array<std::array<TCreatureType, 7>, 2>, kNumTowns>& gDwellingType
= get_global_var_ref(0x6747B4, std::array<std::array<std::array<TCreatureType, 7>, 2>, kNumTowns>);

class town;
class hero;

// Army array /
// Массив армии
// size = 0x38 = 56, align = 4
class armyGroup
{
    public:
        // Distribute <amount> creature of <type> /
        // Распределить количество существ <amount> типа <type>.
        inline armyGroup(TCreatureType type_, int32_t amount_) noexcept
            : armyGroup(nh3api::dummy_tag)
        { THISCALL_3(void, 0x44A770, this, type_, amount_); }

        inline armyGroup(const nh3api::dummy_tag_t&) noexcept
        {}

        inline constexpr armyGroup() noexcept = default;
        inline ~armyGroup() noexcept = default;
        inline constexpr armyGroup(const armyGroup& other) noexcept = default;
        inline constexpr armyGroup& operator=(const armyGroup& other) noexcept = default;
        inline constexpr armyGroup(armyGroup&& other) noexcept = default;
        inline constexpr armyGroup& operator=(armyGroup&& other) noexcept = default;

    #ifdef __cpp_impl_three_way_comparison
        inline constexpr bool operator==(const armyGroup& other) const noexcept = default;
        inline constexpr bool operator!=(const armyGroup& other) const noexcept = default;
    #else
        inline constexpr bool operator==(const armyGroup& other) const noexcept
        { return __builtin_memcmp(this, &other, sizeof(*this)) == 0;}

        inline constexpr bool operator!=(const armyGroup& other) const noexcept
        { return !(*this == other); }
    #endif

    public:
        // Has creatures /
        // В армии есть существа.
        [[nodiscard]] bool HasCreatures() const
        { return THISCALL_1(bool, 0x449370, this); }

        // Has all armies undead /
        // Все существа армии - нежить.
        [[nodiscard]] bool HasAllUndead() const
        { return THISCALL_1(bool, 0x44A7F0, this ); }

        // Dismiss creature at <whichIndex> /
        // Удалить существо на позиции <whichIndex>
        constexpr void Dismiss(size_t whichIndex)
        {
            if ( whichIndex < 7 )
            {
                type[whichIndex]   = CREATURE_NONE;
                amount[whichIndex] = 0;
            }
        }

        // Has creature of type <monType>  /
        // В армии есть существо типа <monType>.
        [[nodiscard]] bool IsMember(TCreatureType monType) const
        { return THISCALL_2(bool, 0x44A850, this, monType); }

        // Can creature of type <monType> join army? /
        // Может ли существо типа <monType> присоединится?
        [[nodiscard]] bool CanJoin(TCreatureType monType) const
        { return THISCALL_2(bool32_t, 0x44A920, this, monType); }

        // Sum AI_value of each creature /
        // Посчитать сумму AI_value всей армии.
        [[nodiscard]] int32_t get_AI_value() const
        { return THISCALL_1(int32_t, 0x44A950, this); }

        // Sum amount of held positions /
        // Посчитать количество непустых ячеек армии
        [[nodiscard]] int32_t GetNumArmies() const
        { return THISCALL_1(int32_t, 0x44A990, this); }

        // Add creature of type <armyType> and amount <newNumTroops> to <newIndex> /
        // Добавить существо типа <armyType> количества <newNumTroops> в ячейку <newIndex>
        /// @param armyType Тип существа
        /// @param newNumTroops Количество существ
        /// @param newIndex Индекс добавления
        bool Add(TCreatureType armyType, int32_t newNumTroops, int32_t newIndex)
        { return THISCALL_4(bool32_t, 0x44A9B0, this, armyType, newNumTroops, newIndex); }

        // Swap two armies stacks. Use std::swap to swap whole two armies /
        // Обмен ячеек двух армий. Используйте std::swap чтобы полностью обменять ячейки двух армией.
        /// @param srcIndex индекс армии
        /// @param destGroup армия, с которой осуществляется обмен
        /// @param destIndex индекс армии обмена
        void Swap(int32_t srcIndex, armyGroup* destGroup, int32_t destIndex)
        { THISCALL_4(void, 0x44AA30, this, srcIndex, destGroup, destIndex); }

        // Sum each creature amount /
        // Посчитать сумму количества существ армии.
        [[nodiscard]] uint32_t get_creature_total() const
        { return static_cast<uint32_t>(THISCALL_1(int32_t,  0x44AA70, this)); }

        // Sum amount of creatures of type <monType> /
        // Посчитать количество существ типа <monType> в армии.
        [[nodiscard]] constexpr uint32_t get_creature_total(TCreatureType monType) const
        {
            uint32_t result = 0;
            for ( size_t i = 0; i < 7; ++i )
                if ( type[i] == monType && amount[i] > 0 )
                    result += static_cast<uint32_t>(static_cast<uint16_t>(amount[i]));

            return result;
        }

        // static. Get army size name depending on <howMany> /
        // Не нужен указатель this. Получить название количества существ в армии.
        /// @param howMany количество существ
        /// @param iNameSet 0 or 2 is upper limit, 1 is interval / 0 или 2 это верхнее ограничение, 1 это интервал
        [[nodiscard]] static char const* GetArmySizeName(int32_t howMany, int32_t iNameSet)
        { return FASTCALL_2(const char*, 0x44AAB0, howMany, iNameSet); }

        // Get army morale /
        // Боевой дух всей армии.
        /// @param ownerHero обладатель этой армии
        /// @param ownerTown город героя-обладателя
        /// @param otherHero unused / не используется
        /// @param otherGroup unused / не используется
        /// @param on_cursed_ground на проклятой земле
        /// @param apply_limits применить лимит [-3;3]
        /// @param angelic_alliance ангельский союз
        [[nodiscard]] int32_t GetMorale(const hero*      ownerHero,
                                        const town*      ownerTown,
                                        const hero*      otherHero,
                                        const armyGroup* otherGroup,
                                        bool             on_cursed_ground,
                                        bool             angelic_alliance,
                                        bool             apply_limits = true) const
        { return THISCALL_8(int32_t, 0x44AB30, this,
                                            ownerHero,
                                            ownerTown,
                                            otherHero,
                                            otherGroup,
                                            on_cursed_ground,
                                            angelic_alliance,
                                            apply_limits); }

        [[nodiscard]]
        // Get <index> morale /
        // Боевой дух ячейки <index> в армии
        /// @param index индекс отдельной ячейки армии
        /// @param ownerHero обладатель этой армии
        /// @param ownerTown город героя-обладателя
        /// @param ground_type тип накладной земли
        /// @param angelic_alliance Ангельский альянс
        /// @param apply_limits применить лимит [-3;3]
        int32_t GetArmyMorale(int32_t     index,
                              const hero* ownerHero,
                              const town* ownerTown,
                              int32_t     ground_type,
                              bool        angelic_alliance,
                              bool        apply_limits = true) const
        { return THISCALL_7(int32_t, 0x44ADD0, this, index, ownerHero, ownerTown, ground_type, angelic_alliance, apply_limits); }

        [[nodiscard]]
        // Get army luck /
        // Удача всей армии.
        /// @param ownerHero обладатель этой армии
        /// @param ownerTown город героя-обладателя
        /// @param otherHero unused / не используется
        /// @param otherGroup unused / не используется
        /// @param on_cursed_ground на проклятой земле
        /// @param apply_limits применить лимит [-3;3]
        int32_t GetLuck(const hero*      ownerHero,
                        const town*      ownerTown,
                        const hero*      otherHero,
                        const armyGroup* otherGroup,
                        bool             on_cursed_ground = false,
                        bool             apply_limits     = false) const
        { return THISCALL_7(int32_t, 0x44AFA0, this, ownerHero, ownerTown, otherHero, otherGroup, on_cursed_ground, apply_limits); }

        [[nodiscard]]
        // Get <index> luck /
        // Удача ячейки <index> в армии
        /// @param index индекс отдельной ячейки армии
        /// @param ownerHero обладатель этой армии
        /// @param ownerTown город героя-обладателя
        /// @param ground_type тип накладной магической земли
        /// @param apply_limits применить лимит [-3;3]
        int32_t GetArmyLuck(int32_t       index,
                            const hero*   ownerHero,
                            const town*   ownerTown,
                            EMagicTerrain ground_type,
                            bool          apply_limits = true) const
        { return THISCALL_6(int32_t, 0x44B090, this, index, ownerHero, ownerTown, ground_type, apply_limits); }

        bool Merge(armyGroup* ag)
        { return THISCALL_2(bool, 0x44B2F0, this, ag); }

        void merge_armies(armyGroup& source)
        { THISCALL_2(void, 0x44B4F0, this, &source); }

        [[nodiscard]] inline
        exe_string get_morale_description(TCreatureType    creature,
                                          int32_t          morale,
                                          const hero*      ownerHero,
                                          const town*      ownerTown,
                                          const hero*      other_hero,
                                          const armyGroup* other_group,
                                          EMagicTerrain    ground_type) const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_9(exe_string*, 0x44B630, this, &result, creature, morale, ownerHero, ownerTown, other_hero, other_group, ground_type);
            return result;
        }

        [[nodiscard]] inline
        exe_string get_luck_description(TCreatureType army_type,
                                        int32_t       luck,
                                        hero*         our_hero,
                                        town*         our_town,
                                        hero*         enemy_hero,
                                        bool          on_cursed_ground,
                                        EMagicTerrain magic_terrain) const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_9(exe_string*, 0x44BE90, this, &result, army_type, luck, our_hero, our_town, enemy_hero, on_cursed_ground, magic_terrain);
            return result;
        }

        // Get first available unit of army native terrain /
        // Родная земля первой доступной ячейки армии(именно поэтому, например, кочевников нужно ставить в самый левый слот).
        [[nodiscard]] TTerrainType GetNativeTerrain() const
        { return THISCALL_1(TTerrainType, 0x44C260, this); }

    // utility functions
    public:
        [[nodiscard]] inline constexpr auto to_pair() const noexcept
        { return std::pair { type, amount }; }

    public:
        // Creature type /
        // Тип существа в ячейке.
        // offset: +0x0 = +0,  size: 0x1C = 28
        std::array<TCreatureType, 7> type
        {CREATURE_NONE, CREATURE_NONE, CREATURE_NONE, CREATURE_NONE, CREATURE_NONE, CREATURE_NONE, CREATURE_NONE};

        // Creature amount /
        // Количество существ в ячейке.
        // offset: +0x1C = +28,  size: 0x1C = 28
        std::array<int32_t, 7> amount
        {-1, -1, -1, -1, -1, -1, -1};

};
#pragma pack(pop) // 4

inline void get_upgrade_cost(TCreatureType creature, int32_t amount, std::array<int32_t, 7>& cost)
{ FASTCALL_3(void, 0x54EB50, creature, amount, cost.data()); }

inline void GetMonsterCost(int32_t monId, std::array<int32_t, 7>& resCost)
{ FASTCALL_2(void, 0x54EBC0, monId, resCost.data()); }

NH3API_WARNING(pop)
