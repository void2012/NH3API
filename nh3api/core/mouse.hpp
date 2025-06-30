//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "base_manager.hpp" // baseManager
#include "nh3api_std/memory.hpp" // NH3API_SCALAR_DELETING_DESTRUCTOR

#pragma pack(push, 4)
// Mouse manager /
// Менеджер мыши.
// size = 0x90 = 144, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS mouseManager : public baseManager
{
    // structures
    public:
        struct vftable_t : baseManager::vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(mouseManager*, uint8_t);
        };

    // enums
    public:
        enum EPointerSet : int32_t 
        {
            SAME_SET         = -1, //
            INVALID_SET      = -1, //
            DEFAULT_SET      =  0, //
            ADVENTURE_SET    =  1, //
            COMBAT_SET       =  2, //
            SPELL_SET        =  3, //
            ARTIFACT_SET     =  4, //
            MAX_POINTER_SETS =  5  //
        };

    // constructors / destructor
    public:
        NH3API_FORCEINLINE
        mouseManager() NH3API_NOEXCEPT
            : baseManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x50CD40, this); }

        NH3API_FORCEINLINE
        mouseManager(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : baseManager(tag)
        {}

    // member functions
    public:
        void SetPointer(int32_t new_frame, mouseManager::EPointerSet new_set)
        { THISCALL_3(void, 0x50CEA0, this, new_frame, new_set); }

        void Update(bool bForceIt)
        { THISCALL_2(void, 0x50CF90, this, bForceIt); }

        void MouseCoords(int32_t& x, int32_t& y)
        { THISCALL_3(void, 0x50D700, this, &x, &y); }

        void MouseCoords(POINT& point)
        { THISCALL_2(void, 0x50D700, this, &point); }

        void HidePointer()
        { THISCALL_1(void, 0x50D740, this); }

        void ShowPointer(bool force)
        { THISCALL_2(void, 0x50D7B0, this, force); }

        void CheckUpdate()
        { THISCALL_1(void, 0x50D880, this); }

        void ShowSystemCursor(bool show_it)
        { THISCALL_2(void, 0x50DC20, this, show_it); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(mouseManager)

        NH3API_SCALAR_DELETING_DESTRUCTOR

    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t bNoChangePointer;

        // offset: +0x3C = +60,  size = 0x10 = 16
        RECT LastDraw;

        // offset: +0x4C = +76,  size = 0x4 = 4
        EPointerSet Set;

        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t Frame;

        // offset: +0x54 = +84,  size = 0x4 = 4
        CSprite* Sprite;

        // offset: +0x58 = +88,  size = 0x4 = 4
        int32_t ImageX;

        // offset: +0x5C = +92,  size = 0x4 = 4
        int32_t ImageY;

        // offset: +0x60 = +96,  size = 0x4 = 4
        int32_t DisableCount;

        // offset: +0x64 = +100,  size = 0x1 = 1
        bool SystemPointerIsOn;

        // offset: +0x68 = +104,  size = 0x4 = 4
        int32_t iHideCount;

        // offset: +0x6C = +108,  size = 0x8 = 8
        POINT cursorPos;

        // offset: +0x74 = +116,  size = 0x4 = 4
        int32_t Busy;

        // offset: +0x78 = +120,  size = 0x18 = 24
        CRITICAL_SECTION CriticalSection;

};

NH3API_SIZE_ASSERT(0x90, mouseManager);

NH3API_INLINE_OR_EXTERN
mouseManager*& gpMouseManager NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6992B0, mouseManager*));

#pragma pack(pop)
