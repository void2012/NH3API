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

#include "nh3api_std.hpp"
#include "type_traits.hpp" // is_nothrow_move_constructible

#include <exception>
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
#include <stdexcept> // std::length_error, std::out_of_range
#endif

// you can specify your own behaviour using template specialization
/*
template<>
[[noreturn]]
NH3API_NOINLINE
void throw_exception<MyException>(const char what[])
{
    throw T(what);
}
*/

namespace nh3api
{
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<typename T>
[[noreturn]]
NH3API_NOINLINE
void throw_exception(const char what[])
{
    throw T(what);
}
#else

template<typename>
[[noreturn]]
NH3API_NOINLINE
void throw_exception(const char[])
{
    RaiseException(0xE06D7363u, EXCEPTION_NONCONTINUABLE, 0, nullptr);
    ::std::abort();
}
#endif

} // namespace nh3api

#ifndef NH3API_THROW
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
        #define NH3API_THROW(EXCEPTION_TYPE, WHAT) ::nh3api::throw_exception<EXCEPTION_TYPE>(WHAT)
    #else
        #define NH3API_THROW(EXCEPTION_TYPE, WHAT) ::nh3api::throw_exception<void>("")
    #endif
#endif // NH3API_THROW

namespace nh3api
{

template<typename RollBack>
struct exception_guard_rollback
{
    exception_guard_rollback() = delete;

    constexpr explicit exception_guard_rollback(RollBack _rollback)
    noexcept(noexcept(std::is_nothrow_move_constructible_v<RollBack>))
        : rollback(::std::move(_rollback)), completed(false)
    {}

    constexpr exception_guard_rollback(exception_guard_rollback&& other)
    noexcept(noexcept(std::is_nothrow_move_constructible_v<RollBack>))
        : rollback(::std::move(other.rollback)), completed(other.completed)
    { other.completed = true; }

    exception_guard_rollback(const exception_guard_rollback&) = delete;
    exception_guard_rollback& operator=(const exception_guard_rollback&) = delete;
    exception_guard_rollback& operator=(const exception_guard_rollback&&) = delete;

    constexpr void complete() noexcept
    { completed = true; }

    ~exception_guard_rollback()
    {
        if ( !completed )
            rollback();
    }

    protected:
    #if NH3API_HAS_CPP_ATTRIBUTE(msvc::no_unique_address)
    [[msvc::no_unique_address]]
    #elif NH3API_HAS_CPP_ATTRIBUTE(no_unique_address)
    [[no_unique_address]]
    #endif
    RollBack rollback;
    bool completed;
};

template<typename RollBack>
struct exception_guard_noop
{
    exception_guard_noop() noexcept = delete;

    constexpr explicit exception_guard_noop(RollBack) noexcept
        : completed(false)
    {}

    constexpr exception_guard_noop(exception_guard_noop&& other) noexcept
        : completed(other.completed)
    { other.completed = true; }

    exception_guard_noop(const exception_guard_noop&) noexcept = delete;
    exception_guard_noop& operator=(const exception_guard_noop&) noexcept = delete;
    exception_guard_noop& operator=(const exception_guard_noop&&) noexcept = delete;

    constexpr void complete() noexcept
    { completed = true; }

    ~exception_guard_noop() noexcept
    { assert(completed); }

    protected:
    bool completed;
};

template<bool IsNoexcept, typename RollBack>
struct exception_guard_factory
{
    using type = exception_guard_noop<RollBack>;

    inline constexpr static type get(RollBack rollback) noexcept
    { return type(rollback); }
};

// partial specialization for when exceptions are enabled
// when they are disabled, do noop in both IsNoexcept:bool cases
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<typename RollBack>
struct exception_guard_factory<false, RollBack>
{
    using type = exception_guard_rollback<RollBack>;

    constexpr NH3API_FORCEINLINE static type get(RollBack rollback)
    noexcept(noexcept(std::is_nothrow_move_constructible_v<RollBack>))
    { return type(::std::move(rollback)); }
};
#endif

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<bool IsNoexcept, typename RollBack>
NH3API_FORCEINLINE constexpr typename exception_guard_factory<IsNoexcept, RollBack>::type make_exception_guard(RollBack rollback)
noexcept(noexcept(std::is_nothrow_move_constructible_v<RollBack>))
{ return exception_guard_factory<IsNoexcept, RollBack>::get(::std::move(rollback)); }

#else
template<bool IsNoexcept, typename RollBack> NH3API_FORCEINLINE
constexpr exception_guard_noop<RollBack> make_exception_guard(RollBack rollback) noexcept
{ return exception_guard_noop<RollBack>(rollback); }
#endif // NH3API_FLAG_NO_CPP_EXCEPTIONS

}
