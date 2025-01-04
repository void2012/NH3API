//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.
#pragma once

#include "function.hpp"

#if (NH3API_TYPE_TRAITS_IMPL)

#pragma component(mintypeinfo, on)

namespace nh3api
{
namespace tt
{
template <class T>
struct is_const : public false_type {};
template <class T> struct is_const<T const> : public true_type{};

template <class T>
struct is_volatile : public false_type {};
template <class T> struct is_volatile<T volatile> : public true_type{};

template<typename T>
struct is_enum : public integral_constant<bool, __is_enum(T)> {};

template<typename T>
struct is_class : public integral_constant<bool, __is_class(T) && !__is_union(T)> {};

template<typename T>
struct is_union : public integral_constant<bool, !__is_class(T) && __is_union(T)> {};

template<typename T>
struct is_pod : public integral_constant<bool, __is_pod(T)> {};

template<typename T>
struct is_empty : public integral_constant<bool, __is_empty(T)> {};

template<typename T>
struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)> {};

template<typename T>
struct is_abstract : public integral_constant<bool, __is_abstract(T)> {};

template<typename T>
struct has_virtual_destructor : public integral_constant<bool, __has_virtual_destructor(T)> {};

template <class T>
struct is_unsigned; // implemented in type_traits_cxx98.hpp

template <> struct is_unsigned<unsigned char> : public true_type{};
template <> struct is_unsigned<const unsigned char> : public true_type{};
template <> struct is_unsigned<volatile unsigned char> : public true_type{};
template <> struct is_unsigned<const volatile unsigned char> : public true_type{};
template <> struct is_unsigned<unsigned short> : public true_type{};
template <> struct is_unsigned<const unsigned short> : public true_type{};
template <> struct is_unsigned<volatile unsigned short> : public true_type{};
template <> struct is_unsigned<const volatile unsigned short> : public true_type{};
template <> struct is_unsigned<unsigned int> : public true_type{};
template <> struct is_unsigned<const unsigned int> : public true_type{};
template <> struct is_unsigned<volatile unsigned int> : public true_type{};
template <> struct is_unsigned<const volatile unsigned int> : public true_type{};
template <> struct is_unsigned<unsigned long> : public true_type{};
template <> struct is_unsigned<const unsigned long> : public true_type{};
template <> struct is_unsigned<volatile unsigned long> : public true_type{};
template <> struct is_unsigned<const volatile unsigned long> : public true_type{};

template <> struct is_unsigned<signed char> : public false_type{};
template <> struct is_unsigned<const signed char> : public false_type{};
template <> struct is_unsigned<volatile signed char> : public false_type{};
template <> struct is_unsigned<const volatile signed char> : public false_type{};
template <> struct is_unsigned<short> : public false_type{};
template <> struct is_unsigned<const  short> : public false_type{};
template <> struct is_unsigned<volatile  short> : public false_type{};
template <> struct is_unsigned<const volatile  short> : public false_type{};
template <> struct is_unsigned<int> : public false_type{};
template <> struct is_unsigned<const  int> : public false_type{};
template <> struct is_unsigned<volatile  int> : public false_type{};
template <> struct is_unsigned<const volatile  int> : public false_type{};
template <> struct is_unsigned<long> : public false_type{};
template <> struct is_unsigned<const  long> : public false_type{};
template <> struct is_unsigned<volatile  long> : public false_type{};
template <> struct is_unsigned<const volatile  long> : public false_type{};
template <> struct is_unsigned<unsigned __int64> : public true_type{};
template <> struct is_unsigned<const unsigned __int64> : public true_type{};
template <> struct is_unsigned<volatile unsigned __int64> : public true_type{};
template <> struct is_unsigned<const volatile unsigned __int64> : public true_type{};

template <> struct is_unsigned<signed __int64> : public false_type{};
template <> struct is_unsigned<const signed __int64> : public false_type{};
template <> struct is_unsigned<volatile signed __int64> : public false_type{};
template <> struct is_unsigned<const volatile signed __int64> : public false_type{};

template <> struct is_unsigned<char> : public false_type{};
template <> struct is_unsigned<const char> : public false_type{};
template <> struct is_unsigned<volatile char> : public false_type{};
template <> struct is_unsigned<const volatile char> : public false_type{};

template <> struct is_unsigned<wchar_t> : public false_type{};
template <> struct is_unsigned<const wchar_t> : public false_type{};
template <> struct is_unsigned<volatile wchar_t> : public false_type{};
template <> struct is_unsigned<const volatile wchar_t> : public false_type{};

template <class T>
struct is_signed; // implemented in type_traits_cxx98.hpp

template <> struct is_signed<signed char> : public true_type{};
template <> struct is_signed<const signed char> : public true_type{};
template <> struct is_signed<volatile signed char> : public true_type{};
template <> struct is_signed<const volatile signed char> : public true_type{};
template <> struct is_signed<short> : public true_type{};
template <> struct is_signed<const short> : public true_type{};
template <> struct is_signed<volatile short> : public true_type{};
template <> struct is_signed<const volatile short> : public true_type{};
template <> struct is_signed<int> : public true_type{};
template <> struct is_signed<const int> : public true_type{};
template <> struct is_signed<volatile int> : public true_type{};
template <> struct is_signed<const volatile int> : public true_type{};
template <> struct is_signed<long> : public true_type{};
template <> struct is_signed<const long> : public true_type{};
template <> struct is_signed<volatile long> : public true_type{};
template <> struct is_signed<const volatile long> : public true_type{};

template <> struct is_signed<unsigned char> : public false_type{};
template <> struct is_signed<const unsigned char> : public false_type{};
template <> struct is_signed<volatile unsigned char> : public false_type{};
template <> struct is_signed<const volatile unsigned char> : public false_type{};
template <> struct is_signed<unsigned short> : public false_type{};
template <> struct is_signed<const unsigned short> : public false_type{};
template <> struct is_signed<volatile unsigned short> : public false_type{};
template <> struct is_signed<const volatile unsigned short> : public false_type{};
template <> struct is_signed<unsigned int> : public false_type{};
template <> struct is_signed<const unsigned int> : public false_type{};
template <> struct is_signed<volatile unsigned int> : public false_type{};
template <> struct is_signed<const volatile unsigned int> : public false_type{};
template <> struct is_signed<unsigned long> : public false_type{};
template <> struct is_signed<const unsigned long> : public false_type{};
template <> struct is_signed<volatile unsigned long> : public false_type{};
template <> struct is_signed<const volatile unsigned long> : public false_type{};

template <> struct is_signed<signed __int64> : public true_type{};
template <> struct is_signed<const signed __int64> : public true_type{};
template <> struct is_signed<volatile signed __int64> : public true_type{};
template <> struct is_signed<const volatile signed __int64> : public true_type{};

template <> struct is_signed<unsigned __int64> : public false_type{};
template <> struct is_signed<const unsigned __int64> : public false_type{};
template <> struct is_signed<volatile unsigned __int64> : public false_type{};
template <> struct is_signed<const volatile unsigned __int64> : public false_type{};

template <> struct is_signed<char> : public false_type{};
template <> struct is_signed<const char> : public false_type{};
template <> struct is_signed<volatile char> : public false_type{};
template <> struct is_signed<const volatile char> : public false_type{};

template <> struct is_signed<wchar_t> : public false_type{};
template <> struct is_signed<const wchar_t> : public false_type{};
template <> struct is_signed<volatile wchar_t> : public false_type{};
template <> struct is_signed<const volatile wchar_t> : public false_type{};

template <class T> struct is_integral : public false_type {};
template <class T> struct is_integral<const T> : public is_integral<T> {};
template <class T> struct is_integral<volatile const T> : public is_integral<T>{};
template <class T> struct is_integral<volatile T> : public is_integral<T>{};

template<> struct is_integral<unsigned char> : public true_type {};
template<> struct is_integral<unsigned short> : public true_type{};
template<> struct is_integral<unsigned int> : public true_type{};
template<> struct is_integral<unsigned long> : public true_type{};
template<> struct is_integral<unsigned __int64> : public true_type{};

template<> struct is_integral<signed char> : public true_type{};
template<> struct is_integral<short> : public true_type{};
template<> struct is_integral<int> : public true_type{};
template<> struct is_integral<long> : public true_type{};
template<> struct is_integral<__int64> : public true_type{};

template<> struct is_integral<char> : public true_type{};
template<> struct is_integral<bool> : public true_type{};

template <class T> struct is_floating_point : public false_type{};
template <class T> struct is_floating_point<const T> : public is_floating_point<T>{};
template <class T> struct is_floating_point<volatile const T> : public is_floating_point<T>{};
template <class T> struct is_floating_point<volatile T> : public is_floating_point<T>{};
template<> struct is_floating_point<float> : public true_type{};
template<> struct is_floating_point<double> : public true_type{};
template<> struct is_floating_point<long double> : public true_type{};

template <class T>
struct is_arithmetic : public integral_constant<bool,
is_integral<T>::value || is_floating_point<T>::value> {};

template <class T>
struct is_function
: public integral_constant<bool, details::is_function_impl<T>::value>
{};

// Work in Progress
/*
namespace details
{
template <typename T>
struct is_member_function_pointer_impl
    : public is_mem_fun_pointer_select<is_reference<T>::value>::template result_<T>{};

template <typename T>
struct is_member_function_pointer_impl<T&> : public false_type{};
template<> struct is_member_function_pointer_impl<void> : public false_type{};
} // namespace details

template <class T>
struct is_member_function_pointer
    : public integral_constant<bool, nh3api::details::is_member_function_pointer_impl<T>::value>{};

template <class T> struct is_member_pointer : public integral_constant<bool, is_member_function_pointer<T>::value>{};
template <class T, class U> struct is_member_pointer<U T::* > : public true_type{};

template <class T> struct is_member_object_pointer
   : public integral_constant<bool,
   is_member_pointer<T>::value && !is_member_function_pointer<T>::value>{};

*/

    template <class T> struct is_fundamental
    : public integral_constant<bool, is_arithmetic<T>::value || is_void<T>::value>
    {};

    template <class T> struct is_scalar
    : public integral_constant<bool,
    is_arithmetic<T>::value
    || is_pointer<T>::value
    /*|| is_member_pointer<T>::value*/
    || is_enum<T>::value>
    {};

    template <class T> struct is_compound
    : public integral_constant<bool, !is_fundamental<T>::value>
    {};

    template <class T> struct is_object : public integral_constant<bool,
    !(is_void<T>::value || is_reference<T>::value || is_function<T>::value)>
    {};

    template<typename T>
    struct is_trivially_copyable : public integral_constant<bool,
    is_union<T>::value || __has_trivial_copy(T) || is_scalar<T>::value || is_pod<T>::value >
    {};

    template<typename T>
    struct is_trivially_copy_assignable : public integral_constant<bool,
    !is_const<T>::value && (__has_trivial_assign(T) || is_scalar<T>::value || is_pod<T>::value) >
    {};

    template<typename T>
    struct is_trivially_default_constructible : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    __has_trivial_constructor(T)>
    {};

    template<typename T>
    struct is_trivially_destructible : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    __has_trivial_destructor(T)> {};

    template<typename T>
    struct is_nothrow_assignable : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    __has_nothrow_assign(T)>
    {};

    template<typename T>
    struct is_nothrow_default_constructible : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    __has_nothrow_constructor(T)>
    {};

    template<typename T>
    struct is_nothrow_destructible // this is not precisely what we want but we're ok
        : public integral_constant<bool, __has_trivial_destructor(T)>
    {};

    template<typename T>
    struct is_nothrow_copy_constructible : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    __has_nothrow_copy(T)>
    {};

    template<typename T>
    struct is_trivial : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    ((is_union<T>::value || __has_trivial_copy(T)) && __has_trivial_constructor(T))>
    {};

    template<typename T>
    struct is_trivially_copy_constructible : public integral_constant<bool,
    is_scalar<T>::value || is_pod<T>::value ||
    ((is_union<T>::value || __has_trivial_copy(T)))>
    {};

    template<class T>
    struct is_array : false_type {};

    template<class T>
    struct is_array<T[]> : true_type {};

    template<class T, size_t N>
    struct is_array<T[N]> : true_type {};

} // namespace tt
} // namespace nh3api

#pragma component(mintypeinfo, off)
#else
#include <type_traits>
namespace nh3api
{
namespace tt
{
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

} // namespace tt
} // namespace nh3api
#endif // NH3API_TYPE_TRAITS_IMPL