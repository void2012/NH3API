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

#include <array>        // std::array
#include <cassert>      // assert
#include <functional>   // std::less
#ifdef __cpp_lib_ranges
#include <ranges>       // std::default_sentinel_t
#endif
#include <stdexcept>
#include <type_traits>  // std::add_pointer_t, std::conditional_t
#include <utility>      // std::exchange, std::pair, std::swap

#include "iterator.hpp"          // iterator_for_container
#include "memory.hpp"            // exe_allocator, exe_scoped_lock
#include "nh3api_exceptions.hpp" // nh3api::throw_exception

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wnull-dereference")
NH3API_WARNING_MSVC_DISABLE(6011)

namespace nh3api
{

enum class rbtree_type : uint8_t
{
    set_like,
    map_like
};

template <class _Key, class... _Args>
struct in_place_key_extract_set_impl
{
    // by default we can't extract the key in the emplace family and must construct a node we might not use
    inline static constexpr bool _Extractable = false;
};

template <class _Key>
struct in_place_key_extract_set_impl<_Key, _Key> {
    // we can extract the key in emplace if the emplaced type is identical to the key type
    inline static constexpr bool _Extractable = true;
    static const _Key& _Extract(const _Key& _Value) noexcept {
        return _Value;
    }
};

template <class... _Args>
using in_place_key_extract_set = in_place_key_extract_set_impl<::std::remove_const_t<::std::remove_reference_t<_Args>>...>;

// assumes _Args have already been _Remove_const_ref_t'd
template <class _Key, class... _Args>
struct in_place_key_extract_map_impl {
    // by default we can't extract the key in the emplace family and must construct a node we might not use
    static constexpr bool _Extractable = false;
};

template <class _Key, class _Second>
struct in_place_key_extract_map_impl<_Key, _Key, _Second> {
    // if we would call the pair(key, value) constructor family, we can use the first parameter as the key
    static constexpr bool _Extractable = true;
    static const _Key& _Extract(const _Key& _Value, const _Second&) noexcept {
        return _Value;
    }
};

template <class _Key, class _First, class _Second>
struct in_place_key_extract_map_impl<_Key, ::std::pair<_First, _Second>> {
    // if we would call the pair(pair<other, other>) constructor family, we can use the pair.first member as the key
    static constexpr bool _Extractable = ::std::is_same_v<_Key, ::std::remove_const_t<::std::remove_reference_t<_First>>>;
    static const _Key& _Extract(const ::std::pair<_First, _Second>& _Val) noexcept {
        return _Val.first;
    }
};


template <class... _Args>
using in_place_key_extract_map = in_place_key_extract_map_impl<::std::remove_const_t<::std::remove_reference_t<_Args>>...>;

template<class KeyType,
         class ValueType,
         rbtree_type TreeType,
         uintptr_t   NilAddress,
         uintptr_t   NilrefsAddress>
class exe_rbtree;

template <class _Ty, class = void>
inline constexpr bool is_transparent_v = false;

template <class _Ty>
inline constexpr bool is_transparent_v<_Ty, ::std::void_t<typename _Ty::is_transparent>> = true;

template <class _Ty>
struct is_transparent : ::std::bool_constant<is_transparent_v<_Ty>> {};

template<typename DerivedType, class KeyType, class ValueType>
struct node_handle_map_base
{
    public:
        using key_type    = KeyType;
        using mapped_type = ValueType;

    private:
        using pair_type = ::std::pair<key_type, mapped_type>;

    public:
        using allocator_type = ::exe_allocator<pair_type>;

        [[nodiscard]] allocator_type get_allocator() const noexcept
        { return {}; }

        [[nodiscard]] key_type& key() const noexcept
        { return reinterpret_cast<pair_type&>(static_cast<const DerivedType*>(this)->_Getptr()->_Myval).first; }

        [[nodiscard]] mapped_type& mapped() const noexcept
        { return reinterpret_cast<pair_type&>(static_cast<const DerivedType*>(this)->_Getptr()->_Myval).second; }

};

template<typename DerivedType, class ValueType>
struct node_handle_set_base
{
    public:
        using value_type     = ValueType;
        using allocator_type = ::exe_allocator<value_type>;

    public:
        [[nodiscard]] allocator_type get_allocator() const noexcept
        { return {}; }

        [[nodiscard]] value_type& value() const noexcept
        { return static_cast<const DerivedType*>(this)->_Getptr()->_Myval; }

};

template<class _Node, template <class...> class _Base, class... _Types>
class node_handle : _Base<node_handle<_Node, _Base, _Types...>, _Types...>
{
    private:
        using _Nodeptr = ::std::add_pointer_t<_Node>;

    public:
        inline constexpr node_handle() noexcept    = default;
        node_handle(const node_handle&)            = delete;
        node_handle& operator=(const node_handle&) = delete;
        node_handle(node_handle&&)                 = default;

        inline constexpr explicit node_handle(_Nodeptr _Src_ptr) noexcept
            : _Ptr { _Src_ptr }
        {}

        inline ~node_handle() noexcept
        {
            _Node::_Freenode(_Ptr);
            _Ptr = nullptr;
        }

        inline node_handle& operator=(node_handle&& _Other) noexcept
        {
            if ( _Ptr == nullptr )
            {
                _Ptr = ::std::exchange(_Other._Ptr, nullptr);
                return *this;
            }

            if ( _Other._Ptr == nullptr || this == &_Other )
            {
                _Node::_Freenode(_Ptr);
                _Ptr = nullptr;
                return *this;
            }

            _Node::_Freenode(_Ptr);
            _Ptr = ::std::exchange(_Other._Ptr, nullptr);
            return *this;
        }

        explicit operator bool() const noexcept
        { return _Ptr != nullptr; }

        [[nodiscard]] bool empty() const noexcept
        { return _Ptr == nullptr; }

        void swap(node_handle& _Other) noexcept
        {
            using ::std::swap;
            swap(_Ptr, _Other._Ptr); // intentional ADL
        }

        friend void swap(node_handle& _Left, node_handle& _Right) noexcept
        { _Left.swap(_Right); }

    private:
        [[nodiscard]] _Nodeptr _Getptr() const noexcept
        { return _Ptr; }

        // extract the node from *this
        _Nodeptr _Release() noexcept
        { return ::std::exchange(_Ptr, nullptr); }

    private:
        _Nodeptr _Ptr = nullptr;

};

enum class rbtree_child : uint8_t
{
    _Right,
    _Left,
    _Unused
};

#pragma pack(push, 8)
// Implementation of ordered red-black tree for [multi_]{map set}
// from VC6.0 STL (<xtree> std::_Tree)
template<class       KeyType,        // key
         class       ValueType,      // stored type
         rbtree_type TreeType,       // std::set or std::map
         uintptr_t   NilAddress,     // null node address inside .exe
         uintptr_t   NilrefsAddress> // constructor-destructor reference counter address inside .exe
class exe_rbtree
{
    protected:
        // colors for link to parent
        enum _Redbl : uint32_t
        {
            _Red   = 0,
            _Black = 1
        };

        struct _Tree_node;
        friend struct _Tree_node;
        using _Node            = _Tree_node;
        using _Nodeptr         = _Node*;

    public:
        using key_type       = KeyType;
        using value_type     = ValueType;
        using key_compare    = ::std::less<>;
        using allocator_type = ::exe_allocator<value_type>;

        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using node_type       = ::std::conditional_t<TreeType == rbtree_type::map_like,
                                node_handle<_Node, node_handle_map_base, key_type, value_type>,
                                node_handle<_Node, node_handle_set_base, key_type>>;

    protected:
        struct _Tree_node
        {
            [[nodiscard]] bool _Isnil() const noexcept
            { return this == _Getnil(); }

            // free the node
            static void _Freenode0(_Nodeptr _Ptr) noexcept
            { ::operator delete(_Ptr, ::exe_heap); }

            // destroy the value inside of the node and free the node
            static void _Freenode(_Nodeptr _Ptr) noexcept
            {
                ::std::destroy_at(__builtin_addressof(_Ptr->_Myval));
                ::operator delete(_Ptr, ::exe_heap);
            }

            // allocate a head node (sentinel)
            static _Nodeptr _Buyheadnode() noexcept
            {
                _Nodeptr _Pnode = static_cast<_Nodeptr>(::operator new(sizeof(_Tree_node), ::exe_heap));
                assert(_Pnode);
                _Pnode->_Left   = _Pnode;
                _Pnode->_Parent = _Pnode;
                _Pnode->_Right  = _Pnode;
                _Pnode->_Color  = _Black;
                return _Pnode;
            }

            // allocate a regular node
            template <class... _Args>
            static _Nodeptr _Buynode(_Nodeptr _Myhead, _Args&&... _Values) noexcept
            {
                _Nodeptr _Pnode = static_cast<_Nodeptr>(::operator new(sizeof(_Tree_node), ::exe_heap));
                ::new (static_cast<void*>(__builtin_addressof(_Pnode->_Myval))) value_type(::std::forward<_Args>(_Values)...);
                _Pnode->_Left   = _Myhead;
                _Pnode->_Parent = _Myhead;
                _Pnode->_Right  = _Myhead;
                _Pnode->_Color  = _Red;
                return _Pnode;
            }

            _Tree_node* _Left;
            _Tree_node* _Parent;
            _Tree_node* _Right;
            value_type  _Myval;
            _Redbl      _Color;
        };

        struct _Tree_id
        {
            _Nodeptr _Parent;
            rbtree_child _Child;
        };

        struct _Tree_find_result
        {
            _Tree_id _Location;
            _Nodeptr _Bound;
        };

        struct _Tree_find_hint_result
        {
            _Tree_id _Location;
            bool _Duplicate;
        };

        [[nodiscard]] static const key_type& _Key_access(const value_type& _Value) noexcept
        {
            if constexpr ( TreeType == rbtree_type::map_like )
                return _Value.first;
            else
                return _Value;
        }

        // get null node
        static _Nodeptr& _Getnil() noexcept
        { return get_global_var_ref(NilAddress, _Nodeptr); }

        static size_t* _Getnilrefs() noexcept
        { return get_global_var_ptr(NilrefsAddress, size_t); }

    public:
        // iterator for nonmutable exe_tree
        class const_iterator
        {
            protected:
                friend class exe_rbtree;

            protected:
                _Nodeptr _Ptr;

            public:
                using iterator_category = ::std::bidirectional_iterator_tag;
                using value_type        = const ValueType;
                using difference_type   = ptrdiff_t;
                using pointer           = const ValueType*;
                using reference         = const ValueType&;

            public:
                inline const_iterator() noexcept                                 = default;
                inline const_iterator(const const_iterator&) noexcept            = default;
                inline ~const_iterator() noexcept                                = default;
                inline const_iterator(const_iterator&&) noexcept                 = default;
                inline const_iterator& operator=(const_iterator&&) noexcept      = default;
                inline const_iterator& operator=(const const_iterator&) noexcept = default;

                inline explicit const_iterator(_Nodeptr _Src_ptr) noexcept
                    :  _Ptr { _Src_ptr }
                {}

                // return designated value
                [[nodiscard]] inline const value_type& operator*() const noexcept
                { return _Ptr->_Myval; }

                // return pointer to class object
                [[nodiscard]] inline const value_type* operator->() const noexcept
                { return __builtin_addressof(*(*this)); }

                // climb looking for right subtree
                const_iterator& operator++() noexcept
                {
                    if ( _Ptr->_Right->_Isnil() )
                    {                      // climb looking for right subtree
                        _Nodeptr _Pnode = nullptr;
                        while ( !(_Pnode = _Ptr->_Parent)->_Isnil() && _Ptr == _Pnode->_Right )
                            _Ptr = _Pnode; // ==> parent while right subtree

                        _Ptr = _Pnode;     // ==> parent (head if end())
                    }
                    else
                    {
                        _Ptr = _Min(_Ptr->_Right); // ==> smallest of right subtree
                    }

                    return *this;
                }

                // postfix increment
                const_iterator operator++(int) noexcept
                {
                    const_iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                const_iterator& operator--() noexcept
                {
                    if ( _Ptr->_Isnil() )
                    {
                        _Ptr = _Ptr->_Right; // end() ==> rightmost
                    }
                    else if ( _Ptr->_Left->_Isnil() )
                    {                        // climb looking for left subtree
                        _Nodeptr _Pnode = nullptr;
                        while ( !(_Pnode = _Ptr->_Parent)->_Isnil() && _Ptr == _Pnode->_Left )
                            _Ptr = _Pnode;   // ==> parent while left subtree

                        if ( !_Ptr->_Isnil() )
                        {                    // decrement non-begin()
                            _Ptr = _Pnode;   // ==> parent if not head
                        }
                    }
                    else
                    {
                        _Ptr = _Max(_Ptr->_Left); // ==> largest of left subtree
                    }

                    return *this;
                }

                const_iterator operator--(int) noexcept
                {
                    const_iterator temp = *this;
                    --(*this);
                    return temp;
                }

                [[nodiscard]] inline bool operator==(const const_iterator& other) const noexcept
                { return _Ptr == other._Ptr; }

            #ifndef __cpp_impl_three_way_comparison
                [[nodiscard]] inline bool operator!=(const const_iterator& other) const noexcept
                { return !(*this == other); }
            #endif

            #ifdef __cpp_lib_ranges
                [[nodiscard]] bool operator==(::std::default_sentinel_t) const noexcept
                { return _Ptr->_Isnil(); }

                [[nodiscard]] bool operator!=(::std::default_sentinel_t) const noexcept
                { return !_Ptr->_Isnil(); }
            #endif
        };

        class iterator
        {
            protected:
                friend class exe_rbtree;

            protected:
                _Nodeptr _Ptr;

            public:
                using iterator_category = ::std::bidirectional_iterator_tag;
                using value_type        = ValueType;
                using difference_type   = ptrdiff_t;
                using pointer           = ValueType*;
                using reference         = ValueType&;

            public:
                inline iterator() noexcept                           = default;
                inline iterator(const iterator&) noexcept            = default;
                inline ~iterator() noexcept                          = default;
                inline iterator(iterator&&) noexcept                 = default;
                inline iterator& operator=(iterator&&) noexcept      = default;
                inline iterator& operator=(const iterator&) noexcept = default;

                inline explicit iterator(_Nodeptr _Src_ptr) noexcept
                    :  _Ptr { _Src_ptr }
                {}

                [[nodiscard]] inline operator const_iterator() noexcept
                { return { _Ptr}; }

                // return designated value
                [[nodiscard]] inline value_type& operator*() noexcept
                { return _Ptr->_Myval; }

                // return pointer to class object
                [[nodiscard]] inline value_type* operator->() noexcept
                { return __builtin_addressof(*(*this)); }

                // return designated value
                [[nodiscard]] inline const value_type& operator*() const noexcept
                { return _Ptr->_Myval; }

                // return pointer to class object
                [[nodiscard]] inline const value_type* operator->() const noexcept
                { return __builtin_addressof(*(*this)); }

                // climb looking for right subtree
                iterator& operator++() noexcept
                {
                    if ( _Ptr->_Right->_Isnil() )
                    {                      // climb looking for right subtree
                        _Nodeptr _Pnode = nullptr;
                        while ( !(_Pnode = _Ptr->_Parent)->_Isnil() && _Ptr == _Pnode->_Right )
                            _Ptr = _Pnode; // ==> parent while right subtree

                        _Ptr = _Pnode;     // ==> parent (head if end())
                    }
                    else
                    {
                        _Ptr = _Min(_Ptr->_Right); // ==> smallest of right subtree
                    }

                    return *this;
                }

                // postfix increment
                iterator operator++(int) noexcept
                {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                iterator& operator--() noexcept
                {
                    if ( _Ptr->_Isnil() )
                    {
                        _Ptr = _Ptr->_Right; // end() ==> rightmost
                    }
                    else if ( _Ptr->_Left->_Isnil() )
                    {                        // climb looking for left subtree
                        _Nodeptr _Pnode = nullptr;
                        while ( !(_Pnode = _Ptr->_Parent)->_Isnil() && _Ptr == _Pnode->_Left )
                            _Ptr = _Pnode;   // ==> parent while left subtree

                        if ( !_Ptr->_Isnil() )
                        {                    // decrement non-begin()
                            _Ptr = _Pnode;   // ==> parent if not head
                        }
                    }
                    else
                    {
                        _Ptr = _Max(_Ptr->_Left); // ==> largest of left subtree
                    }

                    return *this;
                }

                iterator operator--(int) noexcept
                {
                    iterator temp = *this;
                    --(*this);
                    return temp;
                }

                [[nodiscard]] inline bool operator==(const iterator& other) const noexcept
                { return _Ptr == other._Ptr; }

            #ifndef __cpp_impl_three_way_comparison
                [[nodiscard]] inline bool operator!=(const iterator& other) const noexcept
                { return !(*this == other); }
            #endif

            #ifdef __cpp_lib_ranges
                [[nodiscard]] bool operator==(::std::default_sentinel_t) const noexcept
                { return _Ptr->_Isnil(); }

                [[nodiscard]] bool operator!=(::std::default_sentinel_t) const noexcept
                { return !_Ptr->_Isnil(); }
            #endif
        };

        private:
            struct _Insert_return_type
            {
                iterator  position;
                bool      inserted;
                node_type node;
            };

        public:

        using reverse_iterator       = ::std::reverse_iterator<iterator>;
        using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

        explicit exe_rbtree() noexcept
            : _Myhead { _Tree_node::_Buyheadnode() },
              _Mysize { 0 }
        {
            ::exe_scoped_lock lock;
            _Init_ref_count();
        }

        explicit exe_rbtree(const_pointer _First, const_pointer _Last) noexcept
        {
            insert(_First, _Last);
        }

        exe_rbtree(const exe_rbtree& _Right) noexcept
            : _Myhead{ _Tree_node::_Buyheadnode() },
              _Mysize{ 0 }
        {
            ::exe_scoped_lock lock;
            _Init_ref_count();
            _Copy_nodes(_Right);
        }

        exe_rbtree& operator=(const exe_rbtree& other) noexcept
        {
            if ( this != &other )
            {
                clear();
                _Copy_nodes(other);
            }
            return *this;
        }

        exe_rbtree& operator=(exe_rbtree&& _Right) noexcept
        {
            if ( this != &_Right )
            {
                clear();
                auto& _Dst = reinterpret_cast<::std::array<uint32_t, 4>&>(*this);
                auto& _Src = reinterpret_cast<::std::array<uint32_t, 4>&>(_Right);
                _Dst = ::std::exchange(_Src, {});
            }
            return *this;
        }

        exe_rbtree(exe_rbtree&& _Right) noexcept
        {
            auto& _Dst = reinterpret_cast<::std::array<uint32_t, 4>&>(*this);
            auto& _Src = reinterpret_cast<::std::array<uint32_t, 4>&>(_Right);
            _Dst = ::std::exchange(_Src, {});
        }

        inline explicit exe_rbtree(const dummy_tag_t&) noexcept
        {}

        ~exe_rbtree() noexcept
        {
            clear();
            _Free_ref_count();
        }

        // access
    public:
        [[nodiscard]] iterator begin() noexcept
        { return iterator { _Myhead->_Left }; }

        [[nodiscard]] const_iterator begin() const noexcept
        { return const_iterator { _Myhead->_Left }; }

        [[nodiscard]] const_iterator cbegin() const noexcept
        { return const_iterator { _Myhead->_Left }; }

        [[nodiscard]] iterator end() noexcept
        { return iterator { _Myhead }; }

        [[nodiscard]] const_iterator end() const noexcept
        { return const_iterator { _Myhead }; }

        [[nodiscard]] const_iterator cend() const noexcept
        { return const_iterator { _Myhead }; }

        [[nodiscard]] reverse_iterator rbegin() noexcept
        { return reverse_iterator { end() }; }

        [[nodiscard]] const_reverse_iterator rbegin() const noexcept
        { return const_reverse_iterator { end() }; }

        [[nodiscard]] const_reverse_iterator crbegin() const noexcept
        { return const_reverse_iterator { end() }; }

        [[nodiscard]] reverse_iterator rend() noexcept
        { return reverse_iterator { begin() }; }

        [[nodiscard]] const_reverse_iterator rend() const noexcept
        { return const_reverse_iterator { begin() }; }

        [[nodiscard]] const_reverse_iterator crend() const noexcept
        { return const_reverse_iterator { begin() }; }

        [[nodiscard]] size_t size() const noexcept
        { return _Mysize; }

        [[nodiscard]] inline constexpr static size_t max_size() noexcept
        { return size_t(~0U) / sizeof(value_type); }

        [[nodiscard]] inline bool empty() const noexcept
        { return _Mysize == 0; }

        [[nodiscard]] inline static constexpr allocator_type get_allocator() noexcept
        { return {}; }

        [[nodiscard]] inline static constexpr key_compare key_comp() noexcept
        { return {}; }

        // inserting
    public:
        template<class... _Args>
        ::std::pair<iterator, bool> emplace(_Args&&... _Values)
        {
            using _In_place_key_extractor = ::std::conditional_t<TreeType == rbtree_type::map_like,
                                                                 in_place_key_extract_map<_Args...>,
                                                                 in_place_key_extract_set<_Args...>>;

            _Tree_find_result _Loc;
            _Nodeptr          _Inserted = nullptr;
            if constexpr ( _In_place_key_extractor::_Extractable )
            {
                const auto& _Keyval = _In_place_key_extractor::_Extract(_Values...);
                _Loc                = _Find_lower_bound(_Keyval);
                if ( _Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                    return { _Loc._Bound, false };
                _Check_grow_by_1();
                _Inserted = _Tree_node::_Buynode(_Myhead, ::std::forward<_Args>(_Values)...);
            }
            else
            {
                _Nodeptr    _Newnode = _Tree_node::_Buynode(_Myhead, ::std::forward<_Args>(_Values)...);
                const auto& _Keyval  = _Key_access(_Newnode->_Myval);
                _Loc                 = _Find_lower_bound(_Keyval);
                if ( _Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                    return { _Loc._Bound, false };

                _Check_grow_by_1();
                // nothrow hereafter
                _Inserted = _Newnode;
            }

            return { _Insert_node(_Loc._Location, _Inserted), true };
        }

        template<class... _Args>
        iterator emplace_hint(const_iterator _Hint, _Args&&... _Values)
        {
            using _In_place_key_extractor = ::std::conditional_t<TreeType == rbtree_type::map_like,
                                                                 in_place_key_extract_map<_Args...>,
                                                                 in_place_key_extract_set<_Args...>>;

            _Tree_find_result _Loc;
            _Nodeptr          _Inserted = nullptr;
            if constexpr ( _In_place_key_extractor::_Extractable )
            {
                _Loc = _Find_hint(_Hint._Ptr, _In_place_key_extractor::_Extract(_Values...));
                if ( _Loc._Duplicate )
                    return { _Loc._Location._Parent };

                _Check_grow_by_1();
                _Inserted = _Tree_node::_Buynode(_Myhead, ::std::forward<_Args>(_Values)...);
            }
            else
            {
                _Nodeptr    _Newnode = _Tree_node::_Buynode(_Myhead, ::std::forward<_Args>(_Values)...);
                const auto& _Keyval  = _Key_access(_Newnode->_Myval);
                if ( _Loc._Duplicate )
                    return _Loc._Location._Parent;

                _Check_grow_by_1();
                // nothrow hereafter
                _Inserted = _Newnode;
            }
            return _Insert_node(_Loc._Location, _Inserted);
        }

        ::std::pair<iterator, bool> insert(const value_type& _Value)
        { return emplace(_Value); }

        ::std::pair<iterator, bool> insert(value_type&& _Value)
        { return emplace(::std::move(_Value)); }

        iterator insert(const_iterator _Where, const value_type& _Value)
        { return emplace_hint(_Where, _Value); }

        iterator insert(const_iterator _Where, value_type&& _Value)
        { return emplace_hint(_Where, ::std::move(_Value)); }

    #ifdef __cpp_concepts
        template<::nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, ::std::enable_if_t<::nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        void insert(_Iter _First, _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            for ( ; _First != _Last; ++_First )
                emplace_hint(*_First);
        }

        void insert(::std::initializer_list<value_type> _Ilist)
        { insert(_Ilist.begin(), _Ilist.end()); }

        iterator erase(const_iterator _Where) noexcept
        {
            const_iterator _Next = _Where;
            ++_Next;
            _Tree_node::_Freenode(_Extract(_Where));
            return iterator { _Next._Ptr };
        }

        iterator erase(const_iterator _First, const_iterator _Last) noexcept
        {
            while (_First != _Last)
                erase(_First++);
            return iterator { _Last._Ptr };
        }

        size_t erase(const key_type& _Keyval) noexcept
        {
            auto _Range = _Eqrange(_Keyval);
            size_t _Count = 0;
            while (_Range.first != _Range.second)
            {
                erase(const_iterator(_Range.first));
                _Range = _Eqrange(_Keyval);
                ++_Count;
            }
            return _Count;
        }

    #ifdef __cpp_lib_associative_heterogeneous_erasure
        template <class _Kx>
        requires (!::std::is_convertible_v<_Kx, const_iterator>) && (!::std::is_convertible_v<_Kx, iterator>)
        size_type erase(_Kx&& _Keyval) noexcept(noexcept(_Eqrange(_Keyval)))
        {
            auto _Range = _Eqrange(_Keyval);
            size_t _Count = 0;
            while (_Range.first != _Range.second)
            {
                erase(const_iterator(_Range.first));
                _Range = _Eqrange(_Keyval);
                ++_Count;
            }
            return _Count;
        }
    #endif

        void clear() noexcept
        {
            _Erase_tree(_Myhead->_Parent);
            _Myhead->_Left = _Myhead;
            _Myhead->_Parent = _Myhead;
            _Myhead->_Right = _Myhead;
            _Mysize = 0;
        }

        template <class _Keyty>
        iterator find(const _Keyty& _Keyval) noexcept
        {
            auto _Loc = _Find_lower_bound(_Keyval);
            if (_Lower_bound_duplicate(_Loc._Bound, _Keyval))
                return iterator{_Loc._Bound};
            return end();
        }

        template <class _Keyty>
        const_iterator find(const _Keyty& _Keyval) const noexcept
        {
            auto _Loc = _Find_lower_bound(_Keyval);
            if (_Lower_bound_duplicate(_Loc._Bound, _Keyval))
                return const_iterator(_Loc._Bound);
            return end();
        }

        template <class _Keyty>
        bool contains(const _Keyty& _Keyval) const noexcept
        { return _Lower_bound_duplicate(_Find_lower_bound(_Keyval)._Bound, _Keyval); }

        // returns either 0 or 1 because map and set store unique keys
        template <class _Keyty>
        size_t count(const _Keyty& _Keyval) const noexcept
        { return contains(_Keyval) ? 1 : 0; }

        template <class _Keyty>
        iterator lower_bound(const _Keyty& _Keyval) noexcept
        { return iterator(_Find_lower_bound(_Keyval)._Bound); }

        template<class _Keyty>
        const_iterator lower_bound(const _Keyty& _Keyval) const noexcept
        { return const_iterator { _Find_lower_bound(_Keyval)._Bound }; }

        template<class _Keyty>
        iterator upper_bound(const _Keyty& _Keyval) noexcept
        { return iterator { _Find_upper_bound(_Keyval)._Bound }; }

        template<class _Keyty>
        const_iterator upper_bound(const _Keyty& _Keyval) const noexcept
        { return const_iterator { _Find_upper_bound(_Keyval)._Bound }; }

        template<class _Keyty>
        ::std::pair<iterator, iterator> equal_range(const _Keyty& _Keyval) noexcept
        {
            auto _Range = _Eqrange(_Keyval);
            return { iterator { _Range.first }, iterator { _Range.second } };
        }

        template<class _Keyty>
        ::std::pair<const_iterator, const_iterator> equal_range(const _Keyty& _Keyval) const noexcept
        {
            auto _Range = _Eqrange(_Keyval);
            return { const_iterator { _Range.first }, const_iterator { _Range.second } };
        }

        node_type extract(const_iterator _Where) noexcept
        {
            _Nodeptr _Ptr = _Extract(_Where);
            return { _Ptr };
        }

        node_type extract(const key_type& _Keyval) noexcept
        {
            const_iterator _It = find(_Keyval);
            if (_It == cend())
                return {};

            return extract(_It);
        }

    #ifdef __cpp_lib_associative_heterogeneous_erasure
        template <class _Kx>
        requires (!::std::is_convertible_v<_Kx, const_iterator>) && (!::std::is_convertible_v<_Kx, iterator>)
        node_type extract(const _Kx& _Keyval) noexcept
        {
            const_iterator _It = find(_Keyval);
            if (_It == end())
                return {};

            return extract(_It);
        }
    #endif

        auto insert(node_type&& _Handle) noexcept
        {
            if ( _Handle.empty() )
                return _Insert_return_type { end(), false, {} };

            const _Nodeptr _Attempt_node = _Handle._Getptr();
            const key_type&    _Keyval       = _Key_access(_Attempt_node->_Myval);

            _Tree_find_result _Loc = _Find_lower_bound(_Keyval);
            if ( _Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                return _Insert_return_type { iterator { _Loc._Bound }, false, ::std::move(_Handle) };

            _Check_grow_by_1();

            _Attempt_node->_Left = this->_Myhead;
            // _Attempt_node->_Parent handled in _Insert_node
            _Attempt_node->_Right = this->_Myhead;
            _Attempt_node->_Color = _Red;

            return _Insert_return_type { iterator { _Insert_node(_Loc._Location, _Handle._Release()) }, true, std::move(_Handle) };
        }

        iterator insert(const const_iterator _Hint, node_type&& _Handle)
        {
            if ( _Handle.empty() )
                return end();

            const _Nodeptr  _Attempt_node = _Handle._Getptr();
            const key_type& _Keyval       = _Key_access(_Attempt_node->_Myval);
            const auto      _Loc          = _Find_hint(_Hint._Ptr, _Keyval);
            if ( _Loc._Duplicate )
                return iterator { _Loc._Location._Parent };

            _Check_grow_by_1();

            _Attempt_node->_Left = this->_Myhead;
            // _Attempt_node->_Parent handled in _Insert_node
            _Attempt_node->_Right = this->_Myhead;
            _Attempt_node->_Color = _Red;

            return iterator { _Insert_node(_Loc._Location, _Handle._Release()) };
        }

        void merge(exe_rbtree& _That)
        {
            if ( this == &_That )
                return;

            iterator _First = _That.begin();
            while ( !_First._Ptr->_Isnil )
            {
                const _Nodeptr _Attempt_node = _First._Ptr;
                ++_First;
                _Tree_find_result _Loc;
                const key_type&             _Keyval = _Key_access(_Attempt_node->_Myval);

                _Loc = _Find_lower_bound(_Keyval);
                if ( _Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                    continue;

                _Check_grow_by_1();

                // nothrow hereafter for this iteration
                const _Nodeptr _Extracted = _That._Extract(const_iterator { _Attempt_node });
                _Extracted->_Left         = _Myhead;
                // _Extracted->_Parent handled in _Insert_node
                _Extracted->_Right = _Myhead;
                _Extracted->_Color = _Red;

                const _Nodeptr _Inserted = this->_Insert_node(_Loc._Location, _Extracted);
            }
        }

        void merge(exe_rbtree&& _That)
        { merge(_That); }

        void swap(exe_rbtree& _Other) noexcept
        {
            if ( this != &_Other )
            {
                auto& _Left  = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
                auto& _Right = reinterpret_cast<std::array<uint32_t, 4>&>(_Other);
                using ::std::swap;
                swap(_Left, _Right);
            }
        }

        // implementation
    protected:
        // copy the entire tree from _Right
        void _Copy(const exe_rbtree& _Right)

        {
            _Myhead->_Parent = _Copy_nodes(_Right._Myhead->_Parent, _Myhead);
            _Mysize          = _Right.size();
            if ( !_Myhead->_Parent->_Isnil() )
            {
                // nonempty tree, look for new smallest and largest
                _Myhead->_Left  = _Min(_Myhead->_Parent);
                _Myhead->_Right = _Max(_Myhead->_Parent);
            }
            else
            {
                // empty tree
                _Myhead->_Left  = _Myhead;
                _Myhead->_Right = _Myhead;
            }
        }

        // copy entire subtree, recursively
        _Nodeptr _Copy_nodes(_Nodeptr _Rootnode, _Nodeptr _Wherenode)
        {
            _Nodeptr _Newroot = _Rootnode; // point at nil node
            if ( !_Rootnode->_Isnil() )
            {
                // copy a node, then any subtrees
                _Newroot          = _Buynode(_Rootnode->_Myval); // memorize new root
                _Newroot->_Parent = _Wherenode;
                _Newroot->_Color  = _Rootnode->_Color;

                _Newroot->_Left  = _Copy_nodes(_Rootnode->_Left, _Newroot);
                _Newroot->_Right = _Copy_nodes(_Rootnode->_Right, _Newroot);
            }
            _Wherenode->_Left = const_cast<_Nodeptr>(_Getnil());
            return _Newroot; // return newly constructed tree
        }

        // find range of nodes equivalent to _Keyval
        template <class _Other>
        [[nodiscard]] ::std::pair<_Nodeptr, _Nodeptr> _Eqrange(const _Other& _Keyval) const noexcept
        {
            _Nodeptr _Pnode  = _Myhead->_Parent;
            _Nodeptr _Lonode = _Myhead; // end() if search fails
            _Nodeptr _Hinode = _Myhead; // end() if search fails

            while ( !_Pnode->_Isnil() )
            {
                const key_type& _Nodekey = _Key_access(_Pnode->_Myval);
                if ( _Nodekey < _Keyval )
                {
                    _Pnode = _Pnode->_Right; // descend right subtree
                }
                else
                {
                    if ( _Hinode->_Isnil() && _Nodekey > _Keyval )
                        _Hinode = _Pnode;    // _Pnode greater, remember it

                    _Lonode = _Pnode;
                    _Pnode  = _Pnode->_Left; // descend left subtree
                }
            }

            // continue scan for upper bound
            _Pnode = _Hinode->_Isnil() ? _Myhead->_Parent : _Hinode->_Left;
            while ( !_Pnode->_Isnil() )
            {
                if ( _Keyval < _Key_access(_Pnode->_Myval) )
                {
                    // _Pnode greater than _Keyval, remember it
                    _Hinode = _Pnode;
                    _Pnode  = _Pnode->_Left; // descend left subtree
                }
                else
                {
                    _Pnode = _Pnode->_Right; // descend right subtree
                }
            }

            return { _Lonode, _Hinode };
        }

        template <class _Keyty>
        _Tree_find_result _Find_lower_bound(const _Keyty& _Keyval) const noexcept
        {
            _Tree_find_result _Result{{_Myhead->_Parent, rbtree_child::_Right}, _Myhead};
            _Nodeptr _Trynode = _Result._Location._Parent;

            while (!_Trynode->_Isnil())
            {
                _Result._Location._Parent = _Trynode;
                if ( _Key_access(_Trynode->_Myval) < _Keyval )
                {
                    _Result._Location._Child = rbtree_child::_Right;
                    _Trynode = _Trynode->_Right;
                }
                else
                {
                    _Result._Location._Child = rbtree_child::_Left;
                    _Result._Bound = _Trynode;
                    _Trynode = _Trynode->_Left;
                }
            }
            return _Result;
        }

        template <class _Keyty>
        _Tree_find_result _Find_upper_bound(const _Keyty& _Keyval) const noexcept
        {
            _Tree_find_result _Result{{_Myhead->_Parent, rbtree_child::_Right}, _Myhead};
            _Nodeptr _Trynode = _Result._Location._Parent;

            while (!_Trynode->_Isnil())
            {
                _Result._Location._Parent = _Trynode;
                if ( _Keyval < _Key_access(_Trynode->_Myval) )
                {
                    _Result._Location._Child = rbtree_child::_Left;
                    _Result._Bound = _Trynode;
                    _Trynode = _Trynode->_Left;
                }
                else
                {
                    _Result._Location._Child = rbtree_child::_Right;
                    _Trynode = _Trynode->_Right;
                }
            }
            return _Result;
        }

        template <class _Keyty>
        bool _Lower_bound_duplicate(const _Nodeptr _Bound, const _Keyty& _Keyval) const noexcept
        { return !_Bound->_Isnil() && !key_compare{}(_Keyval, _Key_access(_Bound->_Myval)); }

        template<class _Keyty>
        _Tree_find_hint_result _Find_hint(const _Nodeptr _Hint, const _Keyty& _Keyval) const noexcept
        {
            // insert at end if after last element
            if ( _Hint->_Isnil() )
            {
                // insert at end if greater than last element
                if ( _Myhead->_Parent->_Isnil() || (_Key_access(_Myhead->_Right->_Myval) < _Keyval) )
                    return {
                        { _Myhead->_Right, rbtree_child::_Right },
                        false
                    };
            }
            else if ( _Hint == _Myhead->_Left )
            {
                // insert at beginning if less than first element
                if ( _Keyval < _Key_access(_Hint->_Myval) )
                    return {
                        { _Hint, rbtree_child::_Left },
                        false
                    };
            }
            else if ( _Keyval < _Key_access(_Hint->_Myval) )
            {
                _Nodeptr* const _Prev = (--(const_iterator { _Hint }))._Ptr;
                if ( _Key_access(_Prev->_Myval) < _Keyval )
                {
                    if ( _Prev->_Right->_Isnil() )
                        return {
                            { _Prev, rbtree_child::_Right },
                            false
                        };

                    return {
                        { _Hint, rbtree_child::_Left },
                        false
                    };
                }
            }
            else if ( _Key_access(_Hint->_Myval) < _Keyval )
            {
                _Nodeptr* const _Next = (++(const_iterator { _Hint }))._Ptr;
                if ( _Next->_Isnil() || (_Keyval < _Key_access(_Next->_Myval)) )
                {
                    if ( _Hint->_Right->_Isnil() )
                        return {
                            { _Hint, rbtree_child::_Right },
                            false
                        };
                    return {
                        { _Next, rbtree_child::_Left },
                        false
                    };
                }
            }
            else
            {
                return {
                    { _Hint, rbtree_child::_Right },
                    true
                };
            }

            const auto _Loc = _Find_lower_bound(_Keyval);
            if ( _Lower_bound_duplicate(_Loc._Bound, _Keyval) )
                return {
                    { _Loc._Bound, rbtree_child::_Unused },
                    true
                };

            return { _Loc._Location, false };
        }

        _Nodeptr _Insert_node(const _Tree_id _Loc, _Nodeptr _Newnode) noexcept
        {
            ++_Mysize;
            _Newnode->_Parent = _Loc._Parent;

             // first node in tree, just set head values
            if (_Loc._Parent == _Myhead)
            {
                _Myhead->_Left = _Newnode;
                _Myhead->_Parent = _Newnode;
                _Myhead->_Right = _Newnode;
                _Newnode->_Color = _Black; // the root is black
                return _Newnode;
            }

            // add to right of _Loc._Parent
            if (_Loc._Child == rbtree_child::_Right)
            {
                _Loc._Parent->_Right = _Newnode;
                if (_Loc._Parent == _Myhead->_Right) // remember rightmost node
                    _Myhead->_Right = _Newnode;
            }
            // add to left of _Loc._Parent
            else
            {
                _Loc._Parent->_Left = _Newnode;
                if (_Loc._Parent == _Myhead->_Left) // remember leftmost node
                    _Myhead->_Left = _Newnode;
            }

            // rebalance
            for (_Nodeptr _Pnode = _Newnode; _Pnode->_Parent->_Color == _Red;)
            {
                // fixup red-red in left subtree
                if (_Pnode->_Parent == _Pnode->_Parent->_Parent->_Left)
                {
                    _Tree_node* const _Parent_sibling = _Pnode->_Parent->_Parent->_Right;
                    if (_Parent_sibling->_Color == _Red) // parent's sibling has two red children, blacken both
                    {
                        _Pnode->_Parent->_Color = _Black;
                        _Parent_sibling->_Color = _Black;
                        _Pnode->_Parent->_Parent->_Color = _Red;
                        _Pnode = _Pnode->_Parent->_Parent;
                    }
                    // parent's sibling has red and black children
                    else
                    {
                        // rotate right child to left
                        if (_Pnode == _Pnode->_Parent->_Right)
                        {
                            _Pnode = _Pnode->_Parent;
                            _Lrotate(_Pnode);
                        }
                        _Pnode->_Parent->_Color = _Black; // propagate red up
                        _Pnode->_Parent->_Parent->_Color = _Red;
                        _Rrotate(_Pnode->_Parent->_Parent);
                    }
                }
                // fixup red-red in right subtree
                else
                {
                    _Tree_node* const _Parent_sibling = _Pnode->_Parent->_Parent->_Left;
                    // parent's sibling has two red children, blacken both
                    if (_Parent_sibling->_Color == _Red)
                    {
                        _Pnode->_Parent->_Color = _Black;
                        _Parent_sibling->_Color = _Black;
                        _Pnode->_Parent->_Parent->_Color = _Red;
                        _Pnode = _Pnode->_Parent->_Parent;
                    }
                    // parent's sibling has red and black children
                    else
                    {
                        // rotate left child to right
                        if (_Pnode == _Pnode->_Parent->_Left)
                        {
                            _Pnode = _Pnode->_Parent;
                            _Rrotate(_Pnode);
                        }
                        _Pnode->_Parent->_Color = _Black; // propagate red up
                        _Pnode->_Parent->_Parent->_Color = _Red;
                        _Lrotate(_Pnode->_Parent->_Parent);
                    }
                }
            }

            _Myhead->_Parent->_Color = _Black; // root is always black
            return _Newnode;
        }

        _Nodeptr _Extract(const_iterator _Where) noexcept
        {
            _Nodeptr _Erasednode = _Where._Ptr; // node to erase
            ++_Where;                           // save successor iterator for return

            _Nodeptr _Fixnode       = nullptr;  // the node to recolor as needed
            _Nodeptr _Fixnodeparent = nullptr;  // parent of _Fixnode (which may be nil)
            _Nodeptr _Pnode         = _Erasednode;

            if ( _Pnode->_Left->_Isnil() )
            {
                _Fixnode = _Pnode->_Right; // stitch up right subtree
            }
            else if ( _Pnode->_Right->_Isnil() )
            {
                _Fixnode = _Pnode->_Left;  // stitch up left subtree
            }
            else
            {                              // two subtrees, must lift successor node to replace erased
                _Pnode   = _Where._Ptr;    // _Pnode is successor node
                _Fixnode = _Pnode->_Right; // _Fixnode is only subtree
            }

            if ( _Pnode == _Erasednode )
            {                                                              // at most one subtree, relink it
                _Fixnodeparent = _Erasednode->_Parent;
                if ( !_Fixnode->_Isnil() )
                    _Fixnode->_Parent = _Fixnodeparent;                    // link up

                if ( _Myhead->_Parent == _Erasednode )
                    _Myhead->_Parent = _Fixnode;                           // link down from root
                else if ( _Fixnodeparent->_Left == _Erasednode )
                    _Fixnodeparent->_Left = _Fixnode;                      // link down to left
                else
                    _Fixnodeparent->_Right = _Fixnode;                     // link down to right

                if ( _Myhead->_Left == _Erasednode )
                    _Myhead->_Left = _Fixnode->_Isnil() ? _Fixnodeparent   // smallest is parent of erased node
                                                        : _Min(_Fixnode);  // smallest in relinked subtree

                if ( _Myhead->_Right == _Erasednode )
                    _Myhead->_Right = _Fixnode->_Isnil() ? _Fixnodeparent  // largest is parent of erased node
                                                         : _Max(_Fixnode); // largest in relinked subtree
            }
            else
            {                                                              // erased has two subtrees, _Pnode is successor to erased
                _Erasednode->_Left->_Parent = _Pnode;                      // link left up
                _Pnode->_Left               = _Erasednode->_Left;          // link successor down

                if ( _Pnode == _Erasednode->_Right )
                {
                    _Fixnodeparent = _Pnode;                            // successor is next to erased
                }
                else
                {                                                       // successor further down, link in place of erased
                    _Fixnodeparent = _Pnode->_Parent;                   // parent is successor's
                    if ( !_Fixnode->_Isnil() )
                        _Fixnode->_Parent = _Fixnodeparent;             // link fix up

                    _Fixnodeparent->_Left        = _Fixnode;            // link fix down
                    _Pnode->_Right               = _Erasednode->_Right; // link next down
                    _Erasednode->_Right->_Parent = _Pnode;              // right up
                }

                if ( _Myhead->_Parent == _Erasednode )
                    _Myhead->_Parent = _Pnode;                    // link down from root
                else if ( _Erasednode->_Parent->_Left == _Erasednode )
                    _Erasednode->_Parent->_Left = _Pnode;         // link down to left
                else
                    _Erasednode->_Parent->_Right = _Pnode;        // link down to right

                _Pnode->_Parent = _Erasednode->_Parent;           // link successor up
                ::std::swap(_Pnode->_Color, _Erasednode->_Color); // recolor it
            }

            if ( _Erasednode->_Color == _Black )
            { // erasing black link, must recolor/rebalance tree
                for ( ; _Fixnode != _Myhead->_Parent && _Fixnode->_Color == _Black; _Fixnodeparent = _Fixnode->_Parent )
                {
                    if ( _Fixnode == _Fixnodeparent->_Left )
                    {     // fixup left subtree
                        _Pnode = _Fixnodeparent->_Right;
                        if ( _Pnode->_Color == _Red )
                        { // rotate red up from right subtree
                            _Pnode->_Color         = _Black;
                            _Fixnodeparent->_Color = _Red;
                            _Lrotate(_Fixnodeparent);
                            _Pnode = _Fixnodeparent->_Right;
                        }

                        if ( _Pnode->_Isnil() )
                        {
                            _Fixnode = _Fixnodeparent; // shouldn't happen
                        }
                        else if ( _Pnode->_Left->_Color == _Black
                                  && _Pnode->_Right->_Color == _Black )
                        { // redden right subtree with black children
                            _Pnode->_Color = _Red;
                            _Fixnode       = _Fixnodeparent;
                        }
                        else
                        {     // must rearrange right subtree
                            if ( _Pnode->_Right->_Color == _Black )
                            { // rotate red up from left sub-subtree
                                _Pnode->_Left->_Color = _Black;
                                _Pnode->_Color        = _Red;
                                _Rrotate(_Pnode);
                                _Pnode = _Fixnodeparent->_Right;
                            }

                            _Pnode->_Color         = _Fixnodeparent->_Color;
                            _Fixnodeparent->_Color = _Black;
                            _Pnode->_Right->_Color = _Black;
                            _Lrotate(_Fixnodeparent);
                            break; // tree now recolored/rebalanced
                        }
                    }
                    else
                    {     // fixup right subtree
                        _Pnode = _Fixnodeparent->_Left;
                        if ( _Pnode->_Color == _Red )
                        { // rotate red up from left subtree
                            _Pnode->_Color         = _Black;
                            _Fixnodeparent->_Color = _Red;
                            _Rrotate(_Fixnodeparent);
                            _Pnode = _Fixnodeparent->_Left;
                        }

                        if ( _Pnode->_Isnil() )
                        {
                            _Fixnode = _Fixnodeparent; // shouldn't happen
                        }
                        else if ( _Pnode->_Right->_Color == _Black
                                  && _Pnode->_Left->_Color == _Black )
                        { // redden left subtree with black children
                            _Pnode->_Color = _Red;
                            _Fixnode       = _Fixnodeparent;
                        }
                        else
                        {     // must rearrange left subtree
                            if ( _Pnode->_Left->_Color == _Black )
                            { // rotate red up from right sub-subtree
                                _Pnode->_Right->_Color = _Black;
                                _Pnode->_Color         = _Red;
                                _Lrotate(_Pnode);
                                _Pnode = _Fixnodeparent->_Left;
                            }

                            _Pnode->_Color         = _Fixnodeparent->_Color;
                            _Fixnodeparent->_Color = _Black;
                            _Pnode->_Left->_Color  = _Black;
                            _Rrotate(_Fixnodeparent);
                            break; // tree now recolored/rebalanced
                        }
                    }
                }

                _Fixnode->_Color = _Black; // stopping node is black
            }

            if ( 0 < _Mysize )
                --_Mysize;

            return _Erasednode;
        }

        _Nodeptr _Erase_unchecked(const_iterator _Where) noexcept
        {
            const_iterator _Successor = _Where;
            ++_Successor; // save successor iterator for return
            _Nodeptr _Erasednode = _Extract(_Where); // node to erase
            _Node::_Freenode(_Erasednode); // delete erased node
            return _Successor._Ptr; // return successor nodeptr
        }

        _Nodeptr _Erase_unchecked(const_iterator _First, const_iterator _Last) noexcept
        {
            if ( _First == cbegin() && _Last._Ptr->_Isnil() )
            {
                // erase all
                clear();
                return _Last._Ptr;
            }

            // partial erase, one at a time
            while ( _First != _Last )
                _Erase_unchecked(_First++);

            return _Last._Ptr;
        }

        size_t _Erase(const ::std::pair<_Nodeptr, _Nodeptr> _Where) noexcept
        {
            const const_iterator _First { _Where.first, nullptr };
            const const_iterator _Last  { _Where.second, nullptr };
            const size_t         _Num = static_cast<size_t>(::std::distance(_First, _Last));
            _Erase_unchecked(_First, _Last);
            return _Num;
        }

        static void _Init_ref_count() noexcept
        {
            ::exe_scoped_lock _Lock;
            if ( _Getnil() == nullptr )
            {
                _Getnil()      = static_cast<_Nodeptr>(::operator new(sizeof(_Tree_node), ::exe_heap));
                _Nodeptr& _Nil = _Getnil();
                _Nil->_Left    = nullptr;
                _Nil->_Right   = nullptr;
                _Nil->_Parent  = nullptr;
                _Nil->_Color   = _Black;
            }
            // increment the global _Nilrefs counter
            _InterlockedIncrement(reinterpret_cast<volatile long*>(_Getnilrefs()));
        }

        void _Free_ref_count() noexcept
        {
            exe_scoped_lock _Lock;
            _Node::_Freenode0(_Myhead);
            _Myhead = nullptr;
            _Mysize = 0;
            {
                if ( _InterlockedDecrement(reinterpret_cast<volatile long*>(_Getnilrefs())) == 0 )
                {
                    _Node::_Freenode0(const_cast<_Nodeptr>(_Getnil()));
                    _Getnil() = nullptr;
                }
            }
        }

        void _Tidy() noexcept
        { clear(); }

        void _Check_grow_by_1()
        {
            if ( max_size() == this->_Mysize ) NH3API_UNLIKELY
                ::nh3api::throw_exception<::std::length_error>("map/set too long");
        }

        static _Nodeptr _Max(_Nodeptr _Pnode) noexcept
        {
            while ( !_Pnode->_Right->_Isnil() )
                _Pnode = _Pnode->_Right;

            return _Pnode;
        }

        static _Nodeptr _Min(_Nodeptr _Pnode) noexcept
        {
            while ( !_Pnode->_Left->_Isnil() )
                _Pnode = _Pnode->_Left;

            return _Pnode;
        }

        // promote right node to root of subtree
        void _Lrotate(_Nodeptr _Wherenode) noexcept
        {
            _Nodeptr _Pnode  = _Wherenode->_Right;
            _Wherenode->_Right = _Pnode->_Left;

            if ( !_Pnode->_Left->_Isnil() )
                _Pnode->_Left->_Parent = _Wherenode;

            _Pnode->_Parent = _Wherenode->_Parent;

            if ( _Wherenode == _Myhead->_Parent )
                _Myhead->_Parent = _Pnode;
            else if ( _Wherenode == _Wherenode->_Parent->_Left )
                _Wherenode->_Parent->_Left = _Pnode;
            else
                _Wherenode->_Parent->_Right = _Pnode;

            _Pnode->_Left       = _Wherenode;
            _Wherenode->_Parent = _Pnode;
        }

        // promote left node to root of subtree
        void _Rrotate(_Nodeptr _Wherenode) noexcept
        {
            _Nodeptr _Pnode = _Wherenode->_Left;
            _Wherenode->_Left = _Pnode->_Right;

            if ( !_Pnode->_Right->_Isnil() )
                _Pnode->_Right->_Parent = _Wherenode;

            _Pnode->_Parent = _Wherenode->_Parent;

            if ( _Wherenode == _Myhead->_Parent )
                _Myhead->_Parent = _Pnode;
            else if ( _Wherenode == _Wherenode->_Parent->_Right )
                _Wherenode->_Parent->_Right = _Pnode;
            else
                _Wherenode->_Parent->_Left = _Pnode;

            _Pnode->_Right      = _Wherenode;
            _Wherenode->_Parent = _Pnode;
        }

        template<typename... _Args>
        _Nodeptr _Buynode(_Args... _Values) noexcept
        { return _Tree_node::_Buynode(this->_Myhead, ::std::forward<_Args>(_Values)...); }

        void _Erase_tree(_Nodeptr _Root) noexcept
        {
            while (!_Root->_Isnil())
            {
                _Erase_tree(_Root->_Right);
                _Tree_node::_Freenode(::std::exchange(_Root, _Root->_Left));
            }
        }

        uint32_t : 32;
        _Nodeptr _Myhead; // pointer to head node
        uint32_t : 32;
        size_t   _Mysize; // number of elements
} NH3API_MSVC_LAYOUT;

#pragma pack(pop) // 8

} // namespace nh3api

NH3API_WARNING(pop)
