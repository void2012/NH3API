//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
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
        T& get(int32_t offset = 0) NH3API_NOEXCEPT
        { return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset); }

        template <typename T>
        NH3API_FORCEINLINE
        const T& get(int32_t offset = 0) const NH3API_NOEXCEPT
        { return *reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(this) + offset); }

        bstruct_t() NH3API_DELETED_FUNCTION
        bstruct_t(const bstruct_t&) NH3API_DELETED_FUNCTION
        bstruct_t& operator=(const bstruct_t&) NH3API_DELETED_FUNCTION
}; // this is just a fancy wrapper for void*...

template <typename T> NH3API_FORCEINLINE
bstruct_t& get_bstruct(T* ptr, int32_t offset = 0) NH3API_NOEXCEPT
{ return *(reinterpret_cast<bstruct_t*>(ptr) + offset); }

NH3API_FORCEINLINE
bstruct_t& get_bstruct(uint32_t address, int32_t offset = 0) NH3API_NOEXCEPT
{ return *(reinterpret_cast<bstruct_t*>(address) + offset); }

template <typename T> NH3API_FORCEINLINE
const bstruct_t& get_const_bstruct(const T* ptr, int32_t offset = 0) NH3API_NOEXCEPT
{ return *(reinterpret_cast<const bstruct_t*>(ptr) + offset); }

NH3API_FORCEINLINE
const bstruct_t& get_const_bstruct(const uint32_t address, int32_t offset = 0) NH3API_NOEXCEPT
{ return *(reinterpret_cast<const bstruct_t*>(address) + offset); }

template<typename T> NH3API_FORCEINLINE
T* get_ptr(bstruct_t arg, size_t pos) NH3API_NOEXCEPT
{ return arg.get<T*>(pos); }

// default construction policy for the padstruct_t
template<size_t N>
struct padstruct_construct_t
{
    NH3API_CONSTEXPR_CPP_14 padstruct_construct_t() NH3API_NOEXCEPT
    {}

    NH3API_CONSTEXPR_CPP_14 padstruct_construct_t(const padstruct_construct_t&) NH3API_NOEXCEPT
    {}

    NH3API_CONSTEXPR_CPP_14 void operator()(std::array<uint8_t, N>& buf) const
    { buf.fill(0); }
};

// default destruction policy for the padstruct_t
template<size_t N>
struct padstruct_destruct_t
{
    NH3API_CONSTEXPR_CPP_14 padstruct_destruct_t() NH3API_NOEXCEPT
    {}

    NH3API_CONSTEXPR_CPP_14 padstruct_destruct_t(const padstruct_destruct_t&) NH3API_NOEXCEPT
    {}

    NH3API_CONSTEXPR_CPP_14 void operator()(std::array<uint8_t, N>&) const NH3API_NOEXCEPT
    {}
};

// struct of known size but unknown fields. Sized equivalent of bstruct_t /
//
template<size_t N,
         typename ConstructorT = padstruct_construct_t<N>,
         typename DestructorT  = padstruct_destruct_t<N>>
struct padstruct_t
{
    public:
        typedef ConstructorT     constructor_type;
        typedef DestructorT      destructor_type;
        typedef size_t           size_type;
        typedef uint8_t          value_type;

    public:

        // C++23's std::start_lifetime_as is ideal for such a task,
        // sadly it cannot be implemented in C++11..20 without compiler magic.
        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE padstruct_t()
        NH3API_NOEXCEPT_EXPR(nh3api::declval<constructor_type>().operator())
        { constructor_type()(buf); }

        NH3API_FORCEINLINE
        ~padstruct_t()
        NH3API_NOEXCEPT_EXPR(nh3api::declval<destructor_type>().operator())
        {
            #if NH3API_CHECK_CPP11
            static_assert(noexcept(nh3api::declval<destructor_type>().operator()),
            "destructor must not throw");
            #endif
            destructor_type()(buf);
        }

        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE uint8_t* data() NH3API_NOEXCEPT
        { return buf.data(); }

        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE const uint8_t* data() const NH3API_NOEXCEPT
        { return buf.data(); }

        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE size_t size() const NH3API_NOEXCEPT
        { return buf.size(); }

        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE std::array<uint8_t, N>& get() NH3API_NOEXCEPT
        { return buf; }

        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE const std::array<uint8_t, N>& get() const NH3API_NOEXCEPT
        { return buf; }

        template <typename T>
        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE
        T& get(int32_t offset) NH3API_NOEXCEPT
        { return *(T*)((uintptr_t)data() + offset); }

        template <typename T>
        NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE
        const T& get(int32_t offset) const NH3API_NOEXCEPT
        { return *(const T*)((uintptr_t)data() + offset); }

    public:
        std::array<uint8_t, N> buf;
};

template<typename T,
         size_t N,
         typename ConstructPolicyT,
         typename DestructPolicyT> NH3API_FORCEINLINE
T* get_ptr(padstruct_t<N, ConstructPolicyT, DestructPolicyT>& arg, size_t pos) NH3API_NOEXCEPT
{ return arg.template get<T*>(pos); }

NH3API_DISABLE_WARNING_END