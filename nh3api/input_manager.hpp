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

#include <array>

#include "core/base_manager.hpp" // baseManager
#include "core/events.hpp"       // message

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#pragma pack(push, 4)
// Input system manager /
// Менеджер системы ввода с клавиатуры.
// size = 0x960 = 2400, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS inputManager : public baseManager
{
    public:
        inline inputManager() noexcept
            : baseManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4EC540, this); }

        inline inputManager(const nh3api::dummy_tag_t& tag) noexcept
            : baseManager(tag)
        {}

    public:
        void Flush()
        { THISCALL_1(void, 0x4EC640, this); }

        [[nodiscard]] message GetEvent()
        {
            message result(nh3api::dummy_tag);
            THISCALL_2(void, 0x4EC660, this, &result);
            return result;
        }

        message PeekEvent()
        {
            message result(nh3api::dummy_tag);
            THISCALL_2(void, 0x4EC710, this, &result);
            return result;
        }

        void AsciiConvert(message& msg)
        { THISCALL_2(void, 0x4EC7C0, this, &msg); }

        void ForceMouseMove()
        { THISCALL_1(void, 0x4ECCD0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(inputManager)

    public:
        // Буфер сообщений /
        // Events buffer.
        // offset: +0x38 = +56,  size = 0x800 = 2048
        std::array<message, 64> iBuffer;

        // Индекс самого первого сообщения в буфере /
        // Index of the first event in the buffer.
        // offset: +0x838 = +2104,  size = 0x4 = 4
        int32_t iHead;

        // Индекс после самого последнего сообщение в буфере /
        // Index following the last event in the buffer.
        // offset: +0x83C = +2108,  size = 0x4 = 4
        int32_t iTail;

        // Буфер сообщений изменяется /
        // Event buffer is being modified.
        // offset: +0x840 = +2112,  size = 0x4 = 4
        bool32_t bufferBusy;

    protected:
        // offset: +0x844 = +2116,  size = 0x4 = 4
        bool32_t mouseInstalled;

    public:
        // Таблица сканкодов /
        // Scancode table.
        // offset: +0x848 = +2120,  size = 0x100 = 256
        std::array<int16_t, 128> scanCodeTable;

    protected:
        // offset: +0x948 = +2376,  size = 0x4 = 4
        bool32_t keyboardInstalled;

        // offset: +0x94C = +2380,  size = 0x4 = 4
        int32_t keyboardFilter;

        // offset: +0x950 = +2384,  size = 0x4 = 4
        int32_t keyCodeType;

        // offset: +0x954 = +2388,  size = 0x4 = 4
        int32_t extendFlag;

        // offset: +0x958 = +2392,  size = 0x4 = 4
        int32_t currWidgetID;

        // offset: +0x95C = +2396,  size = 0x4 = 4
        int32_t possibleWidgetID;

};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x960, inputManager);

inline inputManager* const& gpInputManager = get_global_var_ref(0x699530, inputManager*);

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63FE10, inputManager)

NH3API_WARNING(pop)
