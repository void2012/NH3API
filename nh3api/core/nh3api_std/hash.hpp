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

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace nh3api
{
// FNV-1a hash simple implementation
template <typename ResultT, ResultT OffsetBasis, ResultT Prime>
class basic_fnv1a final
{
    static_assert(::std::is_unsigned_v<ResultT>, "need unsigned integer");

    public:
        using result_type = ResultT;
        inline static constexpr result_type offset_basis = OffsetBasis;
        inline static constexpr result_type prime = Prime;

    private:
        result_type state_;

    public:
        constexpr basic_fnv1a() noexcept
            : state_(OffsetBasis)
        {}

        constexpr void update(const char* const data, const size_t size) noexcept
        {
            result_type acc = this->state_;
            for (size_t i = 0; i < size; ++i)
            {
                const size_t next = static_cast<size_t>(data[i]);
                acc = (acc ^ next) * Prime;
            }
            this->state_ = acc;
        }

        constexpr void update(const wchar_t* const data, const size_t size) noexcept
        {
            result_type acc = this->state_;
            for (size_t i = 0; i < size; ++i)
            {
                // we can't use reinterpret_cast in constexpr, so calculate for low and high bytes
                size_t next = static_cast<size_t>(data[i] & 0xFF00U);
                acc = (acc ^ next) * Prime;
                next = static_cast<size_t>(data[i] & 0x00FFU);
                acc = (acc ^ next) * Prime;
            }
            this->state_ = acc;
        }

        constexpr void update(const uint32_t* const data, const size_t size) noexcept
        {
            result_type acc = this->state_;
            for (size_t i = 0; i < size; ++i)
            {
                // we can't use reinterpret_cast in constexpr, so calculate for low and high bytes
                size_t next = static_cast<size_t>(data[i] & 0xFF000000U);
                acc = (acc ^ next) * Prime;
                next = static_cast<size_t>(data[i] & 0x00FF0000U);
                acc = (acc ^ next) * Prime;
                next = static_cast<size_t>(data[i] & 0x0000FF00U);
                acc = (acc ^ next) * Prime;
                next = static_cast<size_t>(data[i] & 0x000000FFU);
                acc = (acc ^ next) * Prime;
            }
            this->state_ = acc;
        }

        [[nodiscard]] constexpr result_type digest() const noexcept
        { return this->state_; }
};

using default_hash = basic_fnv1a<size_t, 2166136261, 16777619>;

[[nodiscard]] inline constexpr size_t hash_string(const char* const str, size_t size) noexcept
{
    default_hash hasher;
    hasher.update(str, size);
    return hasher.digest();
}

[[nodiscard]] inline constexpr size_t hash_string(const wchar_t* const str, size_t size) noexcept
{
    default_hash hasher;
    hasher.update(str, size);
    return hasher.digest();
}

[[nodiscard]] inline constexpr size_t hash_string(::std::string_view str) noexcept
{
    default_hash hasher;
    hasher.update(str.data(), str.size());
    return hasher.digest();
}

[[nodiscard]] inline constexpr size_t hash_string(::std::wstring_view str) noexcept
{
    default_hash hasher;
    hasher.update(str.data(), str.size());
    return hasher.digest();
}

template <size_t size>
[[nodiscard]] inline constexpr size_t hash_string(const char (&str)[size]) noexcept
{
    default_hash hasher;
    // ignore null terminator for string literals
    hasher.update(str, size - 1);
    return hasher.digest();
}

template <size_t size>
[[nodiscard]] inline constexpr size_t hash_string(const wchar_t (&str)[size]) noexcept
{
    default_hash hasher;
    // ignore null terminator for string literals
    hasher.update(str, size - 1);
    return hasher.digest();
}

/*
#ifdef __cpp_lib_ranges
template<::std::ranges::range Range>
size_t hash_range(const Range& range)
{
    TODO: implement it
    using value_t = ::std::ranges::range_value_t<Range>;
    ::std::hash<value_t> hasher;
    size_t result = default_hash::offset_basis; // FNV-1a offset basis
    constexpr size_t salt = 0x9E3779B9; // Golden ratio fractional part multiplied by 2^32
    size_t index = 1;
    for ( const auto& it : range )
    {
        result ^= (hasher(it) + index) * salt;
        ++index;
    }
    return result;

}
#endif
*/

} // namespace nh3api
