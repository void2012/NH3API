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

#include "nh3api_std/nh3api_std.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_MSVC_DISABLE(4714)

// Abstract structure. Similiar to WoG _Struct_ (Field<T> = get<T>). Used only as reference or a pointer! /
// Examples:
// bstruct_t* ptr = &get_bstruct(c->esi); // use &get_bstruct(c->esi) instead...
// ptr->get<int32_t>(12) = 64; // set int var at offset 12 value 64.
//
// const bstruct_t& win = get_const_bstruct(windowPtr); // assume 'windowPtr' of type void*; 'name' of type exe_string
// name += win.get<exe_string>(0x128).front(); // get the first character of the string at offset 0x128 and append it to 'name'
//
// bstruct_t& struct1 = get_bstruct(mapPtr, 12); // assume 'mapPtr' of type void*
// // get reference to object at offset 12 of unknown structure in 'mapPtr'
struct
#if NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__may_alias__)
[[__gnu__::__may_alias__]]
#endif
bstruct_t
{
    public:
        template <typename T> NH3API_FORCEINLINE
        T& get(ptrdiff_t offset = 0) noexcept
        { return *__builtin_launder(reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + static_cast<uintptr_t>(offset))); }

        template <typename T> NH3API_FORCEINLINE
        const T& get(ptrdiff_t offset = 0) const noexcept
        { return *__builtin_launder(reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(this) + static_cast<uintptr_t>(offset))); }

        bstruct_t()                            = delete;
        bstruct_t(bstruct_t&&)                 = delete;
        bstruct_t& operator=(bstruct_t&&)      = delete;
        bstruct_t(const bstruct_t&)            = delete;
        bstruct_t& operator=(const bstruct_t&) = delete;
        ~bstruct_t()                           = default;
}; // this is just a fancy wrapper for void*...

NH3API_FORCEINLINE bstruct_t& get_bstruct(void* ptr, ptrdiff_t offset = 0) noexcept
{ return *(__builtin_launder(reinterpret_cast<bstruct_t*>(reinterpret_cast<uintptr_t>(ptr) + static_cast<uintptr_t>(offset)))); }

NH3API_FORCEINLINE bstruct_t& get_bstruct(uintptr_t address, ptrdiff_t offset = 0) noexcept
{ return *(__builtin_launder(reinterpret_cast<bstruct_t*>(address + static_cast<uintptr_t>(offset)))); }

NH3API_FORCEINLINE const bstruct_t& get_const_bstruct(const void* ptr, ptrdiff_t offset = 0) noexcept
{ return *(__builtin_launder(reinterpret_cast<const bstruct_t*>(reinterpret_cast<uintptr_t>(ptr) + static_cast<uintptr_t>(offset)))); }

NH3API_FORCEINLINE const bstruct_t& get_const_bstruct(const uintptr_t address, ptrdiff_t offset = 0) noexcept
{ return *(__builtin_launder(reinterpret_cast<const bstruct_t*>(address + static_cast<uintptr_t>(offset)))); }

NH3API_WARNING(pop)
