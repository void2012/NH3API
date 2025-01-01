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

#include "check.hpp"

#if NH3API_TYPE_TRAITS_IMPL
#pragma component(mintypeinfo, on)

namespace nh3api
{

namespace tt
{
namespace details
{
template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{
    enum { value = A < S ? A : S };
};

template<typename T>
struct alignment_of_impl
{
    enum { value = (alignment_logic<sizeof(alignment_of_hack<T>) - sizeof(T), __alignof(T)>::value)};
};
} // namespace details

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template<class T> struct type_identity
{ typedef T type; };

template< class Base, class Derived >
struct is_base_of : public integral_constant<bool, __is_base_of(Base, Derived)> {};

template <class T, class U> struct is_same : public false_type {};
template <class T> struct is_same<T,T> : public true_type {};
template <class T> struct is_same<T&, T&> : public true_type{};

#pragma warning(push)
#pragma warning(disable: 4121 4512 4324)
template <typename T> struct alignment_of;

template <class T>
struct alignment_of : public integral_constant<size_t, details::alignment_of_impl<T>::value>
{};

template <typename T>
struct alignment_of<T&> : public alignment_of<T*>
{};

template<> struct alignment_of<void> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void const> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void const volatile> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void volatile> : integral_constant<size_t, 0>{};

template <bool b, class T, class U>
struct conditional
{ typedef T type; };
template <class T, class U>
struct conditional<false, T, U>
{ typedef U type; };

template <class From, class To>
struct is_convertible : public integral_constant<bool, __is_convertible_to(From, To)> {};

template <class T>
struct is_void : public false_type {};

template<> struct is_void<void> : public true_type {};
template<> struct is_void<const void> : public true_type{};
template<> struct is_void<const volatile void> : public true_type{};
template<> struct is_void<volatile void> : public true_type{};

template <class T> struct is_lvalue_reference : public false_type{};
template <class T> struct is_lvalue_reference<T&> : public true_type{};

template <class T> struct is_reference
   : public integral_constant<bool, is_lvalue_reference<T>::value> {};

template <class T> struct is_pointer : public false_type{};
template <class T> struct is_pointer<T*> : public true_type{};
template <class T> struct is_pointer<T* const> : public true_type{};
template <class T> struct is_pointer<T* const volatile> : public true_type{};
template <class T> struct is_pointer<T* volatile> : public true_type{};
template <class T> struct is_pointer<T const> : public is_pointer<T>{};
template <class T> struct is_pointer<T const volatile> : public is_pointer<T>{};
template <class T> struct is_pointer<T volatile> : public is_pointer<T>{};

template<class B>
struct negation : integral_constant<bool, !bool(B::value)>
{};

template <typename T1, typename T2>
struct conjunction_2
    : false_type
{};

template <>
struct conjunction_2<true_type, true_type>
    : true_type
{};

template <typename T, typename U>
struct disjunction_2
    : true_type
{};

template <>
struct disjunction_2<false_type, false_type>
    : false_type
{};

} // namespace tt
} // namespace nh3api

#pragma warning(pop)

#pragma component(mintypeinfo, off)

#else
#include <type_traits>
namespace nh3api
{
namespace tt
{
using ::std::enable_if;
using ::std::is_base_of;
using ::std::is_same;
using ::std::alignment_of;

#ifdef __cpp_lib_type_identity
using ::std::type_identity;
#else
template<class T> struct type_identity
{ typedef T type; };
#endif
using ::std::conditional;
using ::std::is_convertible;
using ::std::is_void;
using ::std::is_lvalue_reference;
using ::std::is_rvalue_reference;
using ::std::is_reference;

#ifdef __cpp_lib_logical_traits
using ::std::negation;

template<typename T1, typename T2>
struct conjunction_2
    : ::std::conjunction<T1, T2>
{};

template<typename T1, typename T2>
struct disjunction_2
    : ::std::disjunction<T1, T2>
{};
#else
template<class B>
struct negation : integral_constant<bool, !bool(B::value)>
{};

template <typename T1, typename T2>
struct conjunction_2
    : conditional<T1::value, T2, false_type>::type
{};

template <>
struct conjunction_2<true_type, true_type>
    : true_type
{};

template <typename T1, typename T2>
struct disjunction_2
    : conditional<T1::value, true_type, T2>::type
{};

template <>
struct disjunction_2<false_type, false_type>
    : false_type
{};
#endif
using ::std::is_pointer;
} // namespace tt
} // namespace nh3api
#endif