//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "../input_manager.hpp"
#include "windows.hpp"

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#pragma pack(push, 4)
// Icon widget(sprite) /
// Виджет-иконка(спрайт).
// size = 0x48 = 72, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS iconWidget : public widget
{
    public:
        struct vftable_t : widget::vftable_t
        {
            bool (__thiscall* handle_click)(iconWidget*, bool, bool);
        };

        iconWidget()                             = delete;
        iconWidget(const iconWidget&)            = delete;
        iconWidget(iconWidget&&)                 = delete;
        iconWidget& operator=(const iconWidget&) = delete;
        iconWidget& operator=(iconWidget&&)      = delete;

    public:
        iconWidget(int32_t x_,
                   int32_t y_,
                   int32_t w,
                   int32_t h,
                   int32_t id_,
                   const char* image,
                   int32_t frame,
                   int32_t sequence,
                   bool flipped,
                   uint16_t back_color,
                   widget::ETypes style_ = widget::ICON_WIDGET) noexcept
            : widget(::nh3api::dummy_tag)
        { THISCALL_12(void, 0x4EA800, this, x_, y_, w, h, id_, image, frame, sequence, flipped, back_color, style_); }

        iconWidget(const ::nh3api::dummy_tag_t&) noexcept
            : widget(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~iconWidget() noexcept
        { THISCALL_1(void, 0x4EA890, this); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(iconWidget)

        // vftable shift: +52
        virtual bool __thiscall handle_click(bool down_click, bool right_click) noexcept
        { return get_type_vftable(this)->handle_click(this, down_click, right_click); }

    public:
        // Set sprite frame index /
        // Установить текущий индекс кадра спрайта.
        void SetIconFrame(int32_t newFrame)
        { THISCALL_2(void, 0x4EB0D0, this, newFrame); }

        // Set sprite sequence /
        // Установить текущую последовательность кадров спрайта.
        void SetIconSequence(int32_t new_sequence)
        {
            this->seqId = new_sequence;
            this->Frame = 0;
        }

        // Set new sprite /
        // Поменять спрайт на новый.
        void SetSprite(const char* new_sprite)
        { THISCALL_2(void, 0x4EB110, this, new_sprite); }

        void NextRandomFrame()
        { THISCALL_1(void, 0x4EB140, this); }

        void NextRandomSiegeEngineFrame()
        { THISCALL_1(void, 0x4EB330, this); }

    public:
        // Sprite /
        // Спрайт.
        // offset: +0x30 = +48,  size = 0x4 = 4
        CSprite* Sprite;

        // Current sprite frame /
        // Текущий кадр спрайта.
        // offset: +0x34 = +52,  size = 0x4 = 4
        int32_t Frame;

        // Current sprite sequence /
        // Текущая последовательность кадров спрайта.
        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t seqId;

        // Sprite is flipped horizontally /
        // Спрайт перевёрнут горизонтально.
        // offset: +0x3C = +60,  size = 0x1 = 1
        bool IsFlipped;

    protected:
        [[maybe_unused]]
        // offset: +0x3D = +61,  size = 0x3 = 3
        std::byte gap_3D[3];

    public:
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t PostPostWalkSequence;

        // offset: +0x44 = +68,  size = 0x2 = 2
        uint16_t BackColor;

    protected:
        [[maybe_unused]]
        // offset: +0x46 = +70,  size = 0x2 = 2
        std::byte gap_46[2];

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_ICONWIDGET
#define NH3API_VIRTUAL_OVERRIDE_ICONWIDGET(CLASS_NAME) \
NH3API_VIRTUAL_OVERRIDE_WIDGET(CLASS_NAME) \
bool __thiscall handle_click(bool down_click, bool right_click) noexcept override\
{ return get_type_vftable(this)->handle_click(this, down_click, right_click); }
#endif

#pragma pack(push, 4)
// size = 0x50 = 80, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS textWidget : public widget
{
    public:
        struct vftable_t : public widget::vftable_t
        {
            void (__thiscall* SetText)(textWidget*, const char*);
        };

        textWidget() = delete;
        textWidget(const textWidget&)            = delete;
        textWidget(textWidget&&)                 = delete;
        textWidget& operator=(const textWidget&) = delete;
        textWidget& operator=(textWidget&&)      = delete;

    public:
        NH3API_FORCEINLINE
        textWidget(int32_t textWidgetX,
                   int32_t textWidgetY,
                   int32_t textWidgetWidth,
                   int32_t textWidgetHeight,
                   const char* textString,
                   const char* textFontName,
                   font::TColor color,
                   int32_t textWidgetId,
                   font::EJustify justify,
                   font::TColor back_color = font::TColor::WHITE) noexcept
        NH3API_DELEGATE_DUMMY(textWidget)
        { THISCALL_11(void, 0x5BC6A0, this, textWidgetX, textWidgetY, textWidgetWidth, textWidgetHeight, textString, textFontName, color, textWidgetId, justify, back_color); }

        NH3API_FORCEINLINE
        textWidget(const ::nh3api::dummy_tag_t& tag) noexcept
            : widget(tag), Text(tag)
        {}

        NH3API_FORCEINLINE
        ~textWidget() noexcept
        { THISCALL_1(void, 0x5BC7D0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(textWidget)

        // vftable shift: +52
        virtual void __thiscall SetText(const char* new_text)
        { get_type_vftable(this)->SetText(this, new_text); }

    public:
        union {
        // Widget text /
        // Текст виджета.
        // offset: +0x30 = +48,  size = 0x10 = 16
        exe_string Text;
        };

        // Widget font /
        // Шрифт виджета.
        // offset: +0x40 = +64,  size = 0x4 = 4
        font* Font;

        // offset: +0x44 = +68,  size = 0x4 = 4
        font::TColor Color;

        // offset: +0x48 = +72,  size = 0x4 = 4
        font::TColor BackColor;

        // Font justification /
        // Выравнивание текста.
        // offset: +0x4C = +76,  size = 0x4 = 4
        font::EJustify Justify;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_TEXTWIDGET
#define NH3API_VIRTUAL_OVERRIDE_TEXTWIDGET(CLASS_NAME) \
NH3API_VIRTUAL_OVERRIDE_WIDGET(CLASS_NAME) \
void __thiscall SetText(const char* new_text) override\
{ get_type_vftable(this)->SetText(this, new_text); }
#endif

#pragma pack(push, 4)
// Widget with editable text via input /
// Виджет с функцией ввода текста.
// size = 0x70 = 112, align = 4, baseclass: textWidget
NH3API_VIRTUAL_CLASS textEntryWidget : public textWidget
{
    public:
        struct vftable_t : public textWidget::vftable_t
        {
            void (__thiscall* SetFocus)(textEntryWidget*, bool);
            int32_t (__thiscall* OnKeyPress)(textEntryWidget*, message*);
            bool (__thiscall* IgnoreKey)(textEntryWidget*, message*);
            void (__thiscall* SetAutoDraw)(textEntryWidget*, bool);
            void (__thiscall* SaveBackground)(const textEntryWidget*);
        };

        textEntryWidget() = delete;
        textEntryWidget(const textEntryWidget&)            = delete;
        textEntryWidget(textEntryWidget&&)                 = delete;
        textEntryWidget& operator=(const textEntryWidget&) = delete;
        textEntryWidget& operator=(textEntryWidget&&)      = delete;

    public:
        textEntryWidget(int32_t textEntryWidgetX,
                        int32_t textEntryWidgetY,
                        int32_t textEntryWidgetWidth,
                        int32_t textEntryWidgetHeight,
                        int32_t textStringSize,
                        const char* textString,
                        const char* textFontName,
                        font::TColor color,
                        font::EJustify justification,
                        const char* backgroundIconName,
                        int32_t textBackgroundFrame,
                        int32_t textEntryWidgetId,
                        int32_t textEntryWidgetStyle,
                        int32_t iReadType,
                        int32_t textInsetX,
                        int32_t textInsetY) noexcept
            : textWidget(::nh3api::dummy_tag)
        { THISCALL_17(void, 0x5BACD0, this, textEntryWidgetX, textEntryWidgetY, textEntryWidgetWidth, textEntryWidgetHeight, textStringSize, textString, textFontName, color, justification, backgroundIconName, textBackgroundFrame, textEntryWidgetId, textEntryWidgetStyle, iReadType, textInsetX, textInsetY); }

        textEntryWidget(const ::nh3api::dummy_tag_t&) noexcept
            : textWidget(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~textEntryWidget() noexcept
        { THISCALL_1(void, 0x5BAE90, this); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_TEXTWIDGET(textEntryWidget)

        // vftable shift: +56
        virtual void __thiscall SetFocus(bool state)
        { get_type_vftable(this)->SetFocus(this, state); }

        // vftable shift: +60
        virtual int32_t __thiscall OnKeyPress(message& msg)
        { return get_type_vftable(this)->OnKeyPress(this, &msg); }

        // vftable shift: +64
        virtual bool __thiscall IgnoreKey(message& msg)
        { return get_type_vftable(this)->IgnoreKey(this, &msg); }

        // vftable shift: +68
        virtual void __thiscall SetAutoDraw(bool b)
        { get_type_vftable(this)->SetAutoDraw(this, b); }

        // vftable shift: +72
        virtual void __thiscall SaveBackground() const
        { get_type_vftable(this)->SaveBackground(this); }

    public:
        [[nodiscard]] char GetCharPressed(const message& msg)
        { return THISCALL_2(char, 0x5BAF50, this, &msg); }

    public:
        // offset: +0x50 = +80,  size = 0x4 = 4
        Bitmap816* textBack;

        // offset: +0x54 = +84,  size = 0x4 = 4
        CTextEntrySave* saveBack;

        // offset: +0x58 = +88,  size = 0x2 = 2
        uint16_t cursorIndex;

        // offset: +0x5A = +90,  size = 0x2 = 2
        uint16_t bufferSize;

        // offset: +0x5C = +92,  size = 0x2 = 2
        int16_t textWidth;

        // offset: +0x5E = +94,  size = 0x2 = 2
        int16_t textHeight;

        // offset: +0x60 = +96,  size = 0x2 = 2
        int16_t textX;

        // offset: +0x62 = +98,  size = 0x2 = 2
        int16_t textY;

        // offset: +0x64 = +100,  size = 0x2 = 2
        int16_t textLines;

        // offset: +0x66 = +102,  size = 0x2 = 2
        int16_t attributes;

        // offset: +0x68 = +104,  size = 0x2 = 2
        int16_t type;

        // offset: +0x6A = +106,  size = 0x2 = 2
        int16_t displayOffset;

        // offset: +0x6C = +108,  size = 0x1 = 1
        bool cursorFlashOn;

        // offset: +0x6D = +109,  size = 0x1 = 1
        bool focus;

        // offset: +0x6E = +110,  size = 0x1 = 1
        bool autoDraw;

    protected:
        [[maybe_unused]]
        std::byte gap_6D[1];

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_TEXTENTRYWIDGET
#define NH3API_VIRTUAL_OVERRIDE_TEXTENTRYWIDGET(CLASS_NAME) \
NH3API_VIRTUAL_OVERRIDE_TEXTWIDGET(CLASS_NAME) \
void __thiscall SetFocus(bool state) override\
{ get_type_vftable(this)->SetFocus(this, state); }\
int32_t __thiscall OnKeyPress(message& msg) override\
{ return get_type_vftable(this)->OnKeyPress(this, &msg); }\
bool __thiscall IgnoreKey(message& msg) override\
{ return get_type_vftable(this)->IgnoreKey(this, &msg); }\
void __thiscall SetAutoDraw(bool b) override\
{ get_type_vftable(this)->SetAutoDraw(this, b); }\
void __thiscall SaveBackground() const override\
{ get_type_vftable(this)->SaveBackground(this); }
#endif

#pragma pack(push, 4)
// size = 0x30 = 48, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS border : public widget
{
    public:
        struct vftable_t : public widget::vftable_t
        {
            bool (__thiscall* handle_click)(border*, bool, bool);
        };

    protected:
        border() noexcept
            : widget(::nh3api::dummy_tag)
        { THISCALL_1(void, 0x5FE9D0, this); }

    public:
        border(const border&)            = delete;
        border(border&&)                 = delete;
        border& operator=(const border&) = delete;
        border& operator=(border&&)      = delete;

        border(int32_t        x_,
               int32_t        y_,
               int32_t        w,
               int32_t        h,
               int32_t        id_,
               widget::ETypes style_) noexcept
            : widget(::nh3api::dummy_tag)
        { THISCALL_7(void, 0x44FBE0, this, x_, y_, w, h, id_, style_); }

        border(const ::nh3api::dummy_tag_t&) noexcept
            : widget(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~border() noexcept
        { THISCALL_1(void, 0x44FC20, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(border)

        // vftable shift: +52
        virtual bool __thiscall handle_click(bool down_click, bool right_click)
        { return get_type_vftable(this)->handle_click(this, down_click, right_click); }

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_BORDER
#define NH3API_VIRTUAL_OVERRIDE_BORDER(CLASS_NAME) \
NH3API_VIRTUAL_OVERRIDE_WIDGET(CLASS_NAME) \
bool __thiscall handle_click(bool down_click, bool right_click) override\
{ return get_type_vftable(this)->handle_click(this, down_click, right_click); }
#endif

#pragma pack(push, 4)
// size = 0x34 = 52, align = 4, baseclass: border
NH3API_VIRTUAL_CLASS bitmapBorder : public border
{
    public:
        struct vftable_t : public border::vftable_t{};

    protected:
        bitmapBorder() noexcept
            : border()
        {}

    public:
        bitmapBorder(const bitmapBorder&)            = delete;
        bitmapBorder(bitmapBorder&&)                 = delete;
        bitmapBorder& operator=(const bitmapBorder&) = delete;
        bitmapBorder& operator=(bitmapBorder&&)      = delete;

        bitmapBorder(int32_t        x_,
                     int32_t        y_,
                     int32_t        w,
                     int32_t        h,
                     int32_t        id_,
                     const char*    image,
                     widget::ETypes style_) noexcept
            : border(::nh3api::dummy_tag)
        { THISCALL_8(void, 0x44FFA0, this, x_, y_, w, h, id_, image, style_); }

        bitmapBorder(const ::nh3api::dummy_tag_t&) noexcept
            : border(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~bitmapBorder() noexcept
        { THISCALL_1(void, 0x450060, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_BORDER(bitmapBorder)

    public:
        void SetImage(const char* bitmap_name)
        { THISCALL_2(void, 0x450170, this, bitmap_name); }

        void SetPlayerPaletteColors(int32_t whichPlayer)
        { THISCALL_2(void, 0x4501D0, this, whichPlayer); }

    public:
        // offset: +0x30 = +48,  size = 0x4 = 4
        Bitmap816* borderBitmap16;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x34 = 52, align = 4, baseclass: border
NH3API_VIRTUAL_CLASS coloredBorderFrame : public border
{
    public:
        struct vftable_t : public border::vftable_t {};

    protected:
        coloredBorderFrame() noexcept
            : border()
        {}

    public:
        coloredBorderFrame(const coloredBorderFrame&)            = delete;
        coloredBorderFrame(coloredBorderFrame&&)                 = delete;
        coloredBorderFrame& operator=(const coloredBorderFrame&) = delete;
        coloredBorderFrame& operator=(coloredBorderFrame&&)      = delete;

        coloredBorderFrame(int32_t        x_,
                           int32_t        y_,
                           int32_t        w,
                           int32_t        h,
                           int32_t        id_,
                           uint32_t       color_,
                           widget::ETypes style_ = widget::COLORED_BORDER) noexcept
            : border(::nh3api::dummy_tag)
        { THISCALL_8(void, 0x44FE00, this, x_, y_, w, h, id_, color_, style_); }

        coloredBorderFrame(const ::nh3api::dummy_tag_t&) noexcept
            : border(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~coloredBorderFrame() noexcept
        { THISCALL_1(void, 0x44FEA0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_BORDER(coloredBorderFrame)

    public:
        // offset: +0x30 = +48,  size = 0x4 = 4
        int32_t color;
};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x54 = 84, align = 4, baseclass: textWidget
NH3API_VIRTUAL_CLASS bitmapBackedTextWidget : public textWidget
{
    public:
        struct vftable_t : public textWidget::vftable_t {};

        bitmapBackedTextWidget() NH3API_DELETED_FUNCTION

        bitmapBackedTextWidget(const bitmapBackedTextWidget&)            = delete;
        bitmapBackedTextWidget(bitmapBackedTextWidget&&)                 = delete;
        bitmapBackedTextWidget& operator=(const bitmapBackedTextWidget&) = delete;
        bitmapBackedTextWidget& operator=(bitmapBackedTextWidget&&)      = delete;

    public:
        bitmapBackedTextWidget(int32_t x_,
                               int32_t y_,
                               int32_t w,
                               int32_t h,
                               const char* text,
                               const char* font,
                               const char* back,
                               font::TColor color,
                               int32_t id_,
                               font::EJustify justify,
                               widget::ETypes style_ = widget::TEXT_WIDGET) noexcept
            : textWidget(::nh3api::dummy_tag)
        { THISCALL_12(void, 0x5BCB70, this, x_, y_, w, h, text, font, back, color, id_, justify, style_); }

        bitmapBackedTextWidget(const ::nh3api::dummy_tag_t&) noexcept
            : textWidget(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~bitmapBackedTextWidget() noexcept
        { THISCALL_1(void, 0x5BCAE0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_TEXTWIDGET(bitmapBackedTextWidget)

    public:
        // offset: +0x50 = +80,  size = 0x4 = 4
        Bitmap816* Background;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Button /
// кнопка.
// size = 0x68 = 104, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS button : public widget
{
    public:
        struct vftable_t : public widget::vftable_t {};

    public:
        enum TButtonStates : int32_t
        {
            eButtonNormal      = 0,
            eButtonSelected    = 1,
            eButtonDisabled    = 2,
            eButtonHighlighted = 3,
        };

        button() = delete;
        button(const button&)            = delete;
        button(button&&)                 = delete;
        button& operator=(const button&) = delete;
        button& operator=(button&&)      = delete;

    public:
        button(int32_t x_,
               int32_t y_,
               int32_t w,
               int32_t h,
               int32_t id_,
               const char* image,
               int32_t normal = 0,
               int32_t selected = 1,
               bool end = false,
               EKeyCodes hotkey = KEYCODE_NONE,
               widget::ETypes style_ = widget::BUTTON) noexcept
        NH3API_DELEGATE_DUMMY(button)
        { THISCALL_12(void, 0x455BD0, this, x_, y_, w, h, id_, image, normal, selected, end, hotkey, style_); }

        button(const ::nh3api::dummy_tag_t& tag) noexcept
            : widget(tag), hotKeyCodes(tag), Text(tag)
        {}

        NH3API_FORCEINLINE
        ~button() noexcept
        { THISCALL_1(void, 0x455DD0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(button)

    public:
        void SetPlayerPaletteColors(int32_t whichPlayer)
        { THISCALL_2(void, 0x4566C0, this, whichPlayer); }

        void set_hotkey(EKeyCodes key)
        { THISCALL_2(void, 0x404230, this, key); }

    public:
        // Button sprite /
        // Спрайт кнопки.
        // offset: +0x30 = +48,  size = 0x4 = 4
        CSprite* buttonIcon;

        // Neutral state frame /
        // Кадр нейтрального состояния кнопки.
        // offset: +0x34 = +52,  size = 0x4 = 4
        int32_t normalFrame;

        // Pressed frame /
        // Кадр нажатого состояния кнопки.
        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t selectedFrame;

        // Disabled frame /
        // Кадр заблокированного состояния кнопки.
        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t disabled_frame;

        // Highlighted button frame /
        // Кадр выделенного состояния кнопки.
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t highlightedFrame;

    protected:
        [[maybe_unused]]
        // offset: +0x44 = +68,  size = 0x1 = 1
        bool _end;

        [[maybe_unused]]
        std::byte gap_45[3];

    public:
        union {
        // Hotkeys that activate the button /
        // Горячие клавиши, активирующие кнопку.
        // offset: +0x48 = +72,  size = 0x10 = 16
        exe_vector<EKeyCodes> hotKeyCodes;
        };

        union {
        // Текст кнопки /
        // button caption
        // offset: +0x58 = +88,  size = 0x10 = 16
        exe_string Text;
        };

};
#pragma pack(pop)

#pragma pack(push, 4)
// Button with text /
// кнопка с текстом.
// size = 0x74 = 116, align = 4, baseclass: button
NH3API_VIRTUAL_CLASS textButton : public button
{
    public:
        struct vftable_t : public button::vftable_t {};

        textButton() = delete;
        textButton(const textButton&)            = delete;
        textButton(textButton&&)                 = delete;
        textButton& operator=(const textButton&) = delete;
        textButton& operator=(textButton&&)      = delete;

    public:
        textButton(int32_t x_,
                   int32_t y_,
                   int32_t w,
                   int32_t h,
                   int32_t id_,
                   const char* image,
                   const char* text_,
                   const char* font_name,
                   int32_t normal = 0,
                   int32_t selected = 1,
                   bool end = false,
                   EKeyCodes hotkey = KEYCODE_NONE,
                   widget::ETypes style_ = widget::TEXT_BUTTON,
                   font::TColor new_color = font::TColor::WHITE) noexcept
            : button(::nh3api::dummy_tag)
        { THISCALL_15(void, 0x456730, this, x_, y_, w, h, id_, image, text_, font_name, normal, selected, end, hotkey, style_, new_color); }

        textButton(const ::nh3api::dummy_tag_t&) noexcept
            : button(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~textButton() noexcept
        { THISCALL_1(void, 0x4568D0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(textButton)

    public:
        // Button font /
        // Шрифт кнопки.
        // offset: +0x68 = +104,  size = 0x4 = 4
        font* Font;

        // Button text color /
        // Цвет текста кнопки
        // offset: +0x6C = +108,  size = 0x4 = 4
        font::TColor textColor;

        // offset: +0x70 = +112,  size = 0x4 = 4
        Bitmap16Bit* textImage;
};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x6C = 108, align = 4, baseclass: button
NH3API_VIRTUAL_CLASS type_func_button : public button
{
    public:
        struct vftable_t : public button::vftable_t {};

        type_func_button() = delete;
        type_func_button(const type_func_button&)            = delete;
        type_func_button(type_func_button&&)                 = delete;
        type_func_button& operator=(const type_func_button&) = delete;
        type_func_button& operator=(type_func_button&&)      = delete;

    public:
        NH3API_CALLBACK(callback, -1)
        type_func_button(int32_t x_,
                         int32_t y_,
                         int32_t width_,
                         int32_t height_,
                         int32_t id_,
                         const char* image_,
                         int32_t (__fastcall* callback)(message*),
                         int32_t normal_ = 0,
                         int32_t selected_ = 1) noexcept
            : button(::nh3api::dummy_tag)
        { THISCALL_10(void, 0x456A10, this, x_, y_, width_, height_, id_, image_, callback, normal_, selected_); }

        type_func_button(const ::nh3api::dummy_tag_t&) noexcept
            : button(::nh3api::dummy_tag)
        {}

        NH3API_FORCEINLINE
        ~type_func_button() noexcept
        { THISCALL_1(void, 0x456A90, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(type_func_button)

    public:
        // Callback /
        // Функция, вызываемая при нажатии на кнопку.
        // offset: +0x68 = +104,  size = 0x4 = 4
        int32_t (__fastcall* handler)(message*);

};
#pragma pack(pop)

using TSliderFunction = int32_t(__fastcall*)(int32_t, heroWindow*);
#pragma pack(push, 4)
// size = 0x68 = 104, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS slider : public widget
{
    public:
        struct vftable_t : widget::vftable_t
        {
            void (__thiscall *SetResolution)(slider*, int32_t);
            void (__thiscall *SetState)(slider*, int32_t);
            void (__thiscall *UpdateResolution)(slider*, int32_t);
            void (__thiscall *Refresh)(slider*); // overriden only in type_text_slider
        };

    public:
        enum EGraphics : uint32_t
        {
            BROWN = 0,
            BLUE = 1
        };

    public:
        NH3API_FORCEINLINE
        slider(int32_t x_,
               int32_t y_,
               int32_t w,
               int32_t h,
               int32_t id_,
               int32_t num_,
               TSliderFunction func,
               slider::EGraphics graphics,
               int32_t page,
               bool hotKey) noexcept
        NH3API_DELEGATE_DUMMY_BASE(slider)
        { THISCALL_11(void, 0x5963C0, this, x_, y_, w, h, id_, num_, func, graphics, page, hotKey); }

        NH3API_FORCEINLINE
        slider(const ::nh3api::dummy_tag_t& tag) noexcept
            : widget(tag)
        {}

        NH3API_FORCEINLINE
        ~slider() noexcept
        { THISCALL_1(void, 0x596470, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(slider)

        // vftable shift: +52
        virtual void __thiscall SetResolution(int32_t arg)
        { get_type_vftable(this)->SetResolution(this, arg); }

        // vftable shift: +56
        virtual void __thiscall SetState(int32_t state)
        { get_type_vftable(this)->SetState(this, state); }

        // vftable shift: +60
        virtual void __thiscall UpdateResolution(int32_t arg)
        { get_type_vftable(this)->UpdateResolution(this, arg); }

        // vftable shift: +64
        virtual void __thiscall Refresh()
        { get_type_vftable(this)->Refresh(this); }

    public:
        // offset: +0x30 = +48,  size = 0x4 = 4
        CSprite* sliderSprite;

        // offset: +0x34 = +52,  size = 0x4 = 4
        Bitmap816* sliderBitmap;

        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t oldState;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t knobRange;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t length;

        // offset: +0x44 = +68,  size = 0x4 = 4
        int32_t knob_start;

        // offset: +0x48 = +72,  size = 0x4 = 4
        int32_t num;

        // offset: +0x4C = +76,  size = 0x4 = 4
        int32_t clickX;

        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t clickY;

        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t someX;

        // offset: +0x58 = +88,  size = 0x4 = 4
        int32_t someY;

        // offset: +0x5C = +92,  size = 0x1 = 1
        bool hotKeys;

        // offset: +0x5D = +93,  size = 0x1 = 1
        bool scrolling;

    protected:
        [[maybe_unused]]
        std::byte gap_5E[2];

    public:
        // offset: +0x60 = +96,  size = 0x4 = 4
        int32_t lastFocus;

        // offset: +0x64 = +100,  size = 0x4 = 4
        TSliderFunction sliderFunction;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_SLIDER
#define NH3API_VIRTUAL_OVERRIDE_SLIDER(CLASS_NAME) \
NH3API_VIRTUAL_OVERRIDE_WIDGET(CLASS_NAME) \
void __thiscall SetResolution(int32_t arg) override \
{ get_type_vftable(this)->SetResolution(this, arg); } \
void __thiscall SetState(int32_t state) override \
{ get_type_vftable(this)->SetState(this, state); } \
void __thiscall UpdateResolution(int32_t arg) override \
{ get_type_vftable(this)->UpdateResolution(this, arg); } \
void __thiscall Refresh() override \
{ get_type_vftable(this)->Refresh(this); }
#endif

class type_text_slider;
#pragma pack(push, 4)
// size = 0x5C = 92, align = 4, baseclass: widget
NH3API_VIRTUAL_CLASS type_text_scroller : public widget
{
    public:
        NH3API_FORCEINLINE
        type_text_scroller(const char* text,
                           int32_t x_,
                           int32_t y_,
                           int32_t w,
                           int32_t h,
                           const char* font_name,
                           font::TColor color,
                           slider::EGraphics graphics) noexcept
        NH3API_DELEGATE_DUMMY(type_text_scroller)
        { THISCALL_9(void, 0x5BA360, this, text, x_, y_, w, h, font_name, color, graphics); }

        NH3API_FORCEINLINE
        type_text_scroller(const ::nh3api::dummy_tag_t& tag) noexcept
            : widget(tag), lines(tag), line_images(tag)
        {}

        NH3API_FORCEINLINE
        ~type_text_scroller() noexcept
        { THISCALL_1(void, 0x5BA760, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_WIDGET(type_text_scroller)

    public:
        // offset: +0x30 = +48,  size = 0x4 = 4
        const char* font_filename;

        union {
        // offset: +0x34 = +52,  size = 0x10 = 16
        exe_vector<exe_string> lines;
        };

        union {
        // offset: +0x44 = +68,  size = 0x10 = 16
        exe_vector<textWidget*> line_images;
        };

        // offset: +0x54 = +84,  size = 0x4 = 4
        type_text_slider* text_slider;

        // offset: +0x58 = +88,  size = 0x4 = 4
        Bitmap16Bit* background;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x6C = 108, align = 4, baseclass: slider
NH3API_VIRTUAL_CLASS type_text_slider : public slider
{
    public:
        NH3API_FORCEINLINE
        type_text_slider(type_text_scroller* parent,
                         int32_t x_,
                         int32_t y_,
                         int32_t w,
                         int32_t h,
                         int32_t id_,
                         int32_t num_,
                         slider::EGraphics graphics,
                         int32_t page,
                         bool hotKey) noexcept
            : slider(x_, y_, w, h, id_, num_, nullptr, graphics, page, hotKey),
              scroller(parent)
        {
            NH3API_SET_VFTABLE();
        }

        NH3API_FORCEINLINE
        type_text_slider(const ::nh3api::dummy_tag_t& tag) noexcept
            : slider(tag)
        {}

        NH3API_DEFAULT_DESTRUCTOR(type_text_slider)

    public:
        NH3API_VIRTUAL_OVERRIDE_SLIDER(type_text_slider)

    public:
        // offset: +0x68 = +104,  size = 0x4 = 4
        type_text_scroller* scroller;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x70 = 112, align = 4, baseclass: textEntryWidget
NH3API_VIRTUAL_CLASS CChatEdit : public textEntryWidget
{
    public:
        struct vftable_t : public textEntryWidget::vftable_t
        {
            void (__thiscall* UpdateScreen)(CChatEdit*);
            int32_t (__thiscall* OnEnter)(CChatEdit*, message); // copy message by value
            int32_t (__thiscall* OnEscape)(CChatEdit*, message); // copy message by value
            int32_t (__thiscall* OnFunctionKey)(CChatEdit*, message, int32_t); // copy message by value
            bool (__thiscall* IsOpen)(const CChatEdit*);
            void (__thiscall* SendChat)(CChatEdit*, const char*, int32_t);
        };

        CChatEdit() NH3API_DELETED_FUNCTION

    public:
        CChatEdit(int32_t textWidgetX,
                  int32_t textWidgetY,
                  int32_t textWidgetWidth,
                  int32_t textWidgetHeight,
                  int32_t textStringSize,
                  const char* textString,
                  const char* textFontName,
                  font::TColor colorIndex,
                  font::EJustify justification,
                  const char* backgroundIconName,
                  int32_t backgroundFrame,
                  int32_t textWidgetId,
                  int32_t textWidgetStyle,
                  int32_t iReadType,
                  int32_t textInsetX,
                  int32_t textInsetY) noexcept
            : textEntryWidget(::nh3api::dummy_tag)
        { THISCALL_17(void, 0x5545A0, this, textWidgetX, textWidgetY, textWidgetWidth, textWidgetHeight, textStringSize, textString, textFontName, colorIndex, justification, backgroundIconName, backgroundFrame, textWidgetId, textWidgetStyle, iReadType, textInsetX, textInsetY); }

        CChatEdit(const ::nh3api::dummy_tag_t&) noexcept
            : textEntryWidget(::nh3api::dummy_tag)
        {}

        NH3API_DEFAULT_DESTRUCTOR(CChatEdit)

    public:
        NH3API_VIRTUAL_OVERRIDE_TEXTENTRYWIDGET(CChatEdit)

        // vftable shift: +76
        virtual void __thiscall UpdateScreen() noexcept
        { get_type_vftable(this)->UpdateScreen(this); }

        // vftable shift: +80
        virtual int32_t __thiscall OnEnter(message msg) noexcept
        { return get_type_vftable(this)->OnEnter(this, msg); }

        // vftable shift: +84
        virtual int32_t __thiscall OnEscape(message msg) noexcept
        { return get_type_vftable(this)->OnEscape(this, msg); }

        // vftable shift: +88
        virtual int32_t __thiscall OnFunctionKey(message msg, int32_t toWho) noexcept
        { return get_type_vftable(this)->OnFunctionKey(this, msg, toWho); }

        // vftable shift: +92
        [[nodiscard]] virtual bool __thiscall IsOpen() const noexcept
        { return get_type_vftable(this)->IsOpen(this); }

        // vftable shift: +96
        virtual void __thiscall SendChat(const char* sChat, int32_t toWho) noexcept
        { get_type_vftable(this)->SendChat(this, sChat, toWho); }

};
#pragma pack(pop)

NH3API_FORCEINLINE
void SetPlayerPaletteColors(uint16_t* pPalette, int32_t whichPlayer)
{
    if ( whichPlayer >= 0 && whichPlayer < 8 )
        FASTCALL_2(void, 0x6003E0, pPalette, whichPlayer);
}

NH3API_FORCEINLINE
void SetPlayerPaletteColors(TPalette24& pPalette, int32_t whichPlayer)
{
    if ( whichPlayer >= 0 && whichPlayer < 8 )
        FASTCALL_2(void, 0x600400, &pPalette, whichPlayer);
}

NH3API_DISABLE_WARNING_END

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63EC48, iconWidget)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642DC0, textWidget)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642D50, textEntryWidget)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BA24, border)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BA94, bitmapBorder)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BA5C, coloredBorderFrame)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642DF8, bitmapBackedTextWidget)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BB54, button)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BB88, textButton)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BBBC, type_func_button)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x641D60, slider)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642D1C, type_text_scroller)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642CD8, type_text_slider)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x640E40, CChatEdit)
