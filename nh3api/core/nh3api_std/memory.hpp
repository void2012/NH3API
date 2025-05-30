//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <new> // std::nothrow_t
#include <memory> // std::allocator_traits, std::unique_ptr

#include "intrin.hpp"
#include "patcher_x86.hpp"
#include "type_traits.hpp" // tt::has_scalar_deleting_destructor
#include "stl_extras.hpp" 

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

// maximum allocated size per call: 520177 bytes

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// address: 0x617492
// Heroes3.exe internal operator delete /
// Внутренняя реализация CRT-функции operator new Heroes3.exe.
void* __cdecl exe_new(size_t size) NH3API_NOEXCEPT
{ return CDECL_1(void*, 0x617492, size); }

NH3API_FORCEINLINE
// address: 0x60B0F0
// Heroes3.exe internal operator delete /
// Внутренняя реализация CRT-функции operator delete Heroes3.exe.
void __cdecl exe_delete(void* ptr) NH3API_NOEXCEPT
{ CDECL_1(void, 0x60B0F0, ptr); }

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// address: 0x61A9D5
// Heroes3.exe internal malloc /
// Внутренняя реализация CRT-функции malloc Heroes3.exe.
void* __cdecl exe_malloc(size_t size) NH3API_NOEXCEPT
{ return CDECL_1(void*, 0x61A9D5, size); }

NH3API_FORCEINLINE
// address: 0x61A9D5
// Heroes3.exe internal free /
// Внутренняя реализация CRT-функции free Heroes3.exe.
void __cdecl exe_free(void* ptr) NH3API_NOEXCEPT
{ CDECL_1(void, 0x619BB0, ptr); }

NH3API_NODISCARD
NH3API_FORCEINLINE
// address: 0x619890
// Heroes3.exe internal realloc /
// Внутренняя реализация CRT-функции realloc Heroes3.exe.
void* __cdecl exe_realloc(void* ptr, size_t newSize) NH3API_NOEXCEPT
{ return CDECL_2(void*, 0x619890, ptr, newSize); }

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1, 2)
// address: 0x61AA61
// Heroes3.exe internal calloc /
// Внутренняя реализация CRT-функции calloc Heroes3.exe.
void* __cdecl exe_calloc(size_t numOfElements, size_t sizeOfElements) NH3API_NOEXCEPT
{ return CDECL_2(void*, 0x61AA61, numOfElements, sizeOfElements); }

NH3API_NODISCARD
NH3API_FORCEINLINE
// address: 0x61E504
// Heroes3.exe internal _msize /
// Внутренняя реализация CRT-функции _msize Heroes3.exe.
int32_t __cdecl exe_msize(const void* ptr) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x61E504, ptr); }

// exe_heap flag passed to the placement new form to allocate via the exe_new /
// exe_heap флаг, который можно передать в placement new для выделения памяти с помощью exe_new.
struct exe_heap_t
{
    static const uintptr_t handle_address = 0x6ABD60;
    static HANDLE& get_handle()
    { return get_global_var_ref(handle_address, HANDLE); }

} const exe_heap; // exe_heap constant / константа exe_heap

// I tried to make new throw exception on invalid allocation
// but there is simply no way to check it with VC6 runtime...
// So I leave the standard C++ definitions with std::nothrow_t
// doing the same thing as without it.

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// usage: new (exe_heap) new-initializer...
void* __cdecl operator new(size_t size, const exe_heap_t&)
{ return exe_new(size); }

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// usage: new (exe_heap, std::nothrow) new-initializer...
void* __cdecl operator new(size_t size, const exe_heap_t&, const std::nothrow_t&) NH3API_NOEXCEPT
{ return exe_new(size); }

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// usage: new (exe_heap) new-initializer...
// NOTE: It appears that both Itanium ABI and MSVC ABI implicitly allocate size + 4,
// and return exe_new(size) + 4 pointer, so we don't do manual handling
void* __cdecl operator new[](size_t size, const exe_heap_t&)
{ return exe_new(size); }

NH3API_NODISCARD
NH3API_FORCEINLINE
NH3API_MALLOC(1)
// usage: new (exe_heap, std::nothrow) new-initializer...
// NOTE: It appears that both Itanium ABI and MSVC ABI implicitly allocate size + 4,
// and return exe_new(size) + 4 pointer, so we don't do manual handling
void* __cdecl operator new[](size_t size, const exe_heap_t&, const std::nothrow_t&) NH3API_NOEXCEPT
{ return exe_new(size); }

/*
// what I mean is that if both ABIs didn't do that, we would have to manually handle that:
void* __cdecl operator new[](size_t size, const exe_heap_t&)
{ 
    int32_t* ptr = reinterpret_cast<int32_t*>(exe_new(size + sizeof(int32_t))); 
    *ptr = size;
    return reinterpret_cast<void*>(ptr + 4);
}
*/

NH3API_FORCEINLINE
// added for the parity
void __cdecl operator delete(void* ptr, const exe_heap_t&) NH3API_NOEXCEPT
{ exe_delete(ptr); }

NH3API_FORCEINLINE
// added for the parity
void __cdecl operator delete[](void* ptr, const exe_heap_t&) NH3API_NOEXCEPT
{ exe_delete(ptr); }

NH3API_FORCEINLINE
// added for the parity
void __cdecl operator delete(void* ptr, const exe_heap_t&, const std::nothrow_t&) NH3API_NOEXCEPT
{ exe_delete(ptr); }

NH3API_FORCEINLINE
// added for the parity
void __cdecl operator delete[](void* ptr, const exe_heap_t&, const std::nothrow_t&) NH3API_NOEXCEPT
{ exe_delete(ptr); }

namespace nh3api
{

// we can't obtain the pointer to the default constructor of type directly
// we also need to pass the __thiscall constructor to exe_vector_constructor_iterator call
// MSVC can't use __thiscall outside of class, use __fastcall + unused 2nd parameter trick
template <typename T>
void __fastcall construct_indirect(T* ptr, uint32_t unused = 0)
{
    (void) unused;
    ::new (const_cast<void*>(static_cast<const volatile void*>(
                ::nh3api::addressof(*ptr)))) T; // invoke default constructor
}

// we can't obtain the pointer to the destructor of type directly
// we also need to pass the __thiscall destructor to exe_vector_destructor_iterator call
// MSVC can't use __thiscall outside of class, use __fastcall + unused 2nd parameter trick
template <typename T>
void __fastcall destroy_indirect(T* ptr, uint32_t unused = 0)
{
    (void) unused;
    ptr->~T(); // invoke destructor
}

} // namespace nh3api

// address: 0x6181FA
// Heroes3.exe internal vector constructor iterator /
// Внутренняя реализация CRT-функции vector constructor iterator Heroes3.exe.
template<typename T> void __stdcall exe_vector_constructor_iterator(T* array_start,
                                                                    size_t size,
                                                                    size_t count,
                                                                    void (__thiscall* init)(T*))
{ STDCALL_4(void, 0x6181FA, array_start, size, count, init); }

template<typename T> void __stdcall exe_vector_constructor_iterator(T* array_start,
                                                                    size_t size,
                                                                    size_t count)
{ STDCALL_4(void, 0x6181FA, array_start, size, count, reinterpret_cast<void (__thiscall*)(T*)>(&nh3api::construct_indirect<T>)); }

// address: 0x6181FA
// Heroes3.exe internal vector destructor iterator /
// Внутренняя реализация CRT-функции vector destructor iterator Heroes3.exe.
template<typename T> void __stdcall exe_vector_destructor_iterator(T* array_start,
                                                                   size_t size,
                                                                   size_t count,
                                                                   void (__thiscall* destr)(T*))
{ STDCALL_4(void, 0x61827C, array_start, size, count, destr); }

// address: 0x6181FA
// Heroes3.exe internal vector destructor iterator /
// Внутренняя реализация CRT-функции vector destructor iterator Heroes3.exe.
template<typename T> void __stdcall exe_vector_destructor_iterator(T* array_start,
                                                                   size_t size,
                                                                   size_t count)
{ STDCALL_4(void, 0x61827C, array_start, size, count, reinterpret_cast<void (__thiscall*)(T*)>(&nh3api::destroy_indirect<T>)); }

namespace nh3api
{
namespace details
{
template<typename T, bool has_deleting_destructor = tt::has_scalar_deleting_destructor<T>::value>
struct NH3API_NODEBUG exe_invoke_delete_helper
{
    NH3API_STATIC_ASSERT("exe_invoke_delete: can't invoke destructor on void*",
                         !::nh3api::tt::is_void<T>::value);

    NH3API_FORCEINLINE
    static void do_destroy_delete(T* ptr) NH3API_NOEXCEPT
    {
        ::nh3api::destroy_at(ptr);
        exe_delete(ptr);
    }
};

template<typename T>
struct NH3API_NODEBUG exe_invoke_delete_helper<T, true>
{
    NH3API_FORCEINLINE
    // invoke virtual destructor on polymorphic objects
    static void do_destroy_delete(T* ptr) NH3API_NOEXCEPT
    { ptr->scalar_deleting_destructor(1); }
};

} // namespace details

} // namespace nh3api

// invoke delete and destroy (use this instead of plain exe_delete)
template<typename T> NH3API_FORCEINLINE
void exe_invoke_delete(T* ptr) NH3API_NOEXCEPT
{ nh3api::details::exe_invoke_delete_helper<T>::do_destroy_delete(ptr); }

// overload for void*
template<> NH3API_FORCEINLINE
void exe_invoke_delete<void>(void* ptr) NH3API_NOEXCEPT
{  exe_delete(ptr); }

namespace nh3api
{
namespace details
{
template<typename T, bool has_deleting_destructor = tt::has_scalar_deleting_destructor<T>::value>
struct NH3API_NODEBUG exe_invoke_destructor_helper
{
    NH3API_STATIC_ASSERT("exe_invoke_destructor: can't invoke destructor on void*",
                         !::nh3api::tt::is_void<T>::value);

    NH3API_FORCEINLINE
    static void do_destroy(T* ptr) NH3API_NOEXCEPT
    { ::nh3api::destroy_at(ptr); }
};

template<typename T>
struct NH3API_NODEBUG exe_invoke_destructor_helper<T, true>
{
    NH3API_FORCEINLINE
    // invoke virtual destructor on polymorphic objects
    static void do_destroy(T* ptr) NH3API_NOEXCEPT
    { ptr->scalar_deleting_destructor(0); }
};

} // namespace nh3api::details
} // namespace nh3api

// invoke destructor(plain or scalar_deleting_destructor)
template<typename T> NH3API_FORCEINLINE
void exe_invoke_destructor(T* ptr) NH3API_NOEXCEPT
{ nh3api::details::exe_invoke_destructor_helper<T>::do_destroy(ptr); }

namespace nh3api
{
namespace details
{
template<typename T, bool trivially_destructible = tt::is_trivially_destructible<T>::value>
struct NH3API_NODEBUG exe_invoke_array_delete_helper
{
    NH3API_FORCEINLINE
    static void do_destroy_delete(T* ptr) NH3API_NOEXCEPT
    {
        int32_t* pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        const int32_t array_size = *pre_ptr;
        for ( int32_t i = 0; i < array_size; ++i )
            exe_invoke_destructor(::nh3api::addressof(ptr[i]));

        exe_delete(pre_ptr);
    }
};

template<typename T>
struct NH3API_NODEBUG exe_invoke_array_delete_helper<T, true>
{
    NH3API_FORCEINLINE
    static void do_destroy_delete(T* ptr) NH3API_NOEXCEPT
    {
        int32_t* pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        exe_delete(pre_ptr);
    }
};

} // namespace nh3api::details
} // namespace nh3api

// invoke array form of delete. Use it instead of plain exe_delete
template<typename T> NH3API_FORCEINLINE
void exe_invoke_array_delete(T* ptr) NH3API_NOEXCEPT
{ nh3api::details::exe_invoke_array_delete_helper<T>::do_destroy_delete(ptr); }

// overload for void*
template<> NH3API_FORCEINLINE
void exe_invoke_array_delete<void>(void* ptr) NH3API_NOEXCEPT
{
    int32_t* pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
    exe_delete(pre_ptr);
}

namespace nh3api
{
// MSVC ABI implementation of virtual destructor
// lies at the [0] of type's vftable
// flag == true if: 'delete ptr'(explicitly deleted)
// note that 'delete' and '::operator delete' are different:
// on polymorphic objects with virtual destructor call to 'delete'
// invokes virtual destructor, while call to
// '::operator delete' deallocates memory for that object
// so this is how MSVC solves this problem.
template<typename T>
void scalar_deleting_destructor(T* ptr, uint8_t flag)
{
    destroy_at(ptr);
    if ( flag )
        exe_delete(ptr);
}

// MSVC ABI implementation of virtual destructor
// called via operator delete[] array form
// MSVC operator new[] stores the size of the array
// right before the allocated block.
template<typename T>
void vector_deleting_destructor(T* ptr, uint32_t flag)
{
    NH3API_STATIC_ASSERT("vector_deleting_destructor<T>: T must be a class",
                         ::nh3api::tt::is_class<T>::value);

    // operator delete[]
    if ( (flag & 2) != 0 )
    {
        int32_t* pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        const int32_t allocated_size = *pre_ptr;
        // if ( allocated_size != -1 )
            exe_vector_destructor_iterator(ptr, sizeof(T), allocated_size);
        if ( (flag & 1 ) != 0 )
            exe_delete(pre_ptr);
    }
    else // operator delete
    {
        destroy_at(ptr);
        if ( (flag & 1 ) != 0 )
            exe_delete(ptr);
    }
}

} // namespace nh3api

#ifndef NH3API_SCALAR_DELETING_DESTRUCTOR
#define NH3API_SCALAR_DELETING_DESTRUCTOR virtual void __thiscall scalar_deleting_destructor(uint8_t flag) \
                                          { get_vftable(this)->scalar_deleting_destructor(this, flag); }
#endif

// requires T::vftable_t
template<class T>
typename T::vftable_t* get_vftable(T* ptr)
{ return *reinterpret_cast<typename T::vftable_t**>(ptr); }

// requires T::vftable_t
template<class T>
const typename T::vftable_t* get_vftable(const T* ptr)
{ return *reinterpret_cast<const typename T::vftable_t* const*>(ptr); }

// template specializations provided in vftables.hpp
template<class T>
struct vftable_address
{
    static const uintptr_t address = 0;
};

template<typename T> NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable()
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

template<typename T> NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable(const T*)
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

// for constructors
template<typename T>
void set_vftable(T* ptr)
{
    NH3API_MEMSHIELD_BEGIN
    //#ifdef __cpp_lib_launder
    //*reinterpret_cast<void**>(std::launder(ptr)) = get_type_vftable(ptr);
    //#else
    *reinterpret_cast<const void**>(ptr) = get_type_vftable(ptr);
    //#endif
    NH3API_MEMSHIELD_END
}

#ifndef NH3API_SET_VFTABLE
    #define NH3API_SET_VFTABLE() NH3API_MEMSHIELD_BEGIN set_vftable(this); NH3API_MEMSHIELD_END
#endif

#if NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT
// low-level virtual call with only shift in vftable
template<typename Result, typename T, typename ... Args> inline
Result virtual_call(T* thisPtr, size_t shift, Args&& ... args)
{ return THISCALL_N(Result, reinterpret_cast<uintptr_t>(get_vftable(thisPtr)) + shift, ::std::forward<Args>(args)...); }
#endif

// allocator which uses internal .exe operator new and operator delete /
// allocator, использующий внутренние операторы new и delete .exe для управления памятью.
template<class T>
class exe_allocator
{
protected:
    NH3API_STATIC_ASSERT("The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed."
                         "because allocator<const T> is ill-formed.", !nh3api::tt::is_const<T>::value);
    NH3API_STATIC_ASSERT("The C++ Standard forbids allocators for function elements "
                         "because of [allocator.requirements].", !nh3api::tt::is_function<T>::value);
    NH3API_STATIC_ASSERT("The C++ Standard forbids allocators for reference elements "
                         "because of [allocator.requirements].", !nh3api::tt::is_reference<T>::value);

public:
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
    typedef nh3api::tt::false_type propagate_on_container_move_assignment;
    typedef nh3api::tt::false_type propagate_on_container_swap;
    typedef nh3api::tt::true_type  is_always_equal;

    template<class U>
    struct rebind
    { typedef exe_allocator<U> other; };

public:
    NH3API_NODISCARD NH3API_CONSTEXPR NH3API_FORCEINLINE
    pointer address(reference x) const NH3API_NOEXCEPT
    { return ::nh3api::addressof(x); }

    NH3API_NODISCARD NH3API_CONSTEXPR NH3API_FORCEINLINE
    const_pointer address(const_reference x) const NH3API_NOEXCEPT
    { return ::nh3api::addressof(x); }

    NH3API_NODISCARD NH3API_FORCEINLINE
    pointer allocate(size_type size) NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
    { 
        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
        return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap));
        #else
        return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap, ::std::nothrow));
        #endif
    }

    NH3API_NODISCARD NH3API_FORCEINLINE
    pointer allocate(size_type size, const void*) NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
    {
        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
        return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap));
        #else
        return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap, ::std::nothrow));
        #endif
    }

    NH3API_FORCEINLINE
    void deallocate(void* ptr, size_type) NH3API_NOEXCEPT
    { exe_delete(ptr); }

    #if NH3API_STD_MOVE_SEMANTICS
    template <typename U, typename ... Args 
    #if NH3API_CHECK_CPP11
    , class = decltype(::new(::std::declval<void*>()) U(::std::declval<Args>()...))
    #endif
    > NH3API_FORCEINLINE
    void construct(U* ptr, Args&& ... args)
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_constructible<U, Args ...>::value)
    { ::new ((void*)ptr) U(::std::forward<Args>(args) ... ); }
    #else
    template <typename U> NH3API_FORCEINLINE
    void construct(U* ptr, const_reference copy_value)
    { ::new (static_cast<void*>(ptr)) U(copy_value); }
    #endif

    NH3API_FORCEINLINE
    void destroy(pointer ptr)
    NH3API_NOEXCEPT_EXPR(::nh3api::tt::is_nothrow_destructible<value_type>::value)
    { ptr->~value_type(); }

    NH3API_NODISCARD NH3API_FORCEINLINE NH3API_CONSTEXPR
    size_t max_size() const NH3API_NOEXCEPT
    { return size_type(~0) / sizeof(value_type); }

    NH3API_NODISCARD NH3API_FORCEINLINE
    const exe_allocator& select_on_container_copy_construction() const
    { return *this; }
};

template<class T, class U> NH3API_CONSTEXPR
bool operator==(const exe_allocator<T>&, const exe_allocator<U>&)
{ return true; }

template<class T, class U> NH3API_CONSTEXPR
bool operator!=(const exe_allocator<T>&, const exe_allocator<U>&)
{ return false; }

namespace nh3api
{

#if NH3API_STD_MOVE_SEMANTICS
template <class Allocator, class U, typename ... Args
#if NH3API_CHECK_CPP11
, class = decltype(::new(::std::declval<void*>()) U(::std::declval<Args>()...))
#endif
> NH3API_FORCEINLINE
void variadic_construct(Allocator a, U* ptr, Args&& ... args)
{
    #if NH3API_CHECK_CPP11
    ::std::allocator_traits<Allocator>::construct(a, ptr, ::std::forward<Args>(args) ... );
    #else
    alloc.construct(ptr, ::std::forward<Args>(args) ...);
    #endif
}

template <typename U, typename T, typename ... Args 
#if NH3API_CHECK_CPP11
, class = decltype(::new(::std::declval<void*>()) U(::std::declval<Args>()...))
#endif
> NH3API_FORCEINLINE
void variadic_construct(const ::exe_allocator<T>&, U* ptr, Args&& ... args)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_constructible<U, Args ...>::value)
{ ::new ((void*)ptr) U(::std::forward<Args>(args) ... ); }
#endif

template<class T, class Allocator> NH3API_FORCEINLINE
void copy_construct(T* ptr, const T& value, const Allocator& alloc)
{
    #if NH3API_CHECK_CPP11
    ::std::allocator_traits<Allocator>::construct(alloc, ptr, value);
    #else
    alloc.construct(ptr, value);
    #endif
}

template<class U, class T> NH3API_FORCEINLINE 
#if __cpp_lib_constexpr_new
constexpr
#endif
void copy_construct(U* ptr, const U& value, const ::exe_allocator<T>&)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_copy_constructible<U>::value)
{ ::new (static_cast<void*>(ptr)) U(value); }

#if NH3API_STD_MOVE_SEMANTICS
template<class T, class Allocator> NH3API_FORCEINLINE
void move_construct(T* ptr, T&& rvalue, const Allocator& alloc)
{
    #if NH3API_CHECK_CPP11
    ::std::allocator_traits<Allocator>::construct(alloc, ptr, rvalue);
    #else
    alloc.construct(ptr, value);
    #endif
}

template<class U, class T> NH3API_FORCEINLINE 
#if __cpp_lib_constexpr_new
constexpr
#endif
void move_construct(U* ptr, U&& rvalue, const ::exe_allocator<T>&)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_move_constructible<U>::value)
{
    ::new (static_cast<void*>(ptr)) U(::std::forward<U>(rvalue));
}
#endif

template<typename T, class Allocator> NH3API_FORCEINLINE
void default_construct(T* ptr, const Allocator& alloc)
{
    #if NH3API_CHECK_CPP11
    ::std::allocator_traits<Allocator>::construct(alloc, ptr, T());
    #else
    alloc.construct(ptr, T());
    #endif
}

template<typename U, class T> NH3API_FORCEINLINE
#if __cpp_lib_constexpr_new
constexpr
#endif
void default_construct(U* ptr, const ::exe_allocator<T>&)
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_constructible<T>::value)
{ ::new (static_cast<void*>(ptr)) U(); }

template<class Allocator>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
Allocator allocator_select_on_container_copy_construction(const Allocator& a)
{
    #if NH3API_CHECK_CPP11
        return ::std::allocator_traits<Allocator>::select_on_container_copy_construction(a);
    #else 
        return a;
    #endif
}

template<class T>
NH3API_FORCEINLINE NH3API_CONSTEXPR
exe_allocator<T> allocator_select_on_container_copy_construction(const exe_allocator<T>& a)
{ return a; }

template<class Allocator>
NH3API_FORCEINLINE
typename Allocator::pointer allocate( Allocator& a, typename Allocator::size_type n )
{
    #if NH3API_CHECK_CPP11
        return ::std::allocator_traits<Allocator>::allocate(a, n);
    #else 
        a.allocate(n);
    #endif
}

template<class T>
NH3API_FORCEINLINE
T* allocate( const exe_allocator<T>&, size_t n ) 
NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
{
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    return static_cast<T*>(::operator new(n * sizeof(T), exe_heap));
    #else
    return static_cast<T*>(::operator new(n * sizeof(T), exe_heap, ::std::nothrow));
    #endif
}

template<class Allocator>
NH3API_FORCEINLINE
typename Allocator::pointer allocate( Allocator& a, typename Allocator::size_type n, const void* hint )
{
    #if NH3API_CHECK_CPP11
        return ::std::allocator_traits<Allocator>::allocate(a, n, hint);
    #else 
        a.allocate(n, hint);
    #endif
}

template<class T>
NH3API_FORCEINLINE
T* allocate( const exe_allocator<T>&, size_t n, const void* ) 
NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
{
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    return static_cast<T*>(::operator new(n * sizeof(T), ::exe_heap));
    #else
    return static_cast<T*>(::operator new(n * sizeof(T), ::exe_heap, ::std::nothrow));
    #endif
}

template<typename U, class Allocator> NH3API_FORCEINLINE
#ifdef __cpp_decltype_auto	
decltype(auto)
#else 
typename Allocator::template rebind<U>::other::pointer
#endif
allocate_rebind(Allocator& a, typename Allocator::size_type n)
{
    #if NH3API_CHECK_CPP11
        typedef typename ::std::allocator_traits<Allocator>::template rebind_alloc<U> rebound_alloc;
        rebound_alloc rebound(a);
        return ::std::allocator_traits<rebound_alloc>::allocate(rebound, size);
    #else 
        typename Allocator::template rebind<U>::other rebound(a);
        rebound.allocate(n);
    #endif
}

template<typename U, class T> NH3API_FORCEINLINE
U* allocate_rebind(::exe_allocator<T>&, size_t n)
NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
{
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    return static_cast<U*>(::operator new(n * sizeof(U), ::exe_heap));
    #else
    return static_cast<U*>(::operator new(n * sizeof(U), ::exe_heap, ::std::nothrow));
    #endif
}

template<typename U, class Allocator> NH3API_FORCEINLINE
#ifdef __cpp_decltype_auto	
decltype(auto)
#else 
typename Allocator::template rebind<U>::other::pointer
#endif
allocate_rebind(Allocator& a, typename Allocator::size_type n, const void* hint)
{
    #if NH3API_CHECK_CPP11
        typedef typename ::std::allocator_traits<Allocator>::template rebind_alloc<U> rebound_alloc;
        rebound_alloc rebound(a);
        return ::std::allocator_traits<rebound_alloc>::allocate(rebound, size, hint);
    #else 
        typename Allocator::template rebind<U>::other rebound(a);
        rebound.allocate(n, hint);
    #endif
}

template<typename U, class T> NH3API_FORCEINLINE
U* allocate_rebind(::exe_allocator<T>&, size_t n, const void*)
NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
{
    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    return static_cast<U*>(::operator new(n * sizeof(U), ::exe_heap));
    #else
    return static_cast<U*>(::operator new(n * sizeof(U), ::exe_heap, ::std::nothrow));
    #endif
}

template<class Allocator> NH3API_FORCEINLINE
void deallocate( Allocator& a, typename Allocator::pointer p, typename Allocator::size_type n )
{
    #if NH3API_CHECK_CPP11
        ::std::allocator_traits<Allocator>::deallocate(a, p, n);
    #else 
        a.deallocate(p, n);
    #endif
}

template<class T> NH3API_FORCEINLINE
void deallocate( ::exe_allocator<T>&, T* p, size_t ) NH3API_NOEXCEPT
{ ::operator delete(p, ::exe_heap); }

template<class U, class Allocator> NH3API_FORCEINLINE
void deallocate_rebind(Allocator& a, U* p, typename Allocator::size_type n )
{
    #if NH3API_CHECK_CPP11
        typedef typename ::std::allocator_traits<Allocator>::template rebind_alloc<U> rebound_alloc;
        rebound_alloc rebound(a);
        return ::std::allocator_traits<rebound_alloc>::deallocate(a, p, n);
    #else 
        typename Allocator::template rebind<U>::other rebound(a);
        rebound.deallocate(p, n);
    #endif
}

template<class U, class T> NH3API_FORCEINLINE
void deallocate_rebind(::exe_allocator<T>&, U* p, size_t ) NH3API_NOEXCEPT
{ ::operator delete(p, ::exe_heap); }

template<class Allocator, class T> NH3API_FORCEINLINE
void allocator_destroy( Allocator& a, T* p )
{
    #if NH3API_CHECK_CPP11
        ::std::allocator_traits<Allocator>::destroy(a, p);
    #else 
        a.deallocate(p, n);
    #endif
}

template<class U, class T> NH3API_FORCEINLINE
void allocator_destroy( ::exe_allocator<T>&, U* p ) 
NH3API_NOEXCEPT_EXPR(tt::is_nothrow_destructible<U>::value)
{ p->~U(); }

template<class Allocator> NH3API_FORCEINLINE
typename Allocator::size_type allocator_max_size(const Allocator& a) 
{
    #if NH3API_CHECK_CPP11
        return ::std::allocator_traits<Allocator>::max_size(a);
    #else 
        return a.max_size();
    #endif
}

template<class T> NH3API_FORCEINLINE NH3API_CONSTEXPR
size_t allocator_max_size(const ::exe_allocator<T>&) NH3API_NOEXCEPT
{ return size_t(~0) / sizeof(T); }

} // namespace nh3api

namespace nh3api
{

namespace tt
{
#if NH3API_CHECK_CPP11
template<typename Allocator>
struct NH3API_NODEBUG allocator_may_throw
    : public integral_constant<bool,
    !noexcept(declval<Allocator>().allocate( declval<typename Allocator::size_type>() ))>
{};
#else
template<typename Allocator>
struct NH3API_NODEBUG allocator_may_throw
    : public integral_constant<bool, true>
{};
#endif

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
template<typename T>
struct NH3API_NODEBUG allocator_may_throw<exe_allocator<T> >
    : public true_type
{};
#else
template<typename T>
struct NH3API_NODEBUG allocator_may_throw<exe_allocator<T> >
    : public false_type
{};
#endif

template<typename Allocator> 
struct allocator_container_traits
{
    #if NH3API_CHECK_CPP11
    typedef typename ::std::allocator_traits<Allocator>::propagate_on_container_copy_assignment 
    propagate_on_container_copy_assignment;
    typedef typename ::std::allocator_traits<Allocator>::propagate_on_container_move_assignment
    propagate_on_container_move_assignment;
    typedef typename ::std::allocator_traits<Allocator>::propagate_on_container_swap
    propagate_on_container_swap;
    typedef typename ::std::allocator_traits<Allocator>::is_always_equal
    is_always_equal;

    typedef typename ::std::allocator_traits<Allocator>::size_type       size_type;
    typedef typename ::std::allocator_traits<Allocator>::difference_type difference_type;
    typedef typename ::std::allocator_traits<Allocator>::value_type      value_type;
    typedef typename ::std::allocator_traits<Allocator>::pointer         pointer;
    typedef typename ::std::allocator_traits<Allocator>::const_pointer   const_pointer;
    typedef typename ::std::allocator_traits<Allocator>::reference       reference;
    typedef typename ::std::allocator_traits<Allocator>::const_reference const_reference;
    #else 
    typedef true_type  propagate_on_container_copy_assignment;
    typedef true_type  propagate_on_container_move_assignment;
    typedef true_type  propagate_on_container_swap;
    typedef false_type is_always_equal;

    typedef typename Allocator::size_type       size_type;
    typedef typename Allocator::difference_type difference_type;
    typedef typename Allocator::value_type      value_type;
    typedef typename Allocator::pointer         pointer;
    typedef typename Allocator::const_pointer   const_pointer;
    typedef typename Allocator::reference       reference;
    typedef typename Allocator::const_reference const_reference;
    #endif
};

template<typename T> 
struct allocator_container_traits<::exe_allocator<T> >
{
    typedef false_type propagate_on_container_copy_assignment;
    typedef false_type propagate_on_container_move_assignment;
    typedef false_type propagate_on_container_swap;
    typedef true_type  is_always_equal;

    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
};

#if NH3API_CHECK_CPP11
template<typename Allocator>
struct NH3API_NODEBUG allocator_always_equal_after_move
    : integral_constant<bool,
    ::std::allocator_traits<Allocator>::is_always_equal::value && 
    ::std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value>
{};

template<typename T>
struct NH3API_NODEBUG allocator_always_equal_after_move<exe_allocator<T> >
    : true_type
{};
#endif



} // namespace tt
} // namespace nh3api

// used only in map editor and campaign editor
// remains in NH3API for binary compability with the .exe-s
// dangerous in C++98 mode!
template <class T>
class exe_auto_ptr
{
  public:
    typedef T element_type;
    explicit exe_auto_ptr(T *_P = 0) NH3API_NOEXCEPT
        : _Owns(_P != 0), _Ptr(_P)
    {}
    exe_auto_ptr(const exe_auto_ptr<T> &_Y) NH3API_NOEXCEPT :
        _Owns(_Y._Owns), _Ptr(_Y.release())
    {}
    exe_auto_ptr<T>& operator=(const exe_auto_ptr<T>& _Y) NH3API_NOEXCEPT
    {
        if (this != &_Y)
        {
            if (this->_Ptr != _Y.get())
            {
                if (this->_Owns)
                    exe_invoke_delete(this->_Ptr);
                this->_Owns = _Y._Owns;
            }
            else if (_Y._Owns)
            {
                this->_Owns = true;
            }
            this->_Ptr = _Y.release();
        }
        return *this;
    }
    #if NH3API_STD_MOVE_SEMANTICS
    exe_auto_ptr(exe_auto_ptr<T>&& _Y) NH3API_NOEXCEPT :
        _Owns(nh3api::exchange(_Y._Owns, false)), _Ptr(nh3api::exchange(_Y._Ptr, nullptr))
    {}

    exe_auto_ptr<T>& operator=(exe_auto_ptr<T>&& _Y) NH3API_NOEXCEPT
    {
        if (this != &_Y)
        {
            if (this->_Ptr != _Y._Ptr)
            {
                if (this->_Owns)
                    exe_invoke_delete(this->_Ptr);
                this->_Owns = nh3api::exchange(_Y._Owns, false);
            }
            else if (_Y._Owns)
            {
                this->_Owns = true;
            }
            this->_Ptr = _Y.release();
            this->_Owns = true;
        }

        return *this;
    }
    #endif

    ~exe_auto_ptr()
    {
        if (_Owns)
            exe_invoke_delete(_Ptr);
    }
    T& operator*() const NH3API_NOEXCEPT
    {
        return (*get());
    }
    T* operator->() const NH3API_NOEXCEPT
    {
        return (get());
    }
    T* get() const NH3API_NOEXCEPT
    {
        return (_Ptr);
    }
    T* release() const NH3API_NOEXCEPT
    {
        _Owns = false;
        return (_Ptr);
    }

  private:
    mutable bool _Owns;
    mutable T* _Ptr;
};

#if NH3API_CHECK_CPP11

template <class T>
struct exe_default_delete
{
    exe_default_delete() noexcept = default;

    template <class U, typename nh3api::tt::enable_if<nh3api::tt::is_convertible<U*, T*>::value, int>::type = 0>
    exe_default_delete(const exe_default_delete<U>&) noexcept {}

    void operator()(T* ptr) const noexcept
    {
        exe_invoke_delete(ptr);
    }
};

template <class T>
struct exe_default_delete<T[]>
{
    exe_default_delete() noexcept = default;

    template <class U, typename nh3api::tt::enable_if<nh3api::tt::is_convertible<U (*)[], T (*)[]>::value, int>::type = 0>
    exe_default_delete(const exe_default_delete<U>&) noexcept {}

    void operator()(T* ptr) const noexcept
    {
        exe_invoke_array_delete(ptr);
    }
};

template<typename T>
using exe_unique_ptr = std::unique_ptr<T, exe_default_delete<T>>;

#ifdef __cpp_lib_make_unique
template <class T, class... Args, typename nh3api::tt::enable_if<!nh3api::tt::is_array<T>::value, int>::type = 0>
exe_unique_ptr<T> exe_make_unique( Args&&... args )
{ return exe_unique_ptr<T>(new (exe_heap) T(std::forward<Args>(args)...)); }
#endif

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T, typename nh3api::tt::enable_if<!nh3api::tt::is_array<T>::value, int>::type = 0>
exe_unique_ptr<T> exe_make_unique_for_overwrite()
{ return exe_unique_ptr<T>(new (exe_heap) T); }
#endif

#ifdef __cpp_lib_make_unique
template <class T, typename nh3api::tt::enable_if<nh3api::tt::is_array<T>::value && std::extent<T>::value == 0, int>::type = 0>
exe_unique_ptr<T> exe_make_unique(const size_t size)
{
    typedef typename std::remove_extent<T>::type elem_t;
    return exe_unique_ptr<T>(new (exe_heap) elem_t[size]());
}
#endif

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T, typename nh3api::tt::enable_if<nh3api::tt::is_unbounded_array<T>::value, int>::type = 0>
exe_unique_ptr<T> exe_make_unique_for_overwrite(const size_t size)
{
    typedef typename std::remove_extent<T>::type elem_t;
    return exe_unique_ptr<T>(new (exe_heap) elem_t[size]);
}
#endif

#ifdef __cpp_lib_make_unique
template <class T, class... Args, typename nh3api::tt::enable_if<std::extent<T>::value != 0, int>::type = 0>
void exe_make_unique(Args&&...) = delete;
#endif

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T, class... Args, typename nh3api::tt::enable_if<nh3api::tt::is_bounded_array<T>::value != 0, int>::type = 0>
void exe_make_unique_for_overwrite(Args&&...) = delete;
#endif

#endif // C++11

// .exe's _Lockit
struct exe_scoped_lock
{
    exe_scoped_lock() NH3API_NOEXCEPT
    { THISCALL_1(void*, 0x60BB58, this); }

    ~exe_scoped_lock() NH3API_NOEXCEPT
    { THISCALL_1(void, 0x60BBF4, this); }
};

namespace nh3api 
{

template<size_t size>
void trivial_zero(void* ptr)
{
    // MSVC and Clang do a good job at optimizing it to a single zeroed SSE2 register move, if size == 16
    for (size_t i = 0; i < size; ++i )
    {
        (static_cast<uint8_t*>(ptr))[i] = 0;
    }
}

template<size_t size> NH3API_FORCEINLINE
void trivial_swap(void* __restrict left, void* __restrict right)
{
    #if NH3API_CHECK_MSVC 
    if ( size == 16 )
    {
        const __m128i val1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(left));
        const __m128i val2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(right));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(left), val2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(right), val1);  
    }
    else 
    {
    
    #endif
    // GCC and Clang easily optimize this to SSE2 swap if size == 16
    for (size_t i = 0; i < size; ++i) 
    {
        unsigned char temp = (static_cast<uint8_t*>(left))[i];
        (static_cast<uint8_t*>(left))[i] = (static_cast<uint8_t*>(right))[i];
        (static_cast<uint8_t*>(right))[i] = temp;
    }

    #if NH3API_CHECK_MSVC
    }
    #endif
}

} // namespace nh3api 

NH3API_DISABLE_WARNING_END
