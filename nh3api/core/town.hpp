//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "resources/resources.hpp" // EGameResource
#include "creatures.hpp" // TCreatureType, armyGroup
#include "spells.hpp" // SpellID

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

struct bitNumber_impl_t
{
    #if NH3API_STD_STATIC_SUBSCRIPT_OPERATOR
    static
    #endif
    inline NH3API_CONSTEXPR uint64_t operator[](const size_t pos) 
    #if !NH3API_STD_STATIC_SUBSCRIPT_OPERATOR
    const
    #endif
    { return 1ULL << pos; }
} 
#ifdef NH3API_STD_INLINE_VARIABLES
inline constexpr
#endif
// Bit shift masks lookup table /
// Таблица побитовых масок.
const bitNumber;

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
    HALL_VILLAGE_ID     = 10, // Городская управа.
    HALL_TOWN_ID        = 11, // Префектура.
    HALL_CITY_ID        = 12, // Муниципалитет.
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

    const_village_hall  = 10, // Городская управа.
    const_town_hall     = 11, // Префектура.
    const_city_hall     = 12, // Муниципалитет.
    const_capitol_hall  = 13, // Капитолий.
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
    const_shipyard      = 6,  // Верфь.

    BUILDING_MAGE_GUILD_1       = 0,  // Гильдия магов 1 уровня
    BUILDING_MAGE_GUILD_2       = 1,  // Гильдия магов 2 уровня
    BUILDING_MAGE_GUILD_3       = 2,  // Гильдия магов 3 уровня
    BUILDING_MAGE_GUILD_4       = 3,  // Гильдия магов 4 уровня
    BUILDING_MAGE_GUILD_5       = 4,  // Гильдия магов 5 уровня
    BUILDING_TAVERN             = 5,  // Таверна
    BUILDING_DOCK               = 6,  // Верфь
    BUILDING_FORT               = 7,  // Форт
    BUILDING_CITADEL            = 8,  // Цитадель
    BUILDING_CASTLE             = 9,  // Замок
    BUILDING_HALL_VILLAGE       = 10, // Городская управа.
    BUILDING_HALL_TOWN          = 11, // Префектура.
    BUILDING_HALL_CITY          = 12, // Муниципалитет.
    BUILDING_HALL_CAPITOL       = 13, // Капитолий.
    BUILDING_HALL_MARKETPLACE   = 14, // Рынок
    BUILDING_HALL_RESOURCE_SILO = 15, // Хранилище ресурсов
    BUILDING_BLACKSMITH         = 16, // Кузница
    BUILDING_SPECIAL            = 17, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    BUILDING_HORDE              = 18, // Орда (увеличивает прирост определенного существа)
    BUILDING_HORDE_UPGRAGE      = 19, // Is being built altogether with HORDE_ID / Строится одновременно с HORDE_ID.
    BUILDING_DOCK_WITH_BOAT     = 20, // Верфь с кораблём(TODO: выяснить технические подробности)
    BUILDING_EXTRA_0            = 21, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    BUILDING_EXTRA_1            = 22, // Unique building for every town type / Специальное строение города(см. аналогичные ID для каждой фракции).
    BUILDING_EXTRA_2            = 23, // Decorative(upon building village, so always present) / Декорация(появляется сразу)
    BUILDING_HORDE_2            = 24, // Second horde building(if present) / Вторая орда(если есть)
    BUILDING_HORDE_2_UPGRADE    = 25, // Is being built altogether with HORDE_2_ID / Строится одновременно с HORDE_2_ID.
    BUILDING_HOLY_GRAIL         = 26, // Постройка Грааля
    BUILDING_EXTRA_3            = 27, // Decorative(upon building town, so always present) / Декорация(появляется сразу)
    BUILDING_EXTRA_4            = 28, // Decorative / Декор.
    BUILDING_EXTRA_5            = 29, // Decorative / Декор.
    BUILDING_DWELLING_0         = 30, // Lvl. 1 creature dwelling / Жилище существ 1 уровня.
    BUILDING_DWELLING_1         = 31, // Lvl. 2 creature dwelling / Жилище существ 2 уровня
    BUILDING_DWELLING_2         = 32, // Lvl. 3 creature dwelling / Жилище существ 3 уровня
    BUILDING_DWELLING_3         = 33, // Lvl. 4 creature dwelling / Жилище существ 4 уровня
    BUILDING_DWELLING_4         = 34, // Lvl. 5 creature dwelling / Жилище существ 5 уровня
    BUILDING_DWELLING_5         = 35, // Lvl. 6 creature dwelling / Жилище существ 6 уровня
    BUILDING_DWELLING_6         = 36, // Lvl. 7 creature dwelling / Жилище существ 7 уровня
    BUILDING_DWELLING_0_UPGRADE = 37, // Lvl. 1 upgraded creature dwelling / Улучшенное жилище существ 1 уровня
    BUILDING_DWELLING_1_UPGRADE = 38, // Lvl. 2 upgraded creature dwelling / Улучшенное жилище существ 2 уровня
    BUILDING_DWELLING_2_UPGRADE = 39, // Lvl. 3 upgraded creature dwelling / Улучшенное жилище существ 3 уровня
    BUILDING_DWELLING_3_UPGRADE = 40, // Lvl. 4 upgraded creature dwelling / Улучшенное жилище существ 4 уровня
    BUILDING_DWELLING_4_UPGRADE = 41, // Lvl. 5 upgraded creature dwelling / Улучшенное жилище существ 5 уровня
    BUILDING_DWELLING_5_UPGRADE = 42, // Lvl. 6 upgraded creature dwelling / Улучшенное жилище существ 6 уровня
    BUILDING_DWELLING_6_UPGRADE = 43, // Lvl. 7 upgraded creature dwelling / Улучшенное жилище существ 7 уровня
};

NH3API_FORCEINLINE const char* GetBuildingName(TTownType townType, type_building_id buildingId)
{ return FASTCALL_2(const char*, 0x460CC0, townType, buildingId); }

NH3API_INLINE_OR_EXTERN
// Town possible buildings mask /
// Маска возможных построек в городе(вообще).
std::array<uint64_t, kNumTowns>&
gTownEligibleBuildMask NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x697740, std::array<uint64_t, kNumTowns>));

NH3API_INLINE_OR_EXTERN
// Each building dependency mask /
// Маска необходимых построек для строительства каждой постройки каждой фракции.
std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>&
gHierarchyMask NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6977E8, std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>));

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
        bool HasGarrison() const
        { return !!THISCALL_1(bool32_t, 0x5BE3E0, this); }

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
        void get_build_cost(type_building_id building, std::array<int32_t, 7>& resources) const
        { THISCALL_3(void, 0x5C14F0, this, building, resources.data()); }

        // Get <building> cost as pointer to array of 7 ints /
        // Получить массив ресурсов, необходимый для постройки <building>.
        const std::array<int32_t, 7>& get_build_cost_array(type_building_id building) const
        {   
            typedef std::array<int32_t, 7> build_cost_array_t;
            return *THISCALL_2(const build_cost_array_t*, 0x5C1480, this, building); 
        }

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

NH3API_SIZE_ASSERT(0x168, town);

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

NH3API_SIZE_ASSERT(0x88, TownExtra);

NH3API_DISABLE_WARNING_END
