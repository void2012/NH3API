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

#include "nh3api_std/memory.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

// Abstract structure. Similiar to WoG _Struct_ (Field<T> = get<T>). Used only as reference or a pointer!!! /
// Examples:
// bstruct_t* ptr = &get_bstruct(c->esi); // use &get_bstruct(c->esi) instead...
// ptr->get<int32_t>(12) = 64; // set int var at offset 12 value 64.
//
// const bstruct_t& win = get_const_bstruct(windowPtr); // assume 'windowPtr' of type void*; 'name' of type exe_string
// name += win.get<exe_string>(0x128).front(); // get the first character of the string at offset 0x128 and append it to 'name'
//
// bstruct_t& struct1 = get_bstruct(mapPtr, 12); // assume 'mapPtr' of type void*
// // get reference to object at offset 12 of unknown structure in 'mapPtr'
struct bstruct_t
{
    public:
        template <typename T>
        NH3API_FORCEINLINE
        T& get(ptrdiff_t offset = 0) noexcept
        { return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(__builtin_launder(this)) + offset); }

        template <typename T>
        NH3API_FORCEINLINE
        const T& get(ptrdiff_t offset = 0) const noexcept
        { return *reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(__builtin_launder(this)) + offset); }

        bstruct_t()                            = delete;
        bstruct_t(bstruct_t&&)                 = delete;
        bstruct_t& operator=(bstruct_t&&)      = delete;
        bstruct_t(const bstruct_t&)            = delete;
        bstruct_t& operator=(const bstruct_t&) = delete;
}; // this is just a fancy wrapper for void*...

template <typename T> NH3API_FORCEINLINE
bstruct_t& get_bstruct(T* ptr, ptrdiff_t offset = 0) noexcept
{ return *(reinterpret_cast<bstruct_t*>(ptr) + offset); }

NH3API_FORCEINLINE bstruct_t& get_bstruct(uint32_t address, ptrdiff_t offset = 0) noexcept
{ return *(reinterpret_cast<bstruct_t*>(address) + offset); }

template <typename T> NH3API_FORCEINLINE
const bstruct_t& get_const_bstruct(const T* ptr, ptrdiff_t offset = 0) noexcept
{ return *(reinterpret_cast<const bstruct_t*>(ptr) + offset); }

NH3API_FORCEINLINE const bstruct_t& get_const_bstruct(const uint32_t address, ptrdiff_t offset = 0) noexcept
{ return *(reinterpret_cast<const bstruct_t*>(address) + offset); }

template<typename T> NH3API_FORCEINLINE
T* get_ptr(bstruct_t arg, ptrdiff_t pos) noexcept
{ return arg.get<T*>(pos); }

// default construction policy for the padstruct_t
template<ptrdiff_t N>
struct padstruct_construct_t
{
    constexpr padstruct_construct_t() noexcept = default;
    padstruct_construct_t(padstruct_construct_t&&)                         = delete;
    padstruct_construct_t& operator=(const padstruct_construct_t&)         = delete;
    padstruct_construct_t& operator=(padstruct_construct_t&&)              = delete;
    constexpr padstruct_construct_t(const padstruct_construct_t&) noexcept = default;
    ~padstruct_construct_t() = default;
    constexpr void operator()(std::array<uint8_t, N>& buf) const noexcept
    { buf.fill(0); }

};

// default destruction policy for the padstruct_t
template<ptrdiff_t N>
struct padstruct_destruct_t
{
    constexpr padstruct_destruct_t() noexcept = default;
    padstruct_destruct_t(padstruct_destruct_t&&)                         = delete;
    padstruct_destruct_t& operator=(const padstruct_destruct_t&)         = delete;
    padstruct_destruct_t& operator=(padstruct_destruct_t&&)              = delete;
    constexpr padstruct_destruct_t(const padstruct_destruct_t&) noexcept = default;
    ~padstruct_destruct_t() = default;

    constexpr void operator()(std::array<uint8_t, N>&) const noexcept
    {}
};

// struct of known size but unknown fields. Sized equivalent of bstruct_t /
//
template<ptrdiff_t N,
         typename ConstructorT = padstruct_construct_t<N>,
         typename DestructorT  = padstruct_destruct_t<N>>
struct padstruct_t
{
    public:
        using constructor_type = ConstructorT;
        using destructor_type  = DestructorT;
        using size_type        = ptrdiff_t;
        using value_type       = uint8_t;

    public:
        // C++23's std::start_lifetime_as is ideal for such a task,
        // sadly it cannot be implemented in C++11..20 without compiler magic.
        constexpr NH3API_FORCEINLINE padstruct_t()
        noexcept(noexcept(std::declval<constructor_type>().operator()))
        { constructor_type()(buf); }

        NH3API_FORCEINLINE ~padstruct_t() noexcept(noexcept(std::declval<destructor_type>().operator()))
        {
            #if NH3API_CHECK_CPP11
            static_assert(noexcept(nh3api::declval<destructor_type>().operator()),
            "destructor must not throw");
            #endif
            destructor_type()(buf);
        }

        constexpr NH3API_FORCEINLINE uint8_t* data() noexcept
        { return buf.data(); }

        [[nodiscard]] constexpr NH3API_FORCEINLINE const uint8_t* data() const noexcept
        { return buf.data(); }

        [[nodiscard]] constexpr NH3API_FORCEINLINE ptrdiff_t size() const noexcept
        { return buf.size(); }

        constexpr NH3API_FORCEINLINE std::array<uint8_t, N>& get() noexcept
        { return buf; }

        constexpr NH3API_FORCEINLINE const std::array<uint8_t, N>& get() const noexcept
        { return buf; }

        template <typename T>
        constexpr NH3API_FORCEINLINE
        T& get(int32_t offset) noexcept
        { return *(T*)((uintptr_t)data() + offset); }

        template <typename T>
        constexpr NH3API_FORCEINLINE
        const T& get(int32_t offset) const noexcept
        { return *(const T*)((uintptr_t)data() + offset); }

    public:
        std::array<uint8_t, N> buf;
};

template<typename T,
         ptrdiff_t N,
         typename ConstructPolicyT,
         typename DestructPolicyT> NH3API_FORCEINLINE
T* get_ptr(padstruct_t<N, ConstructPolicyT, DestructPolicyT>& arg, ptrdiff_t pos) noexcept
{ return arg.template get<T*>(pos); }

NH3API_DISABLE_WARNING_END
