//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "nh3api_std/exe_vector.hpp" // exe_vector<T>
#include "resources/resources_include.hpp" // EGameResource
#include "creatures.hpp" // TCreatureType, armyGroup
#include "spells.hpp" // SpellID

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#if NH3API_CHECK_CPP11
// Bit shift masks lookup table /
// Таблица побитовых масок.
constexpr const std::array<uint64_t, 64> bitNumber =
{
    {
        1ULL << 0ULL,
        1ULL << 1ULL,
        1ULL << 2ULL,
        1ULL << 3ULL,
        1ULL << 4ULL,
        1ULL << 5ULL,
        1ULL << 6ULL,
        1ULL << 7ULL,
        1ULL << 8ULL,
        1ULL << 9ULL,
        1ULL << 10ULL,

        1ULL << 11ULL,
        1ULL << 12ULL,
        1ULL << 13ULL,
        1ULL << 14ULL,
        1ULL << 15ULL,
        1ULL << 16ULL,
        1ULL << 17ULL,
        1ULL << 18ULL,
        1ULL << 19ULL,
        1ULL << 20ULL,

        1ULL << 21ULL,
        1ULL << 22ULL,
        1ULL << 23ULL,
        1ULL << 24ULL,
        1ULL << 25ULL,
        1ULL << 26ULL,
        1ULL << 27ULL,
        1ULL << 28ULL,
        1ULL << 29ULL,
        1ULL << 30ULL,

        1ULL << 31ULL,
        1ULL << 32ULL,
        1ULL << 33ULL,
        1ULL << 34ULL,
        1ULL << 35ULL,
        1ULL << 36ULL,
        1ULL << 37ULL,
        1ULL << 38ULL,
        1ULL << 39ULL,
        1ULL << 40ULL,

        1ULL << 41ULL,
        1ULL << 42ULL,
        1ULL << 43ULL,
        1ULL << 44ULL,
        1ULL << 45ULL,
        1ULL << 46ULL,
        1ULL << 47ULL,
        1ULL << 48ULL,
        1ULL << 49ULL,
        1ULL << 50ULL,

        1ULL << 51ULL,
        1ULL << 52ULL,
        1ULL << 53ULL,
        1ULL << 54ULL,
        1ULL << 55ULL,
        1ULL << 56ULL,
        1ULL << 57ULL,
        1ULL << 58ULL,
        1ULL << 59ULL,
        1ULL << 60ULL,

        1ULL << 61ULL,
        1ULL << 62ULL,
        1ULL << 63ULL,
    }
};
#else
// Bit shift masks lookup table /
// Таблица побитовых масок.
extern const std::array<uint64_t, 64>& bitNumber;
#endif

// Town building IDs. /
// Идентификаторы городских построек.
enum type_building_id : int32_t
{

    // Common buildings IDs /
    // Общие для всех фракций ID построек.

    MAGE_GUILD_ID       = 0,  // Гильдия магов 1 уровня
    MAGE_GUILD2_ID      = 1,  // Гильдия магов 2 уровня
    MAGE_GUILD3_ID      = 2,  // Гильдия магов 3 уровня
    MAGE_GUILD4_ID      = 3,  // Гильдия магов 4 уровня
    MAGE_GUILD5_ID      = 4,  // Гильдия магов 5 уровня
    TAVERN_ID           = 5,  // Таверна
    DOCK_ID             = 6,  // Верфь
    CASTLE_FORT_ID      = 7,  // Форт
    CASTLE_CITADEL_ID   = 8,  // Цитадель
    CASTLE_CASTLE_ID    = 9,  // Замок
    HALL_VILLAGE_ID     = 10, // Decorative / Декор.
    HALL_TOWN_ID        = 11, // Decorative / Декор.
    HALL_CITY_ID        = 12, // Decorative / Декор.
    HALL_CAPITOL_ID     = 13, // Капитолий.
    MARKETPLACE_ID      = 14, // Рынок
    MARKETPLACE_SILO_ID = 15, // Хранилище ресурсов
    BLACKSMITH_ID       = 16, // Кузница
    SPECIAL_BUILDING_ID = 17, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    HORDE_ID            = 18, // Орда (увеличивает прирост определенного существа)
    HORDE_UPG_ID        = 19, // Is being built altogether with HORDE_ID / Строится одновременно с HORDE_ID.
    DOCK_WITH_BOAT_ID   = 20, // Верфь с кораблём(TODO: выяснить технические подробности)
    EXTRA_0_ID          = 21, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    EXTRA_1_ID          = 22, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    EXTRA_2_ID          = 23, // Decorative(upon building village, so always present) / Декорация(появляется сразу)
    HORDE_2_ID          = 24, // Second horde building(if present) / Вторая орда(если есть)
    HORDE_2_UPG_ID      = 25, // Is being built altogether with HORDE_2_ID / Строится одновременно с HORDE_2_ID.
    HOLY_GRAIL_ID       = 26, // Постройка Грааля
    EXTRA_3_ID          = 27, // Decorative(upon building town, so always present) / Декорация(появляется сразу)
    EXTRA_4_ID          = 28, // Decorative / Декор.
    EXTRA_5_ID          = 29, // Decorative / Декор.
    DWELLING_0_ID       = 30, // Lvl. 1 creature dwelling / Жилище существ 1 уровня.
    DWELLING_1_ID       = 31, // Lvl. 2 creature dwelling / Жилище существ 2 уровня
    DWELLING_2_ID       = 32, // Lvl. 3 creature dwelling / Жилище существ 3 уровня
    DWELLING_3_ID       = 33, // Lvl. 4 creature dwelling / Жилище существ 4 уровня
    DWELLING_4_ID       = 34, // Lvl. 5 creature dwelling / Жилище существ 5 уровня
    DWELLING_5_ID       = 35, // Lvl. 6 creature dwelling / Жилище существ 6 уровня
    DWELLING_6_ID       = 36, // Lvl. 7 creature dwelling / Жилище существ 7 уровня
    DWELLING_0_UPG_ID   = 37, // Lvl. 1 upgraded creature dwelling / Улучшенное жилище существ 1 уровня
    DWELLING_1_UPG_ID   = 38, // Lvl. 2 upgraded creature dwelling / Улучшенное жилище существ 2 уровня
    DWELLING_2_UPG_ID   = 39, // Lvl. 3 upgraded creature dwelling / Улучшенное жилище существ 3 уровня
    DWELLING_3_UPG_ID   = 40, // Lvl. 4 upgraded creature dwelling / Улучшенное жилище существ 4 уровня
    DWELLING_4_UPG_ID   = 41, // Lvl. 5 upgraded creature dwelling / Улучшенное жилище существ 5 уровня
    DWELLING_5_UPG_ID   = 42, // Lvl. 6 upgraded creature dwelling / Улучшенное жилище существ 6 уровня
    DWELLING_6_UPG_ID   = 43, // Lvl. 7 upgraded creature dwelling / Улучшенное жилище существ 7 уровня
    MAX_BUILDING_TYPE   = 44, // Максимальное количество построек в городе.

    const_village_hall  = 10, // Decorative / Декор.
    const_town_hall     = 11, // Decorative / Декор.
    const_city_hall     = 12, // Decorative / Декор.
    const_capitol_hall  = 13, // Decorative / Декор.
    const_fort          = 7,  // Форт.
    const_citadel       = 8,  // Цитадель.
    const_castle        = 9,  // Замок.
    const_tavern        = 5,  // Таверна.
    const_blacksmith    = 16, // Кузница.
    const_market        = 14, // Рынок.
    const_resource_silo = 15, // Хранилище ресурсов.
    const_mage_guild_1  = 0,  // Гильдия магов 1 уровня.
    const_mage_guild_2  = 1,  // Гильдия магов 2 уровня.
    const_mage_guild_3  = 2,  // Гильдия магов 3 уровня.
    const_mage_guild_4  = 3,  // Гильдия магов 4 уровня.
    const_mage_guild_5  = 4,  // Гильдия магов 5 уровня.
    const_shipyard      = 6  // Верфь.
};

NH3API_FORCEINLINE
const char* GetBuildingName(TTownType townType, type_building_id buildingId)
{ return FASTCALL_2(const char*, 0x460CC0, townType, buildingId); }

NH3API_INLINE_OR_EXTERN
// Town possible buildings mask /
// Маска возможных построек в городе(вообще).
const std::array<uint64_t, kNumTowns>&
gTownEligibleBuildMask NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x697740, const std::array<uint64_t, kNumTowns>));

NH3API_INLINE_OR_EXTERN
// Each building dependency mask /
// Маска необходимых построек для строительства каждой постройки каждой фракции.
const std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>&
gHierarchyMask NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6977E8, const std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>));

typedef std::array<int32_t, 7> build_cost_array_t;

#pragma pack(push, 4)
// Town /
// Город.
// size = 0x168 = 360, align = 8
class town
{
    public:
        NH3API_FORCEINLINE
        town() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(town)
        { THISCALL_1(void, 0x5BE280, this); }

        NH3API_FORCEINLINE
        town(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : cName(tag),
              SpellDisabledMask(tag),
              town_army(tag)
        {}

        NH3API_FORCEINLINE
        ~town() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4ACDD0, this); }

    public:
        // Get town location on the map /
        // Координаты города.
        type_point get_location() const
        { return type_point(mapX, mapY, mapZ); }

        // Does town has creatures in garrison? /
        // Есть ли в городе гарнизонные войска или герой с войсками?
        bool32_t HasGarrison() const
        { return THISCALL_1(bool32_t, 0x5BE3E0, this); }

        // Check if town has building <buildingId> /
        // Проверка, есть ли в городе постройка <buildingId>.
        bool HasBuilding(type_building_id buildingId, bool check_included = true) const
        { return THISCALL_3(bool, 0x4305A0, this, buildingId, check_included); }

        // Make garrison hero a guest(move down) /
        // Убрать гарнизонного героя из гарнизона, сделать его героем-гостем.
        void remove_garrison_hero()
        { THISCALL_1(void, 0x5BE790, this); }

        // Swap guest and garrison heroes. Transmit swap hero message /
        // Поменять местами гарнизонных героев. Оповестить обработчик сообщений новым сообщением о смене героев местами.
        void SwapHeroes()
        { THISCALL_1(void, 0x5BE850, this); }

        // Build <building> in town /
        // Построить постройку <building> в городе.
        type_building_id BuildBuilding(type_building_id building, bool SetBuiltFlag = true, bool apply_special_effect = true)
        { return THISCALL_4(type_building_id, 0x5BF1E0, this, building, SetBuiltFlag, apply_special_effect); }

        // If player has enough resources, build <building> /
        // Если у игрока достаточно ресурсов, построить <building>.
        bool buy_building(type_building_id building)
        { return THISCALL_2(bool, 0x5BF7C0, this, building); }

        // Get citadel/castle growth bonus of <creature>. Returns 0 if none /
        // Бонус прироста существа <creature>. Возвращает 0, если таких построек нет.
        int32_t get_castle_growth_bonus(TCreatureType creature) const
        { return THISCALL_2(int32_t, 0x5BF970, this, creature); }

        // Get town gold income /
        // Ежедневный доход в казну от города, в золоте.
        int16_t get_gold_income(bool include_silo = true) const
        { return THISCALL_2(int16_t, 0x5BFA00, this, include_silo); }

        // Can player build building now? /
        // Может ли игрок построить <building> сейчас?
        bool can_build(type_building_id building) const
        { return THISCALL_2(bool, 0x5C1120, this, building); }

        // Can player build building ever?
        // Может ли игрок построить <building> вообще?
        bool can_ever_build(type_building_id building_id) const
        { return THISCALL_2(bool, 0x5C1260, this, building_id); }

        // Get mask of buildings that can be built /
        // Маска построек, которые можно построить.
        uint64_t get_buildable_mask() const
        { return THISCALL_1(uint64_t, 0x5C1320, this); }

        // Fill array <resources> with the cost of the <building> /
        // Заполнить массив ресурсов <resources> стоимостью постройки <building>.
        void get_build_cost(type_building_id building, build_cost_array_t& resources) const
        { THISCALL_3(void, 0x5C14F0, this, building, resources.data()); }

        // Get <building> cost as pointer to array of 7 ints /
        // Получить массив ресурсов, необходимый для постройки <building>.
        const build_cost_array_t& get_build_cost_array(type_building_id building) const
        { return *THISCALL_2(const build_cost_array_t*, 0x5C1480, this, building); }

        // Is building ever available in town?
        // Возможно ли построить <building> в городе?
        bool is_legal_building(type_building_id building) const
        { return !!THISCALL_2(bool32_t, 0x5C16A0, this, building); }

        // Town native terrain /
        // Родная почва фракции.
        TTerrainType GetNativeTerrain() const
        { return THISCALL_1(TTerrainType, 0x5C1840, this); }

        // Town type(fraction) name /
        // Название типа(фракции) города.
        const char* GetTypeName() const
        { return THISCALL_1(const char*, 0x5C1850, this); }

        // Current town army. Priority is garrison army /
        // Текущая армия города. Приоритет отдаётся гарнизонной армии.
        armyGroup& get_army()
        { return *THISCALL_1(armyGroup*, 0x5C1860, this); }

        // Current town army. Priority is garrison army /
        // Текущая армия города. Приоритет отдаётся гарнизонной армии.
        const armyGroup& get_army() const
        { return *THISCALL_1(const armyGroup*, 0x5C1860, this); }

        static type_building_id UpgradedDwellingID(type_building_id id)
        { return FASTCALL_1(type_building_id, 0x5C18A0, id); }

    public:
        // Town id /
        // Идентификатор города.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t id;

        // Player
        // Игрок-владелец города
        // offset: +0x1 = +1,  size = 0x1 = 1
        int8_t playerOwner;

        // A building was built during the current turn /
        // На этом ходу было построено здание
        // offset: +0x2 = +2,  size: 0x1 = 1
        bool builtThisTurn;

        // AI: Has heroes that threaten the town /
        // (ИИ) Есть угрожающие герои.
        // offset: +0x3 = +3,  size: 0x1 = 1
        bool threatening_heroes;

        // Town type(fraction) /
        // Тип(фракция) города.
        // offset: +0x4 = +4,  size = 0x1 = 1
        int8_t townType;

        // Town x coordinate on the map /
        // X-координата города на карте
        // offset: +0x5 = +5,  size = 0x1 = 1
        uint8_t mapX;

        // Town y coordinate on the map /
        // Y-координата города на карте
        // offset: +0x6 = +6,  size = 0x1 = 1
        uint8_t mapY;

        // Town z coordinate on the map /
        // Z-координата города на карте
        // offset: +0x7 = +7,  size = 0x1 = 1
        uint8_t mapZ;

        // Boat generated by the town dock x coordinate /
        // X-координата позиции установки лодки портом города на карте
        // offset: +0x8 = +8,  size = 0x1 = 1
        uint8_t boatX;

        // Boat generated by the town dock y coordinate /
        // X-координата позиции установки лодки портом города на карте
        // offset: +0x9 = +9,  size = 0x1 = 1
        uint8_t boatY;

        // Garrison hero ID /
        // ID Героя в гарнизоне(сверху)
        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t garrisonHero;

        // Occupying hero ID /
        // ID Героя-гостя(снизу)
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t occupyingHero;

        // Mage guild level (0 = no mage guild) /
        // Уровень гильдии магов(0 = нет гильдии магов)
        // offset: +0x14 = +20,  size = 0x1 = 1
        int8_t mageLevel;

        // Creature dwellings populations /
        // Ненанятые войска в городе
        // offset: +0x16 = +22,  size = 0x1C = 28
        std::array<std::array<int16_t, 7>, 2> population;

        // Garrison formation is groupped /
        // Гарнизон(существа сверху) города сгруппирован
        // offset: +0x32 = +50,  size = 0x1 = 1
        bool bIsGrouped;

        // Mana vortex(Dungeon) can be used /
        // Можно ли использовать вихрь маны(Темница)
        // offset: +0x33 = +51,  size = 0x1 = 1
        bool ManaVortexFull;

        // Amount of magic pound(Rampart) resources generated this week /
        // Количество ресурсов, которые сгенерировал магический пруд(Оплот) на этой неделе.
        // offset: +0x34 = +52,  size = 0x1 = 1
        uint8_t pond_amount;

        // Type of magic pound(Rampart) resource generated this week /
        // Тип ресурса, который сгенерировал магический пруд(Оплот) на этой неделе.
        // offset: +0x38 = +56,  size = 0x4 = 4
        EGameResource pond_resource;

        // Type of creatures summoned by the Portal of summoning(Dungeon) /
        // Тип существа, который появился в портале призыва(Темница)
        // offset: +0x3C = +60,  size = 0x4 = 4
        TCreatureType summoningType;

        // Amount of creatures summoned by the Portal of summoning(Dungeon) /
        // Количество существа, который появился в портале призыва(Темница)
        // offset: +0x40 = +64,  size = 0x2 = 2
        int16_t summoningPopulation;

        // Spells for each level of mage guild /
        // Заклинания на каждом из 5 уровней гильдии магов.
        // offset: +0x44 = +68,  size = 0x78 = 120
        std::array<std::array<SpellID, 6>, MAX_SPELL_LEVELS> townSpells;

        // Maximum spells available for each level of mage guild /
        // Максимальное для каждого уровня гильдии магов количество заклинаний.
        // offset: +0xBC = +188,  size = 0x5 = 5
        std::array<int8_t, MAX_SPELL_LEVELS> maxTownSpellAvailable;

        // Town name /
        // Название города.
        // offset: +0xC4 = +196,  size = 0x10 = 16
        exe_string cName;

        // Forbidden spells in town /
        // Запрещённые в городе заклинания
        // offset: +0xD4 = +212,  size = 0x9 = 9
        exe_bitset<MAX_BOOK_SPELLS> SpellDisabledMask;

        // Town garrison army /
        // Гарнизон города(сверху).
        // offset: +0xE0 = +224,  size = 0x38 = 56
        armyGroup town_army;

        // Creature dwellings bonuses /
        // Бонусы к существам.
        // offset: +0x118 = +280,  size = 0x38 = 56
        std::array<std::array<int32_t, 7>, 2> generator_bonus;

        // Visible buildings mask /
        // Маска построек, которые видно на экране города.
        // offset: +0x150 = +336,  size = 0x8 = 8
        uint64_t populationMask;

        // Built buildings mask /
        // Маска построенных построек.
        // offset: +0x158 = +344,  size = 0x8 = 8
        uint64_t full_building_mask;

        // Available buildings mask /
        // Маска доступных построек.
        // offset: +0x160 = +352,  size = 0x8 = 8
        uint64_t legal_buildings;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Town map information /
// Информация о городе с карты.
// size = 0x88 = 136, align = 4
struct TownExtra
{
    // Map object reference index /
    // Ссылка(индекс) на объект карты приключений.
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t objRef;

    // Player owning this town /
    // Игрок, владеющий этим замком.
    // offset: +0x4 = +4,  size = 0x1 = 1
    int8_t playerOwner;

    // Has setup custom buildings /
    // Есть настроенные строения
    // offset: +0x5 = +5,  size = 0x1 = 1
    bool bCustomBuildings;

    // Built buildings mask /
    // Маска построенных построек.
    // offset: +0x8 = +8,  size = 0x8 = 8
    int64_t BuildingBuiltMask;

    // Disabled buildings mask /
    // Маска запрещённых построек.
    // offset: +0x10 = +16,  size = 0x8 = 8
    int64_t BuildingDisabledMask;

    // Town has fort /
    // У города есть форт.
    // offset: +0x18 = +24,  size = 0x1 = 1
    bool HasFort;

    // Town has setup custom garrison army /
    // У города настроен гарнизон.
    // offset: +0x19 = +25,  size = 0x1 = 1
    bool bCustomArmies;

    // Town garrison army(empty if not setup) /
    // Гарнизон города(пустой, если не настроен).
    // offset: +0x1C = +28,  size = 0x38 = 56
    armyGroup townArmy;

    // Town has setup custom name /
    // У города настроено название.
    // offset: +0x54 = +84,  size = 0x1 = 1
    bool bCustomName;

    // Town custom name(empty if not setup) /
    // Название города(пустое, если не настроено).
    // offset: +0x58 = +88,  size = 0x10 = 16
    exe_string cName;

    // Town fraction(type) /
    // Фракция (тип) города.
    // offset: +0x68 = +104,  size = 0x4 = 4
    TTownType townType;

    // Town garrison army is in groupped formation /
    // Гарнизон города в уплотнённой тактической стойке.
    // offset: +0x6C = +108,  size = 0x1 = 1
    bool bIsGrouped;

protected:
    NH3API_MAYBE_UNUSED
    byte_t gap_6D[3];

public:

    // Disabled spells mask /
    // Маска заклинаний, которые никогда не появятся в городе
    // offset: +0x70 = +112,  size = 0x9 = 9
    exe_bitset<MAX_BOOK_SPELLS> SpellDisabledMask;

    // Guatanteed spells mask /
    // Маска заклинаний, которые точно появятся в городе.
    // offset: +0x7C = +124,  size = 0x9 = 9
    exe_bitset<MAX_BOOK_SPELLS> SpellMask;

};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END