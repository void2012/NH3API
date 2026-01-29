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

#include <type_traits> // std::type_identity_t

#include "nh3api_std/call_macros.hpp"
#include "nh3api_std/stl_extras.hpp"
#if !defined(__cpp_lib_type_identity)
#include "nh3api_std/type_traits.hpp"
#endif

// forward declaration to avoid reliance on heavy <windef.h> include
#ifndef _WINDEF_
extern "C"
{
struct HWND__;
struct HMENU__;
struct HINSTANCE__;
typedef HWND__*      HWND;
typedef HMENU__*     HMENU;
typedef HINSTANCE__* HINSTANCE;
} // extern "C"
#endif

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

class heroWindow;
#pragma pack(push, 4)
// Dialog or Input systems event(message) /
// Сообщение диалоговой системы или системы ввода.
// size = 0x20 = 32, align = 4
struct message
{
    public:
        enum ECommandType : uint32_t
        {
            COMMAND_NONE             = 0,
            COMMAND_INPUT_KEY_DOWN   = 1,
            COMMAND_INPUT_KEY_UP     = 2,
            COMMAND_MOUSE_ROLLOVER   = 4,
            COMMAND_MOUSE_LEFT_DOWN  = 8,
            COMMAND_MOUSE_LEFT_UP    = 16,
            COMMAND_MOUSE_RIGHT_DOWN = 32,
            COMMAND_MOUSE_RIGHT_UP   = 64,
            COMMAND_WINDOW           = 512,
            COMMAND_MANAGER          = 16384
        };

        NH3API_FLAG_ENUM EQualifiers : uint32_t
        {
            QUALIFIER_NONE        = 0,
            QUALIFIER_KEY_SHIFT   = 0b0000000001,
            QUALIFIER_KEY_RSHIFT  = 0b0000000010,
            QUALIFIER_KEY_CTRL    = 0b0000000100,
            QUALIFIER_KEY_ALT     = 0b0000100000,
            QUALIFIER_RIGHT_CLICK = 0b1000000000
        };

    public:
        inline constexpr message() noexcept = default;
        inline constexpr message(ECommandType command_,
                                 int32_t      subType_,
                                 int32_t      itemId_,
                                 EQualifiers  qualifier_,
                                 int32_t      mouseX_,
                                 int32_t      mouseY_,
                                 uint32_t     extra_,
                                 heroWindow*  window_) noexcept
            : command { command_ },
              subType { subType_ },
              itemId { itemId_ },
              qualifier { qualifier_ },
              mouseX { mouseX_ },
              mouseY { mouseY_ },
              extra { extra_ },
              window { window_ }
        {}

        inline message(const nh3api::dummy_tag_t&) noexcept
        {}

        inline constexpr message(const message&) noexcept            = default;
        inline constexpr message& operator=(const message&) noexcept = default;
        inline constexpr message(message&&) noexcept                 = default;
        inline constexpr message& operator=(message&&) noexcept      = default;

        inline ~message() noexcept = default;

    public:
    #if defined(__cpp_lib_concepts) && defined(__cpp_lib_type_identity)
        template<typename T = uint32_t>
        [[nodiscard]] T get_extra() const noexcept
        requires(std::is_trivially_copyable_v<T> || std::is_scalar_v<T>)
        { return nh3api::cast<T>(extra); }

        template<typename T>
        void set_extra(std::type_identity_t<T> arg) noexcept
        requires(std::is_trivially_copyable_v<T> || std::is_scalar_v<T>)
        { extra = nh3api::cast<uint32_t>(arg); }
    #else // defined(__cpp_lib_concepts) && defined(__cpp_lib_type_identity)
        template<typename T = uint32_t, std::enable_if_t<std::is_trivially_copyable_v<T> || std::is_scalar_v<T>, bool> = false>
        [[nodiscard]] T get_extra() const noexcept
        { return nh3api::cast<T>(extra); }

        template<typename T, std::enable_if_t<std::is_trivially_copyable_v<T> || std::is_scalar_v<T>, bool> = false>
        void set_extra(nh3api::tt::type_identity_t<T> arg) noexcept
        { extra = nh3api::cast<uint32_t>(arg); }
    #endif // defined(__cpp_lib_concepts) && defined(__cpp_lib_type_identity)

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        ECommandType command {COMMAND_NONE};

        // COMMAND_INPUT_KEY_DOWN -> EKeyCode
        // COMMAND_INPUT_KEY_UP   -> EKeyCode
        // COMMAND_WINDOW -> widget::EReturnCodes
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t subType {0};

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t itemId {0};

        // offset: +0xC = +12,  size = 0x4 = 4
        EQualifiers qualifier {QUALIFIER_NONE};

        // Cursor X coordinate /
        // X-Координата курсора.
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t mouseX {0};

        // Cursor Y coordinate /
        // Y-Координата курсора.
        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t mouseY {0};

        // Message information /
        // Информация, передаваемая в сообщении.
        // offset: +0x18 = +24,  size = 0x4 = 4
        uint32_t extra {0};

        // A window where the event has occured /
        // Окно, в котором появилось сообщение.
        // offset: +0x1C = +28,  size = 0x4 = 4
        heroWindow* window {nullptr};

};
#pragma pack(pop)

inline void PollSound()
{ FASTCALL_0(void, 0x4EDB20); }

// Keep processing Windows messages:
// this is useful when we're dealing with a process that takes too long to calculate.
// Basically, this function prevents Windows from freezing the program with "Heroes3.exe is not responding" /
// Продолжение обработки сообщений Windows:
// эту функцию стоит вызвать, когда происходит какой-то очень долгий процесс,
// делается это для того, чтобы Windows не подумало, что игра зависла и заморозило его с окошком "Heroes3.exe перестало отвечать".
inline void Process1WindowsMessage()
{ CDECL_0(void, 0x4F8640); }

inline void KBChangeMenu(HMENU menu)
{ FASTCALL_1(void, 0x4F8810, menu); }

inline void SetNoDialogMenus(int32_t arg)
{ FASTCALL_1(void, 0x4F8870, arg); }

namespace GameTime
{

[[nodiscard]] inline uint32_t Get()
{ return FASTCALL_0(uint32_t, 0x4F8970); }

inline void DelayTil(uint32_t time)
{ FASTCALL_1(void, 0x4F8980, time); }

inline void Delay(uint32_t time)
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

NH3API_WARNING(pop)
