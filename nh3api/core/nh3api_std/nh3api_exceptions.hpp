//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
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
NH3API_NORETURN
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
NH3API_NORETURN
NH3API_NOINLINE
void throw_exception(const char what[])
{
    throw T(what);
}
#else 

template<typename>
NH3API_NORETURN
NH3API_NOINLINE
void throw_exception(const char[])
{
    RaiseException(0xE06D7363u, EXCEPTION_NONCONTINUABLE, 0, nullptr);
    ::std::terminate();
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
    exception_guard_rollback() NH3API_DELETED_FUNCTION;
   
    NH3API_CONSTEXPR
    explicit exception_guard_rollback(RollBack _rollback)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
    #if NH3API_STD_MOVE_SEMANTICS
        : rollback(::std::move(_rollback)), completed(false) {}
    #else 
        : rollback(_rollback), completed(false) {}
    #endif
    
    #if NH3API_STD_MOVE_SEMANTICS
    NH3API_CONSTEXPR
    exception_guard_rollback(exception_guard_rollback&& other)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
        : rollback(::std::move(other.rollback)), completed(other.completed)
    { other.completed = true; }
    #endif

    exception_guard_rollback(const exception_guard_rollback&) NH3API_DELETED_FUNCTION;
    exception_guard_rollback& operator=(const exception_guard_rollback&) NH3API_DELETED_FUNCTION;
    #if NH3API_STD_MOVE_SEMANTICS
    exception_guard_rollback& operator=(const exception_guard_rollback&&) NH3API_DELETED_FUNCTION;
    #endif 

    NH3API_CONSTEXPR 
    void complete() NH3API_NOEXCEPT 
    { completed = true; }

    ~exception_guard_rollback()
    {
        if ( !completed )
            rollback();
    }

    protected:
    RollBack rollback;
    bool completed;
};

template<typename RollBack>
struct exception_guard_noop
{
    exception_guard_noop() NH3API_DELETED_FUNCTION;
   
    NH3API_CONSTEXPR
    explicit exception_guard_noop(RollBack _rollback)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
        : completed(false)
    {}
    
    #if NH3API_STD_MOVE_SEMANTICS
    NH3API_CONSTEXPR
    exception_guard_noop(exception_guard_noop&& other)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
        : completed(other.completed)
    { other.completed = true; }
    #endif

    exception_guard_noop(const exception_guard_noop&) NH3API_DELETED_FUNCTION;
    exception_guard_noop& operator=(const exception_guard_noop&) NH3API_DELETED_FUNCTION;
    #if NH3API_STD_MOVE_SEMANTICS
    exception_guard_noop& operator=(const exception_guard_noop&&) NH3API_DELETED_FUNCTION;
    #endif 

    NH3API_CONSTEXPR 
    void complete() NH3API_NOEXCEPT 
    { completed = true; }

    ~exception_guard_noop()
    { assert(completed); }

    protected:
    bool completed;
};

template<bool IsNoexcept, typename RollBack>
struct exception_guard_factory 
{
    typedef exception_guard_noop<RollBack> type;

    NH3API_CONSTEXPR NH3API_FORCEINLINE
    static type get(RollBack rollback) NH3API_NOEXCEPT
    { return type(rollback); }
};

// partial specialization for when exceptions are enabled
// when they are disabled, do noop in both IsNoexcept:bool cases
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<typename RollBack>
struct exception_guard_factory<false, RollBack>
{
    typedef exception_guard_rollback<RollBack> type;

    NH3API_CONSTEXPR NH3API_FORCEINLINE
    static type get(RollBack rollback) 
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
    #if NH3API_STD_MOVE_SEMANTICS
    { return type(::std::move(rollback)); } 
    #else 
    { return type(rollback); }
    #endif
};
#endif

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<bool IsNoexcept, typename RollBack> NH3API_FORCEINLINE
NH3API_CONSTEXPR typename exception_guard_factory<IsNoexcept, RollBack>::type make_exception_guard(RollBack rollback)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<RollBack>::value)
#if NH3API_STD_MOVE_SEMANTICS
{ return exception_guard_factory<IsNoexcept, RollBack>::get(::std::move(rollback)); } 
#else 
{ return exception_guard_factory<IsNoexcept, RollBack>::get(rollback); } 
#endif // NH3API_STD_MOVE_SEMANTICS

#else
template<bool IsNoexcept, typename RollBack> NH3API_FORCEINLINE
NH3API_CONSTEXPR exception_guard_noop<RollBack> make_exception_guard(RollBack rollback) NH3API_NOEXCEPT
{ return exception_guard_noop<RollBack>(rollback); }
#endif // NH3API_FLAG_NO_CPP_EXCEPTIONS

}