//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// <type_traits> implementation for old MSVC compilers (C++98/C++03)
// MSVC C++6.0 does not support partial template specialization
// https://jeffpar.github.io/kbarchive/kb/240/Q240866/
// Hence, NH3API does not support MSVC C++ 6.0 :(
// implementation mostly by the boost software library https://boost.org/
// std::underlying_type implemented by void_17

#pragma once
#include "nh3api_std.hpp"
#include "type_traits/type_traits_cxx98.hpp"
#include <utility>

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, on)
#endif

#if NH3API_CHECK_CPP11 || NH3API_VS2010
#include <type_traits> // something works since vs2010
#endif

#if NH3API_VS2010_ONLY
namespace nh3api
{

template<class T>
typename add_rvalue_reference<T>::type declval() throw()
{ static_assert(false, "declval not allowed in an evaluated context"); }

namespace tt
{
namespace details
{
struct is_default_constructible_impl
{
   template<typename _Tp, typename>
   static yes_type test(int);

   template<typename>
   static no_type test(...);
};

struct is_constructible_impl
{
   template<typename T, typename Arg, typename>
   static yes_type test1(int);
   template<typename, typename>
   static no_type test1(...);

   template <typename T>
   static yes_type ref_test(T);
   template <typename T>
   static no_type ref_test(...);
};

struct is_destructible_impl
{
   template<typename T, typename>
   static yes_type test(int);
   template<typename>
   static no_type test(...);
};

struct is_assignable_impl
{
   template<typename T, typename U, typename>
   static yes_type test(int);

   template<typename, typename>
   static no_type test(...);
};

} // namespace details

template <class T, class U> struct is_assignable : public integral_constant<bool, sizeof(details::is_assignable_impl::test<T, U, decltype(declval<T>() = declval<U>())>(0)) == sizeof(details::yes_type)>{};
template <class T, size_t N, class U> struct is_assignable<T[N], U> : public is_assignable<T, U>{};
template <class T, size_t N, class U> struct is_assignable<T(&)[N], U> : public is_assignable<T&, U>{};
template <class T, class U> struct is_assignable<T[], U> : public is_assignable<T, U>{};
template <class T, class U> struct is_assignable<T(&)[], U> : public is_assignable<T&, U>{};
template <class U> struct is_assignable<void, U> : public integral_constant<bool, false>{};
template <class U> struct is_assignable<void const, U> : public integral_constant<bool, false>{};
template <class U> struct is_assignable<void volatile, U> : public integral_constant<bool, false>{};
template <class U> struct is_assignable<void const volatile, U> : public integral_constant<bool, false>{};

template <class T> struct is_destructible
: public integral_constant<bool, sizeof(details::is_destructible_impl::test<T, decltype(declval<T&>().~T())>(0)) == sizeof(details::yes_type)>{};

template <class T> struct is_default_constructible
: public integral_constant<bool, sizeof(details::is_default_constructible_impl::test<T, decltype(T())>(0)) == sizeof(details::yes_type)>{};
template <class T, size_t N> struct is_default_constructible<T[N]> : public is_default_constructible<T>{};
template <class T> struct is_default_constructible<T[]> : public is_default_constructible<T>{};
template <class T> struct is_default_constructible<T&> : public integral_constant<bool, false>{};
template <class T> struct is_default_constructible<T&&> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void const> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void volatile> : public integral_constant<bool, false>{};
template <> struct is_default_constructible<void const volatile> : public integral_constant<bool, false>{};

template <class T, class Arg> struct is_constructible<T, Arg> : public integral_constant<bool, is_destructible<T>::value && sizeof(details::is_constructible_impl::test1<T, Arg, decltype(::new T(declval<Arg>()))>(0)) == sizeof(details::yes_type)>{};
template <class Ref, class Arg> struct is_constructible<Ref&, Arg> : public integral_constant<bool, sizeof(details::is_constructible_impl::ref_test<Ref&>(declval<Arg>())) == sizeof(details::yes_type)>{};
template <class Ref, class Arg> struct is_constructible<Ref&&, Arg> : public integral_constant<bool, sizeof(details::is_constructible_impl::ref_test<Ref&&>(declval<Arg>())) == sizeof(details::yes_type)>{};

template <> struct is_constructible<void> : public false_type{};
template <> struct is_constructible<void const> : public false_type{};
template <> struct is_constructible<void const volatile> : public false_type{};
template <> struct is_constructible<void volatile> : public false_type{};

template <class T> struct is_constructible<T> : public is_default_constructible<T>{};

template<class T>
struct is_move_constructible :
    is_constructible<T, typename add_rvalue_reference<T>::type> {};

template<class T>
struct is_trivially_move_constructible :
    is_trivially_constructible<T, typename add_rvalue_reference<T>::type> {};

template<class T>
struct is_nothrow_move_constructible :
    is_nothrow_constructible<T, typename add_rvalue_reference<T>::type> {};

} // namespace tt
} // namespace nh3api

#elif NH3API_CHECK_CPP11 // NH3API_VS2010_ONLY
namespace nh3api
{
using ::std::declval;
} // namespace nh3api
#endif

#if NH3API_VS2012_2013 || NH3API_CHECK_CPP11
namespace nh3api
{
namespace tt
{
using ::std::is_move_constructible;
using ::std::is_trivially_move_constructible;
using ::std::is_trivially_move_assignable;
using ::std::is_nothrow_move_constructible;
using ::std::is_copy_constructible;
using ::std::is_nothrow_constructible;
} // namespace tt
} // namespace nh3api
#endif

#if NH3API_CHECK_CPP11
namespace nh3api
{
namespace tt
{
template<class...>
using void_t = void;

} // namespace tt
} // namespace nh3api
#endif // if NH3API_CHECK_CPP11


namespace nh3api
{
namespace tt
{
#if !defined(__cpp_lib_bounded_array_traits)
template<class T>
struct is_unbounded_array
    : false_type { };

template<class T>
struct is_unbounded_array<T[]>
    : true_type { };

template<class T>
struct is_unbounded_array<const T[]>
    : true_type { };

template<class T>
struct is_unbounded_array<volatile T[]>
    : true_type { };

template<class T>
struct is_unbounded_array<const volatile T[]>
    : true_type { };

template<class T>
struct is_bounded_array
    : false_type { };

template<class T, size_t N>
struct is_bounded_array<T[N]>
    : true_type { };

template<class T, size_t N>
struct is_bounded_array<const T[N]>
    : true_type { };

template<class T, size_t N>
struct is_bounded_array<volatile T[N]>
    : true_type { };

template<class T, size_t N>
struct is_bounded_array<const volatile T[N]>
    : true_type { };
#else
using ::std::is_unbounded_array;
using ::std::is_bounded_array;
#endif
} // namespace tt

template<class T> NH3API_NODISCARD NH3API_MSVC_INTRIN NH3API_CONSTEXPR
typename tt::add_const<T>::type& as_const(T& t) NH3API_NOEXCEPT
{ return t; }

template<typename T> NH3API_NODISCARD NH3API_MSVC_INTRIN NH3API_CONSTEXPR
typename tt::make_unsigned<T>::type to_unsigned(T arg)
{ return static_cast<typename tt::make_unsigned<T>::type>(arg); }

template<class Enum> NH3API_NODISCARD NH3API_MSVC_INTRIN NH3API_CONSTEXPR
typename tt::underlying_type<Enum>::type to_underlying(Enum arg) NH3API_NOEXCEPT
{ return static_cast<typename tt::underlying_type<Enum>::type>(arg); }

} // namespace nh3api


namespace nh3api
{
namespace tt
{
#if NH3API_CHECK_CPP11
template <class T, class = void>
struct has_scalar_deleting_destructor
    : false_type {};

template <class T>
struct has_scalar_deleting_destructor<T,
decltype((void)declval<T>().scalar_deleting_destructor(declval<uint8_t>()))>
    : true_type {};
#else
template <typename type>
class scalar_deleting_destructor_test
{
class yes { char m;};
class no { yes m[2];};

struct base_mixin
{
    void scalar_deleting_destructor(){}
};

struct base : public type, public base_mixin {};

template <typename T, T t>  class helper{};

template <typename U>
static no deduce(U*, helper<void (base_mixin::*)(), &U::scalar_deleting_destructor>*
= 0);
static yes deduce(...);

public:
static const bool result = sizeof(yes) == sizeof(deduce((base*)
(0)));

};

namespace details
{
template <typename type>
class void_exp_result
{};

template <typename type, typename U>
U const& operator,(U const&, void_exp_result<type>);

template <typename type, typename U>
U& operator,(U&, void_exp_result<type>);

template <typename src_type, typename dest_type>
struct clone_constness
{
    typedef dest_type type;
};

template <typename src_type, typename dest_type>
struct clone_constness<const src_type, dest_type>
{
    typedef const dest_type type;
};

}

template <typename type, typename call_details>
struct scalar_deleting_destructor_impl_helper
{
private:
class yes {};
class no { yes m[2]; };

struct derived : public type
{
    using type::scalar_deleting_destructor;
    no scalar_deleting_destructor(...) const;
};

typedef typename details::clone_constness<type, derived>::type
derived_type;

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

template <typename arg1, typename r>
struct deduce_impl<true, r(arg1)>
{
    static const bool value =
    sizeof(
            return_value_check<type, r>::deduce(
            (((derived_type*)0)->scalar_deleting_destructor(*(arg1*)0),
details::void_exp_result<type>())
                        )
            ) == sizeof(yes);

};
public:
static const bool value = deduce_impl<scalar_deleting_destructor_test<type>::result,
call_details>::value;

};

template <class T, bool = is_class<T>::value>
struct has_scalar_deleting_destructor_impl
    : false_type
{
};

template <class T>
struct has_scalar_deleting_destructor_impl<T, true>
    : integral_constant<bool, scalar_deleting_destructor_impl_helper<T, void(unsigned char)>::value>
{
};

template <class T>
struct has_scalar_deleting_destructor
    : integral_constant<bool, has_scalar_deleting_destructor_impl<T>::value>
{};
#endif
} // namespace tt
} // namespace nh3api

#if NH3API_CHECK_MSVC
#pragma component(mintypeinfo, off)
#endif
