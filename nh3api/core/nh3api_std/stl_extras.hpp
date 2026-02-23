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

#include <algorithm>   // std::clamp
#include <cmath>
#include <type_traits> // std::make_unsigned_t
#include <utility>

#if (__cplusplus >= 202002L) || defined(__cpp_lib_concepts)
#include <concepts>    // std::integral concept
#endif

#if (__cplusplus >= 202002L) || defined(__cpp_lib_bit_cast)
#include <bit>         // std::bit_cast
#endif

#include "nh3api_std.hpp"

namespace nh3api
{

#if defined(__cpp_lib_bit_cast)
using ::std::bit_cast;
#else

template<class To, class From, ::std::enable_if_t<(sizeof(To) == sizeof(From)) && ::std::is_trivially_copyable_v<To> && ::std::is_trivially_copyable_v<From>, bool> = false >
inline
#if NH3API_HAS_BUILTIN_BIT_CAST
constexpr
#endif
To bit_cast(const From& from) noexcept
{
    #if NH3API_HAS_BUILTIN_BIT_CAST
    return __builtin_bit_cast(To, from);
    #else
    To result;
    ::memcpy(__builtin_addressof(result), __builtin_addressof(from), sizeof(To));
    return result;
    #endif
}

#endif // __cpp_lib_bit_cast

#if defined(__cpp_lib_concepts) && __cpp_lib_concepts >= 202002L

template<class To, class From>
constexpr To cast(const From& from) noexcept
requires(::std::is_trivially_copyable_v<To> &&
         ::std::is_trivially_copyable_v<From>)
{
    if constexpr ( ::std::is_integral_v<To> || ::std::is_enum_v<To> )
        return static_cast<To>(from);
    else if constexpr ( ::std::is_pointer_v<To> || ::std::is_pointer_v<From> )
        return reinterpret_cast<To>(from);
    else
        return bit_cast<To>(from);
}

#else

template<class To, class From,
         typename = ::std::enable_if_t<
             ::std::is_trivially_copyable_v<To> &&
             ::std::is_trivially_copyable_v<From>>>
inline
#if NH3API_HAS_BUILTIN_BIT_CAST
constexpr
#endif
To cast(const From& from) noexcept
{
    if constexpr ( ::std::is_integral_v<To> || ::std::is_enum_v<To> )
        return static_cast<To>(from);
     else if constexpr ( ::std::is_pointer_v<To> || ::std::is_pointer_v<From> )
        return reinterpret_cast<To>(from);
     else
        return bit_cast<To>(from);
}

#endif // __cpp_lib_concepts

template<class To, class From>
inline To cast(const From* from) noexcept
{
    return cast<To>(reinterpret_cast<uintptr_t>(from));
}

#ifdef __cpp_lib_integer_comparison_functions
using ::std::cmp_equal;
using ::std::cmp_not_equal;
using ::std::cmp_less;
using ::std::cmp_greater;
using ::std::cmp_less_equal;
using ::std::cmp_greater_equal;
#else
template<class T, class U>
inline constexpr bool cmp_equal(T t, U u) noexcept
{
    if constexpr (::std::is_signed_v<T> == ::std::is_signed_v<U>)
        return t == u;
    else if constexpr (::std::is_signed_v<T>)
        return t >= 0 && ::std::make_unsigned_t<T>(t) == u;
    else
        return u >= 0 && ::std::make_unsigned_t<U>(u) == t;
}

template<class T, class U>
inline constexpr bool cmp_not_equal(T t, U u) noexcept
{ return !cmp_equal(t, u); }

template<class T, class U>
inline constexpr bool cmp_less(T t, U u) noexcept
{
    if constexpr (::std::is_signed_v<T> == ::std::is_signed_v<U>)
        return t < u;
    else if constexpr (std::is_signed_v<T>)
        return t < 0 || ::std::make_unsigned_t<T>(t) < u;
    else
        return u >= 0 && t < ::std::make_unsigned_t<U>(u);
}

template<class T, class U>
inline constexpr bool cmp_greater(T t, U u) noexcept
{ return cmp_less(u, t); }

template<class T, class U>
constexpr bool cmp_less_equal(T t, U u) noexcept
{
    return !cmp_less(u, t);
}

template<class T, class U>
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
[[nodiscard]] inline constexpr T min_limit() noexcept
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
[[nodiscard]] inline constexpr T max_limit() noexcept
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
[[nodiscard]] inline constexpr bool in_range(const T value) noexcept
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

#ifdef __cpp_lib_three_way_comparison

#if NH3API_MSVC_STL
inline constexpr std::_Synth_three_way synth_three_way = {};

template <class T1, class T2 = T1>
using synth_three_way_result = ::std::_Synth_three_way_result<T1, T2>;
#elif NH3API_GCC_STL
inline constexpr auto synth_three_way = ::std::__detail::__synth3way;

template <class T1, class T2 = T1>
using synth_three_way_result = ::std::__detail::__synth3way_t<T1, T2>;
#elif NH3API_CLANG_STL

inline constexpr auto synth_three_way = ::std::__synth_three_way;
template <class T1, class T2 = T1>
using synth_three_way_result = ::std::__synth_three_way_result<T1, T2>;
#endif

#endif // C++20 three-way comparison

} // namespace nh3api
