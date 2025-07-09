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

#include "nh3api_std/exe_vector.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

NH3API_FORCEINLINE
// Heroes3.exe internal srand /
// Внутренний srand Heroes3.exe.
void exe_srand(uint32_t seed) NH3API_NOEXCEPT
{ CDECL_1(void, 0x61841F, seed); }

NH3API_FORCEINLINE
// Heroes3.exe internal rand /
// Внутренний rand Heroes3.exe.
int32_t exe_rand() NH3API_NOEXCEPT
{ return CDECL_0(int32_t, 0x61842C); }

NH3API_FORCEINLINE
// Generate random number using rand(). /
// Сгенерировать случайное число используя rand().
/// @param iMin
/// @param iMax
/// @return random number in range [iMin; iMax] / Случайное число в пределах [iMin; iMax]
int32_t Random(int32_t iMin, int32_t iMax) NH3API_NOEXCEPT
{ return FASTCALL_2(int32_t, 0x50C7C0, iMin, iMax); }

NH3API_FORCEINLINE
// Same as Random(int, int) except that this one uses timeGetTime() as seed value. /
// То же, что и Random(int, int), но используя timeGetTime() в качестве сида.
/// @param iMin
/// @param iMax
/// @return random number in range [iMin; iMax] / Случайное число в пределах [iMin; iMax]
int32_t SafeRandom(int32_t iMin, int32_t iMax) NH3API_NOEXCEPT
{ return FASTCALL_2(int32_t, 0x50B3C0, iMin, iMax); }

NH3API_FORCEINLINE
/// @brief Set global random seed using srand()
/// @param seed
void SRand(int32_t seed) NH3API_NOEXCEPT
{ FASTCALL_1(void, 0x50C7B0, seed); }

#pragma pack(push, 4)
// Chooses a random number and makes said number unavailable to choose next time. /
// Выбирает случайное число и делает его недоступным при последующем выборе.
// size = 0x18 = 24, align = 4
struct TPickANumber
{
    public:
        NH3API_FORCEINLINE
        TPickANumber(int32_t low, int32_t high) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(TPickANumber)
        { THISCALL_3(void, 0x50C8D0, this, low, high); }

        NH3API_FORCEINLINE
        TPickANumber(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Available(tag)
        { NH3API_IGNORE(Low, Numbersleft); }

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

        // Available numbers
        // Доступные числа.
        // offset: +0x8 = +8,  size = 0x10 = 16
        exe_vector<bool> Available;
};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END
