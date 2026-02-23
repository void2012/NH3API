//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
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

#include <algorithm>             // std::copy, std::copy_backward
#include <utility>               // std::swap
#if (__cplusplus >= 202002L) || defined(__cpp_lib_three_way_comparison)
#include <compare>               // std::strong_ordering
#endif
#include <stdexcept> // std::out_of_range

#include "nh3api_exceptions.hpp" // exceptions
#include "iterator.hpp"          // iterator_for_container concept OR tt::is_iterator_v<T> type trait
#include "memory.hpp"            // exe_allocator
#include "stl_extras.hpp"        // std::strong_ordering

#pragma pack(push, 4)
// Visual C++ 6.0 std::deque implementation used by heroes3.exe
// _Ty stored type
template<class _Ty>
class exe_deque
{
public:
    using allocator_type  = exe_allocator<_Ty>;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using value_type      = _Ty;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

protected:
    using _Mapptr = pointer*;
    inline static constexpr int DEQUE_MAP_SIZE = 2;
    inline static constexpr int DEQUE_SIZE     = (4096 < static_cast<ptrdiff_t>(sizeof(value_type)) ? 1 : 4096 / static_cast<ptrdiff_t>(sizeof(value_type)));

    class iterator;
    class const_iterator;

    class iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = _Ty;
        using difference_type   = ptrdiff_t;
        using pointer           = _Ty*;
        using reference         = _Ty&;

        friend class exe_deque<_Ty>;

    protected:
        pointer _Myfirst { nullptr };
        pointer _Mylast  { nullptr };
        pointer _Next    { nullptr };
        _Mapptr _Map     { nullptr };

    public:
        inline iterator() noexcept                           = default;
        inline iterator& operator=(const iterator&) noexcept = default;
        inline ~iterator() noexcept                          = default;

        inline iterator(iterator&& _Right) noexcept
            : _Myfirst { std::exchange(_Right._Myfirst, nullptr) },
              _Mylast  { std::exchange(_Right._Mylast, nullptr) },
              _Next    { std::exchange(_Right._Next, nullptr) },
              _Map     { std::exchange(_Right._Map, nullptr) }
        {}

        inline iterator& operator=(iterator&& _Right) noexcept
        {
            _Myfirst = std::exchange(_Right._Myfirst, nullptr);
            _Mylast  = std::exchange(_Right._Mylast, nullptr);
            _Next    = std::exchange(_Right._Next, nullptr);
            _Map     = std::exchange(_Right._Map, nullptr);
            return *this;
        }

        inline iterator(pointer _P, _Mapptr _M) noexcept
            : _Myfirst { *_M }, _Mylast { *_M + DEQUE_SIZE }, _Next { _P }, _Map { _M }
        {}

        inline iterator(const iterator& other) noexcept = default;

        inline iterator(const nh3api::dummy_tag_t&) noexcept
        {}

        [[nodiscard]] inline operator const_iterator() const noexcept
        { return *this; }

        [[nodiscard]] inline reference operator*() noexcept
        { return *_Next; }

        [[nodiscard]] inline const_reference operator*() const noexcept
        { return *_Next; }

        [[nodiscard]] inline pointer operator->() noexcept
        { return &(*(*this)); }

        [[nodiscard]] inline const_pointer operator->() const noexcept
        { return &(*(*this)); }

        inline iterator& operator++() noexcept
        {
            if ( ++_Next == _Mylast )
            {
                ++_Map;
                _Myfirst = *_Map;
                _Mylast = _Myfirst + DEQUE_SIZE;
                _Next = _Myfirst;
            }
            return *this;
        }

        inline iterator operator++(int) noexcept
        {
            iterator _Temp = *this;
            ++(*this);
            return _Temp;
        }

        inline iterator& operator--() noexcept
        {
            if ( _Next == _Myfirst )
            {
                --_Map;
                _Myfirst = *_Map;
                _Mylast = _Myfirst + DEQUE_SIZE;
                _Next = _Mylast;
            }
            --_Next;
            return *this;
        }

        inline iterator operator--(int) noexcept
        {
            iterator _Temp = *this;
            --(*this);
            return _Temp;
        }

        iterator& operator+=(const ptrdiff_t _Count) noexcept
        {
            const ptrdiff_t _Off  = _Count + _Next - _Myfirst;
            const ptrdiff_t _Moff = (0 <= _Off)
                                      ? _Off / DEQUE_SIZE
                                      : -((DEQUE_SIZE - 1 - _Off) / DEQUE_SIZE);
            if ( _Moff == 0 )
            {
                _Next += _Count;
            }
            else
            {
                _Map     += _Moff;
                _Myfirst  = *_Map;
                _Mylast   = _Myfirst + DEQUE_SIZE;
                _Next     = _Myfirst + (_Off - _Moff * DEQUE_SIZE);
            }
            return *this;
        }

        inline iterator& operator-=(const ptrdiff_t _Count) noexcept
        { return *this += -_Count; }

        inline iterator operator+(const ptrdiff_t _Count) const noexcept
        {
            iterator _Temp = *this;
            return _Temp += _Count;
        }

        inline iterator operator-(const ptrdiff_t _Count) const noexcept
        {
            iterator _Temp = *this;
            return _Temp -= _Count;
        }

        inline ptrdiff_t operator-(const iterator& other) const noexcept
        {
            return _Map == other._Map ? _Next - other._Next
                                      : DEQUE_SIZE * (_Map - other._Map)
                                            + (_Next - _Myfirst) - (other._Next - other._Myfirst);
        }

        [[nodiscard]] inline const_reference operator[](const ptrdiff_t _Count) const noexcept
        { return *(*this + _Count); }

        [[nodiscard]] inline bool operator==(const iterator& other) const noexcept
        { return _Next == other._Next && _Map == other._Map; }

    #ifdef __cpp_lib_three_way_comparison
        [[nodiscard]] inline std::strong_ordering operator<=>(const iterator& other) const noexcept
        {
            if ( _Map < other._Map )
                return std::strong_ordering::less;
            if ( _Map > other._Map )
                return std::strong_ordering::greater;
            if ( _Next < other._Next )
                return std::strong_ordering::less;
            if ( _Next > other._Next )
                return std::strong_ordering::greater;

            return std::strong_ordering::equivalent;
        }
    #else
        inline bool operator!=(const iterator& other) const noexcept
        { return !(*this == other); }

        inline bool operator<(const iterator& other) const noexcept
        { return _Map < other._Map || (_Map == other._Map && _Next < other._Next); }

        inline bool operator<=(const iterator& other) const noexcept
        { return !(other < *this); }

        inline bool operator>(const iterator& other) const noexcept
        { return other < *this; }

        inline bool operator>=(const iterator& other) const noexcept
        { return !(*this < other); }
    #endif

    };

    class const_iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = const _Ty;
        using difference_type   = ptrdiff_t;
        using pointer           = const _Ty*;
        using reference         = const _Ty&;

        friend class exe_deque<_Ty>;

    protected:
        pointer _Myfirst { nullptr };
        pointer _Mylast  { nullptr };
        pointer _Next    { nullptr };
        _Mapptr _Map     { nullptr };

    public:
        inline const_iterator() noexcept                                 = default;
        inline const_iterator& operator=(const const_iterator&) noexcept = default;
        inline ~const_iterator() noexcept                                = default;

        inline const_iterator(const_iterator&& _Right) noexcept
            : _Myfirst { std::exchange(_Right._Myfirst, nullptr) },
              _Mylast  { std::exchange(_Right._Mylast, nullptr) },
              _Next    { std::exchange(_Right._Next, nullptr) },
              _Map     { std::exchange(_Right._Map, nullptr) }
        {}

        inline const_iterator& operator=(const_iterator&& _Right) noexcept
        {
            _Myfirst = std::exchange(_Right._Myfirst, nullptr);
            _Mylast  = std::exchange(_Right._Mylast, nullptr);
            _Next    = std::exchange(_Right._Next, nullptr);
            _Map     = std::exchange(_Right._Map, nullptr);
            return *this;
        }

        inline const_iterator(pointer _P, _Mapptr _M) noexcept
            : _Myfirst { *_M }, _Mylast { *_M + DEQUE_SIZE }, _Next { _P }, _Map { _M }
        {}

        inline const_iterator(const const_iterator& other) noexcept = default;

        inline const_iterator(const nh3api::dummy_tag_t&) noexcept
        {}

        [[nodiscard]] inline const_reference operator*() const noexcept
        { return *_Next; }

        [[nodiscard]] inline const_pointer operator->() const noexcept
        { return &(*(*this)); }

        inline const_iterator& operator++() noexcept
        {
            if ( ++_Next == _Mylast )
            {
                ++_Map;
                _Myfirst = *_Map;
                _Mylast = _Myfirst + DEQUE_SIZE;
                _Next = _Myfirst;
            }
            return *this;
        }

        inline const_iterator operator++(int) noexcept
        {
            const_iterator _Temp = *this;
            ++(*this);
            return _Temp;
        }

        inline const_iterator& operator--() noexcept
        {
            if ( _Next == _Myfirst )
            {
                --_Map;
                _Myfirst = *_Map;
                _Mylast = _Myfirst + DEQUE_SIZE;
                _Next = _Mylast;
            }
            --_Next;
            return *this;
        }

        inline const_iterator operator--(int) noexcept
        {
            const_iterator _Temp = *this;
            --(*this);
            return _Temp;
        }

        const_iterator& operator+=(const ptrdiff_t _Count) noexcept
        {
            const ptrdiff_t _Off  = _Count + _Next - _Myfirst;
            const ptrdiff_t _Moff = (0 <= _Off)
                                      ? _Off / DEQUE_SIZE
                                      : -((DEQUE_SIZE - 1 - _Off) / DEQUE_SIZE);
            if ( _Moff == 0 )
            {
                _Next += _Count;
            }
            else
            {
                _Map     += _Moff;
                _Myfirst  = *_Map;
                _Mylast   = _Myfirst + DEQUE_SIZE;
                _Next     = _Myfirst + (_Off - _Moff * DEQUE_SIZE);
            }
            return *this;
        }

        inline const_iterator& operator-=(const ptrdiff_t _Count) noexcept
        { return *this += -_Count; }

        inline const_iterator operator+(const ptrdiff_t _Count) const noexcept
        {
            const_iterator _Temp = *this;
            return _Temp += _Count;
        }

        inline const_iterator operator-(const ptrdiff_t _Count) const noexcept
        {
            const_iterator _Temp = *this;
            return _Temp -= _Count;
        }

        inline ptrdiff_t operator-(const const_iterator& other) const noexcept
        {
            return _Map == other._Map ? _Next - other._Next
                                      : DEQUE_SIZE * (_Map - other._Map)
                                            + (_Next - _Myfirst) - (other._Next - other._Myfirst);
        }

        [[nodiscard]] inline const_reference operator[](const ptrdiff_t _Count) const noexcept
        { return *(*this + _Count); }

        [[nodiscard]] inline bool operator==(const const_iterator& other) const noexcept
        { return _Next == other._Next && _Map == other._Map; }

    #ifdef __cpp_lib_three_way_comparison
        [[nodiscard]] inline std::strong_ordering operator<=>(const const_iterator& other) const noexcept
        {
            if ( _Map < other._Map )
                return std::strong_ordering::less;
            if ( _Map > other._Map )
                return std::strong_ordering::greater;
            if ( _Next < other._Next )
                return std::strong_ordering::less;
            if ( _Next > other._Next )
                return std::strong_ordering::greater;

            return std::strong_ordering::equivalent;
        }
    #else
        inline bool operator!=(const const_iterator& other) const noexcept
        { return !(*this == other); }

        inline bool operator<(const const_iterator& other) const noexcept
        { return _Map < other._Map || (_Map == other._Map && _Next < other._Next); }

        inline bool operator<=(const const_iterator& other) const noexcept
        { return !(other < *this); }

        inline bool operator>(const const_iterator& other) const noexcept
        { return other < *this; }

        inline bool operator>=(const const_iterator& other) const noexcept
        { return !(*this < other); }
    #endif

    };

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator       = std::reverse_iterator<iterator>;

protected:
#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
    template<nh3api::tt::iterator_for_container _Iter>
#else
    template<class _Iter, ::std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
    // initialize from [_Begin, _End), input iterators
    void _Construct(_Iter _Begin, _Iter _End)
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

    void _Construct_n(const size_t _Count, const value_type& _Value)
    {
        NH3API_TRY
        {
            _Insert_n(begin(), _Count, _Value);
        }
        NH3API_CATCH(...)
        {
            _Tidy();
            NH3API_RETHROW
        }
    }

public:
    exe_deque() noexcept
        : _Myfirst {},
          _Mylast {},
          _Map { nullptr },
          _Mapsize { 0 },
          _Mysize { 0 }
    {}

    exe_deque(const exe_deque& other)
    { _Construct(other.begin(), other.end()); }

    explicit exe_deque(const size_t _Count, const value_type& _Value)
        : _Myfirst {},
          _Mylast {},
          _Map { nullptr },
          _Mapsize { 0 },
          _Mysize { 0 }
    { _Construct_n(_Count, _Value); }

    exe_deque(exe_deque&& _Other) noexcept
        : _Myfirst { std::move(_Other._Myfirst) },
          _Mylast  { std::move(_Other._Mylast) },
          _Map     { std::exchange(_Other._Map, nullptr) },
          _Mapsize { std::exchange(_Other._Mapsize, 0) },
          _Mysize  { std::exchange(_Other._Mysize, 0) }
    {}

    exe_deque& operator=(exe_deque&& _Other) noexcept
    {
        if ( this != &_Other )
        {
            _Tidy();
            this->_Myfirst = std::move(_Other._Myfirst);
            this->_Mylast  = std::move(_Other._Mylast);
            this->_Map     = std::exchange(_Other._Map, nullptr);
            this->_Mapsize = std::exchange(_Other._Mapsize, 0);
            this->_Mysize  = std::exchange(_Other._Mysize, 0);
        }
        return *this;
    }

#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
    template<nh3api::tt::iterator_for_container _Iter>
#else
    template<class _Iter, ::std ::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
    exe_deque(_Iter _First, _Iter _Last)
        : _Myfirst {},
          _Mylast {},
          _Map { nullptr },
          _Mapsize { 0 },
          _Mysize { 0 }
    {
        _Construct(_First, _Last);
    }

    exe_deque(const nh3api::dummy_tag_t& tag) noexcept
        : _Myfirst { tag }, _Mylast { tag }
    {}

    ~exe_deque() noexcept
    { _Tidy(); }

    exe_deque& operator=(const exe_deque& _Right)
    {
        if ( this != &_Right )
        {
            if ( _Right.size() <= size() )
            {
                iterator _S = std::copy(_Right.begin(), _Right.end(), begin());
                erase(_S, end());
            }
            else
            {
                const_iterator _Sx = _Right.cbegin() + size();
                iterator       _S  = std::copy(_Right.begin(), _Sx, begin());
                std::copy(_Sx, _Right.end(), std::inserter(*this, _S));
            }
        }
        return *this;
    }

    [[nodiscard]] inline iterator begin() noexcept
    { return _Myfirst; }

    [[nodiscard]] inline const_iterator begin() const noexcept
    { return const_iterator { _Myfirst }; }

    [[nodiscard]] inline iterator end() noexcept
    { return _Mylast; }

    [[nodiscard]] inline const_iterator cbegin() const noexcept
    { return const_iterator { _Myfirst }; }

    [[nodiscard]] inline const_iterator cend() const noexcept
    { return const_iterator { _Mylast }; }

    [[nodiscard]] inline reverse_iterator rbegin() noexcept
    { return reverse_iterator { end() }; }

    [[nodiscard]] inline const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator { end() }; }

    [[nodiscard]] inline reverse_iterator rend() noexcept
    { return reverse_iterator { begin() }; }

    [[nodiscard]] inline const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator { begin() }; }

    [[nodiscard]] inline const_reverse_iterator crbegin() const noexcept
    { return const_reverse_iterator { end() }; }

    [[nodiscard]] inline const_reverse_iterator crend() const noexcept
    { return const_reverse_iterator { begin() }; }

    void resize(const size_t _Count, const value_type _Value = value_type())
    {
        if ( size() < _Count )
            _Insert_n(end(), _Count - size(), _Value);
        else if ( _Count < size() )
            erase(begin() + _Count, end());
    }

    [[nodiscard]] inline size_t size() const noexcept
    { return _Mysize; }

    [[nodiscard]] inline constexpr static size_t max_size() noexcept
    { return size_t(~0U) / sizeof(value_type); }

    [[nodiscard]] inline bool empty() const noexcept
    { return size() == 0; }

    [[nodiscard]] inline allocator_type get_allocator() const noexcept
    { return {}; }

    [[nodiscard]] inline const_reference at(const size_t _Pos) const
    {
        if ( size() <= _Pos )
            _Throw_invalid_subscript();
        return *(begin() + _Pos);
    }

    [[nodiscard]] inline reference at(const size_t _Pos)
    {
        if ( size() <= _Pos )
            _Throw_invalid_subscript();
        return *(begin() + _Pos);
    }

    [[nodiscard]] inline const_reference operator[](const size_t _Pos) const noexcept
    { return *(begin() + _Pos); }

    inline reference operator[](const size_t _Pos) noexcept
    { return *(begin() + _Pos); }

    [[nodiscard]] inline reference front() noexcept
    { return *begin(); }

    [[nodiscard]] inline const_reference front() const noexcept
    { return *begin(); }

    [[nodiscard]] inline reference back() noexcept
    { return *(end() - 1); }

    [[nodiscard]] inline const_reference back() const noexcept
    { return *(end() - 1); }

    void push_front(const value_type& _Value)
    {
        if ( empty() || _Myfirst._Next == _Myfirst._Myfirst )
            _Buyfront();

        ::new (static_cast<void*>(--_Myfirst._Next)) value_type(_Value);

        ++_Mysize;
    }

    void pop_front() noexcept
    {
        std::destroy_at(_Myfirst._Next++);
        --_Mysize;
        if ( empty() || _Myfirst._Next == _Myfirst._Mylast )
            _Freefront();
    }

    void push_back(const value_type& _Value)
    {
        if ( empty() || (_Mylast._Next == _Mylast._Mylast) )
        {
            _Buyback();
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(_Value);
        }
        else if ( _Mylast._Next + 1 == _Mylast._Mylast )
        {
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(_Value);
            _Buyback();
        }
        else
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(_Value);

        ++_Mysize;
    }

    template<class... Args>
    reference emplace_back(Args&&... args)
    {
        if ( empty() || (_Mylast._Next == _Mylast._Mylast) )
        {
            _Buyback();
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(std::forward<Args>(args)...);
        }
        else if ( _Mylast._Next + 1 == _Mylast._Mylast )
        {
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(std::forward<Args>(args)...);
            _Buyback();
        }
        else
            ::new (static_cast<void*>(_Mylast._Next++)) value_type(std::forward<Args>(args)...);

        ++_Mysize;
        return back();
    }

    void push_back(value_type&& _Value)
    { emplace_back(std::move(_Value)); }

    template<class... Args>
    reference emplace_front(Args&&... args)
    {
        if ( empty() || _Myfirst._Next == _Myfirst._Myfirst )
            _Buyfront();

        ::new (static_cast<void*>(--_Myfirst._Next)) value_type(std::forward<Args>(args)...);
        ++_Mysize;
        return front();
    }

    void push_front(value_type&& _Value)
    { emplace_front(std::move(_Value)); }

    template<class... Args>
    iterator emplace(const_iterator _Where, Args&&... args)
    {
        if ( _Where == cbegin() )
        {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        }
        if ( _Where == cend() )
        {
            emplace_back(std::forward<Args>(args)...);
            return end() - 1;
        }

        iterator     _S;
        const size_t _Off = static_cast<size_t>(_Where - cbegin());
        if ( _Off < size() / 2 )
        {
            push_front(front());
            _S = begin() + _Off;
            std::copy(begin() + 2, _S + 1, begin() + 1);
        }
        else
        {
            push_back(back());
            _S = begin() + _Off;
            std::copy_backward(_S, end() - 2, end() - 1);
        }

        ::new (static_cast<void*>(_S._Next)) value_type(std::forward<Args>(args)...);

        return _S;
    }

    void shrink_to_fit()
    {	// reduce capacity
        exe_deque _Temp(*this);
        swap(_Temp);
    }

    void pop_back() noexcept
    {
        if ( _Mylast._Next == _Mylast._Myfirst )
            _Freeback();

        if ( !empty() )
            std::destroy_at(--_Mylast._Next );

        --_Mysize;
        if ( empty() )
            _Freeback();
    }

#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
    template<nh3api::tt::iterator_for_container _Iter>
#else
    template<class _Iter, ::std ::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
    void assign(_Iter _First, _Iter _Last)
    {
        nh3api::verify_range(_First, _Last);
        erase(begin(), end());
        insert(begin(), _First, _Last);
    }

    void assign(const size_t _Count, const value_type& _Value)
    {
        _Assign_n(_Count, _Value);
    }

    iterator insert(const_iterator _Where, const value_type& _Value)
    {
        if ( _Where == cbegin() )
        {
            push_front(_Value);
            return begin();
        }
        if ( _Where == cend() )
        {
            push_back(_Value);
            return end() - 1;
        }

        iterator     _S;
        const size_t _Off = static_cast<size_t>(_Where - cbegin());
        if ( _Off < size() / 2 )
        {
            push_front(front());
            _S = begin() + _Off;
            std::copy(begin() + 2, _S + 1, begin() + 1);
        }
        else
        {
            push_back(back());
            _S = begin() + _Off;
            std::copy_backward(_S, end() - 2, end() - 1);
        }
        *_S = _Value;
        return _S;
    }

    void insert(iterator _Where, const size_t _Count, const value_type& _Value)
    { _Insert_n(_Where, _Count, _Value); }

#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
    template<nh3api::tt::iterator_for_container _Iter>
#else
    template<class _Iter, ::std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
    void insert(const_iterator _Where, _Iter _First, _Iter _Last)
    {
        const size_t _M   = static_cast<size_t>(std::distance(_First, _Last));
        const size_t _Off = static_cast<size_t>(_Where - begin());
        const size_t _Rem = _Mysize - _Off;
        if ( _Off < _Rem )
        {
            if ( _Off < _M )
            {
                _Iter _Qx = _First;
                std::advance(_Qx, _M - _Off);

                for ( _Iter _Q = _Qx; _First != _Q; )
                    push_front(*--_Q);

                for ( size_t i = _Off; 0 < i; --i )
                    push_front(begin()[_M - 1]);

                std::copy(_Qx, _Last, begin() + _M);
            }
            else
            {
                for ( size_t i = _M; 0 < i; --i )
                    push_front(begin()[_M - 1]);

                iterator _S = begin() + _M;
                std::copy(_S + _M, _S + _Off, _S);
                std::copy(_First, _Last, begin() + _Off);
            }
        }
        else if ( _Rem < _M )
        {
            _Iter _Qx = _First;
            std::advance(_Qx, _Rem);

            for ( _Iter _Q = _Qx; _Q != _Last; ++_Q )
                push_back(*_Q);

            for ( size_t i = 0; i < _Rem; ++i )
                push_back(begin()[_Off + i]);

            std::copy(_First, _Qx, begin() + _Off);
        }
        else
        {
            for ( size_t i = 0; i < _M; ++i )
                push_back(begin()[_Off + _Rem - _M + i]);

            iterator _S = begin() + _Off;
            std::copy_backward(_S, _S + _Rem - _M, _S + _Rem);
            std::copy(_First, _Last, _S);
        }
    }

    iterator erase(iterator _Where) noexcept
    { return erase(_Where, _Where + 1); }

    iterator erase(iterator _First, iterator _Last) noexcept
    {
        size_t       _Count = static_cast<size_t>(_Last - _First);
        const size_t _M = static_cast<size_t>(_First - begin());
        if ( _M < static_cast<size_t>(end() - _Last) )
        {
            std::copy_backward(begin(), _First, _Last);
            for ( ; 0 < _Count; --_Count )
                pop_front();
        }
        else
        {
            std::copy(_Last, end(), _First);
            for ( ; 0 < _Count; --_Count )
                pop_back();
        }
        return _M == 0 ? begin() : begin() + _M;
    }

    void clear() noexcept
    { erase(begin(), end()); }

    void swap(exe_deque& other) noexcept
    {
        if ( this != &other )
        {
            std::swap(this->_Myfirst, other._Myfirst);
            std::swap(this->_Mylast,  other._Mylast);
            std::swap(this->_Map,     other._Map);
            std::swap(this->_Mapsize, other._Mapsize);
            std::swap(this->_Mysize,  other._Mysize);
        }
    }

protected:
    template<class _Iter>
    static inline pointer _Move(_Iter _Begin, _Iter _End, pointer _Ptr)
    {
        if constexpr ( std::is_move_assignable_v<value_type> )
            return std::move(_Begin, _End, _Ptr);
        else if constexpr ( std::is_copy_assignable_v<value_type> )
            return std::copy(_Begin, _End, _Ptr);
    }

    template<class _Iter>
    static inline pointer _Move_backward(_Iter _Begin, _Iter _End, pointer _Ptr)
    {
        if constexpr ( std::is_move_assignable_v<value_type> )
            return std::move_backward(_Begin, _End, _Ptr);
        else if constexpr ( std::is_copy_assignable_v<value_type> )
            return std::copy_backward(_Begin, _End, _Ptr);
    }

    void _Assign_n(const size_t _Count, const value_type& _Val)
    {	// assign _Count * _Val
        value_type _Temp = _Val;	// in case _Val is in sequence
        erase(begin(), end());
        _Insert_n(begin(), _Count, _Temp);
    }

    void _Insert_n(const_iterator _Where, const size_t _Count, const value_type& _Value)
    { // insert _Count * _Val at _Where
        iterator     _Mid;
        size_t       _Num     = 0;
        const size_t _Off     = static_cast<size_t>(_Where - begin());
        const size_t _Rem     = this->_Mysize - _Off;
        const size_t _Oldsize = this->_Mysize;

        if ( _Off < _Rem )
        { // closer to front
            NH3API_TRY
            {
                if ( _Off < _Count )
                {                                                     // insert longer than prefix
                    for ( _Num = _Count - _Off; 0 < _Num; --_Num )
                        push_front(_Value);                             // push excess values

                    for ( _Num = _Off; 0 < _Num; --_Num )
                        push_front(front());                          // push prefix

                    _Mid = begin() + _Count;
                    std::fill(_Mid, _Mid + _Off, _Value);               // fill in rest of values
                }
                else
                {                                                     // insert not longer than prefix
                    for ( _Num = _Count; 0 < _Num; --_Num )
                        push_front(front());                          // push part of prefix

                    _Mid             = begin() + _Count;
                    value_type _Temp = _Value;                          // in case _Val is in sequence
                    _Move(begin() + _Count, begin() + _Off, begin()); // copy rest of prefix
                    std::fill(begin() + _Off, _Mid + _Off, _Temp);    // fill in values
                }
            }
            NH3API_CATCH(...)
            {
                for ( ; _Oldsize < this->_Mysize; )
                    pop_front(); // restore old size, at least
                NH3API_RETHROW
            }
        }
        else
        { // closer to back
            NH3API_TRY
            {
                if ( _Rem < _Count )
                {                                                        // insert longer than suffix
                    for ( _Num = _Count - _Rem; 0 < _Num; --_Num )
                        push_back(_Value);                                 // push excess values

                    for ( _Num = 0; _Num < _Rem; ++_Num )
                        push_back(begin()[_Off + _Num]);                 // push suffix

                    _Mid = begin() + _Off;
                    std::fill(_Mid, _Mid + _Rem, _Value);                  // fill in rest of values
                }
                else
                {                                                        // insert not longer than prefix
                    for ( _Num = 0; _Num < _Count; ++_Num )
                        push_back(begin()[_Off + _Rem - _Count + _Num]); // push part of prefix

                    _Mid             = begin() + _Off;
                    value_type _Temp = _Value;                             // in case _Val is in sequence
                    _Move_backward(begin() + _Off, begin() + _Rem - _Count,
                                   begin() + _Rem);                      // copy rest of prefix
                    std::fill(_Mid, _Mid + _Count, _Temp);               // fill in values
                }
            }
            NH3API_CATCH(...)
            {
                for ( ; _Oldsize < this->_Mysize; )
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
            _Mapsize = DEQUE_MAP_SIZE;
            const size_t _Count = _Mapsize / 2;
            _Allocate_map();
            *(_Map + _Count) = _P;
            _Myfirst     = iterator { _P + DEQUE_SIZE / 2, _Map + _Count };
            _Mylast      = _Myfirst;
        }
        else if ( _Mylast._Map < _Map + (_Mapsize - 1) )
        {
            *(++_Mylast._Map) = _P;
            _Mylast           = iterator { _P, _Mylast._Map };
        }
        else
        {
            const ptrdiff_t _Diff = _Mylast._Map - _Myfirst._Map + 1;
            _Mapptr         _M    = _Growmap(2 * _Diff);
            *(_M + _Diff)         = _P;
            _Myfirst              = iterator { _Myfirst._Next, _M };
            _Mylast               = iterator { _P, _M + _Diff };
        }
    }

    void _Buyfront()
    {
        pointer _P = static_cast<pointer>(::operator new(DEQUE_SIZE * sizeof(value_type), exe_heap, std::nothrow));
        if ( empty() )
        {
            _Mapsize = DEQUE_MAP_SIZE;
            const size_t _Count = _Mapsize / 2;
            _Allocate_map();
            *(_Map + _Count) = _P;
            _Myfirst     = iterator { _P + (DEQUE_SIZE / 2 + 1), _Map + _Count };
            _Mylast      = _Myfirst;
        }
        else if ( _Map < _Myfirst._Map )
        {
            *(--_Myfirst._Map) = _P;
            _Myfirst           = iterator { _P + DEQUE_SIZE, _Myfirst._Map };
        }
        else if ( _Mylast._Map == _Myfirst._Map )
        {
            *(_Mylast._Map++)    = *_Myfirst._Map;
            *(_Myfirst._Map + 1) = *_Myfirst._Map;
            *(_Myfirst._Map)     = _P;
            _Myfirst             = iterator { _P + DEQUE_SIZE, _Myfirst._Map };
        }
        else
        {
            const ptrdiff_t _Diff = _Mylast._Map - _Myfirst._Map + 1;
            _Mapptr         _M    = _Growmap(2 * _Diff);
            *(--_M)               = _P;
            _Myfirst              = iterator { _P + DEQUE_SIZE, _M };
            _Mylast               = iterator { _Mylast._Next, _M + _Diff };
        }
    }

    void _Freeback() noexcept
    {
        std::destroy_at(--_Mylast._Next);
        if (_Mylast._Next == _Mylast._Myfirst)
        {
            _Freeptr( _Mylast._Map );
            if ( _Myfirst._Map == _Mylast._Map )
            {
                _Freemap();
            }
            else
            {
                --_Mylast._Map;
            }
        }
    }

    void _Freefront() noexcept
    {
        std::destroy_at(_Myfirst._Next);
        _Myfirst._Next++;
        if ( _Myfirst._Next == _Myfirst._Mylast)
        {
            _Freeptr( _Myfirst._Map );
            if ( _Myfirst._Map == _Mylast._Map )
            {
                _Freemap();
            }
            else
            {
                ++_Myfirst._Map;
            }
        }
    }

    [[noreturn]] inline static void _Throw_invalid_subscript() noexcept(false)
    { nh3api::throw_exception<std::out_of_range>("invalid deque position"); }

    void _Freemap() noexcept
    {
        _Freeptr(_Map);
        _Map     = nullptr;
        _Mapsize = 0;
    }

    static void _Freeptr(_Mapptr _M) noexcept
    { ::operator delete(_M, exe_heap); }

    void _Allocate_map() noexcept
    {
        _Map = static_cast<_Mapptr>(::operator new(_Mapsize * sizeof(pointer), exe_heap, std::nothrow));
        if ( _Map == nullptr )
            nh3api::throw_exception<std::bad_alloc>();

        std::memset(_Map, 0, _Mapsize * sizeof(pointer));
    }

    _Mapptr _Growmap(size_t _Newsize)
    {
        _Mapptr _M = static_cast<_Mapptr>(::operator new(_Newsize * sizeof(pointer), exe_heap, std::nothrow));
        if ( _M == nullptr )
            nh3api::throw_exception<std::bad_alloc>();

        std::copy(_Myfirst._Map, _Mylast._Map + 1, _M + _Newsize / 4);
        _Freeptr(_Map);
        _Map     = _M;
        _Mapsize = _Newsize;
        return _M + _Newsize / 4;
    }

    void _Tidy() noexcept
    {
        while ( !empty() )
            pop_front();
        if ( _Map )
            _Freemap();
    }

protected:
    uint32_t : 32;
    iterator  _Myfirst;
    iterator  _Mylast;
    _Mapptr   _Map;     // pointer to array of pointers to blocks
    size_t _Mapsize; // size of map array
    size_t _Mysize;  // current length of sequence
} NH3API_MSVC_LAYOUT;

#pragma pack(pop)

template<class _Ty>
inline bool operator==(const exe_deque<_Ty>& _Left,
                       const exe_deque<_Ty>& _Right) noexcept
{
    return _Left.size() == _Right.size()
        && std::equal(_Left.begin(), _Left.end(), _Right.begin());
}

#ifdef __cpp_lib_three_way_comparison
template<class _Ty>
inline nh3api::synth_three_way_result<_Ty> operator<=>(const exe_deque<_Ty>& _Left,
                                                       const exe_deque<_Ty>& _Right) noexcept
{
    return std::lexicographical_compare_three_way(_Left.cbegin(), _Left.cend(),
                                                  _Right.cbegin(), _Right.cend(), nh3api::synth_three_way);
}
#else
template<class _Ty>
inline bool operator!=(const exe_deque<_Ty>& _Left,
                       const exe_deque<_Ty>& _Right) noexcept
{ return !(_Left == _Right); }

template<class _Ty>
inline bool operator<(const exe_deque<_Ty>& _Left,
                      const exe_deque<_Ty>& _Right) noexcept
{
    return std::lexicographical_compare(_Left.cbegin(), _Left.cend(),
                                        _Right.cbegin(), _Right.cend());
}

template<class _Ty>
inline bool operator<=(const exe_deque<_Ty>& _Left,
                       const exe_deque<_Ty>& _Right) noexcept
{ return !(_Right < _Left); }

template<class _Ty>
inline bool operator>(const exe_deque<_Ty>& _Left,
                      const exe_deque<_Ty>& _Right) noexcept
{ return _Right < _Left; }

template<class _Ty>
inline bool operator>=(const exe_deque<_Ty>& _Left,
                       const exe_deque<_Ty>& _Right) noexcept
{ return !(_Left < _Right); }
#endif
