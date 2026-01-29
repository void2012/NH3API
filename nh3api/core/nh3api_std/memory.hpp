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

#include <cstring> // std::memcpy
#include <memory> // std::unique_ptr
#include <new> // std::nothrow_t
#include <utility> // std::declval, std::exchange

#include "call_macros.hpp" // call macros
#include "type_traits.hpp" // nh3api::tt::has_scalar_deleting_destructor_v

NH3API_WARNING(push)
NH3API_WARNING_MSVC_DISABLE(4714)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")

// maximum allocated size per call: 520177 bytes

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// address: 0x617492
// Heroes3.exe internal operator delete /
// Внутренняя реализация CRT-функции operator new Heroes3.exe.
inline void* __cdecl exe_new(size_t size) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x617492, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x617492, size);
#endif // __has_builtin(__builtin_assume_aligned)
}

// address: 0x60B0F0
// Heroes3.exe internal operator delete /
// Внутренняя реализация CRT-функции operator delete Heroes3.exe.
inline void __cdecl exe_delete(void* ptr) noexcept
{ CDECL_1(void, 0x60B0F0, ptr); }

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// address: 0x61A9D5
// Heroes3.exe internal malloc /
// Внутренняя реализация CRT-функции malloc Heroes3.exe.
inline void* __cdecl exe_malloc(size_t size) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x61A9D5, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x61A9D5, size);
#endif // __has_builtin(__builtin_assume_aligned)
}


// address: 0x61A9D5
// Heroes3.exe internal free /
// Внутренняя реализация CRT-функции free Heroes3.exe.
inline void __cdecl exe_free(void* ptr) noexcept
{ CDECL_1(void, 0x619BB0, ptr); }

[[nodiscard]] NH3API_RETURNS_ALIGNED(8)
// address: 0x619890
// Heroes3.exe internal realloc /
// Внутренняя реализация CRT-функции realloc Heroes3.exe.
inline void* __cdecl exe_realloc(void* ptr, size_t newSize) noexcept
{ return CDECL_2(void*, 0x619890, ptr, newSize); }

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1, 2)
// address: 0x61AA61
// Heroes3.exe internal calloc /
// Внутренняя реализация CRT-функции calloc Heroes3.exe.
inline void* __cdecl exe_calloc(size_t numOfElements, size_t sizeOfElements) noexcept
{ return CDECL_2(void*, 0x61AA61, numOfElements, sizeOfElements); }

// address: 0x61E504
// Heroes3.exe internal _msize /
// Внутренняя реализация CRT-функции _msize Heroes3.exe.
[[nodiscard]] inline int32_t __cdecl exe_msize(const void* ptr) noexcept
{ return CDECL_1(int32_t, 0x61E504, ptr); }

// exe_heap flag passed to the placement new form to allocate via the exe_new /
// exe_heap флаг, который можно передать в placement new для выделения памяти с помощью exe_new.
struct exe_heap_t
{
    inline static constexpr uintptr_t handle_address = 0x6ABD60;
    static HANDLE& get_handle() noexcept
    { return get_global_var_ref(handle_address, HANDLE); }

    // maximum size a heap can manage
    static constexpr size_t max_size() noexcept
    { return size_t(~0U); }

}
// exe_heap constant / константа exe_heap
inline constexpr exe_heap;

// I tried to make new throw exception on invalid allocation
// but there is simply no way to check it with VC6 runtime...
// So I leave the standard C++ definitions with std::nothrow_t
// doing the same thing as without it.

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// usage: new (exe_heap) new-initializer...
inline void* __cdecl operator new(size_t size, const exe_heap_t&) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x617492, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x617492, size);
#endif // __has_builtin(__builtin_assume_aligned)
}

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// usage: new (exe_heap, std::nothrow) new-initializer...
inline void* __cdecl operator new(size_t size, const exe_heap_t&, const std::nothrow_t&) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x617492, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x617492, size);
#endif // __has_builtin(__builtin_assume_aligned)
}

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// usage: new (exe_heap) new-initializer...
// NOTE: It appears that both Itanium ABI and MSVC ABI implicitly allocate size + 4,
// and return exe_new(size) + 4 pointer, so we don't do manual handling
inline void* __cdecl operator new[](size_t size, const exe_heap_t&) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x617492, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x617492, size);
#endif // __has_builtin(__builtin_assume_aligned)
}

[[nodiscard]] NH3API_RETURNS_ALIGNED(8) NH3API_MALLOC(1)
// usage: new (exe_heap, std::nothrow) new-initializer...
// NOTE: It appears that both Itanium ABI and MSVC ABI implicitly allocate size + 4,
// and return exe_new(size) + 4 pointer, so we don't do manual handling
inline void* __cdecl operator new[](size_t size, const exe_heap_t&, const std::nothrow_t&) noexcept
{
#if NH3API_HAS_BUILTIN_ASSUME_ALIGNED
    return __builtin_assume_aligned(CDECL_1(void*, 0x617492, size), 8);
#else // __has_builtin(__builtin_assume_aligned)
    return CDECL_1(void*, 0x617492, size);
#endif // __has_builtin(__builtin_assume_aligned)
}

/*
// what I mean is that if both ABIs didn't do that, we would have to manually handle that:
void* __cdecl operator new[](size_t size, const exe_heap_t&)
{
    int32_t* ptr = reinterpret_cast<int32_t*>(exe_new(size + sizeof(int32_t)));
    *ptr = size;
    return reinterpret_cast<void*>(ptr + 4);
}
*/

// added for the parity
inline void __cdecl operator delete(void* ptr, const exe_heap_t&) noexcept
{ CDECL_1(void, 0x60B0F0, ptr); }

// added for the parity
inline void __cdecl operator delete[](void* ptr, const exe_heap_t&) noexcept
{ CDECL_1(void, 0x60B0F0, ptr); }

// added for the parity
inline void __cdecl operator delete(void* ptr, const exe_heap_t&, const std::nothrow_t&) noexcept
{ CDECL_1(void, 0x60B0F0, ptr); }

// added for the parity
inline void __cdecl operator delete[](void* ptr, const exe_heap_t&, const std::nothrow_t&) noexcept
{ CDECL_1(void, 0x60B0F0, ptr); }

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
                __builtin_addressof(*ptr)))) T; // invoke default constructor
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

// invoke delete and destroy (use this instead of plain exe_delete)
template<typename T>
inline void exe_invoke_delete(T* ptr) noexcept
{
    if constexpr ( nh3api::tt::has_scalar_deleting_destructor_v<T> )
    {
        ptr->scalar_deleting_destructor(1);
    }
    else
    {
        ptr->~T();
        exe_delete(ptr);
    }
}

// overload for void*
template<>
inline void exe_invoke_delete<void>(void* ptr) noexcept
{ exe_delete(ptr); }

// invoke destructor(plain or scalar_deleting_destructor)
template<typename T>
inline void exe_invoke_destructor(T* ptr) noexcept
{
    if constexpr ( nh3api::tt::has_scalar_deleting_destructor_v<T> )
        ptr->scalar_deleting_destructor(0);
    else
        ptr->~T();
}

// invoke array form of delete. Use it instead of plain exe_delete
template<typename T> NH3API_FORCEINLINE
void exe_invoke_array_delete(T* ptr) noexcept
{
    if constexpr ( std::is_trivially_destructible_v<T> )
    {
        int32_t* const pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        exe_delete(pre_ptr);
    }
    else
    {
        int32_t* const pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        const int32_t array_size = *pre_ptr;
        for ( int32_t i = 0; i < array_size; ++i )
            exe_invoke_destructor(__builtin_addressof(ptr[i]));

        exe_delete(pre_ptr);
    }
}

// overload for void*
template<> NH3API_FORCEINLINE
void exe_invoke_array_delete<void>(void* ptr) noexcept
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
    ptr->~T();
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
    static_assert(::std::is_class_v<T>, "vector_deleting_destructor<T>: T must be a class");

    // operator delete[]
    if ( (flag & 2U) != 0 )
    {
        int32_t* pre_ptr = (reinterpret_cast<int32_t*>(ptr)) - 1;
        const int32_t allocated_size = *pre_ptr;
        // if ( allocated_size != -1 )
            exe_vector_destructor_iterator(ptr, sizeof(T), allocated_size / sizeof(T));
        if ( (flag & 1U ) != 0 )
            exe_delete(pre_ptr);
    }
    else // operator delete
    {
        ptr->~T();
        if ( (flag & 1U ) != 0 )
            exe_delete(ptr);
    }
}

} // namespace nh3api

// low-level virtual call with only shift in vftable
template<typename Result, typename T, typename ... Args> inline
Result virtual_call(T* thisPtr, size_t shift, Args&& ... args)
{ return THISCALL_N(Result, reinterpret_cast<uintptr_t>(get_vftable(thisPtr)) + shift, ::std::forward<Args>(args)...); }

// allocator which uses internal .exe operator new and operator delete /
// allocator, использующий внутренние операторы new и delete .exe для управления памятью.
template<class T>
class exe_allocator
{
protected:
    static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed."
                                       "because allocator<const T> is ill-formed.");
    static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements "
                                          "because of [allocator.requirements].");
    static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements "
                                           "because of [allocator.requirements].");

public:
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using value_type      = T;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = nh3api::tt::false_type;
    using propagate_on_container_swap            = std::false_type;
    using is_always_equal                        = std::true_type;

    template<class U>
    struct rebind
    { using other = exe_allocator<U>; };

// constructors for standard compliance
public:
    constexpr exe_allocator() noexcept = default;
    constexpr exe_allocator(const exe_allocator&) noexcept = default;

    template<typename U>
    constexpr explicit exe_allocator(const exe_allocator<U>&) noexcept
    {}

public:
    [[nodiscard]] inline static pointer allocate(size_t size) noexcept
    { return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap)); }

    [[nodiscard]] inline static pointer allocate(size_t size, const void*) noexcept
    { return static_cast<pointer>(::operator new(size * sizeof(value_type), exe_heap)); }

    inline static void deallocate(void* ptr) noexcept
    { exe_delete(ptr); }

    inline static void deallocate(void* ptr, size_t) noexcept
    { exe_delete(ptr); }

    [[nodiscard]] inline constexpr static size_t max_size() noexcept
    { return size_t(~0U) / sizeof(value_type); }

    [[nodiscard]] inline constexpr exe_allocator& select_on_container_copy_construction() const noexcept
    { return *this; }
};

template<class T, class U>
inline constexpr bool operator==(const exe_allocator<T>&, const exe_allocator<U>&) noexcept
{ return true; }

template<class T, class U>
inline constexpr bool operator!=(const exe_allocator<T>&, const exe_allocator<U>&) noexcept
{ return false; }

// used only in map editor and campaign editor
// remains in NH3API for binary compability with the .exe-s
// dangerous in C++98 mode!
template <class T>
class exe_auto_ptr
{
  public:
    using element_type = T;
    explicit exe_auto_ptr(T* ptr = nullptr) noexcept
        : _Owns(ptr != 0), _Ptr(ptr)
    {}

    exe_auto_ptr(const exe_auto_ptr<T> &other) noexcept :
        _Owns(other._Owns), _Ptr(other.release())
    {}

    exe_auto_ptr<T>& operator=(const exe_auto_ptr<T>& other) noexcept
    {
        if (this != &other)
        {
            if (this->_Ptr != other.get())
            {
                if (this->_Owns)
                    exe_invoke_delete(this->_Ptr);
                this->_Owns = other._Owns;
            }
            else if (other._Owns)
            {
                this->_Owns = true;
            }
            this->_Ptr = other.release();
        }
        return *this;
    }

    exe_auto_ptr(exe_auto_ptr<T>&& other) noexcept :
        _Owns(std::exchange(other._Owns, false)), _Ptr(std::exchange(other._Ptr, nullptr))
    {}

    exe_auto_ptr<T>& operator=(exe_auto_ptr<T>&& other) noexcept
    {
        if (this != &other)
        {
            if (this->_Ptr != other._Ptr)
            {
                if (this->_Owns)
                    exe_invoke_delete(this->_Ptr);
                this->_Owns = std::exchange(other._Owns, false);
            }
            else if (other._Owns)
            {
                this->_Owns = true;
            }
            this->_Ptr = other.release();
            this->_Owns = true;
        }

        return *this;
    }

    ~exe_auto_ptr()
    {
        if (_Owns)
            exe_invoke_delete(_Ptr);
    }

    T& operator*() const noexcept
    { return (*get()); }

    T* operator->() const noexcept
    { return (get()); }

    T* get() const noexcept
    { return (_Ptr); }

    T* release() const noexcept
    {
        _Owns = false;
        return _Ptr;
    }

  private:
    mutable bool _Owns;
    mutable T* _Ptr;
};

template <class T>
struct exe_default_delete
{
    exe_default_delete() noexcept = default;

    #ifdef __cpp_concepts
    template <class U>
    explicit exe_default_delete(const exe_default_delete<U>&) noexcept
    requires (std::is_convertible_v<U*, T*>) {}
    #else
    template <class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    exe_default_delete(const exe_default_delete<U>&) noexcept {}
    #endif

    void operator()(T* ptr) const noexcept
    {
        exe_invoke_delete(ptr);
    }
};

template <class T>
struct exe_default_delete<T[]>
{
    exe_default_delete() noexcept = default;

    #ifdef __cpp_concepts
    template <class U>
    explicit exe_default_delete(const exe_default_delete<U>&) noexcept
    requires (std::is_convertible_v<U (*)[], T (*)[]>) {}
    #else
    template <class U, std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>, int> = 0>
    exe_default_delete(const exe_default_delete<U>&) noexcept {}
    #endif

    void operator()(T* ptr) const noexcept
    { exe_invoke_array_delete(ptr); }
};

template<typename T>
using exe_unique_ptr = std::unique_ptr<T, exe_default_delete<T>>;

template <class T, class... Args, std::enable_if_t<!std::is_array_v<T>, int> = 0>
exe_unique_ptr<T> exe_make_unique(Args&&... args)
{ return exe_unique_ptr<T>(new (exe_heap) T(std::forward<Args>(args)...)); }

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T>
exe_unique_ptr<T> exe_make_unique_for_overwrite() requires (!std::is_array_v<T>)
{ return exe_unique_ptr<T>(new (exe_heap) T); }
#endif

template <class T, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, int> = 0>
exe_unique_ptr<T> exe_make_unique(const size_t size)
{ return exe_unique_ptr<T>(new (exe_heap) std::remove_extent_t<T> [size]()); }

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T, std::enable_if_t<nh3api::tt::is_unbounded_array<T>::value, int> = 0>
exe_unique_ptr<T> exe_make_unique_for_overwrite(const size_t size)
{
    return exe_unique_ptr<T>(new (exe_heap) std::remove_extent_t<T> [size]);
}
#endif

template <class T, class... Args, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
void exe_make_unique(Args&&...) = delete;

#ifdef __cpp_lib_smart_ptr_for_overwrite
template <class T, class... Args, std::enable_if_t<nh3api::tt::is_bounded_array<T>::value != 0, int> = 0>
void exe_make_unique_for_overwrite(Args&&...) = delete;
#endif

// .exe's _Lockit
struct exe_scoped_lock
{
    exe_scoped_lock() noexcept
    {
        NH3API_MEMSHIELD_BEGIN
        THISCALL_1(void*, 0x60BB58, this);
        NH3API_MEMSHIELD_END
    }

    exe_scoped_lock(const exe_scoped_lock&)            = delete;
    exe_scoped_lock(exe_scoped_lock&&)                 = delete;
    exe_scoped_lock& operator=(const exe_scoped_lock&) = delete;
    exe_scoped_lock& operator=(exe_scoped_lock&&)      = delete;

    ~exe_scoped_lock() noexcept
    {
        NH3API_MEMSHIELD_BEGIN
        THISCALL_1(void, 0x60BBF4, this);
        NH3API_MEMSHIELD_END
    }
};

#ifdef _MSVC_STL_UPDATE
    #if __has_include(<__msvc_sanitizer_annotate_container.hpp>)
    // for now, we disable ASan annotations for MSVC STL containers in case the user uses exe_allocator
    template<typename T>
    inline constexpr bool std::_Disable_ASan_container_annotations_for_allocator<exe_allocator<T>> = true;
    #endif // __has_include(<__msvc_sanitizer_annotate_container.hpp>)
#endif // _MSVC_STL_UPDATE

NH3API_WARNING(pop)
