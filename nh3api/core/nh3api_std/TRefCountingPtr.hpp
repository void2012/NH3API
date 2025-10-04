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

#include "memory.hpp"

// used by the map editor and the campaign editor
// reference-counting pointer for binary compability with the .exe
// use std::shared_ptr<T> if you want modern ref-counting pointer.
template <typename T>
class TRefCountingPtr
{
protected:
    NH3API_STATIC_ASSERT("TRefCountingPtr<T> can't store const element", !nh3api::tt::is_const<T>::value);
    NH3API_STATIC_ASSERT("TRefCountingPtr<T> can't store non-objects", nh3api::tt::is_object<T>::value);

public:
    TRefCountingPtr() 
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<T>::value)
        : m_pWrapper(new (exe_heap) _TWrapper)
    {}

    explicit TRefCountingPtr(T const& value) 
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<T>::value)
        : m_pWrapper(new (exe_heap) _TWrapper(value) )
    {}

    TRefCountingPtr(TRefCountingPtr const& other) 
    NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<T>::value)
        : m_pWrapper(other.m_pWrapper)
    {}

    #if NH3API_STD_MOVE_SEMANTICS
    TRefCountingPtr(TRefCountingPtr&& other) NH3API_NOEXCEPT
        : m_pWrapper(nh3api::exchange(other.m_pWrapper, nullptr))
    {}

    TRefCountingPtr& operator=(TRefCountingPtr&& other) NH3API_NOEXCEPT
    {
        if (this != &other)
        {
            release();
            this->m_pWrapper = nh3api::exchange(other.m_pWrapper, nullptr);
        }
        return *this;
    }
    #endif

    ~TRefCountingPtr() NH3API_NOEXCEPT
    {
        release();
    }

public:
    T* get() NH3API_NOEXCEPT
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

    T const* get() const NH3API_NOEXCEPT
    { return m_pWrapper ? &m_pWrapper->m_object : nullptr; }

    TRefCountingPtr& operator=(TRefCountingPtr const& other) NH3API_NOEXCEPT
    {
        if (this != &other)
        {
            _TWrapper* oldWrapper = m_pWrapper;
            if (other.m_pWrapper)
                ++other.m_pWrapper->m_refCnt;
            
            m_pWrapper = other.m_pWrapper;
            if (oldWrapper)
                release(oldWrapper);
        }
        return *this;
    }

    T&	operator*() NH3API_NOEXCEPT
    { return *get(); }

    const T& operator*() const NH3API_NOEXCEPT
    { return *get(); }

    T*	operator->()  NH3API_NOEXCEPT
    { return get(); }

    const T* operator->() const NH3API_NOEXCEPT
    { return get(); }

    #if NH3API_CHECK_CPP11
    explicit
    #endif
    operator bool() const NH3API_NOEXCEPT
    { return get(); }

public: // private
    struct _TWrapper
    {
        _TWrapper()
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<T>::value)
            : m_refCnt(1), m_object()
        {}

        _TWrapper(const T& value)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<T>::value)
            : m_refCnt(1), m_object(value)
        {}

        _TWrapper(_TWrapper&& other)
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_move_constructible<T>::value)
            : m_refCnt(nh3api::exchange(other.m_refCnt, 1)), m_object(std::move(other.m_object))
        {}

        NH3API_NODISCARD size_t refcount() const NH3API_NOEXCEPT
        { return m_refCnt; }

        ~_TWrapper() NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<T>::value)
        #if NH3API_CHECK_CPP11
        = default;
        #else 
        {}
        #endif

        friend class TRefCountingPtr<T>;

    private:
        size_t m_refCnt;
        T      m_object;
    };

    _TWrapper*	m_pWrapper;

public: 
    void release() NH3API_NOEXCEPT 
    {
        if (m_pWrapper)
        {
            release(m_pWrapper);
            m_pWrapper = nullptr;
        }
    }

    void release(_TWrapper* wrapper) NH3API_NOEXCEPT
    {
        if (--wrapper->m_refCnt == 0)
        {
            nh3api::destroy_at(&wrapper->m_object);
            exe_delete(wrapper);
        }
    }

    bool split() NH3API_NOEXCEPT
    {
        if (!m_pWrapper)
            return false; 

        _TWrapper* pNewWrapper = new (exe_heap) _TWrapper(m_pWrapper->m_object);
        if ( pNewWrapper == nullptr )
            return false;
        
        pNewWrapper->m_refCnt = 1;
        
        if ( --m_pWrapper->m_refCnt == 0)
        {
            nh3api::destroy_at(&m_pWrapper->m_object);
            exe_delete(m_pWrapper);
        }

        m_pWrapper = pNewWrapper;
        return true;
    }
};

template <typename T>
bool operator==(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right)
{ return left.get() == right.get(); }

template <typename T>
bool operator!=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right)
{ return left.get() != right.get(); }
