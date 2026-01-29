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

#include "artifact.hpp"            // TArtifact, type_artifact
#include "creatures.hpp"           // TCreatureType, armyGroup
#include "nh3api_std/point.hpp"    // TPoint
#include "player_enums.hpp"        // EPlayerColor
#include "random.hpp"              // Random(int, int)
#include "resources/files.hpp"     // TAbstractFile
#include "resources/resources.hpp" // EGameResource
#include "skills.hpp"              // TSkillMastery, TSecondarySkill
#include "terrain.hpp"             // type_point

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#pragma pack(push, 4)
// Map object info. Width(1 byte), Height(1 byte), PlacementMask(6 bytes), ShadowMask(6 bytes) are gathered from the corresponding <ImageName>.msk file,
// while PassableMask, TriggerMask, Type, Subtype, IsUnderlay are loaded from the map(see NewfullMap::readObjectType) /
// Информация об объекте на карте.
// Такие поля как Width(1 байт), Height(1 байт), PlacementMask(6 байт), ShadowMask(6 байт)
// загружаются из соответсвующего файла <ImageName>.msk,
// В то время такие поля как PassableMask, TriggerMask, Type, Subtype, IsUnderlay заполняются при загрузке файла карты.
// size = 0x44 = 68, align = 4
struct CObjectType
{
    public:
        // Sprite file name /
        // Название файла спрайта объекта.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string ImageName;

        // Object width(in cells). For example, hillfort has width = 3 /
        // Ширина изображения объекта(в клетках). Например, хиллфорт имеет ширину = 3.
        // offset: +0x10 = +16,  size = 0x1 = 1
        int8_t Width;

        // Object height(in cells). For example, hillfort has heigth = 3 /
        // Высота объекта(в клетках). Например, хиллфорт имеет высоту = 3.
        // offset: +0x11 = +17,  size = 0x1 = 1
        int8_t Height;

        unsigned char : 8;
        unsigned char : 8;

        // Draw mask. For example, hilfort(AVXhilg0.msk) has the following placement mask:
        // 00000000
        // 00000000
        // 00000000
        // 11000000
        // 11000000
        // 11100000
        // Note that every 2 last bits in each byte are ignored(the last and second-to-last columns).
        // If you write this 48-bit mask in 6 rows, omit the last and second-to-last columns and then (visually) horizontally rotate it,
        // it will give you a 6x6 map of cells which will be drawn(= 1). There are no pixels of this object in zero cells.
        // 000000
        // 000000
        // 000000
        // 000011
        // 000011
        // 000111
        // You can place any object in map editor, turn on the grid and
        // see it yourself, zero cells has no pixels on it! /
        /////////////////////////////////////////////////////////////////////////////////////
        // Маска отрисовки. Например, хиллфорт(AVXhilg0.msk) имеет следующую маску отрисовки:
        // 00000000
        // 00000000
        // 00000000
        // 11000000
        // 11000000
        // 11100000
        // При этом, 2 каждых бита в каждом байте игнорируются(последний и предпоследний столбцы).
        // Если записать эту 48-битную маску в 6 строк (visually), отсеять последние два столбца и повернуть (визуально) маску горизонтально,
        // вы увидите 6x6 карту отрисовки по каждым клеткам(= 1). В нулевых клетках пикселей (этого) объекта нет.
        // 000000
        // 000000
        // 000000
        // 000011
        // 000011
        // 000111
        // Вы можете убедится в этом сами. Поставьте объект на карту, включите сетку.
        // Там, где значение клетки = 1, есть пиксели объекта.
        // offset: +0x14 = +20,  size = 0x8 = 8
        exe_bitset<48> PlacementMask;

        // Passable cells mask. (= 1 empty cell, = 0 unpassable cell).
        // Use PassableMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
        // Маска клеток проходимости (= 1 пустая клетка, = 0 непроходимая клетка).
        // Используйте PassableMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
        // offset: +0x1C = +28,  size = 0x8 = 8
        exe_bitset<48> PassableMask;

        // Shadow draw mask. Similiar to PlacementMask but for the shadow /
        // Маска отрисовки тени. Аналогично PlacementMask, но для тени.
        // offset: +0x24 = +36,  size = 0x8 = 8
        exe_bitset<48> ShadowMask;

        // Trigger mask. (= 1 trigger cell, = 0 non-trigger cell).
        // Use TriggerMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
        // Маска триггерных(/входов) клеток. (= 1 клетка-триггер, = 0 клетка без триггера).
        // Используйте TriggerMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
        // offset: +0x2C = +44,  size = 0x8 = 8
        exe_bitset<48> TriggerMask;

        // Terrain availability mask(1 = object can be placed on that terrain) /
        // Маска доступности на почве(1 = объект можно установить на почве).
        // offset: +0x34 = +52,  size = 0x4 = 4
        exe_bitset<10> TerrainMask;

        // Object type /
        // Тип объекта.
        // offset: +0x38 = +56,  size = 0x4 = 4
        TAdventureObjectType Type;

        // Object subtype /
        // Подтип объекта.
        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t Subtype;

        // Drawn underlying? (as part of the ground, below other objects) /
        // Является ли частью земли?
        // offset: +0x40 = +64,  size = 0x1 = 1
        bool IsUnderlay;

        unsigned char : 8;

        // offset: +0x42 = +66,  size = 0x2 = 2
        int16_t objectTypeIndex;

} NH3API_MSVC_LAYOUT;

// Map editor struct, also used by the RMG. Object information /
// Структура из редактора карт. Также используется RMG. Информация об объекте.
// size 0x4C = 76, align = 4
struct TObjectType
{
public:
    // Index in sprite filenames tree(TUniqueSet<std::string> gImageNameSet) /
    // Индекс названия объекта в дереве названий спрайтов(TUniqueSet<std::string> gImageNameSet)
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t imageNum;

    // Passable cells mask. (= 1 empty cell, = 0 unpassable cell).
    // Use PassableMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
    // Маска клеток проходимости (= 1 пустая клетка, = 0 непроходимая клетка).
    // Используйте PassableMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
    // offset: +0x4 = +4,  size = 0x8 = 8
    exe_bitset<48> passableMask;

    // Trigger mask. (= 1 trigger cell, = 0 non-trigger cell).
    // Use TriggerMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
    // Маска триггерных(/входов) клеток. (= 1 клетка-триггер, = 0 клетка без триггера).
    // Используйте TriggerMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
    // offset: +0xC = +12,  size = 0x8 = 8
    exe_bitset<48> triggerMask;

    // Terrain availability mask(1 = object can be placed on that terrain) /
    // Маска доступности на почве(1 = объект можно установить на почве).
    // offset: +0x14 = +20,  size = 0x4 = 4
    exe_bitset<10> terrainMask;

    // Recommended terrain mask.
    // In the Map Editor or RMG this object will appear on terrains with value = 1 /
    // Маска родных для объекта почв. В редакторе карт и при генерации карты RMG
    // этот объект появляется на тех почвах, где значение = 1.
    // offset: +0x18 = +24,  size = 0x4 = 4
    exe_bitset<10> terrainRecommendedMask;

    // Object type /
    // Тип объекта.
    // offset: +0x1C = +28,  size = 0x4 = 4
    TAdventureObjectType type;

    // Object subtype /
    // Подтип объекта.
    // offset: +0x20 = +32,  size = 0x4 = 4
    int32_t subtype;

    // Slot category in map editor. /
    // Категория слота в редакторе карт.
    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t slotCategory;

    // Drawn underlying? (as part of the ground, below other objects) /
    // Является ли частью земли?
    // offset: +0x28 = +40,  size = 0x1 = 1
    bool isUnderlay;

    // Has any trigger cells? /
    // Имеет хотя бы одну триггерную клетку?
    // offset: +0x29 = +41,  size = 0x1 = 1
    bool hasTrigger;

    unsigned char : 8;
    unsigned char : 8;

    // The first trigger cell. (Start from the lower right and iterate to the left, from bottom to top) /
    // If there are no such cells, _triggerCell = {8, 6}.
    // Самая первая триггерная клетка. (Начиная с нижней правой клетки, итерируя влево, в конце строки на верх).
    // Если такой клетки нет, _triggerCell = {8, 6}.
    // offset: +0x2C = +44,  size = 0x8 = 8
    TPoint triggerCell;

    // Object image info /
    // Информация об изображении объекта.
    // offset: +0x34 = +52,  size = 0x18 = 24
    struct TImageInfo
    {
        // Object sprite dimensions(measured in cells). (objSize.x = width, objSize.y = height) /
        // Размер изображения объекта в клетках(objSize.x = ширина, objSize.y = высота).
        // offset: +0x0 = +0,  size = 0x8 = 8
        TPoint objSize;

        // Same as CObjectType::PlacementMask /
        // Маска отрисовки. Аналогично CObjectType::PlacementMask
        // offset: +0x8 = +8,  size = 0x8 = 8
        exe_bitset<48> drawMask;

        // Same as CObjectType::ShadowMask /
        // Маска тени. Аналогично CObjectType::ShadowMask
        // offset: +0x10 = +16,  size = 0x8 = 8
        exe_bitset<48> shadowMask;

    } imageInfo;
};

[[nodiscard]] inline constexpr uint32_t getBitPos(uint32_t x, uint32_t y) noexcept
{ return 8 * (6 - y) - x - 1; }

// Artifact/resource custom data /
// Настройки артефакта или ресурса на карте.
// size = 0x4C = 76, align = 4
struct TreasureData
{
    public:
        inline TreasureData() noexcept                          = default;
        inline TreasureData(const TreasureData&)                = default;
        inline TreasureData(TreasureData&&) noexcept            = default;
        inline TreasureData& operator=(const TreasureData&)     = default;
        inline TreasureData& operator=(TreasureData&&) noexcept = default;
        inline ~TreasureData() noexcept                         = default;

        inline TreasureData(const nh3api::dummy_tag_t& tag) noexcept
            : Message { tag }, Guardians { tag }
        {}

    public:
        // Displayed message upon gathering /
        // Сообщение, выводимое при собирании ресурса/артефакта.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message {};

        // Has custom guards? /
        // Настроена охрана?
        // offset: +0x10 = +16,  size = 0x1 = 1
        bool HasCustomGuardians {false};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Guards /
        // Охрана.
        // offset: +0x14 = +20,  size = 0x38 = 56
        armyGroup Guardians {};

};

// Adventure map monster data /
// Информация о существе на карте.
// size = 0x30 = 48, align = 4
struct MonsterData
{
    public:
        inline constexpr MonsterData() noexcept = default;

        inline MonsterData(const nh3api::dummy_tag_t& tag) noexcept
            : Message { tag }
        {}

        inline ~MonsterData() noexcept                        = default;
        inline MonsterData(const MonsterData&)                = default;
        inline MonsterData(MonsterData&&) noexcept            = default;
        inline MonsterData& operator=(const MonsterData&)     = default;
        inline MonsterData& operator=(MonsterData&&) noexcept = default;

    public:
        // Attack message /
        // Сообщение при атаке существа.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message {};

        // Resources bonus upon defeat /
        // Ресурсы, которые можно получить при победе над существами.
        // offset: +0x10 = +16,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty {};

        // Artifact bonus upon defeat /
        // Артефакт, который можно получить при победе над существами.
        // offset: +0x2C = +44,  size = 0x4 = 4
        TArtifact Artifact {ARTIFACT_NONE};

};

// Black Box(aka Pandora's Box) and Event data /
// Информация о ящике пандоры и событии.
// size = 0xE4 = 228, align = 4, baseclass: TreasureData
struct BlackBoxData : TreasureData
{
    public:
        inline BlackBoxData() noexcept = default;

        inline BlackBoxData(const nh3api::dummy_tag_t& tag) noexcept
            : TreasureData(tag),
              SecondarySkills { tag },
              Artifacts { tag },
              Spells { tag },
              Creatures { tag }
        {}

        inline ~BlackBoxData() noexcept                         = default;
        inline BlackBoxData(const BlackBoxData&)                = default;
        inline BlackBoxData(BlackBoxData&&) noexcept            = default;
        inline BlackBoxData& operator=(const BlackBoxData&)     = default;
        inline BlackBoxData& operator=(BlackBoxData&&) noexcept = default;

    public:
        // Has custom treasure? /
        // Имеет настроенные ресурсы?
        // offset: +0x4C = +76,  size = 0x1 = 1
        bool HasCustomTreasure {false};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Experience Bonus(always positive) /
        // Бонус опыта(всегда положительный).
        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t ExperienceBonus {0};

        // Mana Bonus(always positive) /
        // Бонус маны(всегда положительный).
        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t ManaBonus {0};

        // Morale Bonus(can be negative) /
        // Бонус боевого духа(может быть отрицательным).
        // offset: +0x58 = +88,  size = 0x1 = 1
        int8_t MoraleBonus {0};

        // Luck Bonus(can be negative) /
        // Бонус удачи(может быть отрицательным).
        // offset: +0x59 = +89,  size = 0x1 = 1
        int8_t LuckBonus {0};

        unsigned char : 8;
        unsigned char : 8;

        // Bonus resources(can be negative) /
        // Бонусные ресурсы(может быть отрицательным).
        // offset: +0x5C = +92,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty {};

        // Primary skill bonuses(always positive) /
        // Бонусы первичных навыков(всегда положительные).
        // offset: +0x78 = +120,  size = 0x4 = 4
        std::array<int8_t, kNumPrimarySkills> PrimarySkillBonus {};

        // Bonus secondary skills /
        // Бонусные вторичные навыки.
        // offset: +0x7C = +124,  size = 0x10 = 16
        exe_vector<SecondarySkillData> SecondarySkills {};

        // Bonus artifacts /
        // Бонусные артефакты.
        // offset: +0x8C = +140,  size = 0x10 = 16
        exe_vector<TArtifact> Artifacts {};

        // Bonus spells /
        // Бонусные заклинания.
        // offset: +0x9C = +156,  size = 0x10 = 16
        exe_vector<SpellID> Spells {};

        // Bonus creatures that will join your army /
        // Бонусные существа, которые присоединятся к армии героя.
        // offset: +0xAC = +172,  size = 0x38 = 56
        armyGroup Creatures {};
};
#pragma pack(pop) // 4

// Original map editor names: (changed to not to make confusion with random map generator)
// TRandomlyLeveledGenerator,
// TRandomlyAlignedGenerator.

class CObject;
#pragma pack(push, 1)
// Random dwelling /
// Случайное жилище.
// size = 0x10 = 16, align = 4
class TRandomDwelling
{
    public:
        inline constexpr TRandomDwelling() noexcept = default;

        inline TRandomDwelling(const nh3api::dummy_tag_t&) noexcept
        {}

        inline ~TRandomDwelling() noexcept = default;

    public:
        // Town ID /
        // ID города.
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t townId {-1};

        // Allowed town types /
        // Указанные фракции жилища. Выбирается случайная из выбранных фракций.
        // offset: +0x4 = +4,  size = 0x4 = 4
        uint16_t towns {0};

        // Player owning this dwelling /
        // Игрок, владеющий этим жилищем.
        // offset: +0x6 = +6,  size = 0x1 = 1
        int8_t playerOwner {PLAYER_NONE};

        // Creature lowest level /
        // Наименьший уровень существа.
        // offset: +0x7 = +7,  size = 0x1 = 1
        uint8_t minLVL {0};

        // Creature largest level /
        // Наибольший уровень существа.
        // offset: +0x8 = +8,  size = 0x1 = 1
        uint8_t maxLVL {6};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Object /
        // Объект.
        // offset: +0xC = +12,  size = 0x4 = 4
        CObject* object {nullptr};
};
#pragma pack(pop) // 1

#pragma pack(push, 4)
// Black market.
// Чёрный рынок.
// size = 0x1C = 28, align = 4
class TBlackMarket
{
    public:
        inline constexpr TBlackMarket() noexcept = default;

        inline TBlackMarket(const nh3api::dummy_tag_t&) noexcept
        {}

    public:
        // Artifacts /
        // Артефакты.
        // offset: +0x0 = +0,  size = 0x1C = 28
        std::array<TArtifact, 7> artifacts
        {ARTIFACT_NONE, ARTIFACT_NONE, ARTIFACT_NONE, ARTIFACT_NONE, ARTIFACT_NONE, ARTIFACT_NONE, ARTIFACT_NONE};
};

// Creature dwelling /
// Жилище существ
// size = 0x5C = 92, align = 4
class generator
{
    public:
        inline generator() noexcept
            : generator(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4B8250, this); }

        inline generator(const nh3api::dummy_tag_t& tag) noexcept
            : guards { tag }
        {}

    public:
        [[nodiscard]] int32_t get_owner() const
        { return playerOwner; }

        // Read from game save file /
        // Загрузить из сохранения игры.
        bool load(TAbstractFile* infile)
        { return THISCALL_2(bool, 0x4B82A0, this, infile); }

        // Write to game save file /
        // Записать в сохранение игры.
        bool save(TAbstractFile* outfile) const
        { return THISCALL_2(bool, 0x4B83E0, this, outfile); }

        void update_bonus()
        { THISCALL_1(char, 0x4B84A0, this); }

        void Initialize(int32_t new_owner)
        { THISCALL_2(void, 0x4B8560, this, new_owner); }

        // Refresh creatures and grow guards /
        // Обновить существ и добавить охрану.
        void Grow()
        { THISCALL_1(void, 0x4B8760, this); }

    public:
        // Dwelling type(TAdventureObjectType) /
        // Тип жилища(TAdventureObjectType).
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t genClass;

        // Object subtype /
        // Подтип объекта.
        // offset: +0x1 = +1,  size = 0x1 = 1
        int8_t genType;

        unsigned char : 8;
        unsigned char : 8;

        // Creatures /
        // Существа.
        // offset: +0x4 = +4,  size = 0x10 = 16
        std::array<TCreatureType, 4> type;

        // Creature amounts /
        // Количества существ.
        // offset: +0x14 = +20,  size = 0x8 = 8
        std::array<int16_t, 4> population;

        // Guards /
        // Охрана.
        // offset: +0x1C = +28,  size = 0x38 = 56
        armyGroup guards;

        // cell x coordinate /
        // X клетки.
        // offset: +0x54 = +84,  size = 0x1 = 1
        uint8_t mapX;

        // cell y coordinate /
        // Y клетки
        // offset: +0x55 = +85,  size = 0x1 = 1
        uint8_t mapY;

        // cell z coordinate /
        // Z клетки.
        // offset: +0x56 = +86,  size = 0x1 = 1
        uint8_t mapZ;

        // Player owning this dwelling /
        // Игрок, владеющий этим жилищем.
        // offset: +0x57 = +87,  size = 0x1 = 1
        int8_t playerOwner;

        // Town ID that will specify this dwelling type /
        // ID города, от типа которого зависит тип жилища.
        // offset: +0x58 = +88,  size = 0x1 = 1
        int8_t town_id;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

};

// Mine or Lighthouse /
// Шахта или маяк.
// size = 0x40 = 64, align = 4
class mine
{
    public:
        inline constexpr mine() noexcept = default;

        inline mine(const nh3api::dummy_tag_t& tag) noexcept
            : guards { tag }
        {}

    public:
        // Player owning this object /
        // Игрок, владеющий этим объектом.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t playerOwner {PLAYER_NONE};

        // Resource created by this mine /
        // Ресурс, производимый шахтой.
        // offset: +0x1 = +1,  size = 0x1 = 1
        int8_t type {const_no_resource};

        // Is mine abandoned /
        // Шахта является заброшенной.
        // offset: +0x2 = +2,  size = 0x1 = 1
        bool is_abandoned {false};

        unsigned char : 8;

        // Mine guards /
        // Охраны шахты.
        // offset: +0x4 = +4,  size = 0x38 = 56
        armyGroup guards {};

        // Object x-coordinate /
        // x-координата объекта.
        // offset: +0x3C = +60,  size = 0x1 = 1
        uint8_t mapX {static_cast<uint8_t>(-1)};

        // Object y-coordinate /
        // y-координата объекта.
        // offset: +0x3D = +61,  size = 0x1 = 1
        uint8_t mapY {static_cast<uint8_t>(-1)};

        // Object z-coordinate /
        // z-координата объекта.
        // offset: +0x3E = +62,  size = 0x1 = 1
        uint8_t mapZ {static_cast<uint8_t>(-1)};

        unsigned char : 8;

};

// Garrison /
// Гарнизон.
// size = 0x40 = 64, align = 4
class garrison
{
    public:
        inline constexpr garrison() noexcept = default;

        inline garrison(const nh3api::dummy_tag_t& tag) noexcept
            : garrisonArmy { tag }
        {}

    public:
        // Player owning this garrison /
        // Игрок, владеющий этим гарнизоном.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t playerOwner {PLAYER_NONE};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Garrison army /
        // Армия гарнизона.
        // offset: +0x4 = +4,  size = 0x38 = 56
        armyGroup garrisonArmy {};

        // Army is removable /
        // Армию можно забрать.
        // offset: +0x3C = +60,  size = 0x1 = 1
        bool armyRemovable {false};

        // Object x-coordinate /
        // x-координата объекта.
        // offset: +0x3D = +61,  size = 0x1 = 1
        uint8_t mapX {static_cast<uint8_t>(-1)};

        // Object y-coordinate /
        // y-координата объекта.
        // offset: +0x3E = +62,  size = 0x1 = 1
        uint8_t mapY {static_cast<uint8_t>(-1)};

        // Object z-coordinate /
        // z-координата объекта.
        // offset: +0x3F = +63,  size = 0x1 = 1
        uint8_t mapZ {static_cast<uint8_t>(-1)};

};

// University /
// Университет.
// size = 0x10 = 16, align = 4
class type_university
{
    public:
        inline constexpr type_university() noexcept = default;

        inline type_university(const nh3api::dummy_tag_t&) noexcept
        {}

    public:
        // Secondary skills /
        // Вторичные навыки, предлагаемые университетом для изучения.
        // offset: +0x0 = +0,  size = 0x10 = 16
        std::array<TSecondarySkill, 4> skills
        {SKILL_NONE, SKILL_NONE, SKILL_NONE, SKILL_NONE};
};

//
// size = 0xC = 12, align = 4
struct type_creature_source
{
public:
    // offset: +0x0 = +0,  size = 0x4 = 4
    TCreatureType type;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int16_t* ptr;

    // offset: +0x8 = +8,  size = 0x2 = 2
    int16_t number;

    // offset: +0xA = +10,  size = 0x1 = 1
    bool is_free;

    unsigned char : 8;
};

// Creature bank /
// Банк существ.
// size = 0x6C = 108, align = 4
class type_creature_bank
{
    public:
        type_creature_bank() noexcept                                = default;
        type_creature_bank(const type_creature_bank&)                = default;
        type_creature_bank& operator=(const type_creature_bank&)     = default;
        type_creature_bank(type_creature_bank&&) noexcept            = default;
        type_creature_bank& operator=(type_creature_bank&&) noexcept = default;

        inline type_creature_bank(const nh3api::dummy_tag_t& tag) noexcept
            : guards { tag }, artifacts { tag }
        {}

        inline ~type_creature_bank() noexcept = default;

    public:
        // Bank guardians /
        // Охрана банка.
        // offset: +0x0 = +0,  size = 0x38 = 56
        armyGroup guards;

        // Resources aquired on victory /
        // Ресурсы, получаемые в качестве награды при зачистке банка.
        // offset: +0x38 = +56,  size = 0x1C = 28
        std::array<int32_t, 7> resources;

        // Creatures aquired on victory /
        // Существа, получаемые в качестве награды при зачистке банка.
        // offset: +0x54 = +84,  size = 0x4 = 4
        TCreatureType reward_creature;

        // Amount of creatures aquired on victory /
        // Кол-во существ, получаемых в качестве награды при зачистке банка.
        // offset: +0x58 = +88,  size = 0x1 = 1
        int8_t reward_creatures;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Artifacts aquired on victory /
        // Артефакты, получаемых в качестве награды при зачистке банка.
        // offset: +0x5C = +92,  size = 0x10 = 16
        exe_vector<type_creature_source> artifacts;

};

// Sign /
// Дорожный знак.
// size = 0x14 = 20, align = 4
class Sign
{
    public:
        inline Sign() noexcept = default;

        inline Sign(const nh3api::dummy_tag_t& tag) noexcept
            : signText { tag }
        {}

        inline ~Sign() noexcept = default;

    public:
        // Has customized text /
        // Есть настроенный текст.
        // offset: +0x0 = +0,  size = 0x4 = 4
        bool32_t hasText {false};

        // Custom text /
        // Настроенный текст.
        // offset: +0x4 = +4,  size = 0x10 = 16
        exe_string signText {};

};

struct MapIdInfo
{
    unsigned id           : 5;
    unsigned visited_bits : 8;
    unsigned unused       : 6;
    unsigned custom_index : 12;
    unsigned custom       : 1;
} NH3API_MSVC_LAYOUT;

// RoE Artifact pickup requirements /
// Типы требований для поднятия артефактов из RoE.
enum ArtifactPrices : uint32_t
{
    const_free_artifact                = 0,
    const_artifact_costs_2000          = 1,
    const_artifact_requires_wisdom     = 2,
    const_artifact_requires_leadership = 3,
    const_artifact_costs_2500          = 4,
    const_artifact_costs_3000          = 5,
    const_artifact_defended            = 6
};

struct MapArtifactInfo_RoE
{
    ArtifactPrices price          : 4;
    unsigned       guard          : 9;
    unsigned       resource_price : 4;
    unsigned       guard_qty      : 14;
    unsigned       custom         : 1;

} NH3API_MSVC_LAYOUT;

struct MapArtifactInfo
{
    ArtifactPrices price          : 4;
    unsigned       guard          : 9;
    unsigned       resource_price : 4;
    unsigned                      : 2;
    unsigned       guard_qty      : 12;
    unsigned       custom         : 1;

} NH3API_MSVC_LAYOUT;

struct CustomMapArtifactInfo
{
    unsigned unused : 19;
    unsigned index  : 12;
    unsigned custom : 1;
};

struct CampfireInfo
{
    unsigned resource : 4;
    unsigned amount   : 16;
    unsigned          : 12;

} NH3API_MSVC_LAYOUT;

struct SkeletonInfo_RoE
{
    unsigned id           : 5;
    unsigned unused       : 1;
    signed   artifact     : 7;
    unsigned has_treasure : 1;
    unsigned              : 15;

} NH3API_MSVC_LAYOUT;

struct SkeletonInfo
{
    unsigned  id           : 5;
    unsigned  unused       : 1;
    TArtifact artifact     : 10;
    unsigned  has_treasure : 1;
    unsigned               : 15;

} NH3API_MSVC_LAYOUT;

struct type_creature_bank_info
{
    unsigned unused       : 5;
    unsigned visited_bits : 8;
    unsigned index        : 12;
    unsigned empty        : 1;
    unsigned              : 6;

} NH3API_MSVC_LAYOUT;

struct EventInfo
{
    unsigned index              : 10;
    unsigned allow_player       : 8;
    unsigned allow_computer     : 1;
    unsigned cancel_after_visit : 1;
    unsigned                    : 12;

} NH3API_MSVC_LAYOUT;

struct FlotsamInfo
{
    enum : uint32_t {
        FLOTSAM_EMPTY = 0,
        FLOTSAM_WOOD5 = 1,
        FLOTSAM_WOOD5_GOLD200 = 2,
        FLOTSAM_WOOD10_GOLD500 = 3
    } type;
} NH3API_MSVC_LAYOUT;

struct FountainOfFortuneInfo
{
    unsigned unused       : 5;
    unsigned visited_bits : 8;
    signed   luck_bonus   : 4;
    unsigned              : 15;

} NH3API_MSVC_LAYOUT;

struct LeanToInfo
{
    unsigned id       : 5;
    unsigned unused   : 1;
    unsigned amount   : 4;
    unsigned resource : 4;
    unsigned          : 18;

} NH3API_MSVC_LAYOUT;

struct ShrineInfo
{
    unsigned unused       : 5;
    unsigned visited_bits : 8;
    unsigned spell        : 10;
    unsigned              : 9;

} NH3API_MSVC_LAYOUT;

struct MagicSpringInfo
{
    unsigned id     : 5;
    unsigned unused : 1;
    unsigned full   : 1;
    unsigned        : 25;

} NH3API_MSVC_LAYOUT;

struct MonsterInfo_RoE
{
    unsigned qty         : 12;
    unsigned disposition : 5;
    unsigned never_flee  : 1;
    unsigned dont_grow   : 1;
    unsigned index       : 12;
    unsigned custom      : 1;

} NH3API_MSVC_LAYOUT;

struct MonsterInfo
{
    unsigned qty              : 12;
    unsigned disposition      : 5;
    unsigned never_flee       : 1;
    unsigned dont_grow        : 1;
    unsigned index            : 8;
    unsigned growth_remainder : 4;
    unsigned custom           : 1;

} NH3API_MSVC_LAYOUT;

struct MysticGardenInfo
{
    unsigned id       : 5;
    unsigned unused   : 1;
    unsigned resource : 4;
    unsigned full     : 1;
    unsigned          : 21;

} NH3API_MSVC_LAYOUT;

struct BlackBoxInfo
{
    unsigned index : 10;
    unsigned       : 22;

} NH3API_MSVC_LAYOUT;

struct type_pyramid_info
{
    unsigned guarded      : 1;
    unsigned unused       : 4;
    unsigned visited_bits : 8;
    unsigned spell        : 8;
    unsigned              : 11;

} NH3API_MSVC_LAYOUT;

struct CustomResourceInfo
{
    unsigned qty    : 19;
    unsigned index  : 12;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

enum ScholarAwards : uint32_t
{
    const_scholar_primary_skill   = 0,
    const_scholar_secondary_skill = 1,
    const_scholar_spell           = 2
};

struct ScholarInfo
{
    ScholarAwards   award_type      : 3;
    unsigned        primary_skill   : 3;
    TSecondarySkill secondary_skill : 7;
    unsigned        spell           : 10;
    unsigned                        : 9;

} NH3API_MSVC_LAYOUT;

struct CustomSpellScrollInfo
{
    unsigned spell  : 8;
    unsigned unused : 11;
    unsigned index  : 12;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

enum SeaChestRewardTypes : uint32_t
{
    const_sea_chest_nothing  = 0,
    const_sea_chest_gold     = 1,
    const_sea_chest_artifact = 2
};

struct SeaChestInfo_RoE
{
    SeaChestRewardTypes reward   : 3;
    signed              artifact : 8;
    unsigned                     : 19;

} NH3API_MSVC_LAYOUT;

struct SeaChestInfo
{
    SeaChestRewardTypes reward   : 3;
    TArtifact           artifact : 10;
    unsigned                     : 19;

} NH3API_MSVC_LAYOUT;

struct ShipyardInfo
{
    unsigned owner : 8;
    unsigned boatX : 8;
    unsigned boatY : 8;
    unsigned       : 8;

} NH3API_MSVC_LAYOUT;

struct TreasureInfo_RoE
{
    signed   artifact    : 8;
    unsigned is_artifact : 1;
    unsigned gold_amount : 4;
    unsigned             : 19;

} NH3API_MSVC_LAYOUT;

struct TreasureInfo
{
    TArtifact artifact    : 10;
    unsigned  is_artifact : 1;
    unsigned  gold_amount : 4;
    unsigned              : 17;

} NH3API_MSVC_LAYOUT;

enum WiseTreePrices : uint32_t
{
    const_tree_wants_nothing = 0,
    const_tree_wants_gold    = 1,
    const_tree_wants_gems    = 2,
    const_tree_price_count   = 3
};

struct WiseTreeInfo
{
    unsigned       id           : 5;
    unsigned       visited_bits : 8;
    WiseTreePrices price        : 2;
    unsigned                    : 17;

} NH3API_MSVC_LAYOUT;

struct type_university_info
{
    unsigned unused       : 5;
    unsigned visited_bits : 8;
    unsigned index        : 12;
    unsigned              : 7;

} NH3API_MSVC_LAYOUT;

struct WagonInfo_RoE
{
    unsigned resource_amount : 5;
    unsigned visited_bits    : 8;
    unsigned full            : 1;
    unsigned has_artifact    : 1;
    signed   artifact        : 8;
    unsigned resource        : 4;
    unsigned                 : 5;

} NH3API_MSVC_LAYOUT;

struct WagonInfo
{
    unsigned  resource_amount : 5;
    unsigned  visited_bits    : 8;
    unsigned  full            : 1;
    unsigned  has_artifact    : 1;
    TArtifact artifact        : 10;
    unsigned  resource        : 4;
    unsigned                  : 3;

} NH3API_MSVC_LAYOUT;

struct TombInfo_RoE
{
    unsigned full         : 1; // есть артефакт
    unsigned unused       : 4;
    unsigned visited_bits : 8; // посещение каждым из игроков
    signed   artifact     : 8;
    unsigned              : 11;

} NH3API_MSVC_LAYOUT;

struct TombInfo
{
    unsigned  full         : 1; // есть артефакт
    unsigned  unused       : 4;
    unsigned  visited_bits : 8; // посещение каждым из игроков
    TArtifact artifact     : 10;
    unsigned               : 9;

} NH3API_MSVC_LAYOUT;

struct WaterWheelInfo
{
    unsigned amount       : 5;
    unsigned visited_bits : 8;
    unsigned              : 19;

} NH3API_MSVC_LAYOUT;

struct WindMillInfo
{
    unsigned resource     : 4; // тип ресурса
    unsigned unused       : 1;
    unsigned visited_bits : 8;
    unsigned amount       : 4; // кол-во ресурса
    unsigned              : 15;

} NH3API_MSVC_LAYOUT;

struct WitchHutInfo
{
    unsigned        unused       : 5;
    unsigned        visited_bits : 8;
    TSecondarySkill skill        : 7; // тип вторичного навыка
    unsigned                     : 12;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

#pragma pack(push, 1)
// Map object extra info /
// Дополнительная информация об объекте на карте.
// size = 0x4 = 4, align = 1.
struct ExtraInfoUnion
{
    public:
        [[nodiscard]] BlackBoxData* get_black_box()
        { return THISCALL_1(BlackBoxData*, 0x405D70, this); }

        [[nodiscard]] const BlackBoxData* get_black_box() const
        { return THISCALL_1(BlackBoxData*, 0x405D70, this); }

        [[nodiscard]] type_creature_bank* get_creature_bank()
        { return THISCALL_1(type_creature_bank*, 0x405D80, this); }

        [[nodiscard]] const type_creature_bank* get_creature_bank() const
        { return THISCALL_1(type_creature_bank*, 0x405D80, this); }

        [[nodiscard]] type_university* get_university()
        { return THISCALL_1(type_university*, 0x405DA0, this); }

        [[nodiscard]] const type_university* get_university() const
        { return THISCALL_1(type_university*, 0x405DA0, this); }

        [[nodiscard]] int16_t GetItemId() const noexcept
        { return static_cast<int16_t>(item_id.id); }

        [[nodiscard]] SpellID GetShrineSpell() const noexcept
        { return static_cast<SpellID>(shrine_info.spell); }

        [[nodiscard]] WiseTreePrices GetTreePrice() const noexcept
        { return tree_info.price; }

        [[nodiscard]] int16_t get_wheel_gold() const noexcept
        { return static_cast<int16_t>(water_wheel_info.amount); }

        [[nodiscard]] int16_t get_windmill_amount() const noexcept
        { return static_cast<int16_t>(wind_mill_info.amount); }

        [[nodiscard]] TSecondarySkill get_witch_skill() const noexcept
        { return witch_hut_info.skill; }

        // Is Artifact/Monster/Resource/Spell Scroll customized? /
        // Настроен ли текущий Артефакт/Монстр/Ресурс/Свиток Заклинания?
        [[nodiscard]] bool IsCustomized() const noexcept
        { return static_cast<bool>(item_id.custom); }

        [[nodiscard]] TCreatureType GetArtifactDefender() const noexcept
        { return static_cast<TCreatureType>(artifact_info.guard); }

        // Get RoE mode artifact price /
        // Цена артефакта на картах RoE.
        [[nodiscard]] ArtifactPrices GetArtifactPrice() const noexcept
        { return artifact_info.price; }

        [[nodiscard]] EGameResource GetArtifactResourceCost() const noexcept
        { return static_cast<EGameResource>(artifact_info.resource_price); }

        [[nodiscard]] int16_t GetCampfireSize() const noexcept
        { return static_cast<int16_t>(campfire_info.amount); }

        [[nodiscard]] EGameResource GetCampfireResource() const noexcept
        { return static_cast<EGameResource>(campfire_info.resource); }

        void SetLeanTo(int16_t id, int16_t amount, EGameResource resource) noexcept
        {
            assert(id < 32);
            assert(amount >= 0 && amount < 16);
            assert(resource >= WOOD && resource <= GOLD);
            lean_to_info.id       = static_cast<uint8_t>(id)       & 0b11111U;
            lean_to_info.amount   = static_cast<uint8_t>(amount)   & 0b1111U;
            lean_to_info.resource = static_cast<uint8_t>(resource) & 0b1111U;
        }

        [[nodiscard]] int16_t GetLeanToAmount() const noexcept
        { return static_cast<int16_t>(lean_to_info.amount); }

        [[nodiscard]] EGameResource GetLeanToResource() const noexcept
        { return static_cast<EGameResource>(lean_to_info.resource); }

        [[nodiscard]] bool MagicSpringIsFull() const noexcept
        { return static_cast<bool>(magic_spring_info.full); }

        void FillMagicSpring(bool full) noexcept
        { magic_spring_info.full = full; }

        [[nodiscard]] bool GardenIsFull() const noexcept
        { return static_cast<bool>(mystic_garden_info.full); }

        [[nodiscard]] EGameResource GetGardenResource() const noexcept
        { return static_cast<EGameResource>(mystic_garden_info.resource); }

        void SetGardenEmpty() noexcept
        { mystic_garden_info.full = false; }

        [[nodiscard]] SpellID get_pyramid_spell() const noexcept
        { return static_cast<SpellID>(pyramid_info.spell); }

        [[nodiscard]] bool pyramid_is_guarded() const noexcept
        { return static_cast<bool>(pyramid_info.guarded); }

        void set_pyramid(bool guards, SpellID new_spell) noexcept
        {
            assert(new_spell >= FIRST_BOOK_SPELL && new_spell <= LAST_BOOK_SPELL);
            pyramid_info.guarded = guards;
            pyramid_info.spell   = static_cast<uint8_t>(new_spell);
        }

        [[nodiscard]] ScholarAwards GetScholarAward() const noexcept
        { return scholar_info.award_type; }

        [[nodiscard]] TPrimarySkill GetScholarPrimarySkill() const noexcept
        { return static_cast<TPrimarySkill>(scholar_info.primary_skill); }

        [[nodiscard]] TSecondarySkill GetScholarSecondarySkill() const noexcept
        { return scholar_info.secondary_skill; }

        [[nodiscard]] SpellID GetScholarSpell() const noexcept
        { return static_cast<SpellID>(scholar_info.spell); }

        [[nodiscard]] bool SkeletonHasTreasure() const noexcept
        { return static_cast<bool>(skeleton_info.has_treasure); }

        [[nodiscard]] TArtifact GetSkeletonArtifact() const noexcept
        { return skeleton_info.artifact; }

        void SetSkeleton(int16_t id, bool has_treasure, TArtifact artifact) noexcept
        {
            assert(id < 32);
            assert(artifact >= FIRST_ARTIFACT && artifact <= LAST_ARTIFACT);
            skeleton_info.id           = static_cast<uint8_t>(id);
            skeleton_info.artifact     = artifact;
            skeleton_info.has_treasure = has_treasure;
        }

        [[nodiscard]] SeaChestRewardTypes GetSeaChestReward() const noexcept
        { return sea_chest_info.reward; }

        [[nodiscard]] TArtifact GetSeaChestArtifact() const noexcept
        { return sea_chest_info.artifact; }

        [[nodiscard]] TArtifact GetTreasureArtifact() const noexcept
        { return treasure_info.artifact; }

        [[nodiscard]] int16_t GetTreasureSize() const noexcept
        { return static_cast<int16_t>(treasure_info.gold_amount) * 500; }

        [[nodiscard]] bool TreasureIsArtifact() const noexcept
        { return static_cast<bool>(treasure_info.is_artifact); }

        void EmptyWagon() noexcept
        { wagon_info.full = false; }

        [[nodiscard]] int16_t GetWagonAmount() const noexcept
        { return static_cast<int16_t>(wagon_info.resource_amount); }

        [[nodiscard]] TArtifact GetWagonArtifact() const noexcept
        { return wagon_info.artifact; }

        [[nodiscard]] EGameResource GetWagonResource() const noexcept
        { return static_cast<EGameResource>(wagon_info.resource); }

        [[nodiscard]] bool WagonHasArtifact() const noexcept
        { return wagon_info.has_artifact; }

        [[nodiscard]] bool WagonIsFull() const noexcept
        { return wagon_info.full; }

        void empty_tomb() noexcept
        { tomb_info.full = false; }

        [[nodiscard]] TArtifact get_tomb_artifact() const noexcept
        { return tomb_info.artifact; }

        [[nodiscard]] bool tomb_is_full() const noexcept
        { return static_cast<bool>(tomb_info.full); }

        void set_wheel_gold(int16_t amount) noexcept
        {
            switch ( amount )
            {
                case 0:
                    water_wheel_info.amount = 0;
                    return;
                case 500:
                    water_wheel_info.amount = 1;
                    return;
                case 1000:
                    water_wheel_info.amount = 2;
                    return;
                default:
                    return;
            }
        }

        [[nodiscard]] EGameResource get_windmill_resource() const noexcept
        { return static_cast<EGameResource>(wind_mill_info.resource); }

        void set_windmill(EGameResource resource, int8_t amount) noexcept
        {
            assert(resource >= WOOD && resource <= GOLD);
            assert(amount >= 0 && amount < 16);
            wind_mill_info.resource = static_cast<uint8_t>(resource) & 0b1111U;
            wind_mill_info.amount   = static_cast<uint8_t>(amount)   & 0b1111U;
        }

        void clear_visited_bits() noexcept
        { item_id.visited_bits = 0; }

        void FillGarden(EGameResource resource) noexcept
        {
            assert(resource >= WOOD && resource <= GOLD);
            mystic_garden_info.resource = static_cast<uint8_t>(resource) & 0b1111U;
            mystic_garden_info.full     = true;
        }

        // Set object visited by <player> /
        // Пометить объект клетки посещенным игроком <player>.
        void SetCellVisited(int16_t player)
        { THISCALL_2(void, 0x4FC620, this, player); }

        void SetGarden(int16_t id, EGameResource resource) noexcept
        {
            assert(id < 32);
            assert(resource >= WOOD && resource <= GOLD);
            mystic_garden_info.id       = static_cast<uint8_t>(id)       & 0b11111U;
            mystic_garden_info.resource = static_cast<uint8_t>(resource) & 0b1111U;
            mystic_garden_info.full     = true;
        }

        void SetMagicSpring(int16_t id, bool full) noexcept
        {
            assert(id < 32);
            magic_spring_info.id   = static_cast<uint8_t>(id) & 0b11111U;
            magic_spring_info.full = full;
        }

        void SetScholar(ScholarAwards award, TPrimarySkill primary, TSecondarySkill secondary, SpellID spell) noexcept
        {
            assert(award == const_scholar_primary_skill || award == const_scholar_secondary_skill || award == const_scholar_spell);
            assert(primary >= FIRST_PRIMARY_SKILL && primary <= LAST_PRIMARY_SKILL);
            assert(secondary >= FIRST_SECONDARY_SKILL && secondary <= LAST_SECONDARY_SKILL);
            assert(spell >= FIRST_BOOK_SPELL && spell <= LAST_BOOK_SPELL);
            scholar_info.award_type      = award;
            scholar_info.primary_skill   = static_cast<uint8_t>(primary)   & 0b111U;
            scholar_info.secondary_skill = secondary;
            scholar_info.spell           = static_cast<uint8_t>(spell)     & 0b1111111111U;
        }

        void SetWagon(EGameResource resource, int16_t amount) noexcept
        {
            assert(resource >= WOOD && resource <= GOLD);
            assert(amount >= 0 && amount < 16);
            wagon_info.resource_amount = static_cast<uint8_t>(amount)   & 0b11111U;
            wagon_info.visited_bits    = 0;
            wagon_info.resource        = static_cast<uint8_t>(resource) & 0b1111U;
            wagon_info.artifact        = ARTIFACT_NONE;
            wagon_info.full            = true;
        }

        void SetWagon(TArtifact artifact) noexcept
        {
            assert(artifact >= FIRST_ARTIFACT && artifact <= LAST_ARTIFACT);
            wagon_info.resource_amount = 0;
            wagon_info.visited_bits    = 0;
            wagon_info.resource        = static_cast<uint8_t>(GAME_RESOURCE_NONE) & 0b1111U;
            wagon_info.artifact        = ARTIFACT_NONE;
            wagon_info.full            = true;
        }

        void set_tomb(TArtifact artifact) noexcept
        {
            assert(artifact >= FIRST_ARTIFACT && artifact <= LAST_ARTIFACT);
            tomb_info.artifact     = artifact;
            tomb_info.full         = true;
            tomb_info.visited_bits = 0;
        }

        void set_witch_skill(TSecondarySkill skill) noexcept
        {
            assert(skill >= FIRST_SECONDARY_SKILL && skill <= LAST_SECONDARY_SKILL);
            witch_hut_info.skill        = skill;
            witch_hut_info.visited_bits = 0;
        }

        bool PlayerKnowsCell(int16_t player)
        { return THISCALL_2(bool, 0x529B70, this, player); }

    public:
        union
        {
            uint32_t                extraInfo;
            MapIdInfo               item_id;
            MapArtifactInfo         artifact_info;            // Артефакт
            CampfireInfo            campfire_info;            // Костёр
            type_creature_bank_info creature_bank_info;       // Банк существ
            CustomMapArtifactInfo   custom_artifact_info;
            EventInfo               event_info;               // Событие
            FlotsamInfo             flotsam_info;             // Обломки(вода)
            FountainOfFortuneInfo   fountain_of_fortune_info; // Фонтан удачи
            LeanToInfo              lean_to_info;             // Навес(снег)
            MagicSpringInfo         magic_spring_info;        // Магический сад
            MonsterInfo             monster_info;             // Нейтрал
            MysticGardenInfo        mystic_garden_info;       // Мистический сад
            BlackBoxInfo            black_box_info;           // Ящик пандоры
            type_pyramid_info       pyramid_info;             // Пирамида
            CustomResourceInfo      custom_resource_info;     // Ресурс
            ScholarInfo             scholar_info;             // Учёный
            SeaChestInfo            sea_chest_info;           // Морской сундук с сокровищами
            ShipyardInfo            shipyard_info;            // Верфь
            TreasureInfo            treasure_info;            // Сундук с сокровищами
            SkeletonInfo            skeleton_info;            // Скелет
            ShrineInfo              shrine_info;              // Святыня заклинаний
            CustomSpellScrollInfo   custom_spell_scroll_info; // Свиток с заклинаниями
            WiseTreeInfo            tree_info;                // Дерево мудрости
            type_university_info    university_info;          // Университет
            WagonInfo               wagon_info;               // Тележка(степь)
            TombInfo                tomb_info;                // Гробница воина
            WaterWheelInfo          water_wheel_info;         // Водяная мельница
            WindMillInfo            wind_mill_info;           // Ветряная мельница
            WitchHutInfo            witch_hut_info;           // Хижина ведьмы
            MapArtifactInfo_RoE     artifact_info_RoE;        // Артефакт (RoE)
            SkeletonInfo_RoE        skeleton_info_RoE;        // Скелет(RoE)
            MonsterInfo_RoE         monster_info_RoE;         // Нейтрал(RoE)
            SeaChestInfo_RoE        sea_chest_info_RoE;       // Морской сундук(RoE)
            TreasureInfo_RoE        treasure_info_RoE;        // Сундук с сокровищами(RoE)
            TombInfo_RoE            tomb_info_RoE;            // Гробница воина(RoE)
        } NH3API_MSVC_LAYOUT;
};
#pragma pack(pop) // 1

NH3API_SIZE_ASSERT(4, ExtraInfoUnion);

#pragma pack(push, 2)
// Map object /
// Объект на карте.
// size = 0xC = 12, align = 2, baseclass: ExtraInfoUnion
class CObject : public ExtraInfoUnion
{
    public:
        inline CObject(uint8_t  x_,
                       uint8_t  y_,
                       uint8_t  z_,
                       uint16_t type_,
                       uint32_t extraInfo_) noexcept
            : ExtraInfoUnion(nh3api::bit_cast<ExtraInfoUnion>(extraInfo_)),
              x(x_),
              y(y_),
              z(z_),
              TypeID(type_),
              frameOffset(Random(0U, 255U) & UINT8_MAX)
        {}

        inline CObject(uint8_t        x_,
                       uint8_t        y_,
                       uint8_t        z_,
                       uint16_t       type_,
                       ExtraInfoUnion extraInfo_) noexcept
            : ExtraInfoUnion(extraInfo_),
              x(x_),
              y(y_),
              z(z_),
              TypeID(type_),
              frameOffset(Random(0U, 255U) & UINT8_MAX)
        {}

        inline CObject() noexcept
            : ExtraInfoUnion(nh3api::bit_cast<ExtraInfoUnion>(0)),
              x(255),
              y(255),
              z(255),
              TypeID(static_cast<uint16_t>(OBJECT_NONE)),
              frameOffset(Random(0U, 255U) & UINT8_MAX)
        {}

    public:
        [[nodiscard]] type_point get_trigger() const
        {
            int32_t result_x = 0;
            int32_t result_y = 0;

            FindTrigger(result_x, result_y);
            return { static_cast<uint8_t>(static_cast<uint8_t>(result_x) & UINT8_MAX), static_cast<uint8_t>(static_cast<uint8_t>(result_y) & UINT8_MAX), static_cast<uint8_t>(z & 0xFU) };
        }

        void FindTrigger(int32_t& result_x, int32_t& result_y) const
        { THISCALL_3(void, 0x4FF280, this, &result_x, &result_y); }

        [[nodiscard]] CObjectType* get_object_type_ptr()
        { return THISCALL_1(CObjectType*, 0x4FF260, this); }

        [[nodiscard]] const CObjectType* get_object_type_ptr() const
        { return THISCALL_1(CObjectType*, 0x4FF260, this); }

    public:
        // offset: +0x4 = +4,  size = 0x1 = 1
        uint8_t x;

        // offset: +0x5 = +5,  size = 0x1 = 1
        uint8_t y;

        // offset: +0x6 = +6,  size = 0x1 = 1
        uint8_t z;

        unsigned char : 8;

        // Object type /
        // Тип объекта.
        // offset: +0x8 = +8,  size = 0x2 = 2
        uint16_t TypeID;

        // Sprite frame offset. Game sets this value randomly during the loading of a map /
        // Смещение номера кадра анимации объекта. Выставляется случайно при загрузке карты.
        // Наглядный пример: если поставить несколько анимированных объектов, они не будут "двигаться" синхронно.
        // offset: +0xA = +10,  size = 0x1 = 1
        uint8_t frameOffset;

        unsigned char : 8;

};
#pragma pack(pop) // 2

NH3API_SIZE_ASSERT(0xC, CObject);

#pragma pack(push, 4)
// Base class for hero and boat - an object that obscures other objects /
// Базовый класс для hero(герой) и boat(лодка) - объект, который может заслонить другие объекты.
// size = 0x18 = 24, align = 1
class type_obscuring_object
{
    public:
        inline type_obscuring_object() noexcept
            : type_obscuring_object(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4D76E0, this); }

        inline type_obscuring_object(const nh3api::dummy_tag_t& tag) noexcept
            : obscured_location { tag }
        {}

        inline ~type_obscuring_object() noexcept                                       = default;
        inline type_obscuring_object(const type_obscuring_object&) noexcept            = default;
        inline type_obscuring_object(type_obscuring_object&&) noexcept                 = default;
        inline type_obscuring_object& operator=(const type_obscuring_object&) noexcept = default;
        inline type_obscuring_object& operator=(type_obscuring_object&&) noexcept      = default;

    public:
        inline void initialize() noexcept
        { THISCALL_1(void, 0x4D7740, this); }

        [[nodiscard]] TAdventureObjectType get_obscured_type() const noexcept
        { return type; }

        [[nodiscard]] ExtraInfoUnion get_obscured_extra_info() const noexcept
        { return extra_info; }

        [[nodiscard]] bool is_on_map() const noexcept
        { return valid; }

        [[nodiscard]] type_point get_location() const noexcept
        { return { mapX, mapY, mapZ }; }

        [[nodiscard]] bool obscured_is_trigger() const noexcept
        { return was_trigger; }

        // Get town currently occupied by a hero /
        // Город, на данный момент занятый героем.
        [[nodiscard]] town* get_obscured_town() const noexcept
        { return THISCALL_1(town*, 0x4D7700, this); }

        void obscure_cell(TAdventureObjectType new_type, int32_t id)
        { THISCALL_3(void, 0x4D7840, this, new_type, id); }

        void restore_cell()
        { THISCALL_1(void, 0x4D7950, this); }

        [[nodiscard]] bool obscures_town() const
        { return ( is_on_map() ) ? was_trigger && (get_obscured_type() == OBJECT_TOWN) : false; }

        [[nodiscard]] TAdventureObjectType get_obscured_object() const
        {
            if ( is_on_map() )
                return get_obscured_type();

            return OBJECT_NONE;
        }

    public:
        // X coordinate /
        // X координата
        // offset: +0x0 = +0,  size = 0x2 = 2
        int16_t mapX;

        // Y coordinate /
        // Y координата
        // offset: +0x2 = +2,  size = 0x2 = 2
        int16_t mapY;

        // Z coordinate /
        // Z координата
        // offset: +0x4 = +4,  size = 0x2 = 2
        int16_t mapZ;

        // offset: +0x6 = +6,  size = 0x1 = 1
        bool valid;

        // offset: +0x7 = +7,  size = 0x4 = 4
        type_point obscured_location;

        unsigned char : 8;

        // type as adventure object /
        // тип объекта как объекта карты
        // offset: +0xC = +12,  size = 0x4 = 4
        TAdventureObjectType type;

        // offset: +0x10 = +16,  size = 0x4 = 4
        bool32_t was_trigger;

        // offset: +0x14 = +20,  size = 0x4 = 4
        ExtraInfoUnion extra_info;

};
#pragma pack(pop) // 4

#pragma pack(push, 1)
// Map cell information /
// Информация о клетке карты
// size = 0x26 = 38, align = 2, baseclass: ExtraInfoUnion
class NewmapCell : public ExtraInfoUnion
{
    public:
        inline NewmapCell() noexcept
            : NewmapCell(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4FDCA0, this); }

        inline NewmapCell(const nh3api::dummy_tag_t& tag) noexcept
            : ObjectCellList { tag }
        {}

        inline ~NewmapCell() noexcept
        { THISCALL_1(void, 0x4FDB10, this); }

    public:
        // Find trigger cell /
        // Найти триггерную клетку.
        [[nodiscard]] NewmapCell* get_trigger_cell()
        { return THISCALL_1(NewmapCell*, 0x4FD0F0, this); }

        // Find trigger cell /
        // Найти триггерную клетку.
        [[nodiscard]] const NewmapCell* get_trigger_cell() const
        { return THISCALL_1(NewmapCell*, 0x4FD0F0, this); }

        // Get cell map object(including hero or boat if present on this cell) /
        // Текущий объект клетки(включая героя/лодку, если он стоит на ней).
        [[nodiscard]] TAdventureObjectType get_map_object() const
        { return THISCALL_1(TAdventureObjectType, 0x4FD220, this); }

        // Get cell ExtraInfoUnion info(including hero or boat if present on this cell) /
        // Получить информацию ExtraInfoUnion о клетке(включая героя/лодку, если он стоит на ней).
        [[nodiscard]] ExtraInfoUnion get_map_extraInfo() const
        { return THISCALL_1(ExtraInfoUnion, 0x4FD280, this); }

        [[nodiscard]] TArtifact GetArtifactIndex() const noexcept
        { return (objectIndex < MAX_ARTIFACTS) ? static_cast<TArtifact>(objectIndex) : ARTIFACT_NONE; }

        // Current cell is a trigger cell /
        // Текущая клетка является триггерной?
        [[nodiscard]] bool cell_is_trigger() const
        { return THISCALL_1(bool, 0x4FD2D0, this); }

        // Current cell is diggable /
        // Можно ли копать на этой клетке?
        [[nodiscard]] bool is_diggable() const
        { return THISCALL_1(bool, 0x4FD340, this); }

        // Current cell has active event /
        // На этой клетке можно активировать событие?
        [[nodiscard]] bool HasTriggerableEvent() const
        { return THISCALL_1(bool, 0x4FD410, this); }

        // Get underlying magical terrain(if present) adventure object type /
        // Получить магическую почву на клетке(если есть) как объект.
        [[nodiscard]] TAdventureObjectType get_special_terrain() const
        { return THISCALL_1(TAdventureObjectType, 0x4FD470, this); }

        // Get underlying magical terrain type(if present) /
        // Получить тип магической почвы на клетке(если есть).
        [[nodiscard]] EMagicTerrain get_magic_terrain_type() const
        { return THISCALL_1(EMagicTerrain, 0x4FD590, this); }

    public:
        // Информация об объекте
        // size = 0x4 = 4, align = 1
        struct TObjectCell
        {
            public:
                // Object type /
                // Тип объекта.
                // offset: +0x0 = +0,  size = 0x2 = 2
                int16_t ObjectIndex;

                // X offset from the beginning(0,0) of an object /
                // X Смещение клетки относительно начала(0,0) самого объекта
                // occupy: +0x2{00001111}
                int8_t  CellX : 4;

                // Y offset from the beginning(0,0) of an object /
                // Y Смещение клетки относительно начала(0,0) самого объекта
                // occupy: +0x2{11110000}
                int8_t  CellY : 4;

                // Relative "height" of an object(an object with the largest height is drawn above others)
                // Относительная "высота" объекта(объект с самой "высотой" рисуется поверх других)
                // offset: +0x3 = +3,  size = 0x1 = 1
                int8_t  Height;
        } NH3API_MSVC_LAYOUT;

    public:
        // Ground type /
        // Тип почвы.
        // offset: +0x4 = +4,  size = 0x1 = 1
        int8_t GroundSet;

        // Ground sprite frame index /
        // Номер кадра спрайта почвы.
        // offset: +0x5 = +5,  size = 0x1 = 1
        int8_t GroundIndex;

        // River type /
        // Тип реки.
        // offset: +0x6 = +6,  size = 0x1 = 1
        int8_t RiverSet;

        // River sprite frame index /
        // Номер кадра спрайта реки.
        // offset: +0x7 = +7,  size = 0x1 = 1
        int8_t RiverIndex;

        // Road type /
        // Тип дороги
        // offset: +0x8 = +8,  size = 0x1 = 1
        int8_t RoadSet;

        // River sprite frame index /
        // Номер кадра спрайта реки.
        // offset: +0x9 = +9,  size = 0x1 = 1
        int8_t RoadIndex;

        unsigned char : 8;
        unsigned char : 8;

    public:
        // Повёрнута ли почва горизонтально?
        // occupy: +0xC{00000001}
        uint8_t GroundFlippedHorizontal : 1;
        // Повёрнута ли почва вертикально?
        // occupy: +0xC{00000010}
        uint8_t GroundFlippedVertical   : 1;
        // Повёрнута ли река горизонтально?
        // occupy: +0xC{00000100}
        uint8_t RiverFlippedHorizontal  : 1;
        // Повёрнута ли река вертикально?
        // occupy: +0xC{00001000}
        uint8_t RiverFlippedVertical    : 1;
        // Повёрнута ли дорога горизонтально?
        // occupy: +0xC{00010000}
        uint8_t RoadFlippedHorizontal   : 1;
        // Повёрнута ли дорога вертикально?
        // occupy: +0xC{00100000}
        uint8_t RoadFlippedVertical     : 1;
        // Проходима ли клетка?
        // occupy: +0xC{01000000}
        uint8_t Passable                : 1;
        // Есть ли анимированные объекты/реки/анимированная почва на клетке?
        // occupy: +0xC{01000000}
        uint8_t Animated                : 1;
        // Заблокирована ли клетка?
        // offset: +0xD = +13, size = 1 bit, mask = 00000001
        uint8_t IsBlocked               : 1;
        // Примыкает ли клетка к воде?
        // offset: +0xD = +13, size = 1 bit, mask = 00000010
        uint8_t IsBeachBorder           : 1;

    protected:
        // Неиспользуемый бит
        // offset: +0xD = +13, size = 1 bit, mask = 00000100
        uint8_t unused_bit              : 1;

    public:
        // На клетке можно построить корабль?
        // offset: +0xD = +13, size = 1 bit, mask = 00001000
        uint8_t can_build_ship          : 1;

        // Клетка является триггером?
        // offset: +0xD = +13, size = 1 bit, mask = 00010000
        uint8_t is_trigger              : 1;

    protected:
        uint8_t : 3;

    public:
        union {
        // Map object cells on this cell /
        // Клетки объектов на клетке.
        // offset: +0xE = +14,  size = 0x10 = 16
        exe_vector<TObjectCell> ObjectCellList;
        };

        // Type of the "highest" object(that is, one that is drawn above other objects), see TObjectCell::Height /
        // Тип самого "высокого" объекта(который отрисовывается поверх других), см. TObjectCell::Height.
        // offset: +0x1E = +30,  size = 0x4 = 4
        TAdventureObjectType type;

        // "Highest" object subtype. /
        // Подтип самого "высокого" объекта.
        // offset: +0x22 = +34,  size = 0x2 = 2
        int16_t objectIndex;

        // Filled with random values. Do not use. /
        // Заполняется случайными значениями. Лучше не трогать.
        // offset: +0x24 = +36,  size = 0x2 = 2
        int16_t object_type_index;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x26, NewmapCell);

inline bool hasFlag(TAdventureObjectType objType)
{ return FASTCALL_1(bool, 0x40FC00, objType); }

inline int32_t GetFlaggedObjectOwner(NewmapCell* thisCell)
{ return FASTCALL_1(int32_t, 0x40FC80, thisCell); }

NH3API_WARNING(pop)
