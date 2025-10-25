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

#include "core/nh3api_std/memory.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#pragma pack(push, 4)
// Turn duration timer /
// Таймер хода.
// size = 0x14 = 20, align = 4
struct CTurnDuration
{
    public:
        NH3API_FORCEINLINE
        CTurnDuration() noexcept
            : m_lastWarned(0), m_turnStartTime(0), m_currDuration(0), m_nextWarning(0), m_pauseTime(0)
        {}

        NH3API_FORCEINLINE
        CTurnDuration(const ::nh3api::dummy_tag_t&) noexcept
        {}

        [[nodiscard]] bool IsOn() const
        { return THISCALL_1(bool, 0x557E20, this); }

        [[nodiscard]] bool IsExpired() const
        { return THISCALL_1(bool, 0x557E40, this); }

        void CheckForWarning()
        { THISCALL_1(void, 0x557E90, this); }

        [[nodiscard]] bool IsClose(uint32_t howClose) const
        { return THISCALL_2(bool, 0x5580A0, this, howClose); }

        void Clear()
        { THISCALL_1(void, 0x558100, this); }

        void SetDuration(uint32_t ms)
        { THISCALL_2(void, 0x558110, this, ms); }

        void Start()
        { THISCALL_1(void, 0x558130, this); }

        void Pause()
        { THISCALL_1(void, 0x558170, this); }

        void Resume()
        { THISCALL_1(void, 0x558190, this); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        uint32_t m_lastWarned;

        // offset: +0x4 = +4,  size = 0x4 = 4
        uint32_t m_turnStartTime;

        // offset: +0x8 = +8,  size = 0x4 = 4
        uint32_t m_currDuration;

        // offset: +0xC = +12,  size = 0x4 = 4
        uint32_t m_nextWarning;

        // offset: +0x10 = +16,  size = 0x4 = 4
        uint32_t m_pauseTime;

};
#pragma pack(pop)

inline CTurnDuration& g_turnDuration = get_global_var_ref(0x69D680, CTurnDuration);

NH3API_DISABLE_WARNING_END
