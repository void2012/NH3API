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

#include "nh3api_std/stl_extras.hpp"
#include "nh3api_std/memory.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

class heroWindow;
#pragma pack(push, 4)
// Dialog or Input systems event(message) /
// Сообщение диалоговой системы или системы ввода.
// size = 0x20 = 32, align = 4
struct message
{
    public:
        enum ECommandType
        {
            COMMAND_INPUT_KEY_DOWN   = 1,
            COMMAND_INPUT_KEY_UP     = 2,
            COMMAND_MOUSE_ROLLOVER   = 4,
            COMMAND_MOUSE_LEFT_DOWN  = 8,
            COMMAND_MOUSE_LEFT_UP    = 16,
            COMMAND_MOUSE_RIGHT_DOWN = 32,
            COMMAND_MOUSE_RIGHT_UP   = 64,
            COMMAND_WINDOW           = 512
        };

    public:
        NH3API_FORCEINLINE constexpr
        message(ECommandType command_,
                int32_t subType_,
                int32_t itemId_,
                int32_t qualifier_,
                int32_t mouseX_,
                int32_t mouseY_,
                uint32_t extra_,
                heroWindow* _window ) noexcept
        :
            command(command_),
            subType(subType_),
            itemId(itemId_),
            qualifier(qualifier_),
            mouseX(mouseX_),
            mouseY(mouseY_),
            extra(extra_),
            window(_window)
        {}

        message() noexcept = default;
        message(const message&) noexcept = default;
        message& operator=(const message&) noexcept = default;
        message(message&&) noexcept = default;
        message& operator=(message&&) noexcept = default;

        message(const ::nh3api::dummy_tag_t&) noexcept
        {}

        inline ~message() noexcept = default;

    public:
        template<typename T = uint32_t
        #ifndef __cpp_lib_concepts
        , std::enable_if_t<std::is_trivially_copyable_v<T> || std::is_scalar_v<T>, bool> = false
        #endif
        >
        [[nodiscard]] T getExtra() const noexcept
        #ifdef __cpp_lib_concepts
        requires(std::is_trivially_copyable_v<T> || std::is_scalar_v<T>)
        #endif
        { return nh3api::cast<T>(extra); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        ECommandType command;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t subType;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t itemId;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t qualifier;

        // Cursor X coordinate /
        // X-Координата курсора.
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t mouseX;

        // Cursor Y coordinate /
        // Y-Координата курсора.
        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t mouseY;

        // Message information /
        // Информация, передаваемая в сообщении.
        // offset: +0x18 = +24,  size = 0x4 = 4
        uint32_t extra;

        // A window where the event has occured /
        // Окно, в котором появилось сообщение.
        // offset: +0x1C = +28,  size = 0x4 = 4
        heroWindow* window;

};
#pragma pack(pop)

NH3API_FORCEINLINE void PollSound() noexcept
{ FASTCALL_0(void, 0x4EDB20); }

NH3API_FORCEINLINE void Process1WindowsMessage() noexcept
{ CDECL_0(void, 0x4F8640); }

NH3API_FORCEINLINE int32_t AppCommand(HWND hwnd, uint32_t msg, uint32_t wParam, int32_t lParam) noexcept
{ return FASTCALL_4(int32_t, 0x4F86F0, hwnd, msg, wParam, lParam); }

NH3API_FORCEINLINE void KBChangeMenu(HMENU menu) noexcept
{ FASTCALL_1(void, 0x4F8810, menu); }

NH3API_FORCEINLINE void SetNoDialogMenus(int32_t arg) noexcept
{ FASTCALL_1(void, 0x4F8870, arg); }

namespace GameTime
{

[[nodiscard]] NH3API_FORCEINLINE uint32_t Get() noexcept
{ return FASTCALL_0(uint32_t, 0x4F8970); }

NH3API_FORCEINLINE void DelayTil(uint32_t time) noexcept
{ FASTCALL_1(void, 0x4F8980, time); }

NH3API_FORCEINLINE void Delay(uint32_t time) noexcept
{ FASTCALL_1(void, 0x4F8A50, time); }

} // namespace GameTime

inline HWND&     hwndApp                    = get_global_var_ref(0x699650, HWND);
inline HMENU&    hmnuApp                    = get_global_var_ref(0x699654, HMENU);
inline int32_t&  gGameCommand               = get_global_var_ref(0x697728, int32_t);
inline HMENU&    gDefaultMenu               = get_global_var_ref(0x698A34, HMENU);
inline HMENU&    gCombatMenu                = get_global_var_ref(0x698A38, HMENU);
inline HMENU&    hmnuRecruitSave            = get_global_var_ref(0x69D644, HMENU);
inline HMENU&    hmnuCurrent                = get_global_var_ref(0x69960C, HMENU);
inline bool&     gbProcessingCombatAction   = get_global_var_ref(0x698A3C, bool);
inline bool&     insideProcessMessage       = get_global_var_ref(0x699608, bool);
inline bool&     WeAreActiveWindow          = get_global_var_ref(0x6783D0, bool);
inline bool32_t& gbCheatMenus               = get_global_var_ref(0x698A84, bool32_t);
inline std::array<char, 60>&  gcCommandLine = get_global_var_ref(0x699610, std::array<char, 60>);
inline std::array<char, 368>& gcRegAppPath  = get_global_var_ref(0x698614, std::array<char, 368>);

NH3API_DISABLE_WARNING_END
