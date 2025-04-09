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

#include "memory.hpp"            // exe_allocator
#include "iterator.hpp"          // container_iterator
#include "nh3api_exceptions.hpp" // exceptions

#pragma pack(push, 4)
//namespace nh3api
//{

/// @brief Visual C++ 6.0 std::deque implementation used by heroes3.exe
/// @tparam _Ty stored type
/// @tparam _A  allocator type
template<class _Ty, typename _A = exe_allocator<_Ty> >
class exe_deque
{
public:
    typedef _A allocator_type;

protected:
    typedef nh3api::allocator_adaptor<allocator_type> adaptor_type NH3API_NODEBUG;
    typedef exe_deque<_Ty, _A>    this_type NH3API_NODEBUG;
    typedef typename adaptor_type::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
    typedef typename adaptor_type::propagate_on_container_move_assignment propagate_on_container_move_assignment;
    typedef typename adaptor_type::propagate_on_container_swap propagate_on_container_swap;

public:
    typedef typename adaptor_type::size_type       size_type;
    typedef typename adaptor_type::difference_type difference_type;
    typedef typename adaptor_type::reference       reference;
    typedef typename adaptor_type::const_reference const_reference;
    typedef typename adaptor_type::value_type      value_type;
    typedef typename adaptor_type::pointer         pointer;
    typedef typename adaptor_type::const_pointer   const_pointer;

protected:
    typedef pointer* _Mapptr;

protected:
    enum deque_properties : unsigned
    { DEQUE_MAP_SIZE = 2, DEQUE_SIZE = (4096 < (difference_type)(sizeof(value_type)) ? 1 : 4096 / (difference_type)(sizeof(value_type))) };

public:
    class iterator;

protected:
    template<typename _ValueType = typename std::add_const<exe_deque<_Ty,_A>::value_type>::type>
    class _const_iterator :
    public nh3api::container_iterator<_ValueType, exe_deque<_Ty,_A>::difference_type, std::random_access_iterator_tag>
    {
    public:
        friend class exe_deque<_Ty, _A>;

    protected:
        pointer _First;
        pointer _Last;
        pointer _Next;
        _Mapptr _Map;

    public:
        NH3API_FORCEINLINE
        _const_iterator() NH3API_NOEXCEPT
            : _First(nullptr), _Last(nullptr), _Next(nullptr), _Map(nullptr)
        {
        }
        NH3API_FORCEINLINE
        _const_iterator( pointer _P, _Mapptr _M ) NH3API_NOEXCEPT
            : _First( *_M ), _Last( *_M + DEQUE_SIZE ),
            _Next( _P ), _Map( _M )
        {
        }
        NH3API_FORCEINLINE
        _const_iterator( const iterator& other ) NH3API_NOEXCEPT
            : _First( other._First ), _Last( other._Last ), _Next( other._Next ),
            _Map( other._Map )
        {
        }
        NH3API_FORCEINLINE
        _const_iterator(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {
            NH3API_IGNORE(_First, _Last, _Next, _Map);
        }
        NH3API_FORCEINLINE
        const_reference operator*() const NH3API_NOEXCEPT
        {
            return (*_Next);
        }
        NH3API_FORCEINLINE
        const_pointer operator->() const NH3API_NOEXCEPT
        {
            return (&**this);
        }
        NH3API_FORCEINLINE
        _const_iterator& operator++() NH3API_NOEXCEPT
        {
            if ( ++_Next == _Last )
            {
                _First = *++_Map;
                _Last = _First + DEQUE_SIZE;
                _Next = _First;
            }
            return (*this);
        }
        NH3API_FORCEINLINE
        _const_iterator operator++( int ) NH3API_NOEXCEPT
        {
            _const_iterator _Tmp = *this;
            ++*this;
            return (_Tmp);
        }
        NH3API_FORCEINLINE
        _const_iterator& operator--() NH3API_NOEXCEPT
        {
            if ( _Next == _First )
            {
                _First = *--_Map;
                _Last = _First + DEQUE_SIZE;
                _Next = _Last;
            }
            --_Next;
            return (*this);
        }
        NH3API_FORCEINLINE
        _const_iterator operator--( int ) NH3API_NOEXCEPT
        {
            _const_iterator _Tmp = *this;
            --* this;
            return (_Tmp);
        }
        _const_iterator& operator+=( difference_type _N ) NH3API_NOEXCEPT
        {
            _Add( _N );
            return (*this);
        }
        NH3API_FORCEINLINE
        _const_iterator& operator-=( difference_type _N ) NH3API_NOEXCEPT
        {
            return (*this += -_N);
        }
        NH3API_FORCEINLINE
        _const_iterator operator+( difference_type _N ) const NH3API_NOEXCEPT
        {
            _const_iterator _Tmp = *this;
            return (_Tmp += _N);
        }
        NH3API_FORCEINLINE
        _const_iterator operator-( difference_type _N ) const NH3API_NOEXCEPT
        {
            _const_iterator _Tmp = *this;
            return (_Tmp -= _N);
        }
        NH3API_FORCEINLINE
        difference_type operator-( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Map == other._Map ? _Next - other._Next
                        : DEQUE_SIZE * (_Map - other._Map - 1)
                        + (_Next - _First) + (other._Last - other._Next));
        }
        NH3API_FORCEINLINE
        const_reference operator[]( difference_type _N ) const NH3API_NOEXCEPT
        {
            return (*(*this + _N));
        }
        NH3API_FORCEINLINE
        bool operator==( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Next == other._Next);
        }
        NH3API_FORCEINLINE
        bool operator!=( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (!(*this == other));
        }
        NH3API_FORCEINLINE
        bool operator<( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Map < other._Map
                || (_Map == other._Map && _Next < other._Next));
        }
        NH3API_FORCEINLINE
        bool operator<=( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (!(other < *this));
        }
        NH3API_FORCEINLINE
        bool operator>( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (other < *this);
        }
        NH3API_FORCEINLINE
        bool operator>=( const _const_iterator& other ) const NH3API_NOEXCEPT
        {
            return (!(*this < other));
        }

    protected:
        NH3API_FORCEINLINE
        void _Add( difference_type _N ) NH3API_NOEXCEPT
        {
            difference_type _Off = _N + _Next - _First;
            difference_type _Moff = (0 <= _Off)
                ? _Off / DEQUE_SIZE
                : -((DEQUE_SIZE - 1 - _Off) / DEQUE_SIZE);
            if ( _Moff == 0 )
                _Next += _N;
            else
            {
                _Map += _Moff;
                _First = *_Map;
                _Last = _First + DEQUE_SIZE;
                _Next = _First + (_Off - _Moff * DEQUE_SIZE);
            }
        }
    };

public:
    typedef _const_iterator<typename std::add_const<exe_deque<_Ty,_A>::value_type>::type> const_iterator;

public:
    class iterator : public _const_iterator<exe_deque<_Ty,_A>::value_type>
    {
    private:
        typedef _const_iterator<exe_deque<_Ty,_A>::value_type> base_type NH3API_NODEBUG;
    public:
        NH3API_FORCEINLINE
        iterator() NH3API_NOEXCEPT
        {}

        NH3API_FORCEINLINE
        iterator( pointer _P, _Mapptr _M ) NH3API_NOEXCEPT
            : base_type(_P, _M)
        {}

        NH3API_FORCEINLINE
        iterator(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : base_type(tag)
        {}

    public:
        NH3API_FORCEINLINE
        reference operator*() const
        {
            return *(this->_Next);
        }
        NH3API_FORCEINLINE
        pointer operator->() const
        {
            return (&**this);
        }
        NH3API_FORCEINLINE
        iterator& operator++()
        {
            if ( ++this->_Next == this->_Last )
            {
                this->_First = *++this->_Map;
                this->_Last = this->_First + DEQUE_SIZE;
                this->_Next = this->_First;
            }
            return (*this);
        }
        NH3API_FORCEINLINE
        iterator operator++( int )
        {
            iterator _Tmp = *this;
            ++* this;
            return (_Tmp);
        }
        NH3API_FORCEINLINE
        iterator& operator--()
        {
            if ( this->_Next == this->_First )
            {
                this->_First = *--this->_Map;
                this->_Last = this->_First + DEQUE_SIZE;
                this->_Next = this->_Last;
            }
            --this->_Next;
            return (*this);
        }
        NH3API_FORCEINLINE
        iterator operator--( int )
        {
            iterator _Tmp = *this;
            --*this;
            return (_Tmp);
        }
        NH3API_FORCEINLINE
        iterator& operator+=( difference_type _N )
        {
            this->_Add( _N );
            return (*this);
        }
        NH3API_FORCEINLINE
        iterator& operator-=( difference_type _N )
        {
            return (*this += -_N);
        }
        NH3API_FORCEINLINE
        iterator operator+( difference_type _N ) const
        {
            iterator _Tmp = *this;
            return (_Tmp += _N);
        }
        NH3API_FORCEINLINE
        iterator operator-( difference_type _N ) const
        {
            iterator _Tmp = *this;
            return (_Tmp -= _N);
        }
        NH3API_FORCEINLINE
        difference_type operator-( const iterator& other ) const
        {
            return (this->_Map == other._Map ? this->_Next - other._Next
                        : DEQUE_SIZE * (this->_Map - other._Map - 1)
                        + (this->_Next - this->_First) + (other._Last - other._Next));
        }
        NH3API_FORCEINLINE
        reference operator[]( difference_type _N ) const
        {
            return (*(*this + _N));
        }
        NH3API_FORCEINLINE
        bool operator==( const iterator& other ) const
        {
            return (this->_Next == other._Next);
        }
        NH3API_FORCEINLINE
        bool operator!=( const iterator& other ) const
        {
            return (!(*this == other));
        }
        NH3API_FORCEINLINE
        bool operator<( const iterator& other ) const
        {
            return (this->_Map < other._Map
                || (this->_Map == other._Map && this->_Next < other._Next));
        }
        NH3API_FORCEINLINE
        bool operator<=( const iterator& other ) const
        {
            return (!(other < *this));
        }
        NH3API_FORCEINLINE
        bool operator>( const iterator& other ) const
        {
            return (other < *this);
        }
        NH3API_FORCEINLINE
        bool operator>=( const iterator& other ) const
        {
            return (!(*this < other));
        }
    };
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;

protected:
    // initialize from [_First, _Last), input iterators
    template<class IterT>
    void _Construct(IterT _Begin, IterT _End)
    {
        NH3API_IF_CONSTEXPR ( !nh3api::tt::allocator_may_throw<allocator_type>::value )
        {
            #if NH3API_STD_MOVE_SEMANTICS
            for (; _Begin != _End; ++_Begin)
                emplace_back(*_Begin);
            #else
            for (; _Begin != _End; ++_Begin)
                push_back(*_Begin);
            #endif
        }
        else
        {
            NH3API_TRY
            {
                #if NH3API_STD_MOVE_SEMANTICS
                for (; _Begin != _End; ++_Begin)
                    emplace_back(*_Begin);
                #else
                for (; _Begin != _End; ++_Begin)
                    push_back(*_Begin);
                #endif
            }
            NH3API_CATCH(...)
            {
                _Tidy();
                NH3API_RETHROW
            }
        }
    }

    void _Construct_n(size_type _Count, const value_type& _Val)
    {
        if (_Buy(_Count))
        {	// nonzero, fill it
            NH3API_IF_CONSTEXPR ( nh3api::tt::is_nothrow_copy_constructible<value_type>::value
                                  || !nh3api::tt::allocator_may_throw<allocator_type>::value )
            {
                for (; 0 < _Count; --_Count)
                    push_back(_Val);
            }
            else
            {
                NH3API_TRY
                {
                    for (; 0 < _Count; --_Count)
                        push_back(_Val);
                }
                NH3API_CATCH(...)
                {
                    _Tidy();
                    NH3API_RETHROW
                }
            }
        }
    }

public:
    exe_deque()
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<allocator_type>::value)
        : adaptor(), _First(), _Last(), _Map(nullptr), _Mapsize(nullptr), _Size(0)
    {}

    explicit exe_deque(const allocator_type& a)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<allocator_type>::value)
        : adaptor(a), _First(), _Last(), _Map(nullptr), _Mapsize(nullptr), _Size(0)
    {}

    explicit exe_deque( size_type _N, const value_type& _V = value_type(),
                        const _A& _Al = _A() )
        : adaptor( _Al ), _First(), _Last(), _Map(nullptr), _Mapsize(nullptr), _Size(0)
    { _Construct_n(_N, _V); }

    explicit exe_deque( size_type _N, const value_type& _V = value_type())
        : adaptor(), _First(), _Last(), _Map(nullptr), _Mapsize(nullptr), _Size(0)
    { _Construct_n(_N, _V); }

    exe_deque( const this_type& other )
        : adaptor(other.adaptor.select_on_container_copy_construction())
    { _Construct(other.begin(), other.end()); }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    exe_deque(IterT _F, IterT _L, const _A& _Al = _A()
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        : adaptor( _Al ),
        _First(), _Last(), _Map( nullptr ), _Mapsize( nullptr ), _Size( 0 )
    {
        _Construct(_F, _L);
        //std::copy( _F, _L, std::back_inserter( *this ) );
    }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    exe_deque(IterT _F, IterT _L
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        : adaptor(),
        _First(), _Last(), _Map( nullptr ), _Mapsize( nullptr ), _Size( 0 )
    {
        _Construct(_F, _L);
        //std::copy( _F, _L, std::back_inserter( *this ) );
    }

    exe_deque(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        : adaptor(tag), _First(tag), _Last(tag)
    { NH3API_IGNORE(_Map, _Mapsize, _Size); }

    ~exe_deque() NH3API_NOEXCEPT_DESTRUCT
    {
        _Tidy();
    }

    this_type& operator=( const this_type& value )
    {
        if ( this != &value )
        {
            iterator _S;
            if ( value.size() <= size() )
            {
                _S = std::copy( value.begin(), value.end(), begin() );
                erase( _S, end() );
            }
            else
            {
                const_iterator _Sx = value.begin() + size();
                _S = std::copy( value.begin(), _Sx, begin() );
                std::copy( _Sx, value.end(), std::inserter( *this, _S ) );
            }
        }
        return *this;
    }

    iterator begin() NH3API_NOEXCEPT
    {
        return (_First);
    }
    const_iterator begin() const NH3API_NOEXCEPT
    {
        return ((const_iterator)_First);
    }

    iterator end() NH3API_NOEXCEPT
    {
        return (_Last);
    }

    const_iterator cbegin() const NH3API_NOEXCEPT
    {
        return ((const_iterator)_First);
    }

    const_iterator cend() const NH3API_NOEXCEPT
    {
        return ((const_iterator)_Last);
    }

    reverse_iterator rbegin() NH3API_NOEXCEPT
    {
        return (reverse_iterator( end() ));
    }

    const_reverse_iterator rbegin() const NH3API_NOEXCEPT
    {
        return (const_reverse_iterator( end() ));
    }

    reverse_iterator rend() NH3API_NOEXCEPT
    {
        return (reverse_iterator( begin() ));
    }

    const_reverse_iterator rend() const NH3API_NOEXCEPT
    {
        return (const_reverse_iterator( begin() ));
    }
    const_reverse_iterator crbegin() const NH3API_NOEXCEPT
    {
        return (const_reverse_iterator( end() ));
    }
    const_reverse_iterator crend() const NH3API_NOEXCEPT
    {
        return (const_reverse_iterator( begin() ));
    }
    void resize( size_type _N, value_type value = value_type() )
    {
        if ( size() < _N )
            insert( end(), _N - size(), value );
        else if ( _N < size() )
            erase( begin() + _N, end() );
    }
    size_type size() const NH3API_NOEXCEPT
    {
        return (_Size);
    }
    size_type max_size() const NH3API_NOEXCEPT
    {
        return (adaptor.max_size());
    }
    bool empty() const NH3API_NOEXCEPT
    {
        return (size() == 0);
    }
    allocator_type get_allocator() const NH3API_NOEXCEPT
    {
        return adaptor.alloc;
    }
    const_reference at( size_type _P ) const
    {
        if ( size() <= _P )
            _Xran();
        return (*(begin() + _P));
    }
    reference at( size_type _P )
    {
        if ( size() <= _P )
            _Xran();
        return (*(begin() + _P));
    }
    const_reference operator[]( size_type _P ) const NH3API_NOEXCEPT
    {
    #if !NH3API_DEBUG
        return (*(begin() + _P));
    #else
        return at(_P);
    #endif
    }
    reference operator[]( size_type _P ) NH3API_NOEXCEPT
    {
    #if !NH3API_DEBUG
        return (*(begin() + _P));
    #else
        return at(_P);
    #endif
    }
    reference front() NH3API_NOEXCEPT
    {
        return (*begin());
    }
    const_reference front() const NH3API_NOEXCEPT
    {
        return (*begin());
    }
    reference back() NH3API_NOEXCEPT
    {
        return (*(end() - 1));
    }
    const_reference back() const NH3API_NOEXCEPT
    {
        return (*(end() - 1));
    }
    void push_front(const value_type& value)
    {
        if ( empty() || _First._Next == _First._First )
            _Buyfront();
        adaptor.copy_construct( --_First._Next, value );
        ++_Size;
    }
    void pop_front() NH3API_NOEXCEPT_DESTRUCT
    {
        adaptor.destroy( _First._Next++ );
        --_Size;
        if ( empty() || _First._Next == _First._Last )
            _Freefront();
    }
    void push_back( const value_type& value )
    {
        if ( empty() || (_Last._Next == _Last._Last) )
        {
            _Buyback();
            adaptor.copy_construct( _Last._Next++, value );
        }
        else if ( _Last._Next + 1 == _Last._Last )
        {
            adaptor.copy_construct( _Last._Next++, value );
            _Buyback();
        }
        else
            adaptor.copy_construct( _Last._Next++, value );
        ++_Size;
    }

#if NH3API_STD_MOVE_SEMANTICS
    template<class ...Args>
    reference emplace_back(Args&& ...args)
    {
        if ( empty() || (_Last._Next == _Last._Last) )
        {
            _Buyback();
            adaptor.construct( _Last._Next++, std::forward<Args>(args)...);
        }
        else if ( _Last._Next + 1 == _Last._Last )
        {
            adaptor.construct( _Last._Next++, std::forward<Args>(args)...);
            _Buyback();
        }
        else
            adaptor.construct( _Last._Next++, std::forward<Args>(args)...);
        ++_Size;
        return back();
    }

    void push_back( value_type&& value )
    { emplace_back(std::forward(value)); }

    template<class ... Args>
    reference emplace_front(Args&& ... args)
    {
        if ( empty() || _First._Next == _First._First )
            _Buyfront();
        adaptor.construct( --_First._Next, std::forward<Args>(args) ...);
        ++_Size;
        return front();
    }

    void push_front( value_type&& value )
    { emplace_front(std::move(value)); }

    template<class ... Args>
    iterator emplace( iterator _P, Args&& ... args )
    {
        if ( _P == begin() )
        {
            emplace_front(std::forward<Args>(args) ...);
            return (begin());
        }
        else if ( _P == end() )
        {
            emplace_back(std::forward<Args>(args) ...);
            return (end() - 1);
        }
        else
        {
            iterator _S;
            size_type _Off = _P - begin();
            if ( _Off < size() / 2 )
            {
                push_front( front() );
                _S = begin() + _Off;
                std::copy( begin() + 2, _S + 1, begin() + 1 );
            }
            else
            {
                push_back( back() );
                _S = begin() + _Off;
                std::copy_backward( _S, end() - 2, end() - 1 );
            }
            adaptor.construct(_S._Next, std::forward<Args>(args) ...);
            return (_S);
        }
    }
#endif

    void pop_back()
    {
        if ( _Last._Next == _Last._First )
            _Freeback();
        if ( !empty() )
            adaptor.destroy( --_Last._Next );
        --_Size;
        if ( empty() )
            _Freeback();
    }
    void assign( const_iterator _F, const_iterator _L )
    {
        erase( begin(), end() );
        insert( begin(), _F, _L );
    }
    void assign( size_type _N, const value_type& value = value_type() )
    {
        erase( begin(), end() );
        insert( begin(), _N, value );
    }
    iterator insert( iterator _P, const value_type& value = value_type() )
    {
        if ( _P == begin() )
        {
            push_front( value );
            return (begin());
        }
        else if ( _P == end() )
        {
            push_back( value );
            return (end() - 1);
        }
        else
        {
            iterator _S;
            size_type _Off = _P - begin();
            if ( _Off < size() / 2 )
            {
                push_front( front() );
                _S = begin() + _Off;
                std::copy( begin() + 2, _S + 1, begin() + 1 );
            }
            else
            {
                push_back( back() );
                _S = begin() + _Off;
                std::copy_backward( _S, end() - 2, end() - 1 );
            }
            *_S = value;
            return (_S);
        }
    }

    void insert( iterator _P, size_type _M, const value_type& value )
    {
        iterator _S;
        size_type _I;
        size_type _Off = _P - begin();
        size_type _Rem = _Size - _Off;
        if ( _Off < _Rem )
            if ( _Off < _M )
            {
                for ( _I = _M - _Off; 0 < _I; --_I )
                    push_front( value );
                for ( _I = _Off; 0 < _I; --_I )
                    push_front( begin()[_M - 1] );
                _S = begin() + _M;
                std::fill( _S, _S + _Off, value );
            }
            else
            {
                for ( _I = _M; 0 < _I; --_I )
                    push_front( begin()[_M - 1] );
                _S = begin() + _M;
                _Umove( _S + _M, _S + _Off, _S );
                std::fill( begin() + _Off, _S + _Off, value );
            }
        else
            if ( _Rem < _M )
            {
                for ( _I = _M - _Rem; 0 < _I; --_I )
                    push_back( value );
                for ( _I = 0; _I < _Rem; ++_I )
                    push_back( begin()[_Off + _I] );
                _S = begin() + _Off;
                std::fill( _S, _S + _Rem, value );
            }
            else
            {
                for ( _I = 0; _I < _M; ++_I )
                    push_back( begin()[_Off + _Rem - _M + _I] );
                _S = begin() + _Off;
                std::copy_backward( _S, _S + _Rem - _M, _S + _Rem );
                std::fill( _S, _S + _M, value );
            }
    }
    void insert( iterator _P, const_iterator _F, const_iterator _L )
    {
        size_type _M = std::distance(_F, _L);
        size_type _I;
        size_type _Off = _P - begin();
        size_type _Rem = _Size - _Off;
        if ( _Off < _Rem )
            if ( _Off < _M )
            {
                const_iterator _Qx = _F;
                std::advance( _Qx, _M - _Off );
                for ( const_iterator _Q = _Qx; _F != _Q; )
                    push_front( *--_Q );
                for ( _I = _Off; 0 < _I; --_I )
                    push_front( begin()[_M - 1] );
                std::copy( _Qx, _L, begin() + _M );
            }
            else
            {
                for ( _I = _M; 0 < _I; --_I )
                    push_front( begin()[_M - 1] );
                iterator _S = begin() + _M;
                std::copy( _S + _M, _S + _Off, _S );
                std::copy( _F, _L, begin() + _Off );
            }
        else
            if ( _Rem < _M )
            {
                const_iterator _Qx = _F;
                std::advance( _Qx, _Rem );
                for ( const_iterator _Q = _Qx; _Q != _L; ++_Q )
                    push_back( *_Q );
                for ( _I = 0; _I < _Rem; ++_I )
                    push_back( begin()[_Off + _I] );
                std::copy( _F, _Qx, begin() + _Off );
            }
            else
            {
                for ( _I = 0; _I < _M; ++_I )
                    push_back( begin()[_Off + _Rem - _M + _I] );
                iterator _S = begin() + _Off;
                std::copy_backward( _S, _S + _Rem - _M, _S + _Rem );
                std::copy( _F, _L, _S );
            }
    }
    iterator erase( iterator _P ) NH3API_NOEXCEPT_DESTRUCT
    {
        return (erase( _P, _P + 1 ));
    }
    iterator erase( iterator _F, iterator _L ) NH3API_NOEXCEPT_DESTRUCT
    {
        size_type _N = _L - _F;
        size_type _M = _F - begin();
        if ( _M < end() - _L )
        {
            std::copy_backward( begin(), _F, _L );
            for ( ; 0 < _N; --_N )
                pop_front();
        }
        else
        {
            std::copy( _L, end(), _F );
            for ( ; 0 < _N; --_N )
                pop_back();
        }
        return (_M == 0 ? begin() : begin() + _M);
    }
    void clear() NH3API_NOEXCEPT_DESTRUCT
    {
        erase( begin(), end() );
    }
    void swap( this_type& other )
    NH3API_NOEXCEPT_EXPR(adaptor_type::is_always_equal::value)
    {
        if ( adaptor.alloc == other.adaptor.alloc )
        {
            std::swap(_First, other._First);
            std::swap(_Last, other._Last);
            std::swap(_Map, other._Map);
            std::swap(_Mapsize, other._Mapsize);
            std::swap(_Size, other._Size);
        }
        else
        {
            this_type _Ts = *this;
            *this = other;
            other = _Ts;
        }
    }

protected:
    template<class _Iter>
    pointer _Umove(_Iter _Begin, _Iter _End, pointer _Ptr)
    {
        #if !NH3API_STD_MOVE_SEMANTICS
            return std::copy(_Begin, _End, _Ptr);
        #else
        if ( nh3api::tt::is_move_assignable<value_type>::value )
            return std::move(_Begin, _End, _Ptr);

        else if ( nh3api::tt::is_copy_assignable<value_type>::value )
            return std::copy(_Begin, _End, _Ptr);
        else
            assert("deque::value_type must be either copy or move assignable"&&0);
        #endif
    }

    template<class _Iter>
    pointer _UmoveBackward(_Iter _Begin, _Iter _End, pointer _Ptr)
    {
        #if !NH3API_STD_MOVE_SEMANTICS
            return std::copy_backward(_Begin, _End, _Ptr);
        #else
        if ( nh3api::tt::is_move_assignable<value_type>::value )
            return std::move_backward(_Begin, _End, _Ptr);

        else if ( nh3api::tt::is_copy_assignable<value_type>::value )
            return std::copy_backward(_Begin, _End, _Ptr);
        else
            assert("deque::value_type must be either copy or move assignable" && false);
        #endif
    }

    void _Buyback()
    {
        pointer _P = adaptor.allocate( DEQUE_SIZE, nullptr );
        if ( empty() )
        {
            _Mapsize = DEQUE_MAP_SIZE;
            size_type _N = _Mapsize / 2;
            _Getmap();
            *(_Map + _N) = _P;
            _First = iterator( _P + DEQUE_SIZE / 2, _Map + _N );
            _Last = _First;
        }
        else if ( _Last._Map < _Map + (_Mapsize - 1) )
        {
            *(++_Last._Map) = _P;
            _Last = iterator( _P, _Last._Map );
        }
        else
        {
            difference_type _I = _Last._Map - _First._Map + 1;
            _Mapptr _M = _Growmap( 2 * _I );
            *(_M + _I) = _P;
            _First = iterator( _First._Next, _M );
            _Last = iterator( _P, _M + _I );
        }
    }
    void _Buyfront()
    {
        pointer _P = adaptor.allocate( DEQUE_SIZE, nullptr );
        if ( empty() )
        {
            _Mapsize = DEQUE_MAP_SIZE;
            size_type _N = _Mapsize / 2;
            _Getmap();
            *(_Map + _N) = _P;
            _First = iterator( _P + (DEQUE_SIZE / 2 + 1),
                                _Map + _N );
            _Last = _First;
        }
        else if ( _Map < _First._Map )
        {
            *(--_First._Map) = _P;
            _First = iterator( _P + DEQUE_SIZE, _First._Map );
        }
        else if ( _Last._Map == _First._Map )
        {
            *(_Last._Map++) = *_First._Map ;
            *(_First._Map + 1) = *_First._Map ;
            *(_First._Map) = _P ;
            _First = iterator( _P + DEQUE_SIZE, _First._Map );
        }
        else
        {
            difference_type _I = _Last._Map - _First._Map + 1;
            _Mapptr _M = _Growmap( 2 * _I );
            *( --_M) = _P;
            _First = iterator( _P + DEQUE_SIZE, _M );
            _Last = iterator( _Last._Next, _M + _I );
        }
    }
    void _Freeback()
    {
        _Freeptr( _Last._Map-- );
        if ( empty() )
        {
            if ( _First._Map == _Last._Map )
                _Freeptr( _First._Map );
            _First = iterator();
            _Last = _First;
            _Freemap();
        }
        else
            _Last = iterator( *_Last._Map + DEQUE_SIZE,
                                _Last._Map );
    }
    void _Freefront()
    {
        _Freeptr( _First._Map++ );
        if ( empty() )
        {
            _First = iterator();
            _Last = _First;
            _Freemap();
        }
        else
        {
            _First = iterator( *_First._Map, _First._Map );
        }

    }
    static void _Xran()
    {
        NH3API_THROW(std::out_of_range, "invalid deque position");
    }
    void _Freemap()
    {
        adaptor.template deallocate_rebind<unsigned char>
            (reinterpret_cast<unsigned char*>(_Map), _Mapsize * sizeof(pointer));
    }
    void _Freeptr( _Mapptr _M )
    {
        adaptor.deallocate( *_M, DEQUE_SIZE );
    }
    void _Getmap()
    {
        _Map = (_Mapptr)adaptor.template allocate_rebind<unsigned char>(_Mapsize * sizeof(pointer));
    }
    _Mapptr _Growmap( size_type _Newsize )
    {
        _Mapptr _M = (_Mapptr)adaptor.template allocate_rebind<unsigned char>(_Newsize * sizeof(pointer));
        std::copy( _First._Map, _Last._Map + 1,
                _M + _Newsize / 4 );
        adaptor.deallocate( _Map, _Mapsize );
        _Map = _M;
        _Mapsize = _Newsize;
        return (_M + _Newsize / 4);
    }

    void _Tidy() NH3API_NOEXCEPT_DESTRUCT
    {
        while (!empty())
            pop_front();
    }

protected:
    adaptor_type adaptor;
    iterator _First;
    iterator _Last;
    _Mapptr _Map;
    size_type _Mapsize;
    size_type _Size;
};

#pragma pack(pop)

    // exe_deque TEMPLATE OPERATORS
template<class _Ty, class _A> inline
    bool operator==( const exe_deque<_Ty, _A>& lhs,
                        const exe_deque<_Ty, _A>& rhs )
{
    return (lhs.size() == rhs.size()
                && std::equal( lhs.begin(), lhs.end(), rhs.begin() ));
}
template<class _Ty, class _A> inline
    bool operator!=( const exe_deque<_Ty, _A>& lhs,
                        const exe_deque<_Ty, _A>& rhs )
{
    return (!(lhs == rhs));
}
template<class _Ty, class _A> inline
    bool operator<( const exe_deque<_Ty, _A>& lhs,
                    const exe_deque<_Ty, _A>& rhs )
{
    return (std::lexicographical_compare( lhs.begin(), lhs.end(),
                rhs.begin(), rhs.end() ));
}
template<class _Ty, class _A> inline
    bool operator<=( const exe_deque<_Ty, _A>& lhs,
                        const exe_deque<_Ty, _A>& rhs )
{
    return (!(rhs < lhs));
}
template<class _Ty, class _A> inline
    bool operator>( const exe_deque<_Ty, _A>& lhs,
                    const exe_deque<_Ty, _A>& rhs )
{
    return (rhs < lhs);
}
template<class _Ty, class _A> inline
    bool operator>=( const exe_deque<_Ty, _A>& lhs,
                        const exe_deque<_Ty, _A>& rhs )
{
    return (!(lhs < rhs));
}
//}
