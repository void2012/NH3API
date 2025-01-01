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

#include "is_xyz.hpp"
#include "common.hpp"

#if NH3API_TYPE_TRAITS_IMPL

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, on)
#endif

namespace nh3api
{
namespace tt
{
template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<T const> { typedef T type; };

template <class T> struct remove_volatile { typedef T type; };
template <class T> struct remove_volatile<T volatile> { typedef T type; };

template <class T> struct remove_cv { typedef T type; };
template <class T> struct remove_cv<T const> { typedef T type;  };
template <class T> struct remove_cv<T volatile> { typedef T type; };
template <class T> struct remove_cv<T const volatile> { typedef T type; };

template <class T> struct remove_reference { typedef T type; };
template <class T> struct remove_reference<T&> { typedef T type; };

template <class T> struct remove_cvref
    : remove_cv<typename remove_reference<T>::type> {};

namespace details
{
template <class T> struct remove_pointer_imp { typedef T type; };
template <class T> struct remove_pointer_imp<T*> { typedef T type; };
template <class T, bool b> struct remove_pointer_imp3{ typedef typename remove_pointer_imp<typename remove_cv<T>::type>::type type; };
template <class T> struct remove_pointer_imp3<T, false> { typedef T type; };
template <class T> struct remove_pointer_imp2 { typedef typename remove_pointer_imp3<T, is_pointer<T>::value>::type type; };
template <class T, bool Array, bool Function> struct decay_impl { typedef typename remove_cv<T>::type type; };
template <class T> struct decay_impl<T, true, false> { typedef typename type_identity<T>::type* type; };
template <class T> struct decay_impl<T, false, true> { typedef T* type; };
} // namespace details

template <class T> struct remove_pointer
{ typedef typename details::remove_pointer_imp2<T>::type type; };

template< class T >
struct decay
{
    private:
        typedef typename remove_reference<T>::type Ty;
    public:
        typedef typename details::decay_impl<Ty, false_type::value, is_function<Ty>::value>::type type;
};
} // namespace tt
} // namespace nh3api

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, off)
#endif

#else
#include <type_traits>
namespace nh3api
{
namespace tt
{
using ::std::remove_const;
using ::std::remove_volatile;
using ::std::remove_cv;
using ::std::remove_reference;
#ifdef __cpp_lib_remove_cvref
using ::std::remove_cvref;
#else
template <class T>
struct remove_cvref
: ::std::remove_cv<typename remove_reference<T>::type>
{};
#endif
using ::std::remove_pointer;
using ::std::decay;
} // namespace tt
} // namespace nh3api

#endif // NH3API_TYPE_TRAITS_IMPL