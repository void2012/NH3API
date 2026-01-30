//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "windows.hpp"
#include "../base_manager.hpp"

// dialog result is stored in heroWindowManager::dialogReturn
enum DialogReturnType : int32_t
{
    DIALOG_RETURN_NONE    = -1,
    DIALOG_RETURN_CANCEL  = 30721,
    DIALOG_RETURN_OK      = 30722,
    DIALOG_RETURN_ACCEPT  = 30725,
    DIALOG_RETURN_DECLINE = 30726,
    DIALOG_SELECT_LEFT    = 30729,
    DIALOG_SELECT_RIGHT   = 30730,
};

#pragma pack(push, 4)
// Dialog system manager /
// Менеджер системы диалогов.
// size = 0x60 = 96, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS heroWindowManager : public baseManager
{
    public:
        heroWindowManager() = delete;
        heroWindowManager(const heroWindowManager&) = delete;
        heroWindowManager(heroWindowManager&&)                 = delete;
        heroWindowManager& operator=(const heroWindowManager&) = delete;
        heroWindowManager& operator=(heroWindowManager&&)      = delete;
        ~heroWindowManager() = delete;

    public:
        int32_t BroadcastMessage(int32_t command, int32_t subType, int32_t itemId, int32_t msgExtra) noexcept
        { return THISCALL_5(int32_t, 0x602930, this, command, subType, itemId, msgExtra); }

        NH3API_CALLBACK(dialogFunction, -1)
        int32_t DoDialog(heroWindow* dialogWindow, int32_t (__thiscall* dialogFunction)(message*), bool32_t bFadeIn) noexcept
        { return THISCALL_4(int32_t, 0x602AE0, this, dialogWindow, dialogFunction, bFadeIn); }

        NH3API_CALLBACK(dialogFunction, -1)
        int32_t DoDialog(heroWindow* dialogWindow, int32_t (__thiscall* dialogFunction)(message&), bool32_t bFadeIn) noexcept
        { return THISCALL_4(int32_t, 0x602AE0, this, dialogWindow, dialogFunction, bFadeIn); }

        void DoQuickView(heroWindow* newWindow) noexcept
        { THISCALL_2(void, 0x603000, this, newWindow); }

        void UpdateScreen(int32_t x, int32_t y, int32_t h, int32_t w)
        { THISCALL_5(void, 0x603190, this, x, y, h, w); }

        void FadeScreen(int32_t inOut, int32_t speed, bool expect_fadein) noexcept
        { THISCALL_4(void, 0x603210, this, inOut, speed, expect_fadein); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(heroWindowManager)

    public:
        // Dialog return result /
        // Результат работы диалога.
        // offset: +0x38 = +56,  size = 0x4 = 4
        DialogReturnType dialogReturn;

        // Last dialog element ID cursor hover /
        // Элемент диалога, на который был наведён курсор.
        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t lastHover;

        // Screen graphics bitmap /
        // Картинка на экране(да, буквально ВСЯ графика игры рисуется в эту переменную).
        // offset: +0x40 = +64,  size = 0x4 = 4
        Bitmap16Bit* screenBitmap;

    protected:
        // offset: +0x44 = +68,  size = 0x4 = 4
        bool32_t colorCyclingOn;

    public:
        // Waiting for fadein effect /
        // Ожидание эффекта плавного перетухания экрана.
        // offset: +0x48 = +72,  size = 0x1 = 1
        bool isWaitingForFadeIn;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x4C = +76,  size = 0x4 = 4
        Bitmap16Bit* bmpFizzleSource;

        // Current window /
        // Текущее окно.
        // offset: +0x50 = +80,  size = 0x4 = 4
        heroWindow* activeWindow;

        // offset: +0x54 = +84,  size = 0x4 = 4
        heroWindow* lastActive;

        // offset: +0x58 = +88,  size = 0x4 = 4
        heroWindow* headWindow;

        // offset: +0x5C = +92,  size = 0x4 = 4
        heroWindow* tailWindow;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

// Dialog system manager /
// Менеджер системы диалогов.
inline heroWindowManager* const& gpWindowManager = get_global_var_ref(0x6992D0, heroWindowManager*);
inline const uint32_t&           giDialogTimeout = get_global_var_ref(0x6977D4, const uint32_t);

NH3API_SPECIALIZE_TYPE_VFTABLE(0x643D5C, heroWindowManager)
