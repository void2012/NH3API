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

#include <algorithm>      // std::equal
#include <functional>     // std::less
#include <tuple>          // std::tuple, std::forward_as_tuple
#include <utility>        // std::pair, std::tuple, std::piecewise_construct

#include "exe_rbtree.hpp" // exe_rbtree
#include "stl_extras.hpp" // three-way comparison

#pragma pack(push, 8)
// Visual C++ 6.0 std::map implementation used by heroes3.exe
template<class KeyType,                // key type
         class ValueType,              // stored type
         uintptr_t NilAddress     = 0, // null node address inside .exe
         uintptr_t NilrefsAddress = 0  // constructor-destructor reference counter address inside .exe
>
class exe_map : public nh3api::exe_rbtree<KeyType,
                                          std::pair<const KeyType, ValueType>,
                                          nh3api::rbtree_type::map_like,
                                          NilAddress,
                                          NilrefsAddress>
{
    public:
        using value_type = std::pair<const KeyType, ValueType>;
        using base_type  = nh3api::exe_rbtree<KeyType,
                                              std::pair<const KeyType, ValueType>,
                                              nh3api::rbtree_type::map_like,
                                              NilAddress,
                                              NilrefsAddress>;

    protected:
        struct value_compare
        {
            #if defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23
                static
            #endif
                inline bool operator()(const value_type& _Left, const value_type& _Right)
            #if !(defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23)
                const
            #endif
                noexcept
                { return _Left.first < _Right.first; }
        };

    public:
        using key_type               = KeyType;
        using mapped_type            = ValueType;
        using key_compare            = std::less<key_type>;
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
        exe_map() noexcept = default;

        exe_map(const_iterator first, const_iterator last)
            : base_type(first, last)
        {}

        exe_map(const exe_map& other)                = default;
        exe_map& operator=(const exe_map& other)     = default;
        exe_map(exe_map&& other) noexcept            = default;
        exe_map& operator=(exe_map&& other) noexcept = default;

        // no-op constructor
        exe_map(const nh3api::dummy_tag_t& tag)
            : base_type(tag)
        {}

        inline ~exe_map() noexcept = default;

    public:
        mapped_type& operator[](const key_type& key)
        {
            iterator pos = this->insert(value_type(key, mapped_type())).first;
            return (*pos).second;
        }

        value_compare value_comp() const noexcept
        { return value_compare(this->key_comp()); }

        using base_type::insert;

        template <class _Valty, std::enable_if_t<std::is_constructible_v<value_type, _Valty>, int> = 0>
        std::pair<iterator, bool> insert(_Valty&& _Val)
        { return this->emplace(std::forward<_Valty>(_Val)); }

        template <class _Valty, std::enable_if_t<std::is_constructible_v<value_type, _Valty>, int> = 0>
        iterator insert(const_iterator _Where, _Valty&& _Val)
        { return this->emplace_hint(_Where, std::forward<_Valty>(_Val)); }

    private:
        using _Nodeptr = typename base_type::_Nodeptr;

        template<class _Keyty, class... _Mappedty>
        std::pair<_Nodeptr, bool> _Try_emplace(_Keyty&& _Keyval, _Mappedty&&... _Mapval)
        {
            const auto _Loc = this->_Find_lower_bound(_Keyval);
            if ( this->_Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                return { _Loc._Bound, false };

            this->_Check_grow_by_1();

            const auto _Inserted = base_type::_Tree_node::_Buynode(this->_Myhead, std::piecewise_construct,
                                                                   std::forward_as_tuple(std::forward<_Keyty>(_Keyval)),
                                                                   std::forward_as_tuple(std::forward<_Mappedty>(_Mapval)...));

            // nothrow hereafter
            return { _Insert_node(_Loc._Location, _Inserted), true };
        }

        template<class _Keyty, class... _Mappedty>
        _Nodeptr _Try_emplace_hint(const _Nodeptr _Hint, _Keyty&& _Keyval, _Mappedty&&... _Mapval)
        {
            const auto _Loc = this->_Find_hint(_Hint, _Keyval);
            if ( _Loc._Duplicate )
                return _Loc._Location._Parent;

            this->_Check_grow_by_1();

            const auto _Inserted = base_type::_Tree_node::_Buynode(this->_Myhead, std::piecewise_construct,
                                                                   std::forward_as_tuple(std::forward<_Keyty>(_Keyval)),
                                                                   std::forward_as_tuple(std::forward<_Mappedty>(_Mapval)...));

            // nothrow hereafter
            return _Insert_node(_Loc._Location, _Inserted);
        }

    public:
        template<class... _Mappedty>
        std::pair<iterator, bool> try_emplace(const key_type& _Keyval, _Mappedty&&... _Mapval)
        {
            const auto _Result = _Try_emplace(_Keyval, std::forward<_Mappedty>(_Mapval)...);
            return { iterator { _Result.first }, _Result.second };
        }

        template<class... _Mappedty>
        iterator try_emplace(const const_iterator _Hint, const key_type& _Keyval, _Mappedty&&... _Mapval)
        { return _Try_emplace_hint(_Hint._Ptr, _Keyval, std::forward<_Mappedty>(_Mapval)...); }

        template<class... _Mappedty>
        std::pair<iterator, bool> try_emplace(key_type&& _Keyval, _Mappedty&&... _Mapval)
        {
            const auto _Result = _Try_emplace(std::move(_Keyval), std::forward<_Mappedty>(_Mapval)...);
            return { iterator { _Result.first }, _Result.second };
        }

        template<class... _Mappedty>
        iterator try_emplace(const const_iterator _Hint, key_type&& _Keyval, _Mappedty&&... _Mapval)
        { return iterator { _Try_emplace_hint(_Hint._Ptr, std::move(_Keyval), std::forward<_Mappedty>(_Mapval)...) }; }

    private:
        template <class _Keyty, class _Mappedty>
        std::pair<_Nodeptr, bool> _Insert_or_assign(_Keyty&& _Keyval, _Mappedty&& _Mapval) {
        const auto _Loc = this->_Find_lower_bound(_Keyval);
        if (this->_Lower_bound_duplicate(_Loc._Bound, _Keyval)) {
            _Loc._Bound->_Myval.second = std::forward<_Mappedty>(_Mapval);
            return {_Loc._Bound, false};
        }

        this->_Check_grow_by_1();

        const auto _Inserted = base_type::_Tree_node::_Buynode(this->_Myhead, std::forward<_Keyty>(_Keyval), std::forward<_Mappedty>(_Mapval));

        // nothrow hereafter
        return { _Insert_node(_Loc._Location, _Inserted), true};
    }

    template <class _Keyty, class _Mappedty>
    _Nodeptr _Insert_or_assign_hint(const _Nodeptr _Hint, _Keyty&& _Keyval, _Mappedty&& _Mapval) {
        const auto _Loc = this->_Find_hint(_Hint, _Keyval);
        if (_Loc._Duplicate) {
            _Loc._Location._Parent->_Myval.second = std::forward<_Mappedty>(_Mapval);
            return _Loc._Location._Parent;
        }

        this->_Check_grow_by_1();

        const auto _Inserted = base_type::_Tree_node::_Buynode(this->_Myhead, std::forward<_Keyty>(_Keyval), std::forward<_Mappedty>(_Mapval));

        // nothrow hereafter
        return _Insert_node(_Loc._Location, _Inserted);
    }

public:
    template<class _Mappedty>
    std::pair<iterator, bool> insert_or_assign(const key_type& _Keyval, _Mappedty&& _Mapval)
    {
        const auto _Result = _Insert_or_assign(_Keyval, std::forward<_Mappedty>(_Mapval));
        return { iterator { _Result.first }, _Result.second };
    }

    template<class _Mappedty>
    iterator insert_or_assign(const const_iterator _Hint, const key_type& _Keyval, _Mappedty&& _Mapval)
    { return _Insert_or_assign_hint(_Hint._Ptr, _Keyval, std::forward<_Mappedty>(_Mapval)); }

    template<class _Mappedty>
    std::pair<iterator, bool> insert_or_assign(key_type&& _Keyval, _Mappedty&& _Mapval)
    {
        const auto _Result = _Insert_or_assign(std::move(_Keyval), std::forward<_Mappedty>(_Mapval));
        return { iterator(_Result.first), _Result.second };
    }

    template<class _Mappedty>
    iterator insert_or_assign(const const_iterator _Hint, key_type&& _Keyval, _Mappedty&& _Mapval)
    { return _Insert_or_assign_hint(_Hint._Ptr, std::move(_Keyval), std::forward<_Mappedty>(_Mapval)); }

} NH3API_MSVC_LAYOUT;

#pragma pack(pop)

template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator==(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return _Left.size() == _Right.size() && std::equal(_Left.cbegin(), _Left.cend(), _Right.cbegin()); }

#ifdef __cpp_lib_three_way_comparison
template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
[[nodiscard]] inline nh3api::synth_three_way_result<std::pair<const KeyType, ValueType>>
operator<=>(const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
            const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return std::lexicographical_compare_three_way(_Left.cbegin(), _Left.cend(), _Right.cbegin(), _Right.cend(), nh3api::synth_three_way); }
#else // C++20 three-way comparison
template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator!=(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Left == _Right); }

template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator<(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return std::lexicographical_compare(_Left.cbegin(), _Left.cend(), _Right.cbegin(), _Right.cend()); }

template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator>(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return _Right < _Left; }

template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator<=(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Right < _Left); }

template<class KeyType, class ValueType, uintptr_t Nil_Address, uintptr_t Nilrefs_Address>
inline bool operator>=(
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Left,
    const exe_map<KeyType, ValueType, Nil_Address, Nilrefs_Address>& _Right) noexcept
{ return !(_Left < _Right); }
#endif // C++20 three-way comparison

