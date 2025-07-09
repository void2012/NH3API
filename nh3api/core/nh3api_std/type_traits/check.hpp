//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#if defined(_MSC_VER)
    #include <yvals.h>
    #if (_CPPLIB_VER < 520) // pre-vs2010 MSVC STL
        #define NH3API_TYPE_TRAITS_IMPL (1)
    #else
        #define NH3API_TYPE_TRAITS_IMPL (0)
    #endif
#else 
    #define NH3API_TYPE_TRAITS_IMPL (0)
#endif
