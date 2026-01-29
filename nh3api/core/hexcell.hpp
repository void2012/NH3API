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

#include <array>

#include "nh3api_std/memory.hpp" // std::array<T, N>, THISCALL_ macros

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#pragma pack(push, 4)
// size = 0x10 = 16, align = 4
struct SLimitData
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t MinX;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t MinY;

    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t MaxX;

    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t MaxY;

};

class army;
// Hex combat cell /
// Клетка поля боя(гекс).
// size = 0x70 = 112, align = 4
struct hexcell
{
    public:
        inline hexcell() noexcept
            : hexcell(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4E7210, this); }

        inline hexcell(const nh3api::dummy_tag_t&) noexcept
        {}

    public:
        [[nodiscard]] bool HasArmy() const noexcept
        { return armyGrp > 0; }

        [[nodiscard]] army* get_army()
        { return THISCALL_1(army*, 0x4E7230, this); }

        [[nodiscard]] const army* get_army() const
        { return THISCALL_1(army*, 0x4E7230, this); }

        [[nodiscard]] army* get_dead_army(uint32_t i)
        { return THISCALL_2(army*, 0x4E7270, this, i); }

        [[nodiscard]] const army* get_dead_army(uint32_t i) const
        { return THISCALL_2(army*, 0x4E7270, this, i); }

    public:
        // offset: +0x0 = +0,  size = 0x2 = 2
        int16_t refX;

        // offset: +0x2 = +2,  size = 0x2 = 2
        int16_t refY;

        // offset: +0x4 = +4,  size = 0x2 = 2
        int16_t hexULX;

        // offset: +0x6 = +6,  size = 0x2 = 2
        int16_t hexULY;

        // offset: +0x8 = +8,  size = 0x2 = 2
        int16_t hexBRX;

        // offset: +0xA = +10,  size = 0x2 = 2
        int16_t hexBRY;

        // offset: +0xC = +12,  size = 0x2 = 2
        int16_t fullHexBRY;

        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x10 = +16,  size = 0x4 = 4
        NH3API_FLAG_ENUM TAttributes : uint32_t
        {
            IS_OBSTACLE_ORIGIN = 0b000001,
            IS_BLOCKED         = 0b000010,
            IS_QUICKSAND       = 0b000100,
            IS_LANDMINE        = 0b001000,
            IS_FIREWALL        = 0b010000,
            IS_STONEWALL       = 0b100000,
            IS_OBSTACLIZED     = 0b111111,
            IS_MAGIC_OBSTACLE  = 0b111100
        } attributes;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t obstacleIndex;

        // offset: +0x18 = +24,  size = 0x1 = 1
        int8_t armyGrp;

        // offset: +0x19 = +25,  size = 0x1 = 1
        int8_t armyIndex;

        // offset: +0x1A = +26,  size = 0x1 = 1
        int8_t partOfDouble;

        unsigned char : 8;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t iBodiesInHex;

        // offset: +0x20 = +32,  size = 0xE = 14
        std::array<int8_t, 14> deadArmyGrp;

        // offset: +0x2E = +46,  size = 0xE = 14
        std::array<int8_t, 14> deadArmyIndex;

        // offset: +0x3C = +60,  size = 0xE = 14
        std::array<int8_t, 14> deadPartOfDouble;

        // offset: +0x4A = +74,  size = 0x1 = 1
        bool bValidMove;

        // offset: +0x4B = +75,  size = 0x1 = 1
        bool front_move;

        // offset: +0x4C = +76,  size = 0x1 = 1
        bool mouse_shaded;

        // offset: +0x4D = +77,  size = 0x1 = 1
        int8_t background_offset;

        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x50 = +80,  size = 0x10 = 16
        SLimitData obstacleLimitData;

        // offset: +0x60 = +96,  size = 0x10 = 16
        SLimitData cloudLimitData;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

NH3API_WARNING(pop)
