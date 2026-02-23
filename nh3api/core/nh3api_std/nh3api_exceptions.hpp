//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <type_traits>

#if defined(_MSC_VER) && !defined(__MINGW32__) && __has_include(<vcruntime_exception.h>) && defined(_HAS_EXCEPTIONS) && _HAS_EXCEPTIONS
#include <vcruntime_exception.h>
#else
#pragma pack(push, 4)
struct __std_exception_data
{
    const char* _What;
    bool        _DoFree;
};
#pragma pack(pop)
#endif

#include "nh3api_std.hpp"
#include "memory.hpp"

// size = 0xC = 12, align = 4
class exe_exception
{
	// vftable
	public:
		struct vftable_t
		{
			void        (__thiscall* scalar_deleting_destructor)(exe_exception*, uint8_t);
			const char* (__thiscall* what)(const exe_exception*);
			// void (__thiscall* _Doraise)(const exe_exception*);
		};

	// virtual functions
	public:
		// vftable shift: +0
		virtual void __thiscall scalar_deleting_destructor(uint8_t flag)
		{ return get_vftable(this)->scalar_deleting_destructor(this, flag); }

		// vftable shift: +4
		[[nodiscard]] virtual const char* __thiscall what() const noexcept
		{ return get_vftable(this)->what(this); }

		// vftable shift: +8
		// [[noreturn]] virtual void __thiscall _Doraise() const
		// { get_vftable(this)->_Doraise(this); NH3API_UNREACHABLE(); }

	// member variables
	public:
        __std_exception_data _Data;
};

namespace nh3api
{
#if !defined(NH3API_FLAG_NO_CPP_EXCEPTIONS) && (NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_NATIVE)
template<typename ExceptionTypeT>
[[noreturn]] void throw_exception(const char what_message[]) noexcept(false)
{ throw ExceptionTypeT{what_message}; }

template<typename ExceptionTypeT>
[[noreturn]] void throw_exception() noexcept(false)
{ throw ExceptionTypeT{}; }
#else
template<typename ExceptionTypeT, size_t StrSize>
[[noreturn]] void throw_exception([[maybe_unused]] const char (&what_message)[StrSize])
{
#if NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_EXE
    #pragma pack(push, 4)
    struct pseudo_exe_runtime_error
    {
        uintptr_t            vftable { 0x645650 };
        __std_exception_data _Data   { static_cast<char*>(::operator new(1, ::exe_heap)), true };

        // avoid dependency on exe_string.hpp
        struct pseudo_exe_string
        {
            int32_t dummy   {0};
            char*   _Myptr  {&(static_cast<char*>(::operator new(StrSize + 1, ::exe_heap))[1])};
            size_t  _Mysize {StrSize};
            size_t  _Myres  {StrSize};
        } _Str;
    };
    pseudo_exe_runtime_error pseudo_exception_object;
    (const_cast<char*>(pseudo_exception_object._Data._What))[0] = '\0';
    (reinterpret_cast<uint8_t*>(pseudo_exception_object._Str._Myptr))[-1] = 0; // set refcount to 0
    ::std::memcpy(pseudo_exception_object._Str._Myptr, what_message, StrSize);
    #pragma pack(pop)
    // _CxxThrowException(&exception_object, &__TI2?AVruntime_error@std@@);
    STDCALL_2(void, 0x617B07, static_cast<void*>(&pseudo_exception_object), 0x6487B8);
#elif NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_UD2
    __ud2();
#elif NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_ABORT
    #if NH3API_HAS_BUILTIN(__builtin_abort)
    __builtin_abort();
    #else
    ::abort();
    #endif
#endif
    NH3API_UNREACHABLE();
}

inline constexpr char unknown_cpp_exception_msg[] {"Unknown C++ Runtime Exception"};

template<typename ExceptionTypeT>
[[noreturn]] void throw_exception()
{
#if NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_EXE
    #pragma pack(push, 4)
    struct pseudo_exe_runtime_error
    {
        uintptr_t            vftable { 0x645650 };
        __std_exception_data _Data   { static_cast<char*>(::operator new(1, ::exe_heap)), true };

        // avoid dependency on exe_string.hpp
        struct pseudo_exe_string
        {
            int32_t dummy   {0};
            char*   _Myptr  {&(static_cast<char*>(::operator new(1 + sizeof(unknown_cpp_exception_msg), ::exe_heap))[1])};
            size_t  _Mysize {sizeof(unknown_cpp_exception_msg)};
            size_t  _Myres  {sizeof(unknown_cpp_exception_msg)};
        } _Str;
    } pseudo_exception_object;
    (const_cast<char*>(pseudo_exception_object._Data._What))[0] = '\0';
    (reinterpret_cast<uint8_t*>(pseudo_exception_object._Str._Myptr))[-1] = 0; // set refcount to 0
    ::std::memcpy(pseudo_exception_object._Str._Myptr, &unknown_cpp_exception_msg[0], sizeof(unknown_cpp_exception_msg));
    #pragma pack(pop)
    // _CxxThrowException(&exception_object, &__TI2?AVruntime_error@std@@);
    STDCALL_2(void, 0x617B07, static_cast<void*>(&pseudo_exception_object), 0x6487B8);

#elif NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_UD2
    __ud2();
#elif NH3API_RAISE_EXCEPTION_TYPE == NH3API_RAISE_EXCEPTION_ABORT
    #if NH3API_HAS_BUILTIN(__builtin_abort)
    __builtin_abort();
    #else
    ::abort();
    #endif
#endif
    NH3API_UNREACHABLE();
}
#endif

} // namespace nh3api
