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

#include "nh3api_std/nh3api_std.hpp"

#include <type_traits>
#if (__cplusplus >= 202002L) || defined(__cpp_lib_concepts)
#include <concepts>    // std::integral
#endif

// Player colors /
// Цвета игроков.
enum EPlayerColor : int32_t
{
    PLAYER_NONE   = -1, // Нейтральный
    PLAYER_RED    = 0, // Красный
    PLAYER_BLUE   = 1, // Синий
    PLAYER_TAN    = 2, // Серый
    PLAYER_GREEN  = 3, // Зелёный
    PLAYER_ORANGE = 4, // Оранжевый
    PLAYER_PURPLE = 5, // Фиолетовый
    PLAYER_TEAL   = 6, // Бирюзовый
    PLAYER_PINK   = 7, // Розовый
    MAX_PLAYERS   = 8  // Максимальное кол-во игроков
};

// Player bits /
// Биты игроков.
NH3API_FLAG_ENUM EPlayerBit : uint8_t
{
    PLAYER_BIT_RED    = 0b00000000, // Красный
    PLAYER_BIT_BLUE   = 0b00000010, // Синий
    PLYAER_BIT_TAN    = 0b00000100, // Серый
    PLAYER_BIT_GREEN  = 0b00001000, // Зелёный
    PLAYER_BIT_ORANGE = 0b00010000, // Оранжевый
    PLAYER_BIT_PURPLE = 0b00100000, // Фиолетовый
    PLAYER_BIT_TEAL   = 0b01000000, // Бирюзовый
    PLAYER_BIT_PINK   = 0b10000000  // Розовый
};

inline constexpr EPlayerBit operator|(EPlayerBit lhs, EPlayerBit rhs) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

inline constexpr EPlayerBit& operator|=(EPlayerBit& lhs, EPlayerBit rhs) noexcept
{ return lhs = lhs | rhs; }

inline constexpr EPlayerBit operator&(EPlayerBit lhs, EPlayerBit rhs) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs))); }

inline constexpr EPlayerBit& operator&=(EPlayerBit& lhs, EPlayerBit rhs) noexcept
{ return lhs = lhs & rhs; }

inline constexpr EPlayerBit operator^(EPlayerBit lhs, EPlayerBit rhs) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs))); }

inline constexpr EPlayerBit& operator^=(EPlayerBit& lhs, EPlayerBit rhs) noexcept
{ return lhs = lhs ^ rhs; }

inline constexpr EPlayerBit operator~(EPlayerBit arg) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(~static_cast<uint32_t>(arg))); }

#ifdef __cpp_lib_concepts
inline constexpr EPlayerBit& operator<<=(EPlayerBit& lhs, std::integral auto shift) noexcept
{ return lhs = lhs << shift; }

template <class _Integer>
inline constexpr EPlayerBit operator<<(EPlayerBit lhs, std::integral auto shift) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) << shift)); }

template <class _Integer>
inline constexpr EPlayerBit& operator>>=(EPlayerBit& lhs, std::integral auto shift) noexcept
{ return lhs = lhs >> shift; }

template <class _Integer>
inline constexpr EPlayerBit operator>>(EPlayerBit lhs, std::integral auto shift) noexcept
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) >> shift)); }
#else
template <class _Integer>
inline constexpr EPlayerBit& operator<<=(EPlayerBit& lhs, _Integer shift) noexcept
{
    static_assert(std::is_integral_v<_Integer>, "operand must be an integral type");
    return lhs = lhs << shift;
}

template <class _Integer>
inline constexpr EPlayerBit operator<<(EPlayerBit lhs, _Integer shift) noexcept
{
    static_assert(std::is_integral_v<_Integer>, "operand must be an integral type");
    return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) << shift));
}

template <class _Integer>
inline constexpr EPlayerBit& operator>>=(EPlayerBit& lhs, _Integer shift) noexcept
{
    static_assert(std::is_integral_v<_Integer>, "operand must be an integral type");
    return lhs = lhs >> shift;
}

template <class _Integer>
inline constexpr EPlayerBit operator>>(EPlayerBit lhs, _Integer shift) noexcept
{
    static_assert(std::is_integral_v<_Integer>, "operand must be an integral type");
    return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) >> shift));
}
#endif
