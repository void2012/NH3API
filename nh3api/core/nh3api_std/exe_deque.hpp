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
/// @brief Visual C++ 6.0 std::deque implementation used by heroes3.exe
/// @tparam T stored type
template<class T>
class exe_deque
{
public:
    typedef exe_allocator<T> allocator_type;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

protected:
    typedef pointer* _Mapptr;

protected:
    enum deque_properties : signed
    { 
        DEQUE_MAP_SIZE = 2, 
        DEQUE_SIZE = (4096 < (difference_type)(sizeof(value_type)) ? 1 : 4096 / (difference_type)(sizeof(value_type))) 
    };

public:
    class iterator;

protected:
    template<typename _ValueType = typename nh3api::tt::add_const<typename exe_deque<T>::value_type>::type>
    class deque_iterator :
    public nh3api::container_iterator<_ValueType, typename exe_deque<T>::difference_type, std::random_access_iterator_tag>
    {
    public:
        friend class exe_deque<T>;

    protected:
        pointer _First;
        pointer _Last;
        pointer _Next;
        _Mapptr _Map;

    public:
        NH3API_FORCEINLINE
        deque_iterator() NH3API_NOEXCEPT
            : _First(nullptr), _Last(nullptr), _Next(nullptr), _Map(nullptr)
        {
        }
        NH3API_FORCEINLINE
        deque_iterator( pointer _P, _Mapptr _M ) NH3API_NOEXCEPT
            : _First( *_M ), _Last( *_M + DEQUE_SIZE ),
            _Next( _P ), _Map( _M )
        {
        }
        NH3API_FORCEINLINE
        deque_iterator( const iterator& other ) NH3API_NOEXCEPT
            : _First( other._First ), _Last( other._Last ), _Next( other._Next ),
            _Map( other._Map )
        {
        }
        NH3API_FORCEINLINE
        deque_iterator(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
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
        deque_iterator& operator++() NH3API_NOEXCEPT
        {
            if ( ++_Next == _Last )
            {
                ++_Map;
                _First = *_Map;
                _Last = _First + DEQUE_SIZE;
                _Next = _First;
            }
            return (*this);
        }
        NH3API_FORCEINLINE
        deque_iterator operator++( int ) NH3API_NOEXCEPT
        {
            deque_iterator temp = *this;
            ++*this;
            return (temp);
        }
        NH3API_FORCEINLINE
        deque_iterator& operator--() NH3API_NOEXCEPT
        {
            if ( _Next == _First )
            {
                --_Map;
                _First = *_Map;
                _Last = _First + DEQUE_SIZE;
                _Next = _Last;
            }
            --_Next;
            return (*this);
        }
        NH3API_FORCEINLINE
        deque_iterator operator--( int ) NH3API_NOEXCEPT
        {
            deque_iterator temp = *this;
            --* this;
            return (temp);
        }
        deque_iterator& operator+=( difference_type _N ) NH3API_NOEXCEPT
        {
            _Add( _N );
            return (*this);
        }
        NH3API_FORCEINLINE
        deque_iterator& operator-=( difference_type _N ) NH3API_NOEXCEPT
        {
            return (*this += -_N);
        }
        NH3API_FORCEINLINE
        deque_iterator operator+( difference_type _N ) const NH3API_NOEXCEPT
        {
            deque_iterator temp = *this;
            return (temp += _N);
        }
        NH3API_FORCEINLINE
        deque_iterator operator-( difference_type _N ) const NH3API_NOEXCEPT
        {
            deque_iterator temp = *this;
            return (temp -= _N);
        }
        NH3API_FORCEINLINE
        difference_type operator-( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Map == other._Map ? _Next - other._Next
                        : DEQUE_SIZE * (_Map - other._Map)
                        + (_Next - _First) - (other._Next - other._First));
        }
        NH3API_FORCEINLINE
        const_reference operator[]( difference_type _N ) const NH3API_NOEXCEPT
        {
            return (*(*this + _N));
        }
        NH3API_FORCEINLINE
        bool operator==( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Next == other._Next && _Map == other._Map);
        }
        NH3API_FORCEINLINE
        bool operator!=( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (!(*this == other));
        }
        NH3API_FORCEINLINE
        bool operator<( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (_Map < other._Map
                || (_Map == other._Map && _Next < other._Next));
        }
        NH3API_FORCEINLINE
        bool operator<=( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (!(other < *this));
        }
        NH3API_FORCEINLINE
        bool operator>( const deque_iterator& other ) const NH3API_NOEXCEPT
        {
            return (other < *this);
        }
        NH3API_FORCEINLINE
        bool operator>=( const deque_iterator& other ) const NH3API_NOEXCEPT
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
    typedef deque_iterator<typename nh3api::tt::add_const<typename exe_deque<T>::value_type>::type> const_iterator;

public:
    class iterator : public deque_iterator<typename exe_deque<T>::value_type>
    {
    private:
        typedef deque_iterator<typename exe_deque<T>::value_type> base_type;
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
            iterator temp = *this;
            ++* this;
            return (temp);
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
            iterator temp = *this;
            --*this;
            return (temp);
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
            iterator temp = *this;
            return (temp += _N);
        }
        NH3API_FORCEINLINE
        iterator operator-( difference_type _N ) const
        {
            iterator temp = *this;
            return (temp -= _N);
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
    // initialize from [_Begin, _End), input iterators
    template<class IterT>
    void _Construct(IterT _Begin, IterT _End)
    {
        NH3API_TRY
        {
            insert(begin(), _Begin, _End);
        }
        NH3API_CATCH(...)
        {
            _Tidy();
            NH3API_RETHROW
        }
    }

    void _Construct_n(size_type _Count, const value_type& _Val)
    {
        NH3API_TRY
        {
            _Insert_n(begin(), _Count, _Val);
        }
        NH3API_CATCH(...)
        {
            _Tidy();
            NH3API_RETHROW
        }
    }

public:
    exe_deque() NH3API_NOEXCEPT
        : _Dummy(0), _First(), _Last(), _Map(nullptr), _Mapsize(0), _Size(0)
    {}

    explicit exe_deque(const allocator_type& a) NH3API_NOEXCEPT
        : _Dummy(0), _First(), _Last(), _Map(nullptr), _Mapsize(0), _Size(0)
    {}

    explicit exe_deque( size_type _N, const value_type& _V,
                        const allocator_type& _Al = allocator_type() )
        : _Dummy(0), _First(), _Last(), _Map(nullptr), _Mapsize(0), _Size(0)
    { _Construct_n(_N, _V); }

    explicit exe_deque( size_type _N, const value_type& _V = value_type())
        : _Dummy(0), _First(), _Last(), _Map(nullptr), _Mapsize(0), _Size(0)
    { _Construct_n(_N, _V); }

    exe_deque( const exe_deque& other )
        : _Dummy(0)
    { _Construct(other.begin(), other.end()); }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    exe_deque(IterT first, IterT last, const allocator_type& _Al = allocator_type()
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        : _Dummy(0), _First(), _Last(), _Map( nullptr ), _Mapsize( 0 ), _Size( 0 )
    {
        _Construct(first, last);
        //std::copy( first, last, std::back_inserter( *this ) );
    }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    exe_deque(IterT first, IterT last
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        : _Dummy(0), _First(), _Last(), _Map( nullptr ), _Mapsize( 0 ), _Size( 0 )
    {
        _Construct(first, last);
        //std::copy( first, last, std::back_inserter( *this ) );
    }

    exe_deque(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        : _First(tag), _Last(tag)
    { NH3API_IGNORE(_Dummy, _Map, _Mapsize, _Size); }

    ~exe_deque() NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        _Tidy();
    }

    exe_deque& operator=( const exe_deque& value )
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
            _Insert_n( end(), _N - size(), value );
        else if ( _N < size() )
            erase( begin() + _N, end() );
    }

    size_type size() const NH3API_NOEXCEPT
    {
        return (_Size);
    }

    NH3API_NODISCARD NH3API_FORCEINLINE NH3API_CONSTEXPR
    static size_type max_size() NH3API_NOEXCEPT
    { return NH3API_MAX_HEAP_REQUEST / sizeof(value_type); }

    bool empty() const NH3API_NOEXCEPT
    {
        return (size() == 0);
    }

    allocator_type get_allocator() const NH3API_NOEXCEPT
    { return allocator_type(); }

    const_reference at( size_type _P ) const
    {
        if ( size() <= _P )
            _Throw_invalid_subscript();
        return (*(begin() + _P));
    }
    reference at( size_type _P )
    {
        if ( size() <= _P )
            _Throw_invalid_subscript();
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
        nh3api::construct_at( --_First._Next, value );
        ++_Size;
    }

    void pop_front() NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        nh3api::destroy_at(_First._Next++);
        --_Size;
        if ( empty() || _First._Next == _First._Last )
            _Freefront();
    }
    void push_back( const value_type& value )
    {
        if ( empty() || (_Last._Next == _Last._Last) )
        {
            _Buyback();
            nh3api::construct_at( _Last._Next++, value );
        }
        else if ( _Last._Next + 1 == _Last._Last )
        {
            nh3api::construct_at( _Last._Next++, value );
            _Buyback();
        }
        else
            nh3api::construct_at( _Last._Next++, value );
        ++_Size;
    }

#if NH3API_STD_MOVE_SEMANTICS
    exe_deque(exe_deque&& other) NH3API_NOEXCEPT
    { nh3api::trivial_move<sizeof(exe_deque)>(&other, this); }

    exe_deque& operator=(exe_deque&& other) NH3API_NOEXCEPT
    {
        if (this != &other)
        {
            _Tidy();
            nh3api::trivial_move<sizeof(exe_deque)>(&other, this);
        }
        return *this;
    }

    template<class ...Args>
    reference emplace_back(Args&& ...args)
    {
        if ( empty() || (_Last._Next == _Last._Last) )
        {
            _Buyback();
            nh3api::construct_at(_Last._Next++, std::forward<Args>(args)...);
        }
        else if ( _Last._Next + 1 == _Last._Last )
        {
            nh3api::construct_at(_Last._Next++, std::forward<Args>(args)...);
            _Buyback();
        }
        else
            nh3api::construct_at(_Last._Next++, std::forward<Args>(args)...);
        ++_Size;
        return back();
    }

    void push_back( value_type&& value )
    { emplace_back(std::forward<value_type>(value)); }

    template<class ... Args>
    reference emplace_front(Args&& ... args)
    {
        if ( empty() || _First._Next == _First._First )
            _Buyfront();
        nh3api::construct_at(--_First._Next, std::forward<Args>(args) ...);
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
            nh3api::construct_at( _S._Next, std::forward<Args>(args)...);
            return (_S);
        }
    }
#endif // move semantics

    void shrink_to_fit()
    {	// reduce capacity
        exe_deque temp(*this);
        swap(temp);
    }

    void pop_back()
    {
        if ( _Last._Next == _Last._First )
            _Freeback();
        if ( !empty() )
            nh3api::destroy_at(--_Last._Next );
        --_Size;
        if ( empty() )
            _Freeback();
    }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    void assign(IterT first, IterT last
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
    {
        nh3api::verify_range(first, last);
        erase( begin(), end() );
        insert( begin(), first, last );
    }

    void assign( size_type _Count, const value_type& value )
    {
        _Assign_n(_Count, value);
    }
    iterator insert( iterator pos, const value_type& value )
    {
        if ( pos == begin() )
        {
            push_front( value );
            return (begin());
        }
        else if ( pos == end() )
        {
            push_back( value );
            return (end() - 1);
        }
        else
        {
            iterator _S;
            size_type _Off = pos - begin();
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

    void insert( iterator pos, size_type _Count, const value_type& value)
    {
        _Insert_n(pos, _Count, value);
    }

    template<class IterT
                 NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    void insert(const_iterator _Where, const_iterator first, const_iterator last NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
    {
        size_type _M = std::distance(first, last);
        size_type _I;
        size_type _Off = _Where - begin();
        size_type _Rem = _Size - _Off;
        if ( _Off < _Rem )
            if ( _Off < _M )
                {
                    const_iterator _Qx = first;
                    std::advance(_Qx, _M - _Off);
                    for ( const_iterator _Q = _Qx; first != _Q; )
                        push_front(*--_Q);
                    for ( _I = _Off; 0 < _I; --_I )
                        push_front(begin()[_M - 1]);
                    std::copy(_Qx, last, begin() + _M);
                }
            else
                {
                    for ( _I = _M; 0 < _I; --_I )
                        push_front(begin()[_M - 1]);
                    iterator _S = begin() + _M;
                    std::copy(_S + _M, _S + _Off, _S);
                    std::copy(first, last, begin() + _Off);
                }
        else if ( _Rem < _M )
            {
                const_iterator _Qx = first;
                std::advance(_Qx, _Rem);
                for ( const_iterator _Q = _Qx; _Q != last; ++_Q )
                    push_back(*_Q);
                for ( _I = 0; _I < _Rem; ++_I )
                    push_back(begin()[_Off + _I]);
                std::copy(first, _Qx, begin() + _Off);
            }
        else
            {
                for ( _I = 0; _I < _M; ++_I )
                    push_back(begin()[_Off + _Rem - _M + _I]);
                iterator _S = begin() + _Off;
                std::copy_backward(_S, _S + _Rem - _M, _S + _Rem);
                std::copy(first, last, _S);
            }
    }
    iterator erase( iterator _P ) NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        return (erase( _P, _P + 1 ));
    }
    iterator erase( iterator first, iterator last ) NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        size_type _N = last - first;
        size_type _M = first - begin();
        if ( _M < end() - last )
        {
            std::copy_backward( begin(), first, last );
            for ( ; 0 < _N; --_N )
                pop_front();
        }
        else
        {
            std::copy( last, end(), first );
            for ( ; 0 < _N; --_N )
                pop_back();
        }
        return (_M == 0 ? begin() : begin() + _M);
    }
    void clear() NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        erase( begin(), end() );
    }
    void swap( exe_deque& other ) NH3API_NOEXCEPT
    {
        if ( this != &other )
        {
            nh3api::trivial_swap<sizeof(exe_deque)>(this, &other);
        }
    }

protected:
    template<class IterT> static
    pointer _Move(IterT _Begin, IterT _End, pointer _Ptr)
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

    template<class IterT> static
    pointer _Move_backward(IterT _Begin, IterT _End, pointer _Ptr)
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

    void _Assign_n(size_type _Count, const value_type& _Val)
    {	// assign _Count * _Val
        value_type temp = _Val;	// in case _Val is in sequence
        erase(begin(), end());
        _Insert_n(begin(), _Count, temp);
    }

    void _Insert_n(const_iterator _Where, size_type _Count,
                   const value_type &_Val) { // insert _Count * _Val at _Where
      iterator _Mid;
      size_type _Num;
      size_type _Off = _Where - begin();
      size_type _Rem = this->_Size - _Off;
      size_type _Oldsize = this->_Size;

      if (_Off < _Rem) { // closer to front
        NH3API_TRY {
          if (_Off < _Count) { // insert longer than prefix
            for (_Num = _Count - _Off; 0 < _Num; --_Num)
              push_front(_Val); // push excess values
            for (_Num = _Off; 0 < _Num; --_Num)
              push_front(front()); // push prefix

            _Mid = begin() + _Count;
            std::fill(_Mid, _Mid + _Off,
                      _Val); // fill in rest of values
          } else {           // insert not longer than prefix
            for (_Num = _Count; 0 < _Num; --_Num)
              push_front(front()); // push part of prefix

            _Mid = begin() + _Count;
            value_type temp = _Val; // in case _Val is in sequence
            _Move(begin() + _Count, begin() + _Off, begin());      // copy rest of prefix
            std::fill(begin() + _Off, _Mid + _Off, temp); // fill in values
          }
        }
        NH3API_CATCH(...) {
          for (; _Oldsize < this->_Size;)
            pop_front(); // restore old size, at least
          NH3API_RETHROW
        }
      } else { // closer to back
        NH3API_TRY {
          if (_Rem < _Count) { // insert longer than suffix
            for (_Num = _Count - _Rem; 0 < _Num; --_Num)
              push_back(_Val); // push excess values
            for (_Num = 0; _Num < _Rem; ++_Num)
              push_back(begin()[_Off + _Num]); // push suffix

            _Mid = begin() + _Off;
            std::fill(_Mid, _Mid + _Rem,
                      _Val); // fill in rest of values
          } else {           // insert not longer than prefix
            for (_Num = 0; _Num < _Count; ++_Num)
              push_back(
                  begin()[_Off + _Rem - _Count + _Num]); // push part of prefix

            _Mid = begin() + _Off;
            value_type temp = _Val; // in case _Val is in sequence
            _Move_backward(begin() + _Off, begin() + _Rem - _Count,
                           begin() + _Rem); // copy rest of prefix
            std::fill(_Mid, _Mid + _Count,
                      temp); // fill in values
          }
        }
        NH3API_CATCH(...) {
          for (; _Oldsize < this->_Size;)
            pop_back(); // restore old size, at least
          NH3API_RETHROW
        }
      }
    }

    void _Buyback()
    {
        pointer _P = static_cast<pointer>(::operator new(DEQUE_SIZE * sizeof(value_type), exe_heap, std::nothrow));
        if ( empty() )
        {
            _Mapsize     = DEQUE_MAP_SIZE;
            size_type _N = _Mapsize / 2;
            _Getmap();
            *(_Map + _N) = _P;
            _First = iterator(_P + DEQUE_SIZE / 2, _Map + _N);
            _Last  = _First;
        }
        else if ( _Last._Map < _Map + (_Mapsize - 1) )
        {
            *(++_Last._Map) = _P;
            _Last = iterator(_P, _Last._Map);
        }
        else
        {
            difference_type _I = _Last._Map - _First._Map + 1;
            _Mapptr         _M = _Growmap(2 * _I);
            *(_M + _I)         = _P;
            _First = iterator(_First._Next, _M);
            _Last  = iterator(_P, _M + _I);
        }
    }

    void _Buyfront()
    {
        pointer _P = static_cast<pointer>(::operator new(DEQUE_SIZE * sizeof(value_type), exe_heap, std::nothrow));
        if ( empty() )
        {
            _Mapsize     = DEQUE_MAP_SIZE;
            size_type _N = _Mapsize / 2;
            _Getmap();
            *(_Map + _N) = _P;
            _First = iterator(_P + (DEQUE_SIZE / 2 + 1),
                                _Map + _N);
            _Last  = _First;
        }
        else if ( _Map < _First._Map )
        {
            *(--_First._Map) = _P;
            _First = iterator(_P + DEQUE_SIZE, _First._Map);
        }
        else if ( _Last._Map == _First._Map )
        {
            *(_Last._Map++) = *_First._Map;
            *(_First._Map + 1) = *_First._Map;
            *(_First._Map)     = _P;
            _First = iterator(_P + DEQUE_SIZE, _First._Map);
        }
        else
        {
            difference_type _I = _Last._Map - _First._Map + 1;
            _Mapptr         _M = _Growmap(2 * _I);
            *(--_M)            = _P;
            _First = iterator(_P + DEQUE_SIZE, _M);
            _Last  = iterator(_Last._Next, _M + _I);
        }
    }
    void _Freeback()
    {
        nh3api::destroy_at(--_Last._Next);
        if (_Last._Next == _Last._First) 
        {
            _Freeptr( _Last._Map );
            if ( _First._Map == _Last._Map )
            {
                _Freemap();
            }
            else
            {
                --_Last._Map;
            }
        }
    }
    void _Freefront()
    {
        nh3api::destroy_at(_First._Next);
        _First._Next++;
        if ( _First._Next == _First._Last)
        {
            _Freeptr( _First._Map );
            if ( _First._Map == _Last._Map )
            {
                _Freemap();
            }
            else
            {
                ++_First._Map;
            }
        }
    }
    
    static void _Throw_invalid_subscript()
    { NH3API_THROW(std::out_of_range, "invalid deque position"); }

    void _Freemap()
    { 
        _Freeptr(_Map); 
        _Map = nullptr;
        _Mapsize = 0;
    }

    static void _Freeptr( _Mapptr _M ) NH3API_NOEXCEPT
    { ::operator delete(_M, exe_heap); }

    void _Getmap() NH3API_NOEXCEPT
    { 
        _Map = static_cast<_Mapptr>(::operator new(_Mapsize * sizeof(pointer), exe_heap, std::nothrow)); 
        std::memset(_Map, 0, _Mapsize * sizeof(pointer));
    }
    
    _Mapptr _Growmap( size_type _Newsize )
    {
        _Mapptr _M = static_cast<_Mapptr>(::operator new(_Newsize * sizeof(pointer), exe_heap, std::nothrow));
        std::copy( _First._Map, _Last._Map + 1,_M + _Newsize / 4 );
        _Freeptr( _Map );
        _Map = _M;
        _Mapsize = _Newsize;
        return (_M + _Newsize / 4);
    }

    void _Tidy() NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    {
        while (!empty())
            pop_front();
        if (_Map)
            _Freemap();
    }

protected:
    uint32_t _Dummy;
    iterator _First;
    iterator _Last;
    _Mapptr _Map; // pointer to array of pointers to blocks
    size_type _Mapsize; // size of map array
    size_type _Size; // current length of sequence
};

#pragma pack(pop)

    // exe_deque TEMPLATE OPERATORS
template<class T> inline bool operator==(const exe_deque<T>& lhs,
                                         const exe_deque<T>& rhs)
{
    return (lhs.size() == rhs.size()
                && std::equal( lhs.begin(), lhs.end(), rhs.begin() ));
}
template<class T> inline bool operator!=(const exe_deque<T>& lhs,
                                         const exe_deque<T>& rhs)
{
    return (!(lhs == rhs));
}
template<class T> inline
    bool operator<(const exe_deque<T>& lhs,
                   const exe_deque<T>& rhs)
{
    return (std::lexicographical_compare( lhs.begin(), lhs.end(),
                rhs.begin(), rhs.end() ));
}
template<class T> inline
    bool operator<=( const exe_deque<T>& lhs,
                        const exe_deque<T>& rhs )
{
    return (!(rhs < lhs));
}
template<class T> inline
    bool operator>( const exe_deque<T>& lhs,
                    const exe_deque<T>& rhs )
{
    return (rhs < lhs);
}
template<class T> inline
    bool operator>=( const exe_deque<T>& lhs,
                        const exe_deque<T>& rhs )
{
    return (!(lhs < rhs));
}
//}
