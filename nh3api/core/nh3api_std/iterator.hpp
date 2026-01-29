//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <iterator> // std::iterator_traits, std::random_access_iterator_tag
#ifdef __cpp_lib_ranges
#include <ranges>
#endif

#include "type_traits.hpp"

namespace nh3api
{

namespace tt
{

template<class IterT>
using iterator_category_t = typename ::std::iterator_traits<IterT>::iterator_category;

#ifdef __cpp_concepts

template<class IterT>
concept iterator_for_container = requires { typename iterator_category_t<IterT>; };

template<class IterT>
inline constexpr bool is_iterator_v = iterator_for_container<IterT>;

template<class IterT>
struct is_iterator : ::std::bool_constant<iterator_for_container<IterT>> {};

#ifdef __cpp_lib_containers_ranges
template <class _Rng, class _Elem>
concept container_compatible_range =
    (::std::ranges::input_range<_Rng>) && ::std::convertible_to<::std::ranges::range_reference_t<_Rng>, _Elem>;
#endif

#else

template<class IterT, class = void>
inline constexpr bool is_iterator_v = false;

template<class IterT>
inline constexpr bool is_iterator_v<IterT, ::std::void_t<iterator_category_t<IterT>>> = true;

template<class IterT>
struct is_iterator : ::std::bool_constant<is_iterator_v<IterT>> {};

#endif
} // namespace tt

#ifdef __cpp_concepts
template<tt::iterator_for_container IterT> constexpr
tt::iterator_category_t<IterT> iter_category() noexcept
{
    return tt::iterator_category_t<IterT>{};
};
#else
template<typename IterT, std::enable_if_t<tt::is_iterator_v<IterT>, int> = 0> constexpr
tt::iterator_category_t<IterT> iter_category() noexcept
{
    return tt::iterator_category_t<IterT>{};
};
#endif

template <class IterT>
constexpr bool is_cpp17_fwd_iter_v = ::std::is_convertible_v<typename ::std::iterator_traits<IterT>::iterator_category, ::std::forward_iterator_tag>;

#if NH3API_MSVC_STL

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
    (void) _First;
    (void) _N;
    #endif
}
#else

template <class IterT, class _Sentinel>
inline constexpr void verify_range(const IterT &_First, const _Sentinel &_Last)
{ (void)_First; (void)_Last; }

template <class IterT, class _Size_type>
inline constexpr void verify_range_n(const IterT& _First, const _Size_type _N)
{ (void) _First; (void) _N;}

#endif

// unwrap MSVC STL iterators
// https://stackoverflow.com/questions/61440041/why-does-themicrosoft-visual-c-library-implementation-unwrap-iterators

#if NH3API_MSVC_STL

template<class IterT>
[[nodiscard]] inline constexpr decltype(auto) unfancy(const IterT& _It) noexcept
{ return ::std::_Get_unwrapped(_It); }

template<class IterT>
[[nodiscard]] inline constexpr decltype(auto) unfancy(const IterT&& _It) noexcept
{ return ::std::_Get_unwrapped(::std::forward<const IterT&&>(_It)); }

#elif NH3API_CLANG_STL

template <class IterT,
          class _Impl = ::std::__unwrap_iter_impl<IterT>,
          ::std::__enable_if_t<::std::is_copy_constructible<IterT>::value, int> = 0>
inline constexpr decltype(_Impl::__unwrap(::std::declval<IterT>()))
unfancy(IterT __i) noexcept
{ return _Impl::__unwrap(__i); }

#elif NH3API_GCC_STL

template<typename IterT>
IterT unfancy(const IterT& iter) noexcept
{ return iter; }

template<typename IterT>
IterT unfancy(const IterT&& iter) noexcept
{ return iter; }

#else // Unknown STL
template<typename IterT>
IterT unfancy(const IterT& iter) noexcept
{ return iter; }

#endif

#ifdef __cpp_lib_ranges
#if NH3API_MSVC_STL
inline constexpr ::std::ranges::_Unchecked_begin::_Cpo unfancy_begin;
inline constexpr ::std::ranges::_Unchecked_end::_Cpo   unfancy_end;
#elif NH3API_CLANG_STL
inline constexpr auto unfancy_begin = ::std::ranges::__begin::__fn{};
inline constexpr auto unfancy_end   = ::std::ranges::__end::__fn{};
#elif NH3API_GCC_STL
#if _GLIBCXX_RELEASE > 13
inline constexpr ::std::ranges::__access::_Begin unfancy_begin{};
inline constexpr ::std::ranges::__access::_End   unfancy_end{};
#else // _GLIBCXX_RELEASE > 13
inline constexpr ::std::ranges::__cust_access::_Begin unfancy_begin{};
inline constexpr ::std::ranges::__cust_access::_End   unfancy_end{};
#endif // _GLIBCXX_RELEASE > 13
#endif
#endif // __cpp_lib_ranges

} // namespace nh3api
