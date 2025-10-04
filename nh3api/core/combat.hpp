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

#include "nh3api_std/exe_set.hpp" // exe_set<T>
#include "interface/widgets.hpp" // widgets
#include "nh3api_std/memory.hpp" // byte_t
#include "objects.hpp" // hero, town, etc.
#include "army.hpp" // army, TCreatureType
#include "hexcell.hpp" // hexcell

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

enum type_combat_cursor : int32_t
{
    SELECT_NULL                  = 0, //
    SELECT_MOVE                  = 1, //
    SELECT_FLY                   = 2, //
    SELECT_SPECIAL_ATTACK        = 3, //
    SELECT_TENT                  = 4, //
    SELECT_ARMY_INFO             = 5, //
    CBT_ARROW_POINTER            = 6, //
    const_attack_northeast       = 7, //
    SELECT_ATTACK                = 7, //
    const_attack_east            = 8, //
    const_attack_southeast       = 9, //
    const_attack_southwest       = 10, //
    const_attack_west            = 11, //
    const_attack_northwest       = 12, //
    const_attack_up              = 13, //
    const_attack_down            = 14, //
    SELECT_SPECIAL_ATTACK_BROKEN = 15, //
    SELECT_ATTACK_WALL           = 16, //
    SELECT_FIRST_AID             = 17, //
    SELECT_SACRIFICE             = 18, //
    SELECT_TELEPORT              = 19, //
    SELECT_CREATURE_SPELL        = 20, //
    SELECT_ENEMY_TENT            = 21, //
    const_select_tower_info      = 22 //
};

enum ECombatAction : int32_t
{
    COMBAT_ACTION_NONE = 0,
    COMBAT_ACTION_CAST_SPELL = 1,
    COMBAT_ACTION_ARMY_MOVE = 2,
    COMBAT_ACTION_ARMY_DEFENSE = 3,
    COMBAT_ACTION_RETREAT = 4,
    COMBAT_ACTION_SURRENDER = 5,
    COMBAT_ACTION_ARMY_MELEE_ATTACK = 6,
    COMBAT_ACTION_ARMY_SHOOT_ATTACK = 7,
    COMBAT_ACTION_ARMY_WAIT = 8,
    COMBAT_ACTION_CATAPULT_SHOOT = 9,
    COMBAT_ACTION_ARMY_CAST_SPELL = 10,
    COMBAT_ACTION_FIRST_AID_TENT_CURE = 11,
    COMBAT_ACTION_SKIP_TURN = 12
};

enum TFortificationLevel : int32_t
{
    eFortificationNone    = 0, //
    eFortificationFort    = 1, //
    eFortificationCitadel = 2, //
    eFortificationCastle  = 3 //
};

#pragma pack(push, 4)
// Combat manager /
// Менеджер битвы.
// size = 0x140EC = 82156, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS combatManager : public baseManager
{
    public:
        struct vftable_t : baseManager::vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(combatManager*, uint8_t);
        };

        #pragma pack(push, 2)
        // size = 0xC = 12, align = 2
        struct adjacency_array
        {
            // Adjacent hexes(clockwise) /
            // Гексы, окружающие эту клетку.(по часовой стрелке)
            // offset: +0x0 = +0,  size = 0xC = 12
            std::array<int16_t, 6> adjacent;
        };
        #pragma pack(pop)

        // size = 0x44 = 68, align = 4
        struct SElevationOverlay
        {
            // offset: +0x0 = +0,  size = 0x2 = 2
            uint16_t terrainMask;

            // offset: +0x2 = +2,  size = 0x2 = 2
            uint16_t specialTerrainMask;

            // offset: +0x4 = +4,  size = 0x4 = 4
            int32_t x;

            // offset: +0x8 = +8,  size = 0x4 = 4
            int32_t y;

            // offset: +0xC = +12,  size = 0x34 = 52
            std::array<int16_t, 26> blockedSquares;

            // offset: +0x40 = +64,  size = 0x4 = 4
            const char* FileName;

        };

        // size = 0x10 = 16, align = 4
        struct SCmbtHero
        {
            // offset: +0x0 = +0,  size = 0x4 = 4
            const char* SpriteName;

            // offset: +0x4 = +4,  size = 0x4 = 4
            int32_t castX;

            // offset: +0x8 = +8,  size = 0x4 = 4
            int32_t castY;

            // offset: +0xC = +12,  size = 0x4 = 4
            int32_t castFrame;
        };

        // size = 0x24 = 36, align = 4
        struct TWallTraits
        {
            // offset: +0x0 = +0,  size = 0x2 = 2
            int16_t x;

            // offset: +0x2 = +2,  size = 0x2 = 2
            int16_t y;

        protected:
            NH3API_MAYBE_UNUSED
            // offset: +0x3 = +3,  size = 0x2 = 2
            byte_t gap_3[2];

        public:
            // offset: +0x4 = +4,  size = 0x2 = 2
            int16_t hex;

            // offset: +0x8 = +8,  size = 0x14 = 20
            std::array<const char*, 5> filenames;

            // offset: +0x1C = +28,  size = 0x4 = 4
            const char* name;

            // offset: +0x20 = +32,  size = 0x2 = 2
            int16_t hitpoints;

        protected:
            NH3API_MAYBE_UNUSED
            // offset: +0x22 = +34,  size = 0x2 = 2
            byte_t gap_22[2];

        };

        // size = 0x14 = 20, align = 4
        struct TObstacleInfo
        {
            // offset: +0x0 = +0,  size = 0x4 = 4
            uint32_t backgroundMask;

            // offset: +0x4 = +4,  size = 0x1 = 1
            uint8_t height;

            // offset: +0x5 = +5,  size = 0x1 = 1
            uint8_t width;

            // offset: +0x6 = +6,  size = 0x1 = 1
            uint8_t numSquares;

            // offset: +0x7 = +7,  size = 0x1 = 1
            uint8_t underlay;

            // offset: +0x8 = +8,  size = 0x8 = 8
            std::array<int8_t, 8> sOffsets;

            // offset: +0x10 = +16,  size = 0x4 = 4
            const char* FileName;

        };

        // size = 0x18 = 24, align = 4
        struct TObstacle
        {
            // offset: +0x0 = +0,  size = 0x4 = 4
            CSprite* sprite;

            // offset: +0x4 = +4,  size = 0x4 = 4
            const TObstacleInfo* info;

            // offset: +0x8 = +8,  size = 0x1 = 1
            uint8_t grid_index;

            // offset: +0x9 = +9,  size = 0x1 = 1
            int8_t owner;

            // offset: +0xA = +10,  size = 0x1 = 1
            bool is_visible;

        protected:
            NH3API_MAYBE_UNUSED
            // offset: +0xB = +11,  size = 0x1 = 1
            byte_t gap_B[1];
        
        public:
            // offset: +0xC = +12,  size = 0x4 = 4
            int32_t damage;

            // offset: +0x10 = +16,  size = 0x4 = 4
            int32_t duration;

            // offset: +0x14 = +20,  size = 0x4 = 4
            TSpellEffectID dispel_effect;

        };

        enum TWallSection : int32_t
        {
            eWallSectionDoor              = 0, //
            eWallSectionDoorRope          = 1, //
            eWallSectionMoat              = 2, //
            eWallSectionMoatLip           = 3, //
            eWallSectionBackWall          = 4, //
            eWallSectionUpperTower        = 5, //
            eWallSectionUpperWall         = 6, //
            eWallSectionUpperButtress     = 7, //
            eWallSectionMidUpperWall      = 8, //
            eWallSectionGate              = 9, //
            eWallSectionMidLowerWall      = 10, //
            eWallSectionLowerButtress     = 11, //
            eWallSectionLowerWall         = 12, //
            eWallSectionLowerTower        = 13, //
            eWallSectionMainBuilding      = 14, //
            eWallSectionMainBuildingCover = 15, //
            eWallSectionLowerTowerCover   = 16, //
            eWallSectionUpperTowerCover   = 17, //
            kNumWallSections              = 18, //
        };

        // size = 0xC = 12, align = 4
        struct TWallTarget
        {
            // offset: +0x0 = +0,  size = 0x2 = 2
            int16_t target_hex;

            // offset: +0x2 = +2,  size = 0x2 = 2
            int16_t blocked_row;

            // offset: +0x4 = +4,  size = 0x2 = 2
            int16_t hit_x;

            // offset: +0x6 = +6,  size = 0x2 = 2
            int16_t hit_y;

            // offset: +0x8 = +8,  size = 0x4 = 4
            TWallSection wall;

        };

        // size = 0x24 = 36, align = 4
        struct TArcher
        {
            // offset: +0x0 = +0,  size = 0x4 = 4
            TCreatureType Type;

            // offset: +0x4 = +4,  size = 0x4 = 4
            CSprite* Sprite;

            // offset: +0x8 = +8,  size = 0x4 = 4
            CSprite* Missile;

            // offset: +0xC = +12,  size = 0x4 = 4
            int32_t X;

            // offset: +0x10 = +16,  size = 0x4 = 4
            int32_t Y;

            // offset: +0x14 = +20,  size = 0x4 = 4
            int32_t Facing;

            // offset: +0x18 = +24,  size = 0x4 = 4
            int32_t Sequence;

            // offset: +0x1C = +28,  size = 0x4 = 4
            int32_t Frame;

            // offset: +0x20 = +32,  size = 0x4 = 4
            int32_t Amount;

        };

        // size = 0x20 = 32, align = 4
        struct TArcherTraits
        {
            // offset: +0x0 = +0,  size = 0x4 = 4
            TCreatureType CreatureType;

            // offset: +0x4 = +4,  size = 0x4 = 4
            int32_t MainBuildingX;

            // offset: +0x8 = +8,  size = 0x4 = 4
            int32_t MainBuildingY;

            // offset: +0xC = +12,  size = 0x4 = 4
            int32_t LowerTowerX;

            // offset: +0x10 = +16,  size = 0x4 = 4
            int32_t LowerTowerY;

            // offset: +0x14 = +20,  size = 0x4 = 4
            int32_t UpperTowerX;

            // offset: +0x18 = +24,  size = 0x4 = 4
            int32_t UpperTowerY;

            // offset: +0x1C = +28,  size = 0x4 = 4
            const char* MissileName;

        };

        enum TDoorStatus : int32_t
        {
            DOOR_BROKEN = 0, //
            DOOR_DOWN   = 1, //
            DOOR_DOWN_1 = 2, //
            DOOR_UP     = 3 //
        };

        enum TArcherID : int32_t
        {
            eArcherMainBuilding = 0, //
            eArcherLowerTower   = 1, //
            eArcherUpperTower   = 2, //
            kNumArchers         = 3 //
        };

        enum
        {
            INVALID_HEX_INDEX            = -1, //
            LEFT_AMMOCART_GRIDINDEX      = 18, //
            RIGHT_AMMOCART_GRIDINDEX     = 32, //
            LEFT_BALLISTA_GRIDINDEX      = 51, //
            RIGHT_BALLISTA_GRIDINDEX     = 67, //
            DOOR_GRIDINDEX               = 96, //
            LEFT_FIRSTAID_GRIDINDEX      = 153, //
            RIGHT_FIRSTAID_GRIDINDEX     = 169, //
            LEFT_CATAPULT_GRIDINDEX      = 119, //
            RIGHT_CATAPULT_GRIDINDEX     = 135, //
            LOWER_WALL_RIGHT_GRIDINDEX   = 200, //
            LOWER_TOWER_DRAW_GRIDINDEX   = 251, //
            LEFT_GENERAL_GRIDINDEX       = 252, //
            RIGHT_GENERAL_GRIDINDEX      = 253, //
            MAIN_BUILDING_GRIDINDEX      = 254, //
            UPPER_TOWER_GRIDINDEX        = 255, //
            DRAWBRIDGE_HEX_Y             = 5, //
            DP_MIN                       = 0, //
            DP_WALLS                     = 0, //
            DP_DEAD_ARMIES               = 1, //
            DP_OBSTACLES                 = 2, //
            DP_WALKING_UP                = 3, //
            DP_NORMAL                    = 4, //
            DP_WINCING                   = 5, //
            DP_ATTACKING                 = 6, //
            DP_WALKING_DOWN              = 7, //
            DP_MAX                       = 7, //
            DP_ALL                       = 8, //
            NUM_SIDES                    = 2, //
            MAX_COMBAT_ARMIES            = 20, //
            BASE_COMBAT_ANIMATION_PERIOD = 50, //
            COMBAT_HERO_FIDGET_TIME      = 4500, //
            MAX_WALL_LEVELS              = 5, //
            MAX_COMBAT_HEXES             = 187
        };

    public:
        NH3API_FORCEINLINE
        combatManager() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(combatManager, baseManager)
        { THISCALL_1(void, 0x462340, this); }

        NH3API_FORCEINLINE
        combatManager(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            :
            #if NH3API_STD_INITIALIZER_LIST
            EagleEyeSpellLearned{tag, tag},
            Armies
            {{
            {tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag},
            {tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag, tag}
            }},
            #endif
            Obstacles(tag)
        {}

        combatManager(const combatManager&) NH3API_DELETED_FUNCTION
        combatManager& operator=(const combatManager&) NH3API_DELETED_FUNCTION

        #if NH3API_CHECK_CPP11
        combatManager(combatManager&&) = delete;
        combatManager& operator=(combatManager&&) = delete;
        #endif

        NH3API_FORCEINLINE
        ~combatManager() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4F3FD0, this); }

    public:
        // Hex is valid /
        // Принадлежит ли клетка полю боя?
        static bool ValidHex(int32_t iHex)
        { return (iHex >= 0) && (iHex < MAX_COMBAT_HEXES); }

        NH3API_NODISCARD bool IsQuickCombat() const
        { return THISCALL_1(bool, 0x46A080, this); }

        NH3API_NODISCARD bool is_computer_action() const
        { return THISCALL_1(bool, 0x474520, this); }

        NH3API_NODISCARD int32_t get_total_combat_value(int32_t side, int32_t lowest_attack, int32_t lowest_defense, bool include_cripples) const
        { return THISCALL_5(int32_t, 0x41EC40, this, side, lowest_attack, lowest_defense, include_cripples); }

        NH3API_NODISCARD int32_t compute_fire_shield_damage(int32_t damage, const army* attacker, const army* target, int32_t target_hits) const
        { return THISCALL_5(int32_t, 0x4225C0, this, damage, attacker, target, target_hits); }

        // Creatures turn order /
        // Порядок хода существ.
        void find_move_order(exe_vector<army*>* order)
        { THISCALL_2(void, 0x41F2C0, this, order); }

        NH3API_NODISCARD bool can_cast_spells(int32_t side, bool hero_spell) const
        { return THISCALL_3(bool, 0x41FA10, this, side, hero_spell); }

        // Get hex index by screen coordinates /
        // Индекс клетки по координатам точки на экране.
        NH3API_NODISCARD int32_t GetGridIndex(int32_t x, int32_t y) const
        { return THISCALL_3(int32_t, 0x464380, this, x, y); }

        // Combat is over /
        // Битва завершилась.
        NH3API_NODISCARD bool CombatIsOver() const
        { return THISCALL_1(bool, 0x465410, this); }

        // Is <this_side> winner? /
        // Является ли <this_side> победителем в битве?
        NH3API_NODISCARD bool IsWinner(int32_t this_side) const
        { return THISCALL_2(bool, 0x465490, this, this_side); }

        NH3API_NODISCARD static TWallTargetId GetTargetWallIndex(int32_t grid_index)
        { return FASTCALL_1(TWallTargetId, 0x465550, grid_index); }

        void ResetHitByCreature()
        { THISCALL_1(void, 0x465BC0, this); }

        void PlaceObstacle(const TObstacle& obstacle, int32_t obstacle_index, int32_t hex, hexcell::TAttributes attributes)
        { THISCALL_5(void, 0x466590, this, &obstacle, obstacle_index, hex, attributes); }

        void RemoveObstacle(int32_t index)
        { THISCALL_2(void, 0x466710, this, index); }

        void MakeCreaturesVanish()
        { THISCALL_1(void, 0x4669E0, this); }

        void LowerDoor()
        { THISCALL_1(void, 0x466DA0, this); }

        void RaiseDoor()
        { THISCALL_1(void, 0x466EC0, this); }

        NH3API_NODISCARD static bool InCastle(int32_t index)
        { return FASTCALL_1(bool, 0x467040, index); }

        NH3API_NODISCARD static bool InCastle(const army* arg)
        { return FASTCALL_1(bool, 0x467040, arg->gridIndex); }

        NH3API_NODISCARD static bool LeftOfMoat(int32_t index)
        { return FASTCALL_1(bool, 0x467070, index); }

        NH3API_NODISCARD bool is_adjacent(int32_t first, int32_t second) const
        { return THISCALL_3(bool, 0x4670A0, this, first, second); }

        NH3API_NODISCARD bool ShotIsThroughWall(army* shooter, int32_t sourceIndex, int32_t destIndex) const
        { return THISCALL_4(bool, 0x4670F0, this, shooter, sourceIndex, destIndex); }

        NH3API_NODISCARD bool ShotIsNotOptimal(army* attacker, army* defender) const
        { return THISCALL_3(bool, 0x4671E0, this, attacker, defender); }

        NH3API_NODISCARD bool InLineOfSight(int32_t sourceIndex, int32_t destIndex) const
        { return THISCALL_3(bool, 0x467420, this, sourceIndex, destIndex); }

        void ShootBallisticMissile(int32_t startX,
                                   int32_t startY,
                                   int32_t destX,
                                   int32_t destY,
                                   const CSprite* const missile)
        { THISCALL_6(void, 0x4675E0, this, startX, startY, destX, destY, missile); }

        void ShootAnimatedMissile(int32_t startX,
                                  int32_t startY,
                                  int32_t destX,
                                  int32_t destY,
                                  int32_t nSprites,
                                  const float* const angles,
                                  const char* const* const file_names)
        { THISCALL_8(void, 0x467990, this, startX, startY, destX, destY, nSprites, angles, file_names); }

        void ShootMissile(int32_t startX,
                          int32_t startY,
                          int32_t destX,
                          int32_t destY,
                          const float* const angles,
                          const CSprite* const missile)
        { THISCALL_7(void, 0x467E00, this, startX, startY, destX, destY, angles, missile); }

        void RemoveArmyFromGrid(const army* arg)
        { THISCALL_2(void, 0x468310, this, arg); }

        void PlaceArmyInGrid(const army* arg, int32_t hex)
        { THISCALL_3(void, 0x4683A0, this, arg, hex); }

        void PowEffect(TSpellEffectID spellEffect, bool bResetLimitCreature)
        { THISCALL_3(void, 0x468570, this, spellEffect, static_cast<bool32_t>(bResetLimitCreature)); }

        NH3API_NODISCARD bool enemy_is_adjacent(const army* current_army, int32_t grid_index, const army* excluded)
        { return THISCALL_4(bool, 0x4691E0, this, current_army, grid_index, excluded); }

        NH3API_NODISCARD static int32_t get_distance(int32_t start, int32_t stop)
        { return FASTCALL_2(int32_t, 0x469250, start, stop); }

        NH3API_NODISCARD bool HexIsBlocked(int32_t adjacent_cell) const
        { return THISCALL_2(bool, 0x4695F0, this, adjacent_cell); }

        void damage_message(const char* attacker, int32_t attacker_qty, int32_t damage, const army* defender, int32_t deaths)
        { THISCALL_6(void, 0x469670, this, attacker, attacker_qty, damage, defender, deaths); }

        NH3API_NODISCARD bool IsInMoat(int32_t hex, int32_t* index) const
        { return THISCALL_3(bool, 0x4699A0, this, hex, index); }

        void CalculateGainedExperience(int32_t side, int32_t* experience_gained)
        { THISCALL_3(void, 0x469F30, this, side, experience_gained); }

        NH3API_NODISCARD bool is_outside_placement_boundry(int32_t group, int32_t index) const
        { return THISCALL_3(bool, 0x475D20, this, group, index); }

        NH3API_NODISCARD bool valid_wall_target(TWallTargetId wall_target) const
        { return !!THISCALL_2(bool32_t, 0x475D70, this, wall_target); }

        // Current side surrender cost /
        // Стоимость откупа у текущей стороны битвы.
        NH3API_NODISCARD int32_t get_surrender_cost() const
        { return THISCALL_1(int32_t, 0x477330, this); }

        army* AddArmy(int32_t iSide, int32_t iMonType, int32_t iMonQty, int32_t iGridIndex, int32_t iSetAttributes, bool32_t bFizzleItIn)
        { return THISCALL_7(army*, 0x479A30, this, iSide, iMonType, iMonQty, iGridIndex, iSetAttributes, bFizzleItIn); }

        int32_t ResetLimitCreature()
        { return THISCALL_1(int32_t, 0x493290, this); }

        void UpdateCombatArea()
        { THISCALL_1(void, 0x493300, this); }

        int32_t UpdateGrid(bool bPostGridIsClean, bool bSetupGrid)
        { return THISCALL_3(int32_t, 0x4934B0, this, static_cast<bool32_t>(bPostGridIsClean), static_cast<bool32_t>(bSetupGrid)); }

        void UpdateMouseGrid(int32_t iNewMouseGridIndex, int32_t bAllowDuringAction)
        { THISCALL_3(void, 0x493F10, this, iNewMouseGridIndex, bAllowDuringAction); }

        void DrawFrame(bool update,
                       bool bLimitCreatureEffect,
                       bool bLimitDraw,
                       int32_t iDelay,
                       bool bRefreshBackground,
                       bool bDoDelayTil)
        { THISCALL_7(void, 0x493FC0,
                     this,
                     static_cast<bool32_t>(update),
                     static_cast<bool32_t>(bLimitCreatureEffect),
                     static_cast<bool32_t>(bLimitDraw),
                     iDelay,
                     static_cast<bool32_t>(bRefreshBackground),
                     static_cast<bool32_t>(bDoDelayTil)); }

        void ComputeExtent(CSprite* sprite, int32_t sequence, int32_t frame, int32_t x, int32_t y, SLimitData* pLimitData, int32_t isFlipped, bool saveBiggestExtent)
        { THISCALL_9(void, 0x495AD0, this, sprite, sequence, frame, x, y, pLimitData, isFlipped, saveBiggestExtent); }

        void ComputeMaxExtent()
        { THISCALL_1(void, 0x495770, this); }

        void CycleCombatScreen()
        { THISCALL_1(void, 0x495C50, this); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(combatManager)

        NH3API_SCALAR_DELETING_DESTRUCTOR

    // static variables
    public:
        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const uint8_t, 12>& moatCell
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BCE8, const std::array<const uint8_t, 12>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const uint8_t, 12>& moat2Cell
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BCF4, const std::array<const uint8_t, 12>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const uint8_t, 12>& wallCell
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BD00, const std::array<const uint8_t, 12>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const uint8_t, 12>& leftCell
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BD0C, const std::array<const uint8_t, 12>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const uint32_t, MAX_TOWNS>& moatDamage
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BD18, const std::array<const uint32_t, MAX_TOWNS>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const std::array<const SCmbtHero, 2>, MAX_TOWNS>& sCmbtHero
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BD40, const std::array<const std::array<const typename combatManager::SCmbtHero, 2>, MAX_TOWNS>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const TWallTarget, 8>& wallTargets
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BE60, const std::array<const typename combatManager::TWallTarget, 8>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const SElevationOverlay, 34>& sElevationOverlay
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63BEC0, const std::array<const typename combatManager::SElevationOverlay, 34>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const TObstacleInfo, 91>& ObstacleInfo
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63C7C8, const std::array<const typename combatManager::TObstacleInfo, 91>));

        NH3API_INLINE_STATIC_VARIABLE
        const TObstacleInfo& QuicksandInfo
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63CEE8, const typename combatManager::TObstacleInfo));

        NH3API_INLINE_STATIC_VARIABLE
        const TObstacleInfo& LandMineInfo
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63CF00, const typename combatManager::TObstacleInfo));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const TObstacleInfo, 5>& WallObstacleInfo
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63CF18, const std::array<const typename combatManager::TObstacleInfo, 5>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const float, 3>& CombatSpeedMod
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63CF7C, const std::array<const float, 3>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const TArcherTraits, MAX_TOWNS>& ArcherTraits
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x63CF88, const std::array<const typename combatManager::TArcherTraits, MAX_TOWNS>));

        NH3API_INLINE_STATIC_VARIABLE
        const std::array<const std::array<const TWallTraits, 18>, MAX_TOWNS>& akWallTraits
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x66D848, const std::array<const std::array<const typename combatManager::TWallTraits, 18>, MAX_TOWNS>));


    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x4 = 4
        void* netMsgHandlerPause;
        //CNetMsgHandlerPause* netMsgHandlerPause;

        // offset: +0x3C = +60,  size = 0x4 = 4
        ECombatAction iNextAction;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t iNextActionExtra;

        // offset: +0x44 = +68,  size = 0x4 = 4
        int32_t iNextActionGridIndex;

        // offset: +0x48 = +72,  size = 0x4 = 4
        int32_t iNextActionGridIndex2;

        // offset: +0x4C = +76,  size = 0xBB = 187
        std::array<bool, MAX_COMBAT_HEXES> iLastDrawGridShade;

        // offset: +0x107 = +263,  size = 0xBB = 187
        std::array<bool, MAX_COMBAT_HEXES> iCurDrawGridShade;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1C2 = +450,  size = 0x1 = 1
        byte_t gap_1C2[2];

    public:
        // offset: +0x1C4 = +452,  size = 0x51D0 = 20944
        std::array<hexcell, MAX_COMBAT_HEXES> cell;

        // offset: +0x5394 = +21396,  size = 0x4 = 4
        int32_t combatTerrain;

        // offset: +0x5398 = +21400,  size = 0x4 = 4
        int32_t combatFringe;

        // offset: +0x539C = +21404,  size = 0x4 = 4
        int32_t iCombatCycleType;

        // offset: +0x53A0 = +21408,  size = 0x4 = 4
        int32_t iElevationOverlay;

        // offset: +0x53A4 = +21412,  size = 0x4 = 4
        TDoorStatus iDoorStatus;

        // offset: +0x53A8 = +21416,  size = 0x1 = 1
        bool bMoatOn;

        // offset: +0x53A9 = +21417,  size = 0x1 = 1
        bool moatIsWide;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x14030 = +81968,  size = 0x1 = 1
        byte_t gap_53AA[2];

    public:
        // offset: +0x53AC = +21420,  size = 0x4 = 4
        Bitmap16Bit* SaveScreenPreGrid;

        // offset: +0x53B0 = +21424,  size = 0x4 = 4
        Bitmap16Bit* SaveScreenPostGrid;

        // offset: +0x53B4 = +21428,  size = 0x4 = 4
        Bitmap16Bit* combatMouseBackground;

        // offset: +0x53B8 = +21432,  size = 0x4 = 4
        int32_t bBackgroundDrawn;

        // offset: +0x53BC = +21436,  size = 0x4 = 4
        NewmapCell* EventCell;

        // offset: +0x53C0 = +21440,  size = 0x4 = 4
        EMagicTerrain magic_terrain;

        // offset: +0x53C4 = +21444,  size = 0x1 = 1
        bool OnBoats;

        // offset: +0x53C5 = +21445,  size = 0x1 = 1
        bool OnBeach;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x14030 = +81968,  size = 0x1 = 1
        byte_t gap_53C6[2];

    public:
        // offset: +0x53C8 = +21448,  size = 0x4 = 4
        town* combatTown;

        // offset: +0x53CC = +21452,  size = 0x8 = 8
        std::array<hero*, NUM_SIDES> Heroes;

        // offset: +0x53D4 = +21460,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> iSideSpellPower;

        // offset: +0x53DC = +21468,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> PlayDoh;

        // offset: +0x53DE = +21470,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> PlayYeah;

        // offset: +0x53E0 = +21472,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> DohPlayedThisRound;

        // offset: +0x53E2 = +21474,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> YeahPlayedThisRound;

        // offset: +0x53E4 = +21476,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> cmbtHeroFrameType;

        // offset: +0x53EC = +21484,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> cmbtHeroFrameIndex;

        // offset: +0x53F4 = +21492,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> cmbtHeroDataSet;

        // offset: +0x53FC = +21500,  size = 0x8 = 8
        std::array<uint32_t, NUM_SIDES> cmbtHeroLastFidgetTime;

        // offset: +0x5404 = +21508,  size = 0x8 = 8
        std::array<CSprite*, NUM_SIDES> cmbtHero;

        // offset: +0x540C = +21516,  size = 0x8 = 8
        std::array<CSprite*, NUM_SIDES> cmbtHeroFlag;

        // offset: +0x5414 = +21524,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> cmbtHeroFlagFrame;

        // offset: +0x541C = +21532,  size = 0x20 = 32
        std::array<SLimitData, NUM_SIDES> sCmbtHeroLimitData;

        // offset: +0x543C = +21564,  size = 0x20 = 32
        std::array<SLimitData, NUM_SIDES> sCmbtHeroFlagLimitData;

        typedef exe_set<SpellID, 0x694FA0, 0x694FA4> TSpellSet;

        union {
        // Spells learned with eagle eye /
        // Заклинания, изученные с помощью орлиного глаза.
        // offset: +0x545C = +21596,  size = 0x20 = 32
        std::array<TSpellSet, NUM_SIDES> EagleEyeSpellLearned;
        };

        // offset: +0x547C = +21628,  size = 0x28 = 40
        std::array<std::array<bool, MAX_COMBAT_ARMIES>, NUM_SIDES> ArmyEffected;

        // Is side human player /
        // Является ли одна из сторон игроком-человеком.
        // offset: +0x54A4 = +21668,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> IsHuman;

        // Is side local machine player /
        // Является ли одна из сторон игроком-человеком, играющим за текущим компьютером.
        // offset: +0x54A6 = +21670,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> IsLocalHuman;

        // Each side player color /
        // Цвет игрока каждой из сторон.
        // offset: +0x54A8 = +21672,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> iPlayer;

        // Was the artifact-specific spell casted by each side / 
        // Было ли вызвано заклинание артефактов каждой из сторон.
        // offset: +0x54B0 = +21680,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> bArtifactCast;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_54B1[2];

    public:
        // offset: +0x54B4 = +21684,  size = 0x8 = 8
        std::array<bool32_t, NUM_SIDES> bSpellsCast;

        // offset: +0x54BC = +21692,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> numArmies;

        // Both sides armies /
        // Изначальные армии двух сторон.
        // offset: +0x54C4 = +21700,  size = 0x8 = 8
        std::array<armyGroup*, NUM_SIDES> ArmyGroups;

        union {
        // Combat creature stacks /
        // Стеки существ на поле боя.
        // offset: +0x54CC = +21708,  size = 0xDDD0 = 56784
        std::array<std::array<army, MAX_COMBAT_ARMIES + 1>, NUM_SIDES> Armies;
        };

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1329C = +78492,  size = 0x1 = 1
        byte_t gap_1329C[4];

    public:
        // offset: +0x132A0 = +78496,  size = 0x8 = 8
        std::array<int32_t, NUM_SIDES> turnSinceLastEnchanter;

        // Native terrain of each side / 
        // Родная земля каждой из сторон.
        // offset: +0x132A8 = +78504,  size = 0x8 = 8
        std::array<TTerrainType, NUM_SIDES> nativeTerrain;

        // Side summoned elemental at least once /
        // Сторона вызывала элементаля хотя бы один раз.
        // offset: +0x132B0 = +78512,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> SummonedElemental;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_132B1[2];

    public:
        // offset: +0x132B4 = +78516,  size = 0x4 = 4
        int32_t SideRetreated;

        // offset: +0x132B8 = +78520,  size = 0x4 = 4
        int32_t currArmyGroup;

        // offset: +0x132BC = +78524,  size = 0x4 = 4
        int32_t currArmyIndex;

        // offset: +0x132C0 = +78528,  size = 0x4 = 4
        int32_t currControl;

        // offset: +0x132C4 = +78532,  size = 0x4 = 4
        int32_t autoCombatOn;

        // offset: +0x132C8 = +78536,  size = 0x4 = 4
        army* currTroop;

        // offset: +0x132CC = +78540,  size = 0x1 = 1
        bool selectorOn;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_132CD[3];

    public:
        // offset: +0x132D0 = +78544,  size = 0x4 = 4
        int32_t selectorIndex;

        // offset: +0x132D4 = +78548,  size = 0x4 = 4
        int32_t highlighterOn;

        // offset: +0x132D8 = +78552,  size = 0x4 = 4
        int32_t highlighterIndex;

        // offset: +0x132DC = +78556,  size = 0x4 = 4
        int32_t lastMoveToIndex;

        // offset: +0x132E0 = +78560,  size = 0x4 = 4
        int32_t lastCommand;

        // offset: +0x132E4 = +78564,  size = 0x4 = 4
        int32_t combatCommand;

        // offset: +0x132E8 = +78568,  size = 0x4 = 4
        CSprite* CurLoadedSpellIcon;

        // offset: +0x132EC = +78572,  size = 0x4 = 4
        int32_t CurLoadedSpellEffect;

        // offset: +0x132F0 = +78576,  size = 0x4 = 4
        int32_t CurSpellEffectFrame;

        // offset: +0x132F4 = +78580,  size = 0x4 = 4
        TFortificationLevel fortificationLevel;

        // offset: +0x132F8 = +78584,  size = 0x4 = 4
        int32_t bBattleOver;

        // offset: +0x132FC = +78588,  size = 0x4 = 4
        heroWindow* mainWindow;
        //TCombatWindow* mainWindow; // TODO: Define TCombatWindow

        // offset: +0x13300 = +78592,  size = 0x4 = 4
        bool32_t bCombatShowIt;

        // offset: +0x13304 = +78596,  size = 0x64 = 100
        std::array<iconWidget*, 25> iconWidgetWL;

        // offset: +0x13368 = +78696,  size = 0x64 = 100
        std::array<textWidget*, 25> textWidgetWL;

        // offset: +0x133CC = +78796,  size = 0x30 = 48
        std::array<type_combat_cursor, 12> attack_cursor;

        // offset: +0x133FC = +78844,  size = 0x30 = 48
        std::array<int32_t, 12> attack_hex;

        // offset: +0x1342C = +78892,  size = 0x4 = 4
        type_combat_cursor last_attack_cursor;

        // offset: +0x13430 = +78896,  size = 0x4 = 4
        int32_t iTtlCombatDirections;

        // offset: +0x13434 = +78900,  size = 0x4 = 4
        int32_t iBackgroundFrame;

        // Dead creatures status /
        // Является ли каждая из существ мертвым?
        // offset: +0x13438 = +78904,  size = 0x28 = 40
        std::array<std::array<bool, MAX_COMBAT_ARMIES>, NUM_SIDES> bCreatureIsDead;

        // offset: +0x13460 = +78944,  size = 0x1 = 1
        bool bSomeCreaturesVanish;
    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_13461[3];

    public:
        // offset: +0x13464 = +78948,  size = 0x4 = 4
        const char* cBkgName;

        // offset: +0x13468 = +78952,  size = 0x8C4 = 2244
        std::array<adjacency_array, MAX_COMBAT_HEXES> AdjacentIndex;

        // offset: +0x13D2C = +81196,  size = 0x1 = 1
        bool SaveBiggestExtent;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_13D2D[3];

    public:
        // offset: +0x13D30 = +81200,  size = 0x4 = 4
        int32_t LimitToExtent;

        // offset: +0x13D34 = +81204,  size = 0x4 = 4
        int32_t ComputeExtentOnly;

        // offset: +0x13D38 = +81208,  size = 0x10 = 16
        SLimitData Extent;

        // offset: +0x13D48 = +81224,  size = 0x4 = 4
        int32_t winner;

        // offset: +0x13D4C = +81228,  size = 0x4 = 4
        int32_t SkeletonsCreated;

        // offset: +0x13D50 = +81232,  size = 0x4 = 4
        TCreatureType skeleton_type;

        // offset: +0x13D54 = +81236,  size = 0x4 = 4
        Bitmap816* NumberWindow;

        union {
        // offset: +0x13D58 = +81240,  size = 0x10 = 16
        exe_vector<TObstacle> Obstacles;
        };

        // offset: +0x13D68 = +81256,  size = 0x1 = 1
        bool InPlacementPhase;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_13D69[3];

    public:
        // offset: +0x13D6C = +81260,  size = 0x4 = 4
        int32_t turn_number;

        // offset: +0x13D70 = +81264,  size = 0x4 = 4
        int32_t BattleTacticsAdvantage;

        // offset: +0x13D74 = +81268,  size = 0x1 = 1
        bool DebugNoSpellLimit;

        // offset: +0x13D75 = +81269,  size = 0x1 = 1
        bool DebugShowHiddenObjects;

        // offset: +0x13D76 = +81270,  size = 0x1 = 1
        bool DebugShowBlockedHexes;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_13D77[1];

    public:
        // Besieged castle archers /
        // Информация о стрелковых башнях обороняющегося замка.
        // offset: +0x13D78 = +81272,  size = 0x6C = 108
        std::array<TArcher, kNumArchers> Archers;

        // offset: +0x13DE4 = +81380,  size = 0x1 = 1
        bool in_second_phase;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_13DE5[3];

    public:
        // offset: +0x13DE8 = +81384,  size = 0x4 = 4
        int32_t OriginalAttackSkill;

        // offset: +0x13DEC = +81388,  size = 0x4 = 4
        int32_t OriginalDefenseSkill;

        // offset: +0x13DF0 = +81392,  size = 0x4 = 4
        int32_t OriginalPowerSkill;

        // offset: +0x13DF4 = +81396,  size = 0x4 = 4
        int32_t original_mana;

        // offset: +0x13DF8 = +81400,  size = 0x168 = 360
        std::array<std::array<Bitmap816*, 5>, 18> wallImages;

        // offset: +0x13F60 = +81760,  size = 0x48 = 72
        std::array<int32_t, 18> wallLevel;

        // offset: +0x13FA8 = +81832,  size = 0x48 = 72
        std::array<int32_t, 18> wall_frame;

        // Map point of the location of the battle /
        // Точка карты, на которой происходит сражение.
        // offset: +0x13FF0 = +81904,  size = 0x4 = 4
        type_point map_point;

        // offset: +0x13FF4 = +81908,  size = 0x4 = 4
        Bitmap816* CombatCellGrid;

        // offset: +0x13FF8 = +81912,  size = 0x4 = 4
        Bitmap816* CombatCellShaded;

        // offset: +0x13FFC = +81916,  size = 0x4 = 4
        int32_t ObstacleAnimationFrame;

        // offset: +0x14000 = +81920,  size = 0x28 = 40
        std::array<std::array<bool, MAX_COMBAT_ARMIES>, NUM_SIDES> bCreatureEffect;

        // offset: +0x14028 = +81960,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> bHeroEffect;

        // offset: +0x1402A = +81962,  size = 0x2 = 2
        std::array<bool, NUM_SIDES> bFlagEffect;

        // offset: +0x1402C = +81964,  size = 0x3 = 3
        std::array<bool, kNumArchers> bArcherEffect;

        // offset: +0x1402F = +81967,  size = 0x1 = 1
        bool any_action_taken;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x14030 = +81968,  size = 0x1 = 1
        byte_t gap_14030[1];

    public:
        // offset: +0x14031 = +81969,  size = 0xBB = 187
        std::array<bool, MAX_COMBAT_HEXES> creaturePath;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x140EC, combatManager);

NH3API_INLINE_OR_EXTERN
combatManager* const& gpCombatManager
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699420, combatManager*));

NH3API_INLINE_OR_EXTERN
std::array<int32_t, 2>& iCombatControlNetPos
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x69778C, std::array<int32_t, 2>));

NH3API_INLINE_OR_EXTERN
bool& gbRetreatWin 
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6985F3, bool));

NH3API_INLINE_OR_EXTERN
bool& gbSurrenderWin 
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x697794, bool));

NH3API_INLINE_OR_EXTERN
bool& gbInCombat 
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699590, bool));

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63D3E8, combatManager)

NH3API_DISABLE_WARNING_END
