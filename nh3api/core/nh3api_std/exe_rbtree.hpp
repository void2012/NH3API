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

#include "iterator.hpp"
#include "memory.hpp"
#include "nh3api_std.hpp"
#include <cstddef>
#include <utility>

namespace nh3api
{
#pragma pack(push, 8)
// ordered red-black tree for [multi_]{map set}
template <class _K,   // key
          class _Ty,  // stored type
          class _Kfn, // key access unary function
          class _Pr,  // compare binary predicate
          class _A,   // allocator
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
    typedef _A allocator_type;

    // internal typedefs
  protected:
    typedef nh3api::allocator_adaptor<allocator_type> adaptor_type;
    typedef typename adaptor_type::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
    typedef typename adaptor_type::propagate_on_container_move_assignment propagate_on_container_move_assignment;
    typedef typename adaptor_type::propagate_on_container_swap propagate_on_container_swap;

  public:
    typedef exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> this_type;
    typedef _K  key_type;
    typedef _Ty value_type;
    typedef _Pr key_compare;
    typedef typename adaptor_type::size_type       size_type;
    typedef typename adaptor_type::difference_type difference_type;
    typedef typename adaptor_type::pointer         pointer;
    typedef typename adaptor_type::const_pointer   const_pointer;
    typedef typename adaptor_type::reference       reference;
    typedef typename adaptor_type::const_reference const_reference;
    typedef _Node node_type;

    class iterator;
    friend class const_iterator;

  protected:
        enum
        {
            _bit_swappable =
            tt::is_empty<allocator_type>::value
            && tt::is_empty<key_compare>::value
        };

    // iterator for nonmutable exe_tree
    template <typename _ValueType = typename std::add_const<exe_rbtree::value_type>::type>
    class _const_iterator
        : public nh3api::container_iterator<_ValueType, exe_rbtree::difference_type, ::std::bidirectional_iterator_tag>
    {

      protected:
        node_type* _Ptr;

      public:
        // construct with null node pointer
        _const_iterator() NH3API_NOEXCEPT
            : _Ptr(nullptr)
        {}

        // construct with node pointer _P
        _const_iterator(node_type* _P) NH3API_NOEXCEPT
            : _Ptr(_P)
        {}

        _const_iterator(const iterator &_X) NH3API_NOEXCEPT
            : _Ptr(_X._Ptr)
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

        NH3API_NOINLINE
        // prefix increment
        _const_iterator &operator++() NH3API_NOEXCEPT
        {
            this->_Inc();
            return (*this);
        }

        // postfix increment
        _const_iterator operator++(int) NH3API_NOEXCEPT
        {
            _const_iterator _Tmp = *this;
            ++*this;
            return (_Tmp);
        }

        NH3API_NOINLINE
        _const_iterator &operator--() NH3API_NOEXCEPT
        {
            this->_Dec();
            return (*this);
        }
        _const_iterator operator--(int) NH3API_NOEXCEPT
        {
            const_iterator _Tmp = *this;
            --*this;
            return (_Tmp);
        }
        bool operator==(const _const_iterator &_X) const NH3API_NOEXCEPT
        {
            return (_Ptr == _X._Ptr);
        }
        bool operator!=(const _const_iterator &_X) const NH3API_NOEXCEPT
        {
            return (!(*this == _X));
        }

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
        {
            return (_Ptr);
        }
    };
    // CLASS iterator
  public:
        typedef _const_iterator<typename std::add_const<exe_rbtree::value_type>::type> const_iterator;
        friend class iterator;
        class iterator : public _const_iterator<exe_rbtree::value_type>
        {
        protected:
            typedef _const_iterator<exe_rbtree::value_type> base_type;

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
                iterator _Tmp = *this;
                ++*this;
                return (_Tmp);
            }

            iterator &operator--() NH3API_NOEXCEPT
            {
                this->_Dec();
                return (*this);
            }

            iterator operator--(int) NH3API_NOEXCEPT
            {
                iterator _Tmp = *this;
                --*this;
                return (_Tmp);
            }

            bool operator==(const iterator &_X) const NH3API_NOEXCEPT
            { return (this->_Ptr == _X._Ptr); }
            bool operator!=(const iterator &_X) const NH3API_NOEXCEPT
            {
                return (!(*this == _X));
            }
        };

        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        explicit exe_rbtree(const key_compare &_Parg, bool _Marg = true, const allocator_type &allocator = allocator_type())
        NH3API_NOEXCEPT_ALLOC
            : adaptor(allocator), _KeyCompare(_Parg), _Head(nullptr), _Multi(_Marg)
        {
            _Init();
        }

        exe_rbtree(const_pointer first, const_pointer last, const key_compare &_Parg, bool _Marg = true, const allocator_type &allocator = allocator_type())
            : adaptor(allocator), _KeyCompare(_Parg), _Head(nullptr), _Multi(_Marg)
        {
            _Init();
            insert(first, last);
        }

        exe_rbtree(const exe_rbtree &_X)
            : adaptor(_X.adaptor), _KeyCompare(_X._KeyCompare), _Multi(_X._Multi), _Head(nullptr)
        {
            _Init();
            _Copy(_X);
        }

        exe_rbtree(const exe_rbtree &_X, const allocator_type &allocator)
            : adaptor(allocator), _KeyCompare(_X._KeyCompare), _Multi(_X._Multi), _Head(nullptr)
        {
            _Init();
            _Copy(_X);
        }

        ~exe_rbtree()
        {
            clear();
            _FreeRefCount();
        }

        exe_rbtree &operator=(const exe_rbtree& other)
        {
            if (this != &other)
            {
                clear();
                if (this->get_allocator() != other.get_allocator()
                    && adaptor_type::propagate_on_container_copy_assignment::value)
                {
                    this->adaptor = other.adaptor;
                }
                _KeyCompare = other._KeyCompare;
                _Copy(other);
            }
            return *this;
        }

        #if NH3API_STD_MOVE_SEMANTICS
        exe_rbtree& operator=(exe_rbtree&& other)
        NH3API_NOEXCEPT_EXPR(_bit_swappable)
        {
            if ( this != &other )
            {
                clear();
                if (adaptor_type::propagate_on_container_move_assignment::value
                    && this->adaptor != other.adaptor)
                {
                    this->adaptor = nh3api::exchange(other.adaptor, adaptor_type());
                }
                _Assign_rv(::std::forward<exe_rbtree>(other));
            }
            return *this;
        }

        exe_rbtree(exe_rbtree&& other)
        NH3API_NOEXCEPT_EXPR(_bit_swappable)
            : adaptor()
        {
            _Assign_rv(::std::forward<exe_rbtree>(other), tt::true_type());
        }

        exe_rbtree(exe_rbtree&& other, const allocator_type& _Al)
        NH3API_NOEXCEPT_EXPR(_bit_swappable)
            : adaptor(_Al)
        {
            _Assign_rv(::std::forward<exe_rbtree>(other));
        }
        #endif

        exe_rbtree(const nh3api::dummy_tag_t& tag)
        NH3API_NOEXCEPT
            : adaptor(tag)
        { NH3API_IGNORE(_KeyCompare, _Multi, _Head, _Size); }

    // access
    public:
        iterator begin() NH3API_NOEXCEPT
        {
            return (iterator(_Lmost()));
        }
        const_iterator begin() const NH3API_NOEXCEPT
        {
            return (const_iterator(_Lmost()));
        }
        const_iterator cbegin() const NH3API_NOEXCEPT
        {
            return (const_iterator(_Lmost()));
        }
        iterator end() NH3API_NOEXCEPT
        {
            return (iterator(_Head));
        }
        const_iterator end() const NH3API_NOEXCEPT
        {
            return (const_iterator(_Head));
        }
        const_iterator cend() const NH3API_NOEXCEPT
        {
            return (const_iterator(_Head));
        }
        reverse_iterator rbegin() NH3API_NOEXCEPT
        {
            return (reverse_iterator(end()));
        }
        const_reverse_iterator rbegin() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator(end()));
        }
        const_reverse_iterator crbegin() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator(end()));
        }
        reverse_iterator rend() NH3API_NOEXCEPT
        {
            return (reverse_iterator(begin()));
        }
        const_reverse_iterator rend() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator(begin()));
        }
        const_reverse_iterator crend() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator(begin()));
        }
        size_type size() const NH3API_NOEXCEPT
        { return _Size; }
        size_type max_size() const NH3API_NOEXCEPT
        { return (adaptor.max_size()); }
        bool empty() const NH3API_NOEXCEPT
        {
            return size() == 0;
        }
        allocator_type get_allocator() const NH3API_NOEXCEPT
        {
            return adaptor.alloc;
        }
        key_compare key_comp() const NH3API_NOEXCEPT
        {
            return _KeyCompare;
        }

    // inserting
    public:
        ::std::pair<iterator, bool> insert(const value_type &value)
        {
            node_type* _X = _Root();
            node_type* _Y = _Head;
            bool _Ans = true;
            while (!_Isnil(_X))
            {
                _Y = _X;
                _Ans = _KeyCompare(_Kfn()(value), _Key(_X));
                _X = _Ans ? _Left(_X) : _Right(_X);
            }
            if (_Multi)
                return ::std::make_pair(_Insert(_X, _Y, value), true);
            iterator _P = iterator(_Y);
            if (!_Ans)
                ;
            else if (_P == begin())
                return ::std::make_pair(_Insert(_X, _Y, value), true);
            else
                --_P;
            if (_KeyCompare(_Key(_P._Mynode()), _Kfn()(value)))
                return ::std::make_pair(_Insert(_X, _Y, value), true);
            return ::std::make_pair(_P, false);
        }
        iterator insert(iterator _P, const value_type &value)
        {
            if (size() == 0)
                ;
            else if (_P == begin())
            {
                if (_KeyCompare(_Kfn()(value), _Key(_P._Mynode())))
                    return _Insert(_Head, _P._Mynode(), value);
            }
            else if (_P == end())
            { // _Lockit Lk;
                if (_KeyCompare(_Key(_Rmost()), _Kfn()(value)))
                    return _Insert(_Getnil(), _Rmost(), value);
            }
            else
            {
                iterator _Pb = _P;
                if (_KeyCompare(_Key((--_Pb)._Mynode()), _Kfn()(value)) && _KeyCompare(_Kfn()(value), _Key(_P._Mynode())))
                { // _Lockit _Lk;
                    if (_Isnil(_Right(_Pb._Mynode())))
                        return _Insert(_Getnil(), _Pb._Mynode(), value);
                    else
                        return _Insert(_Head, _P._Mynode(), value);
                }
            }
            return insert(value).first;
        }
        void insert(iterator first, iterator last)
        {
            for (; first != last; ++first)
                insert(*first);
        }
        void insert(const_pointer first, const_pointer last)
        {
            for (; first != last; ++first)
                insert(*first);
        }

    // erasing
    public:
        iterator erase(iterator _P)
        {
            node_type* _X = nullptr;
            node_type* _Y = (_P++)._Mynode();
            node_type* _Z = _Y;
            if (_Isnil(_Left(_Y)))
            {
                _X = _Right(_Y);
            }
            else if (_Isnil(_Right(_Y)))
            {
                _X = _Left(_Y);
            }
            else
            {
                _Y = _Min(_Right(_Y));
                _X = _Right(_Y);
            }

            if (_Y != _Z)
            {
                _Parent(_Left(_Z)) = _Y;
                _Left(_Y) = _Left(_Z);
                if (_Y == _Right(_Z))
                    _Parent(_X) = _Y;
                else
                {
                    _Parent(_X) = _Parent(_Y);
                    _Left(_Parent(_Y)) = _X;
                    _Right(_Y) = _Right(_Z);
                    _Parent(_Right(_Z)) = _Y;
                }
                if (_Root() == _Z)
                    _Root() = _Y;
                else if (_Left(_Parent(_Z)) == _Z)
                    _Left(_Parent(_Z)) = _Y;
                else
                    _Right(_Parent(_Z)) = _Y;
                _Parent(_Y) = _Parent(_Z);
                ::std::swap(_Color(_Y), _Color(_Z));
                _Y = _Z;
            }
            else
            {
                _Parent(_X) = _Parent(_Y);
                if (_Root() == _Z)
                    _Root() = _X;
                else if (_Left(_Parent(_Z)) == _Z)
                    _Left(_Parent(_Z)) = _X;
                else
                    _Right(_Parent(_Z)) = _X;
                if (_Lmost() != _Z)
                    ;
                else if (_Isnil(_Right(_Z)))
                    _Lmost() = _Parent(_Z);
                else
                    _Lmost() = _Min(_X);
                if (_Rmost() != _Z)
                    ;
                else if (_Isnil(_Left(_Z)))
                    _Rmost() = _Parent(_Z);
                else
                    _Rmost() = _Max(_X);
            }
            if (_Color(_Y) == _Black)
            {
                while (_X != _Root() && _Color(_X) == _Black)
                    if (_X == _Left(_Parent(_X)))
                    {
                        node_type* _W = _Right(_Parent(_X));
                        if (_Color(_W) == _Red)
                        {
                            _Color(_W) = _Black;
                            _Color(_Parent(_X)) = _Red;
                            _Lrotate(_Parent(_X));
                            _W = _Right(_Parent(_X));
                        }
                        if (_Color(_Left(_W)) == _Black && _Color(_Right(_W)) == _Black)
                        {
                            _Color(_W) = _Red;
                            _X = _Parent(_X);
                        }
                        else
                        {
                            if (_Color(_Right(_W)) == _Black)
                            {
                                _Color(_Left(_W)) = _Black;
                                _Color(_W) = _Red;
                                _Rrotate(_W);
                                _W = _Right(_Parent(_X));
                            }
                            _Color(_W) = _Color(_Parent(_X));
                            _Color(_Parent(_X)) = _Black;
                            _Color(_Right(_W)) = _Black;
                            _Lrotate(_Parent(_X));
                            break;
                        }
                    }
                    else
                    {
                        node_type* _W = _Left(_Parent(_X));
                        if (_Color(_W) == _Red)
                        {
                            _Color(_W) = _Black;
                            _Color(_Parent(_X)) = _Red;
                            _Rrotate(_Parent(_X));
                            _W = _Left(_Parent(_X));
                        }
                        if (_Color(_Right(_W)) == _Black && _Color(_Left(_W)) == _Black)
                        {
                            _Color(_W) = _Red;
                            _X = _Parent(_X);
                        }
                        else
                        {
                            if (_Color(_Left(_W)) == _Black)
                            {
                                _Color(_Right(_W)) = _Black;
                                _Color(_W) = _Red;
                                _Lrotate(_W);
                                _W = _Left(_Parent(_X));
                            }
                            _Color(_W) = _Color(_Parent(_X));
                            _Color(_Parent(_X)) = _Black;
                            _Color(_Left(_W)) = _Black;
                            _Rrotate(_Parent(_X));
                            break;
                        }
                    }
                _Color(_X) = _Black;
            }
            adaptor.destroy(nh3api::addressof(_Value(_Y)));
            _Freenode(_Y);
            --_Size;
            return (_P);
        }
        iterator erase(iterator first, iterator last)
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
        size_type erase(const key_type &_X)
        {
            ::std::pair<iterator, iterator> _P = equal_range(_X);
            size_type _N = static_cast<size_type>(::std::distance(_P.first, _P.second));
            erase(_P.first, _P.second);
            return (_N);
        }
        void erase(const key_type *first, const key_type *last)
        {
            for (; first != last; ++first)
                erase(*first);
        }
        void clear()
        {
            erase(begin(), end());
        }

    // finding, counting, etc.
    public:
        iterator find(const key_type &key) NH3API_NOEXCEPT
        {
            iterator _P = lower_bound(key);
            return (_P == end() || _KeyCompare(key, _Key(_P._Mynode())) ? end() : _P);
        }
        const_iterator find(const key_type &key) const NH3API_NOEXCEPT
        {
            const_iterator _P = lower_bound(key);
            return (_P == end() || _KeyCompare(key, _Key(_P._Mynode())) ? end() : _P);
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

    // misc
    public:
        void swap(this_type& other)
        NH3API_NOEXCEPT_EXPR(_bit_swappable)
        {
            if ( this != &other)
            {
                return;
            }
            if ( _bit_swappable )
            {
                swap16(this, &other);
            }
            else NH3API_IF_CONSTEXPR (adaptor == other.adaptor)
            {
                ::std::swap(this->_KeyCompare, other._KeyCompare);
                ::std::swap(this->_Head, other._Head);
                ::std::swap(this->_Multi, other._Multi);
                ::std::swap(this->_Size, other._Size);
            }
            else NH3API_IF_CONSTEXPR (adaptor_type::propagate_on_container_swap::value)
            {
                ::std::swap(this->adaptor, other.adaptor);
                ::std::swap(this->_KeyCompare, other._KeyCompare);
                ::std::swap(this->_Head, other._Head);
                ::std::swap(this->_Multi, other._Multi);
                ::std::swap(this->_Size, other._Size);
            }
            else
            {
                assert("map/set containers incompatible for swap"&&0);
                ::std::terminate();
            }
        }

    // implementation
  protected:
    #if NH3API_STD_MOVE_SEMANTICS

    void _Assign_rv(exe_rbtree&& other)
    NH3API_NOEXCEPT_EXPR(_bit_swappable)
    {
        _Assign_rv(::std::forward<exe_rbtree>(other),
            typename adaptor_type::propagate_on_container_move_assignment());
    }

    void _Assign_rv(exe_rbtree&& other, tt::true_type)
    NH3API_NOEXCEPT_EXPR(_bit_swappable)
    {
        if ( _bit_swappable )
        {
            move16(this, &other);
        }
        else
        {
            this->adaptor = std::move(other.adaptor);
            this->_KeyCompare = ::nh3api::exchange(other._KeyCompare, _KeyCompare());
            this->_Head = ::nh3api::exchange(other._Head, nullptr);
            this->_Multi = other._Multi;
            this->_Size = ::nh3api::exchange(other._Size, size_type());
        }
    }

    void _Assign_rv(exe_rbtree&& other, tt::false_type)
    NH3API_NOEXCEPT_EXPR(_bit_swappable)
    {
        if (this->adaptor.alloc == other.adaptor.alloc)
        {
            _Assign_rv(::std::forward<exe_rbtree>(other), tt::true_type());
        }
        else
        {
            _Copy(other);
        }
    }
    #endif

    // copy entire tree from other
    void _Copy(const this_type &other)
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
    {
        node_type* _Newroot = _Rootnode; // point at nil node
        for (; !_Isnil(_Rootnode); _Rootnode = _Left(_Rootnode))
        {
            // copy a node, then any subtrees
            node_type* _Pnode = _Buynode(_Wherenode, _Color(_Rootnode));
            if (_Newroot == _Rootnode)
                _Newroot = _Pnode; // memorize new root
            _Right(_Pnode) = _Copy(_Right(_Rootnode), _Pnode);
            adaptor.copy_construct(addressof(_Value(_Pnode)), _Value(_Rootnode));
            _Left(_Wherenode) = _Pnode;
            _Wherenode = _Pnode;
        }
        _Left(_Wherenode) = _Getnil();
        return _Newroot;
    }
    void _Erase(node_type* _Rootnode) NH3API_NOEXCEPT_DESTRUCT
    {
        for (node_type* _Pnode = _Rootnode; !_Isnil(_Pnode); _Rootnode = _Pnode)
        {
            _Erase(_Right(_Pnode));
            _Pnode = _Left(_Pnode);
            adaptor.destroy(nh3api::addressof(_Value(_Rootnode)));
            _Freenode(_Rootnode);
        }
    }

    void _Init() NH3API_NOEXCEPT_ALLOC
    {
        exe_scoped_lock lock;
        _InitRefCount();
        _Head = _Buynode(const_cast<node_type*>(_Getnil()), _Red);
        _Size = size_type(0);
        _Lmost() = _Head;
        _Rmost() = _Head;
    }

    void _InitRefCount() NH3API_NOEXCEPT_ALLOC
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

    void _Tidy() NH3API_NOEXCEPT_DESTRUCT
    { clear(); }

    iterator _Insert(node_type* _X, node_type* _Y, const value_type& value)
    {
        node_type* _Z = _Buynode(_Y, _Red);
        _Left(_Z) = _Getnil();
        _Right(_Z) = _Getnil();
        adaptor.copy_construct(addressof(_Value(_Z)), value);
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
    void _Lrotate(node_type* _X)
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
    node_type* _Buynode(node_type* _Parg, _Redbl _Carg) NH3API_NOEXCEPT_ALLOC
    {
        node_type* _S = adaptor.template allocate_rebind<node_type>(1);
        _Parent(_S) = _Parg;
        _Color(_S) = _Carg;
        return _S;
    }

    void _Freenode(node_type* _S) NH3API_NOEXCEPT
    { adaptor.template deallocate_rebind<node_type>(_S, 1); }

    adaptor_type adaptor;
    key_compare  _KeyCompare;
    node_type*   _Head;
    bool         _Multi;
    size_type    _Size;
};
#pragma pack(pop)

} // namespace nh3api

template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator==(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (_X.size() == _Y.size() && ::std::equal(_X.begin(), _X.end(), _Y.begin()));
}
template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator!=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_X == _Y));
}
template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator<(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (::std::lexicographical_compare(_X.begin(), _X.end(), _Y.begin(), _Y.end()));
}
template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator>(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (_Y < _X);
}
template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator<=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_Y < _X));
}
template <class _K, class _Ty, class _Kfn, class _Pr, class _A, uintptr_t _Nil_Address, uintptr_t _Nilrefs_Address>
inline bool operator>=(
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_X,
    const nh3api::exe_rbtree<_K, _Ty, _Kfn, _Pr, _A, _Nil_Address, _Nilrefs_Address> &_Y)
{
    return (!(_X < _Y));
}

