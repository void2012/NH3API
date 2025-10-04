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

#ifdef __has_include 
    #if __has_include(<string_view>)
        #if defined(_MSVC_LANG)
            #if _MSVC_LANG>=201702L
                #include <string_view>
            #endif
        #elif __cplusplus >=201702L
            #include <string_view>
        #endif
    #endif
#endif

#include "type_traits.hpp" // is_unsigned
#include "intrin.hpp" // strlen_constexpr

namespace nh3api 
{
// FNV-1a hash simple implementation
template <typename ResultT, ResultT OffsetBasis, ResultT Prime>
class basic_fnv1a NH3API_FINAL 
{
    NH3API_STATIC_ASSERT("need unsigned integer", tt::is_unsigned<ResultT>::value);
public:
    typedef ResultT result_type;

private:
    result_type state_;

public:
    NH3API_CONSTEXPR basic_fnv1a() NH3API_NOEXCEPT 
        : state_(OffsetBasis)
        {}

    NH3API_CONSTEXPR_CPP_14 void update(const char* const data, const size_t size) NH3API_NOEXCEPT 
    {
        result_type acc = this->state_;
        for (size_t i = 0; i < size; ++i) 
        {
            const size_t next = static_cast<size_t>(data[i]);
            acc = (acc ^ next) * Prime;
        }
        this->state_ = acc;
    }

    NH3API_CONSTEXPR_CPP_14 void update(const wchar_t* const data, const size_t size) NH3API_NOEXCEPT 
    {
        result_type acc = this->state_;
        for (size_t i = 0; i < size; ++i) 
        {
            // we can't use reinterpret_cast in constexpr, so calculate for low and high bytes
            size_t next = static_cast<size_t>(data[i] & 0xFF00);
            acc = (acc ^ next) * Prime;
            next = static_cast<size_t>(data[i] & 0x00FF);
            acc = (acc ^ next) * Prime;
        }
        this->state_ = acc;
    }

    NH3API_CONSTEXPR_CPP_14 void update(const uint32_t* const data, const size_t size) NH3API_NOEXCEPT 
    {
        result_type acc = this->state_;
        for (size_t i = 0; i < size; ++i) 
        {
            // we can't use reinterpret_cast in constexpr, so calculate for low and high bytes
            size_t next = static_cast<size_t>(data[i] & 0xFF000000);
            acc = (acc ^ next) * Prime;
            next = static_cast<size_t>(data[i] & 0x00FF0000);
            acc = (acc ^ next) * Prime;
            next = static_cast<size_t>(data[i] & 0x0000FF00);
            acc = (acc ^ next) * Prime;
            next = static_cast<size_t>(data[i] & 0x000000FF);
            acc = (acc ^ next) * Prime;
        }
        this->state_ = acc;
    }

    NH3API_NODISCARD NH3API_CONSTEXPR result_type digest() const NH3API_NOEXCEPT 
    { return this->state_; }
};

typedef basic_fnv1a<size_t, 2166136261u, 16777619u> default_hash;

NH3API_NODISCARD NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE 
size_t hash_string(const char* const str, size_t size) NH3API_NOEXCEPT 
{
    default_hash hasher;
    hasher.update(str, size);
    return hasher.digest();
}

NH3API_NODISCARD NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE
size_t hash_string(const wchar_t* const str, size_t size) NH3API_NOEXCEPT 
{
    default_hash hasher;
    hasher.update(str, size);
    return hasher.digest();
}

#ifdef __cpp_lib_string_view
NH3API_NODISCARD constexpr NH3API_FORCEINLINE size_t hash_string(::std::string_view str) noexcept 
{
    default_hash hasher;
    hasher.update(str.data(), str.size());
    return hasher.digest();
}

NH3API_NODISCARD constexpr NH3API_FORCEINLINE size_t hash_string(::std::wstring_view str) noexcept 
{
    default_hash hasher;
    hasher.update(str.data(), str.size());
    return hasher.digest();
}
#endif
template <size_t size> NH3API_NODISCARD 
NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE size_t hash_string(const char (&str)[size]) NH3API_NOEXCEPT
{
    default_hash hasher;
    // ignore null terminator for string literals
    hasher.update(str, size - 1);
    return hasher.digest();
}

template <size_t size>
NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE size_t hash_string(const wchar_t (&str)[size]) NH3API_NOEXCEPT
{
    default_hash hasher;
    // ignore null terminator for string literals
    hasher.update(str, size - 1);
    return hasher.digest();
}

} // namespace nh3api
