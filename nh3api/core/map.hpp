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

#include "hero.hpp"    // HeroPlaceholder
#include "objects.hpp" // map objects
#include "quests.hpp"  // quests
#include "terrain.hpp" // TTerrainType, type_point

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// Размер карты по горизонтали
inline int32_t& MAP_WIDTH = get_global_var_ref(0x6783C8, int32_t);

// Размер карты по вертикали
inline int32_t& MAP_HEIGHT = get_global_var_ref(0x6783CC, int32_t);

#pragma pack(push, 4)
// Global timed event /
// Глобальное событие карты.
// size = 0x34 = 52, align = 4
class TTimedEvent
{
    public:
        inline TTimedEvent() noexcept = default;
        inline TTimedEvent(const nh3api::dummy_tag_t& tag) noexcept
            : Message { tag }
        {}

        inline ~TTimedEvent() noexcept                        = default;
        inline TTimedEvent(const TTimedEvent&)                = default;
        inline TTimedEvent(TTimedEvent&&) noexcept            = default;
        inline TTimedEvent& operator=(const TTimedEvent&)     = default;
        inline TTimedEvent& operator=(TTimedEvent&&) noexcept = default;

    public:
        // Event message /
        // Сообщение события.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message {};

        // Bonus resources /
        // Бонусные ресурсы(могут быть отрицательными).
        // offset: +0x10 = +16,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty {};

        // Affected players /
        // На каких игроков действует это событие.
        // offset: +0x2C = +44,  size = 0x1 = 1
        uint8_t PlayerFlags {0};

        // Applies to human player /
        // Событие работает на игрока-человека.
        // offset: +0x2D = +45,  size = 0x1 = 1
        bool ApplyToHuman {false};

        // Applies to AI player /
        // Событие работает на компьютера.
        // offset: +0x2E = +46,  size = 0x1 = 1
        bool ApplyToComputer {false};

        unsigned char : 8;

        // First day /
        // Первый день события.
        // offset: +0x30 = +48,  size = 0x2 = 2
        int16_t FirstTime {0};

        // Repeat interval /
        // Интервал повторения события.
        // offset: +0x32 = +50,  size = 0x2 = 2
        int16_t Interval {0};

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

#pragma pack(push, 8)
// Town event /
// Событие города.
// size = 0x50 = 80, align = 8, baseclass: TTimedEvent
class TTownEvent : TTimedEvent
{
    public:
        inline TTownEvent() noexcept = default;
        inline TTownEvent(const nh3api::dummy_tag_t& tag) noexcept
            : TTimedEvent(tag)
        {}

        inline ~TTownEvent() noexcept                       = default;
        inline TTownEvent(const TTownEvent&)                = default;
        inline TTownEvent(TTownEvent&&) noexcept            = default;
        inline TTownEvent& operator=(const TTownEvent&)     = default;
        inline TTownEvent& operator=(TTownEvent&&) noexcept = default;

    public:
        // Town ID /
        // ID города, к которому применяется это событие.
        // offset: +0x34 = +52,  size = 0x1 = 1
        int8_t TownNum {0};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Building bonuses /
        // Бонусы построек.
        // offset: +0x38 = +56,  size = 0x8 = 8
        uint64_t BuildBuildings {0ULL};

        // Creature bonuses /
        // Бонусы существ.
        // offset: +0x40 = +64,  size = 0xE = 14
        std::array<uint16_t, 7> generatorBonuses {};

        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 8

NH3API_SIZE_ASSERT(0x50, TTownEvent);

#pragma pack(push, 4)
// Map information /
// Информация о карте
// size = 0xF5C = 3932, align = 4
class NewfullMap
{
    public:
        inline NewfullMap() noexcept
            : NewfullMap(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4FD6B0, this); }

        inline NewfullMap(const nh3api::dummy_tag_t& tag) noexcept
            : ObjectTypes { tag },
              Objects { tag },
              Sprites { tag },
              CustomTreasureList { tag },
              CustomMonsterList { tag },
              BlackBoxList { tag },
              SeerHutList { tag },
              QuestGuardList { tag },
              TimedEventList { tag },
              TownEventList { tag },
              PlaceHolderList { tag },
              QuestList { tag },
              RandomDwellingList { tag },
              ObjectTypeTables
                {
                  { tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
                   tag, tag, tag, tag, tag, tag, tag }
                }
        {}

        inline ~NewfullMap() noexcept
        { THISCALL_1(void, 0x4FD830, this); }

        NewfullMap(const NewfullMap&)            = delete;
        NewfullMap(NewfullMap&&)                 = delete;
        NewfullMap& operator=(const NewfullMap&) = delete;
        NewfullMap& operator=(NewfullMap&&)      = delete;

    public:
        [[nodiscard]] NewmapCell* cell(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(NewmapCell*, 0x4086D0, this, x, y, z); }

        [[nodiscard]] const NewmapCell* cell(int32_t x, int32_t y, int32_t z) const
        { return THISCALL_4(NewmapCell*, 0x4086D0, this, x, y, z); }

        int32_t PlaceObject(int32_t ObjectIndex, bool setExtraInfo)
        { return THISCALL_3(int32_t, 0x506170, this, ObjectIndex, setExtraInfo); }

    public:
        union {
        // Map object types /
        // Список данных о типах объектов на карте.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_vector<CObjectType> ObjectTypes;
        };

        union {
        // Map objects /
        // Список объектов на карте
        // offset: +0x10 = +16,  size = 0x10 = 16
        exe_vector<CObject> Objects;
        };

        union {
        // Map objects sprites /
        // Список спрайтов объектов.
        // offset: +0x20 = +32,  size = 0x10 = 16
        exe_vector<CSprite*> Sprites;
        };

        union {
        // Customized resources or artifacts /
        // Список настроенных артефактов/ресурсов
        // offset: +0x30 = +48,  size = 0x10 = 16
        exe_vector<TreasureData> CustomTreasureList;
        };

        union {
        // Customized monsters /
        // Список монстров.
        // offset: +0x40 = +64,  size = 0x10 = 16
        exe_vector<MonsterData> CustomMonsterList;
        };

        union {
        // Pandora's Boxes /
        // Список шкатулок пандоры.
        // offset: +0x50 = +80,  size = 0x10 = 16
        exe_vector<BlackBoxData> BlackBoxList;
        };

        union {
        // Seer Huts /
        // Список хижин провидца.
        // offset: +0x60 = +96,  size = 0x10 = 16
        exe_vector<TSeerHut> SeerHutList;
        };

        union {
        // Quest guards /
        // Список стражей границей квеста.
        // offset: +0x70 = +112,  size = 0x10 = 16
        exe_vector<TQuestGuard> QuestGuardList;
        };

        union {
        // Timed global map events list /
        // Список глобальных событий карты.
        // offset: +0x80 = +128,  size = 0x10 = 16
        exe_vector<TTimedEvent> TimedEventList;
        };

        union {
        // Timed town events list /
        // Список событий города.
        // offset: +0x90 = +144,  size = 0x10 = 16
        exe_vector<TTownEvent> TownEventList;
        };

        union {
        // Hero placeholders(used for campaigns only) /
        // Лагеря героев(используются только для кампаний).
        // offset: +0xA0 = +160,  size = 0x10 = 16
        exe_vector<HeroPlaceholder> PlaceHolderList;
        };

        union {
        // Quests /
        // Квесты.
        // offset: +0xB0 = +176,  size = 0x10 = 16
        exe_vector<type_quest*> QuestList;
        };

        union {
        // Random dwellings. Randomized at every restart. /
        // Случайные жилища. Рандомизируются при каждом рестарте.
        // offset: +0xC0 = +192,  size = 0x10 = 16
        exe_vector<TRandomDwelling> RandomDwellingList;
        };

        // Map cells /
        // Клетки карты.
        // offset: +0xD0 = +208,  size = 0x4 = 4
        NewmapCell* cellData;

        // Map size(both length and width) /
        // Размер карты(длина и ширина, совпадают).
        // offset: +0xD4 = +212,  size = 0x4 = 4
        int32_t Size;

        // Has dungeon? /
        // Карта двухуровневая?
        // offset: +0xD8 = +216,  size = 0x1 = 1
        bool HasTwoLevels;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        union {
        // Map editor objects. Used by the Random Map Generator /
        // Объекты редактора карт. Используются генератором случайных карт.
        // offset: +0xDC = +220,  size = 0xE80 = 3712
        std::array<exe_vector<CObjectType>, MAX_OBJECTS> ObjectTypeTables;
        };
} NH3API_MSVC_LAYOUT;

// Creature bank level traits(there are four levels) /
// Свойства уровня банка существ(всего их четыре).
// size = 0x60 = 96, align = 4
struct type_creature_bank_level
{
    // Creature bank level guards /
    // Охрана уровня банка существ.
    // offset: +0x0 = +0,  size = 0x38 = 56
    armyGroup guards;

    // Resources that the player receives on victory /
    // Ресурсы, которые игрок получает после победы в качестве вознаграждения.
    // offset: +0x38 = +56,  size = 0x1C = 28
    std::array<EGameResource, 7> resources;

    // Types of creatures that hero receives on victory /
    // Тип существ, которых герой получает после победы в качестве вознаграждения.
    // offset: +0x54 = +84,  size = 0x4 = 4
    TCreatureType creature_type;

    // Amount of creatures that hero receives on victory /
    // Количество существ, которых герой получает после победы в качестве вознаграждения.
    // offset: +0x58 = +88,  size = 0x1 = 1
    int8_t creature_amount;

    // Chance of the current creature bank level /
    // Шанс текущего уровня банка существ.
    // offset: +0x59 = +89,  size = 0x1 = 1
    int8_t chance;

    // Chance of guards being upgraded /
    // Шанс улучшенного существа в охране.
    // offset: +0x5A = +90,  size = 0x1 = 1
    int8_t upg_chance;

    // Number of treasure artifacts that drop on victory /
    // Количество артефактов-сокровищ, которые выпадают после победы.
    // offset: +0x5B = +91,  size = 0x1 = 1
    int8_t treasure_artifacts;

    // Number of minor artifacts that drop on victory /
    // Количество малых артефактов, которые выпадают после победы.
    // offset: +0x5C = +92,  size = 0x1 = 1
    int8_t minor_artifacts;

    // Number of major artifacts that drop on victory /
    // Количество великих артефактов, которые выпадают после победы.
    // offset: +0x5D = +93,  size = 0x1 = 1
    int8_t major_artifacts;

    // Number of relic artifacts that drop on victory /
    // Количество артефактов-реликвий, которые выпадают после победы.
    // offset: +0x5E = +94,  size = 0x1 = 1
    int8_t relic_artifacts;

    unsigned char : 8;

} NH3API_MSVC_LAYOUT;

// Creature bank traits /
// Свойства банка существ.
// size = 0x190 = 400, align = 4
struct type_creature_bank_traits
{
    // Creature bank name /
    // Название банка существ.
    // offset: +0x0 = +0,  size = 0x10 = 16
    exe_string name;

    // Creature bank levels /
    // Уровни банка существ.
    // offset: +0x10 = +16,  size = 0x180 = 384
    std::array<type_creature_bank_level, 4> levels;

};
#pragma pack(pop) // 4

// Creature bank type /
// Тип банка существ.
enum ECreatureBankType : int32_t
{
    BANK_CYCLOPS_STOCKPILE    = 0,  // Склады Циклопов
    BANK_DWARVEN_TREASURY     = 1,  // Сокровищница Гномов
    BANK_GRIFFIN_CONSERVATORY = 2,  // Консерватория Грифонов
    BANK_IMP_CACHE            = 3,  // Тайник Бесов
    BANK_MEDUSA_STORES        = 4,  // Хранилище Медуз
    BANK_NAGA_BANK            = 5,  // Банк Наг
    BANK_DRAGON_FLY_HIVE      = 6,  // Улей Ядовитых Змиев
    BANK_SHIPWRECK            = 7,  // Место Кораблекрушения
    BANK_DERELICT_SHIP        = 8,  // Покинутый Корабль
    BANK_CRYPT                = 9,  // Склеп
    BANK_DRAGON_UTOPIA        = 10, // Утопия Драконов
    MAX_CREATURE_BANK_TYPES   = 11  // Максимальное количество типов банков существ.
};

// Traits of all creature bank types /
// Свойства всех возможных видов банков существ.
inline std::array<type_creature_bank_traits, MAX_CREATURE_BANK_TYPES>& const_creature_bank_traits
= get_global_var_ref(0x695088, std::array<type_creature_bank_traits, MAX_CREATURE_BANK_TYPES>);

// Map visibility bits for each player and 9th bit for a monster on the map /
// Видимость карты для игроков и для монстров на карте.
inline uint16_t* const& mapExtra = get_global_var_ref(0x698A48, uint16_t*);

[[nodiscard]] inline uint16_t GetMapExtra(uint32_t x, uint32_t y, uint32_t z)
{ return FASTCALL_3(uint16_t, 0x4F8040, x, y, z); }

[[nodiscard]] inline uint16_t* GetMapExtraPtr(uint32_t x, uint32_t y, uint32_t z)
{ return FASTCALL_3(uint16_t*, 0x4F8070, x, y, z); }

NH3API_WARNING(pop)
