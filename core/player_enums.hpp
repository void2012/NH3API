//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once 

#include "nh3api_std/type_traits.hpp"

// Player colors /
// Цвета игроков.
enum EPlayerColor
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
enum EPlayerBit : uint8_t
{
    PLAYER_BIT_RED    = 1 << 0,    // Красный
    PLAYER_BIT_BLUE   = 1 << 1,   // Синий
    PLYAER_BIT_TAN    = 1 << 2,    // Серый
    PLAYER_BIT_GREEN  = 1 << 3,  // Зелёный
    PLAYER_BIT_ORANGE = 1 << 4, // Оранжевый
    PLAYER_BIT_PURPLE = 1 << 5, // Фиолетовый
    PLAYER_BIT_TEAL   = 1 << 6,   // Бирюзовый
    PLAYER_BIT_PINK   = 1 << 7,   // Розовый
};

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator|(EPlayerBit lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit& operator|=(EPlayerBit& lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return lhs = lhs | rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator&(EPlayerBit lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit& operator&=(EPlayerBit& lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return lhs = lhs & rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator^(EPlayerBit lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit& operator^=(EPlayerBit& lhs, EPlayerBit rhs) NH3API_NOEXCEPT
{ return lhs = lhs ^ rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator~(EPlayerBit arg) NH3API_NOEXCEPT
{ return static_cast<EPlayerBit>(static_cast<uint8_t>(~static_cast<uint32_t>(arg))); }

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit& operator<<=(EPlayerBit& lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return lhs = lhs << shift;
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator<<(EPlayerBit lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) << shift));
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit& operator>>=(EPlayerBit& lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return lhs = lhs >> shift;
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE EPlayerBit operator>>(EPlayerBit lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return static_cast<EPlayerBit>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) >> shift));
}