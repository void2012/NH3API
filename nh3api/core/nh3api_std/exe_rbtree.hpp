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

#include <utility>      // std::pair, std::swap
#include "iterator.hpp" // nh3api::container_iterator
#include "memory.hpp"   // nh3api::allocator_adaptor, exe_scoped_lock

NH3API_DISABLE_WARNING_BEGIN("-Wnull-dereference", 6011)

namespace nh3api
{
#pragma pack(push, 8)
// ordered red-black tree for [multi_]{map set}
template <class _K,   // key
          class _Ty,  // stored type
          class _Kfn, // key access unary function
          class _Pr,  // compare binary predicate
          uintptr_t _Nil_Address, // null node address inside .exe
          uintptr_t _Nilrefs_Address> // constructor-destructor reference counter address inside .exe
class exe_rbtree
{
  protected:
    // colors for link to parent
    enum _Redbl
    {
        _Red,
        _Black
    };

    struct _Node;
    friend struct _Node;
    typedef _Node* _Nodeptr;
    struct _Node
    {
        _Node* _Left;
        _Node* _Parent;
        _Node* _Right;
        _Ty    _Value;
        _Redbl _Color;
    };
    typedef _Nodeptr& _Nodepref;
    typedef const _K& _Keyref;
    typedef _Redbl&   _Rbref;
    typedef _Ty&      _Vref;

    // return reference to color in node
    static _Rbref _Color(_Nodeptr _P)
    {
        return ((_Rbref)(*_P)._Color);
    }

    // return reference to key in node
    static _Keyref _Key(_Nodeptr _P)
    {
        return (_Kfn()(_Value(_P)));
    }

    // get null node
    static volatile _Node*& _Getnil()
    {
        return *reinterpret_cast<volatile _Node**>(_Nil_Address);
    }

    static volatile size_t* _Getnilrefs()
    {
        return reinterpret_cast<volatile size_t*>(_Nilrefs_Address);
    }

    // is node null?
    static bool _Isnil(const _Nodeptr _P)
    { return _P == *reinterpret_cast<volatile _Nodeptr*>(_Nil_Address); }

    // return reference to left pointer in node
    static _Nodepref _Left(_Nodeptr _P)
    {
        return ((_Nodepref)(*_P)._Left);
    }

    // return reference to parent pointer in node
    static _Nodepref _Parent(_Nodeptr _P)
    {
        return ((_Nodepref)(*_P)._Parent);
    }

    // return reference to right pointer in node
    static _Nodepref _Right(_Nodeptr _P)
    {
        return ((_Nodepref)(*_P)._Right);
    }

    // return reference to value in node
    static _Vref _Value(_Nodeptr _P)
    {
        return ((_Vref)(*_P)._Value);
    }

  public:
    typedef _K  key_type;
    typedef _Ty value_type;
    typedef _Pr key_compare;
    typedef ::exe_allocator<value_type> allocator_type;

    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef _Node node_type;

  protected:

    // iterator for nonmutable exe_tree
    template <typename _ValueType = typename tt::add_const<typename exe_rbtree::value_type>::type>
    class rbtree_iterator
        : public ::nh3api::container_iterator<_ValueType, typename exe_rbtree::difference_type, ::std::bidirectional_iterator_tag>
    {
        friend class exe_rbtree;
      protected:
        node_type* _Ptr;

      public:
        // construct with null node pointer
        rbtree_iterator() NH3API_NOEXCEPT
            : _Ptr(nullptr)
        {}

        // construct with node pointer _P
        rbtree_iterator(node_type* source) NH3API_NOEXCEPT
            : _Ptr(source)
        {}

        rbtree_iterator(const rbtree_iterator &other) NH3API_NOEXCEPT
            : _Ptr(other._Ptr)
        {}

        // return designated value
        const_reference operator*() const NH3API_NOEXCEPT
        {
            return (_Value(_Ptr));
        }

        // return pointer to class object
        const_pointer operator->() const NH3API_NOEXCEPT
        {
            return &**this;
        }

        // prefix increment
        rbtree_iterator &operator++() NH3API_NOEXCEPT
        {
            this->_Inc();
            return (*this);
        }

        // postfix increment
        rbtree_iterator operator++(int) NH3API_NOEXCEPT
        {
            rbtree_iterator temp = *this;
            ++*this;
            return (temp);
        }

        rbtree_iterator &operator--() NH3API_NOEXCEPT
        {
            this->_Dec();
            return (*this);
        }

        rbtree_iterator operator--(int) NH3API_NOEXCEPT
        {
            const_iterator temp = *this;
            --*this;
            return (temp);
        }

        bool operator==(const rbtree_iterator &other) const NH3API_NOEXCEPT
        {
            return (_Ptr == other._Ptr);
        }

        bool operator!=(const rbtree_iterator &other) const NH3API_NOEXCEPT
        {
            return (!(*this == other));
        }

    protected:
        NH3API_FORCEINLINE
        // move to node with next smaller value
        void _Dec() NH3API_NOEXCEPT
        {
            if (_Color(_Ptr) == _Red && _Parent(_Parent(_Ptr)) == _Ptr)
            {
                _Ptr = _Right(_Ptr); // end() ==> rightmost
            }
            else if (!_Isnil(_Left(_Ptr)))
            {
                _Ptr = _Max(_Left(_Ptr)); // ==> largest of left subtree
            }
            else
            { // climb looking for left subtree
                node_type* _P;
                while (_Ptr == _Left(_P = _Parent(_Ptr)))
                    _Ptr = _P; // ==> parent while left subtree
                _Ptr = _P;
            }
        }

        NH3API_FORCEINLINE
        // move to node with next larger value
        void _Inc() NH3API_NOEXCEPT
        {
            if (_Isnil(_Ptr))
            {
                ;	// end() shouldn't be incremented, don't move
            }
            else if (!_Isnil(_Right(_Ptr)))
            {
                _Ptr = _Min(_Right(_Ptr));
            }
            else
            {
                node_type* _P;
                while (_Ptr == _Right(_P = _Parent(_Ptr)))
                    _Ptr = _P;
                if (_Right(_Ptr) != _P)
                    _Ptr = _P;
            }
        }

        node_type* _Mynode() const NH3API_NOEXCEPT
        { return _Ptr; }
        
    };

    template <typename _ValueType>
    friend class rbtree_iterator;
    
    // CLASS iterator
  public:
        typedef rbtree_iterator<typename tt::add_const<typename exe_rbtree::value_type>::type> const_iterator;
        friend const_iterator;
        class iterator : public rbtree_iterator<typename exe_rbtree::value_type>
        {
        protected:
            typedef rbtree_iterator<typename exe_rbtree::value_type> base_type;

        public:
            iterator() NH3API_NOEXCEPT
                {}

            iterator(node_type* _P) NH3API_NOEXCEPT
                : base_type(_P)
                {}

            reference operator*() const NH3API_NOEXCEPT
            { return (_Value(this->_Ptr)); }

            pointer operator->() const NH3API_NOEXCEPT
            { return (&**this); }

            iterator &operator++() NH3API_NOEXCEPT
            {
                this->_Inc();
                return (*this);
            }

            iterator operator++(int) NH3API_NOEXCEPT
            {
                iterator temp = *this;
                ++*this;
                return (temp);
            }

            iterator &operator--() NH3API_NOEXCEPT
            {
                this->_Dec();
                return (*this);
            }

            iterator operator--(int) NH3API_NOEXCEPT
            {
                iterator temp = *this;
                --*this;
                return (temp);
            }

            bool operator==(const iterator &_X) const NH3API_NOEXCEPT
            { return (this->_Ptr == _X._Ptr); }
            bool operator!=(const iterator &_X) const NH3API_NOEXCEPT
            {
                return (!(*this == _X));
            }
        };

        friend class iterator;

        typedef ::std::reverse_iterator<iterator>       reverse_iterator;
        typedef ::std::reverse_iterator<const_iterator> const_reverse_iterator;

        explicit exe_rbtree(const key_compare &_OtherKeyCompare, bool _Marg = true, const allocator_type &_Allocator = allocator_type())
        NH3API_NOEXCEPT
            : _Dummy(0), _KeyCompare(_OtherKeyCompare), _Head(nullptr), _Multi(_Marg)
        {
            _Init();
        }

        exe_rbtree(const_pointer first, const_pointer last, const key_compare &_OtherKeyCompare, bool _Marg = true, const allocator_type &_Allocator = allocator_type())
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
            : _Dummy(0), _KeyCompare(_OtherKeyCompare), _Head(nullptr), _Multi(_Marg)
        {
            _Init();
            insert(first, last);
        }

        exe_rbtree(const exe_rbtree &other)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
            : _Dummy(0), _KeyCompare(other._KeyCompare), _Multi(other._Multi), _Head(nullptr)
        {
            _Init();
            _Copy(other);
        }

        exe_rbtree(const exe_rbtree &other, const allocator_type &_Allocator)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
            : _Dummy(0), _KeyCompare(other._KeyCompare), _Multi(other._Multi), _Head(nullptr)
        {
            _Init();
            _Copy(other);
        }

        ~exe_rbtree() NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            clear();
            _FreeRefCount();
        }

        exe_rbtree &operator=(const exe_rbtree& other)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            if (this != &other)
            {
                clear();
                _Copy(other);
            }
            return *this;
        }

        #if NH3API_STD_MOVE_SEMANTICS
        exe_rbtree& operator=(exe_rbtree&& other)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            if ( this != &other )
            {
                clear();
                ::nh3api::trivial_move<sizeof(exe_rbtree)>(&other, this);
            }
            return *this;
        }

        exe_rbtree(exe_rbtree&& other) NH3API_NOEXCEPT
        { ::nh3api::trivial_move<sizeof(exe_rbtree)>(&other, this); }

        exe_rbtree(exe_rbtree&& other, const allocator_type&) NH3API_NOEXCEPT
        { ::nh3api::trivial_move<sizeof(exe_rbtree)>(&other, this); }
        #endif

        exe_rbtree(const dummy_tag_t& tag) NH3API_NOEXCEPT
        { NH3API_IGNORE(_KeyCompare, _Multi, _Head, _Size); }

    // access
    public:
        iterator begin() NH3API_NOEXCEPT
        { return (iterator(_Lmost())); }

        const_iterator begin() const NH3API_NOEXCEPT
        { return (const_iterator(_Lmost())); }

        const_iterator cbegin() const NH3API_NOEXCEPT
        { return (const_iterator(_Lmost())); }

        iterator end() NH3API_NOEXCEPT
        { return (iterator(_Head)); }

        const_iterator end() const NH3API_NOEXCEPT
        { return (const_iterator(_Head)); }

        const_iterator cend() const NH3API_NOEXCEPT
        { return (const_iterator(_Head)); }

        reverse_iterator rbegin() NH3API_NOEXCEPT
        { return (reverse_iterator(end())); }

        const_reverse_iterator rbegin() const NH3API_NOEXCEPT
        { return (const_reverse_iterator(end())); }

        const_reverse_iterator crbegin() const NH3API_NOEXCEPT
        { return (const_reverse_iterator(end())); }

        reverse_iterator rend() NH3API_NOEXCEPT
        { return (reverse_iterator(begin())); }

        const_reverse_iterator rend() const NH3API_NOEXCEPT
        { return (const_reverse_iterator(begin())); }

        const_reverse_iterator crend() const NH3API_NOEXCEPT
        { return (const_reverse_iterator(begin())); }

        size_type size() const NH3API_NOEXCEPT
        { return _Size; }

        NH3API_NODISCARD NH3API_FORCEINLINE NH3API_CONSTEXPR
        static size_type max_size() NH3API_NOEXCEPT
        { return NH3API_MAX_HEAP_REQUEST / sizeof(value_type); }

        bool empty() const NH3API_NOEXCEPT
        { return size() == 0; }

        allocator_type get_allocator() const NH3API_NOEXCEPT
        { return allocator_type();}

        key_compare key_comp() const NH3API_NOEXCEPT
        { return _KeyCompare; }

    // inserting
    public:
        ::std::pair<iterator, bool> insert(const value_type &value)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
        {
            node_type* _Trynode = _Root();
            node_type* _Wherenode = _Head;
            bool _Ans = true;
            while (!_Isnil(_Trynode))
            {
                _Wherenode = _Trynode;
                _Ans = _KeyCompare(_Kfn()(value), _Key(_Trynode));
                _Trynode = _Ans ? _Left(_Trynode) : _Right(_Trynode);
            }
            if (_Multi)
                return ::std::make_pair(_Insert(_Trynode, _Wherenode, value), true);
            iterator _Where = iterator(_Wherenode);
            if (!_Ans)
                ;
            else if (_Where == begin())
                return ::std::make_pair(_Insert(_Trynode, _Wherenode, value), true);
            else
                --_Where;
            if (_KeyCompare(_Key(_Where._Mynode()), _Kfn()(value)))
                return ::std::make_pair(_Insert(_Trynode, _Wherenode, value), true);
            return ::std::make_pair(_Where, false);
        }
        iterator insert(iterator pos, const value_type &value)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
        {
            if (size() == 0)
                ;
            else if (pos == begin())
            {
                if (_KeyCompare(_Kfn()(value), _Key(pos._Mynode())))
                    return _Insert(_Head, pos._Mynode(), value);
            }
            else if (pos == end())
            { // _Lockit Lk;
                if (_KeyCompare(_Key(_Rmost()), _Kfn()(value)))
                    return _Insert(_Getnil(), _Rmost(), value);
            }
            else
            {
                iterator _Next = pos;
                if (_KeyCompare(_Key((--_Next)._Mynode()), _Kfn()(value)) && _KeyCompare(_Kfn()(value), _Key(pos._Mynode())))
                { // _Lockit _Lk;
                    if (_Isnil(_Right(_Next._Mynode())))
                        return _Insert(_Getnil(), _Next._Mynode(), value);
                    else
                        return _Insert(_Head, pos._Mynode(), value);
                }
            }
            return insert(value).first;
        }

        void insert(iterator first, iterator last)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
        {
            for (; first != last; ++first)
                insert(*first);
        }

        void insert(const_pointer first, const_pointer last)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
        {
            for (; first != last; ++first)
                insert(*first);
        }

    // erasing
    public:
        iterator erase(iterator pos)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            node_type* _Fixnode = nullptr;
            node_type* _Erasednode = (pos++)._Mynode();
            node_type* _Pnode = _Erasednode;
            if (_Isnil(_Left(_Erasednode)))
            {
                _Fixnode = _Right(_Erasednode);
            }
            else if (_Isnil(_Right(_Erasednode)))
            {
                _Fixnode = _Left(_Erasednode);
            }
            else
            {
                _Erasednode = _Min(_Right(_Erasednode));
                _Fixnode = _Right(_Erasednode);
            }

            if (_Erasednode != _Pnode)
            {
                _Parent(_Left(_Pnode)) = _Erasednode;
                _Left(_Erasednode) = _Left(_Pnode);
                if (_Erasednode == _Right(_Pnode))
                    _Parent(_Fixnode) = _Erasednode;
                else
                {
                    _Parent(_Fixnode) = _Parent(_Erasednode);
                    _Left(_Parent(_Erasednode)) = _Fixnode;
                    _Right(_Erasednode) = _Right(_Pnode);
                    _Parent(_Right(_Pnode)) = _Erasednode;
                }
                if (_Root() == _Pnode)
                    _Root() = _Erasednode;
                else if (_Left(_Parent(_Pnode)) == _Pnode)
                    _Left(_Parent(_Pnode)) = _Erasednode;
                else
                    _Right(_Parent(_Pnode)) = _Erasednode;
                _Parent(_Erasednode) = _Parent(_Pnode);
                ::std::swap(_Color(_Erasednode), _Color(_Pnode));
                _Erasednode = _Pnode;
            }
            else
            {
                _Parent(_Fixnode) = _Parent(_Erasednode);
                if (_Root() == _Pnode)
                    _Root() = _Fixnode;
                else if (_Left(_Parent(_Pnode)) == _Pnode)
                    _Left(_Parent(_Pnode)) = _Fixnode;
                else
                    _Right(_Parent(_Pnode)) = _Fixnode;
                if (_Lmost() != _Pnode)
                    ;
                else if (_Isnil(_Right(_Pnode)))
                    _Lmost() = _Parent(_Pnode);
                else
                    _Lmost() = _Min(_Fixnode);
                if (_Rmost() != _Pnode)
                    ;
                else if (_Isnil(_Left(_Pnode)))
                    _Rmost() = _Parent(_Pnode);
                else
                    _Rmost() = _Max(_Fixnode);
            }
            if (_Color(_Erasednode) == _Black)
            {
                while (_Fixnode != _Root() && _Color(_Fixnode) == _Black)
                    if (_Fixnode == _Left(_Parent(_Fixnode)))
                    {
                        _Pnode = _Right(_Parent(_Fixnode));
                        if (_Color(_Pnode) == _Red)
                        {
                            _Color(_Pnode) = _Black;
                            _Color(_Parent(_Fixnode)) = _Red;
                            _Lrotate(_Parent(_Fixnode));
                            _Pnode = _Right(_Parent(_Fixnode));
                        }
                        if (_Color(_Left(_Pnode)) == _Black && _Color(_Right(_Pnode)) == _Black)
                        {
                            _Color(_Pnode) = _Red;
                            _Fixnode = _Parent(_Fixnode);
                        }
                        else
                        {
                            if (_Color(_Right(_Pnode)) == _Black)
                            {
                                _Color(_Left(_Pnode)) = _Black;
                                _Color(_Pnode) = _Red;
                                _Rrotate(_Pnode);
                                _Pnode = _Right(_Parent(_Fixnode));
                            }
                            _Color(_Pnode) = _Color(_Parent(_Fixnode));
                            _Color(_Parent(_Fixnode)) = _Black;
                            _Color(_Right(_Pnode)) = _Black;
                            _Lrotate(_Parent(_Fixnode));
                            break;
                        }
                    }
                    else
                    {
                        _Pnode = _Left(_Parent(_Fixnode));
                        if (_Color(_Pnode) == _Red)
                        {
                            _Color(_Pnode) = _Black;
                            _Color(_Parent(_Fixnode)) = _Red;
                            _Rrotate(_Parent(_Fixnode));
                            _Pnode = _Left(_Parent(_Fixnode));
                        }
                        if (_Color(_Right(_Pnode)) == _Black && _Color(_Left(_Pnode)) == _Black)
                        {
                            _Color(_Pnode) = _Red;
                            _Fixnode = _Parent(_Fixnode);
                        }
                        else
                        {
                            if (_Color(_Left(_Pnode)) == _Black)
                            {
                                _Color(_Right(_Pnode)) = _Black;
                                _Color(_Pnode) = _Red;
                                _Lrotate(_Pnode);
                                _Pnode = _Left(_Parent(_Fixnode));
                            }
                            _Color(_Pnode) = _Color(_Parent(_Fixnode));
                            _Color(_Parent(_Fixnode)) = _Black;
                            _Color(_Left(_Pnode)) = _Black;
                            _Rrotate(_Parent(_Fixnode));
                            break;
                        }
                    }
                _Color(_Fixnode) = _Black;
            }
            ::nh3api::destroy_at(::nh3api::addressof(_Value(_Erasednode)));
            _Freenode(_Erasednode);
            --_Size;
            return (pos);
        }

        iterator erase(iterator first, iterator last)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            if (empty() || first != begin() || last != end())
            {
                while (first != last)
                    erase(first++);
                return (first);
            }
            else
            {
                _Erase(_Root());
                _Root() = const_cast<node_type*>(_Getnil());
                _Size = 0;
                _Lmost() = _Head;
                _Rmost() = _Head;
                return (begin());
            }
        }

        size_type erase(const key_type &value)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            ::std::pair<iterator, iterator> _P = equal_range(value);
            size_type _N = static_cast<size_type>(::std::distance(_P.first, _P.second));
            erase(_P.first, _P.second);
            return (_N);
        }

        void erase(const key_type *first, const key_type *last)
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        {
            for (; first != last; ++first)
                erase(*first);
        }

        void clear()
        NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
        { erase(begin(), end()); }

    // finding, counting, etc.
    public:
        iterator find(const key_type &key) NH3API_NOEXCEPT
        {
            iterator _Where = lower_bound(key);
            return (_Where == end() || _KeyCompare(key, _Key(_Where._Mynode())) ? end() : _Where);
        }
        const_iterator find(const key_type &key) const NH3API_NOEXCEPT
        {
            const_iterator _Where = lower_bound(key);
            return (_Where == end() || _KeyCompare(key, _Key(_Where._Mynode())) ? end() : _Where);
        }
        size_type count(const key_type &key) const NH3API_NOEXCEPT
        {
            ::std::pair<const_iterator, const_iterator> result = equal_range(key);
            return ::std::distance(result.first, result.second);
        }
        iterator lower_bound(const key_type &key) NH3API_NOEXCEPT
        {
            return (iterator(_Lbound(key)));
        }
        const_iterator lower_bound(const key_type &key) const NH3API_NOEXCEPT
        {
            return (const_iterator(_Lbound(key)));
        }
        iterator upper_bound(const key_type &key) NH3API_NOEXCEPT
        {
            return (iterator(_Ubound(key)));
        }
        const_iterator upper_bound(const key_type &key) const NH3API_NOEXCEPT
        {
            return (iterator(_Ubound(key)));
        }
        ::std::pair<iterator, iterator> equal_range(const key_type &key)
        {
            return ::std::make_pair(lower_bound(key), upper_bound(key));
        }
        ::std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
        {
            return ::std::make_pair(lower_bound(key), upper_bound(key));
        }

    // swap
    public:
        void swap(exe_rbtree& other) NH3API_NOEXCEPT
        {
            if ( this != &other)
            {
                return;
            }
            ::nh3api::trivial_swap<sizeof(exe_rbtree)>(this, &other);
        }

    // implementation
    protected:
    // copy entire tree from other
    void _Copy(const exe_rbtree& other)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
    {
        _Root() = _Copy(other._Root(), _Head);
        _Size = other.size();
        if (!_Isnil(_Root()))
        {
            // nonempty tree, look for new smallest and largest
            _Lmost() = _Min(_Root());
            _Rmost() = _Max(_Root());
        }
        else
        {
            // empty tree
            _Lmost() = _Head;
            _Rmost() = _Head;
        }
    }

    // copy entire subtree, recursively
    node_type* _Copy(node_type* _Rootnode, node_type* _Wherenode)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
    {
        node_type* _Newroot = _Rootnode; // point at nil node
        for (; !_Isnil(_Rootnode); _Rootnode = _Left(_Rootnode))
        {
            // copy a node, then any subtrees
            node_type* _Pnode = _Buynode(_Wherenode, _Color(_Rootnode));
            if (_Newroot == _Rootnode)
                _Newroot = _Pnode; // memorize new root
            _Right(_Pnode) = _Copy(_Right(_Rootnode), _Pnode);
            ::nh3api::construct_at(::nh3api::addressof(_Value(_Pnode)), _Value(_Rootnode));
            _Left(_Wherenode) = _Pnode;
            _Wherenode = _Pnode;
        }
        _Left(_Wherenode) = _Getnil();
        return _Newroot;
    }
    void _Erase(node_type* _Rootnode) 
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
    {
        for (node_type* _Pnode = _Rootnode; !_Isnil(_Pnode); _Rootnode = _Pnode)
        {
            _Erase(_Right(_Pnode));
            _Pnode = _Left(_Pnode);
            ::nh3api::destroy_at(::nh3api::addressof(_Value(_Rootnode)));
            _Freenode(_Rootnode);
        }
    }

    void _Init() NH3API_NOEXCEPT
    {
        ::exe_scoped_lock lock; // mutex
        _InitRefCount();
        _Head = _Buynode(const_cast<node_type*>(_Getnil()), _Red);
        _Size = size_type(0);
        _Lmost() = _Head;
        _Rmost() = _Head;
    }

    void _InitRefCount() NH3API_NOEXCEPT
    {
        if (_Getnil() == nullptr)
        {
            _Getnil() = _Buynode(nullptr, _Black);
            _Left(const_cast<node_type*>(_Getnil()))  = nullptr;
            _Right(const_cast<node_type*>(_Getnil())) = nullptr;
        }
        // increment the global _Nilrefs counter
        _InterlockedIncrement(reinterpret_cast<volatile long*>(_Getnilrefs()));
    }

    void _FreeRefCount() NH3API_NOEXCEPT
    {
        exe_scoped_lock lock;
        _Freenode(_Head);
        _Head = nullptr;
        _Size = 0;
        {
            if ( _InterlockedDecrement(reinterpret_cast<volatile long*>(_Getnilrefs())) == 0)
            {
                _Freenode(const_cast<node_type*>(_Getnil()));
                _Getnil() = nullptr;
            }
        }
    }

    void _Tidy() NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<value_type>::value)
    { clear(); }

    iterator _Insert(node_type* _X, node_type* _Y, const value_type& value)
    NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<value_type>::value)
    {
        node_type* _Z = _Buynode(_Y, _Red);
        _Left(_Z) = _Getnil();
        _Right(_Z) = _Getnil();
        ::nh3api::construct_at(::nh3api::addressof(_Value(_Z)), value);
        ++_Size;
        if (_Y == _Head || !_Isnil(_X) || _KeyCompare(_Kfn()(value), _Key(_Y)))
        {
            _Left(_Y) = _Z;
            if (_Y == _Head)
            {
                _Root() = _Z;
                _Rmost() = _Z;
            }
            else if (_Y == _Lmost())
                _Lmost() = _Z;
        }
        else
        {
            _Right(_Y) = _Z;
            if (_Y == _Rmost())
                _Rmost() = _Z;
        }
        for (_X = _Z; _X != _Root() && _Color(_Parent(_X)) == _Red;)
            if (_Parent(_X) == _Left(_Parent(_Parent(_X))))
            {
                _Y = _Right(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                {
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X));
                }
                else
                {
                    if (_X == _Right(_Parent(_X)))
                    {
                        _X = _Parent(_X);
                        _Lrotate(_X);
                    }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _Rrotate(_Parent(_Parent(_X)));
                }
            }
            else
            {
                _Y = _Left(_Parent(_Parent(_X)));
                if (_Color(_Y) == _Red)
                {
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Y) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _X = _Parent(_Parent(_X));
                }
                else
                {
                    if (_X == _Left(_Parent(_X)))
                    {
                        _X = _Parent(_X);
                        _Rrotate(_X);
                    }
                    _Color(_Parent(_X)) = _Black;
                    _Color(_Parent(_Parent(_X))) = _Red;
                    _Lrotate(_Parent(_Parent(_X)));
                }
            }
        _Color(_Root()) = _Black;
        return (iterator(_Z));
    }
    node_type* _Lbound(const key_type &key) const NH3API_NOEXCEPT
    {
        node_type *_X = _Root();
        node_type *_Y = _Head;
        while (!_Isnil(_X))
        {
            if (_KeyCompare(_Key(_X), key))
            {
                _X = _Right(_X);
            }
            else
            {
                _Y = _X;
                _X = _Left(_X);
            }
        }
        return _Y;
    }
    node_type*& _Lmost() NH3API_NOEXCEPT
    {
        return _Left(_Head);
    }
    node_type*& _Lmost() const NH3API_NOEXCEPT
    {
        return _Left(_Head);
    }
    void _Lrotate(node_type* _X) NH3API_NOEXCEPT
    { // _Lockit _Lk;
        node_type* _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (!_Isnil(_Left(_Y)))
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y;
    }
    static node_type* _Max(node_type* _P) NH3API_NOEXCEPT
    { // _Lockit _Lk;
        while (!_Isnil(_Right(_P)))
            _P = _Right(_P);
        return _P;
    }
    static node_type* _Min(node_type* _P) NH3API_NOEXCEPT
    { // _Lockit _Lk;
        while (!_Isnil(_Left(_P)))
            _P = _Left(_P);
        return _P;
    }
    node_type*& _Rmost() NH3API_NOEXCEPT
    {
        return _Right(_Head);
    }
    node_type*& _Rmost() const NH3API_NOEXCEPT
    {
        return _Right(_Head);
    }
    node_type*& _Root() NH3API_NOEXCEPT
    {
        return _Parent(_Head);
    }
    node_type*& _Root() const NH3API_NOEXCEPT
    {
        return _Parent(_Head);
    }
    // promote left node to root of subtree
    void _Rrotate(node_type* _X) NH3API_NOEXCEPT
    { // _Lockit _Lk;
        node_type* _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (!_Isnil(_Right(_Y)))
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y;
    }
    // find leftmost node greater than _Keyval
    node_type* _Ubound(const key_type &key) const NH3API_NOEXCEPT
    {
        node_type* _X = _Root();
        node_type* _Y = _Head;
        while (!_Isnil(_X))
        {
            if (_KeyCompare(key, _Key(_X)))
            {
                _Y = _X;
                _X = _Left(_X);
            }
            else
            {
                _X = _Right(_X);
            }
        }
        return _Y;
    }
    // allocate a non-value node
    node_type* _Buynode(node_type* parent, _Redbl color) NH3API_NOEXCEPT
    {
        node_type* new_node = static_cast<node_type*>(::operator new(sizeof(node_type), exe_heap, ::std::nothrow));
        _Parent(new_node) = parent;
        _Color(new_node) = color;
        return new_node;
    }

    void _Freenode(node_type* node) NH3API_NOEXCEPT
    { ::operator delete(node, ::exe_heap); }

    uint8_t      _Dummy;
    key_compare  _KeyCompare;
    node_type*   _Head;
    bool         _Multi;
    size_type    _Size;
};
#pragma pack(pop)

} // namespace nh3api

template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator==(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (_X.size() == _Y.size() && ::std::equal(_X.begin(), _X.end(), _Y.begin()));
}
template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator!=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_X == _Y));
}
template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator<(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (::std::lexicographical_compare(_X.begin(), _X.end(), _Y.begin(), _Y.end()));
}
template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator>(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (_Y < _X);
}
template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator<=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_Y < _X));
}
template <class _K, class _Ty, class _Kfn, class _Pr, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator>=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_X < _Y));
}

NH3API_DISABLE_WARNING_END
