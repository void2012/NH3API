//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <iterator> // std::iterator_traits, std::random_access_iterator_tag
#include "type_traits.hpp"

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

// Detect if std::iterator_traits<T> is valid for T
template<typename T, typename = void>
struct has_iterator_traits_impl : false_type {};

template<typename T>
struct has_iterator_traits_impl<T, 
#if NH3API_CHECK_CPP11 || (!NH3API_CHECK_CLANG_CL && NH3API_MSVC_VERSION >= NH3API_MSVC_VERSION_2015)
void_t<typename ::std::iterator_traits<T>::iterator_category> > 
#else 
typename void_1<typename ::std::iterator_traits<T>::iterator_category>::type >
#endif
: true_type {};

template<typename T, bool = is_class<T>::value || is_pointer<T>::value>
struct has_iterator_traits : false_type {};

template<typename T>
struct has_iterator_traits<T, true> : has_iterator_traits_impl<T> {};

template<typename T, typename Enable = void>
struct is_iterator
    : false_type {};

// This solution works in C++98
template<typename T>
struct is_iterator<T, typename enable_if<
has_iterator_traits<T>::value 
&& !is_integral<T>::value 
&& !is_arithmetic<T>::value
>::type>
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

#if NH3API_CHECK_CPP11 || NH3API_CHECK_CLANG_CL
namespace details
{
template <typename T, typename = void>
struct is_iterator_class_test
    : false_type
{};

template <typename T>
struct is_iterator_class_test<T, decltype((void)::nh3api::declval<T>().operator->(), void())>
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
    return typename details::iter_category_helper<IterT, tt::is_iterator<IterT>::value>::type();
};

// verify MSVC STL range of iterators

// there was no such macro before VS 2017 15.5,
// I assume there was also no _Adl_verify_range, so that's it. :P
#ifdef _MSVC_STL_UPDATE

// verify range on MSVC STL checked iterators
template <class IterT, class _Sentinel> inline
constexpr void verify_range(const IterT& _First, const _Sentinel& _Last)
{ _STD _Adl_verify_range(_First, _Last); }

template <class IterT, class _Size_type> inline
constexpr void verify_range_n(const IterT& _First, const _Size_type _N)
{
    using diff_t = typename ::std::iterator_traits<IterT>::difference_type;
    auto _Last = next(_First, static_cast<diff_t>(_N));
    verify_range(_First, _Last);
}

// Old MSVC STL
#elif NH3API_MSVC_STL_VERSION && !defined(_MSVC_STL_UPDATE)

// verify range on (old) MSVC STL checked iterators
template <class IterT, class _Sentinel>
inline void verify_range(const IterT& _First, const _Sentinel& _Last)
{
    #if NH3API_DEBUG
    using namespace std; // to make _DEBUG_RANGE macro range work
    _DEBUG_RANGE(_First, _Last);
    #else
    NH3API_IGNORE(_First, _Last);
    #endif
}

template <class IterT, class _Size_type>
inline void verify_range_n(const IterT& _First, _Size_type _N)
{
    #if NH3API_DEBUG
    const IterT& _Last = next(_First, static_cast<typename ::std::iterator_traits<IterT>::difference_type>(_N));
    using namespace std; // to make _DEBUG_RANGE macro range work
    _DEBUG_RANGE(_First, _Last);
    #else
    NH3API_IGNORE(_First, _N);
    #endif
}
#elif NH3API_GCC_STL

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

template <class IterT, class _Sentinel>
NH3API_CONSTEXPR_CPP_14 inline void verify_range(const IterT &_First, const _Sentinel &_Last)
{ (void)_First; (void)_Last; }

template <class IterT, class _Size_type>
NH3API_CONSTEXPR_CPP_14 inline void verify_range_n(const IterT& _First, const _Size_type _N)
{ (void) _First; (void) _N;}

#endif

// unwrap MSVC STL iterators
// https://stackoverflow.com/questions/61440041/why-does-themicrosoft-visual-c-library-implementation-unwrap-iterators
// different versions have different ways to do this, fine...

// vs 2005..2008 checked iterators
#if NH3API_MSVC_STL && (NH3API_MSVC_STL_VERSION > 300 && NH3API_MSVC_STL_VERSION < NH3API_MSVC_STL_VERSION_2010) && defined(_CHECKED_BASE)

template<typename _Iter_type> inline
typename _STD _Checked_iterator_base_helper<_Iter_type>::_Checked_iterator_base_type
unfancy(const _Iter_type& iter) NH3API_NOEXCEPT
{ return _CHECKED_BASE(iter); }

// vs 2010..2015 checked iterators
#elif NH3API_MSVC_STL && (NH3API_MSVC_STL_VERSION >= NH3API_MSVC_STL_VERSION_2010) && !defined(_MSVC_STL_UPDATE)

template<typename T> inline
T* unfancy(T* iter) NH3API_NOEXCEPT
{ return iter; }

template<typename IterT> inline
typename IterT::_Unchecked_type unfancy(const IterT& iter) NH3API_NOEXCEPT
{ return ::std::_Unchecked(iter); }

template<class _Ty, size_t _Size> inline
typename ::std::_Array_iterator<_Ty, _Size>::_Unchecked_type
unfancy(const ::std::_Array_iterator<_Ty, _Size>& IterT) NH3API_NOEXCEPT
{ return (IterT._Unchecked()); }

template<class _Ty, size_t _Size> inline
typename ::std::_Array_const_iterator<_Ty, _Size>::_Unchecked_type
unfancy(const ::std::_Array_const_iterator<_Ty, _Size>& IterT) NH3API_NOEXCEPT
{ return (IterT._Unchecked()); }

#elif defined(_MSVC_STL_UPDATE)

template<class IterT>
_NODISCARD inline constexpr decltype(auto) unfancy(const IterT& _It) noexcept
{ return ::std::_Get_unwrapped(_It); }

template<class IterT>
_NODISCARD inline constexpr decltype(auto) unfancy(const IterT&& _It) noexcept
{ return ::std::_Get_unwrapped(::std::forward<const IterT&&>(_It)); }

#elif NH3API_CLANG_STL

template <class IterT,
          class _Impl = ::std::__unwrap_iter_impl<IterT>,
          ::std::__enable_if_t<::std::is_copy_constructible<IterT>::value, int> = 0>
inline _LIBCPP_CONSTEXPR_SINCE_CXX14 decltype(_Impl::__unwrap(::std::declval<IterT>()))
unfancy(IterT __i) _NOEXCEPT
{ return _Impl::__unwrap(__i); }

#elif NH3API_GCC_STL

template<typename IterT>
IterT unfancy(const IterT& iter) NH3API_NOEXCEPT
{ return iter; }

template<typename IterT>
IterT unfancy(const IterT&& iter) NH3API_NOEXCEPT
{ return iter; }

#else // Unknown STL
template<typename IterT>
IterT unfancy(const IterT& iter) NH3API_NOEXCEPT
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

namespace tt 
{

template<typename IterT, bool IsIterator = is_iterator<IterT>::value>
struct is_random_access_iterator
    : integral_constant<bool, 
    is_same<
    typename ::std::iterator_traits<IterT>::iterator_category, 
    ::std::random_access_iterator_tag>::value>
{};

template<typename IterT>
struct is_random_access_iterator<IterT, false>
    : false_type
{};    

} // namespace nh3api::tt

} // namespace nh3api
