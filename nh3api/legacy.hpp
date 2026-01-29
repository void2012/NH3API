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

#include <cstdlib> // free, malloc, realloc, calloc
#include <malloc.h> // _msize
#include "core/nh3api_std/exe_string.hpp" // exe_string, to_exe_string, string printing
#include "core/utils.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_MSVC_DISABLE(4714)

// legacy typedefs

[[deprecated("WoG types are deprecated. use int64_t")]] typedef int64_t _qword_;
[[deprecated("WoG types are deprecated. use int32_t")]] typedef int32_t _int32_;
[[deprecated("WoG types are deprecated. use int16_t")]] typedef int16_t _int16_;
[[deprecated("WoG types are deprecated. use int8_t")]] typedef int8_t _int8_;
[[deprecated("WoG types are deprecated. use uint8_t")]] typedef uint8_t _byte_;
[[deprecated("WoG types are deprecated. use uint16_t")]] typedef uint16_t _word_;
[[deprecated("WoG types are deprecated. use uint32_t")]] typedef uint32_t _dword_;
[[deprecated("WoG types are deprecated. use uintptr_t")]] typedef uintptr_t _ptr_;
[[deprecated("WoG types are deprecated. use char")]] typedef char _char_;
[[deprecated("WoG types are deprecated. use bool")]] typedef bool _bool8_;
[[deprecated("WoG types are deprecated. use bool32_t")]] typedef int32_t _bool32_;
[[deprecated("WoG types are deprecated. use float")]] typedef float _float_;
[[deprecated("WoG types are deprecated. use double")]] typedef double _double_;
[[deprecated("WoG types are deprecated. use const char*")]] typedef const char* _cstr_;

template<typename T> NH3API_FORCEINLINE
int32_t IntAt(const T* ptr) noexcept
{ return *const_cast<int32_t*>(reinterpret_cast<const int32_t*>(ptr)); }

NH3API_FORCEINLINE
int32_t IntAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, int32_t); }

template<typename T> NH3API_FORCEINLINE
uint32_t DwordAt(const T* ptr) noexcept
{ return *const_cast<uint32_t*>(reinterpret_cast<const uint32_t*>(ptr)); }

NH3API_FORCEINLINE
uint32_t DwordAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, uint32_t); }

template<typename T> NH3API_FORCEINLINE
uint64_t QwordAt(const T* ptr) noexcept
{ return *const_cast<uint64_t*>(reinterpret_cast<const uint64_t*>(ptr)); }

NH3API_FORCEINLINE
uint64_t QwordAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, uint64_t); }

template<typename T> NH3API_FORCEINLINE
uint8_t ByteAt(const T* ptr) noexcept
{ return *const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(ptr)); }

NH3API_FORCEINLINE
uint8_t ByteAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, uint8_t); }

template<typename T> NH3API_FORCEINLINE
uint16_t WordAt(const T* ptr) noexcept
{ return *const_cast<uint16_t*>(reinterpret_cast<const uint16_t*>(ptr)); }

NH3API_FORCEINLINE
uint16_t WordAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, uint16_t); }

template<typename T> NH3API_FORCEINLINE
const char* CStrAt(const T* ptr) noexcept
{ return *const_cast<const char**>(reinterpret_cast<const char* const *>(ptr)); }

NH3API_FORCEINLINE
const char* CStrAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, const char*); }

template<typename T> NH3API_FORCEINLINE
uintptr_t PtrAt(const T* ptr) noexcept
{ return *const_cast<uintptr_t*>(reinterpret_cast<const uintptr_t*>(ptr)); }

NH3API_FORCEINLINE
uintptr_t PtrAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, uintptr_t); }

template<typename T> NH3API_FORCEINLINE
float FloatAt(const T* ptr) noexcept
{ return *const_cast<float*>(reinterpret_cast<const float*>(ptr)); }

NH3API_FORCEINLINE
float FloatAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, float); }

template<typename T> NH3API_FORCEINLINE
double DoubleAt(const T* ptr) noexcept
{ return *const_cast<double*>(reinterpret_cast<const double*>(ptr)); }

NH3API_FORCEINLINE
double DoubleAt(uintptr_t address) noexcept
{ return get_global_var_ref(address, double); }

template<typename T, typename Size>
inline void* MemCopy(T* dst, const T* src, Size size) noexcept
{ return ::std::memcpy(reinterpret_cast<void*>(dst), reinterpret_cast<const void*>(src), size); }

template<typename T, typename Size>
inline void* MemZero(T* ptr, Size size) noexcept
{ return ::std::memset(reinterpret_cast<void*>(ptr), 0, static_cast<size_t>(size)); }

template<typename T, typename V, typename Size>
inline void* Memset(T* ptr, V value, Size size) noexcept
{ return ::std::memset(reinterpret_cast<void*>(ptr), static_cast<uint8_t>(value), static_cast<size_t>(size)); }

template<typename Size>
inline void* MemAlloc(Size size) noexcept
{ return ::std::malloc(static_cast<size_t>(size)); }

template<typename T>
inline void MemFree(T* ptr) noexcept
{ ::std::free(reinterpret_cast<void*>(ptr)); }

template<typename T, typename Size>
inline void* MemRealloc(T* ptr, Size size) noexcept
{ ::std::realloc(reinterpret_cast<void*>(ptr), static_cast<size_t>(size)); }

template<typename Size>
inline void* MemCalloc(Size num, Size size) noexcept
{ return ::std::calloc(reinterpret_cast<size_t>(num), static_cast<size_t>(size)); }

template<typename T>
inline size_t MemSize(T* ptr) noexcept
{ return ::_msize(reinterpret_cast<void*>(ptr)); }

template<typename StringT>
NH3API_FORCEINLINE
StringT IntToStr(int32_t x)
{ return nh3api::int_to_string<StringT>(x); }

template<typename StringT>
NH3API_FORCEINLINE
StringT IntToStr(uint32_t x)
{ return nh3api::uint_to_string<StringT>(x); }

template<>
NH3API_FORCEINLINE
exe_string IntToStr<exe_string>(int32_t x)
{ return to_exe_string(x); }

template<>
NH3API_FORCEINLINE
exe_string IntToStr<exe_string>(uint32_t x)
{ return to_exe_string(x); }

NH3API_WARNING(pop)
