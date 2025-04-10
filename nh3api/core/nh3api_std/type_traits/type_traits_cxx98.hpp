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
//  is_underlying<T> implementation by void_17
#pragma once

#include "check.hpp"

#if !NH3API_TYPE_TRAITS_IMPL
#include <type_traits>
#endif

#include "integral_constant.hpp"
#include "is_xyz.hpp"
#include "remove_xyz.hpp"
#include "add_xyz.hpp"

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, on)
#endif

namespace nh3api
{
namespace tt
{
template<class From, class To> struct copy_cv
{
private:
    typedef typename conditional<is_const<From>::value, typename add_const<To>::type, To>::type CT;
public:
    typedef typename conditional<is_volatile<From>::value, typename add_volatile<CT>::type, CT>::type type;
};
} // namespace tt
} // namespace nh3api

#if NH3API_TYPE_TRAITS_IMPL

namespace nh3api
{
namespace tt
{
template <typename T> struct add_pointer
{ typedef T* type; };
template <typename T> struct add_pointer<T&>
{ typedef typename remove_cv<T>::type* type; };

namespace details
{
/*
template<class T>
struct remove_all_extents 
{
    typedef T type;
};

template<class T>
struct remove_all_extents<T[]> 
{
    typedef typename remove_all_extents<T>::type type;
};

template<class T, size_t N>
struct remove_all_extents<T[N]> 
{
    typedef typename remove_all_extents<T>::type type;
};

template<class T>
struct element_type 
{
    typedef typename remove_cv<typename remove_all_extents<typename
        remove_reference<T>::type>::type>::type type;
};

template<size_t A, size_t B>
struct min_size
    : integral_constant<size_t, (A < B) ? A : B> 
{};

template<class T>
struct offset_value 
{
    T first;
    char value;
    T second;
};

template<typename T>
struct alignment_of_impl
    : min_size<sizeof(T), sizeof(offset_value<T>) - (sizeof(T) << 1)> 
{};
*/

template <class T>
struct is_signed_values
{
    typedef typename remove_cv<T>::type no_cv_t;
    static const no_cv_t minus_one = (static_cast<no_cv_t>(-1));
    static const no_cv_t zero = (static_cast<no_cv_t>(0));
};

template <class T>
struct is_signed_helper
{
    typedef typename remove_cv<T>::type no_cv_t;
    static const bool value = !(is_signed_values<T>::minus_one > is_signed_values<T>::zero);
};

template <bool integral_type>
struct is_signed_select_helper
{
    template <class T>
    struct rebind
    { typedef is_signed_helper<T> type; };
};

template <>
struct is_signed_select_helper<false>
{
    template <class T>
    struct rebind
    { typedef false_type type; };
};

template <class T>
struct is_signed_impl
{
    typedef is_signed_select_helper<is_integral<T>::value || is_enum<T>::value> selector;
    typedef typename selector::template rebind<T> binder;
    typedef typename binder::type type;
    static const bool value = type::value;
};

template <class T>
struct is_unsigned_values
{
    typedef typename remove_cv<T>::type no_cv_t;
    static const no_cv_t minus_one = (static_cast<no_cv_t>(-1));
    static const no_cv_t zero = (static_cast<no_cv_t>(0));
};

template <class T>
struct is_ununsigned_helper
{
    static const bool value = is_unsigned_values<T>::minus_one > is_unsigned_values<T>::zero;
};

template <bool integral_type>
struct is_unsigned_select_helper
{
    template <class T>
    struct rebind
    { typedef is_ununsigned_helper<T> type; };
};

template <>
struct is_unsigned_select_helper<false>
{
    template <class T>
    struct rebind
    { typedef false_type type; };
};

template <class T>
struct is_unsigned_impl
{
    typedef is_unsigned_select_helper< is_integral<T>::value || is_enum<T>::value > selector;
    typedef typename selector::template rebind<T> binder;
    typedef typename binder::type type;
    static const bool value = type::value;
};

template<typename EnumType, typename IntType>
struct enum_same_type
{
    static const bool value = (sizeof(EnumType) == sizeof(IntType))
                              && (is_convertible<EnumType, IntType>::value)
                              && (is_unsigned<EnumType>::value == is_unsigned<IntType>::value)
                              && (is_signed<EnumType>::value == is_signed<IntType>::value);
};

#define NH3API_ENUM_ST(TYPE) enum_same_type<EnumType, TYPE>::value NH3API_MACRO_COMMA TYPE

template<typename EnumType, bool IsEnum>
struct enum_underlying_impl // false, false case
{ typedef void type; };

template<typename EnumType>
struct enum_underlying_impl<EnumType, true> // this is UGLY and certainly will slow down the compile time.
                                            // but this will make std::underlying_type work in VS2005...
{
    typedef typename
            conditional<NH3API_ENUM_ST(signed __int8), typename
            conditional<NH3API_ENUM_ST(unsigned __int8), typename
            conditional<NH3API_ENUM_ST(signed __int16), typename
            conditional<NH3API_ENUM_ST(unsigned __int16), typename
            conditional<NH3API_ENUM_ST(signed __int32), typename
            conditional<NH3API_ENUM_ST(unsigned __int32), typename
            conditional<NH3API_ENUM_ST(signed __int64), typename
            conditional<NH3API_ENUM_ST(unsigned __int64), void>::type >::type >::type >::type >::type >::type >::type >::type type; // ::type MACHINE GUN!!!
};
#undef NH3API_ENUM_ST

#define NH3API_CORRESP_STYPE_SPEC(TYPE) template<> struct corresp_signed_type<unsigned TYPE> { typedef signed TYPE type; };
#define NH3API_CORRESP_UTYPE_SPEC(TYPE) template<> struct corresp_signed_type<signed TYPE> { typedef unsigned TYPE type; };
#define NH3API_CORRESP_SPEC(TYPE) NH3API_CORRESP_STYPE_SPEC(TYPE) NH3API_CORRESP_UTYPE_SPEC(TYPE)

template<typename T>
struct corresp_signed_type
{ typedef void type; };

template<typename T>
struct corresp_unsigned_type
{ typedef void type; };

NH3API_CORRESP_SPEC(__int8)
NH3API_CORRESP_SPEC(__int16)
NH3API_CORRESP_SPEC(__int32)
NH3API_CORRESP_SPEC(__int64)

#undef NH3API_CORRESP_SPEC

template<typename T>
struct make_signed
{
    typedef typename remove_cv<T>::type no_cv_t;
    // 1) if T is enum, convert to signed underlying type.
    // 2) also copy cv qualifiers
    typedef typename copy_cv<typename conditional<is_enum<T>::value,
                                       typename enum_underlying_impl<no_cv_t, true>::type,
                                       typename corresp_signed_type<no_cv_t>::type>::type, T>::type type;
};

template<typename T>
struct make_unsigned
{
    typedef typename remove_cv<T>::type no_cv_t;
    // 1) if T is enum, convert to signed underlying type.
    // 2) also copy cv qualifiers
    typedef typename copy_cv<typename conditional<is_enum<T>::value,
                                       typename enum_underlying_impl<no_cv_t, true>::type,
                                       typename corresp_unsigned_type<no_cv_t>::type>::type, T>::type type;
};

#define NH3API_MAKE_SIGNED_SPEC(TYPE) template <> struct make_signed<TYPE> { typedef signed TYPE type; };
#define NH3API_MAKE_UNSIGNED_SPEC(TYPE) template <> struct make_unsigned<TYPE> { typedef unsigned TYPE type; };

NH3API_MAKE_SIGNED_SPEC(char)
NH3API_MAKE_SIGNED_SPEC(const char)
NH3API_MAKE_SIGNED_SPEC(volatile char)
NH3API_MAKE_SIGNED_SPEC(const volatile char)

NH3API_MAKE_UNSIGNED_SPEC(char)
NH3API_MAKE_UNSIGNED_SPEC(const char)
NH3API_MAKE_UNSIGNED_SPEC(volatile char)
NH3API_MAKE_UNSIGNED_SPEC(const volatile char)

#undef NH3API_MAKE_SIGNED_SPEC
#undef NH3API_MAKE_UNSIGNED_SPEC
} // namespace details

template <class T> struct is_signed
: public integral_constant<bool, details::is_signed_impl<T>::value> {};
template <class T> struct is_unsigned
: public integral_constant<bool, details::is_unsigned_impl<T>::value> {};

template <class T>
struct make_signed
{
    // if it is already signed, leave T unchanged.
    typedef typename conditional<is_signed<T>::value,
                                 T,
                                 typename details::make_signed<T>::type>::type type;
};

template <class T>
struct make_unsigned
{
    // if it is already signed, leave T unchanged.
    typedef typename conditional<is_unsigned<T>::value,
                                 T,
                                 typename details::make_unsigned<T>::type>::type type;
};

template<class T>
struct underlying_type
{
private:
    static const bool is_enum_value = is_enum<T>::value;
public:
    typedef typename copy_cv<typename details::enum_underlying_impl<T, is_enum_value>::type, T>::type type;
};

template <typename T> struct alignment_of;

template <class T>
struct alignment_of : public integral_constant<size_t, __alignof(T)>
{};

template <typename T>
struct alignment_of<T&> : public alignment_of<T*>
{};

template<> struct alignment_of<void> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void const> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void const volatile> : integral_constant<size_t, 0>{};
template<> struct alignment_of<void volatile> : integral_constant<size_t, 0>{};

} // namespace tt
} // namespace nh3api

#else
namespace nh3api
{
namespace tt
{
using ::std::add_pointer;
using ::std::is_signed;
using ::std::is_unsigned;
using ::std::make_signed;
using ::std::make_unsigned;
using ::std::underlying_type;
} // namespace tt
} // namespace nh3api
#endif

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, off)
#endif
