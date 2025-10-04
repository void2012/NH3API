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
    const K& operator()(const ::std::pair<const K, T>& value) const NH3API_NOEXCEPT
    { return value.first; }
};
} // namespace nh3api

// Visual C++ 6.0 std::map implementation used by heroes3.exe
template<class _K, // key type
         class _Ty, // stored type
         uintptr_t _Nil_Address = 0, // null node address inside .exe
         uintptr_t _Nilrefs_Address = 0, // constructor-destructor reference counter address inside .exe
         typename _Pr = std::less<_K> // compare predicate
         > 
class exe_map : public nh3api::exe_rbtree<_K,
                                          std::pair<const _K, _Ty>,
                                          nh3api::map_key_access<_K, _Ty>,
                                          _Pr,
                                          _Nil_Address,
                                          _Nilrefs_Address>
{
public:
    typedef std::pair<const _K, _Ty> value_type;
    typedef nh3api::exe_rbtree<_K,
                               std::pair<const _K, _Ty>,
                               nh3api::map_key_access<_K, _Ty>,
                               _Pr,
                               _Nil_Address,
                               _Nilrefs_Address> base_type;
protected:
    class value_compare
    {
        public:
            NH3API_FORCEINLINE
            bool operator()(const value_type& lhs, const value_type& rhs) const NH3API_NOEXCEPT
            { return comp(lhs.first, rhs.first); }

        public:
            value_compare(_Pr pred) NH3API_NOEXCEPT
                : comp(pred) {}

        public:
            _Pr comp;
    };

public:
    typedef _K  key_type;
    typedef _Ty mapped_type;
    typedef _Pr key_compare;
    typedef typename base_type::allocator_type         allocator_type;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::node_type              node_type;

public:
    exe_map() NH3API_NOEXCEPT
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        : exe_map(key_compare())
    {}
    #else 
        : base_type(key_compare(), true, allocator_type())
    {}
    #endif

    explicit exe_map(const key_compare &keycomp, const allocator_type& allocator = allocator_type())
    NH3API_NOEXCEPT
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

    #if NH3API_STD_MOVE_SEMANTICS
    exe_map(exe_map&& other) NH3API_NOEXCEPT
        : base_type(std::move<exe_map>(other))
    {}

    exe_map(exe_map&& other, const allocator_type& allocator) NH3API_NOEXCEPT
        : base_type(std::move<exe_map>(other), allocator)
    {}

    exe_map& operator=(exe_map&& other) NH3API_NOEXCEPT 
    { base_type::operator=(other); return *this; }
    #endif

    // no-op constructor
    exe_map(const ::nh3api::dummy_tag_t& tag)
        : base_type(tag)
    {}

    ~exe_map(){}

public:
    mapped_type& operator[](const key_type& key)
    {
        iterator pos = this->insert(value_type(key, mapped_type())).first;
        return (*pos).second;
    }

    value_compare value_comp() const
    { return (value_compare(this->key_comp())); }

};

#pragma pack(pop)

#if !NH3API_STD_MOVE_SEMANTICS
template<class _K,
         class _Ty,
         uintptr_t _Nil_Address, // null node address inside .exe
         uintptr_t _Nilrefs_Address, // constructor-destructor reference counter address inside .exe
         typename _Pr> NH3API_FORCEINLINE
void swap(exe_map<_K, _Ty, _Nil_Address, _Nilrefs_Address, _Pr>& lhs,
          exe_map<_K, _Ty, _Nil_Address, _Nilrefs_Address, _Pr>& rhs) // ADL Swap
{ lhs.swap(rhs); }
#endif

// clang-format on
