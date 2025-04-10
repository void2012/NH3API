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

//namespace nh3api
//{

namespace nh3api
{
template<typename K, typename T>
struct map_key_access
{
    const K& operator()(const std::pair<const K, T>& value) const
    {return value.first; }
};
}

/// @brief Visual C++ 6.0 std::map implementation used by heroes3.exe
/// @tparam _K key type
/// @tparam _Ty stored type
/// @tparam _Pr compare predicate
/// @tparam _A allocator type
template<class _K, // key type
         class _Ty, // stored type
         uintptr_t _Nil_Address = 0, // null node address inside .exe
         uintptr_t _Nilrefs_Address = 0, // constructor-destructor reference counter address inside .exe
         typename _Pr = std::less<_K>, // compare predicate
         typename _A = exe_allocator<std::pair<const _K, _Ty> > > // allocator
class exe_map : public nh3api::exe_rbtree<_K,
                                          std::pair<const _K, _Ty>,
                                          nh3api::map_key_access<_K, _Ty>,
                                          _Pr,
                                          _A,
                                          _Nil_Address,
                                          _Nilrefs_Address>
{
public:
    typedef std::pair<const _K, _Ty> value_type;
    typedef nh3api::exe_rbtree<_K,
                               std::pair<const _K, _Ty>,
                               nh3api::map_key_access<_K, _Ty>,
                               _Pr,
                               _A,
                               _Nil_Address,
                               _Nilrefs_Address> base_type NH3API_NODEBUG;
protected:
    class value_compare
    {
        public:
            bool operator()(const value_type& _X,
                const value_type& _Y) const
                {return (comp(_X.first, _Y.first)); }
        public:
            value_compare(_Pr _Pred)
                : comp(_Pred) {}

        public:
            _Pr comp;
    };

public:
    typedef _K  key_type;
    typedef _Ty mapped_type;
    typedef _Pr key_compare;
    typedef _A  allocator_type;
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
    exe_map() NH3API_NOEXCEPT_ALLOC
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        : exe_map(key_compare())
    {}
    #else 
        : base_type(key_compare(), true, allocator_type())
    {}
    #endif

    explicit exe_map(const key_compare &keycomp, const allocator_type& allocator = allocator_type())
    NH3API_NOEXCEPT_ALLOC
        : base_type(keycomp, true, allocator)
    {}

    exe_map(const_iterator first, const_iterator last, const key_compare& keycomp, const allocator_type &allocator = allocator_type())
        : base_type(first, last, keycomp, true, allocator)
    {}

    exe_map(const exe_map& other)
        : base_type(other)
    {}

    exe_map(const exe_map& other, const allocator_type& allocator)
        : base_type(other, allocator)
    {}

    #if NH3API_STD_MOVE_SEMANTICS
    exe_map(exe_map&& other)
    NH3API_NOEXCEPT_EXPR(base_type::_bit_swappable)
        : base_type(std::forward<exe_map>(other))
    {}

    exe_map(exe_map&& other, const allocator_type& allocator)
    NH3API_NOEXCEPT_EXPR(base_type::_bit_swappable)
        : base_type(std::forward<exe_map>(other), allocator)
    {}
    #endif

    // no-op constructor
    exe_map(const nh3api::dummy_tag_t& tag)
        : base_type(tag)
    {}

public:
    mapped_type& operator[](const key_type& key)
    {
        iterator pos = insert(value_type(key, mapped_type())).first;
        return (*pos).second;
    }

    value_compare value_comp() const
    { return (value_compare(this->key_comp())); }

};

#pragma pack(pop)

// clang-format on