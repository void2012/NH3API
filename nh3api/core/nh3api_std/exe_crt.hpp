//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std.hpp"
#include "patcher_x86.hpp"
#include <winbase.h>

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

typedef void (__cdecl* exe_PHNDLR)(int32_t);
typedef void (__cdecl* exe_new_handler)();
typedef void (__cdecl* exe_se_translator)(uint32_t, EXCEPTION_POINTERS*);
typedef void (__cdecl* exe_atexit_func_t)(void);
typedef void (__cdecl* exe_beginthread_func_t)(void);
typedef uint32_t (__stdcall* exe_beginthreadex_func_t)(void);

NH3API_FORCEINLINE
// address: 0x6193EA
// Heroes3.exe internal exit
void __cdecl exe_exit(int32_t code) NH3API_NOEXCEPT
{ CDECL_1(void,  0x6193EA, code); }

NH3API_FORCEINLINE
// address: 0x617F03
// Heroes3.exe internal atexit
int __cdecl exe_atexit(exe_atexit_func_t func) NH3API_NOEXCEPT
{ return CDECL_1(int, 0x617F03, func); }

NH3API_FORCEINLINE
// address: 0x61D9CA
// Heroes3.exe internal terminate
void __cdecl /*__noreturn*/ exe_terminate() NH3API_NOEXCEPT
{ CDECL_0(void, 0x61D9CA); }

NH3API_FORCEINLINE
// address: 0x624F26
// Heroes3.exe internal abort
void __cdecl exe_abort() NH3API_NOEXCEPT
{ CDECL_0(void, 0x624F26); }

NH3API_FORCEINLINE
// address: 0x618370
// Heroes3.exe internal __purecall
void __cdecl exe_purecall() NH3API_NOEXCEPT
{ CDECL_0(void, 0x618370); }

NH3API_FORCEINLINE
// address: 0x61A433
// Heroes3.exe internal _beginthreadex. Use this function to avoid memory leaks related to CreateThread
uintptr_t __cdecl exe_beginthreadex(SECURITY_ATTRIBUTES* security,
                                    size_t stack_size,
                                    exe_beginthreadex_func_t start_address,
                                    void* arglist,
                                    uint32_t initflag,
                                    uint32_t* thrdaddr) NH3API_NOEXCEPT
{ return CDECL_6(uintptr_t, 0x61A433, security, stack_size, start_address, arglist, initflag, thrdaddr); }

NH3API_FORCEINLINE
// address: 0x61A53A
// Heroes3.exe internal _endthreadex
void __cdecl exe_endthreadex(uint32_t exit_code)
{ CDECL_1(void, 0x61A53A, exit_code); }

NH3API_FORCEINLINE
// address: 0x61A56C
// Heroes3.exe internal _beginthread
uintptr_t __cdecl exe_beginthread(exe_beginthread_func_t start_address, size_t stack_size, void* args)
{ return CDECL_3(uintptr_t, 0x61A56C, start_address, stack_size, args); }

NH3API_FORCEINLINE
// address: 0x61A676
// Heroes3.exe internal _beginthread
void __cdecl exe_endthread()
{ CDECL_0(void, 0x61A676); }

#pragma pack(push, 4)
// Exception-action table used by the C runtime to identify and dispose of
// exceptions corresponding to C runtime errors or C signals
// size = 0xC = 12, align = 4
struct exe_XCPT_ACTION
{
    // Exception code or number
    // offset: +0x0 = +0,  size = 0x4 = 4
    uint32_t XcptNum;

    // Signal code or number. Defined by the C runtime
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t SigNum;

    // Exception action code. Either a special code or the address of
    // a handler function. Always determines how the exception filter
    // should dispose of the exception
    // offset: +0x8 = +8,  size = 0x4 = 4
    union
    {
        exe_PHNDLR handler;
        int32_t    code;
    } XcptAction;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

#pragma pack(push, 4)
// Structure for each thread's data
// size = 0x74 = 116, align = 4
struct exe_tiddata
{
    // Thread ID
    // offset: +0x0 = +0,  size = 0x4 = 4
    uint32_t _tid;

    // Thread handle
    // offset: +0x4 = +4,  size = 0x4 = 4
    HANDLE _thandle;

    // errno value
    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t _terrno;

    // _doserrno value
    // offset: +0xC = +12,  size = 0x4 = 4
    uint32_t _tdoserrno;

protected:
    // offset: +0x10 = +16,  size = 0x4 = 4
    uintptr_t _fpds;
public:

    // rand() seed value
    // offset: +0x14 = +20,  size = 0x4 = 4
    uint32_t _holdrand;

    // Pointer to strtok() token
    // offset: +0x18 = +24,  size = 0x4 = 4
    char* _token;

    // Pointer to wcstok() token
    // offset: +0x1C = +28,  size = 0x4 = 4
    wchar_t* _wtoken;

    // Pointer to _mbstok() token
    // offset: +0x20 = +32,  size = 0x4 = 4
    char* _mtoken;

    // Pointer to strerror()/_strerror() buff
    // offset: +0x24 = +36,  size = 0x4 = 4
    char* _errmsg;

    // Pointer to tmpnam() buffer
    // offset: +0x28 = +40,  size = 0x4 = 4
    char* _namebuf0;

    // Pointer to _wtmpnam() buffer
    // offset: +0x2C = +44,  size = 0x4 = 4
    wchar_t* _wnamebuf0;

    // Pointer to tmpfile() buffer
    // offset: +0x30 = +48,  size = 0x4 = 4
    char* _namebuf1;

    // Pointer to _wtmpfile() buffer
    // offset: +0x34 = +52,  size = 0x4 = 4
    wchar_t* _wnamebuf1;

    // Pointer to asctime() buffer
    // offset: +0x38 = +56,  size = 0x4 = 4
    char* _asctimebuf;

    // Pointer to _wasctime() buffer
    // offset: +0x3C = +60,  size = 0x4 = 4
    wchar_t* _wasctimebuf;

    // Pointer to gmtime() structure
    // offset: +0x40 = +64,  size = 0x4 = 4
    void* _gmtimebuf;

    // Pointer to ecvt()/fcvt() buffer
    // offset: +0x44 = +68,  size = 0x4 = 4
    char* _cvtbuf;

    // Initial user thread address
    // offset: +0x48 = +72,  size = 0x4 = 4
    uintptr_t _initaddr;

    // Initial user thread argument
    // offset: +0x4C = +76,  size = 0x4 = 4
    void* _initarg;

    // Pointer to exception-action table
    // offset: +0x50 = +80,  size = 0x4 = 4
    exe_XCPT_ACTION* _pxcptacttab;

    // Pointer to exception info pointers
    // offset: +0x54 = +84,  size = 0x4 = 4
    void* _tpxcptinfoptrs;

    // Floating point exception code
    // offset: +0x58 = +88,  size = 0x4 = 4
    int32_t _tfpecode;

protected:
    // offset: +0x5C = +92,  size = 0x4 = 4
    uint32_t _NLG_dwCode;
public:

    // Pointer to terminate() function
    // offset: +0x60 = +96,  size = 0x4 = 4
    void (__cdecl *_terminate)();

    // Pointer to unexpected() function
    // offset: +0x64 = +100,  size = 0x4 = 4
    void* _unexpected;

    // Pointer to SE-translator function
    // offset: +0x68 = +104,  size = 0x4 = 4
    exe_se_translator _translator;

    // Current exception object
    // offset: +0x6C = +108,  size = 0x4 = 4
    void* _curexception;

    // Current exception context
    // offset: +0x70 = +112,  size = 0x4 = 4
    void* _curcontext;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

NH3API_FORCEINLINE
// address: [0x61D8C3..0x61D92A],  size = 0x67 = 103
// Get current thread tiddata /
// tiddata информация, связанная с текущим потоком, созданным CRT.
exe_tiddata* exe_getptd()
{ return CDECL_0(exe_tiddata*, 0x61D8C3); }

NH3API_FORCEINLINE
exe_new_handler exe_set_new_handler(exe_new_handler handler)
{
    exe_new_handler old_handler = reinterpret_cast<exe_new_handler>(0x6AB86C);
    *reinterpret_cast<exe_new_handler*>(0x6AB86C) = handler;
    return old_handler;
}

NH3API_FORCEINLINE
exe_se_translator exe_set_se_translator(exe_tiddata* curr, exe_se_translator translator)
{
    exe_se_translator old_translator = curr->_translator;
    curr->_translator = translator;
    return old_translator;
}

NH3API_DISABLE_WARNING_END