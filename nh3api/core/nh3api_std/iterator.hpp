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

#ifdef __cpp_lib_concepts
template <class ContainerType>
concept container = requires(ContainerType a, const ContainerType b)
{
  requires ::std::regular<ContainerType>;
  requires ::std::swappable<ContainerType>;
  requires ::std::destructible<typename ContainerType::value_type>;
  requires ::std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
  requires ::std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
  requires ::std::forward_iterator<typename ContainerType::iterator>;
  requires ::std::forward_iterator<typename ContainerType::const_iterator>;
  requires ::std::signed_integral<typename ContainerType::difference_type>;
  requires ::std::same_as<typename ContainerType::difference_type, typename ::std::iterator_traits<typename ContainerType::iterator>::difference_type>;
  requires ::std::same_as<typename ContainerType::difference_type, typename ::std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
  { a.begin() } -> ::std::same_as<typename ContainerType::iterator>;
  { a.end() } -> ::std::same_as<typename ContainerType::iterator>;
  { b.begin() } -> ::std::same_as<typename ContainerType::const_iterator>;
  { b.end() } -> ::std::same_as<typename ContainerType::const_iterator>;
  { a.cbegin() } -> ::std::same_as<typename ContainerType::const_iterator>;
  { a.cend() } -> ::std::same_as<typename ContainerType::const_iterator>;
  { a.size() } -> ::std::same_as<typename ContainerType::size_type>;
  { a.empty() } -> ::std::same_as<bool>;
};
#endif

#else

template<class IterT, class = void>
inline constexpr bool is_iterator_v = false;

template<class IterT>
inline constexpr bool is_iterator_v<IterT, ::std::void_t<iterator_category_t<IterT>>> = true;

template<class IterT>
struct is_iterator : ::std::bool_constant<is_iterator_v<IterT>> {};

#endif
} // namespace nh3api::tt

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
constexpr inline void verify_range(const IterT &_First, const _Sentinel &_Last)
{ (void)_First; (void)_Last; }

template <class IterT, class _Size_type>
constexpr inline void verify_range_n(const IterT& _First, const _Size_type _N)
{ (void) _First; (void) _N;}

#endif

// unwrap MSVC STL iterators
// https://stackoverflow.com/questions/61440041/why-does-themicrosoft-visual-c-library-implementation-unwrap-iterators

#ifdef _MSVC_STL_UPDATE

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

#ifdef __cpp_concepts
template<tt::iterator_for_container PtrOrIterator>
#else 
template<typename PtrOrIterator, std::enable_if_t<tt::is_iterator_v<PtrOrIterator>, int> = 0>
#endif
[[nodiscard]] void* voidify(PtrOrIterator arg) noexcept
{
    if constexpr (::std::is_pointer_v<PtrOrIterator>)
    {
        return arg;
    }
    else  
    {
        return __builtin_addressof(*arg);
    }
}

// typedefs to comply with the C++ standard
// iterator typedefs used by algorithms and other stuff
template<typename _ValueType, typename _DiffType, typename _IterTag>
struct container_iterator
{
    public:
        using iterator_category = _IterTag;
        using value_type = _ValueType;
        using difference_type = _DiffType;
        using pointer = value_type*;
        using reference = value_type&;

};

} // namespace nh3api
