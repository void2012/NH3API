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

#include "interface/windows.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#pragma pack(push, 4)
// size = 0x38 = 56, align = 4
NH3API_VIRTUAL_CLASS baseManager
{
    public:
        struct vftable_t
        {
            int32_t (__thiscall* Open)(baseManager*, int32_t); // +0
            void (__thiscall* Close)(baseManager*); // +4
            int32_t (__thiscall* Main)(baseManager*, message*); // +8
        };

    public:
        NH3API_FORCEINLINE
        baseManager() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(baseManager)
        { THISCALL_1(void, 0x44D200, this); }

        NH3API_FORCEINLINE
        baseManager(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}

        NH3API_DEFAULT_DESTRUCTOR(baseManager)

    public:
        // vftable shift: +0
        virtual int32_t __thiscall Open(int32_t newPriority)
        { return get_type_vftable(this)->Open(this, newPriority); }

        // vftable shift: +4
        virtual void __thiscall Close()
        { get_type_vftable(this)->Close(this); }

        // vftable shift: +8
        virtual int32_t __thiscall Main(message& msg)
        { return get_type_vftable(this)->Main(this, &msg); }

    public:

    // Next manager /
    // offset: +0x4 = +4,  size = 0x4 = 4
    baseManager* nextManager;

    // Previous manager /
    // offset: +0x8 = +8,  size = 0x4 = 4
    baseManager* prevManager;

    // Current manager ID /
    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t id;

    // Current manager priority /
    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t priority;

    // Current manager name /
    // offset: +0x14 = +20,  size = 0x20 = 32
    std::array<char, 32> cMgrName;

    // offset: +0x34 = +52,  size = 0x4 = 4
    int32_t status;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_BASEMANAGER
#define NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(CLASS_NAME)\
virtual int32_t __thiscall Open(int32_t newPriority) override \
{ return get_type_vftable(this)->Open(static_cast<baseManager*>(this), newPriority); }\
virtual void __thiscall Close() override \
{ get_type_vftable(this)->Close(static_cast<baseManager*>(this)); }\
virtual int32_t __thiscall Main(message& msg) override \
{ return get_type_vftable(this)->Main(static_cast<baseManager*>(this), &msg); }
#endif // NH3API_VIRTUAL_OVERRIDE_BASEMANAGER

#pragma pack(push, 4)
// size = 0x10 = 16, align = 4
struct executive
{
    public:
        NH3API_FORCEINLINE
        executive() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(executive)
        { THISCALL_1(void, 0x4B0660, this); }

        NH3API_FORCEINLINE
        executive(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}

        NH3API_DEFAULT_DESTRUCTOR(executive)

    public:
        int32_t DoDialog(baseManager* newDialog)
        { return THISCALL_2(int32_t, 0x4B0770, this, newDialog); }

        int32_t AddManager(baseManager* newManager, int32_t newPriority)
        { return THISCALL_3(int32_t, 0x4B0880, this, newManager, newPriority); }

        void RemoveManager(baseManager* mgr)
        { THISCALL_2(void, 0x4B0950, this, mgr); }

        void CallManager(baseManager* mgr)
        { THISCALL_2(void, 0x4B09D0, this, mgr); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        baseManager* headManager;

        // offset: +0x4 = +4,  size = 0x4 = 4
        baseManager* tailManager;

        // offset: +0x8 = +8,  size = 0x4 = 4
        baseManager* currentManager;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t dialogReturn;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
executive* const& gpExec
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699550, executive*));

NH3API_DISABLE_WARNING_END

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B9BC, baseManager)
