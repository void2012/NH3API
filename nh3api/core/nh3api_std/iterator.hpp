//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <utility>
#include <iterator>
#include "intrin.hpp"

#if !NH3API_STD_MOVE_SEMANTICS
namespace nh3api
{
    template<class InputIt>
    InputIt next(InputIt it, typename ::std::iterator_traits<InputIt>::difference_type n = 1)
    {
        ::std::advance(it, n);
        return it;
    }

    template<class BidirIt>
    BidirIt prev(BidirIt it, typename ::std::iterator_traits<BidirIt>::difference_type n = 1)
    {
        ::std::advance(it, -n);
        return it;
    }
}
#else
namespace nh3api
{
using ::std::next;
using ::std::prev;
}
#endif

namespace nh3api
{

namespace details
{

template<typename T, bool IsIterator>
struct iter_category_helper
{
    typedef typename ::std::iterator_traits<T>::iterator_category type;
};

template<typename T>
struct iter_category_helper<T, false>
{ typedef void type; };

}

namespace tt
{

template<typename T, typename Enable = void>
struct is_iterator
    : false_type {};

// This solution works in C++98
template<typename T>
struct is_iterator<T, typename enable_if<
!is_void<typename ::std::iterator_traits<T>::value_type>::value
&& !is_integral<T>::value && !is_arithmetic<T>::value>::type>
    : true_type
{};

template<>
struct is_iterator<void*>
    : true_type
{};

template<>
struct is_iterator<void>
    : false_type
{};

#if NH3API_CHECK_CPP14
namespace details
{
template <typename T, typename = void>
struct is_iterator_class_test
    : false_type
{};

template <typename T>
struct is_iterator_class_test<T, decltype((void)nh3api::declval<T>().operator->(), void())>
    : true_type
{};

template <class T, bool = is_class<T>::value>
struct is_iterator_class_impl
    : false_type
{};

template <class T>
struct is_iterator_class_impl<T, true>
    : integral_constant<bool, is_iterator_class_test<T>::value>
{};
} // namespace details

template <class T>
struct is_iterator_class
    : integral_constant<bool, details::is_iterator_class_impl<T>::value>
{};

template <>
struct is_iterator_class<void>
    : false_type
{};
#else
namespace details
{
template <typename type>
class has_arrow_operator_test
{
    class yes { char m; };
    class no { yes m[2]; };
    struct base_mixin
    {
        void operator->(){}
    };
    struct base : public type, public base_mixin {};
    template <typename T, T t> class helper{};
    template <typename U>
    static no deduce(U*, helper<void (base_mixin::*)(), &U::operator-> >* = 0);
    static yes deduce(...);
public:
    static const bool result = sizeof(yes) == sizeof(deduce((base*)
        (0)));
};

template <typename type, typename call_details>
struct is_iterator_class_test
{
private:
    class yes {};
    class no { yes m[2]; };
    struct derived : public type
    {
        using type::operator->;
    };
    typedef typename details::clone_constness<type, derived>::type derived_type;
    template <typename T, typename due_type>
    struct return_value_check
    {
        static yes deduce(due_type);
        static no deduce(...);
        static no deduce(no);
        static no deduce(details::void_exp_result<type>);
    };
    template <typename T>
    struct return_value_check<T, void>
    {
        static yes deduce(...);
        static no deduce(no);
    };
    template <bool has, typename F>
    struct deduce_impl
    {
        static const bool value = false;
    };
    template <typename r>
    struct deduce_impl<true, r()>
    {
        static const bool value =
        sizeof(
        return_value_check<type, r>::deduce( ((derived_type*)0)->operator->() )) == sizeof(yes);

    };
public:
    static const bool value = deduce_impl<has_arrow_operator_test<type>::result,
        call_details>::value;
};

template <class T, bool = is_class<T>::value>
struct is_iterator_class_impl
    : false_type
{
};

template <class T>
struct is_iterator_class_impl<T, true>
    : integral_constant<bool, is_iterator_class_test<T, typename ::std::iterator_traits<T>::pointer()>::value>
{
};
} // namespace details

template <class T>
struct is_iterator_class
    : integral_constant<bool, details::is_iterator_class_impl<T>::value>
{};

template <>
struct is_iterator_class<void>
    : false_type
{};
#endif
} // namespace tt

template<typename IterT> NH3API_CONSTEXPR
typename details::iter_category_helper<IterT, tt::is_iterator<IterT>::value>::type iter_category() NH3API_NOEXCEPT
{
    typename details::iter_category_helper<IterT, tt::is_iterator<IterT>::value>::type category;
    return category;
};

#if NH3API_STD_RELAXED_CONSTEXPR
struct stop_constexpr
{
    static void stop(const char* ptr)
    {
        assert(false && ptr);
    }
};
#endif

#if NH3API_HAS_IS_CONSTANT_EVALUATED
NH3API_CONSTEXPR
#endif
void check_valid_iterator(bool condition)
{
    #if NH3API_DEBUG
        #if NH3API_HAS_IS_CONSTANT_EVALUATED
        if ( is_constant_evaluated() )
        {
            if (!condition)
                stop_constexpr::stop("verify_iterator: invalid iterator");
        }
        else
        {
            assert(!condition && "verify_iterator: invalid iterator");
        }
        #else
            assert(!condition && "verify_iterator: invalid iterator");
        #endif
    #else
    (void) condition;
    #endif
}

#ifndef NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR
    #if NH3API_MSVC_STL
        #define NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR NH3API_CONSTEXPR_CPP_20
    #elif NH3API_CLANG_STL
        // clang has no checked iterators(sad!)
        #define NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR constexpr
    #elif NH3API_GCC_STL
        #define NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR NH3API_CONSTEXPR_CPP_20
    #endif
#endif

#ifndef NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR
    #define NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR
#endif

template<typename T> NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR
void verify_iterator(const T& arg);

template<class T> NH3API_CONSTEXPR
void verify_iterator(T* const ptr)
{
    #if NH3API_DEBUG
    check_valid_iterator(ptr != nullptr);
    #else
    (void) ptr;
    #endif
}

// verify MSVC STL range of iterators

// there was no such macro before VS 2017 15.5,
// I assume there was also no _Adl_verify_range, so that's it. :P
#if defined(_MSVC_STL_UPDATE)

// verify range on MSVC STL checked iterators
template <class _Iter, class _Sentinel> inline
constexpr void verify_range(const _Iter& _First, const _Sentinel& _Last)
{ _STD _Adl_verify_range(_First, _Last); }

template <class _Iter, class _Size_type> inline
constexpr void verify_range_n(const _Iter& _First, const _Size_type _N)
{
    using diff_t = typename ::std::iterator_traits<_Iter>::difference_type;
    auto _Last = next(_First, static_cast<diff_t>(_N));
    verify_range(_First, _Last);
}

#ifdef __cpp_lib_to_address
template<typename IterT NH3API_REQUIRES(is_iterator_class<IterT>::value)
constexpr auto to_address(const IterT& iter) noexcept
{
    return ::std::to_address(iter);
}
#else
template<typename T>
T* to_address(T* ptr) noexcept
{
    static_assert(!tt::is_function<T>::value, "T is a function type");
    return ptr;
}

template<typename IterT NH3API_REQUIRES(is_iterator_class<IterT>::value)
auto to_address(const IterT& iter) noexcept
    -> decltype(declval<const IterT&>().operator->())
{
    return to_address(iter.operator->());
}
#endif

template<typename, typename = void>
struct is_checked_iterator
: tt::false_type {};

template<typename _Iter>
struct is_checked_iterator
<_Iter, void_t<_Iter, decltype(declval<_Iter>()._Getcont())>>
: tt::true_type {};

template<typename _Iter>
constexpr NH3API_INLINE bool is_checked_iterator_v = is_checked_iterator<_Iter>::value;

// this is NOT constexpr
template<typename _Deque>
void verify_iterator(const ::std::_Deque_unchecked_const_iterator<_Deque>& arg)
{
    #if NH3API_DEBUG
    check_valid_iterator((to_address(arg) != nullptr) && (arg._Getcont() != nullptr));
    #else
    (void) arg;
    #endif
}

template <class _Iter, ::std::enable_if_t<is_checked_iterator_v<_Iter>, int> = 0> inline
NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR void verify_iterator(const _Iter& arg)
{
    #if NH3API_DEBUG
    check_valid_iterator((to_address(arg) != nullptr) && (arg._Getcont() != nullptr));
    #else
    (void) arg;
    #endif
}

template <class _Iter, ::std::enable_if_t<!is_checked_iterator_v<_Iter>, int> = 0> inline
NH3API_CONSTEXPR_IF_ITERATOR_CHECK_IS_CONSTEXPR void verify_iterator(const _Iter& arg)
{
    #if NH3API_DEBUG
    check_valid_iterator(to_address(arg) != nullptr);
    #else
    (void) arg;
    #endif
}

#elif defined(_MSC_VER) && !defined(_MSVC_STL_UPDATE) && !defined(__clang__)

template<typename T>
T* to_address(T* ptr) NH3API_NOEXCEPT
{
    return ptr;
}

template<typename IterT NH3API_SFINAE_BEGIN(tt::is_iterator_class<IterT>::value)>
typename ::std::iterator_traits<IterT>::pointer to_address(const IterT& iter
                                                           NH3API_SFINAE_END(tt::is_iterator_class<IterT>::value)) NH3API_NOEXCEPT
{
    return to_address(iter.operator->());
}

template<typename _Iter,
bool is_base_of_checked = tt::is_base_of<_STD _Iterator_base12, _Iter>::value >
struct verify_iterator_helper_msvc
{
    static bool valid(const _Iter&);
};

template<typename _Iter>
struct verify_iterator_helper_msvc<_Iter, true>
{
    static bool valid(const _Iter& arg)
    {
        if ( to_address(arg) != nullptr )
            return arg._Getcont() != nullptr;
        else
             return false;
    }
};

template<typename _Iter>
struct verify_iterator_helper_msvc<_Iter, false>
{
    static bool valid(const _Iter& arg)
    { return to_address(arg) != nullptr; }
};

template<typename T>
void verify_iterator(const T& arg)
{
    #if NH3API_DEBUG
    check_valid_iterator(verify_iterator_helper_msvc<T>::valid(arg));
    #else
    (void) arg;
    #endif
}

// verify range on (old) MSVC STL checked iterators
template <class _Iter, class _Sentinel>
inline void verify_range(const _Iter& _First, const _Sentinel& _Last)
{
    #if NH3API_DEBUG
    using namespace std; // to make _DEBUG_RANGE macro range work
    _DEBUG_RANGE(_First, _Last);
    #else
    NH3API_IGNORE(_First, _Last);
    #endif
}

template <class _Iter, class _Size_type>
inline void verify_range_n(const _Iter& _First, _Size_type _N)
{
    #if NH3API_DEBUG
    const _Iter& _Last = next(_First, static_cast<typename ::std::iterator_traits<_Iter>::difference_type>(_N));
    using namespace std; // to make _DEBUG_RANGE macro range work
    _DEBUG_RANGE(_First, _Last);
    #else
    NH3API_IGNORE(_First, _Last);
    #endif
}

#elif defined(__GLIBCXX__)

#if __GNUC__ >= 8
template<typename IterT>
constexpr auto to_address(const IterT& iter) noexcept
{
    #ifdef __cpp_lib_to_address
    return ::std::to_address(iter);
    #else
    return ::std::__to_address(iter);
    #endif
}

#else
template<typename T>
T* to_address(T* ptr) noexcept
{
    static_assert(!tt::is_function<T>::value, "T is a function type");
    return ptr;
}

template<typename IterT NH3API_REQUIRES(tt::is_iterator_class<IterT>::value)
auto to_address(const IterT& iter) noexcept
    -> decltype(declval<const IterT&>().operator->())
{
    return to_address(iter.operator->());
}
#endif

#ifdef _GLIBCXX_DEBUG
    //template<typename _BaseIter, typename _Sequence, typename _Iter>
    //constexpr bool is_base_of_checked_v = std::is_base_of<__gnu_debug::_Safe_iterator<_BaseIter, _Sequence>, _Iter>::value;

    template<typename _Iter>
    struct verify_iterator_helper_glibcxx
    {
        static NH3API_CONSTEXPR_CPP_20 bool valid(const _Iter& arg)
        { return to_address(arg) != nullptr; }
    };

    template<typename _BaseIter, typename _Sequence>
    struct verify_iterator_helper_glibcxx<::__gnu_debug::_Safe_iterator<_BaseIter, _Sequence>>
    {
        static NH3API_CONSTEXPR_CPP_20 bool valid(const ::__gnu_debug::_Safe_iterator<_BaseIter, _Sequence>& arg)
        {
            if ( to_address(arg) != nullptr )
                return arg._M_value_initialized();
            else
                return false;
        }
    };

    template<typename T> NH3API_CONSTEXPR_CPP_20
    void verify_iterator(const T& arg)
    {
        #if NH3API_DEBUG
        check_valid_iterator(verify_iterator_helper_glibcxx<T>::valid(arg));
        #else
        (void) arg;
        #endif
    }
#else
    template<typename T> constexpr
    void verify_iterator(const T& arg)
    {
        #if NH3API_DEBUG
        check_valid_iterator(to_address(arg) != nullptr);
        #else
        (void) arg;
        #endif
    }
#endif

// verify range on GNU libstdc++ debug iterators
template<typename _InputIterator>
NH3API_CONSTEXPR_CPP_20 inline void
verify_range(_InputIterator _First, _InputIterator _Last)
{ __glibcxx_requires_valid_range(_First, _Last); }

template<typename _InputIterator, typename _Size_type>
NH3API_CONSTEXPR_CPP_20 inline void
verify_range_n(_InputIterator _First, _Size_type _N)
{
    #if NH3API_DEBUG
    using diff_t = typename ::std::iterator_traits<_InputIterator>::difference_type;
    auto _Last = next(_First, static_cast<diff_t>(_N));
    verify_range(_First, _Last);
    #else
    NH3API_IGNORE(_First, _N);
    #endif
}

#else

template<typename T> constexpr
inline void verify_iterator(const T& arg)
{
    #if NH3API_DEBUG
    check_valid_iterator(to_address(arg) != nullptr);
    #else
    (void) arg;
    #endif
}

template <class _Iter, class _Sentinel>
NH3API_CONSTEXPR inline
void verify_range(const _Iter &_First, const _Sentinel &_Last)
{ (void)_First; (void)_Last; }

template <class _Iter, class _Size_type> inline
constexpr void verify_range_n(const _Iter& _First, const _Size_type _N)
{ (void) _First; (void) _N;}

#endif

// unwrap MSVC STL iterators
// https://stackoverflow.com/questions/61440041/why-does-themicrosoft-visual-c-library-implementation-unwrap-iterators
// different versions have different ways to do this, fine...

// vs 2005..2008 checked iterators
#if defined(_MSC_VER) && (_MSC_VER < 1600) && defined(_CHECKED_BASE)

template<typename _Iter_type> inline
typename _STD _Checked_iterator_base_helper<_Iter_type>::_Checked_iterator_base_type
unfancy(const _Iter_type& iter) NH3API_NOEXCEPT
{ return _CHECKED_BASE(iter); }

// vs 2010..2015 checked iterators
#elif defined(_MSC_VER) && (_MSC_VER >= 1600) && !defined(_MSVC_STL_UPDATE) && !defined(__clang__)

template<typename _Iter> inline
typename _Iter::_Unchecked_type unfancy(const _Iter& iter) NH3API_NOEXCEPT
{ return ::std::_Unchecked(iter); }

template<class _Ty, size_t _Size> inline
typename ::std::_Array_iterator<_Ty, _Size>::_Unchecked_type
unfancy(const ::std::_Array_iterator<_Ty, _Size>& _Iter) NH3API_NOEXCEPT
{ return (_Iter._Unchecked()); }

template<class _Ty, size_t _Size> inline
typename ::std::_Array_const_iterator<_Ty, _Size>::_Unchecked_type
unfancy(const ::std::_Array_const_iterator<_Ty, _Size>& _Iter) NH3API_NOEXCEPT
{ return (_Iter._Unchecked()); }

#elif defined(_MSVC_STL_UPDATE)

template<class _Iter>
_NODISCARD inline constexpr decltype(auto) unfancy(const _Iter& _It) noexcept
{ return ::std::_Get_unwrapped(_It); }

template<class _Iter>
_NODISCARD inline constexpr decltype(auto) unfancy(const _Iter&& _It) noexcept
{ return ::std::_Get_unwrapped(::std::forward(_It)); }

#elif NH3API_CLANG_STL

#if __clang_major__ >= 7
template<typename IterT NH3API_REQUIRES(tt::is_iterator_class<IterT>::value)
constexpr auto to_address(const IterT& iter) noexcept
{
    #ifdef __cpp_lib_to_address
    return ::std::to_address(iter);
    #else
    return ::std::__to_address(iter);
    #endif
}

#else
template<typename T>
T* to_address(T* ptr) noexcept
{
    static_assert(!tt::is_function<T>::value, "T is a function type");
    return ptr;
}

template<typename IterT NH3API_REQUIRES(is_iterator_class<IterT>::value)
auto to_address(const IterT& iter) noexcept
    -> decltype(declval<const IterT&>().operator->())
{
    return to_address(iter.operator->());
}
#endif

template <class _Iter,
          class _Impl = ::std::__unwrap_iter_impl<_Iter>,
          ::std::__enable_if_t<::std::is_copy_constructible<_Iter>::value, int> = 0>
inline _LIBCPP_CONSTEXPR_SINCE_CXX14 decltype(_Impl::__unwrap(::std::declval<_Iter>()))
unfancy(_Iter __i) _NOEXCEPT
{
  return _Impl::__unwrap(__i);
}

#elif NH3API_GCC_STL
template<typename _Iter>
_Iter unfancy(const _Iter& iter) NH3API_NOEXCEPT
{ return iter; }

template<typename _Iter>
_Iter unfancy(const _Iter&& iter) NH3API_NOEXCEPT
{ return iter; }

#else
template<typename _Iter>
_Iter unfancy(const _Iter& iter) NH3API_NOEXCEPT
{ return iter; }
#endif

// typedefs to comply with the C++ standard
// iterator typedefs used by algorithms and other stuff
template<typename _ValueType, typename _DiffType, typename _IterTag>
struct container_iterator
{
    public:
        typedef _IterTag iterator_category;
        typedef _ValueType value_type;
        typedef _DiffType difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
};

// TODO: implement ranges for NH3API containers
#if NH3API_STD_RANGES_CONTAINERS
    #if NH3API_MSVC_STL
    template<typename Range, typename T>
    concept container_compatible_range = ::std::_Container_compatible_range<Range, T>;
    #elif NH3API_GCC_STL
    template<typename Range, typename T>
    concept container_compatible_range = ::std::__detail::__container_compatible_range<Range, T>;
    #elif NH3API_CLANG_STL
    template<typename Range, typename T>
    concept container_compatible_range = ::std::_ContainerCompatibleRange<Range, T>;
    #else
    #endif
#endif

/*
#if NH3API_STD_MOVE_SEMANTICS
template<typename IterT>
struct move_if_noexcept_iterator_helper
{
    typedef typename ::std::iterator_traits<IterT>::value_type T;
    typedef tt::integral_constant<bool,
    !tt::is_nothrow_move_constructible<T>::value && tt::is_copy_constructible<T>::value > condition;
    typedef typename tt::conditional<condition::value, IterT, ::std::move_iterator<IterT> >::type type;
};

template<typename IterT> NH3API_CONSTEXPR
typename move_if_noexcept_iterator_helper<IterT>::type
make_move_if_noexcept_iterator(IterT iter)
{
    return typename move_if_noexcept_iterator_helper<IterT>::type(iter);
}
#endif
*/

}