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

// for *whatever* reason old MSVC ignores exe_allocator from memory.hpp
#if NH3API_CHECK_MSVC && !NH3API_VS2010
template<class T>
class exe_allocator;
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
    { __stosd(reinterpret_cast<unsigned long*>(dst), static_cast<unsigned long>(value), (count)); }
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
             const Allocator& allocator)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<typename Allocator::value_type>::value)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

#if NH3API_CHECK_CPP11
    for (; first != last; ++first)
        ::std::allocator_traits<Allocator>::destroy(allocator, ::nh3api::addressof(*first));
#else
    for (; first != last; ++first)
        allocator.destroy(::nh3api::addressof(*first));
#endif

}

template<class ForwardIt, class T>
NH3API_FORCEINLINE
void destroy(ForwardIt first,
             ForwardIt last,
             const ::exe_allocator<T>&)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<T>::value)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

    for (; first != last; ++first)
        ::nh3api::destroy_at(::nh3api::addressof(*first));
}

template<class ForwardIt, class T, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
void uninitialized_fill(ForwardIt first,
                        ForwardIt last,
                        const T& value,
                        Allocator& allocator)
{
#if NH3API_DEBUG
    verify_range(first, last);
    #endif

#if NH3API_MSVC_STL
    ::std::_Uninitialized_fill_n(first, ::std::distance(first, last), value, allocator);
#elif NH3API_GCC_STL
    ::std::__uninitialized_fill_a(first, last, value, allocator);
#elif NH3API_CLANG_STL
    ::std::__uninitialized_allocator_fill_n_multidimensional(allocator, first, std::distance(first, last), value);
#else
    typedef typename std::iterator_traits<ForwardIt>::value_type V;
    ForwardIt current = first;
    if ( tt::is_nothrow_copy_constructible<V>::value )
    {
        for (; current != last; ++current)
            memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), value);
    }
    else 
    {
        NH3API_TRY
        {
            for (; current != last; ++current)
                memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), value);
        } 
        NH3API_CATCH(...)
        {
            destroy(first, current, allocator);
            NH3API_RETHROW
        }
    }
#endif
}

template<class ForwardIt, class T, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
ForwardIt uninitialized_fill_n(ForwardIt first,
                               typename allocator_size_type<Allocator>::type count,
                               const T& value,
                               Allocator& allocator)
{
#if NH3API_DEBUG
    verify_range_n(first, count);
#endif

#if NH3API_MSVC_STL
    return ::std::_Uninitialized_fill_n(first, count, value, allocator);
#elif NH3API_GCC_STL
    return ::std::__uninitialized_fill_n_a(first, count, value, allocator);
#elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_fill_n_multidimensional(allocator, first, count, value);
#else
    typedef typename std::iterator_traits<NoThrowForwardIt>::value_type V;
    NoThrowForwardIt current = first;
    if ( tt::is_nothrow_copy_constructible<V>::value )
    {
        for (; count > 0; ++current, (void) --count)
            memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), value);
        return current;
    }
    else 
    {
        NH3API_TRY
        {
            for (; count > 0; ++current, (void) --count)
                memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), value);
            return current;
        }
        NH3API_CATCH
        {
            destroy(first, current, allocator);
            NH3API_RETHROW
        }
    }
    
    return current;
#endif
}

template<class ForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
ForwardIt uninitialized_value_construct_n(ForwardIt first,
                                            typename allocator_size_type<Allocator>::type count,
                                            Allocator& allocator)
{
#if NH3API_DEBUG
    verify_range_n(first, count);
#endif

#if defined(_MSVC_STL_UPDATE)
    return ::std::_Uninitialized_value_construct_n(first, count, allocator);
#elif NH3API_GCC_STL
    return ::std::__uninitialized_default_n_a(first, count, allocator);
#elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_value_construct_n_multidimensional(allocator, first, count);
#else
    typedef typename ::std::iterator_traits<ForwardIt>::value_type V;
    ForwardIt current = first;
    if ( tt::is_nothrow_constructible<V>::value )
    {
        for (; count > 0; ++current, (void) --count)
            default_construct(static_cast<void*>(::nh3api::addressof(*current)), allocator);
    }
    else
    {
        NH3API_TRY
        {
            for (; count > 0; ++current, (void) --count)
                default_construct(static_cast<void*>(::nh3api::addressof(*current)), allocator);
        }
        NH3API_CATCH(...)
        {
            destroy(first, current, allocator);
            NH3API_RETHROW
        }
    }

    return current;
#endif
}

template<class InputIt, class NoThrowForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
NoThrowForwardIt uninitialized_copy(InputIt first,
                                    InputIt last,
                                    NoThrowForwardIt d_first,
                                    Allocator& allocator)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

#if NH3API_MSVC_STL
    return ::std::_Uninitialized_copy(first, last, d_first, allocator);
#elif NH3API_GCC_STL
    return ::std::__uninitialized_copy_a(first, last, d_first, allocator);
#elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_copy(allocator, first, last, d_first);
#else
    typedef typename std::iterator_traits<NoThrowForwardIt>::value_type T;
    NoThrowForwardIt current = d_first;
    if ( tt::is_nothrow_copy_constructible<T>::value )
    {
        for (; first != last; ++first, (void) ++current)
            memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), T(*first));
        return current;
    }
    else 
    {
        NH3API_TRY
        {
            for (; first != last; ++first, (void) ++current)
                memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), T(*first));
        }
        NH3API_CATCH(...)
        {
            destroy(d_first, current, allocator);
            NH3API_RETHROW
        }
        return current;
    }
#endif
}

template<class InputIt, class NoThrowForwardIt, class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
NoThrowForwardIt uninitialized_copy_n(InputIt first,
                                      typename allocator_size_type<Allocator>::type count,
                                      NoThrowForwardIt d_first,
                                      Allocator& allocator)
{
    #if NH3API_DEBUG
    verify_range_n(first, count);
    #endif

    #if NH3API_MSVC_STL
    return ::std::_Uninitialized_copy(first, ::std::next(first, count), d_first, allocator);
    #elif NH3API_GCC_STL
    return ::std::__uninitialized_copy_a(first, ::std::next(first, count), d_first, allocator);
    #elif NH3API_CLANG_STL
    return ::std::__uninitialized_allocator_copy(allocator, first, ::std::next(first, count), d_first);
    #else
    typedef typename std::iterator_traits<NoThrowForwardIt>::value_type T;
    NoThrowForwardIt current = d_first;
    if ( tt::is_nothrow_copy_constructible<T>::value )
    {
        for (; count > 0; ++first, (void) ++current, --count)
            memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), T(*first));
        return current;
    }
    else 
    {
        NH3API_TRY
        {
            for (; count > 0; ++first, (void) ++current, --count)
                memory::copy_construct(allocator, static_cast<void*>(::nh3api::addressof(*current)), T(*first));
        }
        NH3API_CATCH (...)
        {
            destroy(d_first, current, allocator);
            NH3API_RETHROW
        }
        return current;
    }
    #endif
}

#if NH3API_STD_MOVE_SEMANTICS
template<class InputIt, class NoThrowForwardIt, class Allocator> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move_impl(InputIt first,
                                         InputIt last,
                                         NoThrowForwardIt d_first,
                                         Allocator& allocator,
                                         tt::false_type)
{
    NoThrowForwardIt current = d_first;
    NH3API_TRY
    {
        for (; first != last; ++first, (void) ++current)
        {
            void* addr = static_cast<void*>(::nh3api::addressof(*current));
            NH3API_IF_CONSTEXPR (tt::is_lvalue_reference<decltype(*first)>::value)
                memory::move_construct(allocator, addr, ::std::move(*first));
            else
                memory::copy_construct(allocator, addr, *first);
        }
        return current;
    }
    NH3API_CATCH(...)
    {
        destroy(d_first, current, allocator);
        NH3API_RETHROW
    }
}

template<class InputIt, class NoThrowForwardIt, class Allocator> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move_impl(InputIt first,
                                         InputIt last,
                                         NoThrowForwardIt d_first,
                                         Allocator& allocator,
                                         tt::true_type) NH3API_NOEXCEPT
{
    NoThrowForwardIt current = d_first;
    for (; first != last; ++first, (void) ++current)
    {
        void* addr = static_cast<void*>(::nh3api::addressof(*current));
        NH3API_IF_CONSTEXPR (tt::is_lvalue_reference<decltype(*first)>::value)
            move_construct(addr, ::std::move(*first), allocator);
        else
            copy_construct(addr, *first, allocator);
    }
    return current;
}

template<class InputIt, class NoThrowForwardIt, class Allocator> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move(InputIt first,
                                    InputIt last,
                                    NoThrowForwardIt d_first,
                                    Allocator& allocator)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

    typedef typename ::std::iterator_traits<NoThrowForwardIt>::value_type V;

    NH3API_IF_CONSTEXPR 
    ( tt::is_trivially_move_assignable<V>::value
      && tt::is_trivially_move_constructible<V>::value )
    {
        return ::std::move(first, last, d_first);
    }
    else if ( tt::is_trivially_copy_constructible<V>::value
              && tt::is_trivially_copy_assignable<V>::value )
    {
        return ::std::copy(first, last, d_first);
    }
    else
    {
        return uninitialized_move_impl(first, last, d_first, allocator,
            std::integral_constant<bool,
            (tt::is_nothrow_move_constructible<V>::value &&
                tt::is_nothrow_copy_constructible<V>::value)
                || flags::no_exceptions>());
    }
}

template<class InputIt, class NoThrowForwardIt> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move_impl(InputIt first,
                                         InputIt last,
                                         NoThrowForwardIt d_first,
                                         tt::false_type)
{
    typedef typename ::std::iterator_traits<NoThrowForwardIt>::value_type V;
    NoThrowForwardIt current = d_first;
    NH3API_TRY
    {
        for (; first != last; ++first, (void) ++current)
        {
            void* addr = static_cast<void*>(::nh3api::addressof(*current));
            NH3API_IF_CONSTEXPR (tt::is_lvalue_reference<decltype(*first)>::value)
                ::new (addr) V(::std::move(*first));
            else
                ::new (addr) V(*first);
        }
        return current;
    }
    NH3API_CATCH (...)
    {
        ::nh3api::destroy(d_first, current);
        NH3API_RETHROW
    }
}

template<class InputIt, class NoThrowForwardIt> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move_impl(InputIt first,
                                         InputIt last,
                                         NoThrowForwardIt d_first,
                                         tt::true_type) NH3API_NOEXCEPT
{
    typedef typename ::std::iterator_traits<NoThrowForwardIt>::value_type V;
    NoThrowForwardIt current = d_first;
    for (; first != last; ++first, (void) ++current)
    {
        void* addr = static_cast<void*>(::nh3api::addressof(*current));
        NH3API_IF_CONSTEXPR (tt::is_lvalue_reference<decltype(*first)>::value)
            ::new (addr) V(::std::move(*first));
        else
            ::new (addr) V(*first);
    }
    return current;
}

template<class InputIt, class NoThrowForwardIt> NH3API_FORCEINLINE
NoThrowForwardIt uninitialized_move(InputIt first,
                                    InputIt last,
                                    NoThrowForwardIt d_first)
{
#if NH3API_DEBUG
    verify_range(first, last);
#endif

    typedef typename ::std::iterator_traits<NoThrowForwardIt>::value_type V;

    NH3API_IF_CONSTEXPR
    ( tt::is_trivially_move_assignable<V>::value
      && tt::is_trivially_move_constructible<V>::value )
    {
        return ::std::move(first, last, d_first);
    }
    else if ( tt::is_trivially_copy_constructible<V>::value
               && tt::is_trivially_copy_assignable<V>::value)
    {
        return ::std::copy(first, last, d_first);
    }
    else
    {
        return uninitialized_move_impl(first, last, d_first,
            tt::integral_constant<bool,
            (tt::is_nothrow_move_constructible<V>::value &&
                tt::is_nothrow_copy_constructible<V>::value)
                || flags::no_exceptions>());
    }
}

#endif

#ifndef __cpp_lib_raw_memory_algorithms
template<class T, class Size>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
T* init_array_impl(T* first, Size n, tt::false_type)
{
    T* current = first;
    NH3API_TRY
    {
        for (; n > 0; (void) ++current, --n)
            ::new (const_cast<void*>(static_cast<const volatile void*>(
                ::nh3api::addressof(*current)))) T();
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
            ::nh3api::addressof(*current)))) T();
    return current;
}
#endif // __cpp_lib_raw_memory_algorithms

template<class T, class Size> NH3API_CONSTEXPR_CPP_20
T* init_array(T* first, Size n)
{
    #if NH3API_DEBUG
    verify_range_n(first, n);
    #endif
    #ifdef __cpp_lib_raw_memory_algorithms
    return std::uninitialized_value_construct_n(first, n);
    #else 
    return init_array_impl(first, n, tt::is_nothrow_constructible<T>());
    #endif
}

template <typename InputIterator, typename OutputIterator>
NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED OutputIterator
copy_constexpr(InputIterator first, InputIterator last, OutputIterator d_first)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        return ::std::copy(first, last, d_first);
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
                return ::std::copy(first, last, d_first);
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
        return ::std::copy_n(first, count, result);
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
                return ::std::copy_n(first, count, result);
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

template <typename ForwardIterator, typename T>
NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED void
fill_constexpr(ForwardIterator first, ForwardIterator last, const T& value)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        ::std::fill(first, last, value);
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
                ::std::fill(first, last, value);
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

template<class OutputIt, class Size, class T> NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
OutputIt fill_n_constexpr(OutputIt first, Size count, const T& value)
{
#if NH3API_CHECK_CPP14
    #ifdef __cpp_lib_constexpr_algorithms
        return ::std::fill_n(first, count, value);
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
                return ::std::fill_n(first, count, value);
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

template <class ForwardIterator1, 
          class ForwardIterator2, 
          class BinaryPredicate>
NH3API_CONSTEXPR_CPP_14 ForwardIterator1 
find_first_of_constexpr(ForwardIterator1 first1,
                        ForwardIterator1 last1,
                        ForwardIterator2 first2,
                        ForwardIterator2 last2,
                        #if NH3API_STD_MOVE_SEMANTICS
                        BinaryPredicate&& predicate
                        #else 
                        BinaryPredicate predicate
                        #endif
                        ) 
{
    for (; first1 != last1; ++first1)
        for (ForwardIterator2 i = first2; i != last2; ++i)
            if (predicate(*first1, *i))
                return first1;
    return last1;
}



}

#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
NH3API_DISABLE_MSVC_WARNING_END
#endif

NH3API_DISABLE_WARNING_END