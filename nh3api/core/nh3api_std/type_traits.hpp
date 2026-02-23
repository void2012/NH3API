//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
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

#include <type_traits>
#include <utility>
#ifdef __cpp_lib_concepts
#include <concepts>
#endif

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
using ::std::type_identity_t;
#else
template<class T> struct type_identity
{ using type = T; };

template<class T>
using type_identity_t = typename type_identity<T>::type;
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

} // namespace tt

using ::std::as_const;

} // namespace nh3api
