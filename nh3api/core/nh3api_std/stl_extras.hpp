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

#include <memory>
#include <algorithm> // std::clamp
#include <cmath>
#ifdef __cpp_lib_bit_cast
#include <bit>
#endif
#include <type_traits>
#include "nh3api_std.hpp"

namespace nh3api
{

// the reason we're writing our own addressof is that pre-C++17 version wasn't constexpr

template<typename T>
NH3API_MSVC_INTRIN constexpr NH3API_FORCEINLINE
T* addressof(T& arg) noexcept
{ return __builtin_addressof(arg); }

#ifdef __cpp_lib_bit_cast

using ::std::bit_cast;

#else

template<class To, class From, ::std::enable_if_t<(sizeof(To) == sizeof(From)) && std::is_trivially_copyable_v<To> && std::is_trivially_copyable_v<From>, bool> = false >
NH3API_FORCEINLINE
#if NH3API_HAS_BUILTIN_BIT_CAST
constexpr
#endif
To bit_cast(const From& from) noexcept
{
    #if NH3API_HAS_BUILTIN_BIT_CAST
    return __builtin_bit_cast(To, from);
    #else
    To result;
    ::memcpy(addressof(result), addressof(from), sizeof(To));
    return result;
    #endif
}

#endif // __cpp_lib_bit_cast

#ifndef __cpp_lib_concepts

template<class To, class From>
NH3API_FORCEINLINE
#if NH3API_HAS_BUILTIN_BIT_CAST
constexpr
#endif
To cast(const From& from) noexcept
{
    if constexpr (::std::is_pointer_v<To>)
        return reinterpret_cast<To>(from);
    else if constexpr (::std::is_arithmetic_v<To> || ::std::is_enum_v<To>)
        return static_cast<To>(from);
    else
        return bit_cast<To>(from);
}

#else

template<class To, class From>
constexpr To cast(const From& from) noexcept
requires(::std::is_arithmetic_v<To> || ::std::is_enum_v<To>)
{ return static_cast<To>(from); }

template<class To, class From>
To cast(const From& from) noexcept
requires(::std::is_pointer_v<To>)
{ return reinterpret_cast<To>(from); }

template<class To, class From>
constexpr To cast(const From& from) noexcept
requires(::std::is_trivially_copyable_v<To> && !::std::is_scalar_v<To>)
{ return __builtin_bit_cast(To, from); }

#endif // __cpp_lib_concepts to sfinae nh3api::cast

using ::std::destroy_at;
using ::std::destroy;
using ::std::destroy_n;

using ::std::exchange;


// there is no __cpp_lib_* feature testing macro for std::construct_at
// use __cpp_lib_constexpr_dynamic_alloc because it's closely of related
#ifdef __cpp_lib_constexpr_dynamic_alloc

using ::std::construct_at;

#else // C++20

template <class T, class... Args, class = decltype(::new(std::declval<void*>()) T(std::declval<Args>()...))> NH3API_FORCEINLINE
NH3API_CONSTEXPR_CPP_20
T* construct_at(T* ptr, Args&&... args)
noexcept(noexcept(std::is_nothrow_constructible_v<T, Args...>))
{ return ::new (static_cast<void*>(ptr)) T(::std::forward<Args>(args)...); }

#endif // C++20

using ::std::clamp;

using byte_t = ::std::byte;
using ::std::to_integer;

using ::std::next;
using ::std::prev;

// floating-point math

using ::std::fpclassify;
using ::std::isfinite;
using ::std::isinf;
using ::std::isnan;
using ::std::isnormal;
using ::std::trunc;
using ::std::signbit;

template <size_t N, typename T>
NH3API_FORCEINLINE
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
constexpr
#endif
T* assume_aligned(T* ptr) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED || NH3API_CHECK_CLANG_CL
#if NH3API_HAS_IS_CONSTANT_EVALUATED || defined(__cpp_if_consteval)
    NH3API_IF_CONSTEVAL
    {
        return ptr;
    }
    else
    {
        return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
    }
#else
    return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
#endif
#else // Assume pre-C++20 MSVC
    if ((reinterpret_cast<uintptr_t>(ptr) & ((1 << N) - 1)) == 0)
        return ptr;
    else
        __assume(0);
#endif
}

#ifdef __cpp_lib_to_underlying
using ::std::to_underlying;
#else
template<class Enum> [[nodiscard]] NH3API_MSVC_INTRIN constexpr
std::underlying_type_t<Enum> to_underlying(Enum arg) noexcept
{ return static_cast<std::underlying_type_t<Enum>>(arg); }
#endif

#ifdef __cpp_lib_integer_comparison_functions
using ::std::cmp_equal;
using ::std::cmp_not_equal;
using ::std::cmp_less;
using ::std::cmp_greater;
using ::std::cmp_less_equal;
using ::std::cmp_greater_equal;
#else
template<class T, class U> NH3API_PURE
inline constexpr bool cmp_equal(T t, U u) noexcept
{
    if constexpr (::std::is_signed_v<T> == ::std::is_signed_v<U>)
        return t == u;
    else if constexpr (::std::is_signed_v<T>)
        return t >= 0 && ::std::make_unsigned_t<T>(t) == u;
    else
        return u >= 0 && ::std::make_unsigned_t<U>(u) == t;
}

template<class T, class U> NH3API_PURE
inline constexpr bool cmp_not_equal(T t, U u) noexcept
{ return !cmp_equal(t, u); }

template<class T, class U> NH3API_PURE
inline constexpr bool cmp_less(T t, U u) noexcept
{
    if constexpr (::std::is_signed_v<T> == ::std::is_signed_v<U>)
        return t < u;
    else if constexpr (std::is_signed_v<T>)
        return t < 0 || ::std::make_unsigned_t<T>(t) < u;
    else
        return u >= 0 && t < ::std::make_unsigned_t<U>(u);
}

template<class T, class U> NH3API_PURE
inline constexpr bool cmp_greater(T t, U u) noexcept
{ return cmp_less(u, t); }

template<class T, class U> NH3API_PURE
constexpr bool cmp_less_equal(T t, U u) noexcept
{
    return !cmp_less(u, t);
}

template<class T, class U> NH3API_PURE
constexpr bool cmp_greater_equal(T t, U u) noexcept
{
    return !cmp_less(t, u);
}
#endif

#ifdef __cpp_lib_concepts
template<::std::integral T>
#else
template<typename T>
#endif
[[nodiscard]] NH3API_PURE inline constexpr T min_limit() noexcept
{
    if constexpr (::std::is_signed_v<T>)
    {
        constexpr auto unsigned_max = static_cast<::std::make_unsigned_t<T>>(-1);
        return static_cast<T>((unsigned_max >> 1) + 1);
    }
    else
    {
        return 0;
    }
}

#ifdef __cpp_lib_concepts
template<::std::integral T>
#else
template<typename T>
#endif
[[nodiscard]] NH3API_PURE inline constexpr T max_limit() noexcept
{
    if constexpr (::std::is_signed_v<T>)
    {
        constexpr auto unsigned_max = static_cast<::std::make_unsigned_t<T>>(-1);
        return static_cast<T>(unsigned_max >> 1);
    }
    else
    {
        return static_cast<T>(-1);
    }
}

#ifdef __cpp_lib_integer_comparison_functions
using ::std::in_range;
#else
#ifdef __cpp_lib_concepts
template<typename Rx, ::std::integral T>
#else
template<typename Rx, typename T>
#endif
[[nodiscard]] NH3API_PURE inline constexpr bool in_range(const T value) noexcept
{
    constexpr T  t_min  = min_limit<T>();
    constexpr Rx rx_min = min_limit<Rx>();

    if constexpr ( cmp_less(t_min, rx_min) )
        if ( value < T{rx_min} )
            return false;

    constexpr T  t_max = max_limit<T>();
    constexpr Rx rx_max = max_limit<Rx>();

    if constexpr ( cmp_greater(t_max, rx_max) )
        if ( value > T{rx_max})
            return false;

    return true;
}
#endif

} // namespace nh3api

using byte_t = std::byte;
