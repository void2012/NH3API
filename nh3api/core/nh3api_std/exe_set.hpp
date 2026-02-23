//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */


#pragma once

#include <algorithm>      // std::equal
#include <functional>     // std::less

#include "exe_rbtree.hpp" // exe_rbtree
#include "stl_extras.hpp" // three-way comparison

// clang-format off
#pragma pack(push,8)
// VC6.0 std::set implementation used by the .exe
template<class T,                  // key type
         uintptr_t Nil_Address,    // null node address inside .exe
         uintptr_t Nilrefs_Address // constructor-destructor reference counter address inside .exe
        >
class exe_set : public nh3api::exe_rbtree<T,
                                          T,
                                          nh3api::rbtree_type::set_like,
                                          Nil_Address,
                                          Nilrefs_Address>
{
public:
    using value_type    = T;
    using value_compare = std::less<>;
    using key_type      = T;
    using key_compare   = std::less<>;
    using base_type     = nh3api::exe_rbtree<T,
                                             T,
                                             nh3api::rbtree_type::set_like,
                                             Nil_Address,
                                             Nilrefs_Address>;

    using allocator_type         = typename base_type::allocator_type;
    using size_type              = typename base_type::size_type;
    using difference_type        = typename base_type::difference_type;
    using reference              = typename base_type::reference;
    using const_reference        = typename base_type::const_reference;
    using iterator               = typename base_type::iterator;
    using const_iterator         = typename base_type::const_iterator;
    using reverse_iterator       = typename base_type::reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    using node_type              = typename base_type::node_type;

public:
    exe_set() noexcept = default;
    exe_set& operator=(const exe_set&) = default;
    exe_set& operator=(exe_set&&) noexcept = default;

    exe_set(const_iterator first, const_iterator last)
        : base_type(first, last)
    {}

    exe_set(const exe_set& other)
        : base_type(other)
    {}

    exe_set(exe_set&& other) noexcept
        : base_type(std::move<exe_set>(other))
    {}

    // no-op constructor
    exe_set(const nh3api::dummy_tag_t& tag) noexcept
        : base_type(tag)
    {}

    ~exe_set() noexcept = default;

};

#pragma pack(pop)
// clang-format on

template<class T, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator==(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return _Left.size() == _Right.size() && std::equal(_Left.cbegin(), _Left.cend(), _Right.cbegin()); }

#ifdef __cpp_lib_three_way_comparison
template<class T, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
[[nodiscard]] inline nh3api::synth_three_way_result<T>
operator<=>(const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
            const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return std::lexicographical_compare_three_way(_Left.cbegin(), _Left.cend(), _Right.cbegin(), _Right.cend(), nh3api::synth_three_way); }
#else // C++20 three-way comparison
template<class KeyType, class T, class Kfn, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator!=(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Left == _Right); }

template<class KeyType, class T, class Kfn, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator<(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return std::lexicographical_compare(_Left.cbegin(), _Left.cend(), _Right.cbegin(), _Right.cend()); }

template<class KeyType, class T, class Kfn, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator>(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return _Right < _Left; }

template<class KeyType, class T, class Kfn, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator<=(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Right < _Left); }

template<class KeyType, class T, class Kfn, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator>=(
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Left,
    const exe_set<T, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Left < _Right); }
#endif // C++20 three-way comparison
