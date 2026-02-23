//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "spells.hpp"                // SpellID

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// Artifact type /
// Тип артефакта.
enum TArtifact : int32_t
{
    ARTIFACT_NONE                           = -1,  // Никакой(используется для проверок)
    ARTIFACT_SPELLBOOK                      = 0,   // Книга заклинаний
    ARTIFACT_SPELL_SCROLL                   = 1,   // Свиток заклинания
    ARTIFACT_GRAIL                          = 0,   // Грааль
    ARTIFACT_CATAPULT                       = 3,   // Катапульта(артефакт)
    ARTIFACT_BALLISTA                       = 4,   // Баллиста(артефакт)
    ARTIFACT_AMMO_CART                      = 5,   // Тележка с боеприпасами(артефакт)
    ARTIFACT_FIRST_AID_TENT                 = 6,   // Палатка первой помощи(артефакт)
    ARTIFACT_CENTAUR_AXE                    = 7,   // Секира кентавра
    ARTIFACT_BLACKSHARD_OF_THE_DEAD_KNIGHT  = 8,   // Блэкшард мёртвого рыцаря
    ARTIFACT_GREATER_GNOLLS_FLAIL           = 9,   // Кистень великого гнолла
    ARTIFACT_OGRES_CLUB_OF_HAVOC            = 10,  // Карающая дубина огра
    ARTIFACT_SWORD_OF_HELLFIRE              = 11,  // Меч адского пламени
    ARTIFACT_TITANS_GLADIUS                 = 12,  // Гладиус титана
    ARTIFACT_SHIELD_OF_THE_DWARVEN_LORDS    = 13,  // Щит короля гномов
    ARTIFACT_SHIELD_OF_THE_YAWNING_DEAD     = 14,  // Щит тоскующих мертвецов
    ARTIFACT_BUCKLER_OF_THE_GNOLL_KING      = 15,  // Щит короля гноллов
    ARTIFACT_TARG_OF_THE_RAMPAGING_OGRE     = 16,  // Щит яростного огра
    ARTIFACT_SHIELD_OF_THE_DAMNED           = 17,  // Щит проклятых
    ARTIFACT_SENTINELS_SHIELD               = 18,  // Щит часового
    ARTIFACT_HELM_OF_THE_ALABASTER_UNICORN  = 19,  // Шлем белого единорога
    ARTIFACT_SKULL_HELMET                   = 20,  // Шлем-череп
    ARTIFACT_HELM_OF_CHAOS                  = 21,  // Шлем Хаоса
    ARTIFACT_CROWN_OF_THE_SUPREME_MAGI      = 22,  // Корона верховного мага
    ARTIFACT_HELLSTORM_HELMET               = 23,  // Шлем сатанинской ярости
    ARTIFACT_THUNDER_HELMET                 = 24,  // Шлем небесного грома
    ARTIFACT_BREASTPLATE_OF_PETRIFIED_WOOD  = 25,  // Нагрудник из окаменелого дерева
    ARTIFACT_RIB_CAGE                       = 26,  // Рёбра
    ARTIFACT_SCALES_OF_THE_GREATER_BASILISK = 27,  // Кольчуга великого василиска
    ARTIFACT_TUNIC_OF_THE_CYCLOPS_KING      = 28,  // Туника короля циклопов
    ARTIFACT_BREASTPLATE_OF_BRIMSTONE       = 29,  // Доспехи самородной серы
    ARTIFACT_TITANS_CUIRASS                 = 30,  // Кираса титана
    ARTIFACT_ARMOR_OF_WONDER                = 31,  // Магические доспехи
    ARTIFACT_SANDALS_OF_THE_SAINT           = 32,  // Сандалии святого
    ARTIFACT_CELESTIAL_NECKLACE_OF_BLISS    = 33,  // Ожерелье Божественной благодати
    ARTIFACT_LIONS_SHIELD_OF_COURAGE        = 34,  // Щит львиной храбрости
    ARTIFACT_SWORD_OF_JUDGEMENT             = 35,  // Меч правосудия
    ARTIFACT_HELM_OF_HEAVENLY_ENLIGHTENMENT = 36,  // Шлем небесного просветления
    ARTIFACT_QUIET_EYE_OF_THE_DRAGON        = 37,  // Неподвижный глаз дракона
    ARTIFACT_RED_DRAGON_FLAME_TONGUE        = 38,  // Языки пламени красного дракона
    ARTIFACT_DRAGON_SCALE_SHIELD            = 39,  // Щит из чешуи дракона
    ARTIFACT_DRAGON_SCALE_ARMOR             = 40,  // Доспех из чешуи дракона
    ARTIFACT_DRAGONBONE_GREAVES             = 41,  // Поножи из кости дракона
    ARTIFACT_DRAGON_WING_TABARD             = 42,  // Плащ из крыльев дракона
    ARTIFACT_NECKLACE_OF_DRAGONTEETH        = 43,  // Ожерелье из зубов дракона
    ARTIFACT_CROWN_OF_DRAGONTOOTH           = 44,  // Корона из зубов дракона
    ARTIFACT_STILL_EYE_OF_THE_DRAGON        = 45,  // Застывший глаз дракона
    ARTIFACT_CLOVER_OF_FORTUNE              = 46,  // Клевер удачи
    ARTIFACT_CARDS_OF_PROPHECY              = 47,  // Карты пророчества
    ARTIFACT_LADYBIRD_OF_LUCK               = 48,  // Голубка удачи
    ARTIFACT_BADGE_OF_COURAGE               = 49,  // Медаль мужества
    ARTIFACT_CREST_OF_VALOR                 = 50,  // Знак отваги
    ARTIFACT_GLYPH_OF_GALLANTRY             = 51,  // Глиф доблести
    ARTIFACT_SPECULUM                       = 52,  // Телескоп
    ARTIFACT_SPYGLASS                       = 53,  // Подзорная труба
    ARTIFACT_AMULET_OF_THE_UNDERTAKER       = 54,  // Амулет гробовщика
    ARTIFACT_VAMPIRES_COWL                  = 55,  // Мантия вампира
    ARTIFACT_DEAD_MANS_BOOTS                = 56,  // Башмаки мертвеца
    ARTIFACT_GARNITURE_OF_INTERFERENCE      = 57,  // Колье неприступности
    ARTIFACT_SURCOAT_OF_COUNTERPOISE        = 58,  // Туника равновесия
    ARTIFACT_BOOTS_OF_POLARITY              = 59,  // Башмаки полярности
    ARTIFACT_BOW_OF_ELVEN_CHERRYWOOD        = 60,  // Лук из вишнёвого дерева эльфов
    ARTIFACT_BOWSTRING_OF_THE_UNICORNS_MANE = 61,  // Тетива из гривы единорога
    ARTIFACT_ANGEL_FEATHER_ARROWS           = 62,  // Стрелы из ангельских перьев
    ARTIFACT_BIRD_OF_PERCEPTION             = 63,  // Птица проницательности
    ARTIFACT_STOIC_WATCHMAN                 = 64,  // Стойкий часовой
    ARTIFACT_EMBLEM_OF_COGNIZANCE           = 65,  // Символ знания
    ARTIFACT_STATESMANS_MEDAL               = 66,  // Медаль чиновника
    ARTIFACT_DIPLOMATS_RING                 = 67,  // Кольцо дипломата
    ARTIFACT_AMBASSADORS_SASH               = 68,  // Лента посла
    ARTIFACT_RING_OF_THE_WAYFARER           = 69,  // Кольцо странника
    ARTIFACT_EQUESTRIANS_GLOVES             = 70,  // Перчатки всадника
    ARTIFACT_NECKLACE_OF_OCEAN_GUIDANCE     = 71,  // Ожерелье морского провидения
    ARTIFACT_ANGEL_WINGS                    = 72,  // Крылья ангела
    ARTIFACT_CHARM_OF_MANA                  = 73,  // Амулет маны
    ARTIFACT_TALISMAN_OF_MANA               = 74,  // Талисман маны
    ARTIFACT_MYSTIC_ORB_OF_MANA             = 75,  // Волшебная сфера маны
    ARTIFACT_COLLAR_OF_CONJURING            = 76,  // Магический ошейник
    ARTIFACT_RING_OF_CONJURING              = 77,  // Магическое кольцо
    ARTIFACT_CAPE_OF_CONJURING              = 78,  // Магическая накидка
    ARTIFACT_ORB_OF_THE_FIRMAMENT           = 79,  // Сфера небесного свода
    ARTIFACT_ORB_OF_SILT                    = 80,  // Сфера илистого озера
    ARTIFACT_ORB_OF_TEMPESTUOUS_FIRE        = 81,  // Сфера бушующего огня
    ARTIFACT_ORB_OF_DRIVING_RAIN            = 82,  // Сфера проливного дождя
    ARTIFACT_RECANTERS_CLOAK                = 83,  // Плащ отречения
    ARTIFACT_SPIRIT_OF_OPPRESSION           = 84,  // Дух уныния
    ARTIFACT_HOURGLASS_OF_THE_EVIL_HOUR     = 85,  // Песочные часы недоброго часа
    ARTIFACT_TOME_OF_FIRE_MAGIC             = 86,  // Книга магии огня
    ARTIFACT_TOME_OF_AIR_MAGIC              = 87,  // Книга магии воздуха
    ARTIFACT_TOME_OF_WATER_MAGIC            = 88,  // Книга магии воды
    ARTIFACT_TOME_OF_EARTH_MAGIC            = 89,  // Книга магии земли
    ARTIFACT_BOOTS_OF_LEVITATION            = 90,  // Сапоги левитации
    ARTIFACT_GOLDEN_BOW                     = 91,  // Золотой лук
    ARTIFACT_SPHERE_OF_PERMANENCE           = 92,  // Сфера постоянства
    ARTIFACT_ORB_OF_VULNERABILITY           = 93,  // Сфера уязвимости
    ARTIFACT_RING_OF_VITALITY               = 94,  // Кольцо жизненной силы
    ARTIFACT_RING_OF_LIFE                   = 95,  // Кольцо жизни
    ARTIFACT_VIAL_OF_LIFEBLOOD              = 96,  // Склянка жизненной силы
    ARTIFACT_NECKLACE_OF_SWIFTNESS          = 97,  // Ожерелье стремительности
    ARTIFACT_BOOTS_OF_SPEED                 = 98,  // Сапоги-скороходы
    ARTIFACT_CAPE_OF_VELOCITY               = 99,  // Накидка скорости
    ARTIFACT_PENDANT_OF_DISPASSION          = 100, // Амулет бесстрастия
    ARTIFACT_PENDANT_OF_SECOND_SIGHT        = 101, // Брелок ясновидения
    ARTIFACT_PENDANT_OF_HOLINESS            = 102, // Священный брелок
    ARTIFACT_PENDANT_OF_LIFE                = 103, // Брелок жизни
    ARTIFACT_PENDANT_OF_DEATH               = 104, // Кулон смерти
    ARTIFACT_PENDANT_OF_FREE_WILL           = 105, // Брелок свободной воли
    ARTIFACT_PENDANT_OF_NEGATIVITY          = 106, // Кулон отрицательности
    ARTIFACT_PENDANT_OF_TOTAL_RECALL        = 107, // Брелок абсолютной памяти
    ARTIFACT_PENDANT_OF_COURAGE             = 108, // Брелок смелости
    ARTIFACT_EVERFLOWING_CRYSTAL_CLOAK      = 109, // Плащ бесконечных кристаллов
    ARTIFACT_RING_OF_INFINITE_GEMS          = 110, // Кольцо драгоценных камней
    ARTIFACT_EVERPOURING_VIAL_OF_MERCURY    = 111, // Неиссякаемая склянка ртути
    ARTIFACT_INEXHAUSTIBLE_CART_OF_ORE      = 112, // Неистощимая подвода с рудой
    ARTIFACT_EVERSMOKING_RING_OF_SULFUR     = 113, // Вечное кольцо серы
    ARTIFACT_INEXHAUSTIBLE_CART_OF_LUMBER   = 114, // Неиссякаемая подвода древесины
    ARTIFACT_ENDLESS_SACK_OF_GOLD           = 115, // Неиссякаемый мешок золота
    ARTIFACT_ENDLESS_BAG_OF_GOLD            = 116, // Неиссякаемая сума золота
    ARTIFACT_ENDLESS_PURSE_OF_GOLD          = 117, // Неиссякаемая мошна золота
    ARTIFACT_LEGS_OF_LEGION                 = 118, // Ноги Легионера
    ARTIFACT_LOINS_OF_LEGION                = 119, // Поясница Легионера
    ARTIFACT_TORSO_OF_LEGION                = 120, // Торс Легионера
    ARTIFACT_ARMS_OF_LEGION                 = 121, // Руки Легионера
    ARTIFACT_HEAD_OF_LEGION                 = 122, // Голова Легионера
    ARTIFACT_SEA_CAPTAINS_HAT               = 123, // Шляпа морского капитана
    ARTIFACT_SPELLBINDERS_HAT               = 124, // Шляпа волшебника
    ARTIFACT_SHACKLES_OF_WAR                = 125, // Оковы войны
    ARTIFACT_ORB_OF_INHIBITION              = 126, // Сфера запрещения

    ARTIFACT_VIAL_OF_DRAGON_BLOOD           = 127, // Пузырёк с кровью дракона
    ARTIFACT_ARMAGEDDONS_BLADE              = 128, // Клинок Армагеддона

    ARTIFACT_ANGELIC_ALLIANCE               = 129, // Альянс Ангелов
    ARTIFACT_CLOAK_OF_THE_UNDEAD_KING       = 130, // Плащ короля нежити
    ARTIFACT_ELIXIR_OF_LIFE                 = 131, // Эликсир жизни
    ARTIFACT_ARMOR_OF_THE_DAMNED            = 132, // Доспехи проклятого
    ARTIFACT_STATUE_OF_LEGION               = 133, // Статуя Легионера
    ARTIFACT_POWER_OF_THE_DRAGON_FATHER     = 134, // Мощь Отца драконов
    ARTIFACT_TITANS_THUNDER                 = 135, // Гром титана
    ARTIFACT_ADMIRALS_HAT                   = 136, // Шляпа адмирала
    ARTIFACT_BOW_OF_THE_SHARPSHOOTER        = 137, // Лук снайпера
    ARTIFACT_WIZARDS_WELL                   = 138, // Колодец волшебника
    ARTIFACT_RING_OF_THE_MAGI               = 139, // Кольцо мага
    ARTIFACT_CORNUCOPIA                     = 140, // Рог изобилия
    ARTIFACT_DIPLOMATS_SUIT                 = 141, // Unused / Не используется
    ARTIFACT_MIRED_IN_NEUTRALITY            = 142, // Unused / Не используется
    ARTIFACT_IRONFIST_OF_THE_OGRE           = 143, // Unused / Не используется

    MAX_ARTIFACTS_ROE   = 127,                     // Кол-во артефактов в RoE
    MAX_ARTIFACTS_AB    = 129,                     // Кол-во артефактов в AB
    MAX_ARTIFACTS_SOD   = 144,                     // Кол-во артефактов в SoD
    MAX_ARTIFACTS       = MAX_ARTIFACTS_SOD,       // Кол-во артефактов
    FIRST_ARTIFACT      = ARTIFACT_SPELLBOOK,
    LAST_ARTIFACT       = ARTIFACT_IRONFIST_OF_THE_OGRE
};

template<>
struct nh3api::enum_limits<TArtifact>
    : nh3api::enum_limits_base<TArtifact, ARTIFACT_SPELLBOOK, ARTIFACT_IRONFIST_OF_THE_OGRE>
{ static inline constexpr bool is_specialized = true; };

// Hero artifact slots /
// Слоты артефактов на теле героя
enum TArtifactSlot : int32_t
{
    SLOT_NONE           = -1,
    SLOT_HEAD           = 0,                         // Голова
    SLOT_SHOULDERS      = 1,                         // Плечи
    SLOT_NECK           = 2,                         // Шея
    SLOT_RIGHT_HAND     = 3,                         // Правая рука
    SLOT_LEFT_HAND      = 4,                         // Левая рука
    SLOT_TORSO          = 5,                         // Торс
    SLOT_RIGHT_RING     = 6,                         // Правое кольцо
    SLOT_LEFT_RING      = 7,                         // Левое кольцо
    SLOT_FEET           = 8,                         // Ноги
    SLOT_MISC1          = 9,                         // Разное 1
    SLOT_MISC2          = 10,                        // Разное 2
    SLOT_MISC3          = 11,                        // Разное 3
    SLOT_MISC4          = 12,                        // Разное 4
    SLOT_BALLISTA       = 13,                        // Место Баллисты
    SLOT_AMMO_CART      = 14,                        // Место Подводы с боеприпасами
    SLOT_FIRST_AID_TENT = 15,                        // Место Палатки
    SLOT_CATAPULT       = 16,                        // Место Катапульты
    SLOT_SPELLBOOK      = 17,                        // Место Книги заклинаний
    SLOT_MISC5          = 18,                        // Разное 5

    MAX_ARTIFACT_SLOTS_ROE = 18,                     // Кол-во слотов артефактов в RoE
    MAX_ARTIFACT_SLOTS_AB  = 18,                     // Кол-во слотов артефактов в AB
    MAX_ARTIFACT_SLOTS_SOD = 19,                     // Кол-во слотов артефактов в SoD
    MAX_ARTIFACT_SLOTS     = MAX_ARTIFACT_SLOTS_SOD, // Кол-во слотов артефактов
    MAX_BACKPACK_SLOTS     = 64,                     // Кол-во слотов рюкзака
    FIRST_ARTIFACT_SLOT    = SLOT_HEAD,
    LAST_ARTIFACT_SLOT     = SLOT_MISC5
};

template<>
struct nh3api::enum_limits<TArtifactSlot>
    : nh3api::enum_limits_base<TArtifactSlot, FIRST_ARTIFACT_SLOT, LAST_ARTIFACT_SLOT>
{ static inline constexpr bool is_specialized = true; };

// Artifact class /
// Класс артефакта.
enum ArtifactClass : uint32_t
{
    ARTIFACT_CLASS_ANY      = 0b11111, // Класс по умолчанию
    ARTIFACT_CLASS_NONE     = 0b00000, // Нет класса
    ARTIFACT_CLASS_TREASURE = 0b00010, // Сокровище
    ARTIFACT_CLASS_MINOR    = 0b00100, // Малый
    ARTIFACT_CLASS_MAJOR    = 0b01000, // Великий
    ARTIFACT_CLASS_RELIC    = 0b10000  // Реликвия
};

#pragma pack(push, 4)
// Artifact /
// Артефакт.
// size = 0x8 = 8, align = 4
struct type_artifact
{
    public:
        inline constexpr type_artifact() noexcept = default;

        inline constexpr type_artifact(TArtifact _type, SpellID _spell) noexcept
            : type { _type }, spell { _spell }
        {}

        inline constexpr type_artifact(TArtifact _type, int32_t _spell) noexcept
            : type { _type }, spell { static_cast<SpellID>(_spell) }
        {}

        inline constexpr type_artifact(int32_t _type, int32_t _spell) noexcept
            : type { static_cast<TArtifact>(_type) }, spell { static_cast<SpellID>(_spell) }
        {}

        inline type_artifact(const nh3api::dummy_tag_t&) noexcept
        {}

        inline constexpr type_artifact(const type_artifact&) noexcept            = default;
        inline constexpr type_artifact(type_artifact&&) noexcept                 = default;
        inline constexpr type_artifact& operator=(const type_artifact&) noexcept = default;
        inline constexpr type_artifact& operator=(type_artifact&&) noexcept      = default;

        inline constexpr bool operator==(const type_artifact& other) const noexcept
        { return this->type == other.type && this->spell == other.spell; }

        #ifndef __cpp_impl_three_way_comparison
        inline constexpr bool operator!=(const type_artifact& other) const noexcept
        { return !(*this == other); }
        #endif

        inline ~type_artifact() noexcept = default;

    public:
        inline void get_rollover_text(char* buffer) const
        { THISCALL_2(void, 0x4DB5C0, this, buffer); }

        [[nodiscard]] inline exe_string get_description() const
        {
            exe_string result(nh3api::dummy_tag);
            THISCALL_2(exe_string*, 0x4DB650, this, &result);
            return result;
        }

    public:
        [[nodiscard]] inline constexpr std::pair<TArtifact, SpellID> to_pair() const noexcept
        { return { type, spell }; }

        [[nodiscard]] inline constexpr int64_t to_int64() const noexcept
        { return static_cast<int64_t>((static_cast<uint64_t>(type) << 32U) | static_cast<uint32_t>(spell)); }

        [[nodiscard]] inline constexpr uint64_t to_uint64() const noexcept
        { return (static_cast<uint64_t>(type) << 32U) | static_cast<uint32_t>(spell); }

    public:
        // Artifact type /
        // Тип артефакта.
        // offset: +0x0 = +0,  size = 0x4 = 4
        TArtifact type {ARTIFACT_NONE};

        // Spell scroll spell /
        // Заклинание свитка заклинаний.
        // offset: +0x4 = +4,  size = 0x4 = 4
        SpellID spell {SPELL_NONE};
};

// Combination artifact /
// Сборный артефакт /
// size = 0x18 = 24, align = 4
struct CombinationArtifact
{
    // Artifact type /
    // Тип артефакта.
    // offset: +0x0 = +0,  size = 0x4 = 4
    TArtifact       type;

    // Artifacts required to assemble this artifact /
    // Артефакты, необходимые для того, чтобы собрать этот артефакт.
    // offset: +0x4 = +4,  size = 0x14 = 20
    exe_bitset<144> requirements;
};

// Combination artifact type /
// Тип собираемого артефакта.
enum CombinationArtifactType : int32_t
{
    COMBO_NONE                       = -1,
    COMBO_ANGELIC_ALLIANCE           = 0,  // Альянс Ангелов
    COMBO_CLOAK_OF_THE_UNDEAD_KING   = 1,  // Плащ Короля Нежити
    COMBO_ELIXIR_OF_LIFE             = 2,  // Эликсир Жизни
    COMBO_ARMOR_OF_THE_DAMNED        = 3,  // Доспехи Проклятого
    COMBO_STATUE_OF_LEGION           = 4,  // Статуя Легиона
    COMBO_POWER_OF_THE_DRAGON_FATHER = 5,  // Мощь Отца Драконов
    COMBO_TITANS_THUNDER             = 6,  // Грохот Титана
    COMBO_ADMIRALS_HAT               = 7,  // Шляпа Адмирала
    COMBO_BOW_OF_THE_SHARPSHOOTER    = 8,  // Лук Снайпера
    COMBO_WIZARDS_WELL               = 9,  // Колодец Волшебника
    COMBO_RING_OF_THE_MAGI           = 10, // Кольцо Мага
    COMBO_CORNUCOPIA                 = 11, // Рог изобилия
    MAX_COMBO_ARTIFACTS              = 12  // Кол-во сборных артефактов
};

template<>
struct nh3api::enum_limits<CombinationArtifactType>
    : nh3api::enum_limits_base<CombinationArtifactType, COMBO_ANGELIC_ALLIANCE, COMBO_CORNUCOPIA>
{ static inline constexpr bool is_specialized = true; };

// Artifact traits /
// Свойства артефактов.
// size = 0x20 = 32, align = 4
struct TArtifactTraits
{
    // Artifact name /
    // Название артефакта.
    // offset: +0x0 = +0,  size = 0x4 = 4
    const char* m_name;

    // Artifact cost in gold /
    // Стоимость артефакта(в золоте).
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t m_cost;

    // Slot that this artifact can be held in /
    // Слот, в который можно положить этот артефакт.
    // offset: +0x8 = +8,  size = 0x4 = 4
    TArtifactSlot m_allowableSlotMask;

    // Artifact class /
    // Класс артефакта
    // offset: +0xC = +12,  size = 0x4 = 4
    ArtifactClass m_class;

    // Artifact description
    // Описание артефакта
    // offset: +0x10 = +16,  size = 0x4 = 4
    const char* m_description;

    // Combo artifact type(COMBO_NONE if not combo) /
    // Тип сборного артефакта, если этот артефакт им является.
    // offset: +0x14 = +20,  size = 0x4 = 4
    CombinationArtifactType m_comboType;

    // Combo artifact which the current artifact is one of the requirements /
    // Тип сборного артефакта, для которого данный артефакт является составным.
    // offset: +0x18 = +24,  size = 0x4 = 4
    CombinationArtifactType m_targetCombo;

    unsigned char : 8;

    // Artifact gives spells /
    // Артефакт даёт заклинания.
    // offset: +0x1D = +29,  size = 0x1 = 1
    bool m_givesSpells;

    unsigned char : 8;
    unsigned char : 8;
} NH3API_MSVC_LAYOUT;

// Artifact slots traits /
// Свойства слотов артефактов.
// size = 0x8 = 8, align = 4
struct TArtifactSlotTraits
{
    // Name /
    // Название слота.
    // offset: +0x0 = +0,  size = 0x4 = 4
    const char* name;

    // Slot /
    // Слот.
    // offset: +0x4 = +4,  size = 0x4 = 4
    TArtifactSlot type;

};
#pragma pack(pop)

// Artifact traits /
// Свойства артефактов.
inline std::array<TArtifactTraits, MAX_ARTIFACTS>& akArtifactTraits
= get_global_var_ref(0x693A58, std::array<TArtifactTraits, MAX_ARTIFACTS>);

// Artifact slot traits /
// Свойства слотов артефактов.
inline std::array<TArtifactSlotTraits, MAX_ARTIFACT_SLOTS>& akArtifactSlotTraits
= get_global_var_ref(0x694C58, std::array<TArtifactSlotTraits, MAX_ARTIFACT_SLOTS>);

// Combination artifact info /
// Свойства сборочных артефактов.
inline std::array<CombinationArtifact, MAX_COMBO_ARTIFACTS>& akCombinationArtifacts
= get_global_var_ref(0x693938, std::array<CombinationArtifact, MAX_COMBO_ARTIFACTS>);

// Artifact currently held in mouse in the dialog /
// Артефакт, в данный момент удерживаемый мышью в диалоге.
inline type_artifact& holding_artifact = get_global_var_ref(0x698AD8, type_artifact);

NH3API_WARNING(pop)
