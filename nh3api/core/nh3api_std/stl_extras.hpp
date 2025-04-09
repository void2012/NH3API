//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <algorithm> // std::clamp
#include <cmath>

#ifdef __has_include
    #if __has_include(<bit>)
        #include <bit> // std::bit_cast
    #endif
#endif

#include "type_traits.hpp"

namespace nh3api 
{

// the reason we're writing our own addressof is that pre-C++17 version wasn't constexpr

#if !NH3API_HAS_BUILTIN_ADDRESSOF
template<typename T> NH3API_FORCEINLINE
T* addressof(T& arg) NH3API_NOEXCEPT
{ return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg))); }

#else
template<typename T>
NH3API_MSVC_INTRIN NH3API_CONSTEXPR NH3API_FORCEINLINE
T* addressof(T& arg) NH3API_NOEXCEPT
{ return __builtin_addressof(arg); }

#endif // !NH3API_HAS_BUILTIN_ADDRESSOF

#ifdef __cpp_lib_bit_cast
template<class To, class From>
NH3API_MSVC_INTRIN NH3API_FORCEINLINE 
constexpr To bit_cast( const From& from ) noexcept
{
    return std::bit_cast<To>(from);
}

#else
template<class To, class From>
NH3API_FORCEINLINE
#if NH3API_HAS_BUILTIN_BIT_CAST
NH3API_CONSTEXPR
#endif
To bit_cast( const From& from ) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("nh3api::bit_cast: types must have the same size and trivially copyable.",
    (sizeof(To) == sizeof(From)) && tt::is_trivially_copyable<To>::value && tt::is_trivially_copyable<From>::value);

    #if NH3API_HAS_BUILTIN_BIT_CAST
    return __builtin_bit_cast(To, from);
    #else
    To result;
    ::memcpy(addressof(result), addressof(from), sizeof(To));
    return result;
    #endif
}
#endif // __cpp_lib_bit_cast

#if !defined(__cpp_lib_raw_memory_algorithms) && !defined(__cpp_lib_constexpr_algorithms)

template<class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR
void destroy_at(T* p)
{
    p->~T();
}

template<class ForwardIt>
NH3API_FORCEINLINE NH3API_CONSTEXPR
void destroy(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first)
        destroy_at(nh3api::addressof(*first));
}

template<class ForwardIt, class Size>
NH3API_FORCEINLINE NH3API_CONSTEXPR
ForwardIt destroy_n(ForwardIt first, Size n)
{
    for (; n > 0; (void) ++first, --n)
        destroy_at(nh3api::addressof(*first));
    return first;
}

#else // constexpr destruction algorithms

template<class T>
NH3API_FORCEINLINE constexpr
void destroy_at(T* p)
{ ::std::destroy_at(p); }

template<class ForwardIt>
NH3API_FORCEINLINE constexpr
void destroy(ForwardIt first, ForwardIt last)
{ ::std::destroy(first, last); }

template<class ForwardIt, class Size>
NH3API_FORCEINLINE constexpr
ForwardIt destroy_n(ForwardIt first, Size n)
{ return ::std::destroy_n(first, n); }

#endif // constexpr destruction algorithms

#if NH3API_STD_MOVE_SEMANTICS
#if NH3API_MSVC_STL_VERSION < NH3API_MSVC_STL_VERSION_2015_2022 && !defined(__cpp_lib_exchange_function)
template<class T, class U> NH3API_FORCEINLINE
T exchange(T& obj, U&& new_value)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<T>::value &&
                    tt::is_nothrow_assignable<T&, U>::value)
{
    T old_value = ::std::move(obj);
    obj         = ::std::forward<U>(new_value);
    return old_value;
}

#else
template<class T, class U> NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
T exchange(T& obj, U&& new_value)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<T>::value &&
                        tt::is_nothrow_assignable<T&, U>::value)
{ return ::std::exchange(obj, ::std::forward<U>(new_value)); }
#endif
#endif

#ifndef __cpp_lib_clamp
template<class T = void>
struct less_functor
{	
    NH3API_CONSTEXPR bool operator()(const T& left, const T& right) const 
    { return left < right; }
};

template<class T, class Compare> NH3API_CONSTEXPR
const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{ return comp(v, lo) ? lo : comp(hi, v) ? hi : v; }

template<class T> NH3API_CONSTEXPR
const T& clamp(const T& v, const T& lo, const T& hi)
{ return clamp(v, lo, hi, nh3api::less_functor<T>{}); }
#else 
using ::std::clamp;
#endif

#if !defined(__cpp_lib_byte)

enum
#if NH3API_CHECK_CPP11
class
#endif
byte_t : uint8_t {};

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator|(byte_t lhs, byte_t rhs) NH3API_NOEXCEPT
{ return static_cast<byte_t>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t& operator|=(byte_t& lhs, byte_t rhs) NH3API_NOEXCEPT
{ return lhs = lhs | rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator&(byte_t lhs, byte_t rhs) NH3API_NOEXCEPT
{ return static_cast<byte_t>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t& operator&=(byte_t& lhs, byte_t rhs) NH3API_NOEXCEPT
{ return lhs = lhs & rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator^(byte_t lhs, byte_t rhs) NH3API_NOEXCEPT
{ return static_cast<byte_t>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t& operator^=(byte_t& lhs, byte_t rhs) NH3API_NOEXCEPT
{ return lhs = lhs ^ rhs; }

NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator~(byte_t arg) NH3API_NOEXCEPT
{ return static_cast<byte_t>(static_cast<uint8_t>(~static_cast<uint32_t>(arg))); }

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t& operator<<=(byte_t& lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return lhs = lhs << shift;
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator<<(byte_t lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return static_cast<byte_t>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) << shift));
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t& operator>>=(byte_t& lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return lhs = lhs >> shift;
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE byte_t operator>>(byte_t lhs, _Integer shift) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return static_cast<byte_t>(static_cast<uint8_t>(static_cast<uint32_t>(lhs) >> shift));
}

template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE _Integer to_integer(byte_t value) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("operand must be an integral type", nh3api::tt::is_integral<_Integer>::value);
    return static_cast<_Integer>(value);
}

#else
using byte_t = std::byte;
template <class _Integer>
NH3API_CONSTEXPR NH3API_FORCEINLINE _Integer to_integer(byte_t value) NH3API_NOEXCEPT
{ return ::std::to_integer<_Integer>(value); }
#endif

#if NH3API_MSVC_STL_VERSION >= NH3API_MSVC_STL_VERSION_2010 || NH3API_STD_MOVE_SEMANTICS
using ::std::next;
using ::std::prev;
#else
template<class InputIt>
InputIt next(InputIt it, typename ::std::iterator_traits<InputIt>::difference_type n = 1)
{
    ::std::advance(it, n);
    return it;
}

template<class BidirIt>
BidirIt prev(BidirIt it, typename ::std::iterator_traits<BidirIt>::difference_type n = 1)
{
    ::std::advance(it, -n);
    return it;
}
#endif

// floating-point math

// older versions of MSVC STL
#if NH3API_CHECK_CPP11 || NH3API_MSVC_STL_VERSION >= NH3API_MSVC_STL_VERSION_2010
using ::std::fpclassify;
using ::std::isfinite;
using ::std::isinf;
using ::std::isnan;
using ::std::isnormal;
using ::std::trunc;
using ::std::signbit;
#else 

#define INFINITY ((float)(1e+300 * 1e+300))
#define NAN (-(float)(NH3API_INFINITY * 0.0F))
#define FP_INFINITE  1
#define FP_NAN       2
#define FP_NORMAL    (-1)
#define FP_SUBNORMAL (-2)
#define FP_ZERO      0

namespace details 
{
union fval_t
{ 
    uint16_t _Sh[8];
    float _Val;
};

union dval_t
{ 
    uint16_t _Sh[8];
    double _Val;
};

inline short fdtest(const float* const px)
{
    const fval_t* ps = reinterpret_cast<const fval_t*>(px);
    if ((ps->_Sh[1] & 32640u) == 255u << 7)
        return (ps->_Sh[1] & 127u) != 0 || ps->_Sh[0] != 0 ? FP_NAN : FP_INFINITE;
    else if ((ps->_Sh[1] & ~0x8000u) != 0 || ps->_Sh[0] != 0)
        return (ps->_Sh[1] & 32640u) == 0 ? FP_SUBNORMAL : FP_NORMAL;
    else
        return 0;
}

inline short ddtest(const double* const px)
{
    const dval_t* ps = reinterpret_cast<const dval_t*>(px);
    if ((ps->_Sh[3] & 32752u) == 2047u << 4)
        return (ps->_Sh[3] & 15u) != 0 || ps->_Sh[2] != 0 || ps->_Sh[1] != 0 || ps->_Sh[0] != 0 ? FP_NAN : FP_INFINITE;
    else if ((ps->_Sh[3] & ~0x8000) != 0 || ps->_Sh[2] != 0 || ps->_Sh[1] != 0 || ps->_Sh[0] != 0)
        return (ps->_Sh[3] & 32752u) == 0 ? FP_SUBNORMAL : FP_NORMAL;
    else
        return 0;
}    
}

inline int fpclassify(float num) NH3API_NOEXCEPT
{ return details::fdtest(&num); }

inline int fpclassify(double num) NH3API_NOEXCEPT
{ return details::ddtest(&num); }

inline bool isfinite(float num) NH3API_NOEXCEPT
{ return details::fdtest(&num) <= 0; }

inline bool isinf(float num) NH3API_NOEXCEPT
{ return details::fdtest(&num) == 1; }

inline bool isnan(float num) NH3API_NOEXCEPT
{ return details::fdtest(&num) == 2; }

inline bool isnormal(float num) NH3API_NOEXCEPT
{ return details::fdtest(&num) == (-1); }

inline bool isfinite(double num) NH3API_NOEXCEPT
{ return details::ddtest(&num) <= 0; }

inline bool isinf(double num) NH3API_NOEXCEPT
{ return details::ddtest(&num) == 1; }

inline bool isnan(double num) NH3API_NOEXCEPT
{ return details::ddtest(&num) == 2; }

inline bool isnormal(double num) NH3API_NOEXCEPT
{ return details::ddtest(&num) == (-1); }

inline double trunc(double num)
{ return (num > 0) ? floor(num) : ceil(num); }

inline float trunc(float num)
{ return (num > 0) ? floorf(num) : ceilf(num); }

inline bool signbit(double num) NH3API_NOEXCEPT
{ return !!(*reinterpret_cast<const uint64_t*>(&num) & (1ull << 63ull)); }

inline bool signbit(float num) NH3API_NOEXCEPT
{ return !!(*reinterpret_cast<const uint32_t*>(&num) & (1ul << 31ul)); }

#endif

template <size_t N, typename T>
NH3API_FORCEINLINE
#if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
constexpr
#endif
T* assume_aligned(T* ptr) NH3API_NOEXCEPT
{
#if NH3API_HAS_BUILTINS
#if __has_builtin(__builtin_assume_aligned) || NH3API_CHECK_CLANG_CL
#if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
    if (__builtin_is_constant_evaluated())
        return ptr;
    else
        return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
#else 
    return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
#endif
#else // Assume pre-C++20 MSVC
    if ((reinterpret_cast<uintptr_t>(ptr) & ((1 << N) - 1)) == 0)
        return ptr;
    else
        __assume(0);
#endif
#else // Assume old MSVC
    if ((reinterpret_cast<uintptr_t>(ptr) & ((1 << N) - 1)) == 0)
        return ptr;
    else
        __assume(0);
#endif
}

} // namespace nh3api

using nh3api::byte_t;