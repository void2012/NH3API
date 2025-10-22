//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// <type_traits> implementation for old MSVC compilers (C++98/C++03)
// MSVC C++6.0 does not support partial template specialization
// https://jeffpar.github.io/kbarchive/kb/240/Q240866/
// Hence, NH3API does not support MSVC C++ 6.0 :(
// implementation mostly by the boost software library https://boost.org/
// std::underlying_type implemented by void_17

#pragma once

#include <utility>
#include <type_traits>
#include "nh3api_std.hpp"

namespace nh3api
{
using ::std::declval;

namespace tt
{
using ::std::integral_constant;
using ::std::true_type;
using ::std::false_type;
using ::std::add_volatile;
using ::std::add_const;
using ::std::add_cv;
using ::std::add_lvalue_reference;
using ::std::add_pointer;
using ::std::is_signed;
using ::std::is_unsigned;
using ::std::make_signed;
using ::std::make_unsigned;
using ::std::underlying_type;
using ::std::enable_if;
using ::std::is_base_of;
using ::std::is_same;
using ::std::alignment_of;
#ifdef __cpp_lib_type_identity
using ::std::type_identity;
#else
template<class T> struct type_identity
{ using type = T; };
#endif
using ::std::conditional;
using ::std::is_convertible;
using ::std::is_void;
using ::std::is_lvalue_reference;
using ::std::is_rvalue_reference;
using ::std::is_reference;
using ::std::is_pointer;
using ::std::negation;
template<typename T1, typename T2>
struct conjunction_2
    : ::std::conjunction<T1, T2>
{};

template<typename T1, typename T2>
struct disjunction_2
    : ::std::disjunction<T1, T2>
{};
using ::std::is_move_constructible;
using ::std::is_trivially_move_constructible;
using ::std::is_trivially_move_assignable;
using ::std::is_nothrow_move_constructible;
using ::std::is_copy_constructible;
using ::std::is_nothrow_constructible;
using ::std::is_const;
using ::std::is_volatile;
using ::std::is_enum;
using ::std::is_class;
using ::std::is_union;
// using ::std::is_pod; // we're not using is_pod currently
using ::std::is_empty;
using ::std::is_polymorphic;
using ::std::is_abstract;
using ::std::has_virtual_destructor;
using ::std::is_unsigned;
using ::std::is_signed;
using ::std::is_integral;
using ::std::is_floating_point;
using ::std::is_arithmetic;
using ::std::is_function;
using ::std::is_fundamental;
using ::std::is_scalar;
using ::std::is_compound;
using ::std::is_object;
using ::std::is_trivially_copyable;
using ::std::is_trivially_copy_assignable;
using ::std::is_trivially_default_constructible;
using ::std::is_trivially_destructible;
using ::std::is_nothrow_assignable;
using ::std::is_nothrow_default_constructible;
using ::std::is_nothrow_copy_constructible;
using ::std::is_trivial;
using ::std::is_trivially_copy_constructible;
using ::std::is_nothrow_destructible;
using ::std::is_array;
using ::std::is_move_assignable;
using ::std::is_nothrow_move_assignable;
using ::std::is_nothrow_move_constructible;
using ::std::is_copy_assignable;
using ::std::is_nothrow_copy_assignable;
using ::std::remove_const;
using ::std::remove_volatile;
using ::std::remove_cv;
using ::std::remove_reference;
#ifdef __cpp_lib_remove_cvref
using ::std::remove_cvref;
#else
template <class T>
struct remove_cvref
: ::std::remove_cv<::std::remove_reference_t<T>>
{};
#endif
using ::std::remove_pointer;
using ::std::decay;
using ::std::void_t;
template<typename T>
struct void_1 { using type = void; };

#if !defined(__cpp_lib_bounded_array_traits)
template<class T>
struct is_unbounded_array
    : ::std::false_type { };

template<class T>
struct is_unbounded_array<T[]>
    : ::std::true_type { };

template<class T>
struct is_unbounded_array<const T[]>
    : ::std::true_type { };

template<class T>
struct is_unbounded_array<volatile T[]>
    : ::std::true_type { };

template<class T>
struct is_unbounded_array<const volatile T[]>
    : ::std::true_type { };

template<class T>
struct is_bounded_array
    : ::std::false_type { };

template<class T, size_t N>
struct is_bounded_array<T[N]>
    : ::std::true_type { };

template<class T, size_t N>
struct is_bounded_array<const T[N]>
    : ::std::true_type { };

template<class T, size_t N>
struct is_bounded_array<volatile T[N]>
    : ::std::true_type { };

template<class T, size_t N>
struct is_bounded_array<const volatile T[N]>
    : ::std::true_type { };
#else
using ::std::is_unbounded_array;
using ::std::is_bounded_array;
#endif

template <class T, class = void>
struct has_scalar_deleting_destructor
    : ::std::false_type {};

template <class T>
struct has_scalar_deleting_destructor<T,
decltype((void)declval<T>().scalar_deleting_destructor(declval<uint8_t>()))>
    : ::std::true_type {};

template <class T>
inline constexpr bool has_scalar_deleting_destructor_v = has_scalar_deleting_destructor<T>::value;

template<class T, class... Args>
constexpr bool is_any_of_v = (::std::is_same_v<T, Args> || ...);

#ifdef __cpp_lib_concepts 
template <typename F, typename ArgT, typename ResultT>
concept unary_functor = std::invocable<F, ArgT> &&
                        std::same_as<std::invoke_result_t<F, ArgT>, ResultT>;

#endif

} // namespace tt

using ::std::as_const;

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

} // namespace nh3api
