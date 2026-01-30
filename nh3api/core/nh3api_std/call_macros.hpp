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

#include <utility>

#include "nh3api_std.hpp"

#ifndef NH3API_CALL_NOEXCEPT
    #ifdef NH3API_FLAG_NOEXCEPT_CALL_MACROS
        #define NH3API_CALL_NOEXCEPT noexcept
    #else
        #define NH3API_CALL_NOEXCEPT
    #endif
#endif

#ifndef CALL_0

#define CALL_0(return_type, call_type, address) \
 ((return_type (call_type *)(void))address)()
#define CALL_1(return_type, call_type, address, a1) \
 ((return_type (call_type *)(uintptr_t))(address))((uintptr_t)(a1))
#define CALL_2(return_type, call_type, address, a1, a2) \
 ((return_type (call_type *)(uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2))
#define CALL_3(return_type, call_type, address, a1, a2, a3) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3))
#define CALL_4(return_type, call_type, address, a1, a2, a3, a4) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4))
#define CALL_5(return_type, call_type, address, a1, a2, a3, a4, a5) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5))
#define CALL_6(return_type, call_type, address, a1, a2, a3, a4, a5, a6) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6))
#define CALL_7(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7))
#define CALL_8(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8))
#define CALL_9(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9))
#define CALL_10(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10))
#define CALL_11(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11))
#define CALL_12(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12))
#define CALL_13(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12),(uintptr_t)(a13))
#define CALL_14(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12),(uintptr_t)(a13),(uintptr_t)(a14))
#define CALL_15(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12),(uintptr_t)(a13),(uintptr_t)(a14),(uintptr_t)(a15))
#define CALL_16(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12),(uintptr_t)(a13),(uintptr_t)(a14),(uintptr_t)(a15),(uintptr_t)(a16))
#define CALL_17(return_type, call_type, address, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17) \
 ((return_type (call_type *)(uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t,uintptr_t))(address))((uintptr_t)(a1),(uintptr_t)(a2),(uintptr_t)(a3),(uintptr_t)(a4),(uintptr_t)(a5),(uintptr_t)(a6),(uintptr_t)(a7),(uintptr_t)(a8),(uintptr_t)(a9),(uintptr_t)(a10),(uintptr_t)(a11),(uintptr_t)(a12),(uintptr_t)(a13),(uintptr_t)(a14),(uintptr_t)(a15),(uintptr_t)(a16),(uintptr_t)(a17))
#endif

#ifndef CALL_VA
#define CALL_VA(return_type, adress, a1, ...) \
 ((return_type (__cdecl *)(uint32_t, ...))(adress))((uint32_t)(a1), __VA_ARGS__)
#endif

#ifdef __clang__
#pragma clang deprecated(CALL_0, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_1, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_2, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_3, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_4, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_5, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_6, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_7, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_8, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_9, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_10, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_11, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_12, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_13, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_14, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_15, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_16, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#pragma clang deprecated(CALL_VA, "CALL_* macros are deprecated, use new XYZCALL_* macros instead")
#endif

template<typename return_type, typename Arg1>
inline return_type invoke_thiscall_1(uintptr_t address, Arg1 a1) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1) NH3API_CALL_NOEXCEPT>(address)(a1)); }
template<typename return_type, typename Arg1, typename Arg2>
inline return_type invoke_thiscall_2(uintptr_t address, Arg1 a1, Arg2 a2) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2) NH3API_CALL_NOEXCEPT>(address)(a1, a2)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3>
inline return_type invoke_thiscall_3(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline return_type invoke_thiscall_4(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline return_type invoke_thiscall_5(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline return_type invoke_thiscall_6(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
inline return_type invoke_thiscall_7(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
inline return_type invoke_thiscall_8(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
inline return_type invoke_thiscall_9(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
inline return_type invoke_thiscall_10(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
inline return_type invoke_thiscall_11(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
inline return_type invoke_thiscall_12(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
inline return_type invoke_thiscall_13(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
inline return_type invoke_thiscall_14(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
inline return_type invoke_thiscall_15(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
inline return_type invoke_thiscall_16(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
inline return_type invoke_thiscall_17(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
inline return_type invoke_thiscall_18(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
inline return_type invoke_thiscall_19(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
inline return_type invoke_thiscall_20(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19, Arg20 a20) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__thiscall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)); }

template<typename return_type>
inline return_type invoke_fastcall_0(uintptr_t address) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)() NH3API_CALL_NOEXCEPT>(address)()); }
template<typename return_type, typename Arg1>
inline return_type invoke_fastcall_1(uintptr_t address, Arg1 a1) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1) NH3API_CALL_NOEXCEPT>(address)(a1)); }
template<typename return_type, typename Arg1, typename Arg2>
inline return_type invoke_fastcall_2(uintptr_t address, Arg1 a1, Arg2 a2) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2) NH3API_CALL_NOEXCEPT>(address)(a1, a2)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3>
inline return_type invoke_fastcall_3(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline return_type invoke_fastcall_4(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline return_type invoke_fastcall_5(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline return_type invoke_fastcall_6(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
inline return_type invoke_fastcall_7(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
inline return_type invoke_fastcall_8(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
inline return_type invoke_fastcall_9(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
inline return_type invoke_fastcall_10(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
inline return_type invoke_fastcall_11(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
inline return_type invoke_fastcall_12(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
inline return_type invoke_fastcall_13(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
inline return_type invoke_fastcall_14(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
inline return_type invoke_fastcall_15(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
inline return_type invoke_fastcall_16(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
inline return_type invoke_fastcall_17(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
inline return_type invoke_fastcall_18(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
inline return_type invoke_fastcall_19(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
inline return_type invoke_fastcall_20(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19, Arg20 a20) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__fastcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)); }

template<typename return_type>
inline return_type invoke_cdecl_0(uintptr_t address) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)() NH3API_CALL_NOEXCEPT>(address)()); }
template<typename return_type, typename Arg1>
inline return_type invoke_cdecl_1(uintptr_t address, Arg1 a1) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1) NH3API_CALL_NOEXCEPT>(address)(a1)); }
template<typename return_type, typename Arg1, typename Arg2>
inline return_type invoke_cdecl_2(uintptr_t address, Arg1 a1, Arg2 a2) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2) NH3API_CALL_NOEXCEPT>(address)(a1, a2)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3>
inline return_type invoke_cdecl_3(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline return_type invoke_cdecl_4(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline return_type invoke_cdecl_5(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline return_type invoke_cdecl_6(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
inline return_type invoke_cdecl_7(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
inline return_type invoke_cdecl_8(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
inline return_type invoke_cdecl_9(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
inline return_type invoke_cdecl_10(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
inline return_type invoke_cdecl_11(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
inline return_type invoke_cdecl_12(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
inline return_type invoke_cdecl_13(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
inline return_type invoke_cdecl_14(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
inline return_type invoke_cdecl_15(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
inline return_type invoke_cdecl_16(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
inline return_type invoke_cdecl_17(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
inline return_type invoke_cdecl_18(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
inline return_type invoke_cdecl_19(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
inline return_type invoke_cdecl_20(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19, Arg20 a20) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__cdecl*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)); }

template<typename return_type>
inline return_type invoke_stdcall_0(uintptr_t address) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)() NH3API_CALL_NOEXCEPT>(address)()); }
template<typename return_type, typename Arg1>
inline return_type invoke_stdcall_1(uintptr_t address, Arg1 a1) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1) NH3API_CALL_NOEXCEPT>(address)(a1)); }
template<typename return_type, typename Arg1, typename Arg2>
inline return_type invoke_stdcall_2(uintptr_t address, Arg1 a1, Arg2 a2) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2) NH3API_CALL_NOEXCEPT>(address)(a1, a2)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3>
inline return_type invoke_stdcall_3(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline return_type invoke_stdcall_4(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline return_type invoke_stdcall_5(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline return_type invoke_stdcall_6(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
inline return_type invoke_stdcall_7(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
inline return_type invoke_stdcall_8(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
inline return_type invoke_stdcall_9(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
inline return_type invoke_stdcall_10(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11>
inline return_type invoke_stdcall_11(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12>
inline return_type invoke_stdcall_12(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13>
inline return_type invoke_stdcall_13(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14>
inline return_type invoke_stdcall_14(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15>
inline return_type invoke_stdcall_15(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16>
inline return_type invoke_stdcall_16(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17>
inline return_type invoke_stdcall_17(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18>
inline return_type invoke_stdcall_18(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19>
inline return_type invoke_stdcall_19(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19)); }
template<typename return_type, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10, typename Arg11, typename Arg12, typename Arg13, typename Arg14, typename Arg15, typename Arg16, typename Arg17, typename Arg18, typename Arg19, typename Arg20>
inline return_type invoke_stdcall_20(uintptr_t address, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10, Arg11 a11, Arg12 a12, Arg13 a13, Arg14 a14, Arg15 a15, Arg16 a16, Arg17 a17, Arg18 a18, Arg19 a19, Arg20 a20) NH3API_CALL_NOEXCEPT
{ return (reinterpret_cast<return_type(__stdcall*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, Arg13, Arg14, Arg15, Arg16, Arg17, Arg18, Arg19, Arg20) NH3API_CALL_NOEXCEPT>(address)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20)); }

#ifndef THISCALL_20
    #define THISCALL_1(return_type, address, arg)  invoke_thiscall_1<return_type>(static_cast<uintptr_t>(address), arg)
    #define THISCALL_2(return_type, address, ...)  invoke_thiscall_2<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_3(return_type, address, ...)  invoke_thiscall_3<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_4(return_type, address, ...)  invoke_thiscall_4<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_5(return_type, address, ...)  invoke_thiscall_5<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_6(return_type, address, ...)  invoke_thiscall_6<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_7(return_type, address, ...)  invoke_thiscall_7<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_8(return_type, address, ...)  invoke_thiscall_8<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_9(return_type, address, ...)  invoke_thiscall_9<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_10(return_type, address, ...) invoke_thiscall_10<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_11(return_type, address, ...) invoke_thiscall_11<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_12(return_type, address, ...) invoke_thiscall_12<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_13(return_type, address, ...) invoke_thiscall_13<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_14(return_type, address, ...) invoke_thiscall_14<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_15(return_type, address, ...) invoke_thiscall_15<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_16(return_type, address, ...) invoke_thiscall_16<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_17(return_type, address, ...) invoke_thiscall_17<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_18(return_type, address, ...) invoke_thiscall_18<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_19(return_type, address, ...) invoke_thiscall_19<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define THISCALL_20(return_type, address, ...) invoke_thiscall_20<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif // #ifndef THISCALL_20

#ifndef FASTCALL_20
    #define FASTCALL_0(return_type, address)       invoke_fastcall_0<return_type>(static_cast<uintptr_t>(address))
    #define FASTCALL_1(return_type, address, arg)  invoke_fastcall_1<return_type>(static_cast<uintptr_t>(address), arg)
    #define FASTCALL_2(return_type, address, ...)  invoke_fastcall_2<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_3(return_type, address, ...)  invoke_fastcall_3<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_4(return_type, address, ...)  invoke_fastcall_4<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_5(return_type, address, ...)  invoke_fastcall_5<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_6(return_type, address, ...)  invoke_fastcall_6<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_7(return_type, address, ...)  invoke_fastcall_7<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_8(return_type, address, ...)  invoke_fastcall_8<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_9(return_type, address, ...)  invoke_fastcall_9<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_10(return_type, address, ...) invoke_fastcall_10<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_11(return_type, address, ...) invoke_fastcall_11<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_12(return_type, address, ...) invoke_fastcall_12<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_13(return_type, address, ...) invoke_fastcall_13<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_14(return_type, address, ...) invoke_fastcall_14<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_15(return_type, address, ...) invoke_fastcall_15<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_16(return_type, address, ...) invoke_fastcall_16<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_17(return_type, address, ...) invoke_fastcall_17<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_18(return_type, address, ...) invoke_fastcall_18<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_19(return_type, address, ...) invoke_fastcall_19<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define FASTCALL_20(return_type, address, ...) invoke_fastcall_20<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif // #ifndef FASTCALL_20

#ifndef CDECL_20
    #define CDECL_0(return_type, address)       invoke_cdecl_0<return_type>(static_cast<uintptr_t>(address))
    #define CDECL_1(return_type, address, arg)  invoke_cdecl_1<return_type>(static_cast<uintptr_t>(address), arg)
    #define CDECL_2(return_type, address, ...)  invoke_cdecl_2<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_3(return_type, address, ...)  invoke_cdecl_3<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_4(return_type, address, ...)  invoke_cdecl_4<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_5(return_type, address, ...)  invoke_cdecl_5<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_6(return_type, address, ...)  invoke_cdecl_6<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_7(return_type, address, ...)  invoke_cdecl_7<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_8(return_type, address, ...)  invoke_cdecl_8<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_9(return_type, address, ...)  invoke_cdecl_9<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_10(return_type, address, ...) invoke_cdecl_10<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_11(return_type, address, ...) invoke_cdecl_11<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_12(return_type, address, ...) invoke_cdecl_12<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_13(return_type, address, ...) invoke_cdecl_13<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_14(return_type, address, ...) invoke_cdecl_14<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_15(return_type, address, ...) invoke_cdecl_15<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_16(return_type, address, ...) invoke_cdecl_16<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_17(return_type, address, ...) invoke_cdecl_17<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_18(return_type, address, ...) invoke_cdecl_18<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_19(return_type, address, ...) invoke_cdecl_19<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define CDECL_20(return_type, address, ...) invoke_cdecl_20<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif // #ifndef CDECL_20

#ifndef STDCALL_20
    #define STDCALL_0(return_type, address)       invoke_stdcall_0<return_type>(static_cast<uintptr_t>(address))
    #define STDCALL_1(return_type, address, arg)  invoke_stdcall_1<return_type>(static_cast<uintptr_t>(address), arg)
    #define STDCALL_2(return_type, address, ...)  invoke_stdcall_2<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_3(return_type, address, ...)  invoke_stdcall_3<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_4(return_type, address, ...)  invoke_stdcall_4<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_5(return_type, address, ...)  invoke_stdcall_5<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_6(return_type, address, ...)  invoke_stdcall_6<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_7(return_type, address, ...)  invoke_stdcall_7<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_8(return_type, address, ...)  invoke_stdcall_8<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_9(return_type, address, ...)  invoke_stdcall_9<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_10(return_type, address, ...) invoke_stdcall_10<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_11(return_type, address, ...) invoke_stdcall_11<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_12(return_type, address, ...) invoke_stdcall_12<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_13(return_type, address, ...) invoke_stdcall_13<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_14(return_type, address, ...) invoke_stdcall_14<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_15(return_type, address, ...) invoke_stdcall_15<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_16(return_type, address, ...) invoke_stdcall_16<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_17(return_type, address, ...) invoke_stdcall_17<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_18(return_type, address, ...) invoke_stdcall_18<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_19(return_type, address, ...) invoke_stdcall_19<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
    #define STDCALL_20(return_type, address, ...) invoke_stdcall_20<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif // #ifndef STDCALL_20

template<typename return_type, typename this_type, typename ... Args>
inline return_type invoke_thiscall(uintptr_t address, this_type* _this, Args&& ... args)
{ return (reinterpret_cast<return_type(__thiscall*)(this_type*, Args...)>(address)(_this, std::forward<Args>(args)...)); }

template<typename return_type, typename ... Args>
inline return_type invoke_fastcall(uintptr_t address, Args&& ... args)
{ return (reinterpret_cast<return_type(__fastcall*)(Args...)>(address)(std::forward<Args>(args)...)); }

template<typename return_type, typename ... Args>
inline return_type invoke_cdecl(uintptr_t address, Args&& ... args)
{ return (reinterpret_cast<return_type(__cdecl*)(Args...)>(address)(std::forward<Args>(args)...)); }

template<typename return_type, typename ... Args>
inline return_type invoke_stdcall(uintptr_t address, Args&& ... args)
{ return (reinterpret_cast<return_type(__stdcall*)(Args...)>(address)(std::forward<Args>(args)...)); }

#ifndef THISCALL_N
    #define THISCALL_N(return_type, address, ...) invoke_thiscall<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif

#ifndef FASTCALL_N
    #define FASTCALL_N(return_type, address, ...) invoke_fastcall<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif

#ifndef CDECL_N
    #define CDECL_N(return_type, address, ...)    invoke_cdecl<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif

#ifndef STDCALL_N
    #define STDCALL_N(return_type, address, ...)  invoke_stdcall<return_type>(static_cast<uintptr_t>(address), __VA_ARGS__)
#endif

#if NH3API_CHECK_MSVC
#pragma warning(push)
#pragma warning(disable: 4229)
#endif

#if NH3API_CHECK_MINGW // workaround for a weird GCC bug
template<typename R, typename ThisPtr, typename ...Args>
using thiscall_function_t = R(*)(ThisPtr, Args...) [[__gnu__::__thiscall__]];

template<typename R, typename ...Args>
using fastcall_function_t = R(*)(Args...) [[__gnu__::__fastcall__]];

template<typename R, typename ...Args>
using cdecl_function_t = R(*)(Args...) [[__gnu__::__cdecl__]];

template<typename R, typename ...Args>
using stdcall_function_t = R(*)(Args...) [[__gnu__::__stdcall__]];
#else
template<typename R, typename ThisPtr, typename ...Args>
using thiscall_function_t = R(*__thiscall)(ThisPtr, Args...);

template<typename R, typename ...Args>
using fastcall_function_t = R(*__fastcall)(Args...);

template<typename R, typename ...Args>
using cdecl_function_t = R(*__cdecl)(Args...);

template<typename R, typename ...Args>
using stdcall_function_t = R(*__stdcall)(Args...);
#endif

#if NH3API_CHECK_MSVC
#pragma warning(pop)
#endif
