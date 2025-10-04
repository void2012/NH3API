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

#include "campaign.hpp"

#pragma pack(push, 4)
// Saved game header /
// Заголовок сохранённой игры.
// size = 0x5A4 = 1444, align = 4
struct SavedGameHeader
{
    public:
        NH3API_FORCEINLINE
        SavedGameHeader() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4BBDA0, this); }

        NH3API_FORCEINLINE
        SavedGameHeader(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : map_header(tag), campaign(tag), file_name(tag)
        { THISCALL_1(void, 0x4BBDA0, this); }

        NH3API_FORCEINLINE
        ~SavedGameHeader() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4BDC40, this); }

    public:
        // offset: +0x0 = +0,  size = 0x8 = 8
        std::array<int8_t, 8> id;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t version;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t game_version;

        union {
        // offset: +0x10 = +16,  size = 0x304 = 772
        NewSMapHeader map_header;            
        };

        // offset: +0x314 = +788,  size = 0x1CC = 460
        SGameSetupOptions map_setup;

        // offset: +0x4E0 = +1248,  size = 0x1 = 1
        bool campaign_game;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x4E1 = +1249,  size = 0x3 = 3
        byte_t gap_4E1[3];

    public:
        union {
        // offset: +0x4E4 = +1252,  size = 0x7C = 124
        SCampaign campaign;
        };

        union {
        // offset: +0x560 = +1376,  size = 0x10 = 16
        exe_string file_name;
        };

        // offset: +0x570 = +1392,  size = 0x2 = 2
        int16_t difficultyRating;

        // offset: +0x574 = +1396,  size = 0x4 = 4
        int32_t numDeadPlayers;

        // offset: +0x578 = +1400,  size = 0x8 = 8
        std::array<bool, 8> dead_player;

        // offset: +0x580 = +1408,  size = 0x20 = 32
        std::array<bool32_t, 8> human_player;

        // offset: +0x5A0 = +1440,  size = 0x4 = 4
        int32_t current_player;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x5A4, SavedGameHeader);
