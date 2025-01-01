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

#if (NH3API_TYPE_TRAITS_IMPL)

#pragma component(mintypeinfo, on)

namespace nh3api
{

namespace tt
{
namespace details
{

template<bool C_>
struct bool_
{
    enum : unsigned char { value = C_ };
    typedef bool_ type;
    typedef bool value_type;
    operator bool() const { return value; }
};

typedef char yes_type;
struct no_type
{ char _[2]; };

template <class I, I val> struct integral_c;

} // namespace details

template <typename T, T val>
struct integral_constant
{
    typedef T                         value_type;
    typedef integral_constant<T, val> type;
    static const value_type value;
protected:
    typedef const details::integral_c<T, val>& impl_integral_c_type_ref;
    typedef const details::integral_c<T, val>* impl_integral_c_type_ptr;
public:

    operator impl_integral_c_type_ref () const
    {
        static const char data[sizeof(void*)] = { 0 };
        return *(reinterpret_cast<impl_integral_c_type_ptr>(&data));
    }
    operator T() const { return val; }
}; // struct integral_constant

template <class T, T val>
T const integral_constant<T, val>::value = val;

template <bool val>
struct integral_constant<bool, val>
{
protected:
    typedef const details::bool_<val>& impl_bool_type;
public:
    typedef bool value_type;
    typedef integral_constant<bool, val> type;
    static const bool value = val;

    operator impl_bool_type ()const
    {
        static const char data = 0;
        return *(reinterpret_cast<const details::bool_<val>*>(&data));
    }
    operator bool() const
    { return val; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;
} // namespace tt
} // namespace nh3api

#pragma component(mintypeinfo, off)

#else
#include <type_traits>
namespace nh3api
{
namespace tt
{
using ::std::integral_constant;
using ::std::true_type;
using ::std::false_type;
} // namespace tt
} // namespace nh3api
#endif // NH3API_TYPE_TRAITS_IMPL