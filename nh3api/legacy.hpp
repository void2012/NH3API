//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdlib> // free, malloc, realloc, calloc
#include <malloc.h> // _msize
#include "core/nh3api_std/exe_string.hpp" // exe_string, to_exe_string, string printing
#include "core/utils.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

// legacy typedefs

NH3API_DEPRECATED("WoG types are deprecated. use int64_t") typedef int64_t _qword_;
NH3API_DEPRECATED("WoG types are deprecated. use int32_t") typedef int32_t _int32_;
NH3API_DEPRECATED("WoG types are deprecated. use int16_t") typedef int16_t _int16_;
NH3API_DEPRECATED("WoG types are deprecated. use int8_t") typedef int8_t _int8_;
NH3API_DEPRECATED("WoG types are deprecated. use uint8_t") typedef uint8_t _byte_;
NH3API_DEPRECATED("WoG types are deprecated. use uint16_t") typedef uint16_t _word_;
NH3API_DEPRECATED("WoG types are deprecated. use uint32_t") typedef uint32_t _dword_;
NH3API_DEPRECATED("WoG types are deprecated. use uintptr_t") typedef uintptr_t _ptr_;
NH3API_DEPRECATED("WoG types are deprecated. use char") typedef char _char_;
NH3API_DEPRECATED("WoG types are deprecated. use bool") typedef bool _bool8_;
NH3API_DEPRECATED("WoG types are deprecated. use bool32_t") typedef int32_t _bool32_;
NH3API_DEPRECATED("WoG types are deprecated. use float") typedef float _float_;
NH3API_DEPRECATED("WoG types are deprecated. use double") typedef double _double_;

// legacy pointer macros
// use get_ptr instead...

#ifndef IntAt
    #define IntAt(address) (*(_int32_*)(address))
#endif

#ifndef DwordAt
    #define DwordAt(address) (*(_dword_*)(address))
#endif

#ifndef QwordAt
    #define QwordAt(address) (*(_qword_*)(address))
#endif

#ifndef ByteAt
    #define ByteAt(address) (*(_byte_*)(address))
#endif

#ifndef WordAt
    #define WordAt(address) (*(_word_*)(address))
#endif

#ifndef CStrAt
    #define CStrAt(address) (*(_cstr_*)(address))
#endif

#ifndef PtrAt
    #define PtrAt(address) (*(_ptr_*)(address))
#endif

#ifndef FloatAt
    #define FloatAt(address) (*(_float_*)(address))
#endif

#ifndef DoubleAt
    #define DoubleAt(address) (*(_double_*)(address))
#endif

NH3API_DEPRECATED("WoG MemCpy is deprecated. Use std::memcpy or std::copy or std::copy_n")
inline void _MemCopy(void* dst, const void* src, size_t size)
{ ::std::memcpy(dst, src, size); }
#define MemCopy(dst, src, size) _MemCopy((void*)(dst), (void*)(src), (size_t)(size))
NH3API_DEPRECATED("WoG MemSet is deprecated. Use std::memset or std::fill or std::fill_n")
inline void _MemSet(void* dst, char value, size_t size)
{ ::std::memset(dst, value, size); }
#define MemSet(ptr, value, size) _MemSet((void*)(ptr), (char)(value), (size_t)(size))
#define MemZero(ptr, size) MemSet(ptr, 0, size)
#define MemAlloc(size) ((_ptr_)::std::malloc((size_t)(size)))
#define MemFree(ptr) ::std::free((void*)(ptr))
#define MemRealloc(ptr, size) ((_ptr_)::std::realloc((void*)(ptr), (size_t)(size)))
#define MemCalloc(num, size) ((_ptr_)::std::calloc((size_t)(num), (size_t)(size)))
#define MemSize(ptr) ((_dword_)_msize((void*)(ptr)))

template<typename StringT>
NH3API_FORCEINLINE
StringT IntToStr(int32_t x)
{ return nh3api::print_int<StringT>(x); }

template<typename StringT>
NH3API_FORCEINLINE
StringT IntToStr(uint32_t x)
{ return nh3api::print_uint<StringT>(x); }

template<>
NH3API_FORCEINLINE
exe_string IntToStr<exe_string>(int32_t x)
{ return to_exe_string(x); }

template<>
NH3API_FORCEINLINE
exe_string IntToStr<exe_string>(uint32_t x)
{ return to_exe_string(x); }

template<>
NH3API_FORCEINLINE
exe_wstring IntToStr<exe_wstring>(int32_t x)
{ return to_exe_wstring(x); }

template<>
NH3API_FORCEINLINE
exe_wstring IntToStr<exe_wstring>(uint32_t x)
{ return to_exe_wstring(x); }

inline int32_t StrToInt(const char* cstr)
{ return ::std::atoi(cstr); }

template<typename StringT> inline
int32_t StrToInt(const StringT& str)
{ return ::std::atoi(str.c_str()); }

NH3API_DISABLE_WARNING_END