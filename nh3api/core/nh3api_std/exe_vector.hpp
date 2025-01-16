//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// implementation of vector partially from MSVC STL
// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include "intrin.hpp"
#include "nh3api_std.hpp"
#include "memory.hpp"
#include "algorithm.hpp"
#include "iterator.hpp"
#include "type_traits/is_xyz.hpp"
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
#include <stdexcept>  // std::length_error, std::out_of_range, std::runtime_error
#endif
#include "type_traits.hpp"
#include <utility>

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)
NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

//namespace
//{
template<typename AllocatorT>
struct exe_vector_helper
{
    public:
        typedef AllocatorT allocator_type;

    #if NH3API_CHECK_CPP11
    protected:
        typedef std::allocator_traits<allocator_type> traits;

    public:
        typedef typename traits::size_type size_type;
        typedef typename traits::difference_type difference_type;

        typedef typename traits::value_type    value_type;
        typedef const value_type               const_value_type;
        typedef value_type&                    reference;
        typedef const value_type&              const_reference;
        typedef typename traits::pointer       pointer;
        typedef typename traits::const_pointer const_pointer;

        typedef typename traits::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
        typedef typename traits::propagate_on_container_move_assignment propagate_on_container_move_assignment;
        typedef typename traits::propagate_on_container_swap propagate_on_container_swap;
        typedef typename traits::is_always_equal is_always_equal;

    #else
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::value_type      value_type;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef const value_type                     	 const_value_type;

        typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
        typedef nh3api::tt::false_type propagate_on_container_move_assignment;
        typedef nh3api::tt::false_type propagate_on_container_swap;
        typedef nh3api::tt::is_empty<allocator_type> is_always_equal;
    #endif

    public:
        NH3API_CONSTEXPR_CPP_20
        exe_vector_helper()
            NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<allocator_type>::value)
            : alloc()
        {}

        NH3API_CONSTEXPR_CPP_20
        explicit exe_vector_helper(const allocator_type& allocator)
            : alloc(allocator)
        {}

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_CONSTEXPR_CPP_20
        exe_vector_helper(allocator_type&& other)
            : alloc(std::move(other))
        {}

        NH3API_CONSTEXPR_CPP_20
        exe_vector_helper(exe_vector_helper&& other)
            : alloc(std::move(other.alloc))
        {}

        exe_vector_helper& operator=(exe_vector_helper&& other)
        {
            this->alloc = std::move(other.alloc);
            return *this;
        }
        #endif

        NH3API_CONSTEXPR_CPP_20
        allocator_type select_on_container_copy_construction() const
        {
            #if NH3API_CHECK_CPP11
            return traits::select_on_container_copy_construction(alloc);
            #else
            return alloc;
            #endif
        }

        NH3API_CONSTEXPR
        exe_vector_helper(const nh3api::dummy_tag_t& tag)
        { NH3API_IGNORE(alloc); }

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        size_type max_size() const NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP11
            return traits::max_size(alloc);
            #else
            return alloc.max_size();
            #endif
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        pointer allocate(size_type num, const void* ptr)
        {
            return alloc.allocate(num, ptr);
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        pointer allocate(size_type num)
        {
            return alloc.allocate(num, nullptr);
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        void deallocate(void* ptr, size_type num)
        NH3API_NOEXCEPT
        {
            alloc.deallocate(ptr, num);
        }

        #if NH3API_STD_MOVE_SEMANTICS
        template<typename OtherType, typename NH3API_ARGS_DOTS Args >
        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        void construct(OtherType* ptr, Args&& NH3API_ARGS_DOTS args)
        { alloc.construct(ptr, std::forward<Args>(args) NH3API_ARGS_DOTS ); }
        #else
        template<typename OtherType> NH3API_FORCEINLINE
        void construct(OtherType* ptr, const_reference value)
        { alloc.construct(ptr, value); }
        #endif

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        void destroy(pointer ptr)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
        { nh3api::destroy_at(ptr); }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        void destroy_range(pointer first, pointer last)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
        { nh3api::destroy(first, last, alloc); }

        template<class IterT> NH3API_CONSTEXPR_CPP_20
        pointer ucopy(IterT first, IterT last, pointer d_first)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<value_type>::value)
        {
            return nh3api::uninitialized_copy(first, last, d_first, alloc);
        }

        NH3API_CONSTEXPR_CPP_20
        pointer umove(pointer first, pointer last, pointer ptr)
        {
            #if !NH3API_CHECK_CPP11 // old MSVC compiler
                return nh3api::uninitialized_move(first, last, ptr, alloc);
            #else
                NH3API_STATIC_ASSERT("value_type must be either copy or move constructible",
                nh3api::tt::is_move_constructible<value_type>::value || nh3api::tt::is_copy_constructible<value_type>::value);
                NH3API_IF_CONSTEXPR ( nh3api::tt::is_nothrow_move_constructible<value_type>::value
                                      || !nh3api::tt::is_copy_constructible<value_type>::value )
                    return nh3api::uninitialized_move(first, last, ptr, alloc);
                else if ( nh3api::tt::is_copy_constructible<value_type>::value )
                    return nh3api::uninitialized_copy(first, last, ptr, alloc);
            #endif
        }

        NH3API_CONSTEXPR_CPP_20
        pointer move_backward(pointer first, pointer last, pointer ptr)
        {
            #if !NH3API_CHECK_CPP11 // old MSVC compiler
            return std::_Move_backward(first, last, ptr);
            #else
            NH3API_STATIC_ASSERT("value_type must be either copy or move constructible",
            nh3api::tt::is_move_assignable<value_type>::value || nh3api::tt::is_copy_assignable<value_type>::value);

            NH3API_IF_CONSTEXPR ( nh3api::tt::is_nothrow_move_assignable<value_type>::value
                                  || !nh3api::tt::is_copy_assignable<value_type>::value )
                return std::move_backward(first, last, ptr);
            else if ( nh3api::tt::is_copy_assignable<value_type>::value )
                 return std::copy_backward(first, last, ptr);
            #endif
        }

        NH3API_CONSTEXPR_CPP_20
        pointer ufill(pointer first, size_type count, const value_type& value)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<value_type>::value)
        {
            nh3api::uninitialized_fill_n(first, count, value, alloc);
            return first + count;
        }

        NH3API_CONSTEXPR_CPP_20
        pointer default_fill(pointer first, size_type count)
        {
            return nh3api::uninitialized_default_construct_n(first, count, alloc);
        }

    public:
        allocator_type& alloc;
};

// partial template specialization to improve compilation speed
template<typename T>
struct exe_vector_helper<exe_allocator<T> >
{
    public:
        typedef exe_allocator<T> allocator_type;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef T*               pointer;
        typedef const T*         const_pointer;
        typedef T&               reference;
        typedef const T&         const_reference;
        typedef T                value_type;
        typedef const T          const_value_type;

        typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
        typedef nh3api::tt::false_type propagate_on_container_move_assignment;
        typedef nh3api::tt::false_type propagate_on_container_swap;
        typedef nh3api::tt::true_type is_always_equal;

    public:
        NH3API_CONSTEXPR
        exe_vector_helper() NH3API_NOEXCEPT
            : alloc()
        {}

        NH3API_CONSTEXPR
        exe_vector_helper(const allocator_type& allocator) NH3API_NOEXCEPT
            : alloc(allocator)
        {}

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_CONSTEXPR
        exe_vector_helper(allocator_type&& other) NH3API_NOEXCEPT
            : alloc(std::move(other))
        {}

        NH3API_CONSTEXPR
        exe_vector_helper(exe_vector_helper&& other) NH3API_NOEXCEPT
            : alloc(std::move(other.alloc))
        {}

        exe_vector_helper& operator=(exe_vector_helper&& other)
        {
            return *this;
        }
        #endif

        NH3API_CONSTEXPR
        allocator_type select_on_container_copy_construction() const
        { return alloc; }

        NH3API_CONSTEXPR
        exe_vector_helper(const nh3api::dummy_tag_t& tag)
        { NH3API_IGNORE(alloc); }

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        static size_t max_size() NH3API_NOEXCEPT
        {
            const size_t _N = (size_t)(-1) / sizeof(value_type);
            return (0 < _N ? _N : 1);
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_type num, const void*)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return ((T*)::operator new(
                (size_t)num * sizeof(T), exe_heap));
            #else
                return ((T*)::operator new(
                (size_t)num * sizeof(T), exe_heap, std::nothrow));
            #endif
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_type num)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return ((T*)::operator new(
                (size_t)num * sizeof(T), exe_heap));
            #else
                return ((T*)::operator new(
                (size_t)num * sizeof(T), exe_heap, std::nothrow));
            #endif
        }

        NH3API_FORCEINLINE
        static void deallocate(void* ptr, size_type)
        NH3API_NOEXCEPT
        { exe_delete(ptr); }

        #if NH3API_STD_MOVE_SEMANTICS
        template <typename NH3API_ARGS_DOTS Args > NH3API_FORCEINLINE
        static void construct(void* ptr, Args&& NH3API_ARGS_DOTS args)
        { ::new (ptr) value_type(std::forward<Args>(args) NH3API_ARGS_DOTS ); }
        #else
        NH3API_FORCEINLINE
        static void construct(void* ptr, const_reference value)
        { ::new (ptr) value_type(value); }
        #endif

        NH3API_FORCEINLINE
        static void destroy(pointer ptr)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
        { ptr->~T(); }

        NH3API_FORCEINLINE
        static void destroy_range(pointer first, pointer last)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
        { nh3api::destroy<pointer>(first, last); }

        template<class IterT> NH3API_FORCEINLINE
        static pointer ucopy(IterT first, IterT last, pointer d_first)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<value_type>::value)
        { return std::uninitialized_copy<IterT, pointer>(first, last, d_first); }

        NH3API_FORCEINLINE
        static pointer umove(pointer first, pointer last, pointer ptr)
        {
            #if !NH3API_CHECK_CPP11 // old MSVC compiler
                return nh3api::uninitialized_move(first, last, ptr, alloc);
            #else
                NH3API_STATIC_ASSERT("value_type must be either copy or move constructible",
                nh3api::tt::is_move_constructible<value_type>::value || nh3api::tt::is_copy_constructible<value_type>::value);
                NH3API_IF_CONSTEXPR ( nh3api::tt::is_nothrow_move_constructible<value_type>::value
                                        || !nh3api::tt::is_copy_constructible<value_type>::value )
                    return std::uninitialized_move<pointer, pointer>(first, last, ptr);
                else if ( nh3api::tt::is_copy_constructible<value_type>::value )
                    return std::uninitialized_copy<pointer, pointer>(first, last, ptr);
            #endif
        }

        NH3API_FORCEINLINE
        static pointer move_backward(pointer first, pointer last, pointer ptr)
        {
            #if !NH3API_CHECK_CPP11 // old MSVC compiler
            return std::_Move_backward(first, last, ptr);
            #else
            NH3API_STATIC_ASSERT("value_type must be either copy or move constructible",
            nh3api::tt::is_move_assignable<value_type>::value || nh3api::tt::is_copy_assignable<value_type>::value);

            NH3API_IF_CONSTEXPR ( nh3api::tt::is_nothrow_move_assignable<value_type>::value
                                  || !nh3api::tt::is_copy_assignable<value_type>::value )
                return std::move_backward<pointer, pointer>(first, last, ptr);
            else if ( nh3api::tt::is_copy_assignable<value_type>::value )
                 return std::copy_backward<pointer, pointer>(first, last, ptr);
            #endif
        }

        NH3API_FORCEINLINE
        static pointer ufill(pointer first, size_type count, const value_type& value)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<value_type>::value)
        {
            std::uninitialized_fill_n<pointer, size_type, value_type>(first, count, value);
            return first + count;
        }

        NH3API_FORCEINLINE
        pointer default_fill(pointer first, size_type count)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<value_type>::value)
        {
            return nh3api::init_array(first, count);
        }

    public:
        allocator_type alloc;
};
//} // anonymous namespace

//namespace nh3api
//{
#pragma pack(push, 4)

    /// @brief  Visual C++ 6.0 std::vector implementation used by heroes3.exe
    /// @tparam _Ty stored type
    /// @tparam _A allocator
    template<
        class _Ty,
        class _A = exe_allocator<_Ty> >
    class exe_vector
    {
    protected:
        NH3API_STATIC_ASSERT("The C++ Standard forbids containers of non-object types "
                             "because of [container.requirements].",
                             nh3api::tt::is_object<_Ty>::value);
        NH3API_STATIC_ASSERT("The C++ Standard requires that Allocator's value_type match",
                             nh3api::tt::is_same<_Ty, typename _A::value_type>::value);
    public:
        typedef _A allocator_type;

    // internal typedefs
    protected:
        typedef exe_vector_helper<allocator_type> helper_type;
        typedef exe_vector<_Ty, _A> this_type;
        typedef typename helper_type::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
        typedef typename helper_type::propagate_on_container_move_assignment propagate_on_container_move_assignment;
        typedef typename helper_type::propagate_on_container_swap propagate_on_container_swap;

        typedef
        #if !defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
        nh3api::tt::is_nothrow_copy_constructible<_Ty>
        #else
        std::false_type
        #endif
        noexcept_copy;

        typedef
        #if NH3API_STD_MOVE_SEMANTICS && !defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
        nh3api::tt::is_nothrow_move_constructible<_Ty>
        #else
        std::false_type
        #endif
        noexcept_move;

    // external typedefs
    public:
        typedef typename helper_type::size_type       size_type;
        typedef typename helper_type::difference_type difference_type;
        typedef typename helper_type::reference       reference;
        typedef typename helper_type::const_reference const_reference;
        typedef typename helper_type::value_type      value_type;
        typedef typename helper_type::pointer         pointer;
        typedef typename helper_type::const_pointer   const_pointer;

        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::reverse_iterator<iterator>       reverse_iterator;

    protected:
        #ifndef NH3API_VECTOR_TRY_CATCH_TIDY
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                #define NH3API_VECTOR_TRY_CATCH_TIDY(NO_UNWIND_CONDITION, ...) \
                if (NO_UNWIND_CONDITION)\
                {\
                    __VA_ARGS__\
                }\
                else\
                {\
                    NH3API_TRY\
                    {\
                        __VA_ARGS__\
                    }\
                    NH3API_CATCH(...)\
                    {\
                        _Tidy();\
                        NH3API_RETHROW\
                    }\
                }
            #else
                #define NH3API_VECTOR_TRY_CATCH_TIDY(NO_UNWIND_CONDITION, ...) __VA_ARGS__
            #endif
        #endif

        #ifndef NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                #define NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE(NO_UNWIND_CONDITION, DESTROY_FIRST, DESTROY_LAST, DEALLOC_AT, DEALLOC_SIZE, ...) \
                if (NO_UNWIND_CONDITION)\
                {\
                    __VA_ARGS__\
                }\
                else\
                {\
                    NH3API_TRY\
                    {\
                        __VA_ARGS__\
                    }\
                    NH3API_CATCH(...)\
                    {\
                        helper.destroy_range(DESTROY_FIRST, DESTROY_LAST);\
                        helper.deallocate(DEALLOC_AT, DEALLOC_SIZE);\
                        NH3API_RETHROW\
                    }\
                }
            #else
                #define NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE(NO_UNWIND_CONDITION, DESTROY_FIRST, DESTROY_LAST, DEALLOC_AT, DEALLOC_SIZE, ...) __VA_ARGS__
            #endif
        #endif

        template<class IterT>
        void _Range_construct_or_tidy(IterT _F, IterT _L, std::input_iterator_tag)
        {
        #if NH3API_STD_MOVE_SEMANTICS
            NH3API_VECTOR_TRY_CATCH_TIDY
            (
            !nh3api::tt::allocator_may_throw<allocator_type>::value && noexcept_move::value,
            for (; _F != _L; ++_F)
                emplace_back(*_F);
            )
        #else
            NH3API_VECTOR_TRY_CATCH_TIDY
            (
            !nh3api::tt::allocator_may_throw<allocator_type>::value && noexcept_copy::value,
            for (; _F != _L; ++_F)
                push_back(*_F);
            )
        #endif
        }

        template<class IterT>
        void _Range_construct_or_tidy(IterT _F, IterT _L, std::forward_iterator_tag)
        {
            if ( _Buy( static_cast<size_type>(std::distance(_F, _L))) )
            {
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                !nh3api::tt::allocator_may_throw<allocator_type>::value && noexcept_copy::value,
                this->_Last = helper.ucopy(_F, _L, this->_First);
                )
            }
        }
    public:
        exe_vector()
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<allocator_type>::value)
            : helper(), _First(nullptr), _Last(nullptr), _End(nullptr)
        {}

        explicit exe_vector(const allocator_type& a)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<allocator_type>::value)
            : helper(a), _First(nullptr), _Last(nullptr), _End(nullptr)
        {}

        explicit exe_vector(size_type n)
            : helper()
        {
            if (_Buy(n))
            { // nonzero, fill it
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                    !nh3api::tt::allocator_may_throw<allocator_type>::value
                    && nh3api::tt::is_nothrow_default_constructible<value_type>::value,
                    this->_Last = helper.default_fill(this->_First, n);
                )
            }
        }

    #if NH3API_CHECK_CPP14
        explicit exe_vector(size_type n, const allocator_type& a)
            : helper(a)
        {
            if (_Buy(n))
            { // nonzero, fill it
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                    nh3api::tt::is_nothrow_default_constructible<value_type>::value,
                    this->_Last = helper.default_fill(this->_First, n);
                )
            }
        }
    #endif

        exe_vector(size_type n, const value_type& value, const allocator_type& a = allocator_type())
            : helper(a)
        {
            if (_Buy(n))
            {	// nonzero, fill it
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                    noexcept_copy::value,
                    this->_Last = helper.ufill(this->_First, n, value);
                )
            }
        }

        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_vector( IterT first, IterT last, const allocator_type& a = allocator_type()
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
            : helper(a)
        {
            nh3api::verify_range(first, last);
            _Range_construct_or_tidy(nh3api::unfancy(first),
                                     nh3api::unfancy(last),
                                     nh3api::iter_category<IterT>());
        }

        exe_vector(const this_type& _Right)
            : helper( _Right.helper.select_on_container_copy_construction() )
        {
            if (_Buy(_Right.size()))
            {
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                    noexcept_copy::value,
                    this->_Last = helper.ucopy(_Right.begin(), _Right.end(), this->_First);
                )
            }
        }

        // no-op constructor optimization for low-level code
        exe_vector(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : helper(tag)
        { NH3API_IGNORE(_First, _Last, _End); }

    #if NH3API_STD_MOVE_SEMANTICS
    protected:
        void _Move_from(exe_vector&& _Right)
            NH3API_NOEXCEPT_EXPR(propagate_on_container_move_assignment::value
                                 && helper_type::is_always_equal::value)
        {
            _Move_from(std::forward<exe_vector>(_Right),
            propagate_on_container_move_assignment());
        }

        NH3API_FORCEINLINE
        void _Move_from(exe_vector&& _Right, nh3api::tt::true_type) NH3API_NOEXCEPT
        {
            _Tidy_deallocate();
            this->_Move_nullify(&_Right);
        }

        void _Move_from(exe_vector&& _Right, nh3api::tt::false_type)
        NH3API_NOEXCEPT_EXPR(helper_type::is_always_equal::value)
        {
            if (get_allocator() == _Right.get_allocator())
                _Move_from(std::move(_Right), nh3api::tt::true_type());
            else if (_Buy(_Right.size()))
                this->_Last = helper.umove(_Right._First, _Right._Last, this->_First);
        }

        // move from _Right, stealing its contents
        void _Move_assign_from(exe_vector&& _Right, nh3api::tt::true_type) NH3API_NOEXCEPT
        {
            _Move_from(std::move(_Right), nh3api::tt::true_type{});
        }

        // move from _Right, possibly moving its contents
        void _Move_assign_from(exe_vector&& _Right, nh3api::tt::false_type)
        {
            if (this->get_allocator() == _Right.get_allocator())
            {
                _Move_from(std::move(_Right), nh3api::tt::true_type{});
                return;
            }

            const pointer _F = _Right._First;
            const pointer _L = _Right._Last;
            const size_type _Newsize = _Right.size();
            const size_type _Oldsize = size();
            const size_type _Oldcapacity = capacity();

            if (_Newsize > _Oldcapacity)
            { // reallocate
                if (_Newsize > max_size())
                {
                    NH3API_THROW(std::length_error("vector::assign: too much size requested"));
                }

                const size_type _Newcapacity = _Calculate_growth(_Newsize);

                if (this->_First != pointer())
                { // destroy and deallocate old array
                    helper.destroy_range(this->_First, this->_Last);
                    helper.deallocate(this->_First, _Oldcapacity);
                }

                _Buy(_Newcapacity);
                this->_Last = helper.umove(_F, _L, this->_First);
            }
            else if (_Newsize > _Oldsize)
            {
                const pointer _Mid = _F + _Oldsize;
                std::move(_F, _Mid, this->_First);
                this->_Last = helper.umove(_Mid, _L, this->_Last);
            }
            else
            {
                const pointer _Newlast = this->_First + _Newsize;
                std::move(_F, _L, this->_First);
                helper.destroy_range(_Newlast, this->_Last);
                this->_Last = _Newlast;
            }
        }

        template<class NH3API_ARGS_DOTS Args>
        #if NH3API_CHECK_CPP17
        reference
        #else
        void
        #endif
        _Emplace_back_with_unused_capacity(Args&& NH3API_ARGS_DOTS args)
        {
            helper.construct(_Last, std::forward<Args>(args) NH3API_ARGS_DOTS);
            #if NH3API_CHECK_CPP17
            reference result = *_Last;
            ++_Last;
            return result;
            #else
            ++_Last;
            #endif
        }

        template<class NH3API_ARGS_DOTS Args>
        pointer _Emplace_reallocate(const pointer _Whereptr, Args&& NH3API_ARGS_DOTS _Val)
        { // reallocate and insert by perfectly forwarding _Val at _Whereptr
          // pre: !_Has_unused_capacity()
            const size_type _Whereoff = static_cast<size_type>(_Whereptr - this->_First);
            const size_type _Oldsize = size();

            if (_Oldsize == max_size())
            {
                NH3API_THROW(std::length_error("vector::emplace: size exceeded max_size"));
            }

            const size_type _Newsize = _Oldsize + 1;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            const pointer _Newvec = helper.allocate(_Newcapacity);
            const pointer _Constructed_last = _Newvec + _Whereoff + 1;
            pointer _Constructed_first = _Constructed_last;

            NH3API_CONSTEXPR_VAR bool nothrow_constructible =
            nh3api::tt::is_nothrow_constructible<value_type, Args NH3API_ARGS_DOTS>::value;

            NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
            (
                nothrow_constructible && noexcept_move::value && noexcept_copy::value,
                _Constructed_first, _Constructed_last, _Newvec, _Newcapacity,

                helper.construct(_Newvec + _Whereoff, std::forward<Args>(_Val)...);
                _Constructed_first = _Newvec + _Whereoff;
                if (_Whereptr == this->_Last)
                { // at back, provide strong guarantee
                    _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                }
                else
                { // provide basic guarantee
                    helper.umove(this->_First, _Whereptr, _Newvec);
                    _Constructed_first = _Newvec;
                    helper.umove(_Whereptr, this->_Last, _Newvec + _Whereoff + 1);
                }
            )

            _Change_array(_Newvec, _Newsize, _Newcapacity);
            return (this->_First + _Whereoff);
        }

    public:
        // Move constructor
        exe_vector(exe_vector&& _Right)
        NH3API_NOEXCEPT
        // NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_move_constructible<allocator_type>::value)
            : helper(std::move(_Right.helper))
        {
            _Move_from(std::move(_Right), nh3api::tt::true_type());
        }

        exe_vector(exe_vector&& _Right, const allocator_type& _Al)
        NH3API_NOEXCEPT_EXPR(helper_type::is_always_equal::value)
            : helper(std::move(_Right.helper))
        {
            NH3API_IF_CONSTEXPR (helper_type::is_always_equal::value)
            {
                _Move_from(std::move(_Right), nh3api::tt::true_type());
            }
            else
            {
                NH3API_VECTOR_TRY_CATCH_TIDY
                (
                    noexcept_move::value,
                    _Move_from(std::move(_Right), nh3api::tt::false_type());
                )
            }
        }

        exe_vector(std::initializer_list<value_type> i)
            : helper()
        {
            _Range_construct_or_tidy(i.begin(), i.end(), std::random_access_iterator_tag());
        }

        exe_vector(std::initializer_list<value_type> i, const allocator_type& a)
            : helper(a)
        {
            _Range_construct_or_tidy(i.begin(), i.end(), std::random_access_iterator_tag());
        }

        exe_vector& operator=(exe_vector&& _X)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::allocator_always_equal_after_move<allocator_type>::value)
        {
            if (this != &_X)
            {
                if ( nh3api::tt::allocator_always_equal_after_move<allocator_type>::value
                    || this->get_allocator() == _X.get_allocator() )
                {
                    _Tidy();
                }
                this->helper = std::move(_X.helper);
                _Move_assign_from(std::move(_X), nh3api::tt::allocator_always_equal_after_move<allocator_type>());
            }
            return *this;
        }

        this_type& operator=(std::initializer_list<value_type> i)
        {
            _Assign_range(i.begin(), i.end(), std::random_access_iterator_tag());
            return *this;
        }

        template<class NH3API_ARGS_DOTS Args>
        iterator emplace( const_iterator _Where, Args&& NH3API_ARGS_DOTS args )
        {
            const pointer _Whereptr = const_cast<const pointer>(_Where);
            const pointer _Oldlast  = this->_Last;
        #if NH3API_DEBUG
            if (_Whereptr >= this->_First && _Oldlast >= _Whereptr)
                NH3API_THROW(std::out_of_range("vector::emplace iterator outside range"));
        #endif
            if (_Has_unused_capacity())
            {
                if (_Whereptr == _Oldlast)
                { // at back, provide strong guarantee
                    _Emplace_back_with_unused_capacity(std::forward<Args>(args) NH3API_ARGS_DOTS);
                }
                else
                {
                    value_type _Obj(std::forward<Args>(args) NH3API_ARGS_DOTS); // handle aliasing
                    // after constructing _Obj, provide basic guarantee
                    helper.construct(_Oldlast, std::move(_Oldlast[-1]));
                    ++this->_Last;
                    helper.move_backward(_Whereptr, _Oldlast - 1, _Oldlast);
                    *_Whereptr = std::move(_Obj);
                }

                return _Whereptr;
            }

            return _Emplace_reallocate(_Whereptr, std::forward<Args>(args) NH3API_ARGS_DOTS);
        }

        void push_back(value_type&& value)
        {
            emplace_back(value);
        }

        template<class NH3API_ARGS_DOTS Args>
        #if NH3API_CHECK_CPP17
        reference
        #else
        void
        #endif
        emplace_back( Args&& NH3API_ARGS_DOTS args )
        {
            if (_Has_unused_capacity())
            {
                return (_Emplace_back_with_unused_capacity(std::forward<Args>(args) NH3API_ARGS_DOTS));
            }

            reference _Result = *_Emplace_reallocate(this->_Last, std::forward<Args>(args) NH3API_ARGS_DOTS);
        #if NH3API_CHECK_CPP17
            return _Result;
        #else
            (void) _Result;
        #endif
        }

        iterator insert(const_iterator _Pos, value_type&& _Val)
        {	// insert _Val at _Pos
            return emplace(_Pos, std::move(_Val));
        }
#endif

    public:
        ~exe_vector()
        {
            _Tidy();
        }

        NH3API_INLINE_LARGE
        exe_vector& operator=(const exe_vector& _X)
        {
            if ( this == &_X )
            {
                return *this;
            }
            else
            {
                if (this->get_allocator() != _X.get_allocator()
                    && propagate_on_container_copy_assignment::value)
                {
                    _Tidy();
                }
                this->helper = _X.helper;
                assign(_X._First, _X._Last);
            }
            return *this;
        }

        NH3API_FLATTEN
        void reserve( size_type _Newcapacity )
        { // increase capacity to _Newcapacity (without geometric growth), provide strong guarantee
            if (_Newcapacity > capacity())
            { // something to do (reserve() never shrinks)
                if (_Newcapacity > max_size())
                    NH3API_THROW(std::length_error("vector::reserve: requested size too large."));

                _Reallocate_exactly(_Newcapacity);
            }
        }

        size_type capacity() const NH3API_NOEXCEPT
        { return this->_End - this->_First; }

        void shrink_to_fit()
        {
            if (_Has_unused_capacity())
            {
                if (empty())
                {
                    _Tidy();
                }
                else
                {
                    _Reallocate_exactly(size());
                }
            }
        }

        iterator begin() NH3API_NOEXCEPT
        { return _First; }

        const_iterator begin() const NH3API_NOEXCEPT
        { return (const_iterator)_First; }

        const_iterator cbegin() const NH3API_NOEXCEPT
        { return (const_iterator)_First; }

        iterator end() NH3API_NOEXCEPT
        { return _Last; }

        const_iterator end() const NH3API_NOEXCEPT
        { return (const_iterator)_Last; }

        const_iterator cend() const NH3API_NOEXCEPT
        { return (const_iterator)_Last; }

        reverse_iterator rbegin() NH3API_NOEXCEPT
        { return reverse_iterator(end()); }

        const_reverse_iterator rbegin() const NH3API_NOEXCEPT
        { return const_reverse_iterator(end()); }

        const_reverse_iterator crbegin() const NH3API_NOEXCEPT
        { return const_reverse_iterator(end()); }

        reverse_iterator rend() NH3API_NOEXCEPT
        { return reverse_iterator(begin()); }

        const_reverse_iterator rend() const NH3API_NOEXCEPT
        { return const_reverse_iterator(begin()); }

        const_reverse_iterator crend() const NH3API_NOEXCEPT
        { return const_reverse_iterator(begin()); }

        NH3API_INLINE_LARGE
        void resize(size_type _Newsize)
        {
            const size_type _Oldsize = size();
            const size_type _Oldcapacity = capacity();

            if (_Newsize > _Oldcapacity)
            { // reallocate
                if (_Newsize > max_size())
                {
                    NH3API_THROW(std::length_error("vector::resize: requested too much size."));
                }

                const size_type _Newcapacity = _Calculate_growth(_Newsize);
                const pointer _Newvec = helper.allocate(_Newcapacity);
                const pointer _Appended_first = _Newvec + _Oldsize;
                pointer _Appended_last = _Appended_first;

                NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
                (
                    nh3api::tt::is_nothrow_default_constructible<value_type>::value
                    && noexcept_copy::value && noexcept_move::value,
                    _Appended_first, _Appended_last, _Newvec, _Newcapacity,

                    _Appended_last = helper.default_fill(_Appended_first, _Newsize - _Oldsize);
                    _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                )

                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else if (_Newsize > _Oldsize)
            { // append
                const pointer _Oldlast = this->_Last;
                this->_Last = helper.default_fill(_Oldlast, _Newsize - _Oldsize);
            }
            else if (_Newsize == _Oldsize)
            { // nothing to do, avoid invalidating iterators
            }
            else
            { // trim
                const pointer _Newlast = this->_First + _Newsize;
                helper.deallocate(_Newlast, this->_Last);
                this->_Last = _Newlast;
            }
        }

        void resize(size_type _Newsize, const value_type& _Val)
        {
            // yeah.. repeat the same code again but this is just to avoid
            // usage of lambdas or functors and templates

            const size_type _Oldsize = size();
            const size_type _Oldcapacity = capacity();

            if (_Newsize > _Oldcapacity)
            { // reallocate
                if (_Newsize > max_size())
                {
                    NH3API_THROW(std::length_error("vector::resize: requested too much size."));
                }

                const size_type _Newcapacity = _Calculate_growth(_Newsize);
                const pointer _Newvec = helper.allocate(_Newcapacity);
                const pointer _Appended_first = _Newvec + _Oldsize;
                pointer _Appended_last = _Appended_first;

                NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
                (
                    nh3api::tt::is_nothrow_default_constructible<value_type>::value
                    && noexcept_copy::value && noexcept_move::value,
                    _Appended_first, _Appended_last, _Newvec, _Newcapacity,

                    _Appended_last = helper.ufill(_Appended_first, _Newsize - _Oldsize, _Val);
                    _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                )

                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else if (_Newsize > _Oldsize)
            { // append
                const pointer _Oldlast = this->_Last;
                this->_Last = helper.ufill(_Oldlast, _Newsize - _Oldsize, _Val);
            }
            else if (_Newsize == _Oldsize)
            { // nothing to do, avoid invalidating iterators
            }
            else
            { // trim
                const pointer _Newlast = this->_First + _Newsize;
                helper.deallocate(_Newlast, this->_Last);
                this->_Last = _Newlast;
            }
        }

        NH3API_FORCEINLINE
        size_type size() const NH3API_NOEXCEPT
        { return this->_Last - this->_First; }

        NH3API_FORCEINLINE
        size_type max_size() const NH3API_NOEXCEPT
        { return helper.max_size(); }

        NH3API_FORCEINLINE
        bool empty() const NH3API_NOEXCEPT
        { return size() == 0; }

        NH3API_FORCEINLINE
        allocator_type get_allocator() const NH3API_NOEXCEPT
        { return helper.alloc; }

        NH3API_FORCEINLINE
        pointer data() NH3API_NOEXCEPT
        { return _First; }

        NH3API_FORCEINLINE
        const_pointer data() const NH3API_NOEXCEPT
        { return _First; }

        const_reference at( size_type _P ) const
        {
            if ( size() <= _P )
                NH3API_THROW(std::out_of_range("vector::at(pos): invalid pos"));
            return *(_First + _P);
        }
        reference at( size_type _P )
        {
            if ( size() <= _P )
                NH3API_THROW(std::out_of_range("vector::at(pos): invalid pos"));
            return *(_First + _P);
        }
        NH3API_FORCEINLINE
        const_reference operator[]( size_type _P ) const
        #if !NH3API_DEBUG
        NH3API_NOEXCEPT
        #endif
        {
        #if !NH3API_DEBUG
            return *(_First + _P);
        #else
            return at(_P);
        #endif
        }
        NH3API_FORCEINLINE
        reference operator[]( size_type _P )
        #if !NH3API_DEBUG
        NH3API_NOEXCEPT
        #endif
        {
        #if !NH3API_DEBUG
            return *(_First + _P);
        #else
            return at(_P);
        #endif
        }
        NH3API_FORCEINLINE
        reference front() NH3API_NOEXCEPT
        {
            return (*begin());
        }
        NH3API_FORCEINLINE
        const_reference front() const NH3API_NOEXCEPT
        {
            return (*begin());
        }
        NH3API_FORCEINLINE
        reference back() NH3API_NOEXCEPT
        {
            return (*(end() - 1));
        }
        NH3API_FORCEINLINE
        const_reference back() const NH3API_NOEXCEPT
        {
            return (*(end() - 1));
        }
        NH3API_INLINE_LARGE
        void push_back(const value_type& value)
        {
            #if NH3API_STD_MOVE_SEMANTICS
            emplace_back(value);
            #else
            if (this->_Last == this->_End)
                reserve(1);
            helper.construct(this->_Last, value);
            ++this->_Last;
            #endif
        }

        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        void assign(IterT _F, IterT _L
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            nh3api::verify_range(_F, _L);
            _Assign_range(nh3api::unfancy(_F), nh3api::unfancy(_L), nh3api::iter_category<IterT>());
        }

        // assign _Newsize * _Val
        void assign(size_type _Newsize, const value_type &_Val)
        {
            const size_type _Oldsize = size();
            const size_type _Oldcapacity = capacity();

            if (_Newsize > _Oldcapacity)
            { // reallocate
                if (_Newsize > max_size())
                {
                    NH3API_THROW(std::length_error("vector::assign: size exceeded max_size"));
                }

                const size_type _Newcapacity = _Calculate_growth(_Newsize);

                if (this->_First != pointer())
                { // destroy and deallocate old array
                    helper.destroy_range(this->_First, this->_Last);
                    helper.deallocate(this->_First, _Oldcapacity);
                }

                _Buy(_Newcapacity);
                this->_Last = helper.ufill(this->_First, _Newsize, _Val);
            }
            else if (_Newsize > _Oldsize)
            {
                nh3api::fill(this->_First, this->_Last, _Val);
                this->_Last = helper.ufill(this->_Last, _Newsize - _Oldsize, _Val);
            }
            else
            {
                const pointer _Newlast = this->_First + _Newsize;
                nh3api::fill(this->_First, _Newlast, _Val);
                helper.destroy_range(_Newlast, this->_Last);
                this->_Last = _Newlast;
            }
        }

        #if NH3API_STD_MOVE_SEMANTICS
        void assign(std::initializer_list<value_type> i)
        {	// assign initializer_list
            _Assign_range(i.begin(), i.end(), std::random_access_iterator_tag());
        }
        #endif

        // TODO: Implement C++23 assign_range, append_range, insert_range

    protected:
        template<class IterT>
        void _Assign_range(IterT _F, IterT _L, std::input_iterator_tag )
        {	// assign [_F, _L), input iterators
            nh3api::verify_range(_F, _L);

            pointer _Next = this->_First;
            for (; _F != _L && _Next != this->_Last; ++_F, (void)++_Next)
            {
                *_Next = *_F;
            }

            helper.destroy_range(_Next, this->_Last);
            this->_Last = _Next;

            #if NH3API_STD_MOVE_SEMANTICS
            for (; _F != _L; ++_F)
                emplace_back(*_F);
            #else
            for (; _F != _L; ++_F)
                push_back(*_F);
            #endif
        }

        template<class IterT>
        void _Assign_range(IterT _F, IterT _L, std::forward_iterator_tag )
        { // assign [_F, _L), forward iterators
            nh3api::verify_range(_F, _L);
            if (_F == _L)
                return;

            const size_type _Newsize = static_cast<size_type>(static_cast<size_t>(std::distance(_F, _L)));
            const size_type _Oldsize = size();
            const size_type _Oldcapacity = capacity();

            if (_Newsize > _Oldcapacity)
            { // reallocate
                if (_Newsize > max_size())
                {
                    NH3API_THROW(std::length_error("vector::assign: size exceeded max_size"));
                }

                const size_type _Newcapacity = _Calculate_growth(_Newsize);

                if (this->_First != pointer())
                { // destroy and deallocate old array
                    helper.destroy(this->_First, this->_Last);
                    helper.deallocate(this->_First, _Oldcapacity);
                }

                _Buy(_Newcapacity);

                this->_Last = helper.ucopy(_F, _L, this->_First);
            }
            else if (_Newsize > _Oldsize)
            {
                // performance note: traversing [_F, _Mid) twice
                const IterT _Mid = nh3api::next(_F, static_cast<difference_type>(_Oldsize));
                nh3api::copy(_F, _Mid, this->_First);
                this->_Last = helper.ucopy(_Mid, _L, this->_Last);
            }
            else
            {
                const pointer _Newlast = this->_First + _Newsize;
                nh3api::copy(_F, _L, this->_First);
                helper.destroy_range(_Newlast, this->_Last);
                this->_Last = _Newlast;
            }
        }

    public:
        iterator insert(const_iterator _Where, const value_type& _Val)
        {
            #if NH3API_STD_MOVE_SEMANTICS
            return emplace(_Where, _Val);
            #else
            return insert(_Where, static_cast<size_type>(1), _Val);
            #endif
        }
        iterator insert(const_iterator _Where, size_type _Count, const value_type& _Val)
        {
            const size_type _Whereoff = static_cast<size_type>(_Where - this->_First);
            const bool _One_at_back = _Count == 1 && _Where == this->_Last;

            if (_Count == 0)
            { // nothing to do, avoid invalidating iterators
            }
            else if (_Count > _Unused_capacity())
            { // reallocate
                const size_type _Oldsize = size();

                if (_Count > max_size() - _Oldsize)
                {
                    NH3API_THROW(std::length_error("vector::insert: size exceeded max_size"));
                }

                const size_type _Newsize = _Oldsize + _Count;
                const size_type _Newcapacity = _Calculate_growth(_Newsize);

                const pointer _Newvec = helper.allocate(_Newcapacity);
                const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
                pointer _Constructed_first = _Constructed_last;

                NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
                (
                    noexcept_copy::value && noexcept_move::value,
                    _Constructed_first, _Constructed_last, _Newvec, _Newcapacity,
                    helper.ufill(_Newvec + _Whereoff, _Count, _Val);
                    _Constructed_first = _Newvec + _Whereoff;

                    if (_One_at_back)
                    { // provide strong guarantee
                        _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                    }
                    else
                    { // provide basic guarantee
                        helper.umove(this->_First, _Where, _Newvec);
                        _Constructed_first = _Newvec;
                        helper.umove(_Where, this->_Last, _Newvec + _Whereoff + _Count);
                    }
                )
                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else if (_One_at_back)
            { // provide strong guarantee
                #if NH3API_STD_MOVE_SEMANTICS
                _Emplace_back_with_unused_capacity(_Val);
                #else
                push_back(_Val);
                #endif
            }
            else
            {                          // provide basic guarantee
                const _Ty _Tmp = _Val; // handle aliasing
                const pointer _Oldlast = this->_Last;
                const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where);

                if (_Count > _Affected_elements)
                { // new stuff spills off end
                    this->_Last = helper.ufill(_Oldlast, _Count - _Affected_elements, _Tmp);
                    this->_Last = helper.umove(_Where, _Oldlast, this->_Last);
                    nh3api::fill(_Where, _Oldlast, _Tmp);
                }
                else
                { // new stuff can all be assigned
                    this->_Last = helper.umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                    helper.move_backward(_Where, _Oldlast - _Count, _Oldlast);
                    nh3api::fill(_Where, _Where + _Count, _Tmp);
                }
            }
            return _Make_iterator_offset(_Whereoff);
        }

        template<class IterT>
        typename nh3api::tt::enable_if<nh3api::tt::is_iterator<IterT>::value, iterator>::type
        insert( const_iterator _Where, IterT _F, IterT _L )
        {
            nh3api::verify_range(_F, _L);
            const size_type _Whereoff = static_cast<size_type>(_Where - this->_First);
            _Insert_range(_Where, nh3api::unfancy(_F), nh3api::unfancy(_L), nh3api::iter_category<IterT>());
            return _Make_iterator_offset(_Whereoff);
        }

        #if NH3API_STD_MOVE_SEMANTICS
        iterator insert(const_iterator _Where, std::initializer_list<value_type> i)
        {
            return insert(_Where, i.begin(), i.end());
        }
        #endif

        #if NH3API_DEBUG
        void pop_back()
        {	// erase element at end
            if (empty())
                NH3API_THROW(std::logic_error("vector::pop_back(): vector was empty"));
            else
            {	// erase last element
                helper.destroy(this->_Last - 1);
                --this->_Last;
            }
        }

        #else // NH3API_DEBUG
        void pop_back()
        {	// erase element at end
            if (!empty())
            {	// erase last element
                helper.destroy(this->_Last - 1);
                --this->_Last;
            }
        }
        #endif // NH3API_DEBUG

        iterator erase(const_iterator _P)
        {
        #if NH3API_DEBUG
            if (!(_P < this->_Last && this->_First <= _P))
                NH3API_THROW(std::out_of_range("vector::erase: iterator outside bounds"));
        #endif // NH3API_DEBUG

        #if NH3API_STD_MOVE_SEMANTICS
            NH3API_IF_CONSTEXPR ( nh3api::tt::is_move_assignable<value_type>::value )
                std::move(_P + 1, this->_Last, _P);
            else
                nh3api::copy(_P + 1, this->_Last, _P);
        #else
                nh3api::copy(_P + 1, this->_Last, _P);
        #endif
            helper.destroy_range(this->_Last - 1, this->_Last);
            --this->_Last;
            return _P;
        }

        iterator erase( const_iterator _F, const_iterator _L )
        {
            #if NH3API_DEBUG
            if (_F < _First || _L > _Last)
                NH3API_THROW(std::out_of_range("vector::erase: iterator outside bounds"));
            #endif // NH3API_DEBUG

            iterator _It1 = const_cast<iterator>(_F);
            iterator _It2 = const_cast<iterator>(_L);
            pointer _Ptr;
            if (_It1 != _It2)
            { // worth doing, copy down over hole
            #if NH3API_STD_MOVE_SEMANTICS
                NH3API_IF_CONSTEXPR ( nh3api::tt::is_move_assignable<value_type>::value )
                    _Ptr = std::move(_It2, this->_Last, _It1);
                else
                    _Ptr = nh3api::copy(_It2, this->_Last, _It1);
            #else
                    _Ptr = nh3api::copy(_It2, this->_Last, _It1);
            #endif
                helper.destroy_range(_Ptr, this->_Last);
                this->_Last = _Ptr;
            }
            return _It1;

        }

        void clear() NH3API_NOEXCEPT
        {
            helper.destroy_range(_First, _Last);
            this->_Last = this->_First;
        }

        void swap( this_type& _X )
        NH3API_NOEXCEPT_EXPR(
            propagate_on_container_swap::value
            || helper_type::is_always_equal::value)
        {
            if (this == &_X)
                return;
            if ( helper.alloc == _X.helper.alloc )
            {
                std::swap(this->_First, _X._First);
                std::swap(this->_Last, _X._Last);
                std::swap(this->_End, _X._End);
            }
            else if (propagate_on_container_swap::value)
            {
                std::swap(this->helper.alloc, _X.helper.alloc);
                std::swap(this->_First, _X._First);
                std::swap(this->_Last, _X._Last);
                std::swap(this->_End, _X._End);
            }
            else
            {
                return;
            }
        }

    protected:
        // set capacity to _Newcapacity (without geometric growth), provide strong guarantee
        void _Reallocate_exactly(const size_type _Newcapacity)
        {
            const size_type _Size = size();
            const pointer _Newvec = helper.allocate(_Newcapacity);

            NH3API_IF_CONSTEXPR ( noexcept_copy::value && noexcept_move::value )
            {
                _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
            }
            else
            {
                NH3API_TRY
                {
                    _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                }
                NH3API_CATCH(...)
                {
                    helper.deallocate(_Newvec, _Newcapacity);
                    NH3API_RETHROW
                }
            }

            _Change_array(_Newvec, _Size, _Newcapacity);
        }

        iterator _Make_iterator_offset(const size_type _Offset) NH3API_NOEXCEPT
        {	// return the iterator begin() + _Offset
            return iterator(_First + _Offset);
        }

        // micro-optimization for capacity() - size()
        size_type _Unused_capacity() const NH3API_NOEXCEPT
        { return static_cast<size_type>(this->_End - this->_Last); }

        // micro-optimization for capacity() != size()
        bool _Has_unused_capacity() const NH3API_NOEXCEPT
        { return this->_End != this->_Last; }

        // move [_F, _L) to raw _Dest, using allocator
        void _Umove_if_noexcept(pointer _F, pointer _L, pointer _Dest)
        {
            #if NH3API_STD_MOVE_SEMANTICS
            _Umove_if_noexcept_impl(_F, _L, _Dest,
            nh3api::tt::disjunction_2<
            nh3api::tt::is_nothrow_move_constructible<value_type>,
            nh3api::tt::negation<nh3api::tt::is_copy_constructible<value_type>>
            >());
            #else
            helper.ucopy(_F, _L, _Dest);
            #endif
        }

        // move [_F, _L) to raw _Dest, using allocator
        void _Umove_if_noexcept_impl(pointer _F, pointer _L, pointer _Dest, nh3api::tt::true_type)
        {
            helper.umove(_F, _L, _Dest);
        }

        // copy [_F, _L) to raw _Dest, using allocator
        void _Umove_if_noexcept_impl(pointer _F, pointer _L, pointer _Dest, nh3api::tt::false_type)
        {
            helper.ucopy(_F, _L, _Dest);
        }

        // allocate array with _Capacity elements
        bool _Buy(size_type _Capacity)
        {
            this->_First = nullptr;
            this->_Last  = nullptr;
            this->_End   = nullptr;

            if (_Capacity == 0)
                return false;
            else if (max_size() < _Capacity)
                NH3API_THROW(std::invalid_argument("vector::reserve: invalid requested size."));
            else
            {	// nonempty array, allocate storage
                this->_First = helper.allocate(_Capacity);
                this->_Last  = this->_First;
                this->_End   = this->_First + _Capacity;
            }
            return true;
        }

        // given _Oldcapacity and _Newsize, calculate geometric growth
        size_type _Calculate_growth(const size_type _Newsize) const
        {
            const size_type _Oldcapacity = capacity();
            if (_Oldcapacity > max_size() - _Oldcapacity / 2)
                return _Newsize;	// geometric growth would overflow

            const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;
            if (_Geometric < _Newsize)
                return _Newsize;	// geometric growth would be insufficient
            return _Geometric;	// geometric growth is sufficient
        }

        // discard old array, acquire new array
        void _Change_array(const pointer _Newvec, const size_type _Newsize, const size_type _Newcapacity)
        {
            _Tidy_deallocate();

            this->_First = _Newvec;
            this->_Last = _Newvec + _Newsize;
            this->_End = _Newvec + _Newcapacity;
        }

        // free all storage
        void _Tidy()
        {
            _Tidy_deallocate();
            _Nullify();
        }

        void _Tidy_deallocate() NH3API_NOEXCEPT
        {
            if (this->_First != nullptr)
            {	// something to free, destroy and deallocate it
                helper.destroy_range(this->_First, this->_Last);
                helper.deallocate(this->_First, capacity());
            }
        }

        NH3API_FORCEINLINE
        void _Nullify() NH3API_NOEXCEPT
        {
            if ( nh3api::tt::is_empty<allocator_type>::value )
            {
                nh3api::zero16(this);
            }
            else
            {
                _First = nullptr;
                _Last  = nullptr;
                _End   = nullptr;
            }
        }

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_FORCEINLINE
        void _Move_nullify(exe_vector* other) NH3API_NOEXCEPT
        {
            // clang is the only compiler that can optimize 4 std::exchange-s to a single SSE2 swap
            #ifdef __clang__
            this->helper = std::move(other->helper);
            this->_First = nh3api::exchange(other->_First, nullptr);
            this->_Last  = nh3api::exchange(other->_Last, nullptr);
            this->_End   = nh3api::exchange(other->_End, nullptr);
            #else
            if ( nh3api::tt::is_empty<allocator_type>::value )
            {
                nh3api::move16(this, other);
            }
            else
            {
                this->helper = std::move(other->helper);
                this->_First = nh3api::exchange(other->_First, nullptr);
                this->_Last  = nh3api::exchange(other->_Last, nullptr);
                this->_End   = nh3api::exchange(other->_End, nullptr);
            }
            #endif
        }
        #endif

        // insert [_F, _L) at _Where, input iterators
        template<class IterT>
        void _Insert_range(const_iterator _Where, IterT _F, IterT _L, std::input_iterator_tag)
        {
            if (_F == _L)
            {
                return;	// nothing to do, avoid invalidating iterators
            }

            const size_type _Whereoff = static_cast<size_type>(_Where - this->_First);
            const size_type _Oldsize = size();

#if NH3API_STD_MOVE_SEMANTICS
            for (; _F != _L; ++_F)
                emplace_back(*_F);
#else
            for (; _F != _L; ++_F)
                push_back(*_F);
#endif

            std::rotate(this->_First + _Whereoff, this->_First + _Oldsize, this->_Last);
        }

        template<class IterT>
        void _Insert_range(const_iterator _Where, IterT _F, IterT _L, std::forward_iterator_tag)
        {
            const size_type _Count = static_cast<size_type>(static_cast<size_t>(std::distance(_F, _L)));
            const size_type _Whereoff = static_cast<size_type>(_Where - this->_Last);
            const bool _One_at_back = _Count == 1 && _Where == this->_Last;

            if (_Count == 0)
            { // nothing to do, avoid invalidating iterators
            }
            else if (_Count > _Unused_capacity())
            { // reallocate
                const size_type _Oldsize = size();

                if (_Count > max_size() - _Oldsize)
                {
                    NH3API_THROW(std::length_error("vector::insert: size exceeded max_size"));
                }

                const size_type _Newsize = _Oldsize + _Count;
                const size_type _Newcapacity = _Calculate_growth(_Newsize);

                const pointer _Newvec = helper.allocate(_Newcapacity);
                const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
                pointer _Constructed_first = _Constructed_last;

                NH3API_VECTOR_TRY_CATCH_DESTROY_DEALLOCATE
                (
                    noexcept_copy::value && noexcept_move::value,
                    _Constructed_first, _Constructed_last, _Newvec, _Newcapacity,

                    helper.ucopy(_F, _L, _Newvec + _Whereoff);
                    _Constructed_first = _Newvec + _Whereoff;
                    if (_One_at_back)
                    { // provide strong guarantee
                        _Umove_if_noexcept(this->_Last, this->_Last, _Newvec);
                    }
                    else
                    { // provide basic guarantee
                        helper.umove(this->_Last, _Where, _Newvec);
                        _Constructed_first = _Newvec;
                        helper.umove(_Where, this->_Last, _Newvec + _Whereoff + _Count);
                    }
                )
                _Change_array(_Newvec, _Newsize, _Newcapacity);
            }
            else
            { // Attempt to provide the strong guarantee for EmplaceConstructible failure.
              // If we encounter copy/move construction/assignment failure, provide the basic guarantee.
              // (For one-at-back, this provides the strong guarantee.)

                const pointer _Oldlast = this->_Last;
                const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where);

                if (_Count < _Affected_elements)
                { // some affected elements must be assigned
                    this->_Last = helper.umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                    helper.move_backward(_Where, _Oldlast - _Count, _Oldlast);
                    helper.destroy_range(_Where, _Where + _Count);

                    NH3API_IF_CONSTEXPR ( noexcept_copy::value )
                    {
                        helper.ucopy(_F, _L, _Where);
                    }
                    else
                    {
                        NH3API_TRY
                        {
                            helper.ucopy(_F, _L, _Where);
                        }
                        NH3API_CATCH(...)
                        {
                            NH3API_TRY
                            {
                                helper.umove(_Where + _Count, _Where + 2 * _Count, _Where);
                            }
                            NH3API_CATCH(...)
                            {
                                // vaporize the detached piece
                                helper.destroy_range(_Where + _Count, this->_Last);
                                this->_Last = _Where;
                                NH3API_RETHROW
                            }
                            std::move(_Where + 2 * _Count, this->_Last, _Where + _Count);
                            helper.destroy_range(_Oldlast, this->_Last);
                            this->_Last = _Oldlast;
                            NH3API_RETHROW
                        }
                    }
                }
                else
                { // affected elements don't overlap before/after
                    const pointer _Relocated = _Where + _Count;
                    this->_Last = helper.umove(_Where, _Oldlast, _Relocated);
                    helper.destroy_range(_Where, _Oldlast);
                    NH3API_IF_CONSTEXPR ( noexcept_copy::value )
                    {
                        helper.ucopy(_F, _L, _Where);
                    }
                    else
                    {
                        NH3API_TRY
                        {
                            helper.ucopy(_F, _L, _Where);
                        }
                        NH3API_CATCH(...)
                        {
                            NH3API_TRY
                            {
                                helper.umove(_Relocated, this->_Last, _Where);
                            }
                            NH3API_CATCH(...)
                            {
                                // vaporize the detached piece
                                helper.destroy_range(_Relocated, this->_Last);
                                this->_Last = _Where;
                                NH3API_RETHROW
                            }
                            helper.destroy_range(_Relocated, this->_Last);
                            this->_Last = _Oldlast;
                            NH3API_RETHROW
                        }
                    }
                }
            }
        }

    protected:
        helper_type helper; // helper (contains allocator)
        pointer _First;	// pointer to beginning of array
        pointer _Last;	// pointer to current end of sequence
        pointer _End;	// pointer to end of array
};
#pragma pack(pop)

//}; // namespace nh3api

namespace std
{

#if !NH3API_STD_MOVE_SEMANTICS
template< class T, class Alloc >
NH3API_FORCEINLINE
void swap( exe_vector<T, Alloc>& lhs,
           exe_vector<T, Alloc>& rhs )
         NH3API_NOEXCEPT_EXPR(lhs.swap(rhs))
{
    lhs.swap(rhs);
}
#endif

#ifdef __cpp_lib_erase_if
template<class T, class Alloc, class U>
NH3API_INLINE_LARGE
typename exe_vector<T, Alloc>::size_type
erase(exe_vector<T, Alloc>& vec, const U& value)
{
    typename exe_vector<T, Alloc>::iterator it = std::remove(vec.begin(), vec.end(), value);
    typename exe_vector<T, Alloc>::size_type r = vec.end() - it;
    vec.erase(it, vec.end());
    return r;
}

template<class T, class Alloc, class Pred>
NH3API_INLINE_LARGE
typename exe_vector<T, Alloc>::size_type
erase_if( exe_vector<T, Alloc>& vec, Pred pred )
{
    typename exe_vector<T, Alloc>::iterator it = std::remove_if(vec.begin(), vec.end(), pred);
    typename exe_vector<T, Alloc>::size_type r = vec.end() - it;
    vec.erase(it, vec.end());
    return r;
}
#endif

} // namespace std

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator==(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test for vector equality
    return (_Left.size() == _Right.size()
        && std::equal(_Left.begin(), _Left.end(), _Right.begin()));
}

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator!=(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test for vector inequality
    return (!(_Left == _Right));
}

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator<(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test if _Left < _Right for vectors
    return (std::lexicographical_compare(_Left.begin(), _Left.end(),
        _Right.begin(), _Right.end()));
}

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator>(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test if _Left > _Right for vectors
    return (_Right < _Left);
}

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator<=(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test if _Left <= _Right for vectors
    return (!(_Right < _Left));
}

template<class T,
    class _Alloc> NH3API_FORCEINLINE
    bool operator>=(const exe_vector<T, _Alloc>& _Left,
        const exe_vector<T, _Alloc>& _Right)
{	// test if _Left >= _Right for vectors
    return (!(_Left < _Right));
}

NH3API_DISABLE_WARNING_END
