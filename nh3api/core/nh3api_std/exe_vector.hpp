//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// implementation of vector partially from MSVC STL
// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <array>                 // std::array
#include <cassert>               // assert
#include <initializer_list>      // std::initializer_list
#include <type_traits>

#include "iterator.hpp"          // nh3api::is_iterator
#include "memory.hpp"            // exe_allocator<_Ty>, exe_heap_t
#include "nh3api_exceptions.hpp" // std::length_error, std::out_of_range, std::runtime_error
#include "stl_extras.hpp"        // nh3api::synth_three_way_result

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(4714)
NH3API_WARNING_MSVC_DISABLE(26495)

#pragma pack(push, 4)

// Visual C++ 6.0 std::vector implementation used by heroes3.exe
template<class _Ty>
class exe_vector
{
    protected:
        static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                             "because of [container.requirements].");

        // external typedefs
    public:
        using value_type      = _Ty;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using allocator_type  = exe_allocator<value_type>;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using reference       = value_type&;
        using const_reference = const value_type&;

        using iterator               = pointer;
        using const_iterator         = const_pointer;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator       = std::reverse_iterator<iterator>;

    protected:
        struct [[nodiscard]] _Reallocation_guard
        {
            pointer _New_begin;
            size_t  _New_capacity;
            pointer _Constructed_first;
            pointer _Constructed_last;

            _Reallocation_guard& operator=(const _Reallocation_guard&) = delete;
            _Reallocation_guard& operator=(_Reallocation_guard&&)      = delete;

            NH3API_FORCEINLINE ~_Reallocation_guard() noexcept
            {
                if ( _New_begin != nullptr )
                {
                    std::destroy(_Constructed_first, _Constructed_last);
                    _Deallocate(_New_begin);
                }
            }
        };

        struct [[nodiscard]] _Simple_reallocation_guard
        {
            pointer _New_begin;

            _Simple_reallocation_guard& operator=(const _Simple_reallocation_guard&) = delete;
            _Simple_reallocation_guard& operator=(_Simple_reallocation_guard&&)      = delete;

            NH3API_FORCEINLINE ~_Simple_reallocation_guard() noexcept
            {
                if ( _New_begin != nullptr )
                    _Deallocate(_New_begin);
            }
        };

        // vaporize the detached piece
        struct [[nodiscard]] _Vaporization_guard
        {
            exe_vector* _Target;
            pointer     _Vaporized_first;
            pointer     _Vaporized_last;
            pointer     _Destroyed_first;

            _Vaporization_guard& operator=(const _Vaporization_guard&) = delete;
            _Vaporization_guard& operator=(_Vaporization_guard&&)      = delete;

            NH3API_FORCEINLINE ~_Vaporization_guard() noexcept
            {
                if ( _Target != nullptr )
                {
                    std::destroy(_Destroyed_first, _Target->_Mylast);
                    _Target->_Mylast = _Vaporized_first;
                }
            }
        };

        // class with destructor that calls _Tidy
        struct [[nodiscard]] _Tidy_guard
        {
            exe_vector* _Target;

            NH3API_FORCEINLINE ~_Tidy_guard() noexcept
            {
                if ( _Target )
                    _Target->_Tidy();
            }
        };

    public:
        exe_vector() noexcept
            : _Myfirst { nullptr },
              _Mylast { nullptr },
              _Myend { nullptr }
        {}

        explicit exe_vector(const size_t _Count)
        { _Construct_n(_Count); }

        exe_vector(const size_t _Count, const value_type& _Value)
        { _Construct_n(_Count, _Value); }

#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
#else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
        exe_vector(_Iter _First, _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            auto _UFirst = nh3api::unfancy(_First);
            auto _ULast  = nh3api::unfancy(_Last);

            if constexpr ( nh3api::is_cpp17_fwd_iter_v<_Iter> )
            {
                const size_t _Count = static_cast<size_t>(std::distance(_UFirst, _ULast));
                _Construct_n(_Count, std::move(_UFirst), std::move(_ULast));
#ifdef __cpp_lib_ranges
            }
            else if constexpr ( std::forward_iterator<_Iter> )
            {
                const size_t _Count = static_cast<size_t>(std::ranges::distance(_UFirst, _ULast));
                _Construct_n(_Count, std::move(_UFirst), std::move(_ULast));
#endif // __cpp_lib_ranges
            }
            else
            {
                _Tidy_guard _Guard { this };

                _Append_uncounted_range(std::move(_UFirst), std::move(_ULast));

                _Guard._Target = nullptr;
            }
        }

        exe_vector(std::initializer_list<value_type> _Initializer_list)
        {
            _Construct_n(_Initializer_list.size(), _Initializer_list.begin(), _Initializer_list.end());
        }

#ifdef __cpp_lib_containers_ranges
        template<nh3api::tt::container_compatible_range<value_type> _Rng>
        exe_vector(std::from_range_t, _Rng&& _Range)
        {
            if constexpr ( std::ranges::sized_range<_Rng> || std::ranges::forward_range<_Rng> )
            {
                _Construct_n(static_cast<size_t>(std::ranges::distance(_Range)), nh3api::unfancy_begin(_Range), nh3api::unfancy_end(_Range));
            }
            else
            {
                _Tidy_guard _Guard { this };

                _Append_uncounted_range(nh3api::unfancy_begin(_Range), nh3api::unfancy_end(_Range));

                _Guard._Target = nullptr;
            }
        }
#endif

        exe_vector(const exe_vector& _Right)
        { _Construct_n(static_cast<size_t>(_Right._Mylast - _Right._Myfirst), _Right._Myfirst, _Right._Mylast); }

        // Move constructor
        exe_vector(exe_vector&& _Right) noexcept
        {
            auto& _Dst = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
            auto& _Src = reinterpret_cast<std::array<uint32_t, 4>&>(_Right);
            _Dst       = std::exchange(_Src, {});
        }

        exe_vector& operator=(exe_vector&& _Right) noexcept
        {
            if ( this != &_Right )
            {
                _Tidy();
                auto& _Dst = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
                auto& _Src = reinterpret_cast<std::array<uint32_t, 4>&>(_Right);
                _Dst       = std::exchange(_Src, {});
            }
            return *this;
        }

        ~exe_vector() noexcept
        { _Tidy(); }

        inline exe_vector(const nh3api::dummy_tag_t&) noexcept
        {}

    protected:
        // insert by perfectly forwarding into element at end, provide strong guarantee
        template<class... _Args>
        value_type& _Emplace_one_at_back(_Args&&... _Values)
        {
            if ( _Mylast != _Myend )
                return _Emplace_back_with_unused_capacity(std::forward<_Args>(_Values)...);
            return *_Emplace_reallocate(_Mylast, std::forward<_Args>(_Values)...);
        }

        // insert by perfectly forwarding into element at end, provide strong guarantee
        template<class... _Args>
        value_type& _Emplace_back_with_unused_capacity(_Args&&... _Values)
        noexcept(noexcept(std::is_nothrow_constructible_v<value_type, _Args...>))
        {
            ::new (static_cast<void*>(_Mylast)) value_type(std::forward<_Args>(_Values)...);
            value_type& _Result = *_Mylast;
            ++_Mylast;
            return _Result;
        }

        template<class... _Args>
        pointer _Emplace_reallocate(pointer _Whereptr, _Args&&... _Values)
        { // reallocate and insert by perfectly forwarding _Val at _Whereptr
            // pre: !_Has_unused_capacity()
            const size_t _Whereoff = static_cast<size_t>(_Whereptr - this->_Myfirst);
            const size_t _Oldsize  = static_cast<size_t>(_Mylast - _Myfirst);

            if ( _Oldsize == max_size() ) NH3API_UNLIKELY
                nh3api::throw_exception<std::length_error>("vector::emplace: size exceeded maximum size");

            const size_t _Newsize     = _Oldsize + 1;
            const size_t _Newcapacity = _Calculate_growth(_Newsize);

            pointer             _Newvec           = _Allocate(_Newcapacity);
            pointer             _Constructed_last = _Newvec + _Whereoff + 1;
            _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Constructed_last, _Constructed_last };
            pointer&            _Constructed_first = _Guard._Constructed_first;

            ::new (static_cast<void*>(_Newvec + _Whereoff)) value_type(std::forward<_Args>(_Values)...);
            _Constructed_first = _Newvec + _Whereoff;

            if ( _Whereptr == _Mylast )
            { // at back, provide strong guarantee
                if constexpr ( std::is_nothrow_move_constructible_v<_Ty> || !std::is_copy_constructible_v<_Ty> )
                    std::uninitialized_move(_Myfirst, _Mylast, _Newvec);
                else
                    std::uninitialized_copy(_Myfirst, _Mylast, _Newvec);
            }
            else
            { // provide basic guarantee
                std::uninitialized_move(_Myfirst, _Whereptr, _Newvec);
                _Constructed_first = _Newvec;
                std::uninitialized_move(_Whereptr, _Mylast, _Newvec + _Whereoff + 1);
            }

            _Guard._New_begin = nullptr;
            _Change_array(_Newvec, _Newsize, _Newcapacity);
            return _Newvec + _Whereoff;
        }

    public:
        template<class... _Args>
        value_type& emplace_back(_Args&&... _Values)
        {
            return _Emplace_one_at_back(std::forward<_Args>(_Values)...);
        }

        void push_back(const value_type& _Value)
        {
            _Emplace_one_at_back(_Value);
        }

        void push_back(value_type&& _Value)
        {
            _Emplace_one_at_back(std::move(_Value));
        }

    private:
        template<class _Iter, class _Sent>
        void _Append_uncounted_range(_Iter _First, const _Sent _Last)
        {
            for ( ; _First != _Last; ++_First )
                _Emplace_one_at_back(*_First);
        }

#ifdef __cpp_lib_containers_ranges
        template<class _Iter>
        void _Append_counted_range(_Iter _First, const size_t _Count)
        {
            pointer      _Oldfirst        = _Myfirst;
            pointer      _Oldlast         = _Mylast;
            const size_t _Unused_capacity = static_cast<size_t>(_Myend - _Oldlast);
            if ( _Count == 0 )
            { // nothing to do, avoid invalidating iterators
            }
            else if ( _Count > _Unused_capacity )
            { // reallocate
                const size_t _Oldsize = static_cast<size_t>(_Oldlast - _Oldfirst);

                if ( _Count > max_size() - _Oldsize ) NH3API_UNLIKELY
                    nh3api::throw_exception<std::length_error>("vector::append_range: range too long");

                const size_t _Newsize          = _Oldsize + _Count;
                size_t       _Newcapacity      = _Calculate_growth(_Newsize);
                pointer      _Newvec           = _Allocate(_Newcapacity);
                pointer      _Constructed_last = _Newvec + _Oldsize + _Count;

                _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Constructed_last, _Constructed_last };
                pointer&            _Constructed_first = _Guard._Constructed_first;

                std::uninitialized_copy_n(std::move(_First), _Count, _Newvec + _Oldsize);
                _Constructed_first = _Newvec + _Oldsize;

                if ( _Count == 1 )
                { // one at back, provide strong guarantee
                    if constexpr ( std::is_nothrow_move_constructible_v<_Ty> || !std::is_copy_constructible_v<_Ty> )
                        std::uninitialized_move(_Oldfirst, _Oldlast, _Newvec);
                    else
                        std::uninitialized_copy(_Oldfirst, _Oldlast, _Newvec);
                }
                else
                { // provide basic guarantee
                    std::uninitialized_move(_Oldfirst, _Oldlast, _Newvec);
                }

                _Guard._New_begin = nullptr;
                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else
            { // Provide the strong guarantee.
                // Performance note: except for one-at-back, the strong guarantee is unnecessary here.
                _Mylast = std::uninitialized_copy_n(std::move(_First), _Count, _Oldlast);
            }
        }

public:
    #if defined(__clang__) && defined(__INTELLISENSE__)
        template<typename _Range>
    #else
        template<nh3api::tt::container_compatible_range<value_type> _Rng>
    #endif
        void append_range(_Rng&& _Range)
        {
            if constexpr ( std::ranges::forward_range<_Rng> || std::ranges::sized_range<_Rng> )
                _Append_counted_range(nh3api::unfancy_begin(_Range), static_cast<size_t>(std::ranges::distance(_Range)));
            else
                _Append_uncounted_range(nh3api::unfancy_begin(_Range), nh3api::unfancy_end(_Range));
        }

#endif // __cpp_lib_containers_ranges (C++23)
public:
        template<class... _Args>
        iterator emplace(const_iterator _Where, _Args&&... _Values)
        {
            pointer _Whereptr = const_cast<pointer>(_Where);
            pointer _Oldlast  = this->_Mylast;
#if NH3API_DEBUG
            if (_Whereptr < this->_Myfirst || _Whereptr > this->_Mylast) NH3API_UNLIKELY
                nh3api::throw_exception<std::out_of_range>("vector::emplace iterator outside range");
#endif
            if ( _Oldlast != _Myend )
            {
                if ( _Whereptr == _Oldlast )
                { // at back, provide strong guarantee
                    _Emplace_back_with_unused_capacity(std::forward<_Args>(_Values)...);
                }
                else
                {
                    value_type _Obj(std::forward<_Args>(_Values)...); // handle aliasing
                    // after constructing _Obj, provide basic guarantee
                    ::new (static_cast<void*>(_Oldlast)) value_type { std::move(_Oldlast[-1]) };
                    ++this->_Mylast;
                    std::move_backward(_Whereptr, _Oldlast - 1, _Oldlast);
                    *_Whereptr = std::move(_Obj);
                }

                return _Whereptr;
            }

            return _Emplace_reallocate(_Whereptr, std::forward<_Args>(_Values)...);
        }

        // insert _Value at _Pos
        iterator insert(const_iterator _Pos, const value_type& _Value)
        {
            return emplace(_Pos, _Value);
        }

        // insert _Value at _Pos
        iterator insert(const_iterator _Pos, value_type&& _Value)
        {
            return emplace(_Pos, std::move(_Value));
        }

        iterator insert(const_iterator _Where, const size_t _Count, const value_type& _Value)
        {
            pointer      _Oldfirst        = this->_Myfirst;
            pointer      _Oldlast         = this->_Mylast;
            pointer      _Whereptr        = const_cast<pointer>(_Where);
            const size_t _Unused_capacity = static_cast<size_t>(this->_Myend - _Oldlast);
            const size_t _Whereoff        = static_cast<size_t>(_Whereptr - _Oldfirst);
            const bool   _One_at_back     = _Count == 1 && _Whereptr == _Oldlast;

            if ( _Count == 0 )
            { // nothing to do, avoid invalidating iterators
            }
            else if ( _Count > _Unused_capacity )
            { // reallocate
                const size_t _Oldsize = static_cast<size_t>(_Oldlast - _Oldfirst);

                if ( _Count > max_size() - _Oldsize ) NH3API_UNLIKELY
                    nh3api::throw_exception<std::length_error>("vector::insert: size exceeded max_size");

                const size_t _Newsize     = _Oldsize + _Count;
                const size_t _Newcapacity = _Calculate_growth(_Newsize);

                pointer             _Newvec           = _Allocate(_Newcapacity);
                pointer             _Constructed_last = _Newvec + _Whereoff + _Count;
                _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Constructed_last, _Constructed_last };
                pointer&            _Constructed_first = _Guard._Constructed_last;

                std::uninitialized_fill_n(_Newvec + _Whereoff, _Count, _Value);
                _Constructed_first = _Newvec + _Whereoff;

                if ( _One_at_back )
                { // provide strong guarantee
                    if constexpr ( std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type> )
                        std::uninitialized_move(_Oldfirst, _Oldlast, _Newvec);
                    else
                        std::uninitialized_copy(_Oldfirst, _Oldlast, _Newvec);
                }
                else
                { // provide basic guarantee
                    std::uninitialized_move(this->_Myfirst, _Whereptr, _Newvec);
                    _Constructed_first = _Newvec;
                    std::uninitialized_move(_Whereptr, this->_Mylast, _Newvec + _Whereoff + _Count);
                }

                _Guard._New_begin = nullptr;
                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else if ( _One_at_back )
            { // provide strong guarantee
                _Emplace_back_with_unused_capacity(_Value);
            }
            else
            {                                                 // provide basic guarantee
                const value_type _Tmp               = _Value; // handle aliasing
                const size_t     _Affected_elements = static_cast<size_t>(_Oldlast - _Where);

                if ( _Count > _Affected_elements )
                { // new stuff spills off end
                    this->_Mylast = std::uninitialized_fill_n(_Oldlast, _Count - _Affected_elements, _Tmp);
                    this->_Mylast = std::uninitialized_move(_Whereptr, _Oldlast, this->_Mylast);
                    std::fill(_Whereptr, _Oldlast, _Tmp);
                }
                else
                { // new stuff can all be assigned
                    this->_Mylast = std::uninitialized_move(_Oldlast - _Count, _Oldlast, _Oldlast);
                    std::move_backward(_Whereptr, _Oldlast - _Count, _Oldlast);
                    std::fill_n(_Whereptr, _Count, _Tmp);
                }
            }
            return _Make_iterator_offset(_Whereoff);
        }

    private:
        // insert range [_First, _Last) at _Where
        template<class _Iter, class _Sent>
        void _Insert_uncounted_range(const_iterator _Where, _Iter _First, _Sent _Last)
        {
            if ( _First == _Last )
                return; // nothing to do, avoid invalidating iterators

            const size_t _Whereoff = static_cast<size_t>(_Where - this->_Myfirst);
            const size_t _Oldsize  = static_cast<size_t>(this->_Mylast - this->_Myfirst);

            _Append_uncounted_range(std::move(_First), std::move(_Last));
            std::rotate(this->_Myfirst + _Whereoff, this->_Myfirst + _Oldsize, this->_Mylast);
        }

        // insert counted range _First + [0, _Count) at _Where
        template<class _Iter>
        void _Insert_counted_range(const_iterator _Where, _Iter _First, const size_t _Count)
        {
            pointer      _Whereptr        = const_cast<pointer>(_Where);
            pointer      _Oldfirst        = this->_Myfirst;
            pointer      _Oldlast         = this->_Mylast;
            const size_t _Unused_capacity = static_cast<size_t>(this->_Myend - _Oldlast);
            if ( _Count == 0 )
            { // nothing to do, avoid invalidating iterators
            }
            else if ( _Count > _Unused_capacity )
            { // reallocate
                const size_t _Oldsize = static_cast<size_t>(_Oldlast - _Oldfirst);

                if ( _Count > max_size() - _Oldsize ) NH3API_UNLIKELY
                    nh3api::throw_exception<std::length_error>("vector::insert: size exceeded max_size");

                const size_t _Newsize     = _Oldsize + _Count;
                size_t       _Newcapacity = _Calculate_growth(_Newsize);

                pointer      _Newvec           = _Allocate(_Newcapacity);
                const size_t _Whereoff         = static_cast<size_t>(_Whereptr - _Oldfirst);
                pointer      _Constructed_last = _Newvec + _Whereoff + _Count;

                _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Constructed_last, _Constructed_last };
                pointer&            _Constructed_first = _Guard._Constructed_first;

                std::uninitialized_copy_n(std::move(_First), _Count, _Newvec + _Whereoff);
                _Constructed_first = _Newvec + _Whereoff;

                if ( _Count == 1 && _Whereptr == _Oldlast )
                { // one at back, provide strong guarantee
                    if constexpr ( std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type> )
                        std::uninitialized_move(_Oldfirst, _Oldlast, _Newvec);
                    else
                        std::uninitialized_copy(_Oldfirst, _Oldlast, _Newvec);
                }
                else
                { // provide basic guarantee
                    std::uninitialized_move(_Oldfirst, _Whereptr, _Newvec);
                    _Constructed_first = _Newvec;
                    std::uninitialized_move(_Whereptr, _Oldlast, _Newvec + _Whereoff + _Count);
                }

                _Guard._New_begin = nullptr;
                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else
            { // Attempt to provide the strong guarantee for EmplaceConstructible failure.
                // If we encounter copy/move construction/assignment failure, provide the basic guarantee.
                // (For one-at-back, this provides the strong guarantee.)

                const size_t _Affected_elements = static_cast<size_t>(_Oldlast - _Whereptr);

                if ( _Count < _Affected_elements )
                { // some affected elements must be assigned
                    _Mylast = std::uninitialized_move(_Oldlast - _Count, _Oldlast, _Oldlast);
                    std::move_backward(_Whereptr, _Oldlast - _Count, _Oldlast);
                    std::destroy(_Whereptr, _Whereptr + _Count);

                    if constexpr ( std::is_nothrow_copy_constructible_v<value_type> || std::is_trivially_copy_constructible_v<value_type> )
                    {
                        std::uninitialized_copy_n(std::move(_First), _Count, _Whereptr);
                    }
                    else
                    {
                        NH3API_TRY
                        {
                            std::uninitialized_copy_n(std::move(_First), _Count, _Whereptr);
                        }
                        NH3API_CATCH(...)
                        { // glue the broken pieces back together
                            _Vaporization_guard _Guard { this, _Whereptr, _Oldlast, _Whereptr + _Count };
                            std::uninitialized_move(_Whereptr + _Count, _Whereptr + 2 * _Count, _Whereptr);
                            _Guard._Target = nullptr;

                            std::move(_Whereptr + 2 * _Count, _Mylast, _Whereptr + _Count);
                            std::destroy(_Oldlast, _Mylast);
                            _Mylast = _Oldlast;
                            NH3API_RETHROW
                        }
                    }
                }
                else
                { // affected elements don't overlap before/after
                    pointer _Relocated = _Whereptr + _Count;
                    _Mylast            = std::uninitialized_move(_Whereptr, _Oldlast, _Relocated);
                    std::destroy(_Whereptr, _Oldlast);

                    if constexpr ( std::is_nothrow_copy_constructible_v<value_type> || std::is_trivially_copy_constructible_v<value_type> )
                    {
                        std::uninitialized_copy_n(std::move(_First), _Count, _Whereptr);
                    }
                    else
                    {
                        NH3API_TRY
                        {
                            std::uninitialized_copy_n(std::move(_First), _Count, _Whereptr);
                        }
                        NH3API_CATCH(...)
                        { // glue the broken pieces back together
                            _Vaporization_guard _Guard { this, _Whereptr, _Oldlast, _Relocated };
                            std::uninitialized_move(_Relocated, _Mylast, _Whereptr);
                            _Guard._Target = nullptr;

                            std::destroy(_Relocated, _Mylast);
                            _Mylast = _Oldlast;
                            NH3API_RETHROW
                        }
                    }
                }
            }
        }

    public:
#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
#else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
        iterator insert(const_iterator _Where, _Iter _First, _Iter _Last)
        {
            pointer _Whereptr = const_cast<pointer>(_Where);
            pointer _Oldfirst = this->_Myfirst;
            nh3api::verify_range(_First, _Last);
            auto         _UFirst   = nh3api::unfancy(_First);
            auto         _ULast    = nh3api::unfancy(_Last);
            const size_t _Whereoff = static_cast<size_t>(_Whereptr - _Oldfirst);
            if constexpr ( nh3api::is_cpp17_fwd_iter_v<_Iter> )
                _Insert_counted_range(_Where, _UFirst, static_cast<size_t>(std::distance(_UFirst, _ULast)));
#ifdef __cpp_lib_ranges
            else if ( std::forward_iterator<_Iter> )
                _Insert_counted_range(_Where, _UFirst, static_cast<size_t>(std::ranges::distance(_UFirst, _ULast)));
#endif
            else
                _Insert_uncounted_range(_Where, _UFirst, _ULast);
            return _Make_iterator_offset(_Whereoff);
        }

#ifdef __cpp_lib_containers_ranges
        template<nh3api::tt::container_compatible_range<value_type> _Range>
        iterator insert_range(const_iterator _Where, _Range&& _Rng)
        {
            pointer      _Whereptr = const_cast<pointer>(_Where);
            pointer      _Oldfirst = _Myfirst;
            const size_t _Whereoff = static_cast<size_t>(_Whereptr - _Oldfirst);

            if constexpr ( std::ranges::forward_range<_Range> || std::ranges::sized_range<_Range> )
                _Insert_counted_range(_Where, nh3api::unfancy_begin(_Rng), static_cast<size_t>(std::ranges::distance(_Rng)));
            else
                _Insert_uncounted_range(_Where, nh3api::unfancy_begin(_Rng), nh3api::unfancy_end(_Rng));

            return _Make_iterator_offset(_Whereoff);
        }
#endif

        iterator insert(const_iterator _Where, std::initializer_list<value_type> _Initializer_list)
        {
            pointer      _Whereptr = const_cast<pointer>(_Where);
            pointer      _Oldfirst = _Myfirst;
            const size_t _Whereoff = static_cast<size_t>(_Whereptr - _Oldfirst);

            _Insert_counted_range(_Where, _Initializer_list.begin(), _Initializer_list.size());
            return _Make_iterator_offset(_Whereoff);
        }

        void assign(const size_t _Newsize, const value_type& _Val)
        {
            const size_t _Oldcapacity = static_cast<size_t>(this->_Myend - this->_Myfirst);
            if ( _Newsize > _Oldcapacity )
            { // reallocate
                _Clear_and_reserve_geometric(_Newsize);
                _Mylast = std::uninitialized_fill_n(_Myfirst, _Newsize, _Val);

                return;
            }

            const size_t _Oldsize = static_cast<size_t>(_Mylast - _Myfirst);
            if ( _Newsize > _Oldsize )
            {
                std::fill(_Myfirst, _Mylast, _Val);
                _Mylast = std::uninitialized_fill_n(_Mylast, _Newsize - _Oldsize, _Val);
            }
            else
            {
                pointer _Newlast = this->_Myfirst + _Newsize;
                std::fill(this->_Myfirst, _Newlast, _Val);
                std::destroy(_Newlast, this->_Mylast);
                _Mylast = _Newlast;
            }
        }

    private:
        template<class _Iter, class _Sent>
        void _Assign_uncounted_range(_Iter _First, _Sent _Last)
        {
            pointer _Next = this->_Myfirst;
            for ( ; _First != _Last && _Next != this->_Mylast; ++_First, (void)++_Next )
                *_Next = *_First;

            std::destroy(_Next, _Mylast);
            _Mylast = _Next;
            _Append_uncounted_range(std::move(_First), std::move(_Last));
        }

        template<class _Iter>
        void _Assign_counted_range(_Iter _First, const size_t _Newsize)
        {
            const size_t _Oldcapacity = static_cast<size_t>(_Myend - _Myfirst);
            if ( _Newsize > _Oldcapacity )
            {
                _Clear_and_reserve_geometric(_Newsize);
                this->_Mylast = std::uninitialized_copy_n(std::move(_First), _Newsize, _Myfirst);
                return;
            }

            const size_t _Oldsize = static_cast<size_t>(_Mylast - _Myfirst);
            if ( _Newsize > _Oldsize )
            {
                for ( pointer _Mid = this->_Myfirst; _Mid != this->_Mylast; ++_Mid, (void)++_First )
                    *_Mid = *_First;

                this->_Mylast = std::uninitialized_copy_n(std::move(_First), _Newsize - _Oldsize, this->_Mylast);
            }
            else
            {
                pointer _Newlast = this->_Myfirst + _Newsize;
                std::copy_n(std::move(_First), _Newsize, this->_Myfirst);
                std::destroy(_Newlast, this->_Mylast);
                this->_Mylast = _Newlast;
            }
        }

    public:
#if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
#else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
#endif
        void assign(_Iter _First, _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            auto _UFirst = nh3api::unfancy(_First);
            auto _ULast  = nh3api::unfancy(_Last);

            if constexpr ( nh3api::is_cpp17_fwd_iter_v<_Iter> )
                _Assign_counted_range(_UFirst, static_cast<size_t>(std::distance(_UFirst, _ULast)));
#ifdef __cpp_lib_ranges
            else if constexpr ( std::forward_iterator<_Iter> )
                _Assign_counted_range(_UFirst, static_cast<size_t>(std::ranges::distance(_UFirst, _ULast)));
#endif
            else
                _Assign_uncounted_range(_UFirst, _ULast);
        }

#ifdef __cpp_lib_containers_ranges
        template<nh3api::tt::container_compatible_range<value_type> _Range>
        constexpr void assign_range(_Range&& _Rng)
        {
            static_assert(std::assignable_from<reference, std::ranges::range_reference_t<_Range>>,
                          "Elements must be assignable from the range's reference type (N4993 [sequence.reqmts]/60).");
            if constexpr ( std::ranges::sized_range<_Range> || std::ranges::forward_range<_Range> )
                _Assign_counted_range(nh3api::unfancy_begin(_Rng), static_cast<size_t>(std::ranges::distance(_Rng)));
            else
                _Assign_uncounted_range(nh3api::unfancy_begin(_Rng), nh3api::unfancy_end(_Rng));
        }
#endif

        void assign(const std::initializer_list<value_type>& _Initializer_list)
        {
            _Assign_counted_range(_Initializer_list.begin(), _Initializer_list.size());
        }

        exe_vector& operator=(const exe_vector& _Right)
        {
            if ( this == &_Right )
                return *this;

            _Assign_counted_range(_Right._Myfirst, static_cast<size_t>(_Right._Mylast - _Right._Myfirst));
            return *this;
        }

        exe_vector& operator=(const std::initializer_list<value_type>& _Initializer_list)
        {
            _Assign_counted_range(_Initializer_list.begin(), _Initializer_list.size());
            return *this;
        }

    private:
        void _Resize_reallocate(const size_t _Newsize, const value_type& _Value)
        {
            if ( _Newsize > max_size() ) NH3API_UNLIKELY
                nh3api::throw_exception<std::length_error>("vector::resize: vector too long");

            const size_t _Oldsize     = static_cast<size_t>(this->_Mylast - this->_Myfirst);
            size_t       _Newcapacity = _Calculate_growth(_Newsize);

            pointer _Newvec         = _Allocate(_Newcapacity);
            pointer _Appended_first = _Newvec + _Oldsize;

            _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Appended_first, _Appended_first };
            pointer&            _Appended_last = _Guard._Constructed_last;

            _Appended_last = std::uninitialized_fill_n(_Appended_first, _Newsize - _Oldsize, _Value);
            if constexpr ( std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type> )
                std::uninitialized_move(_Myfirst, _Mylast, _Newvec);
            else
                std::uninitialized_copy(_Myfirst, _Mylast, _Newvec);

            _Guard._New_begin = nullptr;
            _Change_array(_Newvec, _Newsize, _Newcapacity);
        }

        void _Resize_reallocate_default(const size_t _Newsize)
        {
            if ( _Newsize > max_size() ) NH3API_UNLIKELY
                nh3api::throw_exception<std::length_error>("vector::resize: vector too long");

            const size_t _Oldsize     = static_cast<size_t>(this->_Mylast - this->_Myfirst);
            size_t       _Newcapacity = _Calculate_growth(_Newsize);

            pointer _Newvec         = _Allocate(_Newcapacity);
            pointer _Appended_first = _Newvec + _Oldsize;

            _Reallocation_guard _Guard { _Newvec, _Newcapacity, _Appended_first, _Appended_first };
            pointer&            _Appended_last = _Guard._Constructed_last;

            _Appended_last = std::uninitialized_value_construct_n(_Appended_first, _Newsize - _Oldsize);
            if constexpr ( std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type> )
                std::uninitialized_move(_Myfirst, _Mylast, _Newvec);
            else
                std::uninitialized_copy(_Myfirst, _Mylast, _Newvec);

            _Guard._New_begin = nullptr;
            _Change_array(_Newvec, _Newsize, _Newcapacity);
        }

    public:
        void resize(const size_t _Newsize)
        {
            // trim or append value-initialized elements, provide strong guarantee
            const size_t _Oldsize = static_cast<size_t>(_Mylast - _Myfirst);
            if ( _Newsize < _Oldsize )
            { // trim
                pointer _Newlast = _Myfirst + _Newsize;
                std::destroy(_Newlast, _Mylast);
                _Mylast = _Newlast;
                return;
            }

            if ( _Newsize > _Oldsize )
            {     // append
                const size_t _Oldcapacity = static_cast<size_t>(this->_Myend - _Myfirst);
                if ( _Newsize > _Oldcapacity )
                { // reallocate
                    _Resize_reallocate_default(_Newsize);
                    return;
                }

                pointer _Oldlast = _Mylast;
                this->_Mylast    = std::uninitialized_value_construct_n(_Oldlast, _Newsize - _Oldsize);
            }

            // if _Newsize == _Oldsize, do nothing; avoid invalidating iterators
        }

        // trim or append elements, provide strong guarantee
        void resize(const size_t _Newsize, const value_type& _Value)
        {
            const size_t _Oldsize = static_cast<size_t>(_Mylast - _Myfirst);
            if ( _Newsize < _Oldsize )
            { // trim
                pointer _Newlast = _Myfirst + _Newsize;
                std::destroy(_Newlast, _Mylast);
                _Mylast = _Newlast;
                return;
            }

            if ( _Newsize > _Oldsize )
            {     // append
                const size_t _Oldcapacity = static_cast<size_t>(this->_Myend - _Myfirst);
                if ( _Newsize > _Oldcapacity )
                { // reallocate
                    _Resize_reallocate(_Newsize, _Value);
                    return;
                }

                pointer _Oldlast = _Mylast;
                this->_Mylast    = std::uninitialized_fill_n(_Oldlast, _Newsize - _Oldsize, _Value);
            }

            // if _Newsize == _Oldsize, do nothing; avoid invalidating iterators
        }

    private:
        void _Reallocate(const size_t _Newcapacity)
        {
            const size_t _Size   = static_cast<size_t>(_Mylast - _Myfirst);
            pointer      _Newvec = _Allocate(_Newcapacity);

            _Simple_reallocation_guard _Guard { _Newvec };

            if constexpr ( std::is_nothrow_move_constructible_v<value_type> || !std::is_copy_constructible_v<value_type> )
                std::uninitialized_move(_Myfirst, _Mylast, _Newvec);
            else
                std::uninitialized_copy(_Myfirst, _Mylast, _Newvec);

            _Guard._New_begin = nullptr;
            _Change_array(_Newvec, _Size, _Newcapacity);
        }

        void _Clear_and_reserve_geometric(const size_t _Newsize)
        {
            if ( _Newsize > max_size() ) NH3API_UNLIKELY
                nh3api::throw_exception<std::length_error>("vector::reserve: vector too long");

            const size_t _Newcapacity = _Calculate_growth(_Newsize);
            if ( _Myfirst )
            { // destroy and deallocate old array
                std::destroy(_Myfirst, _Mylast);
                _Deallocate(_Myfirst);

                // equivalent to std::memset(this, 0, sizeof(*this));
                reinterpret_cast<std::array<uint32_t, 4>&>(*this) = {};
            }

            _Buy_raw(_Newcapacity);
        }

    public:
        void reserve(const size_t _Newcapacity)
        {
            if ( _Newcapacity > capacity() )
            {
                if ( _Newcapacity > max_size() ) NH3API_UNLIKELY
                    nh3api::throw_exception<std::length_error>("vector::reserve: vector too long");

                _Reallocate(_Newcapacity);
            }
        }

        void shrink_to_fit()
        {
            // something to do
            if ( _Mylast != _Myend )
            {
                if ( _Myfirst == _Mylast )
                    _Tidy();
                else
                    _Reallocate(static_cast<size_t>(_Mylast - _Myfirst));
            }
        }

        void pop_back() noexcept
        {
            if ( _Myfirst != _Mylast )
            {
                std::destroy_at(_Mylast - 1);
                --_Mylast;
            }
        }

        iterator erase(const_iterator _Where)
        noexcept(std::is_nothrow_move_assignable_v<value_type>)
        {
            iterator _Whereptr = const_cast<iterator>(_Where);
            if ( _Where >= _Myfirst && _Where <= _Mylast )
            {
                std::move(_Whereptr + 1, _Mylast, _Whereptr);
                std::destroy_at(_Mylast - 1);
                --_Mylast;
            }
            return _Whereptr;
        }

        iterator erase(const_iterator _First, const_iterator _Last)
        {
            nh3api::verify_range(_First, _Last);
            if (_First < _Myfirst || _Last > _Mylast || _First > _Last) NH3API_UNLIKELY
                nh3api::throw_exception<std::out_of_range>("vector::erase iterator outside range");
            else
            {
                // something to do, invalidate iterators
                if ( _First != _Last )
                {
                    pointer _Newlast = std::move(const_cast<pointer>(_Last), _Mylast, const_cast<pointer>(_First));
                    std::destroy(_Newlast, _Mylast);
                    _Mylast = _Newlast;
                }
            }
            return const_cast<iterator>(_First);
        }

        void clear() noexcept
        {
            if ( _Myfirst == _Mylast )
                return;

            std::destroy(_Myfirst, _Mylast);
            _Mylast = _Myfirst;
        }

        void swap(exe_vector& _Right) noexcept
        {
            auto& _Lhs = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
            auto& _Rhs = reinterpret_cast<std::array<uint32_t, 4>&>(_Right);
            std::swap(_Lhs, _Rhs);
        }

        inline pointer data() noexcept
        { return _Myfirst; }

        [[nodiscard]] inline const_pointer data() const noexcept
        { return _Myfirst; }

        iterator begin() noexcept
        { return _Myfirst; }

        const_iterator begin() const noexcept
        { return _Myfirst; }

        const_iterator cbegin() const noexcept
        { return _Myfirst; }

        iterator end() noexcept
        { return _Mylast; }

        const_iterator end() const noexcept
        { return _Mylast; }

        const_iterator cend() const noexcept
        { return _Mylast; }

        reverse_iterator rbegin() noexcept
        { return reverse_iterator { end() }; }

        const_reverse_iterator rbegin() const noexcept
        { return const_reverse_iterator { end() }; }

        const_reverse_iterator crbegin() const noexcept
        { return const_reverse_iterator { end() }; }

        reverse_iterator rend() noexcept
        { return reverse_iterator { begin() }; }

        const_reverse_iterator rend() const noexcept
        { return const_reverse_iterator { begin() }; }

        const_reverse_iterator crend() const noexcept
        { return const_reverse_iterator { begin() }; }

        [[nodiscard]] inline size_t size() const noexcept
        { return static_cast<size_t>(this->_Mylast - this->_Myfirst); }

        [[nodiscard]] inline constexpr static size_t max_size() noexcept
        { return size_t(~0U) / sizeof(value_type); }

        [[nodiscard]] inline bool empty() const noexcept
        { return _Myfirst == _Mylast; }

        [[nodiscard]] size_t capacity() const noexcept
        { return static_cast<size_t>(this->_Myend - this->_Myfirst); }

        inline constexpr static allocator_type get_allocator() noexcept
        { return {}; }

        const_reference at(size_t pos) const
        {
            if ( pos >= size() ) NH3API_UNLIKELY
                nh3api::throw_exception<std::out_of_range>("vector: invalid subscript");

            return *(_Myfirst + pos);
        }

        reference at(size_t pos)
        {
            if ( size() <= pos ) NH3API_UNLIKELY
                nh3api::throw_exception<std::out_of_range>("vector: invalid subscript");

            return *(_Myfirst + pos);
        }

        inline const_reference operator[](size_t pos) const noexcept
        { return *(_Myfirst + pos); }

        inline reference operator[](size_t pos) noexcept
        { return *(_Myfirst + pos); }

        inline reference front() noexcept
        {
            assert(_Myfirst);
            return *_Myfirst;
        }

        inline const_reference front() const noexcept
        {
            assert(_Myfirst);
            return *_Myfirst;
        }

        inline reference back() noexcept
        {
            assert(_Mylast);
            return _Mylast[-1];
        }

        inline const_reference back() const noexcept
        {
            assert(_Mylast);
            return _Mylast[-1];
        }

    private:
        // return the iterator begin() + _Offset
        iterator _Make_iterator_offset(const size_t _Offset) noexcept
        { return this->_Myfirst + _Offset; }

        [[nodiscard]] static inline pointer _Allocate(size_t _Requested) noexcept
        { return static_cast<pointer>(::operator new(_Requested * sizeof(value_type), exe_heap, std::nothrow)); }

        inline static void _Deallocate(void* _Ptr) noexcept
        { ::operator delete(_Ptr, exe_heap); }

        // allocate array with _Capacity elements
        void _Buy_raw(size_t _Newcapacity)
        {
            NH3API_ASSUME(!_Myfirst && !_Mylast && !_Myend); // check that *this is tidy
            this->_Myfirst = _Allocate(_Newcapacity);
            this->_Mylast  = this->_Myfirst;
            this->_Myend   = this->_Myfirst + _Newcapacity;
        }

        // given _Oldcapacity and _Newsize, calculate geometric growth
        [[nodiscard]] size_t _Calculate_growth(const size_t _Newsize) const noexcept
        {
            const size_t _Oldcapacity = capacity();
            if ( _Oldcapacity > max_size() - _Oldcapacity / 2 )
                return max_size(); // geometric growth would overflow

            const size_t _Geometric = _Oldcapacity + _Oldcapacity / 2;
            if ( _Geometric < _Newsize )
                return _Newsize; // geometric growth would be insufficient
            return _Geometric;   // geometric growth is sufficient
        }

        // discard old array, acquire new array
        void _Change_array(pointer      _Newvec,
                           const size_t _Newsize,
                           const size_t _Newcapacity) noexcept
        {
            if ( _Myfirst )
            {
                std::destroy(_Myfirst, _Mylast);
                _Deallocate(_Myfirst);
            }

            this->_Myfirst = _Newvec;
            this->_Mylast  = _Newvec + _Newsize;
            this->_Myend   = _Newvec + (_Newcapacity ? _Newcapacity : 0);
        }

        // free all storage
        void _Tidy() noexcept
        {
            if ( this->_Myfirst != nullptr )
            { // something to free, destroy and deallocate it
                std::destroy(this->_Myfirst, this->_Mylast);
                _Deallocate(this->_Myfirst);
                reinterpret_cast<std::array<uint32_t, 4>&>(*this) = {};
            }
        }

        // Dispatches between the three sized constructions.
        // 1-arg -> value-construction, e.g. vector(5)
        // 2-arg -> fill, e.g. vector(5, "meow")
        // 3-arg -> sized range construction, e.g. vector{"Hello", "Fluffy", "World"}
        template<typename... _Args>
        void _Construct_n(const size_t _Count, _Args&&... _args)
        {
            if ( _Count != 0 )
            {
                if ( _Count > max_size() ) NH3API_UNLIKELY
                    nh3api::throw_exception<std::length_error>("vector: construction failure - size too large");

                _Buy_raw(_Count);
                _Tidy_guard _Guard { this };
                if constexpr ( sizeof...(_args) == 0 )      // construct filling with the default value
                    this->_Mylast = std::uninitialized_value_construct_n(this->_Myfirst, _Count);
                else if constexpr ( sizeof...(_args) == 1 ) // construct filling with a value
                    this->_Mylast = std::uninitialized_fill_n(this->_Myfirst, _Count, _args...);
                else if constexpr ( sizeof...(_args) == 2 ) // construct copy from two iterators
                    this->_Mylast = std::uninitialized_copy(std::forward<_Args>(_args)..., this->_Myfirst);
                else
                    static_assert(false, "exe_vector::exe_vector: unexpected number of arguments");

                _Guard._Target = nullptr;
            }
            else
            {
                reinterpret_cast<std::array<uint32_t, 4>&>(*this) = {};
            }
        }

    protected:
        uint32_t : 32;
        pointer _Myfirst; // pointer to beginning of array
        pointer _Mylast;  // pointer to current end of sequence
        pointer _Myend;   // pointer to end of array
};

#pragma pack(pop) // 4

// deduction guide for iterator-based constructor
template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, int> = 0>
exe_vector(_Iter, _Iter) -> exe_vector<typename std::iterator_traits<_Iter>::value_type>;

#ifdef __cpp_lib_containers_ranges
template<std::ranges::input_range _Range>
exe_vector(std::from_range_t, _Range&&) -> exe_vector<std::ranges::range_value_t<_Range>>;
#endif

template<class _Ty>
inline bool operator==(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return _Left.size() == _Right.size() && std::equal(_Left.begin(), _Left.end(), _Right.begin()); }

#ifdef __cpp_lib_three_way_comparison
template<typename _Ty>
inline nh3api::synth_three_way_result<_Ty> operator<=>(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{
    return std::lexicographical_compare_three_way(_Left.begin(), _Left.end(),
            _Right.begin(), _Right.end(), nh3api::synth_three_way);
}
#else
template<class _Ty>
inline bool operator!=(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return !(_Left == _Right); }

template<class _Ty>
inline bool operator<(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return std::lexicographical_compare(_Left.begin(), _Left.end(), _Right.begin(), _Right.end()); }

template<class _Ty>
inline bool operator>(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return _Right < _Left; }

template<class _Ty>
inline bool operator<=(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return !(_Right < _Left); }

template<class _Ty>
inline bool operator>=(const exe_vector<_Ty>& _Left, const exe_vector<_Ty>& _Right) noexcept
{ return !(_Left < _Right); }

#endif

NH3API_WARNING(pop)
