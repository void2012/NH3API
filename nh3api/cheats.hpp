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

#include "core/nh3api_std/char_traits.hpp"
#include "core/nh3api_std/exe_string.hpp"

struct TCheatCode
{
    std::array<char, 200> code{};
    explicit constexpr TCheatCode(const char* const _code) noexcept
    {
        static constexpr std::array<char, 27> b { "nopqrstuvwxyzabcdefghijklm" };

        size_t i = 0;
        const size_t inputLength = std::min<size_t>(std::size(code) - 1, nh3api::safe_strlen(_code, code.size()));

        for (; i < inputLength; ++i)
        {
            if (nh3api::isalpha_constexpr(_code[i]))
                code[i] = b[static_cast<size_t>(nh3api::tolower_constexpr(_code[i]) - 'a')];
            else
                code[i] = _code[i];
        }

        code[i] = '0';
    }

    inline constexpr int32_t compare(const char* const arg) const
    { return __builtin_memcmp(code.data(), arg, code.size()); }

};

[[nodiscard]] inline constexpr bool operator==(const TCheatCode& left, const char* const right) noexcept
{ return left.compare(right) == 0; }

[[nodiscard]] inline constexpr bool operator==(const char* const left, const TCheatCode& right) noexcept
{ return right.compare(left) == 0; }

#ifndef __cpp_impl_three_way_comparison
[[nodiscard]] inline constexpr bool operator!=(const TCheatCode& left, const char* const right) noexcept
{ return left.compare(right) != 0; }

[[nodiscard]] inline constexpr bool operator!=(const char* const left, const TCheatCode& right) noexcept
{ return right.compare(left) != 0; }
#endif


// encode cheat code at compile-time:
// static constexpr TCheatCode cheat_openmap("openmap");
// patcherInstance->WriteAddressOf(0x63A4F4, cheat_openmap.code);
