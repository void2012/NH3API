//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/interface/widgets.hpp"

#pragma pack(push, 4)
// Game chat /
// Игровой чат.
// size = 0x44 = 68, align = 4
class CChatManager
{
    public:
        // Chat message string /
        // Строка сообщения чата.
        // size = 0x88 = 136, align = 4
        struct CChatStr
        {
            // Message text /
            // Текст сообщения.
            // offset: +0x0 = +0,  size = 0x80 = 128
            std::array<char, 128> sText {};

            // offset: +0x80 = +128,  size = 0x4 = 4
            uint32_t killTime { 0 };

            // System message /
            // Системное сообщение.
            // offset: +0x84 = +132,  size = 0x1 = 1
            bool isSystem { false };

            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;

        } NH3API_MSVC_LAYOUT;
    /*
        CChatManager(int32_t maxChatLines)
        { SetMaxLines(maxChatLines); }

        CChatManager(const nh3api::dummy_tag_t&)
        {}

        ~CChatManager()
        {
            if ( widgetText )
                exe_delete(widgetText);
            if ( msgArray )
                exe_invoke_array_delete(msgArray);
        }
    */

    public:
        CChatManager() = delete;
        ~CChatManager() = delete;

        template<typename... Args> NH3API_FORMAT_PRINTF(2, 3)
        void AddChat(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553C40, this, cChatMsg, std::forward<Args>(args)...); }

        template<typename... Args> NH3API_FORMAT_PRINTF(2, 3)
        void TurnDurationMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553D60, this, cChatMsg, std::forward<Args>(args)...); }

        template<typename... Args> NH3API_FORMAT_PRINTF(2, 3)
        void SystemMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553EA0, this, cChatMsg, std::forward<Args>(args)...); }

        template<typename... Args> NH3API_FORMAT_PRINTF(2, 3)
        void PlayerDropMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553F60, this, cChatMsg, std::forward<Args>(args)...); }

        template<typename... Args> NH3API_FORMAT_PRINTF(2, 3)
        void PlayerEnterMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x554030, this, cChatMsg, std::forward<Args>(args)...); }

        void UpdateWidget(textWidget* Widget, bool killOld, int32_t numLines)
        { THISCALL_4(void, 0x554100, this, Widget, killOld, numLines); }

        void UpdateWidgetText(int32_t numLines, textWidget* Widget)
        { THISCALL_3(void, 0x5542E0, this, numLines, Widget); }

        [[nodiscard]] bool HasOldChat() const
        { return THISCALL_1(bool, 0x5541B0, this); }

        void KillOldChat()
        { THISCALL_1(void, 0x5541F0, this); }

        void PauseTimeOuts()
        { THISCALL_1(void, 0x554450, this); }

        void ResumeTimeOuts()
        { THISCALL_1(void, 0x554460, this); }

        void ClearChat()
        { THISCALL_1(void, 0x5544B0, this); }

        void SetMaxLines(int32_t maxChatLines)
        { THISCALL_2(void, 0x5544D0, this, maxChatLines); }

        void SetPosition(int32_t newPos)
        { THISCALL_2(void, 0x554570, this, newPos); }

    public:
        // Chat messages
        // Сообщения в чате.
        // offset: +0x0 = +0,  size = 0x4 = 4
        CChatStr* msgArray {nullptr};

        // Current chat message index /
        // Индекс текущего сообщения в чате.
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t currMsg {0};

        // Number of chat messages /
        // Количество сообщений чата.
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t msgCount {0};

        // offset: +0xC = +12,  size = 0x4 = 4
        const char* widgetText {nullptr};

        // offset: +0x10 = +16,  size = 0x4 = 4
        uint32_t pauseTime {0};

        // offset: +0x14 = +20,  size = 0x1 = 1
        bool changed {false};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x18 = +24,  size = 0x4 = 4
        textWidget* lastWidget {nullptr};

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t maxLines {0};

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t position {-1};

        // offset: +0x24 = +36,  size = 0x1 = 1
        bool chatKilled {false};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x28 = +40,  size = 0x4 = 4
        uint32_t channel {0};

        // offset: +0x2C = +44,  size = 0x1 = 1
        bool isSysMsg {false};

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x30 = +48,  size = 0x4 = 4
        sample* chatSample {nullptr};

        // offset: +0x34 = +52,  size = 0x4 = 4
        sample* playerDropSample {nullptr};

        // offset: +0x38 = +56,  size = 0x4 = 4
        sample* sysMsgSample {nullptr};

        // offset: +0x3C = +60,  size = 0x4 = 4
        sample* turnDurSample {nullptr};

        // offset: +0x40 = +64,  size = 0x4 = 4
        sample* playerEnterSample {nullptr};

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

inline CChatManager& chatMan = get_global_var_ref(0x69D800, CChatManager);
