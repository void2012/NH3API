//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std.hpp"

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
