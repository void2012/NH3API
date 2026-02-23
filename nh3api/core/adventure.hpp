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

#include "interface/dialogs.hpp" // TAdventureMapWindow
#include "base_manager.hpp"      // baseManager
#include "creatures.hpp"         // armyGroup, TCreatureType, also includes "terrain.hpp": TTerrainType
#include "hero_enums.hpp"        // hero_seqid
#include "map.hpp"               // MAP_HEIGHT, MAP_WIDTH, NewfullMap
#include "resources/sounds.hpp"  // e_looping_sound_id, sample, soundNode

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(4582)
NH3API_WARNING_MSVC_DISABLE(4583)
NH3API_WARNING_MSVC_DISABLE(26495)

enum type_adventure_cursor : int32_t
{
    ADV_ARROW_POINTER          = 0,
    ADV_WAIT_POINTER           = 1,
    ADV_HERO_INFO_POINTER      = 2,
    ADV_TOWN_INFO_POINTER      = 3,
    ADV_WALK_POINTER           = 4,
    ADV_SWORD_POINTER          = 5,
    ADV_BOAT_POINTER           = 6,
    ADV_MULTI_TURN_OFFSET      = 6,
    ADV_ANCHOR_POINTER         = 7,
    ADV_EXCHANGE_POINTER       = 8,
    ADV_EVENT_POINTER          = 9,
    ADV_BOAT_EVENT_POINTER     = 28,
    ADV_SCROLL_POINTER         = 32,
    ADV_SCROLL_NORTH           = 32,
    ADV_SCROLL_NORTHEAST       = 33,
    ADV_SCROLL_EAST            = 34,
    ADV_SCROLL_SOUTHEAST       = 35,
    ADV_SCROLL_SOUTH           = 36,
    ADV_SCROLL_SOUTHWEST       = 37,
    ADV_SCROLL_WEST            = 38,
    ADV_SCROLL_NORTHWEST       = 39,
    ADV_HIGHLIGHTED_POINTER    = 40,
    ADV_DIMENSION_DOOR_POINTER = 41,
    ADV_SKUTTLE_BOAT_POINTER   = 42
};

struct CNetMsgHandler;
#pragma pack(push, 4)
// Adventure map manager /
// Менеджер карты приключений.
// size = 0x3B8 = 952, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS advManager : public baseManager
{
    // structures
    public:
        struct vftable_t : baseManager::vftable_t
        {
            void (__thiscall* scalar_deleting_destructor)(advManager*, uint8_t);
        };

    // enums
    public:
        enum EBottomViewType
        {
            BVTYPE_NONE             = 0,
            BVTYPE_NEW_TURN         = 1,
            BVTYPE_KINGDOM          = 2,
            BVTYPE_HERO             = 3,
            BVTYPE_TOWN             = 4,
            BVTYPE_ENEMY_TURN       = 5,
            BVTYPE_RESOURCE_MESSAGE = 6,
            BVTYPE_MESSAGE          = 7,
            BVTYPE_HOLD             = 8
        };

    public:
        inline advManager() noexcept
            : advManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x406D80, this); }

        inline advManager(const nh3api::dummy_tag_t& tag) noexcept
            : CachedGraphics { tag },
              map_origin { tag },
              last_map_hover { tag },
              BottomViewText { tag }
        {}

        inline ~advManager() noexcept
        { THISCALL_1(void, 0x4F4080, this); }

        advManager(const advManager&)            = delete;
        advManager& operator=(const advManager&) = delete;
        advManager(advManager&&)                 = delete;
        advManager&& operator=(advManager&&)     = delete;

    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(advManager)

        NH3API_SCALAR_DELETING_DESTRUCTOR

    public:
        // Is screen point (x, y) within the map area /
        // Находится ли точка на экране (x, y) внутри части интерфейса карты?
        [[nodiscard]] int32_t InMapArea(int32_t x, int32_t y) const
        { return THISCALL_3(int32_t, 0x407A20, this, x, y); }

        // Get mouse adventure map cell coordinate /
        // Координата клетки карты, над которой наведена мышь.
        [[nodiscard]] type_point get_mouse_map_point() const
        {
            type_point result(nh3api::dummy_tag);
            (void) THISCALL_2(type_point, 0x407A70, this, &result);
            return result;
        }

        NewmapCell* DoAdvCommand(type_point& trigger_point)
        { return THISCALL_2(NewmapCell*, 0x407AE0, this, &trigger_point); }

        void DrawRolloverText(const char* text)
        { THISCALL_2(void, 0x40B040, this, text); }

        void SetRolloverText(NewmapCell* testCell, int32_t rx, int32_t ry)
        {
            if ( testCell )
                THISCALL_4(void, 0x40B0B0, this, testCell, rx, ry);
        }

        type_adventure_cursor get_normal_cursor(NewmapCell* currCell)
        {
            if ( currCell )
                return THISCALL_2(type_adventure_cursor, 0x40E1E0, this, currCell);

            return ADV_ARROW_POINTER;
        }

        void Reseed() noexcept
        { seedingValid = 0; }

        // Update the whole screen /
        // Обновить весь экран.
        void RedrawAdvScreen(bool bUpdate)
        { THISCALL_3(void, 0x417380, this, bUpdate, false); }

        void UpdateScreen()
        { THISCALL_3(void, 0x40F1D0, this, false, false); }

        int32_t GetCloudLookup(int32_t srcX, int32_t scrY, int32_t Z)
        { return THISCALL_4(int32_t, 0x40F820, this, srcX, scrY, Z); }

        // Get cell info by coordinates /
        // Получить информацию о клетке по координатам.
        [[nodiscard]] NewmapCell* GetCell(type_point point)
        { return THISCALL_2(NewmapCell*, 0x412B30, this, point); }

        // Get cell info by coordinates /
        // Получить информацию о клетке по координатам.
        [[nodiscard]] const NewmapCell* GetCell(type_point point) const
        { return THISCALL_2(NewmapCell*, 0x412B30, this, point); }

        // Get cell info by coordinates /
        // Получить информацию о клетке по координатам.
        [[nodiscard]] NewmapCell* GetCell(int8_t x, int8_t y, int8_t z)
        { return GetCell({ x, y, z }); }

        // Get cell info by coordinates /
        // Получить информацию о клетке по координатам.
        [[nodiscard]] const NewmapCell* GetCell(int8_t x, int8_t y, int8_t z) const
        { return GetCell({ x, y, z }); }

        void UpdateRadar(type_point origin,
                         bool updateFlag,
                         bool bPartialUpdate,
                         bool view_mines = false,
                         bool view_heros = false,
                         bool view_towns = false)
        { THISCALL_7(void, 0x412BA0, this, origin, updateFlag, bPartialUpdate, view_mines, view_heros, view_towns); }

        void UpdateRadar(bool updateFlag     = true,
                         bool bPartialUpdate = true,
                         bool view_mines     = false,
                         bool view_heroes    = false,
                         bool view_towns     = false)
        { THISCALL_6(void, 0x4136F0, this, updateFlag, bPartialUpdate, view_mines, view_heroes, view_towns); }

        void ClearBottomView()
        {
            if ( this->advWindow )
                this->advWindow->ClearBottomView();

            this->CurrentBottomView = BVTYPE_NONE;
        }

        void OverrideBottomView(EBottomViewType view, int32_t time)
        { THISCALL_3(void, 0x415CC0, this, view, time); }

        void UpdBottomView(bool force_update, bool bDrawWindow, bool bUpdate)
        { THISCALL_4(void, 0x415D40, this, force_update, bDrawWindow, bUpdate); }

        void BVResMsg(const char* cMsg, EGameResource iResType, int32_t iResQty)
        {
            if ( cMsg )
                THISCALL_4(void, 0x415FC0, this, cMsg, iResType, iResQty);
        }

        void BVMessage(const char* cMsg)
        {
            if ( cMsg )
                THISCALL_2(void, 0x416170, this, cMsg);
        }

        void DeactivateCurrTown(bool waitingPlayer)
        { THISCALL_2(void, 0x4174D0, this, waitingPlayer); }

        void DeactivateCurrHero(bool waitingPlayer)
        { THISCALL_2(void, 0x417500, this, waitingPlayer); }

        void MobilizeCurrHero(bool bInMove, bool waitingPlayer, bool draw_changes)
        { THISCALL_4(void, 0x417540, this, bInMove, waitingPlayer, draw_changes); }

        void DemobilizeCurrHero()
        { THISCALL_1(void, 0x4175E0, this); }

        void SetTownContext(int32_t townId, bool waitingPlayer, bool update)
        { THISCALL_4(void, 0x417790, this, townId, waitingPlayer, update); }

        void SetHeroContext(int32_t heroId, bool bInMove, bool waitingPlayer, bool draw_changes)
        { THISCALL_5(void, 0x417A80, this, heroId, bInMove, waitingPlayer, draw_changes); }

        void SetEnvironmentOrigin(type_point point, int32_t reset)
        { THISCALL_3(void, 0x418330, this, point, reset); }

        [[nodiscard]] e_looping_sound_id GetSoundId(int32_t x, int32_t y, int32_t z) const
        { return THISCALL_4(e_looping_sound_id, 0x418580, this, x, y, z); }

        void InsertSound(int32_t x, int32_t y, int32_t z, int32_t soundPriority, int32_t soundsType)
        { THISCALL_6(void, 0x418B70, this, x, y, z, soundPriority, soundsType); }

        void SeedTo(type_point target)
        { THISCALL_2(void, 0x419400, this, target); }

        void ForceNewHover()
        { THISCALL_1(void, 0x4194D0, this); }

        void TrimLoopingSounds(int32_t iMaxSoundsAllowed)
        { THISCALL_2(void, 0x41A140, this, iMaxSoundsAllowed); }

        void DisableButtons()
        { THISCALL_1(void, 0x41A240, this); }

        void EnableButtons()
        { THISCALL_1(void, 0x41A300, this); }

        [[nodiscard]] uint16_t GetRouteArray(int32_t x, int32_t y, int32_t z) const noexcept
        {
            if ( x >= 0 && y >= 0 && (z == 0 || z == 1))
                return this->pRouteArray[x + MAP_WIDTH * (y + z * MAP_HEIGHT)];

            return 0;
        }

        [[nodiscard]] const uint16_t* GetRouteArrayPtr(int32_t x, int32_t y, int32_t z) const
        {
            if ( x >= 0 && y >= 0 && (z == 0 || z == 1))
                return &this->pRouteArray[x + MAP_WIDTH * (y + z * MAP_HEIGHT)];

            return nullptr;
        }

        [[nodiscard]] uint16_t* GetRouteArrayPtr(int32_t x, int32_t y, int32_t z)
        {
            if ( x >= 0 && y >= 0 && (z == 0 || z == 1))
                return &this->pRouteArray[x + MAP_WIDTH * (y + z * MAP_HEIGHT)];

            return nullptr;
        }

        bool FindAdjacentMonster(type_point  point,
                                 type_point& result,
                                 type_point  excluded)
        { return THISCALL_4(bool, 0x41A3C0, this, point, &result, excluded); }

        // Begin combat /
        // Начать битву.
        /// @param point клетка, на которой происходит битва
        /// @param leftHero герой слева
        /// @param leftArmyGroup армия слева
        /// @param iRightPlayer  игрок справа
        /// @param rightTown осаждаемый город
        /// @param rightHero герой справа
        /// @param rightArmyGroup армия справа
        /// @param iSeed случайный сид
        /// @return winner side(left = 0, right = 1) / победитель(левый = 0, правый = 1)
        int32_t DoCombat(type_point point,
                         hero*      leftHero,
                         armyGroup* leftArmyGroup,
                         int32_t    iRightPlayer,
                         town*      rightTown,
                         hero*      rightHero,
                         armyGroup* rightArmyGroup,
                         int32_t    iSeed,
                         int32_t    bFinishHeroes    = true,
                         int32_t    alternate_layout = false)
        { return THISCALL_11(int32_t,
                             0x4AD160,
                             this,
                             point,
                             leftHero,
                             leftArmyGroup,
                             iRightPlayer,
                             rightTown,
                             rightHero,
                             rightArmyGroup,
                             iSeed,
                             bFinishHeroes,
                             alternate_layout); }

        // Erase an object /
        // Стереть объект на карте.
        /// @param thisCell клетка
        /// @param point    координаты клетки
        /// @param record   записать это событие
        void EraseObj(NewmapCell* thisCell, type_point point, bool record)
        { THISCALL_4(void, 0x4AA820, this, thisCell, point, record); }

        void EventSound(int32_t eventID, int32_t extraInfo)
        { THISCALL_3(void, 0x4AB100, this, eventID, extraInfo); }

        void FizzleCenter(int32_t whichSound)
        { THISCALL_2(void, 0x4AC8A0, this, whichSound); }

        // Erase an object with a sound effect /
        // Стереть объект на карте со звуковым эффектом
        /// @param eventCell клетка
        /// @param point     координаты клетки
        /// @param fizzleSound true = pickup resource sound, false = kill hero sound
        void EraseAndFizzle(NewmapCell* eventCell, type_point point, bool32_t fizzleSound)
        { THISCALL_4(void, 0x49DDE0, this, eventCell, point, fizzleSound); }

        [[nodiscard]] TreasureData* get_treasure_data(NewmapCell* this_cell)
        { return THISCALL_2(TreasureData*, 0x49ECB0, this, this_cell); }

        [[nodiscard]] const TreasureData* get_treasure_data(NewmapCell* this_cell) const
        { return THISCALL_2(TreasureData*, 0x49ECB0, this, this_cell); }

        [[nodiscard]] BlackBoxData* get_black_box(const ExtraInfoUnion* infounion)
        { return THISCALL_2(BlackBoxData*, 0x4A0890, this, infounion); }

        [[nodiscard]] const BlackBoxData* get_black_box(const ExtraInfoUnion* infounion) const
        { return THISCALL_2(BlackBoxData*, 0x4A0890, this, infounion); }

    // static functions
    public:
        [[nodiscard]] static int32_t get_like_modifier(hero* current_hero, TCreatureType creature)
        { return FASTCALL_2(int32_t, 0x4A7230, current_hero, creature); }

        [[nodiscard]] static int32_t get_force_modifier(float strength_ratio)
        { return FASTCALL_1(int32_t, 0x4A7330, strength_ratio); }

    // public variables
    public:
        // Multiplayer message handler /
        // Обработчик онлайн-сообщений.
        // offset: +0x38 = +56,  size = 0x4 = 4
        CNetMsgHandler* pNetMsgHandler;

        // Show FPS counter /
        // Показать (сломанный) FPS-счётчик.
        // offset: +0x3C = +60,  size = 0x1 = 1
        bool DebugShowFPS;

        // offset: +0x3D = +61,  size = 0x1 = 1
        bool DebugViewAll;

        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t advCommand;

        // Adventure map window /
        // Окно карты приключений.
        // offset: +0x44 = +68,  size = 0x4 = 4
        TAdventureMapWindow* advWindow;

        // Current hero route array /
        // Массив пути текущего героя.
        // offset: +0x48 = +72,  size = 0x4 = 4
        uint16_t* pRouteArray;

        // Show hero route? /
        // Показывать путь героя?
        // offset: +0x4C = +76,  size = 0x4 = 4
        bool32_t bShowRoute;

        // Are human player pathfinding routes still valid? /
        // Актуальны ли пути героя игрока-человека?
        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t seedingValid;

        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t fullySeeded;

        // Terrain of played music /
        // Почва, музыка которой проигрывается сейчас.
        // offset: +0x58 = +88,  size = 0x4 = 4
        TTerrainType lastTerrain;

        // Adventure map /
        // Карта приключений.
        // offset: +0x5C = +92,  size = 0x4 = 4
        NewfullMap* map;

        // Ground sprites /
        // Спрайты почв.
        // offset: +0x60 = +96,  size = 0x28 = 40
        std::array<const CSprite*, 10> groundTileset;

        // River sprites /
        // Спрайты рек.
        // offset: +0x88 = +136,  size = 0x14 = 20
        std::array<const CSprite*, 5> riverTileset;

        // Road sprites /
        // Спрайты дорог.
        // offset: +0x9C = +156,  size = 0x10 = 16
        std::array<const CSprite*, 4> roadTileset;

        // Map edges sprite(edg.def) /
        // Спрайт кромки карты(edg.def).
        // offset: +0xAC = +172,  size = 0x4 = 4
        CSprite* borderTileset;

        // Adventure map path arrow sprite(adag.def) /
        // Спрайты стрелок пути(adag.def).
        // offset: +0xB0 = +176,  size = 0x4 = 4
        CSprite* arrowTileset;

        // agemul.def, agemur.def, agemll.def, agemlr.def
        // offset: +0xB4 = +180,  size = 0x10 = 16
        std::array<const CSprite*, 4> gemIcons;

        // Shroud fill sprite(tshrc.def) /
        // Спрайт заполнения Терры Инкогнито(tshrc.def)
        // offset: +0xC4 = +196,  size = 0x4 = 4
        CSprite* starTileset;

        // Mini-map("radar") rectangle sprite(radar.def) /
        // Спрайт розовой рамки вида на мини-карте(radar.def)
        // offset: +0xC8 = +200,  size = 0x4 = 4
        CSprite* radarIcons;

        // Shroud border sprite(tshre.def) /
        // Спрайт границы Терры Инкогнито(tshre.def).
        // offset: +0xCC = +204,  size = 0x4 = 4
        CSprite* cloudIcons;

        union {
        // Cached game resources /
        // Игровые ресурсы, хранимые в кеше(advManager-а).
        // offset: +0xD0 = +208,  size = 0x10 = 16
        exe_vector<resource*> CachedGraphics;
        };

        // avwattak.def
        // offset: +0xE0 = +224,  size = 0x4 = 4
        CSprite* monAttackSprites;

        // Map focus point /
        // Координаты клетки фокуса карты (по центру экрана).
        // offset: +0xE4 = +228,  size = 0x4 = 4
        type_point map_origin;

        // Cursor cell position /
        // Координаты клетки карты, на которую наведен курсор.
        // offset: +0xE8 = +232,  size = 0x4 = 4
        type_point last_map_hover;

        // Cursor X coordinate /
        // Координата X курсора на экране
        // offset: +0xEC = +236,  size = 0x4 = 4
        int32_t lastHoverX;

        // Cursor Y coordinate /
        // Координата Y курсора на экране
        // offset: +0xF0 = +240,  size = 0x4 = 4
        int32_t lastHoverY;

        // offset: +0xF4 = +244,  size = 0x4 = 4
        int32_t scrollX;

        // offset: +0xF8 = +248,  size = 0x4 = 4
        int32_t scrollY;

    protected:
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0xFC = +252,  size = 0x4 = 4
        int32_t animFrame;

    public:
        // Animation frame during pause /
        // Номер кадра анимации карты во время паузы.
        // offset: +0x100 = +256,  size = 0x4 = 4
        int32_t animCtr;

        // Map animation is paused /
        // Анимация карты на паузе.
        // offset: +0x104 = +260,  size = 0x1 = 1
        bool animCtrPaused;

    protected:
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x108 = +264,  size = 0x4 = 4
        int32_t flagFrame;

    public:
        // Cursor sprites /
        // Спрайты курсоров.
        // offset: +0x10C = +268,  size = 0x48 = 72
        std::array<CSprite*, 18> cursorIcons;

        // Boat sprites /
        // Спрайты лодок.
        // offset: +0x154 = +340,  size = 0xC = 12
        std::array<CSprite*, 3> boatIcons;

        // Boat froth sprites /
        // Спрайты следов лодки("пены" перед лодкой).
        // offset: +0x160 = +352,  size = 0xC = 12
        std::array<CSprite*, 3> boatFrothIcons;

        // Heroes flags sprites /
        // Спрайты флагов героев.
        // offset: +0x16C = +364,  size = 0x20 = 32
        std::array<CSprite*, 8> flagIcons;

        // Boat flag sprites /
        // Спрайты флагов лодок.
        // offset: +0x18C = +396,  size = 0x60 = 96
        std::array<CSprite*, 24> boatFlagIcons;

        // Is current hero visible? /
        // Виден ли текущий герой?
        // offset: +0x1EC = +492,  size = 0x1 = 1
        bool heroVisible;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Current hero, OBJECT_HERO or OBJECT_BOAT /
        // Тип объекта героя(герой, OBJECT_HERO или лодка, OBJECT_BOAT).
        // offset: +0x1F0 = +496,  size = 0x4 = 4
        TAdventureObjectType heroType;

        // Hero current frame (direction) /
        // Направление в которую повёрнут текущий герой(кадр).
        // offset: +0x1F4 = +500,  size = 0x4 = 4
        int32_t heroDirection;

    protected:
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x1F8 = +504,  size = 0x4 = 4
        int32_t heroBaseFrame;

    public:
        // Current hero sprite sequence /
        // Текущая группа спрайта героя.
        // offset: +0x1FC = +508,  size = 0x4 = 4
        hero_seqid heroSequence;

        // Current hero sprite frame /
        // Текущий кадр героя.
        // offset: +0x200 = +512,  size = 0x4 = 4
        int32_t heroFrameCount;

    protected:
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x204 = +516,  size = 0x4 = 4
        bool32_t heroTurning;

        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x208 = +520,  size = 0x4 = 4
        int32_t heroDrawn;

    public:
        // Current hero is mobile /
        // Текущий герой не в спящем режиме?
        // offset: +0x20C = +524,  size = 0x1 = 1
        bool bCurHeroMobile;

    protected:
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x210 = +528,  size = 0x4 = 4
        int32_t iShowMode;

        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x214 = +532,  size = 0x4 = 4
        bool32_t bForceCompleteDraw;

    public:
        // Current attacked monster object index /
        // Индекс объекта текущего атакованного монстра.
        // offset: +0x218 = +536,  size = 0x4 = 4
        int32_t monAttackObjIndex;

        // Current attacked monster sprite frame /
        // Кадр спрайта атакованного монстра.
        // offset: +0x21C = +540,  size = 0x4 = 4
        int32_t monAttackSpriteIndex;

        // Attacked monster sprite direction changed? /
        // Направление спрайта атакованного монстра повёрнуто?
        // offset: +0x220 = +544,  size = 0x4 = 4
        bool32_t monAttackFlip;

        // offset: +0x224 = +548,  size = 0x4 = 4
        int32_t touchedSounds;

        // Currently played sounds /
        // Проигрываемые на данный момент звуки объектов.
        // offset: +0x228 = +552,  size = 0x20 = 32
        std::array<soundNode, 4> soundArray;

        // Looped adventure object sounds /
        // Звуки объектов.
        // offset: +0x248 = +584,  size = 0x118 = 280
        std::array<sample*, max_environment_sounds> loopedSample;

        // Hero walk sounds /
        // Звуки передвижения героя
        // offset: +0x360 = +864,  size = 0x2C = 44
        std::array<sample*, 11> heroSamples;

        // Is Heroes 3 logo shield showed instead of mini-map? /
        // Показывается ли щит Героев 3 вместо мини-карты?
        // offset: +0x38C = +908,  size = 0x4 = 4
        bool32_t bHeroLogoShowing;

        // Hero is currently moving? /
        // Герой двигается?
        // offset: +0x390 = +912,  size = 0x1 = 1
        bool bHeroMoving;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x394 = +916,  size = 0x4 = 4
        EBottomViewType CurrentBottomView;

        // offset: +0x398 = +920,  size = 0x4 = 4
        EBottomViewType BottomViewOverride;

        // offset: +0x39C = +924,  size = 0x4 = 4
        int32_t BottomViewOverrideEndTime;

        // offset: +0x3A0 = +928,  size = 0x4 = 4
        int32_t BottomViewResource;

        // offset: +0x3A4 = +932,  size = 0x4 = 4
        int32_t BottomViewResourceQty;

        union {
        // offset: +0x3A8 = +936,  size = 0x10 = 16
        exe_string BottomViewText;
        };

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x3B8, advManager);

inline advManager* const& gpAdvManager = get_global_var_ref(0x6992B8, advManager*);

// Show adventure map animations /
// Показывать анимации на карте приключений.
inline bool32_t& bShowIt = get_global_var_ref(0x698A10, bool32_t);

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63A678, advManager)

NH3API_WARNING(pop)
