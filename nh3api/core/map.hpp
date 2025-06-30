//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "hero.hpp" // HeroPlaceholder
#include "objects.hpp" // map objects
#include "quests.hpp" // quests
#include "terrain.hpp" // TTerrainType, type_point

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// Размер карты по горизонтали
NH3API_INLINE_OR_EXTERN int32_t& MAP_WIDTH
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6783C8, int32_t));

// Размер карты по вертикали
NH3API_INLINE_OR_EXTERN int32_t& MAP_HEIGHT
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6783CC, int32_t));

#pragma pack(push, 4)
// Global timed event /
// Глобальное событие карты.
// size = 0x34 = 52, align = 4
class TTimedEvent
{
    public:
        NH3API_FORCEINLINE
        TTimedEvent() NH3API_NOEXCEPT
            : PlayerFlags(0),
              ApplyToHuman(false),
              ApplyToComputer(false),
              FirstTime(0),
              Interval(0)
        {
            ResQty.fill(0);
        }

        NH3API_FORCEINLINE
        TTimedEvent(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Message(tag)
        { NH3API_IGNORE(ResQty,
                        PlayerFlags,
                        ApplyToHuman,
                        ApplyToComputer,
                        FirstTime,
                        Interval); }
    public:
        // Event message /
        // Сообщение события.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message;

        // Bonus resources /
        // Бонусные ресурсы(могут быть отрицательными).
        // offset: +0x10 = +16,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty;

        // Affected players /
        // На каких игроков действует это событие.
        // offset: +0x2C = +44,  size = 0x1 = 1
        uint8_t PlayerFlags;

        // Applies to human player /
        // Событие работает на игрока-человека.
        // offset: +0x2D = +45,  size = 0x1 = 1
        bool ApplyToHuman;

        // Applies to AI player /
        // Событие работает на компьютера.
        // offset: +0x2E = +46,  size = 0x1 = 1
        bool ApplyToComputer;

        // First day /
        // Первый день события.
        // offset: +0x30 = +48,  size = 0x2 = 2
        int16_t FirstTime;

        // Repeat interval /
        // Интервал повторения события.
        // offset: +0x32 = +50,  size = 0x2 = 2
        int16_t Interval;

};
#pragma pack(pop)

#pragma pack(push, 8)
// Town event /
// Событие города.
// size = 0x50 = 80, align = 8, baseclass: TTimedEvent
class TTownEvent : TTimedEvent
{
    public:
        NH3API_FORCEINLINE
        TTownEvent() NH3API_NOEXCEPT
            : TTimedEvent()
        { generatorBonuses.fill(0); }

        NH3API_FORCEINLINE
        TTownEvent(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : TTimedEvent(tag)
        { NH3API_IGNORE(TownNum,
                        BuildBuildings,
                        generatorBonuses); }
    public:
        // Town ID /
        // ID города, к которому применяется это событие.
        // offset: +0x34 = +52,  size = 0x1 = 1
        int8_t TownNum;

        // Building bonuses /
        // Бонусы построек.
        // offset: +0x38 = +56,  size = 0x8 = 8
        int64_t BuildBuildings;

        // Creature bonuses /
        // Бонусы существ.
        // offset: +0x40 = +64,  size = 0xE = 14
        std::array<uint16_t, 7> generatorBonuses;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x50, TTownEvent);

#pragma pack(push, 4)
// Map information /
// Информация о карте
// size = 0xF5C = 3932, align = 4
class NewfullMap
{
    public:
        NH3API_FORCEINLINE
        NewfullMap() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(NewfullMap)
        { THISCALL_1(void, 0x4FD6B0, this); }

        NH3API_FORCEINLINE
        NewfullMap(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : ObjectTypes(tag),
              Objects(tag),
              Sprites(tag),
              CustomTreasureList(tag),
              CustomMonsterList(tag),
              BlackBoxList(tag),
              SeerHutList(tag),
              QuestGuardList(tag),
              TimedEventList(tag),
              TownEventList(tag),
              PlaceHolderList(tag),
              QuestList(tag),
              RandomDwellingList(tag)
            #if NH3API_STD_INITIALIZER_LIST
            , // I HATE THIS!
            ObjectTypeTables{tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag,
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
                        tag, tag, tag, tag, tag, tag, tag}
            #endif
        { NH3API_IGNORE(cellData, Size, HasTwoLevels); }

    public:
        NewmapCell* cell(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(NewmapCell*, 0x4086D0, this, x, y, z); }

        const NewmapCell* cell(int32_t x, int32_t y, int32_t z) const
        { return THISCALL_4(NewmapCell*, 0x4086D0, this, x, y, z); }

        int32_t PlaceObject(int32_t ObjectIndex, bool setExtraInfo)
        { return THISCALL_3(int32_t, 0x506170, this, ObjectIndex, setExtraInfo); }

    public:
        // Map object types /
        // Список данных о типах объектов на карте.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_vector<CObjectType> ObjectTypes;

        // Map objects /
        // Список объектов на карте
        // offset: +0x10 = +16,  size = 0x10 = 16
        exe_vector<CObject> Objects;

        // Map objects sprites /
        // Список спрайтов объектов.
        // offset: +0x20 = +32,  size = 0x10 = 16
        exe_vector<CSprite*> Sprites;

         // Customized resources or artifacts /
        // Список настроенных артефактов/ресурсов
        // offset: +0x30 = +48,  size = 0x10 = 16
        exe_vector<TreasureData> CustomTreasureList;

        // Customized monsters /
        // Список монстров.
        // offset: +0x40 = +64,  size = 0x10 = 16
        exe_vector<MonsterData> CustomMonsterList;

        // Pandora's Boxes /
        // Список шкатулок пандоры.
        // offset: +0x50 = +80,  size = 0x10 = 16
        exe_vector<BlackBoxData> BlackBoxList;

        // Seer Huts /
        // Список хижин провидца.
        // offset: +0x60 = +96,  size = 0x10 = 16
        exe_vector<TSeerHut> SeerHutList;

        // Quest guards /
        // Список стражей границей квеста.
        // offset: +0x70 = +112,  size = 0x10 = 16
        exe_vector<TQuestGuard> QuestGuardList;

        // Timed global map events list /
        // Список глобальных событий карты.
        // offset: +0x80 = +128,  size = 0x10 = 16
        exe_vector<TTimedEvent> TimedEventList;

        // Timed town events list /
        // Список событий города.
        // offset: +0x90 = +144,  size = 0x10 = 16
        exe_vector<TTownEvent> TownEventList;

        // Hero placeholders(used for campaigns only) /
        // Лагеря героев(используются только для кампаний).
        // offset: +0xA0 = +160,  size = 0x10 = 16
        exe_vector<HeroPlaceholder> PlaceHolderList;

        // Quests /
        // Квесты.
        // offset: +0xB0 = +176,  size = 0x10 = 16
        exe_vector<type_quest*> QuestList;

        // Random dwellings. Randomized at every restart. /
        // Случайные жилища. Рандомизируются при каждом рестарте.
        // offset: +0xC0 = +192,  size = 0x10 = 16
        exe_vector<TRandomDwelling> RandomDwellingList;

        // Map cells /
        // Клетки карты.
        // offset: +0xD0 = +208,  size = 0x4 = 4
        NewmapCell* cellData;

        // Map size(both length and width) /
        // Размер карты(длина и ширина, совпадают).
        // offset: +0xD4 = +212,  size = 0x4 = 4
        int32_t Size;

        // Has dungeon?
        // Карта двухуровневая?
        // offset: +0xD8 = +216,  size = 0x1 = 1
        bool HasTwoLevels;

        // Map editor objects. Used by the Random Map Generator /
        // Объекты редактора карт. Используются генератором случайных карт.
        // offset: +0xDC = +220,  size = 0xE80 = 3712
        std::array<exe_vector<CObjectType>, MAX_OBJECTS> ObjectTypeTables;
};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END
