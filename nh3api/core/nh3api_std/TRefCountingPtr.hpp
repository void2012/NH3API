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

    TRefCountingPtr() noexcept
        : m_pWrapper(nullptr)
    {}

    TRefCountingPtr(std::nullptr_t) noexcept
        : m_pWrapper(nullptr)
    {}

    explicit TRefCountingPtr(const T& value)
    noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>))
        : m_pWrapper(new (exe_heap) _TWrapper(value))
    {}

    explicit TRefCountingPtr(const TRefCountingPtr& other)
    noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>))
        : m_pWrapper(other.m_pWrapper)
    {
        ++this->m_pWrapper->m_refCnt;
    }

    explicit TRefCountingPtr(TRefCountingPtr&& other) noexcept
        : m_pWrapper(std::exchange(other.m_pWrapper, nullptr))
    {}

    TRefCountingPtr& operator=(TRefCountingPtr&& other)
    noexcept(noexcept(std::is_nothrow_destructible_v<T>))
    {
        if (this != &other)
        {
            reset();
            this->m_pWrapper = std::exchange(other.m_pWrapper, nullptr);
        }
        return *this;
    }

    ~TRefCountingPtr()
    noexcept(noexcept(std::is_nothrow_destructible_v<T>))
    { reset(); }

    [[nodiscard]] T* get() noexcept
    {
        if ( m_pWrapper == nullptr )
            return nullptr;

        if ( m_pWrapper->m_refCnt > 1 )
        {
            if (!split())
                return nullptr;
        }

        return &m_pWrapper->m_object;
    }

    [[nodiscard]] T const* get() const noexcept
    { return m_pWrapper ? &m_pWrapper->m_object : nullptr; }

    TRefCountingPtr& operator=(TRefCountingPtr const& other)
    noexcept(noexcept(std::is_nothrow_destructible_v<T>))
    {
        if (this != &other && this->m_pWrapper && other.m_pWrapper)
        {
            if (other.m_pWrapper)
                ++other.m_pWrapper->m_refCnt;

            reset();
            this->m_pWrapper = other.m_pWrapper;
        }
        return *this;
    }

    [[nodiscard]] T& operator*() noexcept
    { return *get(); }

    [[nodiscard]] const T& operator*() const noexcept
    { return *get(); }

    [[nodiscard]] T* operator->()  noexcept
    { return get(); }

    [[nodiscard]] const T* operator->() const noexcept
    { return get(); }

    [[nodiscard]] explicit operator bool() const noexcept
    { return get(); }

    inline void swap(TRefCountingPtr& other) noexcept
    { std::swap(this->m_pWrapper, other.m_pWrapper); }

public: // private
    struct _TWrapper
    {
        _TWrapper()
        noexcept(noexcept(std::is_nothrow_default_constructible_v<T>))
            : m_refCnt(1), m_object()
        {}

        explicit _TWrapper(const T& value)
        noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>))
            : m_refCnt(1), m_object(value)
        {}

        explicit _TWrapper(_TWrapper&& other)
        noexcept(noexcept(std::is_nothrow_move_constructible_v<T>))
            : m_refCnt(std::exchange(other.m_refCnt, 1)), m_object(std::move(other.m_object))
        {}

        // Warning! Does not affect reference counting
        explicit _TWrapper(const _TWrapper& other)
        noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>))
            : m_refCnt(other.m_refCnt), m_object(other.m_object)
        {}

        // Warning! Does not affect reference counting
        _TWrapper& operator=(const _TWrapper& other)
        noexcept(noexcept(std::is_nothrow_copy_assignable_v<T>))
        {
            this->m_refCnt = other.m_refCnt;
            this->m_object = other.m_object;
        }

        _TWrapper& operator=(_TWrapper&& other)
        noexcept(noexcept(std::is_nothrow_move_assignable_v<T>))
        {
            this->m_refCnt = std::exchange(other.m_refCnt, 1);
            this->m_object = std::move(other.m_object);
        }

        [[nodiscard]] size_t refcount() const noexcept
        { return m_refCnt; }

        ~_TWrapper() noexcept(noexcept(std::is_nothrow_destructible_v<T>)) = default;

        friend class TRefCountingPtr<T>;

    private:
        size_t m_refCnt;
        T      m_object;
    }; // _TWrapper

    _TWrapper* m_pWrapper;

public:
    pointer release()
    noexcept(noexcept(std::is_nothrow_destructible_v<T>))
    {
        if ( m_pWrapper == nullptr )
            return nullptr;

        if ( m_pWrapper->m_refCnt > 1 )
            split();

        return std::exchange(m_pWrapper, nullptr);
    }

    void reset(const T& value)
    noexcept(noexcept(
        std::is_nothrow_copy_assignable_v<T>
        && std::is_nothrow_copy_constructible_v<T>
    ))
    {
        if (this->m_pWrapper == nullptr)
            return;

        if ( this->m_pWrapper->m_refCnt > 1 )
        {
            _TWrapper* pNewWrapper = new (exe_heap) _TWrapper(value);
            if ( pNewWrapper == nullptr )
                return;
            --this->m_pWrapper->m_refCnt;
            this->m_pWrapper = pNewWrapper;
        }
        else
        {
            this->m_pWrapper->m_object = value;
        }
    }

    void reset() noexcept
    {
        if ( m_pWrapper == nullptr )
            return;

        if ( --this->m_pWrapper->m_refCnt == 0 )
        {
            std::destroy_at(&m_pWrapper->m_object);
            exe_delete(m_pWrapper);
        }
    }

    bool split() noexcept
    {
        if (this->m_pWrapper == nullptr)
            return false;

        _TWrapper* pNewWrapper = new (exe_heap) _TWrapper(this->m_pWrapper->m_object);
        if ( pNewWrapper == nullptr )
            return false;

        --this->m_pWrapper->m_refCnt;
        this->m_pWrapper = pNewWrapper;
        return true;
    }
};

template <typename T>
inline void swap(TRefCountingPtr<T>& lhs, TRefCountingPtr<T>& rhs) noexcept
{ lhs.swap(rhs); }

template <typename T>
bool operator==(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() == right.get(); }

template <typename T>
bool operator!=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() != right.get(); }

template <typename T>
bool operator<(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() < right.get(); }

template <typename T>
bool operator<=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() <= right.get(); }

template <typename T>
bool operator>(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() > right.get(); }

template <typename T>
bool operator>=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right) noexcept
{ return left.get() >= right.get(); }
