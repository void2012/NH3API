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

#include "nh3api_std/exe_vector.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// Heroes3.exe internal srand /
// Внутренний srand Heroes3.exe.
inline void exe_srand(uint32_t seed) noexcept
{ CDECL_1(void, 0x61841F, seed); }

// Heroes3.exe internal rand /
// Внутренний rand Heroes3.exe.
inline int32_t exe_rand() noexcept
{ return CDECL_0(int32_t, 0x61842C); }

// Generate random number using rand(). /
// Сгенерировать случайное число используя rand().
// returns random number in range [iMin; iMax] / Случайное число в пределах [iMin; iMax]
inline int32_t Random(int32_t iMin, int32_t iMax) noexcept
{ return FASTCALL_2(int32_t, 0x50C7C0, iMin, iMax); }

// Generate random number using rand(). /
// Сгенерировать случайное число используя rand().
// returns random number in range [iMin; iMax] / Случайное число в пределах [iMin; iMax]
inline uint32_t Random(uint32_t iMin, uint32_t iMax) noexcept
{
    if ( iMax == iMin )
        return iMax;

    const uint32_t randomNumber = static_cast<uint32_t>(exe_rand());
    if ( iMax >= iMin )
        return iMin + randomNumber % (iMax - iMin + 1);

    return iMax + randomNumber % (iMin - iMax + 1);
}

// Same as Random(int, int) except that this one uses timeGetTime() as seed value. /
// То же, что и Random(int, int), но используя timeGetTime() в качестве сида.
// returns random number in range [iMin; iMax] / Случайное число в пределах [iMin; iMax]
inline int32_t SafeRandom(int32_t iMin, int32_t iMax) noexcept
{ return FASTCALL_2(int32_t, 0x50B3C0, iMin, iMax); }

// Set thread-local random seed using srand()
inline void SRand(int32_t seed) noexcept
{ FASTCALL_1(void, 0x50C7B0, seed); }

#pragma pack(push, 4)
// Chooses a random number and makes said number unavailable to choose next time. /
// Выбирает случайное число и делает его недоступным при последующем выборе.
// size = 0x18 = 24, align = 4
struct TPickANumber
{
    public:
        inline TPickANumber(int32_t low, int32_t high) noexcept
            : TPickANumber(nh3api::dummy_tag)
        { THISCALL_3(void, 0x50C8D0, this, low, high); }

        inline TPickANumber(const nh3api::dummy_tag_t& tag) noexcept
            : Available(tag)
        {}

        inline ~TPickANumber() noexcept
        { std::destroy_at(&Available); }

    public:
        // Pick a random number, make it unavailable /
        // Выбрать случайное число и сделать его недоступным.
        int32_t Pick()
        { return THISCALL_1(int32_t, 0x50C930,this); }

    public:
        // Lower limit /
        // Нижний предел.
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t Low;

        // Available numbers left/
        // Кол-во оставшихся чисел.
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t Numbersleft;

        union {
        // Available numbers
        // Доступные числа.
        // offset: +0x8 = +8,  size = 0x10 = 16
        exe_vector<bool> Available;
        };
};
#pragma pack(pop)

NH3API_WARNING(pop)
