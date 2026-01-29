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

#include <limits>      // std::numeric_limits
#include <type_traits> // std::underlying_type_t

#if (__cplusplus >= 202002L) || defined(__cpp_lib_ranges)
#include <ranges>      // std::ranges::view_interface
#endif

#include "stl_extras.hpp"

namespace nh3api
{

template<typename EnumT, EnumT MinValue, EnumT MaxValue>
struct enum_limits_base
{
    static_assert(::std::is_enum_v<EnumT>);
    static inline constexpr EnumT min_value = MinValue;
    static inline constexpr EnumT max_value = MaxValue;
};

template<typename EnumT>
struct enum_limits
{ static inline constexpr bool is_specialized = false; };

template<typename T>
[[nodiscard]] inline constexpr bool in_enum_range(const T value) noexcept
#ifdef __cpp_concepts
requires ( enum_limits<T>::is_specialized )
#endif
{
    #ifndef __cpp_concepts
    static_assert(enum_limits<T>::is_specialized);
    #endif
    return (value >= enum_limits<T>::min_value) && (value <= enum_limits<T>::max_value);
}

// similar to std::views::iota, but for integrals and enums that were specialized for enum_limits::min/max
// usage: for (auto artifact : limit_view<TArtifact>())
template<typename T>
class limit_view
#ifdef __cpp_lib_ranges
: public ::std::ranges::view_interface<limit_view<T>>
#endif
{
    static_assert(::std::numeric_limits<T>::is_specialized);

    private:
        using underlying_t = ::std::underlying_type_t<T>;
        inline static constexpr T min_value = enum_limits<T>::is_specialized ? enum_limits<T>::min_value : (::std::numeric_limits<T>::min)();
        inline static constexpr T max_value = enum_limits<T>::is_specialized ? enum_limits<T>::max_value : (::std::numeric_limits<T>::max)();

        class iterator
        {
            public:
                using value_type        = T;
                using difference_type   = ::std::ptrdiff_t;
                using iterator_category = ::std::forward_iterator_tag;

                inline constexpr iterator() noexcept = default;

                inline constexpr explicit iterator(T val) noexcept
                    : current_value { val }, is_end_sentinel { false }
                {}

                inline constexpr T operator*() const noexcept
                { return current_value; }

                inline constexpr iterator& operator++() noexcept
                {
                    if ( current_value == max_value )
                        is_end_sentinel = true;
                    else
                        current_value = static_cast<T>(static_cast<underlying_t>(current_value) + 1);
                    return *this;
                }

                inline constexpr iterator operator++(int) noexcept
                {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                inline constexpr friend bool operator==(const iterator& lhs, const iterator& rhs) noexcept
                {
                    if ( lhs.is_end_sentinel && rhs.is_end_sentinel )
                        return true;
                    if ( lhs.is_end_sentinel != rhs.is_end_sentinel )
                        return false;
                    return lhs.current_value == rhs.current_value;
                }

                inline constexpr friend bool operator!=(const iterator& lhs, const iterator& rhs) noexcept
                { return !(lhs == rhs); }

            private:
                NH3API_NO_UNIQUE_ADDRESS T    current_value   { max_value };
                NH3API_NO_UNIQUE_ADDRESS bool is_end_sentinel { true };
        };

    public:
        [[nodiscard]] inline constexpr iterator begin() const noexcept
        { return iterator { min_value }; }

        [[nodiscard]] inline constexpr iterator end() const noexcept
        { return {}; }
};

} // namespace nh3api
