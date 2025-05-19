//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/memory.hpp" // bit_cast, byte_t, others

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// Terrain type /
// Тип почвы.
enum TTerrainType : int32_t
{
    eTerrainNone         = -1,
    eTerrainDirt         = 0, // Грязь
    eTerrainSand         = 1, // Песок
    eTerrainGrass        = 2, // Трава
    eTerrainSnow         = 3, // Снег
    eTerrainSwamp        = 4, // Болото
    eTerrainRough        = 5, // Степь
    eTerrainSubterranean = 6, // Подземелье
    eTerrainLava         = 7, // Лава
    eTerrainWater        = 8, // Вода
    eTerrainRock         = 9, // Скала ("чернота" в подземельях)
    kNumTerrainTypes     = 10, // Кол-во видов почв
};

// Road type /
// Тип дороги.
enum TRoadType : uint32_t 
{
    eRoadNone        = 0, //
    eRoadDirt        = 1, // Дорога по грязи
    eRoadGravel      = 2, // Гравийная дорога
    eRoadCobblestone = 3, // Мощеная дорога
    kNumRoadTypes    = 4, // Кол-во видов дороги
};

// River type /
// Тип реки.
enum TRiverType : uint32_t 
{
    eRiverNone     = 0, 
    eRiverClear    = 1, // Чистая река
    eRiverIcy      = 2, // Ледяная река
    eRiverMuddy    = 3, // Грязная река
    eRiverLava     = 4, // Лавовая река
    kNumRiverTypes = 5, // Кол-во видов рек
};

// Map object types /
// Объекты на карте.
enum TAdventureObjectType : int32_t
{
    OBJECT_ALTAR_OF_SACRIFICE          = 2,
    OBJECT_ANCHOR_POINT                = 3, // Точка, на которой можно высадиться
    OBJECT_ARENA                       = 4, // Арена
    OBJECT_ARTIFACT                    = 5, // Артефакт(объект)
    OBJECT_PANDORAS_BOX                = 6, // Ящик пандоры
    OBJECT_BLACK_MARKET                = 7, // Чёрный рынок
    OBJECT_BOAT                        = 8, // Лодка
    OBJECT_BORDERGUARD                 = 9, // Страж задания
    OBJECT_KEYMASTER                   = 10, // Палатка ключника
    OBJECT_BUOY                        = 11, // Буй
    OBJECT_CAMPFIRE                    = 12, // Костёр
    OBJECT_CARTOGRAPHER                = 13, // Картограф
    OBJECT_SWAN_POND                   = 14, // Лебединый пруд
    OBJECT_COVER_OF_DARKNESS           = 15, // Вуаль тьмы
    OBJECT_CREATURE_BANK               = 16, // Банк существ
    OBJECT_CREATURE_GENERATOR1         = 17, //
    OBJECT_CREATURE_GENERATOR2         = 18, //
    OBJECT_CREATURE_GENERATOR3         = 19, //
    OBJECT_CREATURE_GENERATOR4         = 20, //
    OBJECT_CURSED_GROUND               = 21, // Проклятая земля
    OBJECT_CORPSE                      = 22, // Скелет
    OBJECT_MARLETTO_TOWER              = 23, // Башня Марлетто
    OBJECT_DERELICT_SHIP               = 24, // Покинутый корабль
    OBJECT_DRAGON_UTOPIA               = 25, // Утопия драконов
    OBJECT_EVENT                       = 26, // Событие(объект, невидим на карте)
    OBJECT_EYE_OF_MAGI                 = 27, // Глаз мага
    OBJECT_FAERIE_RING                 = 28, // Кольцо фей
    OBJECT_FLOTSAM                     = 29, // Навес
    OBJECT_FOUNTAIN_OF_YOUTH           = 31, // Фонтан молодости
    OBJECT_FOUNTAIN_O_FORTUNE          = 30, // Фонтан удачи
    OBJECT_GARDEN_OF_REVELATION        = 32, // Сад откровений
    OBJECT_GARRISON                    = 33, // Гарнизон
    OBJECT_HERO                        = 34, // Герой(объект)
    OBJECT_HILL_FORT                   = 35, // Форт на холме
    OBJECT_GRAIL                       = 36, // Грааль(объект, не видим на карте)
    OBJECT_HUT_OF_MAGI                 = 37, // Хижина мага
    OBJECT_IDOL_OF_FORTUNE             = 38, // Идол удачи
    OBJECT_LEAN_TO                     = 39, // Навес
    OBJECT_DECORATIVE                  = 40, // ???
    OBJECT_LIBRARY_OF_ENLIGHTENMENT    = 41, // Библиотека просвещения
    OBJECT_LIGHTHOUSE                  = 42, // Маяк
    OBJECT_MONOLITH_ONE_WAY_ENTRANCE   = 43, // Монолит входа
    OBJECT_MONOLITH_ONE_WAY_EXIT       = 44, // Монолит выхода
    OBJECT_MONOLITH_TWO_WAY            = 45, // Двусторонний монолит
    OBJECT_MAGIC_PLAINS1               = 46, //
    OBJECT_SCHOOL_OF_MAGIC             = 47, // Школа магии
    OBJECT_MAGIC_SPRING                = 48, // Магический пруд
    OBJECT_MAGIC_WELL                  = 49, // Колодец
    OBJECT_MARKET_OF_TIME              = 50, // Рынок времени
    OBJECT_MERCENARY_CAMP              = 51, // Лагерь беженцев
    OBJECT_MERMAID                     = 52, // Русалки
    OBJECT_MINE                        = 53, // Шахта
    OBJECT_MONSTER                     = 54, // Монстр
    OBJECT_MYSTICAL_GARDEN             = 55, // Мистический сад
    OBJECT_OASIS                       = 56, // Оазис
    OBJECT_OBELISK                     = 57, // Обелис
    OBJECT_REDWOOD_OBSERVATORY         = 58, // Обсерватория красного дерева
    OBJECT_OCEAN_BOTTLE                = 59, // Морская бутылка
    OBJECT_PILLAR_OF_FIRE              = 60, //
    OBJECT_STAR_AXIS                   = 61, //
    OBJECT_PRISON                      = 62, // Тюрьма
    OBJECT_PYRAMID                     = 63, // Пирамида
    OBJECT_RALLY_FLAG                  = 64, //
    OBJECT_RANDOM_ART                  = 65, // Случайный артефакт
    OBJECT_RANDOM_TREASURE_ART         = 66, // Случайный артефакт-сокровище
    OBJECT_RANDOM_MINOR_ART            = 67, // Случайный малый артефакт
    OBJECT_RANDOM_MAJOR_ART            = 68, //
    OBJECT_RANDOM_RELIC_ART            = 69, // Случайный артефакт-реликвия
    OBJECT_RANDOM_HERO                 = 70, // Случайный герой
    OBJECT_RANDOM_MONSTER              = 71, // Случайный монстр
    OBJECT_RANDOM_MONSTER_L1           = 72, // Случайный монстр 1 уровня
    OBJECT_RANDOM_MONSTER_L2           = 73, // Случайный монстр 2 уровня
    OBJECT_RANDOM_MONSTER_L3           = 74, // Случайный монстр 3 уровня
    OBJECT_RANDOM_MONSTER_L4           = 75, // Случайный монстр 4 уровня
    OBJECT_RANDOM_RESOURCE             = 76, //
    OBJECT_RANDOM_TOWN                 = 77, //
    OBJECT_REFUGEE_CAMP                = 78, //
    OBJECT_RESOURCE                    = 79, //
    OBJECT_SANCTUARY                   = 80, //
    OBJECT_SCHOLAR                     = 81, //
    OBJECT_SEA_CHEST                   = 82, //
    OBJECT_SEER_HUT                    = 83, //
    OBJECT_CRYPT                       = 84, //
    OBJECT_SHIPWRECK                   = 85, //
    OBJECT_SHIPWRECK_SURVIVOR          = 86, //
    OBJECT_SHIPYARD                    = 87, //
    OBJECT_SHRINE_OF_MAGIC_INCANTATION = 88, //
    OBJECT_SHRINE_OF_MAGIC_GESTURE     = 89, //
    OBJECT_SHRINE_OF_MAGIC_THOUGHT     = 90, //
    OBJECT_SIGN                        = 91, //
    OBJECT_SIRENS                      = 92, //
    OBJECT_SPELL_SCROLL                = 93,
    OBJECT_STABLES                     = 94,
    OBJECT_TAVERN                      = 95,
    OBJECT_TEMPLE                      = 96,
    OBJECT_DEN_OF_THIEVES              = 97,
    OBJECT_TOWN                        = 98,
    OBJECT_TRADING_POST                = 99,
    OBJECT_LEARNING_STONE              = 100,
    OBJECT_TREASURE_CHEST              = 101,
    OBJECT_TREE_OF_KNOWLEDGE           = 102,
    OBJECT_SUBTERRANEAN_GATE           = 103,
    OBJECT_UNIVERSITY                  = 104,
    OBJECT_WAGON                       = 105,
    OBJECT_WAR_MACHINE_FACTORY         = 106,
    OBJECT_SCHOOL_OF_WAR               = 107,
    OBJECT_WARRIORS_TOMB               = 108,
    OBJECT_WATER_WHEEL                 = 109,
    OBJECT_WATERING_HOLE               = 110,
    OBJECT_WHIRLPOOL                   = 111,
    OBJECT_WINDMILL                    = 112,
    OBJECT_WITCH_HUT                   = 113,

    TERRAIN_BRUSH                  = 114,
    TERRAIN_BUSH                   = 115,
    TERRAIN_CACTUS                 = 116,
    TERRAIN_CANYON                 = 117,
    TERRAIN_CRATER                 = 118,
    TERRAIN_DEAD_VEGETATION        = 119,
    TERRAIN_FLOWERS                = 120,
    TERRAIN_FROZEN_LAKE            = 121,
    TERRAIN_HEDGE                  = 122,
    TERRAIN_HILL                   = 123,
    TERRAIN_HOLE                   = 124,
    TERRAIN_KELP                   = 125,
    TERRAIN_LAKE                   = 126,
    TERRAIN_LAVA_FLOW              = 127,
    TERRAIN_LAVA_LAKE              = 128,
    TERRAIN_MUSHROOMS              = 129,
    TERRAIN_LOG                    = 130,
    TERRAIN_MANDRAKE               = 131,
    TERRAIN_MOSS                   = 132,
    TERRAIN_MOUND                  = 133,
    TERRAIN_MOUNTAIN               = 134,
    TERRAIN_OAK_TREES              = 135,
    TERRAIN_OUTCROPPING            = 136,
    TERRAIN_PINE_TREES             = 137,
    TERRAIN_PLANT                  = 138,
    TERRAIN_RIVER_1                = 139, // unused / не используется.
    TERRAIN_RIVER_2                = 140, // unused / не используется.
    TERRAIN_RIVER_3                = 141, // unused / не используется.
    TERRAIN_RIVER_4                = 142, // unused / не используется.
    TERRAIN_RIVER_DELTA            = 143,
    TERRAIN_ROAD_1                 = 144, // unused / не используется.
    TERRAIN_ROAD_2                 = 145, // unused / не используется.
    TERRAIN_ROAD_3                 = 146, // unused / не используется.
    TERRAIN_ROCK                   = 147,
    TERRAIN_SAND_DUNE              = 148,
    TERRAIN_SAND_PIT               = 149,
    TERRAIN_SHRUB                  = 150,
    TERRAIN_SKULL                  = 151,
    TERRAIN_STALAGMITE             = 152,
    TERRAIN_STUMP                  = 153,
    TERRAIN_TAR_PIT                = 154,
    TERRAIN_TREES                  = 155,
    TERRAIN_VINE                   = 156,
    TERRAIN_VOLCANIC_TENT          = 157,
    TERRAIN_VOLCANO                = 158,
    TERRAIN_WILLOW_TREES           = 159,
    TERRAIN_YUCCA_TREES            = 160,
    TERRAIN_REEF                   = 161,

    OBJECT_RANDOM_MONSTER_L5           = 162,
    OBJECT_RANDOM_MONSTER_L6           = 163,
    OBJECT_RANDOM_MONSTER_L7           = 164,

    // AB objects

    TERRAIN_BRUSH2                 = 165,
    TERRAIN_BUSH2                  = 166,
    TERRAIN_CACTUS2                = 167,
    TERRAIN_CANYON2                = 168,
    TERRAIN_CRATER2                = 169,
    TERRAIN_DEAD_VEGETATION2       = 170,
    TERRAIN_FLOWERS2               = 171,
    TERRAIN_FROZEN_LAKE2           = 172,
    TERRAIN_HEDGE2                 = 173,
    TERRAIN_HILL2                  = 174,
    TERRAIN_HOLE2                  = 175,
    TERRAIN_KELP2                  = 176,
    TERRAIN_LAKE2                  = 177,
    TERRAIN_LAVA_FLOW2             = 178,
    TERRAIN_LAVA_LAKE2             = 179,
    TERRAIN_MUSHROOMS2             = 180,
    TERRAIN_LOG2                   = 181,
    TERRAIN_MANDRAKE2              = 182,
    TERRAIN_MOSS2                  = 183,
    TERRAIN_MOUND2                 = 184,
    TERRAIN_MOUNTAIN2              = 185,
    TERRAIN_OAK_TREES2             = 186,
    TERRAIN_OUTCROPPING2           = 187,
    TERRAIN_PINE_TREES2            = 188,
    TERRAIN_PLANT2                 = 189,
    TERRAIN_RIVER_DELTA2           = 190,
    TERRAIN_ROCK2                  = 191,
    TERRAIN_SAND_DUNE2             = 192,
    TERRAIN_SAND_PIT2              = 193,
    TERRAIN_SHRUB2                 = 194,
    TERRAIN_SKULL2                 = 195,
    TERRAIN_STALAGMITE2            = 196,
    TERRAIN_STUMP2                 = 197,
    TERRAIN_TAR_PIT2               = 198,
    TERRAIN_TREES2                 = 199,
    TERRAIN_VINE2                  = 200,
    TERRAIN_VOLCANIC_TENT2         = 201,
    TERRAIN_VOLCANO2               = 202,
    TERRAIN_WILLOW_TREES2          = 203,
    TERRAIN_YUCCA_TREES2           = 204,
    TERRAIN_REEF2                  = 205,
    TERRAIN_DESERT_HILLS           = 206,
    TERRAIN_DIRT_HILLS             = 207,
    TERRAIN_GRASS_HILLS            = 208,
    TERRAIN_ROUGH_HILLS            = 209,
    TERRAIN_SUBTERRANEAN_ROCKS     = 210,
    TERRAIN_SWAMP_FOLIAGE          = 211,

    OBJECT_BORDER_GATE                 = 212,
    OBJECT_HERO_PLACEHOLDER            = 214,
    OBJECT_QUEST_GUARD                 = 215,
    OBJECT_RANDOM_DWELLING             = 216,
    OBJECT_RANDOM_DWELLING_LVL         = 217,
    OBJECT_RANDOM_DWELLING_FACTION     = 218,
    OBJECT_GARRISON2                   = 219,
    OBJECT_ABANDONED_MINE              = 220,
    OBJECT_TRADING_POST_SNOW           = 221,

    // SoD objects

    OBJECT_CLOVER_FIELD                = 222,
    OBJECT_CURSED_GROUND2              = 223, // Проклятая земля
    OBJECT_EVIL_FOG                    = 224, // Зловещий туман
    OBJECT_FAVORABLE_WINDS             = 225, // Клеверное поле
    OBJECT_FIERY_FIELDS                = 226, // Огненные поля
    OBJECT_HOLY_GROUNDS                = 227, // Святая земля
    OBJECT_LUCID_POOLS                 = 228, // Прозрачные пруды
    OBJECT_MAGIC_CLOUDS                = 229, // Магические облака
    OBJECT_MAGIC_PLAINS2               = 230, // Магические равнины
    OBJECT_ROCKLANDS                   = 231, // Скалистая земля

    OBJECT_NONE                   = -1,
    MAX_OBJECTS_ROE               = 165, // Кол-во объектов в RoE
    MAX_OBJECTS_AB                = 222, // Кол-во объектов в AB
    MAX_OBJECTS_SOD               = 232, // Кол-во объектов в SoD
    MAX_OBJECTS = MAX_OBJECTS_SOD // Кол-во объектов в игре

};

// Magical terrain type(SoD) /
// Тип накладной магической почвы(SoD).
enum EMagicTerrain
{
    MAGIC_TERRAIN_INVALID       = -1,
    MAGIC_TERRAIN_COAST         = 0, // Побережье
    MAGIC_TERRAIN_MAGIC_PLAINS  = 1, // Магические равнины
    MAGIC_TERRAIN_CURSED_GROUND = 2, // Проклятая земля
    MAGIC_TERRAIN_HOLY_GROUND   = 3, // Святая земля
    MAGIC_TERRAIN_EVIL_FOG      = 4, // Зловещий туман
    MAGIC_TERRAIN_CLOVER_FIELD  = 5, // Клеверное поле
    MAGIC_TERRAIN_LUCID_POOLS   = 6, // Прозрачные пруды
    MAGIC_TERRAIN_FIERY_FIELDS  = 7, // Огненные поля
    MAGIC_TERRAIN_ROCKLANDS     = 8, // Скалистая земля
    MAGIC_TERRAIN_MAGIC_CLOUDS  = 9  // Магические облака
};

// Town types /
// Типы городов(фракций).
enum TTownType : int32_t
{
    eTownNeutral    = -1, // Нейтральный
    eTownCastle     = 0,  // Замок
    eTownRampart    = 1,  // Оплот
    eTownTower      = 2,  // Башня
    eTownInferno    = 3,  // Инферно
    eTownNecropolis = 4,  // Некрополис
    eTownDungeon    = 5,  // Темница
    eTownStronghold = 6,  // Цитадель
    eTownFortress   = 7,  // Крепость
    eTownConflux    = 8,  // Сопряжение
    kNumTownsRoE    = 8,  // Количество городов в RoE
    kNumTownsAB     = 9,  // Количество городов в AB
    kNumTownsSoD    = kNumTownsAB, // Количество городов в SoD
    kNumTowns       = kNumTownsSoD // Количество городов в SoD
};

enum : uint32_t 
{
    kAlignmentsMaskRoE = 0xFF,
    kAlignmentsMaskAB  = 0x1FF, 
    kAlignmentsMaskSoD = kAlignmentsMaskAB,
    kAlignmentsMask    = kAlignmentsMaskSoD
};

// Map point: X, Y, Z coordinates /
// Точка на карте: координаты X, Y, Z.
class type_point
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        // The original game supports X, Y up to 255 and Z from 0 to 1. /
        // Оригинальная игра поддерживает X, Y до 255, а Z от 0 до 1.
        /// @param X is in range [0;1023]
        /// @param Y is in range [0;1023]
        /// @param Z is in range [0;15]
        type_point(int8_t X, int8_t Y, int8_t Z) NH3API_NOEXCEPT
            : x(X), y(Y), z(Z) {}

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        // Default constructor /
        // Конструктор по умолчанию.
        type_point() NH3API_NOEXCEPT
            : x(-1), y(-1), z(-1)
        {}

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        type_point(uint32_t data) NH3API_NOEXCEPT
        {
            *this = nh3api::bit_cast<type_point>(data);
        }

        NH3API_FORCEINLINE
        type_point(const nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        { NH3API_IGNORE(x, y, z); }

    // setters
    public:
        NH3API_CONSTEXPR
        type_point& set(int8_t X, int8_t Y, int8_t Z) NH3API_NOEXCEPT
        {
            x = X;
            y = Y;
            z = Z;
            return *this;
        }

        #if NH3API_HAS_BUILTIN_BIT_CAST
        NH3API_CONSTEXPR
        #endif
        type_point& set(uint32_t data) NH3API_NOEXCEPT
        {
            *this = nh3api::bit_cast<type_point>(data);
            return *this;
        }

        NH3API_CONSTEXPR
        type_point& setX(int8_t X) NH3API_NOEXCEPT
        { x = X; return *this; }

        NH3API_CONSTEXPR
        type_point& setY(int8_t Y) NH3API_NOEXCEPT
        { y = Y; return *this; }

        NH3API_CONSTEXPR
        type_point& setZ(int8_t Z) NH3API_NOEXCEPT
        { z = Z; return *this; }

    // getters
    public:
        NH3API_CONSTEXPR
        int16_t getX() const NH3API_NOEXCEPT
        { return x; }

        NH3API_CONSTEXPR
        int16_t getY() const NH3API_NOEXCEPT
        { return y; }

        NH3API_CONSTEXPR
        int8_t getZ() const NH3API_NOEXCEPT
        { return z; }

        #if NH3API_HAS_BUILTIN_BIT_CAST
        NH3API_CONSTEXPR
        #endif
        // return the underlying data
        uint32_t to_uint() const NH3API_NOEXCEPT
        { 
            //return nh3api::bit_cast<uint32_t>(*this);
            #if NH3API_HAS_BUILTIN_BIT_CAST
            return __builtin_bit_cast(uint32_t, *this);
            #else
            uint32_t result;
            memcpy(&result, this, sizeof(*this));
            return result;
            #endif
        }

        // This point is on map
        // Точка находится на карте
        bool is_valid() const NH3API_NOEXCEPT
        { return THISCALL_1(bool, 0x4B1090, this); }

    public:
        signed x : 10; // +00, bytes 0..1
        signed   : 6; // +10
        signed y : 10; // +16
        signed z : 4;  // +26
        signed   : 2; // +30
} NH3API_MSVC_LAYOUT;

#pragma pack(push, 1)
// size = 0x4 = 4, align = 1
struct tilePoint
{
    // offset: +0x0 = +0,  size = 0x1 = 1
    int8_t x;

    // offset: +0x1 = +1,  size = 0x1 = 1
    int8_t y;

protected:
    NH3API_MAYBE_UNUSED
    byte_t gap_2[2];

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct TPoint
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint() NH3API_NOEXCEPT
            : x(-1), y(-1)
        {}

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint(int32_t X, int32_t Y) NH3API_NOEXCEPT
            : x(X), y(Y)
        {}

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint& operator+=(const TPoint& other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint& operator-=(const TPoint& other)
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        friend TPoint operator+(TPoint left, const TPoint& right)
        {
            left += right;
            return left;
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        friend TPoint operator-(TPoint left, const TPoint& right)
        {
            left -= right;
            return left;
        }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t x;
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t y;
};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0xC = 12, align = 4
struct TPoint3 : public TPoint
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint3() NH3API_NOEXCEPT
            : TPoint(), z(0)
        {}

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TPoint3(int32_t X, int32_t Y, int32_t Z) NH3API_NOEXCEPT
            : TPoint(X, Y), z(Z)
        {}

    public:
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t z;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Additional map objects properties /
// Дополнительные свойства объектов на карте.
// size = 0x10 = 16, align = 4
struct ExtraObjectProperties
{
    public:
        // The object is not passable /
        // На объект невозможно попасть, даже на триггер.
        // offset: +0x0 = +0,  size = 0x1 = 1
        bool impassable;

        // The interaction with the object is possible from any adjacent cell, including cells from above /
        // Взаимодействие с объектом может происходить с любой клетки, в т.ч. клетки сверху.
        // offset: +0x1 = +1,  size = 0x1 = 1
        bool omnidirectional;

        // The object can disappear upon interaction /
        // Объект может исчезнуть после взаимодействия с ним.
        // offset: +0x2 = +2,  size = 0x1 = 1
        bool removable;

        // Object name /
        // Название объекта
        // offset: +0x4 = +4,  size = 0x4 = 4
        const char* name;

        // Object type /
        // Тип объекта.
        // offset: +0x8 = +8,  size = 0x4 = 4
        TAdventureObjectType type;

        // Object is decorative /
        // Объект является декоративным.
        // offset: +0xC = +12,  size = 0x1 = 1
        bool decorative;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
std::array<ExtraObjectProperties, MAX_OBJECTS>& gExtraObjectProperties
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6916E8, std::array<ExtraObjectProperties, MAX_OBJECTS>));

NH3API_INLINE_OR_EXTERN
const std::array<tilePoint, 8>& normalDirTable
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x678150, const std::array<tilePoint, 8>));

NH3API_DISABLE_WARNING_END
