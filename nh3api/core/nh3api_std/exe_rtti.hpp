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

// This header is work in progress

#include <cstddef>
#include <cstdint>
#include <windef.h>

#ifndef NH3API_HAS_VCRUNTIME_INCLUDE
    #if defined(_MSC_VER)
        //#include <rttidata.h>
        #include <vcruntime_typeinfo.h>
        #define NH3API_HAS_VCRUNTIME_INCLUDE (1)
    #else
        #if __has_include(<rttidata.h>) && __has_include(<vcruntime_typeinfo.h>)
            #include <rttidata.h>
            #define NH3API_HAS_VCRUNTIME_INCLUDE (1)
        #else
            #define NH3API_HAS_VCRUNTIME_INCLUDE (0)
        #endif
    #endif
#endif

#include "nh3api_std.hpp"
#include "hash.hpp"

struct __type_info_node
{
#if NH3API_CRT_VERSION >= NH3API_CRT_UCRT
        _SLIST_HEADER _Header {};
#elif NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80 && NH3API_CRT_VERSION <= NH3API_CRT_MSVCR120
        void*             _MemPtr = nullptr;
        __type_info_node* _Next   = nullptr;
#endif // for msvcrt.dll, msvcr70.dll and msvcr71.dll there is no cacheing for undecorated type names
};

#if !NH3API_HAS_VCRUNTIME_INCLUDE
struct __std_type_info_data
{
        const char* _UndecoratedName;
        const char  _DecoratedName[1];
        __std_type_info_data()                            = delete;
        __std_type_info_data(const __std_type_info_data&) = delete;
        __std_type_info_data(__std_type_info_data&&)      = delete;

        __std_type_info_data& operator=(const __std_type_info_data&) = delete;
        __std_type_info_data& operator=(__std_type_info_data&&)      = delete;
};

extern "C" _CRTIMP const char* __cdecl __std_type_info_name(__std_type_info_data* _Data, __type_info_node* _RootNode);
extern "C" _CRTIMP void __cdecl        __std_type_info_destroy_list(__type_info_node* const root_node);

#else
extern "C" _VCRTIMP const char* __cdecl __std_type_info_name(__std_type_info_data* _Data, __type_info_node* _RootNode);
#endif

#if NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80 && NH3API_CRT_VERSION <= NH3API_CRT_MSVCR120
extern "C" _CRTIMP void __cdecl __clean_type_info_names_internal(__type_info_node* p_type_info_root_node);
#endif

namespace nh3api::msvcrt
{

#if !NH3API_CHECK_MSVC_DRIVER
class NH3API_CLANG_TRIVIAL_ABI global_type_info_node
{
    public:
        global_type_info_node() noexcept
        {
    #if NH3API_CRT_VERSION >= NH3API_CRT_UCRT
            ::InitializeSListHead(&node._Header);
    #endif
        }

        ~global_type_info_node() noexcept
        {
    #if NH3API_CRT_VERSION >= NH3API_CRT_UCRT
            __std_type_info_destroy_list(&node);
    #elif NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80 && NH3API_CRT_VERSION <= NH3API_CRT_MSVCR120
            __clean_type_info_names_internal(&node);
    #endif
        }

    protected:
        [[nodiscard]] const __type_info_node* get() const noexcept
        {
            return &node;
        }

        __type_info_node* get() noexcept
        {
            return &node;
        }

        __type_info_node node;
};
#endif // Is not MSVC Driver

#if NH3API_CRT_VERSION < NH3API_CRT_UCRT
// std::type_info::name
extern "C" const char* __thiscall std_type_info_name(const void* typeinfo_ptr)
    #if NH3API_CRT_VERSION < NH3API_CRT_MSVCR80
    asm("\"?name@type_info@@QBEPBDXZ\"");
    #else
    = delete;
    #endif

extern "C" const char* __thiscall std_type_info_name_from_node(const void* typeinfo_ptr, __type_info_node* node)
    #if NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80 && NH3API_CRT_VERSION <= NH3API_CRT_MSVCR120
    asm("\"?name@type_info@@QBEPBDPAU__type_info_node@@@Z\"");
    #else
    = delete;
    #endif

#endif

} // namespace nh3api::msvcrt

class exe_type_info
{
    public:
        struct vftable_t
        {
                void(__thiscall* scalar_deleting_destructor)(exe_type_info*, uint8_t);
        };

    private:
        inline int do_compare(const exe_type_info& other) const noexcept
        {
            if ( this == &other )
                return 0;

            return strcmp(&this->_Data._DecoratedName[1], &other._Data._DecoratedName[1]);
        }

    public:
        exe_type_info(const exe_type_info&)            = delete;
        exe_type_info& operator=(const exe_type_info&) = delete;

        [[nodiscard]] size_t hash_code() const noexcept
        {
            // vcruntime uses the same hashing algorithm (fnv1a) as nh3api::default_hash
            nh3api::default_hash hasher;
            const char* const    string = &_Data._DecoratedName[1];
            if ( string == nullptr )
                return 0u;

            const size_t length = ::std::strlen(string);
            if ( length == 0 )
                return 0u;

            hasher.update(string, length);
            return hasher.digest();
        }

        // ??8type_info@@QBEHABV0@@Z for msvcrt-msvcr71
        // ??8type_info@@QEBA_NAEBV0@@Z for msvcr80-msvcr120
        // __std_type_info_compare for vcruntime/ucrt
        [[nodiscard]] bool operator==(const exe_type_info& other) const noexcept
        {
            return do_compare(other) == 0;
        }

        // ??9type_info@@QBEHABV0@@Z for msvcrt-msvcr71
        // ??9type_info@@QEBA_NAEBV0@@Z for msvcr80-msvcr120
        // __std_type_info_compare for vcruntime/ucrt
        [[nodiscard]] bool operator!=(const exe_type_info& other) const noexcept
        {
            return do_compare(other) != 0;
        }

        // ?before@type_info@@QBEHABV1@@Z for msvcrt-msvcr100
        // ?before@type_info@@QBE_NABV1@@Z for msvcr110-msvcr120
        // __std_type_info_compare for vcruntime/ucrt
        [[nodiscard]] bool before(const exe_type_info& other) const noexcept
        {
            return do_compare(other) < 0;
        }

        // ?name@type_info@@QBEPBDXZ for msvcrt-msvcr71
        // ?name@type_info@@QBEPBDPAU__type_info_node@@@Z for msvcr80-msvcr120
        // __std_type_info_name for vcruntime/ucrt
        [[nodiscard]] const char* name() const noexcept
        {
#if NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80
            __type_info_node* const root_node_ptr =
    #if NH3API_CHECK_MSVC_DRIVER
                &__type_info_root_node;
    #else
                global_root_node.get();
    #endif
#endif

#if NH3API_CRT_VERSION >= NH3API_CRT_UCRT
            return __std_type_info_name(&_Data, root_node_ptr);
#elif NH3API_CRT_VERSION >= NH3API_CRT_MSVCR80 && NH3API_CRT_VERSION <= NH3API_CRT_MSVCR120
            return nh3api::msvcrt::std_type_info_name_from_node(this, root_node_ptr);
#else
            return nh3api::msvcrt::std_type_info_name(this);
#endif
        }

        [[nodiscard]] const char* raw_name() const noexcept
        {
            return _Data._DecoratedName;
        }

    public:
        virtual void scalar_deleting_destructor(uint8_t flag)
        {
            get_vftable(this)->scalar_deleting_destructor(this, flag);
        }

#if !NH3API_CHECK_MSVC_DRIVER
        inline static nh3api::msvcrt::global_type_info_node global_root_node {};
#endif

    private:
        mutable __std_type_info_data _Data;
};

template<>
struct std::hash<exe_type_info>
{
    public:
    #if defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23
        static
    #endif
        size_t operator()(const exe_type_info& arg)
    #if !(defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23)
        const
    #endif
        noexcept
        {
            return arg.hash_code();
        }
};

template<class T>
struct exe_type_info_address
{
        static inline constexpr uintptr_t address = 0;
};

template<typename T>
#if NH3API_HAS_CPP_ATTRIBUTE(clang::no_specializations)
[[clang::no_specializations("Specialize exe_type_info_address<T> instead.")]]
#endif
inline constexpr uintptr_t exe_type_info_address_v
    = exe_type_info_address<T>::address;

template<typename T>
const exe_type_info& exe_typeid(const T&);

// Note: undocumented MSVC __novtordisp attribute can be useful to make polymorphic class use no vtordisp member
// Note: undocumented MSVC (void*)__vtable_introduced_by(DerivedClass, BaseClass) intrinsic can be used to retrieve the global address of the base class vftable

// call __RTDynamicCast
template<class To, class From>
To* exe_dynamic_cast(From*);
