//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "resources/resources_include.hpp" // EGameResource
#include "artifact.hpp" // TArtifact, type_artifact
#include "creatures.hpp" // TCreatureType, armyGroup
#include "player_enums.hpp" // EPlayerColor
#include "skills.hpp" // TSkillMastery, TSecondarySkill
#include "terrain.hpp" // type_point
#include "random.hpp" // Random(int, int)

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

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
    // offset: +0x41 = +65,  size = 0x1 = 1
    protected:
    NH3API_MAYBE_UNUSED
    byte_t gap_41;

    public:
    // offset: +0x42 = +66,  size = 0x2 = 2
    int16_t objectTypeIndex;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Map editor struct, also used by the RMG. Object information /
// Структура из редактора карт! Также используется RMG. Информация об объекте.
// size 0x4C = 76, align = 4
struct TObjectType
{
    public:
    // Index in sprite filenames tree(TUniqueSet<std::string> gImageNameSet) /
    // Индекс названия объекта в дереве названий спрайтов(TUniqueSet<std::string> gImageNameSet)
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t _imageNum;
    // Passable cells mask. (= 1 empty cell, = 0 unpassable cell).
    // Use PassableMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
    // Маска клеток проходимости (= 1 пустая клетка, = 0 непроходимая клетка).
    // Используйте PassableMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
    // offset: +0x4 = +4,  size = 0x8 = 8
    exe_bitset<48> _passableMask;
    // Trigger mask. (= 1 trigger cell, = 0 non-trigger cell).
    // Use TriggerMask[getBitPos(x,y)] to obtain value for the cell at (x,y). /
    // Маска триггерных(/входов) клеток. (= 1 клетка-триггер, = 0 клетка без триггера).
    // Используйте TriggerMask[getBitPos(x,y)] для получения значения для кетки в координатах (x,y).
    // offset: +0xC = +12,  size = 0x8 = 8
    exe_bitset<48> _triggerMask;
    // Terrain availability mask(1 = object can be placed on that terrain) /
    // Маска доступности на почве(1 = объект можно установить на почве).
    // offset: +0x14 = +20,  size = 0x4 = 4
    exe_bitset<10> _terrainMask;
    // Recommended terrain mask.
    // In the Map Editor or RMG this object will appear on terrains with value = 1 /
    // Маска родных для объекта почв. В редакторе карт и при генерации карты RMG
    // этот объект появляется на тех почвах, где значение = 1.
    // offset: +0x18 = +24,  size = 0x4 = 4
    exe_bitset<10> _terrainRecommendedMask;
    // Object type /
    // Тип объекта.
    // offset: +0x1C = +28,  size = 0x4 = 4
    TAdventureObjectType _type;
    // Object subtype /
    // Подтип объекта.
    // offset: +0x20 = +32,  size = 0x4 = 4
    int32_t _subtype;
    // Slot category in map editor. /
    // Категория слота в редакторе карт.
    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t _slotCategory;
    // Drawn underlying? (as part of the ground, below other objects) /
    // Является ли частью земли?
    // offset: +0x28 = +40,  size = 0x1 = 1
    bool _isUnderlay;
    // Has any trigger cells? /
    // Имеет хотя бы одну триггерную клетку?
    // offset: +0x29 = +41,  size = 0x1 = 1
    bool _hasTrigger;
    // The first trigger cell. (Start from the lower right and iterate to the left, from bottom to top) /
    // If there are no such cells, _triggerCell = {8, 6}.
    // Самая первая триггерная клетка. (Начиная с нижней правой клетки, итерируя влево, в конце строки на верх).
    // Если такой клетки нет, _triggerCell = {8, 6}.
    // offset: +0x2C = +44,  size = 0x8 = 8
    TPoint _triggerCell;
    // Object image info /
    // Информация об изображении объекта.
    // offset: +0x34 = +52,  size = 0x18 = 24
    struct _TImageInfo
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
    } _imageInfo;
};
#pragma pack(pop)

NH3API_FORCEINLINE NH3API_CONSTEXPR
uint32_t getBitPos(uint32_t x, uint32_t y)
{ return 8 * (6 - y) - x - 1; }

#pragma pack(push, 4)
// Artifact/resource custom data /
// Настройки артефакта или ресурса на карте.
// size = 0x4C = 76, align = 4
struct TreasureData
{
    public:
        NH3API_FORCEINLINE
        TreasureData() NH3API_NOEXCEPT
            : Message(), HasCustomGuardians(false), Guardians()
        {}

        NH3API_FORCEINLINE
        TreasureData(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Message(tag), Guardians(tag)
        { NH3API_IGNORE(HasCustomGuardians); }

    public:
        // Displayed message upon gathering /
        // Сообщение, выводимое при собирании ресурса/артефакта.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message;

        // Has custom guards? /
        // Настроена охрана?
        // offset: +0x10 = +16,  size = 0x1 = 1
        bool HasCustomGuardians;

        // Guards /
        // Охрана.
        // offset: +0x14 = +20,  size = 0x38 = 56
        armyGroup Guardians;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Secondary Skill Data in Pandora's box /
// Данные вторичного навыка, используется в структуре ящика пандоры.
// size = 0x8 = 8, align = 4
struct SecondarySkillData
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        SecondarySkillData() NH3API_NOEXCEPT
            : type(SKILL_NONE), level(eMasteryNone)
        {}

        NH3API_FORCEINLINE
        SecondarySkillData(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        { NH3API_IGNORE(type, level); }

    public:
        // Secondary skill /
        // Вторичный навык.
        // offset: +0x0 = +0,  size = 0x4 = 4
        TSecondarySkill type;

        // Secondary skill mastery /
        // Ступень вторичного навыка.
        // offset: +0x4 = +4,  size = 0x4 = 4
        TSkillMastery level;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Adventure map monster data /
// Информация о существе на карте.
// size = 0x30 = 48, align = 4
struct MonsterData
{
    public:
        NH3API_FORCEINLINE
        MonsterData() NH3API_NOEXCEPT
            : Message(), Artifact(ARTIFACT_NONE)
        { ResQty.fill(0); }

        NH3API_FORCEINLINE
        MonsterData(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Message(tag)
        { NH3API_IGNORE(Artifact, ResQty); }

    public:
        // Attack message /
        // Сообщение при атаке существа.
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_string Message;

        // Resources bonus upon defeat /
        // Ресурсы, которые можно получить при победе над существами.
        // offset: +0x10 = +16,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty;

        // Artifact bonus upon defeat /
        // Артефакт, который можно получить при победе над существами.
        // offset: +0x2C = +44,  size = 0x4 = 4
        TArtifact Artifact;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Black Box(aka Pandora's Box) and Event data /
// Информация о ящике пандоры и событии.
// size = 0xE4 = 228, align = 4, baseclass: TreasureData
struct BlackBoxData : TreasureData
{
    public:
        NH3API_FORCEINLINE
        BlackBoxData() NH3API_NOEXCEPT
            : HasCustomTreasure(false),
              ExperienceBonus(0),
              ManaBonus(0),
              MoraleBonus(0),
              LuckBonus(0)
        {
            ResQty.fill(0);
            PrimarySkillBonus.fill(0);
        }

        NH3API_FORCEINLINE
        BlackBoxData(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : TreasureData(tag),
              SecondarySkills(tag),
              Artifacts(tag),
              Spells(tag),
              Creatures(tag)
        { NH3API_IGNORE(HasCustomTreasure,
                        ExperienceBonus,
                        ManaBonus,
                        MoraleBonus,
                        LuckBonus,
                        ResQty,
                        PrimarySkillBonus); }

    public:
        // Has custom treasure? /
        // Имеет настроенные ресурсы?
        // offset: +0x4C = +76,  size = 0x1 = 1
        bool HasCustomTreasure;

        // Experience Bonus(always positive) /
        // Бонус опыта(всегда положительный).
        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t ExperienceBonus;

        // Mana Bonus(always positive) /
        // Бонус маны(всегда положительный).
        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t ManaBonus;

        // Morale Bonus(can be negative) /
        // Бонус боевого духа(может быть отрицательным).
        // offset: +0x58 = +88,  size = 0x1 = 1
        int8_t MoraleBonus;

        // Luck Bonus(can be negative) /
        // Бонус удачи(может быть отрицательным).
        // offset: +0x59 = +89,  size = 0x1 = 1
        int8_t LuckBonus;

        // Bonus resources(can be negative) /
        // Бонусные ресурсы(может быть отрицательным).
        // offset: +0x5C = +92,  size = 0x1C = 28
        std::array<int32_t, 7> ResQty;

        // Primary skill bonuses(always positive) /
        // Бонусы первичных навыков(всегда положительные).
        // offset: +0x78 = +120,  size = 0x4 = 4
        std::array<int8_t, kNumPrimarySkills> PrimarySkillBonus;

        // Bonus secondary skills /
        // Бонусные вторичные навыки.
        // offset: +0x7C = +124,  size = 0x10 = 16
        exe_vector<SecondarySkillData> SecondarySkills;

        // Bonus artifacts /
        // Бонусные артефакты.
        // offset: +0x8C = +140,  size = 0x10 = 16
        exe_vector<TArtifact>  Artifacts;

        // Bonus spells /
        // Бонусные заклинания.
        // offset: +0x9C = +156,  size = 0x10 = 16
        exe_vector<SpellID>    Spells;

        // Bonus creatures that will join your army /
        // Бонусные существа, которые присоединятся к армии героя.
        // offset: +0xAC = +172,  size = 0x38 = 56
        armyGroup              Creatures;
};
#pragma pack(pop)

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
        NH3API_FORCEINLINE
        TRandomDwelling() NH3API_NOEXCEPT
            : townId(-1),
              towns(0),
              playerOwner(PLAYER_NONE),
              minLVL(0),
              maxLVL(6),
              object(nullptr)
        {}

        NH3API_FORCEINLINE
        TRandomDwelling(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        { NH3API_IGNORE(townId, playerOwner, minLVL, maxLVL, object); }

    public:
        // Town ID /
        // ID города.
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t townId;

        // Allowed town types /
        // Указанные фракции жилища. Выбирается случайная из выбранных фракций.
        // offset: +0x4 = +4,  size = 0x4 = 4
        uint16_t towns;

        // Player owning this dwelling /
        // Игрок, владеющий этим жилищем.
        // offset: +0x6 = +6,  size = 0x1 = 1
        uint8_t playerOwner;

        // Creature lowest level /
        // Наименьший уровень существа.
        // offset: +0x7 = +7,  size = 0x1 = 1
        uint8_t minLVL;

        // Creature largest level /
        // Наибольший уровень существа.
        // offset: +0x8 = +8,  size = 0x1 = 1
        uint8_t maxLVL;
    
    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_9[3];

    public:
        // Object /
        // Объект.
        // offset: +0xC = +12,  size = 0x4 = 4
        CObject* object;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Black market.
// Чёрный рынок.
// size = 0x1C = 28, align = 4
class TBlackMarket
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        TBlackMarket() NH3API_NOEXCEPT
        { artifacts.fill(ARTIFACT_NONE); }

        NH3API_FORCEINLINE
        TBlackMarket(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        { NH3API_IGNORE(artifacts); }

    public:
        // Artifacts /
        // Артефакты.
        // offset: +0x0 = +0,  size = 0x1C = 28
        std::array<TArtifact, 7> artifacts;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Creature dwelling /
// Жилище существ
// size = 0x5C = 92, align = 4
class generator
{
    public:
        NH3API_FORCEINLINE
        generator() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(generator)
        { THISCALL_1(void, 0x4B8250, this); }

        NH3API_FORCEINLINE
        generator(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : guards(tag)
        { NH3API_IGNORE(genClass, genType, type, population, mapX, mapY, mapZ, playerOwner, town_id); }

    public:
        int32_t get_owner() const
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
};
#pragma pack(pop)

#pragma pack(push, 4)
// Mine or Lighthouse /
// Шахта или маяк.
// size = 0x40 = 64, align = 4
class mine
{
    public:
        NH3API_CONSTEXPR_CPP_20 NH3API_FORCEINLINE
        mine() NH3API_NOEXCEPT
            : playerOwner(PLAYER_NONE),
              type(const_no_resource),
              is_abandoned(false),
              guards(),
              mapX(-1), mapY(-1), mapZ(-1)
        {}

        NH3API_FORCEINLINE
        mine(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : guards(tag)
        { NH3API_IGNORE(playerOwner, type, is_abandoned, mapX, mapY, mapZ); }

    public:
        // Player owning this object /
        // Игрок, владеющий этим объектом.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t playerOwner;

        // Resource created by this mine /
        // Ресурс, производимый шахтой.
        // offset: +0x1 = +1,  size = 0x1 = 1
        int8_t type;

        // Is mine abandoned /
        // Шахта является заброшенной.
        // offset: +0x2 = +2,  size = 0x1 = 1
        bool is_abandoned;

        // Mine guards /
        // Охраны шахты.
        // offset: +0x4 = +4,  size = 0x38 = 56
        armyGroup guards;

        // Object x-coordinate /
        // x-координата объекта.
        // offset: +0x3C = +60,  size = 0x1 = 1
        uint8_t mapX;

        // Object y-coordinate /
        // y-координата объекта.
        // offset: +0x3D = +61,  size = 0x1 = 1
        uint8_t mapY;

        // Object z-coordinate /
        // z-координата объекта.
        // offset: +0x3E = +62,  size = 0x1 = 1
        uint8_t mapZ;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Garrison /
// Гарнизон.
// size = 0x40 = 64, align = 4
class garrison
{
    public:
        NH3API_FORCEINLINE
        garrison() NH3API_NOEXCEPT
            : playerOwner(PLAYER_NONE),
              garrisonArmy()
        {}

        NH3API_FORCEINLINE
        garrison(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : garrisonArmy(tag)
        {}

    public:
        // Player owning this garrison /
        // Игрок, владеющий этим гарнизоном.
        // offset: +0x0 = +0,  size = 0x1 = 1
        uint8_t playerOwner;

        // Garrison army /
        // Армия гарнизона.
        // offset: +0x4 = +4,  size = 0x38 = 56
        armyGroup garrisonArmy;

        // Army is removable /
        // Армию можно забрать.
        // offset: +0x3C = +60,  size = 0x1 = 1
        bool armyRemovable;

        // Object x-coordinate /
        // x-координата объекта.
        // offset: +0x3D = +61,  size = 0x1 = 1
        uint8_t mapX;

        // Object y-coordinate /
        // y-координата объекта.
        // offset: +0x3E = +62,  size = 0x1 = 1
        uint8_t mapY;

        // Object z-coordinate /
        // z-координата объекта.
        // offset: +0x3F = +63,  size = 0x1 = 1
        uint8_t mapZ;

};
#pragma pack(pop)

#pragma pack(push, 4)
// University /
// Университет.
// size = 0x10 = 16, align = 4
class type_university
{
    public:
        NH3API_FORCEINLINE
        type_university() NH3API_NOEXCEPT
        #if NH3API_STD_INITIALIZER_LIST
            : skills{SKILL_NONE, SKILL_NONE, SKILL_NONE, SKILL_NONE}
        {}
        #else
        { skills.fill(SKILL_NONE); }
        #endif

        NH3API_FORCEINLINE
        type_university(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {}

    public:
        // Secondary skills /
        // Вторичные навыки, предлагаемые университетом для изучения.
        // offset: +0x0 = +0,  size = 0x10 = 16
        std::array<TSecondarySkill, 4> skills;
};
#pragma pack(pop)

#pragma pack(push, 4)
//
// size = 0xC = 12, align = 4
struct type_creature_source
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    TCreatureType type;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int16_t* ptr;

    // offset: +0x8 = +8,  size = 0x2 = 2
    int16_t number;

    // offset: +0xA = +10,  size = 0x1 = 1
    bool is_free;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Creature bank /
// Банк существ.
// size = 0x6C = 108, align = 4
class type_creature_bank
{
    public:
        NH3API_FORCEINLINE
        type_creature_bank() NH3API_NOEXCEPT
        {}

        NH3API_FORCEINLINE
        type_creature_bank(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : guards(tag), artifacts(tag)
        {}

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

        // Artifacts aquired on victory /
        // Артефакты, получаемых в качестве награды при зачистке банка.
        // offset: +0x5C = +92,  size = 0x10 = 16
        exe_vector<type_creature_source> artifacts;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Sign /
// Дорожный знак.
// size = 0x14 = 20, align = 4
class Sign
{
    public:
        NH3API_FORCEINLINE
        Sign() NH3API_NOEXCEPT
            : hasText(false), signText()
        {}

        NH3API_FORCEINLINE
        Sign(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : signText(tag)
        { NH3API_IGNORE(hasText); }

    public:
        // Has customized text /
        // Есть настроенный текст.
        // offset: +0x0 = +0,  size = 0x4 = 4
        bool32_t hasText;

        // Custom text /
        // Настроенный текст.
        // offset: +0x4 = +4,  size = 0x10 = 16
        exe_string signText;

};
#pragma pack(pop)

// RoE Artifact pickup requirements /
// Типы требований для поднятия артефактов из RoE.
enum ArtifactPrices : uint32_t
{
    const_free_artifact = 0,
    const_artifact_costs_2000 = 1,
    const_artifact_requires_wisdom = 2,
    const_artifact_requires_leadership = 3,
    const_artifact_costs_2500 = 4,
    const_artifact_costs_3000 = 5,
    const_artifact_defended = 6
};

#pragma pack(push, 4)
struct mapCellArtifact_RoE
{
    ArtifactPrices price : 4;
    unsigned guard : 9;
    unsigned resource_price : 4;
    unsigned guard_qty : 14;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

struct mapCellArtifact
{
    ArtifactPrices price : 4;
    unsigned guard : 8;
    unsigned resource_price : 4;
    unsigned : 2;
    unsigned guard_qty : 12;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

struct mapCellDefaultObject
{
    unsigned id;

} NH3API_MSVC_LAYOUT;

struct mapCellCampfire
{
    unsigned amount : 4;
    unsigned resource : 16;
    unsigned : 12;

} NH3API_MSVC_LAYOUT;

struct mapCellCorpse_RoE
{
    unsigned id : 5;
    unsigned : 1;
    signed   artifact : 7;
    unsigned has_treasure : 1;
    unsigned : 15;

} NH3API_MSVC_LAYOUT;

struct mapCellCorpse
{
    unsigned id : 5;
    unsigned : 1;
    TArtifact artifact : 10;
    unsigned has_treasure : 1;
    unsigned : 15;

} NH3API_MSVC_LAYOUT;

struct mapCellCreatureBank
{
    unsigned : 5;
    unsigned visited_bits : 8;
    unsigned index : 12;
    unsigned empty : 1;
    unsigned : 6;

} NH3API_MSVC_LAYOUT;

struct mapCellEvent
{
    unsigned index : 10;
    unsigned allow_player : 8;
    unsigned allow_computer : 1;
    unsigned cancel_after_visit : 1;
    unsigned : 12;

} NH3API_MSVC_LAYOUT;

struct mapCellFlotsam
{
    enum : uint32_t {
        FLOTSAM_EMPTY = 0,
        FLOTSAM_WOOD5 = 1,
        FLOTSAM_WOOD5_GOLD200 = 2,
        FLOTSAM_WOOD10_GOLD500 = 3
    } type;
} NH3API_MSVC_LAYOUT;

struct mapCellFountainFortune
{
    unsigned : 5;
    unsigned visited_bits : 8;
    signed luck_bonus : 4;
    unsigned : 15;

} NH3API_MSVC_LAYOUT;

struct mapCellLeanTo
{
    unsigned id : 5;
    unsigned : 1;
    unsigned amount : 4;
    unsigned resource : 4;
    unsigned : 18;

} NH3API_MSVC_LAYOUT;

struct mapCellMagicShrine
{
    unsigned : 5;
    unsigned visited_bits : 8;
    unsigned spell : 10;
    unsigned : 9;

} NH3API_MSVC_LAYOUT;

struct mapCellMagicSpring
{
    unsigned id : 5;
    unsigned : 1;
    unsigned full : 1;
    unsigned : 25;

} NH3API_MSVC_LAYOUT;

struct mapCellMonster_RoE
{
    unsigned qty : 12;
    unsigned disposition : 5;
    unsigned never_flee : 1;
    unsigned dont_grow : 1;
    unsigned index : 12;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

struct mapCellMonster
{
    unsigned qty : 12;
    unsigned disposition : 5;
    unsigned never_flee : 1;
    unsigned dont_grow : 1;
    unsigned index : 8;
    unsigned growth_remainder : 4;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

struct mapCellMysticGarden
{
    unsigned id : 5;
    unsigned : 1;
    unsigned resource : 4;
    unsigned full : 1;
    unsigned : 21;

} NH3API_MSVC_LAYOUT;

struct mapCellPandorasBox
{
    unsigned index : 10;
    unsigned : 22;

} NH3API_MSVC_LAYOUT;

struct mapCellPyramid
{
    unsigned guarded : 1;
    unsigned : 4;
    unsigned visited_bits : 8;
    unsigned spell : 8;
    unsigned : 11;

} NH3API_MSVC_LAYOUT;

struct mapCellResource
{
    unsigned value : 19;
    unsigned setupIndex : 12;
    unsigned hasSetup : 1;

} NH3API_MSVC_LAYOUT;

enum ScholarAwards : uint32_t
{
    const_scholar_primary_skill   = 0,
    const_scholar_secondary_skill = 1,
    const_scholar_spell           = 2,
};

struct mapCellScholar
{
    ScholarAwards type : 3;
    unsigned PSkill : 3;
    unsigned SSkill : 7;
    unsigned spell : 10;
    unsigned : 9;

} NH3API_MSVC_LAYOUT;

struct mapCellScroll
{
    unsigned spell : 8;
    unsigned : 11;
    unsigned index : 12;
    unsigned custom : 1;

} NH3API_MSVC_LAYOUT;

enum SeaChestRewardTypes : uint32_t
{
    const_sea_chest_nothing = 0,
    const_sea_chest_gold = 1,
    const_sea_chest_artifact = 2
};

struct mapCellSeaChest_RoE
{
    SeaChestRewardTypes reward : 3;
    signed artifact : 8;
    unsigned : 19;

} NH3API_MSVC_LAYOUT;

struct mapCellSeaChest
{
    SeaChestRewardTypes reward : 3;
    TArtifact artifact : 10;
    unsigned : 19;

} NH3API_MSVC_LAYOUT;

struct mapCellShipwreckSurvivor
{
    TArtifact artifact;

};

struct mapCellShipyard
{
    unsigned owner : 8;
    unsigned boatX : 8;
    unsigned boatY : 8;
    unsigned : 8;

} NH3API_MSVC_LAYOUT;

struct mapCellTreasureChest_RoE
{
    signed   artifact : 8;
    unsigned is_artifact : 1;
    unsigned gold_amount : 4;
    unsigned : 19;

} NH3API_MSVC_LAYOUT;

struct mapCellTreasureChest
{
    TArtifact   artifact : 10;
    unsigned is_artifact : 1;
    unsigned gold_amount : 4;
    unsigned : 17;

} NH3API_MSVC_LAYOUT;

enum WiseTreePrices : uint32_t 
{
    const_tree_wants_nothing = 0,
    const_tree_wants_gold    = 1,
    const_tree_wants_gems    = 2,
    const_tree_price_count   = 3
};

struct mapCellTreeOfKnowledge
{
    unsigned id : 5;
    unsigned visited_bits : 8;
    WiseTreePrices price  : 2;
    unsigned : 17;

} NH3API_MSVC_LAYOUT;

struct mapCellUniversity
{
    unsigned : 5;
    unsigned visited_bits : 8;
    unsigned index : 12;
    unsigned : 7;

} NH3API_MSVC_LAYOUT;

struct mapCellWagon_RoE
{
    unsigned  resource_amount : 5;
    unsigned  visited_bits : 8;
    unsigned  full : 1;
    unsigned  has_artifact : 1;
    signed    artifact : 8;
    unsigned  resource : 4;
    unsigned  : 5;

} NH3API_MSVC_LAYOUT;

struct mapCellWagon
{
    unsigned  resource_amount : 5;
    unsigned  visited_bits : 8;
    unsigned  full : 1;
    unsigned  has_artifact : 1;
    TArtifact artifact : 10;
    unsigned  resource : 4;
    unsigned  : 3;

} NH3API_MSVC_LAYOUT;

struct mapCellWarriorsTomb_RoE
{
    unsigned  full : 1; // есть артефакт
    unsigned  : 4;
    unsigned  visited_bits : 8; // посещение каждым из игроков
    signed    artifact : 8;
    unsigned  : 11;

} NH3API_MSVC_LAYOUT;

struct mapCellWarriorsTomb
{
    unsigned  full : 1; // есть артефакт
    unsigned  : 4;
    unsigned  visited_bits : 8; // посещение каждым из игроков
    TArtifact artifact : 10;
    unsigned  : 9;

} NH3API_MSVC_LAYOUT;

struct mapCellWaterMill
{
    unsigned amount : 5;
    unsigned visited_bits : 8;
    unsigned : 19;

} NH3API_MSVC_LAYOUT;

struct mapCellWindMill
{
    unsigned resource : 4; // тип ресурса
    unsigned : 1;
    unsigned visited_bits : 8;
    unsigned amount : 4; // кол-во ресурса
    unsigned : 15;

} NH3API_MSVC_LAYOUT;

struct mapCellWitchHut
{
    unsigned : 5;
    unsigned visited_bits : 8;
    TSecondarySkill skill : 7;  // тип вторичного навыка
    unsigned : 12;

} NH3API_MSVC_LAYOUT;

// Map object extra info /
// Дополнительная информация об объекте на карте.
// size = 0x4 = 4, align = 4.
struct ExtraInfoUnion
{
    public:
        // Set object visited by <player> /
        // Пометить объект клетки посещенным игроком <player>.
        void SetCellVisited(int16_t player)
        { THISCALL_2(void, 0x4FC620, this, player); }

    public:
        union
        {
            uint32_t extraInfo;
            mapCellArtifact          Artifact;          // Артефакт
            mapCellDefaultObject     BlackMarket;       // Чёрный рынок
            mapCellDefaultObject     Boat;              // Лодка
            mapCellCampfire          Campfire;          // Костёр
            mapCellCorpse            Corpse;            // Скелет
            mapCellCreatureBank      CreatureBank;      // Банк существ
            mapCellEvent             Event;             // Событие
            mapCellFlotsam           Flotsam;           // Обломки(вода)
            mapCellFountainFortune   FountainFortune;   // Фонтан удачи
            mapCellDefaultObject     Garrison;          // Гарнизон
            mapCellDefaultObject     Generator;         // Генератор существ('нычка')
            mapCellDefaultObject     Hero;              // Герой
            mapCellLeanTo            LeanTo;            // Навес(снег)
            mapCellDefaultObject     LearningStone;     // Камень обучения
            mapCellDefaultObject     Lighthouse;        // Маяк
            mapCellMagicShrine       MagicShrine;       // Святыня заклинаний
            mapCellMagicSpring       MagicSpring;       // Магический сад
            mapCellDefaultObject     Mine;              // Шахта
            mapCellDefaultObject     Monolith;          // Монолит
            mapCellMonster           Monster;           // Нейтрал
            mapCellMysticGarden      MysticGarden;      // Мистический сад
            mapCellDefaultObject     Obelisk;           // Обелиск
            mapCellDefaultObject     OceanBottle;       // Бутылка(вода)
            mapCellPandorasBox       PandorasBox;       // Ящик пандоры
            mapCellDefaultObject     Prison;            // Тюрьма
            mapCellPyramid           Pyramid;           // Пирамида
            mapCellDefaultObject     QuestGuard;        // Страж квеста
            mapCellDefaultObject     RefugeeCamp;       // Лагерь беженцев
            mapCellResource          Resource;          // Ресурс
            mapCellScholar           Scholar;           // Учёный
            mapCellScroll            SpellScroll;       // Свиток с заклинаниями
            mapCellSeaChest          SeaChest;          // Морской сундук с сокровищами
            mapCellDefaultObject     SeerHut;           // Хижина провидца
            mapCellShipwreckSurvivor ShipwreckSurvivor; // Потерпевший кораблекрушение
            mapCellShipyard          Shipyard;          // Верфь
            mapCellDefaultObject     SignPost;          // Указатель('табличка')
            mapCellDefaultObject     Town;              // Город
            mapCellTreasureChest     TreasureChest;     // Сундук с сокровищами
            mapCellTreeOfKnowledge   TreeKnowledge;     // Дерево мудрости
            mapCellUniversity        University;        // Университет
            mapCellWagon             Wagon;             // Тележка(степь)
            mapCellWarriorsTomb      WarriorsTomb;      // Гробница воина
            mapCellWaterMill         Watermill;         // Водяная мельница
            mapCellWindMill          Windmill;          // Ветряная мельница
            mapCellWitchHut          WitchHut;          // Хижина ведьмы

            mapCellArtifact_RoE      Artifact_RoE;      // Артефакт (RoE)
            mapCellCorpse_RoE        Corpse_RoE;        // Скелет(RoE)
            mapCellMonster_RoE       Monster_RoE;       // Нейтрал(RoE)
            mapCellSeaChest_RoE      SeaChest_RoE;      // Морской сундук(RoE)
            mapCellTreasureChest_RoE TreasureChest_RoE; // Сундук с сокровищами(RoE)
            mapCellWarriorsTomb_RoE  WarriorsTomb_RoE;  // Гробница воина(RoE)
        } NH3API_MSVC_LAYOUT;
};
#pragma pack(pop) // 4

NH3API_SIZE_ASSERT(4, ExtraInfoUnion);

#pragma pack(push, 2)
// Map object /
// Объект на карте.
// size = 0xC = 12, align = 2, baseclass: ExtraInfoUnion
class CObject : public ExtraInfoUnion
{
    public:
        NH3API_FORCEINLINE
        CObject(uint8_t _x,
                uint8_t _y,
                uint8_t _z,
                uint16_t _type,
                uint32_t _extraInfo) NH3API_NOEXCEPT
            : x(_x), y(_y), z(_z),
              TypeID(_type), frameOffset(Random(0, 255))
        {
            this->extraInfo = _extraInfo;
        }

        NH3API_FORCEINLINE
        CObject() NH3API_NOEXCEPT
            : x(255), y(255), z(255),
              TypeID(OBJECT_NONE), frameOffset(Random(0, 255))
        {
            this->extraInfo = 0;
        }

    public:
        type_point get_trigger() const
        {
            int32_t result_x;
            int32_t result_y;

            FindTrigger(result_x, result_y);
            return type_point(result_x, result_y, z);
        }

        void FindTrigger(int32_t& result_x, int32_t& result_y) const
        { THISCALL_3(void, 0x4FF280, this, &result_x, &result_y); }

        CObjectType* get_object_type_ptr()
        { return THISCALL_1(CObjectType*, 0x4FF260, this); }

        const CObjectType* get_object_type_ptr() const
        { return THISCALL_1(CObjectType*, 0x4FF260, this); }

    public:
        // offset: +0x4 = +4,  size = 0x1 = 1
        uint8_t x;

        // offset: +0x5 = +5,  size = 0x1 = 1
        uint8_t y;

        // offset: +0x6 = +6,  size = 0x1 = 1
        uint8_t z;

        // Object type /
        // Тип объекта.
        // offset: +0x8 = +8,  size = 0x2 = 2
        uint16_t TypeID;

        // Sprite frame offset. Game sets this value randomly during the loading of a map /
        // Смещение номера кадра анимации объекта. Выставляется случайно при загрузке карты.
        // Наглядный пример: если поставить несколько анимированных объектов, они не будут "двигаться" синхронно.
        // offset: +0xA = +10,  size = 0x1 = 1
        uint8_t frameOffset;
};
#pragma pack(pop) // 2

NH3API_SIZE_ASSERT(0xC, CObject);

#pragma pack(push, 1)
// Base class for hero and boat/
// Базовый класс для hero(герой) и boat(лодка).
// size = 0x18 = 24, align = 1
class type_obscuring_object
{
    public:
        NH3API_FORCEINLINE
        type_obscuring_object() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_obscuring_object)
        { THISCALL_1(void, 0x4D76E0, this); }

        NH3API_FORCEINLINE
        type_obscuring_object(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : obscured_location(tag)
        {}

    public:
        void initialize() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4D7740, this); }

        TAdventureObjectType get_obscured_type() const
        { return type; }

        ExtraInfoUnion get_obscured_extra_info()const
        { return extra_info; }

        bool is_on_map() const
        { return valid; }

        type_point get_location() const
        { return type_point(mapX & 0xFF, mapY & 0xFF, mapZ & 0xFF); }

        bool obscured_is_trigger() const
        { return !!was_trigger; }

        //mine* get_obscured_mine() const
        //{ return ( !is_on_map() || get_obscured_type() != OBJECT_MINE || extra_info.Mine.ID == -1 ) ? nullptr : &gpGame->minePool[extra_info]; }
        //town* get_obscured_town() const
        //{ return ( !is_on_map() || get_obscured_type() != OBJECT_TOWN || extra_info.Town.ID == -1 ) ? nullptr : &gpGame->townPool[extra_info]; }

        //
        void obscure_cell(TAdventureObjectType new_type, int32_t id)
        { THISCALL_3(void, 0x4D7840, this, new_type, id); }

        void restore_cell()
        { THISCALL_1(void, 0x4D7950, this); }

        bool obscures_town() const
        { return ( is_on_map() ) ? was_trigger && (get_obscured_type( ) == OBJECT_TOWN) : false; }

        TAdventureObjectType get_obscured_object() const
        { 
            if(is_on_map()) 
                return get_obscured_type(); 
            else 
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

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_B;

    public:

        // type as adventure object /
        // тип объекта как объекта карты
        // offset: +0xC = +12,  size = 0x4 = 4
        TAdventureObjectType type;

        // offset: +0x10 = +16,  size = 0x4 = 4
        bool32_t was_trigger;

        // offset: +0x14 = +20,  size = 0x4 = 4
        ExtraInfoUnion extra_info;

};
#pragma pack(pop) // 1

#pragma pack(push, 2)
// Map cell information /
// Информация о клетке карты
// size = 0x26 = 38, align = 2, baseclass: ExtraInfoUnion
class NewmapCell : public ExtraInfoUnion
{
    public:
        NH3API_FORCEINLINE
        NewmapCell() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(NewmapCell)
        { THISCALL_1(void, 0x4FDCA0, this); }

        NH3API_FORCEINLINE
        NewmapCell(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : ObjectCellList(tag)
        {}

        NH3API_FORCEINLINE
        ~NewmapCell() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4FDB10, this); }

    public:
        // Find trigger cell /
        // Найти триггерную клетку.
        NewmapCell* get_trigger_cell() const
        { return THISCALL_1(NewmapCell*, 0x4FD0F0, this); }

        // Get cell map object(including hero or boat if present on this cell) /
        // Получить текущий объект клетки(включая героя/лодку, если он стоит на ней).
        TAdventureObjectType get_map_object() const
        { return THISCALL_1(TAdventureObjectType, 0x4FD220, this); }

        // Get cell ExtraInfoUnion info(including hero or boat if present on this cell) /
        // Получить информацию ExtraInfoUnion о клетке(включая героя/лодку, если он стоит на ней).
        ExtraInfoUnion get_map_extraInfo() const
        { return THISCALL_1(ExtraInfoUnion, 0x4FD280, this); }

        // Current cell is a trigger cell /
        // Текущая клетка является триггерной?
        bool cell_is_trigger() const
        { return THISCALL_1(bool, 0x4FD2D0, this); }

        // Current cell is diggable /
        // Можно ли копать на этой клетке?
        bool is_diggable() const
        { return THISCALL_1(bool, 0x4FD340, this); }

        // Current cell has active event /
        // На этой клетке можно активировать событие?
        bool HasTriggerableEvent() const
        { return THISCALL_1(bool, 0x4FD410, this); }

        // Get underlying magical terrain(if present) adventure object type /
        // Получить магическую почву на клетке(если есть) как объект.
        TAdventureObjectType get_special_terrain() const
        { return THISCALL_1(TAdventureObjectType, 0x4FD470, this); }

        // Get underlying magical terrain type(if present) /
        // Получить тип магической почвы на клетке(если есть).
        EMagicTerrain get_special_terrain_type() const
        { return THISCALL_1(EMagicTerrain, 0x4FD590, this); }

    public:
        #pragma pack(push, 4)
        // Информация об объекте
        // size = 0x4 = 4, align = 4
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
        #pragma pack(pop) // 4

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

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_A[2];

    public:
        // Повёрнута ли почва горизонтально?
        // occupy: +0xC{00000001}
        bool GroundFlippedHorizontal : 1;
        // Повёрнута ли почва вертикально?
        // occupy: +0xC{00000010}
        bool GroundFlippedVertical   : 1;
        // Повёрнута ли река горизонтально?
        // occupy: +0xC{00000100}
        bool RiverFlippedHorizontal  : 1;
        // Повёрнута ли река вертикально?
        // occupy: +0xC{00001000}
        bool RiverFlippedVertical    : 1;
        // Повёрнута ли дорога горизонтально?
        // occupy: +0xC{00010000}
        bool RoadFlippedHorizontal   : 1;
        // Повёрнута ли дорога вертикально?
        // occupy: +0xC{00100000}
        bool RoadFlippedVertical     : 1;
        // Проходима ли клетка?
        // occupy: +0xC{01000000}
        bool Passable                : 1;
        // Есть ли анимированные объекты/реки/анимированная почва на клетке?
        // occupy: +0xC{01000000}
        bool Animated                : 1;
        // Заблокирована ли клетка?
        // occupy: +0xD{00000001}
        bool IsBlocked               : 1;
        // Примыкает ли клетка к воде?
        // occupy: +0xD{00000010}
        bool IsBeachBorder           : 1;

    protected:
        // Неиспользуемый бит
        // occupy: +0xD{00000100}
        bool unused_bit              : 1;

    public:
        // На клетке можно построить корабль?
        // occupy: +0xD{00001000}
        bool can_build_ship          : 1;
        
        // Клетка является триггером?
        // occupy: +0xD{00010000}
        bool is_trigger              : 1;

        // Map object cells on this cell /
        // Клетки объектов на клетке.
        // offset: +0xE = +14,  size = 0x10 = 16
        exe_vector<TObjectCell> ObjectCellList;

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

NH3API_FORCEINLINE
bool hasFlag(TAdventureObjectType objType) NH3API_NOEXCEPT
{ return FASTCALL_1(bool, 0x40FC00, objType); }

NH3API_FORCEINLINE
int32_t GetFlaggedObjectOwner(NewmapCell* thisCell) NH3API_NOEXCEPT
{ return FASTCALL_1(int32_t, 0x40FC80, thisCell); }

NH3API_DISABLE_WARNING_END
