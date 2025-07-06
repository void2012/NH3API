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


#include "nh3api_std.hpp"
#include "memory.hpp"

// Three major STL implementations(MSVC STL, libc++, libstdc++) 
// Implement std::vector<T> in the same layout as exe_vector, so we can use the native 
// std::vector<T> implementation. Note that the sizeof(std::vector<T>) must be = 12
// So for example in Visual Studio, you should disable iterator debugging.
#ifdef NH3API_FLAG_USE_NATIVE_STD_VECTOR
#include <vector>

namespace nh3api 
{
struct nonempty_base   
{ 
    uint32_t dummy 
    #if NH3API_CHECK_CPP11
    = 0 
    #endif
    ;
};
}

template<typename T>
class exe_vector : public nh3api::nonempty_base, public std::vector<T, allocator_type >
{
    public:
    #if NH3API_CHECK_CPP11
        using std::vector<T, allocator_type >::vector;

        exe_vector(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {}
    #else
    // TODO: inherit constructors the old way.
    #endif
};

// Disable std::vector<bool> specialization
template<>
class exe_vector<bool> : public nh3api::nonempty_base, public std::vector<uint8_t, exe_allocator<uint8_t> >
{
    public:
    #if NH3API_CHECK_CPP11
        using std::vector<uint8_t, exe_allocator<uint8_t> >::vector;

        exe_vector(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {}
    #else
    // TODO: inherit constructors the old way.
    #endif
};

NH3API_STATIC_ASSERT(
    "sizeof(std::vector<T>) should be 12. Disable debugging iterators if you're on MSVC.",
    sizeof(exe_vector<int>) == 16);
#else 
#include "algorithm.hpp"
#include "iterator.hpp" // nh3api::is_iterator
#include "type_traits/is_xyz.hpp"
#include "nh3api_exceptions.hpp" // std::length_error, std::out_of_range, std::runtime_error

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)
NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// Disable warning for std::uninitialized_xyz on old MSVC STL
#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
NH3API_DISABLE_MSVC_WARNING_BEGIN(4996)
#endif

//namespace nh3api
//{
#pragma pack(push, 4)

// Visual C++ 6.0 std::vector implementation used by heroes3.exe
template<class T>
class exe_vector
{
protected:
    NH3API_STATIC_ASSERT("The C++ Standard forbids containers of non-object types "
                            "because of [container.requirements].",
                            nh3api::tt::is_object<T>::value);

// internal typedefs
protected:
    typedef
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    nh3api::tt::is_nothrow_copy_constructible<T>
    #else
    nh3api::tt::true_type
    #endif
    noexcept_copy;

    typedef
    #if NH3API_STD_MOVE_SEMANTICS && !defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
    nh3api::tt::is_nothrow_move_constructible<T>
    #else
    nh3api::tt::true_type
    #endif
    noexcept_move;

    typedef 
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    nh3api::tt::is_nothrow_default_constructible<T> 
    #else 
    nh3api::tt::true_type
    #endif 
    noexcept_default_construct;

// external typedefs
public:
    typedef exe_allocator<T>  allocator_type;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;

protected:
    #ifndef NH3API_MAKE_EXCEPTION_GUARD
    #define NH3API_MAKE_EXCEPTION_GUARD(NO_UNWIND_CONDITION, FUNCTOR, ...) \
    typename nh3api::exception_guard_factory<NO_UNWIND_CONDITION, FUNCTOR>::type \
    guard = nh3api::make_exception_guard<NO_UNWIND_CONDITION>(FUNCTOR(__VA_ARGS__))
    #endif 
    
    template<class IterT>
    void _Range_construct_or_tidy(IterT first, IterT last, std::input_iterator_tag)
    {
        nh3api::verify_range(first, last);
    #if NH3API_STD_MOVE_SEMANTICS
        NH3API_MAKE_EXCEPTION_GUARD(noexcept_move::value, vector_cleanup, *this);
        for (; first != last; ++first)
            emplace_back(*first);
        guard.complete();
    #else
        NH3API_MAKE_EXCEPTION_GUARD(noexcept_copy::value, vector_cleanup, *this);
        for (; first != last; ++first)
            push_back(*first);
        guard.complete();
    #endif
    }

    template<class IterT>
    void _Range_construct_or_tidy(IterT first, IterT last, std::forward_iterator_tag)
    {
        nh3api::verify_range(first, last);
        if ( _Buy( static_cast<size_type>(std::distance(first, last))) )
        {
            NH3API_MAKE_EXCEPTION_GUARD(noexcept_copy::value, vector_cleanup, *this);
            this->_Last = std::uninitialized_copy(first, last, this->_First);
            guard.complete();
        }
    }
public:
    exe_vector() NH3API_NOEXCEPT
        : _Dummy(0), _First(nullptr), _Last(nullptr), _End(nullptr)
    {}

    explicit exe_vector(const allocator_type&) NH3API_NOEXCEPT
        : _Dummy(0), _First(nullptr), _Last(nullptr), _End(nullptr)
    {}

    explicit exe_vector(size_type n)
        : _Dummy(0)
    {
        if (_Buy(n))
        { // nonzero, fill it
            NH3API_MAKE_EXCEPTION_GUARD(noexcept_default_construct::value, vector_cleanup, *this);
            this->_Last = _Default_fill(this->_First, n);
            guard.complete();
        }
    }

#if NH3API_CHECK_CPP14

    explicit exe_vector(size_type n, const allocator_type&)
        : _Dummy(0)
    {
        if (_Buy(n))
        { // nonzero, fill it
            NH3API_MAKE_EXCEPTION_GUARD(noexcept_default_construct::value, vector_cleanup, *this);
            this->_Last = _Default_fill(this->_First, n);
            guard.complete();
        }
    }

#endif

    exe_vector(size_type n, const value_type& value, const allocator_type& = allocator_type())
        : _Dummy(0)
    {
        if (_Buy(n))
        {	// nonzero, fill it
            NH3API_MAKE_EXCEPTION_GUARD(noexcept_copy::value, vector_cleanup, *this);
            this->_Last = _Ufill(this->_First, n, value);
            guard.complete();
        }
    }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    exe_vector( IterT first, IterT last, const allocator_type& = allocator_type()
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        : _Dummy(0)
    {
        nh3api::verify_range(first, last);
        _Range_construct_or_tidy(nh3api::unfancy(first),
                                    nh3api::unfancy(last),
                                    nh3api::iter_category<IterT>());
    }

    exe_vector(const exe_vector& other)
        : _Dummy(0)
    {
        if (_Buy(other.size()))
        {
            NH3API_MAKE_EXCEPTION_GUARD(noexcept_copy::value, vector_cleanup, *this);
            this->_Last = std::uninitialized_copy(other.begin(), other.end(), this->_First);
            guard.complete();
        }
    }

    // no-op constructor optimization for low-level code
    exe_vector(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
    { NH3API_IGNORE(_First, _Last, _End, _Dummy); }

#if NH3API_STD_MOVE_SEMANTICS
protected:
    template<class ... Args>
    #if NH3API_CHECK_CPP17
    reference
    #else
    void
    #endif
    _Emplace_back_with_unused_capacity(Args&& ... args)
    {
        nh3api::construct_at(_Last, std::forward<Args>(args) ...);
        #if NH3API_CHECK_CPP17
        reference result = *_Last;
        ++_Last;
        return result;
        #else
        ++_Last;
        #endif
    }

    template<class ... Args>
    pointer _Emplace_reallocate(const pointer _Whereptr, Args&& ... _Val)
    { // reallocate and insert by perfectly forwarding _Val at _Whereptr
        // pre: !_Has_unused_capacity()
        const size_type _Whereoff = static_cast<size_type>(_Whereptr - this->_First);
        const size_type _Oldsize  = size();

        if (_Oldsize == max_size())
        {
            NH3API_THROW(std::length_error, "vector::emplace: size exceeded max_size");
        }

        const size_type _Newsize = _Oldsize + 1;
        const size_type _Newcapacity = _Calculate_growth(_Newsize);

        pointer _Newvec = _Allocate(_Newcapacity);
        pointer _Constructed_last = _Newvec + _Whereoff + 1;
        pointer _Constructed_first = _Constructed_last;

        { // scope for exception guard
        auto guard = nh3api::make_exception_guard
        <nh3api::tt::is_nothrow_constructible<value_type, Args ...>::value
        && noexcept_move::value
        && noexcept_copy::value>(range_cleanup(*this, _Constructed_first, _Constructed_last, _Newvec, _Newcapacity));

        nh3api::construct_at(_Newvec + _Whereoff, std::forward<Args>(_Val)...);
        _Constructed_first = _Newvec + _Whereoff;
        if (_Whereptr == this->_Last)
        { // at back, provide strong guarantee
            _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
        }
        else
        { // provide basic guarantee
            _Umove(this->_First, _Whereptr, _Newvec);
            _Constructed_first = _Newvec;
            _Umove(_Whereptr, this->_Last, _Newvec + _Whereoff + 1);
        }

        guard.complete(); 
        } // scope for exception guard

        _Change_array(_Newvec, _Newsize, _Newcapacity);
        return (this->_First + _Whereoff);
    }

public:
    // Move constructor
    exe_vector(exe_vector&& other)
    NH3API_NOEXCEPT
    { nh3api::trivial_move<sizeof(exe_vector)>(&other, this); }

    exe_vector(exe_vector&& other, const allocator_type&)
    { nh3api::trivial_move<sizeof(exe_vector)>(&other, this); }

    exe_vector(std::initializer_list<value_type> i)
    {
        _Range_construct_or_tidy(i.begin(), i.end(), std::random_access_iterator_tag());
    }

    exe_vector(std::initializer_list<value_type> i, const allocator_type& a)
    {
        _Range_construct_or_tidy(i.begin(), i.end(), std::random_access_iterator_tag());
    }

    exe_vector& operator=(exe_vector&& other) NH3API_NOEXCEPT
    {
        if (this != &other)
        {
            _Tidy();
            nh3api::trivial_move<sizeof(exe_vector)>(&other, this);
        }
        return *this;
    }

    exe_vector& operator=(std::initializer_list<value_type> i)
    {
        _Assign_range(i.begin(), i.end(), std::random_access_iterator_tag());
        return *this;
    }

    template<class ... Args>
    iterator emplace( const_iterator _Where, Args&& ... args )
    {
        pointer _Whereptr = const_cast<const_pointer>(_Where);
        pointer _Oldlast  = this->_Last;
    #if NH3API_DEBUG
        if (_Whereptr >= this->_First && _Oldlast >= _Whereptr)
            NH3API_THROW(std::out_of_range, "vector::emplace iterator outside range");
    #endif
        if (_Has_unused_capacity())
        {
            if (_Whereptr == _Oldlast)
            { // at back, provide strong guarantee
                _Emplace_back_with_unused_capacity(std::forward<Args>(args) ...);
            }
            else
            {
                value_type _Obj(std::forward<Args>(args) ...); // handle aliasing
                // after constructing _Obj, provide basic guarantee
                nh3api::construct_at(_Oldlast, std::move(_Oldlast[-1]));
                ++this->_Last;
                _Move_backward(_Whereptr, _Oldlast - 1, _Oldlast);
                *_Whereptr = std::move(_Obj);
            }

            return _Whereptr;
        }

        return _Emplace_reallocate(_Whereptr, std::forward<Args>(args) ...);
    }

    void push_back(value_type&& value)
    {
        emplace_back(value);
    }

    template<class ... Args>
    #if NH3API_CHECK_CPP17
    reference
    #else
    void
    #endif
    emplace_back( Args&& ... args )
    {
        if (_Has_unused_capacity())
        {
            return (_Emplace_back_with_unused_capacity(std::forward<Args>(args) ...));
        }

        reference _Result = *_Emplace_reallocate(this->_Last, std::forward<Args>(args) ...);
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
#endif // move semantics

public:
    ~exe_vector()
    {
        _Tidy();
    }

    NH3API_INLINE_LARGE
    exe_vector& operator=(const exe_vector& _X)
    {
        if ( this == &_X )
            return *this;
        else
            assign(_X._First, _X._Last);
        
        return *this;
    }

    NH3API_FLATTEN
    void reserve( size_type _Newcapacity )
    { // increase capacity to _Newcapacity (without geometric growth), provide strong guarantee
        if (_Newcapacity > capacity())
        { // something to do (reserve() never shrinks)
            if (_Newcapacity > max_size())
                NH3API_THROW(std::length_error, "vector::reserve: requested size too large.");

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

public:
    iterator begin() NH3API_NOEXCEPT
    { return (_First); }

    const_iterator begin() const NH3API_NOEXCEPT
    { return (const_iterator)(_First); }

    const_iterator cbegin() const NH3API_NOEXCEPT
    { return (const_iterator)(_First); }

    iterator end() NH3API_NOEXCEPT
    { return (_Last); }

    const_iterator end() const NH3API_NOEXCEPT
    { return (const_iterator)(_Last); }

    const_iterator cend() const NH3API_NOEXCEPT
    { return (const_iterator)(_Last); }

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
                NH3API_THROW(std::length_error, "vector::resize: requested too much size.");
            }

            const size_type _Newcapacity = _Calculate_growth(_Newsize);
            pointer _Newvec = _Allocate(_Newcapacity);
            pointer _Appended_first = _Newvec + _Oldsize;
            pointer _Appended_last = _Appended_first;
            { // scope for exception guard
                NH3API_MAKE_EXCEPTION_GUARD(
                noexcept_default_construct::value 
                && noexcept_copy::value 
                && noexcept_move::value, 
                range_cleanup, *this, _Appended_first, _Appended_last, _Newvec, _Newcapacity);
                
                _Appended_last = _Default_fill(_Appended_first, _Newsize - _Oldsize);
                _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                
                guard.complete();
            } // scope for exception guard
            _Change_array(_Newvec, _Newsize, _Newcapacity);
        }
        else if (_Newsize > _Oldsize)
        { // append
            const pointer _Oldlast = this->_Last;
            this->_Last = _Default_fill(_Oldlast, _Newsize - _Oldsize);
        }
        else if (_Newsize == _Oldsize)
        { // nothing to do, avoid invalidating iterators
        }
        else
        { // trim
            const pointer _Newlast = this->_First + _Newsize;
            _Deallocate(_Newlast);
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
                NH3API_THROW(std::length_error, "vector::resize: requested too much size.");
            }

            const size_type _Newcapacity = _Calculate_growth(_Newsize);
            pointer _Newvec = _Allocate(_Newcapacity);
            pointer _Appended_first = _Newvec + _Oldsize;
            pointer _Appended_last = _Appended_first;

            { // scope for exception guard
                NH3API_MAKE_EXCEPTION_GUARD(
                noexcept_default_construct::value 
                && noexcept_copy::value 
                && noexcept_move::value, 
                range_cleanup, *this, _Appended_first, _Appended_last, _Newvec, _Newcapacity);
                
                _Appended_last = _Ufill(_Appended_first, _Newsize - _Oldsize, _Val);
                _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                
                guard.complete();
            } // scope for exception guard

            _Change_array(_Newvec, _Newsize, _Newcapacity);
        }
        else if (_Newsize > _Oldsize)
        { // append
            const pointer _Oldlast = this->_Last;
            this->_Last = _Ufill(_Oldlast, _Newsize - _Oldsize, _Val);
        }
        else if (_Newsize == _Oldsize)
        { // nothing to do, avoid invalidating iterators
        }
        else
        { // trim
            const pointer _Newlast = this->_First + _Newsize;
            _Deallocate(_Newlast);
            this->_Last = _Newlast;
        }
    }

    NH3API_FORCEINLINE
    size_type size() const NH3API_NOEXCEPT
    { return this->_Last - this->_First; }

    NH3API_NODISCARD NH3API_FORCEINLINE NH3API_CONSTEXPR
    size_type max_size() const NH3API_NOEXCEPT
    { return NH3API_MAX_HEAP_REQUEST / sizeof(value_type); }

    NH3API_FORCEINLINE
    bool empty() const NH3API_NOEXCEPT
    { return size() == 0; }

    NH3API_FORCEINLINE
    allocator_type get_allocator() const NH3API_NOEXCEPT
    { return allocator_type(); }

    NH3API_FORCEINLINE
    pointer data() NH3API_NOEXCEPT
    { return _First; }

    NH3API_FORCEINLINE
    const_pointer data() const NH3API_NOEXCEPT
    { return _First; }

    const_reference at( size_type pos ) const
    {
        if ( size() <= pos )
            NH3API_THROW(std::out_of_range, "vector::at(pos): invalid pos");
        return *(_First + pos);
    }
    reference at( size_type pos )
    {
        if ( size() <= pos )
            NH3API_THROW(std::out_of_range, "vector::at(pos): invalid pos");
        return *(_First + pos);
    }
    NH3API_FORCEINLINE
    const_reference operator[]( size_type pos ) const
    #if !NH3API_DEBUG
    NH3API_NOEXCEPT
    #endif
    {
    #if !NH3API_DEBUG
        return *(_First + pos);
    #else
        return at(pos);
    #endif
    }
    NH3API_FORCEINLINE
    reference operator[]( size_type pos )
    #if !NH3API_DEBUG
    NH3API_NOEXCEPT
    #endif
    {
    #if !NH3API_DEBUG
        return *(_First + pos);
    #else
        return at(pos);
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
        if ( this->_Last == this->_End )
        {
            _Reallocate_exactly(_Calculate_growth(size() + 1));
        }
        nh3api::construct_at(this->_Last, value);
        ++this->_Last;
    }

    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    void assign(IterT first, IterT last
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
    {
        nh3api::verify_range(first, last);
        _Assign_range(nh3api::unfancy(first), nh3api::unfancy(last), nh3api::iter_category<IterT>());
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
                NH3API_THROW(std::length_error, "vector::assign: size exceeded max_size");
            }

            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            if (this->_First != pointer())
            { // destroy and deallocate old array
                _Destroy_range(this->_First, this->_Last);
                _Deallocate(this->_First);
            }

            _Buy(_Newcapacity);
            this->_Last = _Ufill(this->_First, _Newsize, _Val);
        }
        else if (_Newsize > _Oldsize)
        {
            nh3api::fill(this->_First, this->_Last, _Val);
            this->_Last = _Ufill(this->_Last, _Newsize - _Oldsize, _Val);
        }
        else
        {
            const pointer _Newlast = this->_First + _Newsize;
            nh3api::fill(this->_First, _Newlast, _Val);
            _Destroy_range(_Newlast, this->_Last);
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
    void _Assign_range(IterT first, IterT last, std::input_iterator_tag )
    {	// assign [first, last), input iterators
        nh3api::verify_range(first, last);

        pointer _Next = this->_First;
        for (; first != last && _Next != this->_Last; ++first, (void)++_Next)
        {
            *_Next = *first;
        }

        _Destroy_range(_Next, this->_Last);
        this->_Last = _Next;

        #if NH3API_STD_MOVE_SEMANTICS
        for (; first != last; ++first)
            emplace_back(*first);
        #else
        for (; first != last; ++first)
            push_back(*first);
        #endif
    }

    template<class IterT>
    void _Assign_range(IterT first, IterT last, std::forward_iterator_tag )
    { // assign [first, last), forward iterators
        nh3api::verify_range(first, last);
        if (first == last)
            return;

        const size_type _Newsize = static_cast<size_type>(static_cast<size_t>(std::distance(first, last)));
        const size_type _Oldsize = size();
        const size_type _Oldcapacity = capacity();

        if (_Newsize > _Oldcapacity)
        { // reallocate
            if (_Newsize > max_size())
            {
                NH3API_THROW(std::length_error, "vector::assign: size exceeded max_size");
            }

            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            if (this->_First != pointer())
            { // destroy and deallocate old array
                _Destroy_range(this->_First, this->_Last);
                _Deallocate(this->_First);
            }

            _Buy(_Newcapacity);

            this->_Last = std::uninitialized_copy(first, last, this->_First);
        }
        else if (_Newsize > _Oldsize)
        {
            // performance note: traversing [first, _Mid) twice
            const IterT _Mid = nh3api::next(first, static_cast<difference_type>(_Oldsize));
            nh3api::copy(first, _Mid, this->_First);
            this->_Last = std::uninitialized_copy(_Mid, last, this->_Last);
        }
        else
        {
            const pointer _Newlast = this->_First + _Newsize;
            nh3api::copy(first, last, this->_First);
            _Destroy_range(_Newlast, this->_Last);
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
        pointer _Whereptr = const_cast<pointer>(_Where);
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
                NH3API_THROW(std::length_error, "vector::insert: size exceeded max_size");
            }

            const size_type _Newsize = _Oldsize + _Count;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            pointer _Newvec = _Allocate(_Newcapacity);
            pointer _Constructed_last = _Newvec + _Whereoff + _Count;
            pointer _Constructed_first = _Constructed_last;

            { // scope for exception guard
                NH3API_MAKE_EXCEPTION_GUARD(
                    noexcept_copy::value && noexcept_move::value,
                    range_cleanup, *this, _Constructed_first, _Constructed_last, _Newvec, _Newcapacity);
                
                _Ufill(_Newvec + _Whereoff, _Count, _Val);
                _Constructed_first = _Newvec + _Whereoff;

                if (_One_at_back)
                { // provide strong guarantee
                    _Umove_if_noexcept(this->_First, this->_Last, _Newvec);
                }
                else
                { // provide basic guarantee
                    _Umove(this->_First, _Whereptr, _Newvec);
                    _Constructed_first = _Newvec;
                    _Umove(_Whereptr, this->_Last, _Newvec + _Whereoff + _Count);
                }

                guard.complete();
            } // scope for exception guard

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
            const T _Tmp = _Val; // handle aliasing
            const pointer _Oldlast = this->_Last;
            const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where);

            if (_Count > _Affected_elements)
            { // new stuff spills off end
                this->_Last = _Ufill(_Oldlast, _Count - _Affected_elements, _Tmp);
                this->_Last = _Umove(_Whereptr, _Oldlast, this->_Last);
                nh3api::fill(_Whereptr, _Oldlast, _Tmp);
            }
            else
            { // new stuff can all be assigned
                this->_Last = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                _Move_backward(_Whereptr, _Oldlast - _Count, _Oldlast);
                nh3api::fill(_Whereptr, _Whereptr + _Count, _Tmp);
            }
        }
        return _Make_iterator_offset(_Whereoff);
    }

    template<class IterT>
    typename nh3api::tt::enable_if<nh3api::tt::is_iterator<IterT>::value, iterator>::type
    insert( const_iterator _Where, IterT first, IterT last )
    {
        nh3api::verify_range(first, last);
        const size_type _Whereoff = static_cast<size_type>(_Where - this->_First);
        _Insert_range(_Where, nh3api::unfancy(first), nh3api::unfancy(last), nh3api::iter_category<IterT>());
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
            NH3API_THROW(std::logic_error, "vector::pop_back(): vector was empty");
        else
        {	// erase last element
            nh3api::destroy_at(this->_Last - 1);
            --this->_Last;
        }
    }

    #else // NH3API_DEBUG
    void pop_back()
    {	// erase element at end
        if (!empty())
        {	// erase last element
            nh3api::destroy_at(this->_Last - 1);
            --this->_Last;
        }
    }
    #endif // NH3API_DEBUG

    iterator erase(const_iterator pos)
    {
    #if NH3API_DEBUG
        if (pos < this->_Last || this->_First <= pos)
            NH3API_THROW(std::out_of_range, "vector::erase: iterator outside bounds");
    #endif // NH3API_DEBUG

    #if NH3API_STD_MOVE_SEMANTICS
        NH3API_IF_CONSTEXPR ( nh3api::tt::is_move_assignable<value_type>::value )
            std::move(pos + 1, this->_Last, pos);
        else
            nh3api::copy(pos + 1, this->_Last, pos);
    #else
            nh3api::copy(pos + 1, this->_Last, pos);
    #endif
        nh3api::destroy_at(this->_Last - 1);
        --this->_Last;
        return pos;
    }

    iterator erase( const_iterator first, const_iterator last )
    {
        #if NH3API_DEBUG
        if (first < _First || last > _Last)
            NH3API_THROW(std::out_of_range, "vector::erase: iterator outside bounds");
        #endif // NH3API_DEBUG

        iterator _It1 = const_cast<iterator>(first);
        iterator _It2 = const_cast<iterator>(last);
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
            _Destroy_range(_Ptr, this->_Last);
            this->_Last = _Ptr;
        }
        return _It1;

    }

    void clear() NH3API_NOEXCEPT
    {
        _Destroy_range(_First, _Last);
        this->_Last = this->_First;
    }

    void swap( exe_vector& other ) NH3API_NOEXCEPT
    { nh3api::trivial_swap<sizeof(*this)>(this, &other); }

protected:
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    struct vector_cleanup
    {
        public:
            NH3API_CONSTEXPR
            vector_cleanup(exe_vector& _vec) NH3API_NOEXCEPT
                : vec(_vec) 
            {}

            NH3API_CONSTEXPR
            void operator()() 
            { 
                vec._Tidy();
                NH3API_RETHROW
            }

        private:
            exe_vector& vec;
    };

    // functor to pass to nh3api's exception guard
    struct range_cleanup
    {
        public:
            NH3API_CONSTEXPR range_cleanup(exe_vector& _vec, 
                                            pointer& _destroy_first, 
                                            pointer& _destroy_last, 
                                            pointer& _deallocate_at, 
                                            const size_type& _deallocation_size ) NH3API_NOEXCEPT
                : vec(_vec), 
                destroy_first(_destroy_first),
                destroy_last(_destroy_first),
                deallocate_at(_deallocate_at),
                deallocation_size(_deallocation_size)
            {}

            void operator()() 
            { 
                exe_vector::_Destroy_range(destroy_first, destroy_last);
                vec._Deallocate(deallocate_at);
                NH3API_RETHROW
            }
            
        private:
            exe_vector& vec;
            pointer& destroy_first;
            pointer& destroy_last;
            pointer& deallocate_at;
            const size_type& deallocation_size;
    };
    #else 
    struct vector_cleanup
    {
        NH3API_CONSTEXPR
        vector_cleanup(exe_vector&) NH3API_NOEXCEPT
        {}

        NH3API_CONSTEXPR
        void operator()() NH3API_NOEXCEPT
        {}
    };

    struct range_cleanup
    {
        NH3API_CONSTEXPR range_cleanup(exe_vector&, 
                                        pointer, 
                                        pointer,
                                        pointer, 
                                        const size_type) NH3API_NOEXCEPT
        {}

        NH3API_CONSTEXPR
        void operator()() NH3API_NOEXCEPT
        {}
    };
    #endif

    // set capacity to _Newcapacity (without geometric growth), provide strong guarantee
    void _Reallocate_exactly(const size_type _Newcapacity)
    {
        const size_type _Size = size();
        const pointer _Newvec = _Allocate(_Newcapacity);

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
                _Deallocate(_Newvec);
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

    // move [first, last) to raw _Dest, using allocator
    void _Umove_if_noexcept(pointer first, pointer last, pointer _Dest)
    {
        nh3api::verify_range(first, last);
        #if NH3API_STD_MOVE_SEMANTICS
        _Umove_if_noexcept_impl(first, last, _Dest,
        nh3api::tt::disjunction_2<
        nh3api::tt::is_nothrow_move_constructible<value_type>,
        nh3api::tt::negation<nh3api::tt::is_copy_constructible<value_type>>
        >());
        #else
        std::uninitialized_copy<pointer, pointer>(first, last, dst);
        #endif
    }

    // move [first, last) to raw _Dest, using allocator
    void _Umove_if_noexcept_impl(pointer first, pointer last, pointer dst, nh3api::tt::true_type) NH3API_NOEXCEPT
    { _Umove(first, last, dst); }

    // copy [first, last) to raw _Dest, using allocator
    void _Umove_if_noexcept_impl(pointer first, pointer last, pointer dst, nh3api::tt::false_type)
    { std::uninitialized_copy<pointer, pointer>(first, last, dst); }

    NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
    static void _Destroy_range(pointer first, pointer last)
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
    { 
        nh3api::verify_range(first, last);
        nh3api::destroy<pointer>(first, last); 
    }

    NH3API_FORCEINLINE
    #if !NH3API_STD_MOVE_SEMANTICS && NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE) && NH3API_CHECK_MSVC
    #else
    static 
    #endif
    pointer _Umove(pointer first, pointer last, pointer ptr)
    {
        nh3api::verify_range(first, last);
        #if !NH3API_STD_MOVE_SEMANTICS && NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
            return std::_Uninitialized_move(first, last, ptr, allocator_type());
        #else
            NH3API_STATIC_ASSERT("value_type must be either copy or move constructible",
            nh3api::tt::is_move_constructible<value_type>::value || nh3api::tt::is_copy_constructible<value_type>::value);
            return std::uninitialized_move<pointer, pointer>(first, last, ptr);
        #endif
    }
    
    NH3API_FORCEINLINE
    static pointer _Move_backward(pointer first, pointer last, pointer ptr)
    {
        nh3api::verify_range(first, last);
        #if NH3API_MSVC_STL && NH3API_MSVC_STL_VERSION < NH3API_MSVC_STL_VERSION_2010 // old MSVC compiler
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
    static pointer _Ufill(pointer first, size_type count, const value_type& value)
    NH3API_NOEXCEPT_EXPR(noexcept_copy::value)
    {
        std::uninitialized_fill_n<pointer, size_type, value_type>(first, count, value);
        return first + count; // see LWG 1339
    }

    #ifndef __cpp_lib_raw_memory_algorithms

    NH3API_FORCEINLINE static pointer _Default_fill_impl(pointer first, size_type n, tt::false_type)
    {
        pointer current = first;
        NH3API_TRY
        {
            for (; n > 0; (void) ++current, --n)
                ::new (const_cast<void*>(static_cast<const volatile void*>(
                    ::nh3api::addressof(*current)))) T();
        }
        NH3API_CATCH(...)
        {
            ::nh3api::destroy<pointer>(first, current);
            NH3API_RETHROW
        }
        return current;
    }

    NH3API_FORCEINLINE static pointer _Default_fill_impl(pointer first, size_type n, tt::true_type)
    NH3API_NOEXCEPT
    {
        pointer current = first;
        for (; n > 0; (void) ++current, --n)
            ::new (const_cast<void*>(static_cast<const volatile void*>(
                ::nh3api::addressof(*current)))) T();
        return current;
    }

    #endif // __cpp_lib_raw_memory_algorithms

    NH3API_FORCEINLINE
    static pointer _Default_fill(pointer first, size_type count)
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<value_type>::value)
    {
        nh3api::verify_range_n(first, count);
        #ifdef __cpp_lib_raw_memory_algorithms
        return std::uninitialized_value_construct_n<pointer, size_type>(first, count);
        #else 
        return _Default_fill_impl(first, count, nh3api::tt::is_nothrow_default_constructible<value_type>());
        #endif
    }

    NH3API_FORCEINLINE
    static pointer _Allocate(size_type num) NH3API_NOEXCEPT
    { return static_cast<pointer>(::operator new(num * sizeof(value_type), exe_heap, std::nothrow)); }

    NH3API_FORCEINLINE
    static void _Deallocate(void* ptr) NH3API_NOEXCEPT 
    { ::operator delete(ptr, exe_heap); }

    // allocate array with _Capacity elements
    bool _Buy(size_type _Capacity)
    {
        this->_First = nullptr;
        this->_Last  = nullptr;
        this->_End   = nullptr;

        if (_Capacity == 0)
            return false;
        else if (max_size() < _Capacity)
            NH3API_THROW(std::invalid_argument, "vector::reserve: invalid requested size.");
        else
        {	// nonempty array, allocate storage
            this->_First = _Allocate(_Capacity);
            this->_Last  = this->_First;
            this->_End   = this->_First + _Capacity;
        }
        return true;
    }

    // given _Oldcapacity and _Newsize, calculate geometric growth
    size_type _Calculate_growth(const size_type _Newsize) const NH3API_NOEXCEPT
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
    void _Change_array(const pointer _Newvec, 
                       const size_type _Newsize, 
                       const size_type _Newcapacity) NH3API_NOEXCEPT
    {
        _Tidy();

        this->_First = _Newvec;
        this->_Last = _Newvec + _Newsize;
        this->_End = _Newvec + _Newcapacity;
    }

    // free all storage
    void _Tidy() NH3API_NOEXCEPT
    {
        if (this->_First != nullptr)
        {	// something to free, destroy and deallocate it
            _Destroy_range(this->_First, this->_Last);
            _Deallocate(this->_First);
        }
    }

    // insert [first, last) at _Where, input iterators
    template<class IterT>
    void _Insert_range(const_iterator _Where, IterT first, IterT last, std::input_iterator_tag)
    {
        if (first == last)
        {
            return;	// nothing to do, avoid invalidating iterators
        }

        const size_type _Whereoff = static_cast<size_type>(_Where - this->_First);
        const size_type _Oldsize = size();

#if NH3API_STD_MOVE_SEMANTICS
        for (; first != last; ++first)
            emplace_back(*first);
#else
        for (; first != last; ++first)
            push_back(*first);
#endif

        std::rotate(this->_First + _Whereoff, this->_First + _Oldsize, this->_Last);
    }

    template<class IterT>
    void _Insert_range(const_iterator _Where, IterT first, IterT last, std::forward_iterator_tag)
    {
        nh3api::verify_range(first, last);
        iterator _Whereptr = const_cast<pointer>(_Where);
        const size_type _Count = static_cast<size_type>(static_cast<size_t>(std::distance(first, last)));
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
                NH3API_THROW(std::length_error, "vector::insert: size exceeded max_size");
            }

            const size_type _Newsize = _Oldsize + _Count;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            pointer _Newvec = _Allocate(_Newcapacity);
            pointer _Constructed_last = _Newvec + _Whereoff + _Count;
            pointer _Constructed_first = _Constructed_last;

            { // scope for exception guard
            NH3API_MAKE_EXCEPTION_GUARD(
                noexcept_copy::value && noexcept_move::value,
                range_cleanup, *this, _Constructed_first, _Constructed_last, _Newvec, _Newcapacity);
            
            std::uninitialized_copy(first, last, _Newvec + _Whereoff);
            _Constructed_first = _Newvec + _Whereoff;
            if (_One_at_back)
            { // provide strong guarantee
                _Umove_if_noexcept(this->_Last, this->_Last, _Newvec);
            }
            else
            { // provide basic guarantee
                _Umove(this->_Last, _Whereptr, _Newvec);
                _Constructed_first = _Newvec;
                _Umove(_Whereptr, this->_Last, _Newvec + _Whereoff + _Count);
            }

            guard.complete();
            
            } // scope for exception guard

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
                this->_Last = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                _Move_backward(_Whereptr, _Oldlast - _Count, _Oldlast);
                _Destroy_range(_Whereptr, _Whereptr + _Count);

                NH3API_IF_CONSTEXPR ( noexcept_copy::value )
                {
                    std::uninitialized_copy<IterT, pointer>(first, last, _Whereptr);
                }
                else
                {
                    NH3API_TRY
                    {
                        std::uninitialized_copy<IterT, pointer>(first, last, _Whereptr);
                    }
                    NH3API_CATCH(...)
                    {
                        NH3API_TRY
                        {
                            _Umove(_Whereptr + _Count, _Whereptr + 2 * _Count, _Whereptr);
                        }
                        NH3API_CATCH(...)
                        {
                            // vaporize the detached piece
                            _Destroy_range(_Whereptr + _Count, this->_Last);
                            this->_Last = _Whereptr;
                            NH3API_RETHROW
                        }
                        std::move(_Whereptr + 2 * _Count, this->_Last, _Whereptr + _Count);
                        _Destroy_range(_Oldlast, this->_Last);
                        this->_Last = _Oldlast;
                        NH3API_RETHROW
                    }
                }
            }
            else
            { // affected elements don't overlap before/after
                const pointer _Relocated = _Whereptr + _Count;
                this->_Last = _Umove(_Whereptr, _Oldlast, _Relocated);
                _Destroy_range(_Whereptr, _Oldlast);
                NH3API_IF_CONSTEXPR ( noexcept_copy::value )
                {
                    std::uninitialized_copy<IterT, pointer>(first, last, _Whereptr);
                }
                else
                {
                    NH3API_TRY
                    {
                        std::uninitialized_copy<IterT, pointer>(first, last, _Whereptr);
                    }
                    NH3API_CATCH(...)
                    {
                        NH3API_TRY
                        {
                            _Umove(_Relocated, this->_Last, _Whereptr);
                        }
                        NH3API_CATCH(...)
                        {
                            // vaporize the detached piece
                            _Destroy_range(_Relocated, this->_Last);
                            this->_Last = _Whereptr;
                            NH3API_RETHROW
                        }
                        _Destroy_range(_Relocated, this->_Last);
                        this->_Last = _Oldlast;
                        NH3API_RETHROW
                    }
                }
            }
        }
    }

protected:
    uint32_t _Dummy; // the original implementation had no size optimization for empty allocator
    pointer _First;	// pointer to beginning of array
    pointer _Last;	// pointer to current end of sequence
    pointer _End;	// pointer to end of array
};
#pragma pack(pop)

//}; // namespace nh3api

#if !NH3API_STD_MOVE_SEMANTICS
template< class T>
NH3API_FORCEINLINE
void swap( exe_vector<T>& lhs, exe_vector<T>& rhs ) // ADL swap
{ lhs.swap(rhs); }
#endif

template<class T> NH3API_FORCEINLINE
    bool operator==(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test for vector equality
    return (_Left.size() == other.size()
        && std::equal(_Left.begin(), _Left.end(), other.begin()));
}

template<class T> NH3API_FORCEINLINE
    bool operator!=(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test for vector inequality
    return (!(_Left == other));
}

template<class T> NH3API_FORCEINLINE
    bool operator<(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test if _Left < other for vectors
    return (std::lexicographical_compare(_Left.begin(), _Left.end(),
        other.begin(), other.end()));
}

template<class T> NH3API_FORCEINLINE
    bool operator>(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test if _Left > other for vectors
    return (other < _Left);
}

template<class T> NH3API_FORCEINLINE
    bool operator<=(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test if _Left <= other for vectors
    return (!(other < _Left));
}

template<class T> NH3API_FORCEINLINE
    bool operator>=(const exe_vector<T>& _Left, const exe_vector<T>& other)
{	// test if _Left >= other for vectors
    return (!(_Left < other));
}

#if NH3API_MSVC_STL && !defined(_MSVC_STL_UPDATE)
NH3API_DISABLE_MSVC_WARNING_END
#endif

NH3API_DISABLE_WARNING_END
#endif // NH3API_FLAG_USE_NATIVE_STD_VECTOR
