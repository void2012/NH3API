
//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/nh3api_std/exe_string.hpp"

struct TCheatCode
{
    char code[200]
    #if NH3API_CHECK_CPP11
    {}
    #endif
    ;
    NH3API_CONSTEXPR_CPP_14
    TCheatCode(const char* _code) NH3API_NOEXCEPT
    {
        const char b[] = "nopqrstuvwxyzabcdefghijklm";

        size_t index = 0; 
        const size_t inputLength = 
        std::min<size_t>(std::size(code) - 1,
        #if NH3API_CHECK_CPP14
        nh3api::strlen_constexpr<char>(_code));
        #else
        nh3api::str_func_chooser<char>::_strlen(_code));
        #endif 

        for (size_t i = 0; i < inputLength; ++i) 
        {
            if (nh3api::isalpha_constexpr(_code[i])) 
            {
                code[index] = b[nh3api::tolower_constexpr(_code[i]) - 'a'];
                index++;
            } 
            else 
            {
                code[index] = _code[i];
                index++;
            }
        }
        
        code[index] = '0';
    }

    NH3API_CONSTEXPR_CPP_14
    int32_t compare(const char* arg) const
    {
    #if NH3API_CHECK_CPP14
        return nh3api::memcmp_constexpr<char>(code, arg, sizeof(code));
    #else
        return nh3api::str_func_chooser<char>::_memcmp(code, arg, sizeof(code));
    #endif

    }
};

// encode cheat code at compile-time:
// static constexpr TCheatCode cheat_openmap("openmap");
// patcherInstance->WriteAddressOf(0x63A4F4, cheat_openmap.code);