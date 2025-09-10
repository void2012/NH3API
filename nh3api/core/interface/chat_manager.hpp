//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "widgets.hpp"
#if !NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT
#include <stdarg.h>
#endif

#pragma pack(push, 4)
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
            std::array<char, 128> sText;

            // offset: +0x80 = +128,  size = 0x4 = 4
            uint32_t killTime;
            
            // System message /
            // Системное сообщение.
            // offset: +0x84 = +132,  size = 0x1 = 1
            bool isSystem;

        };
    
    private:
    /*
    CChatManager(int32_t maxChatLines)
            : msgArray(nullptr), 
              currMsg(0), 
              msgCount(0),
              changed(true),
              lastWidget(nullptr),
              maxLines(maxChatLines),
              widgetText(nullptr),
              position(-1),
              chatKilled(false),
              isSysMsg(false),
              channel(0),
              chatSample(nullptr),
              playerDropSample(nullptr),
              sysMsgSample(nullptr),
              turnDurSample(nullptr),
              playerEnterSample(nullptr)
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
        CChatManager();
        ~CChatManager();

    public: 
        #if NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT
        template<typename... Args>
        void __cdecl AddChat(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553C40, this, cChatMsg, ::std::forward<Args>(args)...); }
        
        template<typename... Args>
        void __cdecl TurnDurationMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553D60, this, cChatMsg, ::std::forward<Args>(args)...); }

        template<typename... Args>
        void __cdecl SystemMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553EA0, this, cChatMsg, ::std::forward<Args>(args)...); }

        template<typename... Args>
        void __cdecl PlayerDropMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x553F60, this, cChatMsg, ::std::forward<Args>(args)...); }

        template<typename... Args>
        void __cdecl PlayerEnterMsg(const char* cChatMsg, Args... args)
        { CDECL_N(char, 0x554030, this, cChatMsg, ::std::forward<Args>(args)...); }
        #else 
        void __cdecl AddChat(const char* cChatMsg, ...)
        {
            va_list args;
            va_start(args, cChatMsg);
            CALL_VA(void, 0x553C40, this, cChatMsg, args);
            va_end(args);
        }

        void __cdecl TurnDurationMsg(const char* cChatMsg, ...)
        {
            va_list args;
            va_start(args, cChatMsg);
            CALL_VA(void, 0x553D60, this, cChatMsg, args);
            va_end(args);
        }

        void __cdecl SystemMsg(const char* cChatMsg, ...)
        {
            va_list args;
            va_start(args, cChatMsg);
            CALL_VA(void, 0x553EA0, this, cChatMsg, args);
            va_end(args);
        }

        void __cdecl PlayerDropMsg(const char* cChatMsg, ...)
        {
            va_list args;
            va_start(args, cChatMsg);
            CALL_VA(void, 0x553F60, this, cChatMsg, args);
            va_end(args);
        }

        void __cdecl PlayerEnterMsg(const char* cChatMsg, ...)
        {
            va_list args;
            va_start(args, cChatMsg);
            CALL_VA(void, 0x554030, this, cChatMsg, args);
            va_end(args);
        }
        #endif

        void UpdateWidget(textWidget* Widget, bool killOld, int32_t numLines)
        { THISCALL_4(void, 0x554100, this, Widget, killOld, numLines); }

        void UpdateWidgetText(int32_t numLines, textWidget* Widget)
        { THISCALL_3(void, 0x5542E0, this, numLines, Widget); }

        bool HasOldChat() const
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
        CChatStr* msgArray;

        // Current chat message index /
        // Индекс текущего сообщения в чате.
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t currMsg;

        // Number of chat messages /
        // Количество сообщений чата.
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t msgCount;

        // offset: +0xC = +12,  size = 0x4 = 4
        const char* widgetText;

        // offset: +0x10 = +16,  size = 0x4 = 4
        uint32_t pauseTime;

        // offset: +0x14 = +20,  size = 0x1 = 1
        bool changed;

        // offset: +0x18 = +24,  size = 0x4 = 4
        textWidget* lastWidget;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t maxLines;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t position;

        // offset: +0x24 = +36,  size = 0x1 = 1
        bool chatKilled;

        // offset: +0x28 = +40,  size = 0x4 = 4
        uint32_t channel;

        // offset: +0x2C = +44,  size = 0x1 = 1
        bool isSysMsg;

        // offset: +0x30 = +48,  size = 0x4 = 4
        sample* chatSample;

        // offset: +0x34 = +52,  size = 0x4 = 4
        sample* playerDropSample;

        // offset: +0x38 = +56,  size = 0x4 = 4
        sample* sysMsgSample;

        // offset: +0x3C = +60,  size = 0x4 = 4
        sample* turnDurSample;

        // offset: +0x40 = +64,  size = 0x4 = 4
        sample* playerEnterSample;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
CChatManager& chatMan
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x69D800, CChatManager));
