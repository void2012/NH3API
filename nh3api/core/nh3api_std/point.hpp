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

#include <cstdint>

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct TPoint
{
    public:
        inline constexpr TPoint& operator+=(const TPoint& other) noexcept
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        inline constexpr TPoint& operator-=(const TPoint& other) noexcept
        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        inline constexpr friend TPoint operator+(TPoint left, const TPoint& right) noexcept
        {
            left += right;
            return left;
        }

        inline constexpr friend TPoint operator-(TPoint left, const TPoint& right) noexcept
        {
            left -= right;
            return left;
        }

    #ifndef __cpp_impl_three_way_comparison
        [[nodiscard]] inline constexpr bool operator==(const TPoint& right) noexcept
        { return this->x == right.x && this->y == right.y; }

        [[nodiscard]] inline constexpr bool operator!=(const TPoint& right) noexcept
        { return !(*this == right); }
    #endif

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t x = -1;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t y = -1;
};
#pragma pack(pop) // 4
