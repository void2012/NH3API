//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "base_manager.hpp" // baseManager
#include "events.hpp" // message

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// inputManager keycodes /
// Коды клавиш клавиатуры.
enum EKeyCodes : uint32_t
{
    KEYCODE_NONE          = 0x00,
    KEYCODE_ESCAPE        = 0x01,
    KEYCODE_1             = 0x02,
    KEYCODE_2             = 0x03,
    KEYCODE_3             = 0x04,
    KEYCODE_4             = 0x05,
    KEYCODE_5             = 0x06,
    KEYCODE_6             = 0x07,
    KEYCODE_7             = 0x08,
    KEYCODE_8             = 0x09,
    KEYCODE_9             = 0x0A,
    KEYCODE_0             = 0x0B,
    KEYCODE_MINUS         = 0x0C,
    KEYCODE_EQUALS        = 0x0D,
    KEYCODE_BACKSPACE     = 0x0E,
    KEYCODE_TAB           = 0x0F,
    KEYCODE_Q             = 0x10,
    KEYCODE_W             = 0x11,
    KEYCODE_E             = 0x12,
    KEYCODE_R             = 0x13,
    KEYCODE_T             = 0x14,
    KEYCODE_Y             = 0x15,
    KEYCODE_U             = 0x16,
    KEYCODE_I             = 0x17,
    KEYCODE_O             = 0x18,
    KEYCODE_P             = 0x19,
    KEYCODE_LEFTBRACKET   = 0x1A,
    KEYCODE_RIGHTBRACKET  = 0x1B,
    KEYCODE_ENTER         = 0x1C,
    KEYCODE_CTRL          = 0x1D,
    KEYCODE_A             = 0x1E,
    KEYCODE_S             = 0x1F,
    KEYCODE_D             = 0x20,
    KEYCODE_F             = 0x21,
    KEYCODE_G             = 0x22,
    KEYCODE_H             = 0x23,
    KEYCODE_J             = 0x24,
    KEYCODE_K             = 0x25,
    KEYCODE_L             = 0x26,
    KEYCODE_SEMICOLON     = 0x27,
    KEYCODE_DBLAPOSTROPHE = 0x28,
    KEYCODE_TILDE         = 0x29,
    KEYCODE_LEFT_SHIFT    = 0x2A,
    KEYCODE_BACKSLASH     = 0x2B,
    KEYCODE_Z             = 0x2C,
    KEYCODE_X             = 0x2D,
    KEYCODE_C             = 0x2E,
    KEYCODE_V             = 0x2F,
    KEYCODE_B             = 0x30,
    KEYCODE_N             = 0x31,
    KEYCODE_M             = 0x32,
    KEYCODE_COMMA         = 0x33,
    KEYCODE_PERIOD        = 0x34,
    KEYCODE_KP_DIVIDE     = 0x35,
    KEYCODE_RIGHT_SHIFT   = 0x36,
    KEYCODE_KP_MULTIPLY   = 0x37,
    KEYCODE_ALT           = 0x38,
    KEYCODE_SPACE         = 0x39,
    KEYCODE_CAPS_LOCK     = 0x3A,
    KEYCODE_F1            = 0x3B,
    KEYCODE_F2            = 0x3C,
    KEYCODE_F3            = 0x3D,
    KEYCODE_F4            = 0x3E,
    KEYCODE_F5            = 0x3F,
    KEYCODE_F6            = 0x40,
    KEYCODE_F7            = 0x41,
    KEYCODE_F8            = 0x42,
    KEYCODE_F9            = 0x43,
    KEYCODE_F10           = 0x44,
    KEYCODE_NUMLOCK       = 0x45,
    KEYCODE_SCROLLLOCK    = 0x46,
    KEYCODE_KP_7          = 0x47,
    KEYCODE_KP_8          = 0x48,
    KEYCODE_KP_9          = 0x49,
    KEYCODE_KP_MINUS      = 0x4A,
    KEYCODE_KP_4          = 0x4B,
    KEYCODE_KP_5          = 0x4C,
    KEYCODE_KP_6          = 0x4D,
    KEYCODE_KP_PLUS       = 0x4E,
    KEYCODE_KP_1          = 0x4F,
    KEYCODE_KP_2          = 0x50,
    KEYCODE_KP_3          = 0x51,
    KEYCODE_KP_0          = 0x52,
    KEYCODE_KP_ENTER      = 0x53,
    KEYCODE_PRINTSCREEN   = 0x54,
    KEYCODE_FN            = 0x55,
    KEYCODE_F11           = 0x57,
    KEYCODE_F12           = 0x58
};

#pragma pack(push, 4)
// Input system manager /
// Менеджер системы ввода с клавиатуры.
// size = 0x960 = 2400, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS inputManager : public baseManager
{
    public:
        NH3API_FORCEINLINE
        inputManager() NH3API_NOEXCEPT
            : baseManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4EC540, this); }

        NH3API_FORCEINLINE
        inputManager(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : baseManager(tag)
        {}

    public:
        void Flush()
        { THISCALL_1(void, 0x4EC640, this); }

        message GetEvent()
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
        int32_t bufferBusy;

    protected:
        // offset: +0x844 = +2116,  size = 0x4 = 4
        int32_t mouseInstalled;

    public:
        // Таблица сканкодов /
        // Scancode table.
        // offset: +0x848 = +2120,  size = 0x100 = 256
        std::array<int16_t, 128> scanCodeTable;

    protected:
        // offset: +0x948 = +2376,  size = 0x4 = 4
        int32_t keyboardInstalled;

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

NH3API_INLINE_OR_EXTERN
inputManager*& gpInputManager NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699530, inputManager*));

NH3API_DISABLE_WARNING_END
