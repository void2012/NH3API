//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <algorithm> 
#include "iterator.hpp" // tt::is_random_access_iterator
#include "memory.hpp"   // exe_allocator

#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
NH3API_DISABLE_MSVC_WARNING_BEGIN(4996)
#endif

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, on)
#endif

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

namespace nh3api
{

// fix broken ::std::fill_n on visual studio 2005..2008
// as it returns void instead of an iterator
// also optimize it

#if NH3API_CHECK_MSVC && NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)

#pragma intrinsic(__stosb)
#pragma intrinsic(__stosw)
#pragma intrinsic(__stosd)
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)

// since pre-C++17 compilers have no 'if constexpr',
// use structs to switch implementations
// stosx_chooser or movsx_chooser are then called but do nothing on wrong overload 
// to not to cause compile-time errors

template<typename T, size_t SizeOfT = sizeof(T)>
struct stosx_chooser
{
    template<typename IterT>
    inline static void stosx(IterT, const T&, size_t) NH3API_NOEXCEPT
    {} // intentional no-op
};

template<typename T, size_t SizeOfT = sizeof(T)>
struct movsx_chooser
{
    template<typename IterT, typename ConstIterT>
    inline static void movsx(IterT, ConstIterT, size_t) NH3API_NOEXCEPT
    {} // intentional no-op
};

template<typename T>
struct stosx_chooser<T, 1>
{
    inline static void stosx(T* dst, const T& value, size_t count) NH3API_NOEXCEPT
    { __stosb(reinterpret_cast<unsigned char*>(dst), bit_cast<unsigned char>(value), (count)); }
};

template<typename T>
struct movsx_chooser<T, 1>
{
    inline static void movsx(T* dst, const T* src, size_t count) NH3API_NOEXCEPT
    { __movsb(reinterpret_cast<unsigned char*>(dst), reinterpret_cast<const unsigned char*>(src), (count)); }
};

template<typename T>
struct stosx_chooser<T, 2>
{
    inline static void stosx(T* dst, const T& value, size_t count) NH3API_NOEXCEPT
    { __stosw(reinterpret_cast<unsigned short*>(dst), bit_cast<unsigned short>(value), (count)); }
};

template<typename T>
struct movsx_chooser<T, 2>
{
    inline static void movsx(T* dst, const T* src, size_t count) NH3API_NOEXCEPT
    { __movsw(reinterpret_cast<unsigned short*>(dst), reinterpret_cast<const unsigned short*>(src), (count)); }
};

template<typename T>
struct stosx_chooser<T, 4>
{
    inline static void stosx(T* dst, const T& value, size_t count) NH3API_NOEXCEPT
    { __stosd(reinterpret_cast<unsigned long*>(dst), bit_cast<unsigned long>(value), (count)); }
};

template<typename T>
struct movsx_chooser<T, 4>
{
    inline static void movsx(T* dst, const T* src, size_t count) NH3API_NOEXCEPT
    { __movsd(reinterpret_cast<unsigned long*>(dst), reinterpret_cast<const unsigned long*>(src), (count)); }
};

#endif

template <class OutputIt, class Size, class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt fill_n(OutputIt first, Size count, const T& value)
{
#if NH3API_DEBUG
    verify_range_n(first, count);
#endif
// broken std::fill_n on visual studio 2005..2008
// optimized for visual studio 2005..2013
#if NH3API_CHECK_MSVC && (NH3API_MSVC_STL_VERSION < NH3API_MSVC_STL_VERSION_2010)
    if ( tt::is_trivially_copy_assignable<T>::value // trivial assign
         && tt::is_random_access_iterator<OutputIt>::value // linear memory 
         && (sizeof(T) == 4 || sizeof(T) == 2 || sizeof(T) == 1) ) // one of the overloads
    {
        stosx_chooser<T>::stosx(unfancy(first), value, count);
        return first + count;
    }
    else 
    {
        for (; 0 < count; --count, ++first)
            *first = value;
        return first;
    } 
#else
    return ::std::fill_n(first, count, value);
#endif
}

template <class ForwardIt, class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
void fill(ForwardIt first, ForwardIt last, const T &value)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif
// optimized std::fill for visual studio 2005..2013
#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
    if ( tt::is_trivially_copy_assignable<T>::value // trivial assign
         && tt::is_random_access_iterator<ForwardIt>::value // linear memory 
         && (sizeof(T) == 4 || sizeof(T) == 2 || sizeof(T) == 1) ) // one of the overloads
    {
        stosx_chooser<T>::stosx(unfancy(first), value, static_cast<size_t>(::std::distance(first, last)));
    }
    else 
    {
        ::std::fill(first, last, value);
    }
#else
    // memset wrapper
    ::std::fill(first, last, value);
#endif
}

template<class InputIt, class Size, class OutputIt>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
#if NH3API_DEBUG
    verify_range_n(first, count);
#endif

// optimized std::copy_n for visual studio 2005..2013
// implement std::copy_n for visual studio 2005..2010
#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
    typedef typename ::std::iterator_traits<InputIt>::value_type ValueT;
    if ( tt::is_trivially_copy_assignable<ValueT>::value // trivial assign
         && tt::is_random_access_iterator<InputIt>::value // linear memory
         && tt::is_random_access_iterator<OutputIt>::value // linear memory 
         && (sizeof(ValueT) == 4 || sizeof(ValueT) == 2 || sizeof(ValueT) == 1) ) // one of the overloads
    {
        movsx_chooser<ValueT>::movsx(unfancy(result), unfancy(first), count);
    }
    else 
    {
        #if NH3API_MSVC_STL_VERSION >= NH3API_MSVC_STL_VERSION_2010
        return ::std::copy_n(first, count, result);
        #else 
        if (count > 0)
        {
            *result = *first;
            ++result;
            for (Size i = 1; i != count; ++i, (void)++result)
                *result = *++first;
        }
    
        return result;
        #endif
    }
#else
    return ::std::copy_n(first, count, result);
#endif
}

template<class InputIt, class OutputIt>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

// optimized std::copy_n for visual studio 2005..2013
#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
    typedef typename ::std::iterator_traits<InputIt>::value_type ValueT;
    if ( tt::is_trivially_copy_assignable<ValueT>::value // trivial assign
         && tt::is_random_access_iterator<InputIt>::value // linear memory
         && tt::is_random_access_iterator<OutputIt>::value // linear memory 
         && (sizeof(ValueT) == 4 || sizeof(ValueT) == 2 || sizeof(ValueT) == 1) ) // one of the overloads
    {
        movsx_chooser<ValueT>::movsx(unfancy(d_first), unfancy(first), static_cast<size_t>(::std::distance(first, last)));
    }
    else 
    {
        return ::std::copy(first, last, d_first);
    }
#else
    return ::std::copy(first, last, d_first);
#endif
}

/*
NH3API_FORCEINLINE
#ifdef __cpp_lib_constexpr_algorithms
constexpr
#endif
void
swap_chars_range_constexpr(char* __restrict first1,
                           char* __restrict last1,
                           char* __restrict first2) NH3API_NOEXCEPT
{
    #ifdef __cpp_lib_constexpr_algorithms
    ::std::swap_ranges(first1, last1, first2);
    #else
    while ( first1 != last1 )
    {
        char temp = *first1;
        *first1++ = *first2++;
        *(first2 - 1) = temp;
    }
    #endif
}
*/

} // namespace nh3api

#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
NH3API_DISABLE_MSVC_WARNING_END
#endif

NH3API_DISABLE_WARNING_END
