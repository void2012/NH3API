//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once
/*
#include "nh3api_std.hpp"
#if !NH3API_CHECK_MSVC
#include <cxxabi.h>
#endif

namespace nh3api
{
#if NH3API_CHECK_MSVC
const char* get_type_name(const std::type_info& info)
{
    return info.name();
}
#else 
struct get_type_name_t 
{
    const char* operator()(const std::type_info& info)
    {
        size_t buffer_size = 512;
        buffer = reinterpret_cast<char*>(std::malloc(buffer_size));
        int     status;
        return abi::__cxa_demangle(info.name(), buffer, &buffer_size, &status);
    }

    ~get_type_name_t()
    {
        std::free(buffer);
    }
    char* buffer;
} get_type_name;
#endif

}

*/