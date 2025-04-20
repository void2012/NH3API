
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
        char b[] = "nopqrstuvwxyzabcdefghijklm";

        size_t codeLength = 0;
        int minLength = 0;
        char symbol = ' ';
        char curr = ' ';
        int i = 0;

        while( true )
        {
            #if NH3API_CHECK_CPP14
            codeLength = nh3api::strlen_constexpr<char>(_code);
            #else
            codeLength = nh3api::str_func_chooser<char>::_strlen(_code);
            #endif
            minLength = std::min<size_t>(codeLength, 199);
            if (minLength <= i)
                break;
            curr = _code[i];
            minLength = nh3api::isalpha_constexpr(curr);
            if (minLength == 0)
            {
                symbol = curr;
            }
            else 
            {
                minLength = nh3api::fast_tolower(curr);
                symbol = b[minLength - 97];
            }
            code[i] = symbol;
            ++i;
        }
        code[i] = '\0';
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