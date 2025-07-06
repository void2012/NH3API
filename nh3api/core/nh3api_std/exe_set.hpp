//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
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
    const T& operator()(const T& value) const NH3API_NOEXCEPT
    { return value; }
};
}

/// @brief Visual C++ 6.0 std::set implementation used by heroes3.exe
/// @tparam _K stored type
/// @tparam _Pr compare predicate
/// @tparam _A allocator type
template<class _K, // // key type
         uintptr_t _Nil_Address = 0, // null node address inside .exe
         uintptr_t _Nilrefs_Address = 0, // constructor-destructor reference counter address inside .exe
         typename _Pr = std::less<_K> // compare predicate
        >
class exe_set : public nh3api::exe_rbtree<_K,
                                          _K,
                                          nh3api::set_key_access<_K>,
                                          _Pr,
                                          _Nil_Address,
                                          _Nilrefs_Address>
{
public:
    typedef _K  value_type;
    typedef _Pr value_compare;
    typedef _K  key_type;
    typedef _Pr key_compare;
    typedef nh3api::exe_rbtree<_K,
                               _K,
                               nh3api::set_key_access<_K>,
                               _Pr,
                               _Nil_Address,
                               _Nilrefs_Address> base_type;
    
    typedef typename base_type::allocator_type         allocator_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::node_type node_type;

public:
    exe_set() NH3API_NOEXCEPT
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        : exe_set(key_compare())
    {}
    #else 
        : base_type(key_compare(), false, allocator_type())
    {}
    #endif

    explicit exe_set(const key_compare &keycomp, const allocator_type &allocator = allocator_type())
    NH3API_NOEXCEPT
        : base_type(keycomp, false, allocator)
    {}

    exe_set(const_iterator first, const_iterator last, const key_compare &keycomp, const allocator_type &allocator = allocator_type())
        : base_type(first, last, keycomp, false, allocator)
    {}

    exe_set(const exe_set& other)
        : base_type(other)
    {}

    exe_set(const exe_set& other, const allocator_type &allocator)
        : base_type(other, allocator)
    {}

    #if NH3API_STD_MOVE_SEMANTICS
    exe_set(exe_set&& other)
    NH3API_NOEXCEPT
        : base_type(std::forward<exe_set>(other))
    {}

    exe_set(exe_set&& other, const allocator_type& allocator)
    NH3API_NOEXCEPT
        : base_type(std::forward<exe_set>(other), allocator)
    {}
    #endif

    // no-op constructor
    exe_set(const nh3api::dummy_tag_t& tag)
        : base_type(tag)
    {}

};

#if !NH3API_STD_MOVE_SEMANTICS
template<class _K,
         uintptr_t _Nil_Address, // null node address inside .exe
         uintptr_t _Nilrefs_Address, // constructor-destructor reference counter address inside .exe
         typename _Pr> NH3API_FORCEINLINE
void swap(exe_set<_K, _Nil_Address, _Nilrefs_Address, _Pr>& lhs,
          exe_set<_K, _Nil_Address, _Nilrefs_Address, _Pr>& rhs) // ADL Swap
{ lhs.swap(rhs); }
#endif

#pragma pack(pop)
// clang-format on