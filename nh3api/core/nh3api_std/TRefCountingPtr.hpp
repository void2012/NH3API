//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <type_traits>
#ifdef __cpp_lib_three_way_comparison
#include <compare>
#endif

#include "memory.hpp"

// used by the map editor and the campaign editor
// reference-counting pointer for binary compability with the .exe
// use std::shared_ptr<T> if you want modern ref-counting pointer.
template <typename T>
class TRefCountingPtr
{
protected:
    static_assert(!std::is_const_v<T>, "TRefCountingPtr<T> can't store const element");
    static_assert(std::is_object_v<T>, "TRefCountingPtr<T> can't store non-objects");

public:
    using pointer = T*;
    using element_type = T;

    inline TRefCountingPtr() noexcept = default;
    inline explicit TRefCountingPtr(std::nullptr_t) noexcept
    {}

    inline explicit TRefCountingPtr(const T& value)
        : m_pWrapper { new (exe_heap) _TWrapper{value} }
    {}

    inline TRefCountingPtr(const TRefCountingPtr& other)
    noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>))
        : m_pWrapper { other.m_pWrapper }
    {
        if ( m_pWrapper )
            ++this->m_pWrapper->m_refCnt;
    }

    inline TRefCountingPtr(TRefCountingPtr&& other) noexcept
        : m_pWrapper { std::exchange(other.m_pWrapper, nullptr) }
    {}

    inline TRefCountingPtr& operator=(TRefCountingPtr&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            this->m_pWrapper = std::exchange(other.m_pWrapper, nullptr);
        }
        return *this;
    }

    inline ~TRefCountingPtr() noexcept
    { reset(); }

    [[nodiscard]] T* get() noexcept
    {
        if ( m_pWrapper == nullptr )
            return nullptr;

        if ( m_pWrapper->m_refCnt > 1 )
        {
            if ( !split() )
            {
            #if NH3API_DEBUG
                __debugbreak();
            #endif
            #ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
                std::abort();
            #else
                throw std::bad_alloc{};
            #endif
                NH3API_UNREACHABLE();
            }
        }

        return &m_pWrapper->m_object;
    }

    [[nodiscard]] inline T const* get() const noexcept
    { return m_pWrapper ? &m_pWrapper->m_object : nullptr; }

    TRefCountingPtr& operator=(TRefCountingPtr const& other) noexcept
    {
        if ( this != &other && this->m_pWrapper != other.m_pWrapper )
        {
            if ( other.m_pWrapper )
                ++other.m_pWrapper->m_refCnt;

            reset();
            this->m_pWrapper = other.m_pWrapper;
        }
        return *this;
    }

    [[nodiscard]] inline T& operator*() noexcept
    { return *get(); }

    [[nodiscard]] inline const T& operator*() const noexcept
    { return *get(); }

    [[nodiscard]] inline T* operator->()  noexcept
    { return get(); }

    [[nodiscard]] inline const T* operator->() const noexcept
    { return get(); }

    [[nodiscard]] inline explicit operator bool() const noexcept
    { return m_pWrapper != nullptr; }

    inline void swap(TRefCountingPtr& other) noexcept
    { std::swap(this->m_pWrapper, other.m_pWrapper); }

private:
    struct _TWrapper
    {
        explicit _TWrapper(const T& src)
            : m_object{src}
        {}

        _TWrapper(_TWrapper&&)                 = delete;
        _TWrapper(const _TWrapper&)            = delete;
        _TWrapper& operator=(const _TWrapper&) = delete;
        _TWrapper& operator=(_TWrapper&&)      = delete;

        [[nodiscard]] size_t refcount() const noexcept
        { return m_refCnt; }

        ~_TWrapper() noexcept = default;

        friend class TRefCountingPtr<T>;

    private:
        size_t m_refCnt {1};
        T      m_object;
    }; // _TWrapper

    _TWrapper* m_pWrapper { nullptr };

public:
    // same as reset()
    void release() noexcept
    { reset(); }

    void reset(const T& value)
            noexcept(noexcept(
                    std::is_nothrow_copy_assignable_v<T>
                    && std::is_nothrow_copy_constructible_v<T>))
    {
        if ( unique() )
        {
            m_pWrapper->m_object = value;
        }
        else
        {
            reset();
            m_pWrapper = new (exe_heap) _TWrapper { value };
        }
    }

    void reset() noexcept
    {
        if ( m_pWrapper )
        {
            if ( --this->m_pWrapper->m_refCnt == 0 )
            {
                std::destroy_at(m_pWrapper);
                exe_delete(m_pWrapper);
            }

            m_pWrapper = nullptr;
        }
    }

    bool split() noexcept
    {
        if (!m_pWrapper || m_pWrapper->m_refCnt <= 1) return true;

        _TWrapper* pNewWrapper = new (exe_heap) _TWrapper{ this->m_pWrapper->m_object };
        if ( pNewWrapper == nullptr )
            return false;

        --m_pWrapper->m_refCnt;
        m_pWrapper = pNewWrapper;
        return true;
    }

    [[nodiscard]] inline size_t use_count() const noexcept
    { return m_pWrapper ? m_pWrapper->m_refCnt : 0; }

    [[nodiscard]] inline bool unique() const noexcept
    { return use_count() == 1; }

};

template <typename T>
inline void swap(TRefCountingPtr<T>& lhs, TRefCountingPtr<T>& rhs) noexcept
{ lhs.swap(rhs); }

template <typename T>
inline bool operator==(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() == right.get(); }

template <typename T>
inline bool operator==(const TRefCountingPtr<T>& left, std::nullptr_t) noexcept
{ return left.get() == nullptr; }

template <typename T>
inline bool operator==(std::nullptr_t, const TRefCountingPtr<T>& right) noexcept
{ return right.get() == nullptr; }

#ifdef __cpp_lib_three_way_comparison
template <typename T>
inline std::strong_ordering operator<=>(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return std::compare_three_way()(left.get(), right.get()); }

template <typename T>
inline std::strong_ordering operator<=>(const TRefCountingPtr<T>& left, std::nullptr_t) noexcept
{ return std::compare_three_way()(left.get(), static_cast<T*>(nullptr)); }
#else
template <typename T>
inline bool operator!=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() != right.get(); }

template <typename T>
inline bool operator!=(const TRefCountingPtr<T>& left, std::nullptr_t) noexcept
{ return left.get() != nullptr; }

template <typename T>
inline bool operator!=(std::nullptr_t, const TRefCountingPtr<T>& right) noexcept
{ return right.get() != nullptr; }

template <typename T>
inline bool operator<(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() < right.get(); }

template <typename T>
inline bool operator<=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() <= right.get(); }

template <typename T>
inline bool operator>(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() > right.get(); }

template <typename T>
inline bool operator>=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() >= right.get(); }
#endif // three-way comparison
