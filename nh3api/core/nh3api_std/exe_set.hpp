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

#include <functional> // std::less
#include "exe_rbtree.hpp"

// clang-format off
#pragma pack(push,8)

namespace nh3api
{

template<typename T>
struct set_key_access
{
    NH3API_FORCEINLINE
    const T& operator()(const T& value) const noexcept
    { return value; }
};

}

// Visual C++ 6.0 std::set implementation used by heroes3.exe
template<class T, // // key type
         uintptr_t Nil_Address = 0, // null node address inside .exe
         uintptr_t Nilrefs_Address = 0, // constructor-destructor reference counter address inside .exe
         typename BinaryPredicate = std::less<T> // compare predicate
        >
class exe_set : public nh3api::exe_rbtree<T,
                                          T,
                                          nh3api::set_key_access<T>,
                                          BinaryPredicate,
                                          Nil_Address,
                                          Nilrefs_Address>
{
public:
    using value_type = T;
    using value_compare = BinaryPredicate;
    using key_type = T;
    using key_compare = BinaryPredicate;
    using base_type = nh3api::exe_rbtree<T,
                               T,
                               nh3api::set_key_access<T>,
                               BinaryPredicate,
                               Nil_Address,
                               Nilrefs_Address>;

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
    exe_set() noexcept
        : exe_set(key_compare())
    {}

    explicit exe_set(const key_compare &keycomp, const allocator_type &allocator = allocator_type())
    noexcept
        : base_type(keycomp, false, allocator)
    {}

    explicit exe_set(const_iterator first, const_iterator last, const key_compare &keycomp, const allocator_type &allocator = allocator_type())
        : base_type(first, last, keycomp, false, allocator)
    {}

    explicit exe_set(const exe_set& other)
        : base_type(other)
    {}

    explicit exe_set(const exe_set& other, const allocator_type &allocator)
        : base_type(other, allocator)
    {}

    explicit exe_set(exe_set&& other) noexcept
        : base_type(std::move<exe_set>(other))
    {}

    explicit exe_set(exe_set&& other, const allocator_type& allocator) noexcept
        : base_type(std::move<exe_set>(other), allocator)
    {}

    // no-op constructor
    exe_set(const ::nh3api::dummy_tag_t& tag) noexcept
        : base_type(tag)
    {}

};

#pragma pack(pop)
// clang-format on
