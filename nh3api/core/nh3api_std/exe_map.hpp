//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1995
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

#include <functional> // std::less
#include "exe_rbtree.hpp"

// clang-format off
#pragma pack(push,8)

namespace nh3api
{
template<typename K, typename T>
struct map_key_access
{
    NH3API_FORCEINLINE
    const K& operator()(const ::std::pair<const K, T>& value) const noexcept
    { return value.first; }
};
} // namespace nh3api

// Visual C++ 6.0 std::map implementation used by heroes3.exe
template<class K, // key type
         class T, // stored type
         uintptr_t Nil_Address = 0, // null node address inside .exe
         uintptr_t Nilrefs_Address = 0, // constructor-destructor reference counter address inside .exe
         typename BinaryPredicate = std::less<K> // compare predicate
         > 
class exe_map : public nh3api::exe_rbtree<K,
                                          std::pair<const K, T>,
                                          nh3api::map_key_access<K, T>,
                                          BinaryPredicate,
                                          Nil_Address,
                                          Nilrefs_Address>
{
public:
    using value_type = std::pair<const K, T>;
    using base_type  = nh3api::exe_rbtree<K,
                               std::pair<const K, T>,
                               nh3api::map_key_access<K, T>,
                               BinaryPredicate,
                               Nil_Address,
                               Nilrefs_Address>;
protected:
    class value_compare
    {
        public:
            NH3API_FORCEINLINE
            bool operator()(const value_type& lhs, const value_type& rhs) const noexcept
            { return comp(lhs.first, rhs.first); }

        public:
            value_compare(BinaryPredicate pred) noexcept
                : comp(pred) {}

        public:
            BinaryPredicate comp;
    };

public:
    using key_type = K;
    using mapped_type = T;
    using key_compare = BinaryPredicate;
    using allocator_type = typename base_type::allocator_type;
    using size_type = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;
    using reference = typename base_type::reference;
    using const_reference = typename base_type::const_reference;
    using iterator = typename base_type::iterator;
    using const_iterator = typename base_type::const_iterator;
    using reverse_iterator = typename base_type::reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    using node_type = typename base_type::node_type;

public:
    exe_map() noexcept
        : exe_map(key_compare())
    {}

    explicit exe_map(const key_compare &keycomp, const allocator_type& allocator = allocator_type())
    noexcept
        : base_type(keycomp, true, allocator)
    {}

    exe_map(const_iterator first, const_iterator last, const key_compare& keycomp, const allocator_type &allocator = allocator_type())
        : base_type(first, last, keycomp, true, allocator)
    {}

    exe_map(const exe_map& other)
        : base_type(other)
    {}

    exe_map& operator=(const exe_map& other) 
    { base_type::operator=(other); return *this; }

    exe_map(const exe_map& other, const allocator_type& allocator)
        : base_type(other, allocator)
    {}

    exe_map(exe_map&& other) noexcept
        : base_type(std::move<exe_map>(other))
    {}

    exe_map(exe_map&& other, const allocator_type& allocator) noexcept
        : base_type(std::move<exe_map>(other), allocator)
    {}

    exe_map& operator=(exe_map&& other) noexcept 
    { base_type::operator=(other); return *this; }

    // no-op constructor
    exe_map(const ::nh3api::dummy_tag_t& tag)
        : base_type(tag)
    {}

    ~exe_map() = default;

public:
    mapped_type& operator[](const key_type& key)
    {
        iterator pos = this->insert(value_type(key, mapped_type())).first;
        return (*pos).second;
    }

    value_compare value_comp() const
    { return value_compare(this->key_comp()); }

};

#pragma pack(pop)

// clang-format on
