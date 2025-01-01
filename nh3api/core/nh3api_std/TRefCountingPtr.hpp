//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
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
        : m_pWrapper(new (exe_heap) _TWrapper)
    { ++m_pWrapper->m_refCnt; }

    explicit TRefCountingPtr(T const& value)
        : m_pWrapper(new (exe_heap) _TWrapper(value) )
    { ++m_pWrapper->m_refCnt; }

    TRefCountingPtr(TRefCountingPtr const& other)
        : m_pWrapper(other.m_pWrapper)
    { ++m_pWrapper->m_refCnt; }

    #if NH3API_STD_MOVE_SEMANTICS
    TRefCountingPtr(TRefCountingPtr&& other)
        : m_pWrapper(nh3api::exchange(other.m_pWrapper, nullptr))
    {}

    TRefCountingPtr& operator=(TRefCountingPtr&& other)
    {
        this->m_pWrapper = nh3api::exchange(other.m_pWrapper, nullptr);
        return *this;
    }
    #endif

    ~TRefCountingPtr()
    {
        if ( --m_pWrapper->m_refCnt )
            exe_delete(m_pWrapper);
    }

public:
    T* get()
    {
        if ( m_pWrapper->m_refCnt > 1 )
            split();
        return m_pWrapper->m_object;
    }

    T const* get() const
    { return &m_pWrapper->m_object; }

    TRefCountingPtr& operator=(TRefCountingPtr const & other)
    {
        ++other.m_pWrapper->m_refCnt;
        if ( --this->m_pWrapper->m_refCnt )
            exe_delete(this->m_pWrapper);
        this->m_pWrapper = other.m_pWrapper;
        return *this;
    }

    T&	operator*()
    { return *get(); }
    const T& operator*() const
    { return *get(); }
    T*	operator->() { return get(); }
    const T* operator->() const
    { return get(); }

    operator bool() const
    { return get(); }

public: // private
    struct _TWrapper
    {
        _TWrapper()
            : m_refCnt(1), m_object()
        {}

        _TWrapper(const T& value)
            : m_refCnt(1), m_object(value)
        {}

        size_t refcount() const
        { return m_refCnt; }

    private:
        size_t m_refCnt;
        T      m_object;
    };

    _TWrapper*	m_pWrapper;

public: // private
    void split()
    {
        _TWrapper* pNewWrapper = new (exe_heap) _TWrapper(m_pWrapper->m_object);
        --m_pWrapper->m_refCnt;
        m_pWrapper = pNewWrapper;
    }
};

template <typename T>
bool operator==(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right)
{ return left.get() == right.get(); }

template <typename T>
bool operator!=(const TRefCountingPtr<T>& left, const TRefCountingPtr<T>& right)
{ return left.get() != right.get(); }

namespace nh3api
{

// get TRefCountingPtr reference count
template <typename T> NH3API_FORCEINLINE
size_t refcount(const TRefCountingPtr<T>& arg)
{ return arg.m_pWrapper->refcount(); }

} // namespace nh3api