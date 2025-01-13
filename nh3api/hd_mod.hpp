//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// HD Mod API
// source: https://forum.df2.ru/index.php?showtopic=10421&st=2480&p=593378&#entry593378
#pragma once

#include "core/nh3api_std/patcher_x86.hpp"
#include "core/nh3api_std/exe_vector.hpp"
#include "core/nh3api_std/exe_string.hpp"

// Game version of HD Mod /
// Версия игры с точки зрения HD Mod.
enum HD_game_version
{
    HD_NONE = 0x00,
    HD_SOD  = 0x01,
    HD_WOG  = 0x02,
    HD_TE   = 0x04,
    HD_HOTA = 0x08,
    HD_WT   = 0x10,
    HD_ERA  = 0x20,
    HD_MOP  = 0x40,
};

NH3API_FORCEINLINE
HANDLE getHDModHandle()
{ return GetModuleHandleA("_hd3_.dll"); }

NH3API_FORCEINLINE
bool isHDModPresent(Patcher* patcher)
{
    if ( patcher )
        return static_cast<bool>(getHDModHandle()) && static_cast<bool>(patcher->GetInstance("HD"));
    else
        return false;
}

// moved to struct and marked noinline to reduce the code bloat
struct get_hd_mod_varable_impl
{
    template<typename T, T DefaultValue>
    NH3API_NOINLINE
    static T get(Patcher* patcher, const char* name)
    {
        if ( patcher )
        {
            Variable* var = patcher->VarFind("HD.Version.Dword");
            if ( var )
            {
                // static_cast for integrals, reinterpret_cast for pointers...
                // use C-style as a compromise
                return (T)(var->GetValue());
            }
            else
            {
                return DefaultValue;
            }
        }
        else
        {
            return DefaultValue;
        }
    }
};

template<typename T, T DefaultValue>
NH3API_FORCEINLINE
T getHDModVariable(Patcher* patcher, const char* name)
{ return get_hd_mod_varable_impl::get<T, DefaultValue>(patcher, name); }

NH3API_FORCEINLINE
uint32_t getHDModVersion(Patcher* patcher)
{ return getHDModVariable<uint32_t, 0>(patcher, "HD.Version.Dword"); }

NH3API_FORCEINLINE
HD_game_version getHDModEXEVersion(Patcher* patcher)
{ return getHDModVariable<HD_game_version, HD_NONE>(patcher, "HD.ExeVersion"); }

NH3API_FORCEINLINE
const char* getHDModVersionString(Patcher* patcher)
{ return getHDModVariable<const char*, nullptr>(patcher, "HD.Version.CStr"); }

NH3API_FORCEINLINE
// _HD3_Data
const char* getHDModDirectory(Patcher* patcher)
{ return getHDModVariable<const char*, nullptr>(patcher, "HD.Dir"); }

NH3API_FORCEINLINE
bool isHDPlusPresent(Patcher* patcher)
{ return getHDModVariable<bool, false>(patcher, "HD+.On"); }

NH3API_FORCEINLINE
POINT getHDModResolution(Patcher* patcher)
{
    POINT result;
    result.x = getHDModVariable<int32_t, 0>(patcher, "HD.Option.RezX");
    result.y = getHDModVariable<int32_t, 0>(patcher, "HD.Option.RezY");
    return result;
}

NH3API_FORCEINLINE
int32_t getScreenWidth(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModResolution(patcher).x;
    else
        return 800;
}

NH3API_FORCEINLINE
int32_t getScreenHeight(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModResolution(patcher).x;
    else
        return 600;
}

NH3API_FORCEINLINE
POINT getScreenResolution(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
    {
        return getHDModResolution(patcher);
    }
    else
    {
        POINT result;
        result.x = 800;
        result.y = 600;
        return result;
    }
}

NH3API_FORCEINLINE // this will not inline actually, just for the linker
// Retrieve names of HD Mod plugins /
// Названия плагинов HD Mod.
exe_vector<exe_string> getHDModPlugins(Patcher* patcher)
{
    exe_vector<exe_string> result(0);
    const char* const* const packDirs =
    getHDModVariable<const char* const* const, nullptr>(patcher, "HD.PackDirs");

    const size_t packDirsCount =
    getHDModVariable<const size_t, 0>(patcher, "HD.PackDirs.Count");

    if ( packDirs && packDirsCount )
    {
        for ( size_t i = 0; i < packDirsCount; ++i )
            result.push_back(packDirs[i]);
    }
    return result;
}

// static variables will be thread-safe according to the C++11 standard
// so we need to reduce the code bloat:
// move implementation to structure and mark noinline
struct is_wine_present_impl
{
    NH3API_NOINLINE
    static bool get()
    {
        static bool initialized = false;
        static HMODULE library = nullptr;
        static FARPROC func = nullptr;
        if ( !initialized )
        {
            library = GetModuleHandleA("ntdll.dll");
            if ( library ) // I *really* hope you're running it with windows or WINE
                func = GetProcAddress(library, "wine_get_version");
            initialized = true;
        }
        if ( library )
        {
            return static_cast<bool>(func);
        }
        else
        {
            return false; // So you really run without the ntdll.dll.........
        }
    }
};

NH3API_FORCEINLINE
// Check for WINE /
// Проверить наличие WINE.
bool isWinePresent()
{ return is_wine_present_impl::get(); }