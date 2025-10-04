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

#include <windef.h> // RECT
#include "nh3api_std/exe_vector.hpp" // exe_vector<T>
#include "skills.hpp" // TSkillMastery
#include "terrain.hpp" // type_point

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#pragma pack(push, 1)
// Path finding engine cell /
// Клетка движка поиска оптимального пути.
// size = 0x1E = 30, align = 1, baseclass: type_point
struct pathCell : public type_point
{
    // occupy: +0x4{00000001}
    unsigned visited : 1;

    // occupy: +0x4{00000010}
    unsigned bIsTrigger : 1;

    // occupy: +0x4{00000100}
    unsigned in_boat : 1;

    // occupy: +0x4{00001000}
    unsigned magic_forbidden : 1;

    // occupy: +0x4{00010000}
    unsigned flying : 1;

    // occupy: +0x4{00100000}
    unsigned water_walking : 1;

    // occupy: +0x4{01000000}
    unsigned town_portal : 1;

    // occupy: +0x4{10000000}
    unsigned dimension_door : 1;

    // occupy: +0x5{00000001}
    unsigned castle_gate : 1;

    // occupy: +0x5{00000010}
    unsigned can_stop : 1;

    // occupy: +0x5{00000100}
    unsigned last_can_stop : 1;

    // occupy: +0x5{01111000}
    unsigned direction : 4;

    // occupy: +0x5{10000000}..+0x6{00001111}
    unsigned delta_x : 5;

    // occupy: +0x6{11110000}..+0x7{00000001}
    unsigned delta_y : 5;

    // occupy: +0x7{11111110}
    unsigned flight_cost : 6;

    // offset: +0x8 = +8,  size = 0x4 = 4
    type_point last_point;

    // offset: +0xC = +12,  size = 0x4 = 4
    type_point monster;

    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t barrier_value;

    // offset: +0x14 = +20,  size = 0x4 = 4
    int32_t danger_value;

    // offset: +0x18 = +24,  size = 0x2 = 2
    uint16_t cost;

    // offset: +0x1A = +26,  size = 0x2 = 2
    uint16_t adjusted_cost;

    // offset: +0x1C = +28,  size = 0x2 = 2
    uint16_t move_left;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

enum type_search_type : uint32_t
{
    const_normal_search       = 0, //
    const_AI_treasure_search  = 1, //
    const_AI_allied_search    = 2, //
    const_AI_enemy_search     = 3, //
    const_AI_search           = 4, //
    const_AI_alternate_search = 5  //
};

class army;
class hero;
#pragma pack(push, 4)
// Path finding engine in combat and adventure map /
// Движок поиска оптимального пути в бою и на карте приключений.
// size = 0x70 = 112, align = 4
class searchArray
{
    public:
        NH3API_FORCEINLINE
        searchArray() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4B10D0, this); }

        NH3API_FORCEINLINE
        searchArray(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : queue(tag), result(tag), visited_points(tag)
        {}

        NH3API_FORCEINLINE
        ~searchArray() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4B1140, this); }

    public:
        NH3API_NODISCARD const pathCell* get_cell(type_point point, bool flying) const
        { return THISCALL_3(pathCell*, 0x42ECC0, this, point, flying); }

        NH3API_NODISCARD int32_t get_danger_value(type_point point) const
        { return THISCALL_2(int32_t, 0x42ED30, this, point); }

        void SeedCombatPosition(const army* thisArmy,
                                int32_t current_group,
                                int32_t limit,
                                bool in_placement_phase,
                                int32_t base_speed)
        { THISCALL_6(void, 0x4B2B00, this, thisArmy, current_group, limit, in_placement_phase, base_speed); }

        bool FindCombatPath(const army* current_army,
                            int32_t current_group,
                            int32_t destination,
                            int32_t in_placement_phase,
                            int32_t limit,
                            int32_t base_speed)
        { return THISCALL_7(bool, 0x4B3160, this, current_army, current_group, destination, in_placement_phase, limit, base_speed); }

        NH3API_NODISCARD pathCell* getCellData(size_t pos)
        { return THISCALL_2(pathCell*, 0x4B38F0, this, pos); }

        NH3API_NODISCARD const pathCell* getCellData(size_t pos) const
        { return THISCALL_2(pathCell*, 0x4B38F0, this, pos); }

        int32_t get_travel_time(const army* current_army, int32_t hex) const
        { return THISCALL_3(int32_t, 0x4B3C80, this, current_army, hex); }

        int32_t BuildPath(const hero* current_hero, int32_t limit)
        { return THISCALL_3(int32_t, 0x56A3A0, this, current_hero, limit); }

        void SeedPosition(hero* current_hero,
                          type_point start,
                          type_point target,
                          int32_t maxMobility,
                          bool is_boat,
                          type_search_type search_type,
                          int32_t iCurTempMobility,
                          type_point next_cell)
        { THISCALL_9(void, 0x56B710, this, current_hero, start, target, maxMobility, is_boat, search_type, iCurTempMobility, next_cell); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        uint32_t maxQueueCount;

        // offset: +0x4 = +4,  size = 0x1 = 1
        uint8_t pay_transition_costs;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_5[3];

    public:
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t this_turns_movement;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t land_movement;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t sea_movement;

        // offset: +0x14 = +20,  size = 0x1 = 1
        bool can_summon_boat;

        // offset: +0x15 = +21,  size = 0x1 = 1
        bool can_cast_teleport;

        // offset: +0x16 = +22,  size = 0x1 = 1
        bool can_cast_flight;

        // offset: +0x17 = +23,  size = 0x1 = 1
        bool can_cast_water_walk;

        // offset: +0x18 = +24,  size = 0x4 = 4
        TSkillMastery water_walk_level;

        // offset: +0x1C = +28,  size = 0x4 = 4
        TSkillMastery flight_level;

        // offset: +0x20 = +32,  size = 0x1 = 1
        bool limit_reached;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_21[3];

    public:
        // offset: +0x24 = +36,  size = 0x4 = 4
        pathCell* cellData;

        // offset: +0x28 = +40,  size = 0x10 = 16
        RECT valid_rectangle;
        
        union {
        // offset: +0x38 = +56,  size = 0x10 = 16
        exe_vector<pathCell> queue;
        };

        union {
        // offset: +0x48 = +72,  size = 0x10 = 16
        exe_vector<pathCell*> result;
        };

        union {
        // offset: +0x58 = +88,  size = 0x10 = 16
        exe_vector<pathCell*> visited_points;
        };

        // offset: +0x68 = +104,  size = 0x4 = 4
        bool* bIsMoatSlowed;

        // offset: +0x6C = +108,  size = 0x4 = 4
        int32_t* danger_zones;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x70, searchArray);

NH3API_INLINE_OR_EXTERN
searchArray* const& gpSearchArray
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6992D4, searchArray*));

NH3API_DISABLE_WARNING_END
