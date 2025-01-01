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

#pragma warning(push)
#pragma warning(disable:4181)

namespace nh3api
{
namespace tt
{
namespace details
{
template <typename T>
struct add_reference_impl
{
    typedef T& type;
};

} // namespace details

template <class T> struct add_volatile
{ typedef T volatile type; };
template <class T> struct add_volatile<T&>
{ typedef T& type; };

template <class T> struct add_const
{ typedef T const type; };
template <class T> struct add_const<T&>
{ typedef T& type; };

template <class T> struct add_cv
{ typedef T const volatile type; };
template <class T> struct add_cv<T&>
{ typedef T& type; };

template <class T> struct add_lvalue_reference
{ typedef typename details::add_reference_impl<T>::type type; };

template <> struct add_lvalue_reference<void> { typedef void type; };
template <> struct add_lvalue_reference<const void> { typedef const void type; };
template <> struct add_lvalue_reference<const volatile void> { typedef const volatile void type; };
template <> struct add_lvalue_reference<volatile void> { typedef volatile void type; };

} // namespace tt
} // namespace nh3api

#pragma component(mintypeinfo, off)

#else
#include <type_traits>
namespace nh3api
{
namespace tt
{
using ::std::add_volatile;
using ::std::add_const;
using ::std::add_cv;
using ::std::add_lvalue_reference;

} // namespace tt
} // namespace nh3api
#endif // NH3API_TYPE_TRAITS_IMPL