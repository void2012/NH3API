//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <memory>
#include <algorithm> // std::clamp
#include <cmath>
#ifdef __cpp_lib_bit_cast
#include <bit>
#endif
#include <type_traits>
#include "nh3api_std.hpp"

namespace nh3api
{

// the reason we're writing our own addressof is that pre-C++17 version wasn't constexpr

template<typename T>
NH3API_MSVC_INTRIN constexpr NH3API_FORCEINLINE
T* addressof(T& arg) noexcept
{ return __builtin_addressof(arg); }

#ifdef __cpp_lib_bit_cast

using ::std::bit_cast;

#else

template<class To, class From>
NH3API_FORCEINLINE
#if __has_builtin(__builtin_bit_cast)
constexpr
#endif
To bit_cast(const From& from) noexcept
{
    NH3API_STATIC_ASSERT("nh3api::bit_cast: types must have the same size and trivially copyable.",
    (sizeof(To) == sizeof(From)) && std::is_trivially_copyable_v<To> && std::is_trivially_copyable_v<From>);

    #if __has_builtin(__builtin_bit_cast)
    return __builtin_bit_cast(To, from);
    #else
    To result;
    ::memcpy(addressof(result), addressof(from), sizeof(To));
    return result;
    #endif
}

#endif // __cpp_lib_bit_cast

using ::std::destroy_at;
using ::std::destroy;
using ::std::destroy_n;

using ::std::exchange;


// there is no __cpp_lib_* feature testing macro for std::construct_at
// use __cpp_lib_constexpr_dynamic_alloc because it's closely of related
#ifdef __cpp_lib_constexpr_dynamic_alloc

using ::std::construct_at;

#else // C++20

template <class T, class... Args, class = decltype(::new(std::declval<void*>()) T(std::declval<Args>()...))> NH3API_FORCEINLINE
NH3API_CONSTEXPR_CPP_20
T* construct_at(T* ptr, Args&&... args)
noexcept(noexcept(std::is_nothrow_constructible_v<T, Args...>))
{ return ::new (static_cast<void*>(ptr)) T(::std::forward<Args>(args)...); }

#endif // C++20

using ::std::clamp;

using byte_t = ::std::byte;
using ::std::to_integer;

using ::std::next;
using ::std::prev;

// floating-point math

using ::std::fpclassify;
using ::std::isfinite;
using ::std::isinf;
using ::std::isnan;
using ::std::isnormal;
using ::std::trunc;
using ::std::signbit;

template <size_t N, typename T>
NH3API_FORCEINLINE
#if __has_builtin(__builtin_assume_aligned)
constexpr
#endif
T* assume_aligned(T* ptr) noexcept
{
#if __has_builtin(__builtin_assume_aligned) || NH3API_CHECK_CLANG_CL
#if __has_builtin(__builtin_is_constant_evaluated)
    NH3API_IF_CONSTEVAL
    {
        return ptr;
    }
    else
    {
        return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
    }
#else
    return reinterpret_cast<T*>(__builtin_assume_aligned(ptr, N));
#endif
#else // Assume pre-C++20 MSVC
    if ((reinterpret_cast<uintptr_t>(ptr) & ((1 << N) - 1)) == 0)
        return ptr;
    else
        __assume(0);
#endif
}

} // namespace nh3api

using byte_t = std::byte;
