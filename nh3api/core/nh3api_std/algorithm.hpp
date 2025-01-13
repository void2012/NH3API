//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "memory.hpp"
#include "nh3api_std.hpp"
#include "type_traits.hpp"
#include "iterator.hpp"

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, on)
#endif

#ifndef NH3API_ALGORITHMS_INLINED
    #if (defined(_MSC_VER) || defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)) && !defined(_MSVC_STL_UPDATE)
        #define NH3API_ALGORITHMS_INLINED (1)
    #else
        #define NH3API_ALGORITHMS_INLINED (0)
    #endif
#endif

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

namespace nh3api
{

// fix broken ::std::fill_n on visual studio 2005..2008
// as it returns void instead of an iterator
// also optimize it

#if defined(_MSC_VER) && !defined(__clang__) && NH3API_ALGORITHMS_INLINED
    #pragma intrinsic(__stosb)
    #pragma intrinsic(__stosw)
    #pragma intrinsic(__stosd)
    #pragma intrinsic(__movsb)
    #pragma intrinsic(__movsw)
    #pragma intrinsic(__movsd)
#endif

// ::std::fill_n fix

template <class OutputIt, class Size, class T>
inline OutputIt fill_n_impl(OutputIt first, Size count, const T &value)
{
#if defined(_MSC_VER) && (_MSC_VER < 1600)
    for (; 0 < count; --count, ++first)
        *first = value;
    return first;
#else
    return ::std::fill_n(first, count, value);
#endif
}

// 8-bit overloads

#if NH3API_ALGORITHMS_INLINED

template <typename Size> inline char *fill_n_impl(char *first, Size count, char value)
{
    __stosb(reinterpret_cast<unsigned char *>(first), static_cast<unsigned char>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline signed char *fill_n_impl(signed char *first, Size count, signed char value)
{
    __stosb(reinterpret_cast<unsigned char *>(first), static_cast<unsigned char>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline unsigned char *fill_n_impl(unsigned char *first, Size count, unsigned char value)
{
    __stosb(reinterpret_cast<unsigned char *>(first), static_cast<unsigned char>(value), static_cast<size_t>(count));
    return first + count;
}

// 16-bit overloads

template <typename Size> inline wchar_t *fill_n_impl(wchar_t *first, Size count, wchar_t value)
{
    __stosw(reinterpret_cast<unsigned short *>(first), static_cast<unsigned short>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline short *fill_n_impl(short *first, Size count, short value)
{
    __stosw(reinterpret_cast<unsigned short *>(first), static_cast<unsigned short>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size>
inline unsigned short *fill_n_impl(unsigned short *first, Size count, unsigned short value)
{
    __stosw(first, value, static_cast<size_t>(count));
    return first + count;
}

// 32-bit overloads

template <typename Size> inline int *fill_n_impl(int *first, Size count, int value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline unsigned int *fill_n_impl(unsigned int *first, Size count, unsigned int value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline long *fill_n_impl(long *first, Size count, long value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value), static_cast<size_t>(count));
    return first + count;
}

template <typename Size> inline unsigned long *fill_n_impl(unsigned long *first, Size count, unsigned long value)
{
    __stosd(first, value, static_cast<size_t>(count));
    return first + count;
}

// optimize for pointers
template <typename Size, typename T> inline T **fill_n_impl(T **first, Size count, T *value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value), static_cast<size_t>(count));
    return first + count;
}

// ::std::fill optimizations

template <class ForwardIt, class T> void fill_impl(ForwardIt first, ForwardIt last, const T &value)
{ ::std::fill(first, last, value); }

// 8-bit overloads

inline void fill_impl(char *first, char *last, char value)
{
    __stosb(reinterpret_cast<unsigned char *>(first), static_cast<unsigned char>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(signed char *first, signed char *last, signed char value)
{
    __stosb(reinterpret_cast<unsigned char *>(first), static_cast<unsigned char>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(unsigned char *first, unsigned char *last, unsigned char value)
{
    __stosb(first, value, static_cast<size_t>(last - first));
}

// 16-bit overloads

inline void fill_impl(wchar_t *first, wchar_t *last, wchar_t value)
{
    __stosw(reinterpret_cast<unsigned short *>(first), static_cast<unsigned short>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(short *first, short *last, short value)
{
    __stosw(reinterpret_cast<unsigned short *>(first), static_cast<unsigned short>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(unsigned short *first, unsigned short *last, unsigned short value)
{
    __stosw(first, value, static_cast<size_t>(last - first));
}

// 32-bit overloads

inline void fill_impl(int *first, int *last, int value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(unsigned int *first, unsigned int *last, unsigned int value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(long *first, long *last, long value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value),
            static_cast<size_t>(last - first));
}

inline void fill_impl(unsigned long *first, unsigned long *last, unsigned long value)
{
    __stosd(first, value, static_cast<size_t>(last - first));
}

// optimize for pointers
template <typename T> inline void fill_impl(T **first, T **last, T *value)
{
    __stosd(reinterpret_cast<unsigned long *>(first), static_cast<unsigned long>(value),
            static_cast<size_t>(last - first));
}

// ::std::copy_n optimizations

template <class InputIt, class Size, class OutputIt> OutputIt copy_n_impl(InputIt first, Size count, OutputIt result)
{
    return ::std::copy_n(first, count, result);
}
// 8-bit overloads

template <typename Size> inline char *copy_n_impl(const char *first, Size count, char *result)
{
    __movsb(reinterpret_cast<unsigned char *>(result), reinterpret_cast<const unsigned char *>(first), static_cast<size_t>(count));
    return result + count;
}

template <typename Size>
inline signed char *copy_n_impl(const signed char *first, Size count, signed char *result)
{
    __movsb(reinterpret_cast<unsigned char *>(result), reinterpret_cast<const unsigned char *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size>
inline unsigned char *copy_n_impl(const unsigned char *first, Size count, unsigned char *result)
{
    __movsb(result, first, static_cast<size_t>(count));
    return result + count;
}

// 16-bit overloads

template <typename Size> inline wchar_t *copy_n_impl(const wchar_t *first, Size count, wchar_t *result)
{
    __movsw(reinterpret_cast<unsigned short *>(result), reinterpret_cast<const unsigned short *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size> inline short *copy_n_impl(const short *first, Size count, short *result)
{
    __movsw(reinterpret_cast<unsigned short *>(result), reinterpret_cast<const unsigned short *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size>
inline unsigned short *copy_n_impl(const unsigned short *first, Size count, unsigned short *result)
{
    __movsw(result, first, static_cast<size_t>(count));
    return result + count;
}

// 32-bit overloads

template <typename Size> inline int *copy_n_impl(const int *first, Size count, int *result)
{
    __movsd(reinterpret_cast<unsigned long *>(result), reinterpret_cast<const unsigned long *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size>
inline unsigned int *copy_n_impl(const unsigned int *first, Size count, unsigned int *result)
{
    __movsd(reinterpret_cast<unsigned long *>(result), reinterpret_cast<const unsigned long *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size> inline long *copy_n_impl(const long *first, Size count, long *result)
{
    __movsd(reinterpret_cast<unsigned long *>(result), reinterpret_cast<const unsigned long *>(first),
            static_cast<size_t>(count));
    return result + count;
}

template <typename Size>
inline unsigned long *copy_n_impl(const unsigned long *first, Size count, unsigned long *result)
{
    __movsd(result, first, static_cast<size_t>(count));
    return result + count;
}

// optimize for pointers
template <typename Size, typename T> inline T **copy_n_impl(const T **first, Size count, T **result)
{
    __movsd(reinterpret_cast<unsigned long *>(result), reinterpret_cast<const unsigned long *>(first),
            static_cast<size_t>(count));
    return result + count;
}

// ::std::copy optimizations

template <class InputIt, class OutputIt>
OutputIt copy_impl(InputIt first, InputIt last, OutputIt d_first)
{
    return ::std::copy(first, last, d_first);
}

// 8-bit overloads

inline char *copy_impl(const char *first, const char *last, char *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsb(reinterpret_cast<unsigned char *>(d_first), reinterpret_cast<const unsigned char *>(first), diff);
    return d_first + diff;
}

inline signed char *copy_impl(const signed char *first, const signed char *last, signed char *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsb(reinterpret_cast<unsigned char *>(d_first), reinterpret_cast<const unsigned char *>(first), diff);
    return d_first + diff;
}

inline unsigned char *copy_impl(const unsigned char *first, const unsigned char *last, unsigned char *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsb(d_first, first, diff);
    return d_first + diff;
}

// 16-bit overloads

inline wchar_t *copy_impl(const wchar_t *first, const wchar_t *last, wchar_t *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsw(reinterpret_cast<unsigned short *>(d_first), reinterpret_cast<const unsigned short *>(first), diff);
    return d_first + diff;
}

inline short *copy_impl(const short *first, const short *last, short *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsw(reinterpret_cast<unsigned short *>(d_first), reinterpret_cast<const unsigned short *>(first), diff);
    return d_first + diff;
}

inline unsigned short *copy_impl(const unsigned short *first, const unsigned short *last,
                                        unsigned short *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsw(d_first, first, diff);
    return d_first + diff;
}

// 32-bit overloads

inline int *copy_impl(const int *first, const int *last, int *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsd(reinterpret_cast<unsigned long *>(d_first), reinterpret_cast<const unsigned long *>(first), diff);
    return d_first + diff;
}

inline unsigned int *copy_impl(const unsigned int *first, const unsigned int *last, unsigned int *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsd(reinterpret_cast<unsigned long *>(d_first), reinterpret_cast<const unsigned long *>(first), diff);
    return d_first + diff;
}

inline long *copy_impl(const long *first, const long *last, long *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsd(reinterpret_cast<unsigned long *>(d_first), reinterpret_cast<const unsigned long *>(first), diff);
    return d_first + diff;
}

inline unsigned long *copy_impl(const unsigned long *first, const unsigned long *last, unsigned long *d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsd(d_first, first, diff);
    return d_first + diff;
}

// optimize for pointers
template <typename T>
inline T **copy_impl(const T **first, const T **last, T **d_first)
{
    const size_t diff = static_cast<size_t>(last - first);
    __movsd(reinterpret_cast<unsigned long *>(d_first), reinterpret_cast<const unsigned long *>(first), diff);
    return d_first + diff;
}

#endif

// fix the broken ::std::fill_n, optimizations for ::std::fill and ::std::copy
// on visual studio 2005..2008

#if NH3API_ALGORITHMS_INLINED

namespace details
{

template <bool IsEnum> struct fill_n_enum_helper
{
    template <class OutputType, class Size, class T>
    OutputType *operator()(OutputType *first, Size count, const T &value)
    {
        return nh3api::fill_n_impl(first, count, value);
    }
};

template <> struct fill_n_enum_helper<true>
{
    template <class OutputType, class Size, class T>
    OutputType *operator()(OutputType *first, Size count, const T &value)
    {
        typedef typename nh3api::tt::underlying_type<OutputType>::type OutputUnderlying;
        typedef typename nh3api::tt::underlying_type<T>::type UnderlyingType;
        return reinterpret_cast<OutputType *>(nh3api::fill_n_impl(reinterpret_cast<OutputUnderlying *>(first), count,
                                              static_cast<const UnderlyingType &>(value)));
    }
};

} // namespace details

// optimize fill_n for enums
template <bool IsEnum, class OutputType, class Size, class T>
OutputType *fill_n_enum(OutputType *first, Size count, const T &value)
{
    return nh3api::details::fill_n_enum_helper<IsEnum>()(first, count, value);
}

template <bool IsPtr, class OutputIt, class Size, class T>
OutputIt fill_n_ptr(OutputIt first, Size count, const T &value)
{
    if (IsPtr)
    {
        typedef typename tt::remove_pointer<OutputIt>::type OutputType;
        return nh3api::fill_n_enum < tt::is_enum<T>::value
                                       && tt::is_enum<OutputType>::value,
                                       OutputType, Size, T> (first, count, value);
    }
    else
    {
        return nh3api::fill_n_impl(first, count, value);
    }
}

#endif

// optimized and fixed ::std::fill_n
template <class OutputIt, class Size, class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt fill_n(OutputIt first, Size count, const T& value)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif
// broken ::std::fill_n on visual studio 2005..2008
#if NH3API_ALGORITHMS_INLINED
    return fill_n_ptr<tt::is_pointer<OutputIt>::value>(first, count, value);
#else
    // memset wrapper
    // return ::std::fill_n(first, count, value);
    return ::std::fill_n(first, count, value);
#endif
}

#if NH3API_ALGORITHMS_INLINED

namespace details
{

template <bool IsEnum> struct fill_enum_helper
{
    template <class ForwardType, class T> void operator()(ForwardType *first, ForwardType *last, const T &value)
    {
        nh3api::fill_impl(first, last, value);
    }
};

template <> struct fill_enum_helper<true>
{
    template <class ForwardType, class T> void operator()(ForwardType *first, ForwardType *last, const T &value)
    {
        typedef typename nh3api::tt::underlying_type<ForwardType>::type ForwardUnderlying;
        typedef typename nh3api::tt::underlying_type<T>::type UnderlyingType;
        nh3api::fill_impl(reinterpret_cast<ForwardUnderlying *>(first), reinterpret_cast<ForwardUnderlying *>(last),
                            static_cast<UnderlyingType>(value));
    }
};

} // namespace details

template <bool IsEnum, class ForwardType, class T>
void fill_enum(ForwardType *first, ForwardType *last, const T &value)
{
    nh3api::details::fill_enum_helper<IsEnum>()(first, last, value);
}

template <bool IsPtr, class ForwardIt, class T> void fill_ptr(ForwardIt first, ForwardIt last, const T &value)
{
    if (IsPtr)
    {
        typedef typename nh3api::tt::remove_pointer<ForwardIt>::type ForwardType;
        nh3api::fill_enum<nh3api::tt::is_enum<T>::value && nh3api::tt::is_enum<ForwardType>::value,
                            ForwardType, T>(first, last, value);
    }
    else
    {
        nh3api::fill_impl(first, last, value);
    }
}

#endif

// optimized fill
template <class ForwardIt, class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
void fill(ForwardIt first, ForwardIt last, const T &value)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

// optimized ::std::fill for visual studio 2005..2008
#if NH3API_ALGORITHMS_INLINED
    fill_ptr<tt::is_pointer<ForwardIt>::value>(unfancy(first),
                                               unfancy(last),
                                               value);
#else
    // memset wrapper
    ::std::fill(first, last, value);
#endif
}

#if NH3API_ALGORITHMS_INLINED

namespace details
{

template <bool IsEnum>
struct copy_n_enum_helper
{
    template <class InputType, class Size, class OutputType>
    OutputType *operator()(InputType *first, Size count, OutputType *result)
    {
        return nh3api::copy_n_impl(first, count, result);
    }
};

template <>
struct copy_n_enum_helper<true>
{
    template <class InputType, class Size, class OutputType>
    OutputType *operator()(InputType *first, Size count, OutputType *result)
    {
        typedef typename nh3api::tt::underlying_type<InputType>::type InputUnderlying;
        typedef typename nh3api::tt::underlying_type<OutputType>::type OutputUnderlying;
        return reinterpret_cast<OutputType *>(nh3api::copy_n_impl(reinterpret_cast<const InputUnderlying *>(first), count,
                                              reinterpret_cast<OutputUnderlying *>(result)));
    }
};

}

template <bool IsEnum, class InputType, class Size, class OutputType>
OutputType* copy_n_enum(InputType* first, Size count, OutputType* result)
{
    return nh3api::details::copy_n_enum_helper<IsEnum>()(first, count, result);
}

template <bool IsPtr, class InputIt, class Size, class OutputIt>
OutputIt copy_n_ptr(InputIt first, Size count, OutputIt result)
{
    if ( IsPtr )
    {
        typedef typename nh3api::tt::remove_pointer<InputIt>::type  InputType;
        typedef typename nh3api::tt::remove_pointer<OutputIt>::type OutputType;
        return nh3api::copy_n_enum<nh3api::tt::is_enum<InputType>::value
                                     && nh3api::tt::is_enum<OutputType>::value,
                                     InputType, Size, OutputType>(first, count, result);
    }
    else
    {
        return nh3api::copy_n_impl(first, count, result);
    }
}

#endif

// optimized copy_n
template<class InputIt, class Size, class OutputIt>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif

    // optimized ::std::fill for visual studio 2005..2008
    #if NH3API_ALGORITHMS_INLINED
        return copy_n_ptr<tt::is_pointer<InputIt>::value
                          && tt::is_pointer<OutputIt>::value>(first, count, result);
    #else
        return ::std::copy_n(first, count, result);
    #endif
}

#if NH3API_ALGORITHMS_INLINED

namespace details
{

template <bool IsEnum>
struct copy_enum_helper
{
    template <class InputType, class OutputType>
    OutputType *operator()(InputType *first, InputType *last, OutputType *d_first)
    {
        return nh3api::copy_impl(first, last, d_first);
    }
};

template <>
struct copy_enum_helper<true>
{
    template <class InputType, class OutputType>
    OutputType *operator()(InputType *first, InputType *last, OutputType *d_first)
    {
        typedef typename nh3api::tt::underlying_type<InputType>::type  InputUnderlying;
        typedef typename nh3api::tt::underlying_type<OutputType>::type OutputUnderlying;
        return reinterpret_cast<OutputType *>(nh3api::copy_impl(reinterpret_cast<const InputUnderlying *>(first),
                                              reinterpret_cast<const InputUnderlying *>(last),
                                              reinterpret_cast<OutputUnderlying *>(d_first)));
    }
};

}

template<bool IsEnum, class InputType, class OutputType> static
OutputType* copy_enum(InputType* first, InputType* last, OutputType* d_first)
{
    return nh3api::details::copy_enum_helper<IsEnum>()(first, last, d_first);
}

template<bool IsPtr, class InputIt, class OutputIt> static
OutputIt copy_ptr(InputIt first, InputIt last, OutputIt d_first)
{
    if ( IsPtr )
    {
        typedef typename nh3api::tt::remove_pointer<InputIt>::type  InputType;
        typedef typename nh3api::tt::remove_pointer<OutputIt>::type OutputType;
        return nh3api::copy_enum<nh3api::tt::is_enum<InputType>::value
                                   && nh3api::tt::is_enum<OutputType>::value,
                                   InputType, OutputType>(first, last, d_first);
    }
    else
    {
        return nh3api::copy_impl(first, last, d_first);
    }
}

#endif

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, off)
#endif

template<class InputIt, class OutputIt>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    #if NH3API_ALGORITHMS_INLINED
        return copy_ptr<tt::is_pointer<InputIt>::value
                        && tt::is_pointer<OutputIt>::value>(first, last, d_first);
    #else
        return ::std::copy(first, last, d_first);
    #endif
}

template<class Allocator>
struct allocator_size_type
{
    #if NH3API_CHECK_CPP11
    typedef typename ::std::allocator_traits<Allocator>::size_type type;
    #else
    typedef typename Allocator::size_type type;
    #endif
};

// the following algorithms are here since not all implementations
// rethrow on copy constructor failure, making it really hard to debug...

template<class ForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
void destroy(ForwardIt first,
             ForwardIt last,
             Allocator& alloc)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<typename Allocator::value_type>::value)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    #if NH3API_CHECK_CPP11
    for (; first != last; ++first)
        ::std::allocator_traits<Allocator>::destroy(alloc, addressof(*first));
    #else
    for (; first != last; ++first)
        alloc.destroy(addressof(*first));
    #endif

}

template<class ForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
ForwardIt destroy_n(ForwardIt first,
                    typename Allocator::size_type n,
                    Allocator& alloc)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<typename Allocator::value_type>::value)
{
    #if NH3API_DEBUG
    verify_range_n(first, n);
    #endif

    #if NH3API_CHECK_CPP11
    for (; n > 0; (void) ++first, --n)
        ::std::allocator_traits<Allocator>::destroy(alloc, addressof(*first));
    #else
    for (; n > 0; (void) ++first, --n)
        alloc.destroy(addressof(*first));
    #endif
    return first;
}

template<class ForwardIt, class T>
NH3API_FORCEINLINE
void destroy(ForwardIt first,
             ForwardIt last,
             exe_allocator<T> alloc)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<T>::value)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    (void) alloc;
    for (; first != last; ++first)
        destroy_at(addressof(*first));
}

template<class ForwardIt, class T>
NH3API_FORCEINLINE
ForwardIt destroy_n(ForwardIt first,
                    size_t n,
                    exe_allocator<T> alloc)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<T>::value)
{
    #if NH3API_DEBUG
    verify_range_n(first, n);
    #endif

    (void) alloc;
    for (; n > 0; (void) ++first, --n)
        destroy_at(addressof(*first));
    return first;
}

template<class ForwardIt, class T, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
void uninitialized_fill(ForwardIt first,
                        ForwardIt last,
                        const T& value,
                        Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    #if NH3API_MSVC_STL
    ::std::_Uninitialized_fill_n(first, ::std::distance(first, last), value, alloc);
    #elif NH3API_GCC_STL
    ::std::__uninitialized_fill_a(first, last, value, alloc);
    #elif NH3API_CLANG_STL
    ::std::__uninitialized_allocator_fill_n_multidimensional(alloc, first, std::distance(first, last), value);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}

template<class ForwardIt, class T, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
ForwardIt uninitialized_fill_n(ForwardIt first,
                               typename allocator_size_type<Allocator>::type count,
                               const T& value,
                               Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif

    #if NH3API_MSVC_STL
    return ::std::_Uninitialized_fill_n(first, count, value, alloc);
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_fill_n_a(first, count, value, alloc);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_fill_n_multidimensional(alloc, first, count, value);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}

template<class ForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
ForwardIt uninitialized_default_construct_n(ForwardIt first,
                                            typename allocator_size_type<Allocator>::type count,
                                            Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif

    #if NH3API_MSVC_STL
        #ifdef _MSVC_STL_UPDATE
            return ::std::_Uninitialized_value_construct_n(first, count, alloc);
        #else
            return uninitialized_default_construct_n_impl(first, count, alloc);
        #endif
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_default_n_a(first, count, alloc);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_value_construct_n_multidimensional(alloc, first, count);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}

#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
template<class ForwardIt, class Allocator>
NH3API_FORCEINLINE
ForwardIt uninitialized_default_construct_n_impl(ForwardIt first,
                                                 typename allocator_size_type<Allocator>::type count,
                                                 Allocator& alloc)
{
    typedef typename ::std::iterator_traits<ForwardIt>::value_type V;
    ForwardIt current = first;
    if ( is_nothrow_default_constructible<V>::value )
    {
        for (; count > 0; ++current, (void) --count)
            copy_construct(static_cast<void*>(addressof(*current)), V(), alloc);
    }
    else
    {
        NH3API_TRY
        {
            for (; count > 0; ++current, (void) --count)
                copy_construct(static_cast<void*>(addressof(*current)), V(), alloc);
        }
        NH3API_CATCH(...)
        {
            destroy(first, current, alloc);
            NH3API_RETHROW
        }
    }

    return current;
}
#endif

template<class InputIt, class NoThrowForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
NoThrowForwardIt uninitialized_copy(InputIt first,
                                    InputIt last,
                                    NoThrowForwardIt d_first,
                                    Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    #if NH3API_MSVC_STL
    return ::std::_Uninitialized_copy(first, last, d_first, alloc);
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_copy_a(first, last, d_first, alloc);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_copy(alloc, first, last, d_first);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}

template<class InputIt, class NoThrowForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
NoThrowForwardIt uninitialized_copy_n(InputIt first,
                                      typename allocator_size_type<Allocator>::type count,
                                      NoThrowForwardIt d_first,
                                      Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif

    #if NH3API_MSVC_STL
    return ::std::_Uninitialized_copy(first, ::std::next(first, count), d_first, alloc);
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_copy_a(first, ::std::next(first, count), d_first, alloc);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_copy(alloc, first, ::std::next(first, count), d_first);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}

// MSVC 2008 STL has _Uninitialized_move, although without move semantics support
// weird but ok
// #if NH3API_STD_MOVE_SEMANTICS
template<class InputIt, class NoThrowForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
NoThrowForwardIt uninitialized_move(InputIt first,
                                    InputIt last,
                                    NoThrowForwardIt d_first,
                                    Allocator& alloc)
{
    #if NH3API_DEBUG
    verify_range(first, last);
    #endif

    #if NH3API_MSVC_STL
    return ::std::_Uninitialized_move(first, last, d_first, alloc);
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_move_a(first, last, d_first, alloc);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_relocate(alloc, first, last, d_first);
    // return ::std::__uninitialized_allocator_move_if_noexcept(alloc, first, last, d_first);
    #else
    NH3API_STATIC_ASSERT("Unsupported STL vendor", false);
    return first;
    #endif
}
// #endif

template<class T, class Size>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
T* init_array_impl(T* first, Size n, tt::false_type)
{
    T* current = first;
    NH3API_TRY
    {
        for (; n > 0; (void) ++current, --n)
            ::new (const_cast<void*>(static_cast<const volatile void*>(
                addressof(*current)))) T;
    }
    NH3API_CATCH(...)
    {
        destroy(first, current);
        NH3API_RETHROW
    }
    return current;
}

template<class T, class Size>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
T* init_array_impl(T* first, Size n, tt::true_type)
{
    T* current = first;
    for (; n > 0; (void) ++current, --n)
        ::new (const_cast<void*>(static_cast<const volatile void*>(
            nh3api::addressof(*current)))) T;
    return current;
}

template<class T, class Size> NH3API_CONSTEXPR_CPP_20
T* init_array(T* first, Size n)
{
    #if NH3API_DEBUG
    verify_range_n(first, n);
    #endif
    return init_array_impl(first, n, tt::is_nothrow_default_constructible<T>());
}

template <typename InputIterator, typename OutputIterator>
NH3API_CONSTEXPR OutputIterator
copy_constexpr(InputIterator first, InputIterator last, OutputIterator d_first)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        return std::copy(first, last, d_first);
    #else
        #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
            {
                while (first != last)
                {
                    *d_first++ = *first++;
                }
                return d_first;
            }
            else
            {
                return std::copy(first, last, d_first);
            }
        #else
            while (first != last)
            {
                *d_first++ = *first++;
            }
            return d_first;
        #endif
    #endif
#else
    return copy(first, last, d_first);
#endif
}

template<class InputIt, class Size, class OutputIt>
NH3API_CONSTEXPR OutputIt
copy_n_constexpr(InputIt first, Size count, OutputIt result)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        return std::copy_n(first, count, result);
    #else
        #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
            {
                if (count > 0)
                {
                    *result = *first;
                    ++result;
                    for (Size i = 1; i != count; ++i, ++result)
                        *result = *++first;
                }
                return result;
            }
            else
            {
                return std::copy_n(first, count, result);
            }
        #else
            if (count > 0)
            {
                *result = *first;
                ++result;
                for (Size i = 1; i != count; ++i, ++result)
                    *result = *++first;
            }
            return result;
        #endif
    #endif
#else
    return copy_n(first, count, result);
#endif
}

NH3API_CONSTEXPR void
swap_chars_range_constexpr(char* __restrict first1,
                           char* __restrict last1,
                           char* __restrict first2) NH3API_NOEXCEPT
{
    #ifdef __cpp_lib_constexpr_algorithms
    std::swap_ranges(first1, last1, first2);
    #else
    while ( first1 != last1 )
    {
        char temp = *first1;
        *first1++ = *first2++;
        *(first2 - 1) = temp;
    }
    #endif
}

template <typename ForwardIterator, typename T>
NH3API_CONSTEXPR void
fill_constexpr(ForwardIterator first, ForwardIterator last, const T& value)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        std::fill(first, last, value);
    #else
        #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
            {
                for (; first != last; ++first)
                {
                    *first = value;
                }
            }
            else
            {
                std::fill(first, last, value);
            }
        #else
            for (; first != last; ++first)
            {
                *first = value;
            }
        #endif
    #endif
#else
    fill(first, last, value);
#endif
}

template<class OutputIt, class Size, class T> NH3API_CONSTEXPR_CPP_14
OutputIt fill_n_constexpr(OutputIt first, Size count, const T& value)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        return std::fill_n(first, count, value);
    #else
        #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
            {
                for (Size i = 0; i < count; i++)
                    *first++ = value;
                return first;
            }
            else
            {
                return std::fill_n(first, count, value);
            }
        #else
            for (Size i = 0; i < count; i++)
                *first++ = value;
            return first;
        #endif
    #endif
#else
    return fill_n(first, count, value);
#endif
}

}

NH3API_DISABLE_WARNING_END