//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "../resources/resources.hpp"
#include "../events.hpp"

// implementation of no-op constructors with dummy_tag
NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

class heroWindow;
#pragma pack(push, 4)
// widget base class /
// Базовый класс - элемент диалоговой системы(виджет).
// size = 0x30 = 48, align = 4
NH3API_VIRTUAL_CLASS widget
{
    public:
        struct vftable_t
        {
            void* (__thiscall* scalar_deleting_destructor)(widget*, uint8_t);
            int32_t (__thiscall* Open)(widget*, int32_t, heroWindow*);
            int32_t (__thiscall* Main)(widget*, message*);
            void (__thiscall* zBufferDraw)(const widget*, uint16_t*, int32_t);
            void (__thiscall* Draw)(const widget*);
            int32_t (__thiscall* GetRealHeight)(const widget*);
            int32_t (__thiscall* GetRealWidth)(const widget*);
            void (__thiscall* process_hover)(widget*);
            void (__thiscall* Dim)(const widget*);
            void (__thiscall* enable)(widget*, bool);
            void (__thiscall* OnSetFocus)(widget*);
              void (__thiscall* OnKillFocus)(widget*);
            void (__thiscall* sleep)(widget*, bool);
        };

    public:
        enum ETypes : uint16_t
        {
            NULL_WIDGET          = 0, //
            BORDER               = 1, //
            BUTTON               = 2, //
            TEXT_BUTTON          = 3, //
            DRAGBAR              = 4, //
            TEXT_WIDGET          = 8, //
            ICON_WIDGET          = 16, //
            ICON_WIDGET_XC_YBM2  = 17, //
            ICON_WIDGET_CREATURE = 18, //
            UPDATE_WIDGET        = 32, //
            DIMMER_WIDGET        = 64, //
            MONO_WIDGET          = 128, //
            TEXT_ENTRY_WIDGET    = 256, //
            COLORED_BORDER       = 1024, //
            BITMAP_BORDER        = 2048, //
            HIT_SELECT_BUTTON    = 4096, //
            REL_VERIFY_BUTTON    = 8192, //
            USER                 = 16384, //
        };

        enum EQualifiers : uint32_t
        {
            WIDGET_SINGLE_CLICK = 1, //
            WIDGET_DOUBLE_CLICK = 2, //
        };

        enum EReturnCodes : uint32_t
        {
            WIDGET_END_DIALOG   = 10, //
            WIDGET_SELECT       = 12, //
            WIDGET_DESELECT     = 13, //
            WIDGET_RIGHT_SELECT = 14, //
        };

        enum ECommands : uint32_t
        {
            WIDGET_ACTIVATE                  = 1, //
            WIDGET_DRAW                      = 2, //
            WIDGET_SET_TEXT                  = 3, //
            WIDGET_SET_ICON_FRAME            = 4, //
            WIDGET_SET_STATUS                = 5, //
            WIDGET_CLEAR_STATUS              = 6, //
            WIDGET_GET_TEXT                  = 7, //
            WIDGET_SET_ICON_COLOR            = 8, //
            WIDGET_SET_COLOR                 = 8, //
            WIDGET_SET_ICON_NAME             = 9, //
            WIDGET_SET_PALETTE               = 10, //
            WIDGET_SET_IMAGE                 = 11, //
            WIDGET_SET_ICON_SEQUENCE         = 12, //
            WIDGET_SET_PLAYER_PALETTE_COLORS = 13, //
            WIDGET_SET_SLIDER_STATE          = 49, //
            WIDGET_SET_SLIDER_RESOLUTION     = 50, //
            WIDGET_SET_TEXT_LEN              = 51, //
            WIDGET_SET_X                     = 52, //
            WIDGET_SET_Y                     = 53, //
            WIDGET_SET_ITEM                  = 54, //
            WIDGET_GET_ITEM                  = 55, //
            WIDGET_ADD_ITEM                  = 56, //
            WIDGET_CHANGE_ITEM               = 57, //
            WIDGET_DELETE_ITEM               = 58, //
            WIDGET_DELETE_ALL_ITEMS          = 59, //
            WIDGET_SET_WIDTH                 = 61, //
            WIDGET_SET_HEIGHT                = 62, //
            WIDGET_SET_COLORIZE              = 63, //
            WIDGET_SET_FOCUS                 = 64, //
        };

        enum EStatusFlags : uint16_t
        {
            WIDGET_STATUS_MASK   = 65535, //
            WIDGET_SELECTED      = 1, //
            WIDGET_ACTIVE        = 2, //
            WIDGET_DRAWN         = 4, //
            WIDGET_DIMMED        = 8, //
            WIDGET_HIGHLIGHTED   = 16, //
            WIDGET_DISABLED      = 32, //
            WIDGET_DIMMED_NODRAW = 4096, //
            WIDGET_ASLEEP        = 8192, //
            WIDGET_UPDATE        = 16384, //
        };

    protected:
        widget() NH3API_DELETED_FUNCTION

    // constructors and destructors
    protected:
        widget(int16_t x,
               int16_t y,
               int16_t w,
               int16_t h,
               int16_t id,
               ETypes style) NH3API_NOEXCEPT
        { THISCALL_7(void, 0x5FE900, this, x, y, w, h, id, static_cast<int16_t>(style)); }

    public:
        widget(const nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {
            NH3API_IGNORE(parentWindow,
                          prevWidget,
                          nextWidget,
                          id,
                          priority,
                          style,
                          status,
                          x,
                          y,
                          width,
                          height,
                          RollOver,
                          RightClick,
                          freeText,
                          sleepCount);
        }

        ~widget() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x575DF0, this); }

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall Open(int32_t newPriority, heroWindow* parent)
        { return get_type_vftable(this)->Open(this, newPriority, parent); }

        // vftable shift: +8
        virtual int32_t __thiscall Main(message* msg)
        { return get_type_vftable(this)->Main(this, msg); }

        // vftable shift: +12
        virtual void __thiscall zBufferDraw(uint16_t* zBuffer, int32_t id) const
        { get_type_vftable(this)->zBufferDraw(this, zBuffer, id); }

        // vftable shift: +16
        virtual void __thiscall Draw() const
        { get_type_vftable(this)->Draw(this); }

        // vftable shift: +20
        virtual int32_t __thiscall GetRealHeight() const
        { return get_type_vftable(this)->GetRealHeight(this); }

        // vftable shift: +24
        virtual int32_t __thiscall GetRealWidth() const
        { return get_type_vftable(this)->GetRealWidth(this); }

        // vftable shift: +28
        virtual void __thiscall process_hover()
        { get_type_vftable(this)->process_hover(this); }

        // vftable shift: +32
        virtual void __thiscall Dim() const
        { get_type_vftable(this)->Dim(this); }

        // vftable shift: +36
        // Enable the widget /
        // Включить виджет.
        virtual void __thiscall enable(bool arg)
        { get_type_vftable(this)->enable(this, arg); }

        // vftable shift: +40
        virtual void __thiscall OnSetFocus()
        { get_type_vftable(this)->OnSetFocus(this); }

        // vftable shift: +44
        virtual void __thiscall OnKillFocus()
        { get_type_vftable(this)->OnKillFocus(this); }

        // vftable shift: +48
        virtual void __thiscall sleep(bool arg)
        { get_type_vftable(this)->sleep(this, arg); }

    // public methods
    public:
        int32_t set_visible(bool arg)
        { return THISCALL_2(int32_t, 0x562C40, this, arg); }

        int32_t send_message(ECommands command, int32_t extra)
        { return THISCALL_3(int32_t, 0x5FED80, this, command, extra); }

        void set_help_text(const char* text, const char* rclick, bool copyText)
        { THISCALL_4(void, 0x5FEE00, this, text, rclick, copyText); }

    // static variables
    public:
        static widget*& last_hover_widget;

    // member variables
    public:
        // Widget's parent window /
        // Родительское окно, к которому привязан текущий виджет.
        // offset: +0x4 = +4,  size = 0x4 = 4
        heroWindow* parentWindow;

        // Previous widget of parentWindow /
        // Предыдущий виджет родительского окна.
        // offset: +0x8 = +8,  size = 0x4 = 4
        widget* prevWidget;

        // Next widget of parentWindow /
        // Следующий виджет родительского окна.
        // offset: +0xC = +12,  size = 0x4 = 4
        widget* nextWidget;

        // widget's ID
        // ID текущего виджета относительно parentWindow.
        // offset: +0x10 = +16,  size = 0x2 = 2
        int16_t id;

        // Draw priority /
        // Приоритет отрисовки.
        // offset: +0x12 = +18,  size = 0x2 = 2
        int16_t priority;

        // Widget type /
        // Тип виджета.
        // offset: +0x14 = +20,  size = 0x2 = 2
        ETypes style;

        // Widget status /
        // Состояние виджета.
        // offset: +0x16 = +22,  size = 0x2 = 2
        EStatusFlags status;

        // x coordinate /
        // x-координата.
        // offset: +0x18 = +24,  size = 0x2 = 2
        int16_t x;

        // y coordinate /
        // y-координата.
        // offset: +0x1A = +26,  size = 0x2 = 2
        int16_t y;

        // Width /
        // Ширина.
        // offset: +0x1C = +28,  size = 0x2 = 2
        int16_t width;

        // Height /
        // Высота.
        // offset: +0x1E = +30,  size = 0x2 = 2
        int16_t height;

        // Mouse rollover text /
        // Текст при наведении мыши.
        // offset: +0x20 = +32,  size = 0x4 = 4
        const char* RollOver;

        // Right click text /
        // Текст при нажатии ПКМ по виджету.
        // offset: +0x24 = +36,  size = 0x4 = 4
        const char* RightClick;

        // 'RollOver' and 'RightClick' were allocated /
        // Память под 'RollOver' и 'RightClick' была выделена.
        // offset: +0x28 = +40,  size = 0x1 = 1
        bool freeText;

        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t sleepCount;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_WIDGET
#define NH3API_VIRTUAL_OVERRIDE_WIDGET(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag)  override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
virtual int32_t __thiscall Open(int32_t newPriority, heroWindow* parent)  override\
{ return get_type_vftable(this)->Open(this, newPriority, parent); }\
virtual int32_t __thiscall Main(message* msg)  override\
{ return get_type_vftable(this)->Main(this, msg); }\
virtual void __thiscall zBufferDraw(uint16_t* zBuffer, int32_t id) const  override\
{ get_type_vftable(this)->zBufferDraw(this, zBuffer, id); }\
virtual void __thiscall Draw() const  override\
{ get_type_vftable(this)->Draw(this); }\
virtual int32_t __thiscall GetRealHeight() const  override\
{ return get_type_vftable(this)->GetRealHeight(this); }\
virtual int32_t __thiscall GetRealWidth() const  override\
{ return get_type_vftable(this)->GetRealWidth(this); }\
virtual void __thiscall process_hover()  override\
{ get_type_vftable(this)->process_hover(this); }\
virtual void __thiscall Dim() const  override\
{ get_type_vftable(this)->Dim(this); }\
virtual void __thiscall enable(bool arg)  override\
{ get_type_vftable(this)->enable(this, arg); }\
virtual void __thiscall OnSetFocus()  override\
{ get_type_vftable(this)->OnSetFocus(this); }\
virtual void __thiscall OnKillFocus()  override\
{ get_type_vftable(this)->OnKillFocus(this); }\
virtual void __thiscall sleep(bool arg)  override\
{ get_type_vftable(this)->sleep(this, arg); }
#endif

widget*& widget::last_hover_widget = get_global_var_ref(0x6AACD0, widget*);
typedef exe_vector<widget*> TWidgetVector;

#pragma pack(push, 4)
// Window base class /
// Базовый класс - окно.
// size = 0x4C = 76, align = 4
NH3API_VIRTUAL_CLASS heroWindow
{
    public:
        struct vftable_t
        {
            void (__thiscall* scalar_deleting_destructor)(heroWindow*, uint8_t);
            int32_t (__thiscall* Open)(heroWindow*, int32_t, bool);
            void (__thiscall* Close)(heroWindow*, bool);
            int32_t (__thiscall* handle_message)(heroWindow*, message*);
            void (__thiscall* handle_widget_hover)(heroWindow*, widget*);
            void (__thiscall* DrawWindow)(heroWindow*, bool, int32_t,
                                        int32_t);
            void (__thiscall* DoModal)(heroWindow*, bool);
            void (__thiscall* AddWidgetsToMessageStream)(heroWindow*);
            void (__thiscall* sleep)(heroWindow*, bool);
        };

    public:
        enum : int32_t
        {
            MIN_WIDGET_ID     = -65535, //
            MAX_WIDGET_ID     = 65535, //
            DropShadowXOffset = 8, //
            DropShadowYOffset = 8, //
        };

        enum TStatus : int32_t
        {
            NO_STATUS = 0, //
            VISIBLE   = 1, //
            CORRUPT   = 2, //
        };

        enum TAttribute : uint32_t
        {
            NORMAL      = 0, //
            BACKDROP    = 1, //
            SAVEBACK    = 2, //
            DIALOG      = 4, //
            NOSAVEBACK  = 8, //
            DROP_SHADOW = 16, //
        };

    protected:
        heroWindow() NH3API_DELETED_FUNCTION

    public:
        heroWindow(int32_t winX,
                   int32_t winY,
                   int32_t winWidth,
                   int32_t winHeight,
                   TAttribute winType) NH3API_NOEXCEPT
        { THISCALL_6(void, 0x5FEFB0, this, winX, winY, winWidth, winHeight, winType); }

        heroWindow(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Widgets(tag)
        {
            NH3API_IGNORE(priority,
                          nextWindow,
                          prevWindow,
                          type,
                          status,
                          x,
                          y,
                          width,
                          height,
                          headWidget,
                          tailWidget,
                          focusId,
                          background,
                          sleepCount);
        }

    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall Open(int32_t newPriority, bool update)
        { return get_type_vftable(this)->Open(this, newPriority, update); }

        // vftable shift: +8
        virtual void __thiscall Close(bool update)
        { get_type_vftable(this)->Close(this, update); }

        // vftable shift: +12
        virtual int32_t __thiscall handle_message(message& msg)
        { return get_type_vftable(this)->handle_message(this, &msg); }

        // vftable shift: +16
        virtual void __thiscall handle_widget_hover(widget* current_widget)
        { get_type_vftable(this)->handle_widget_hover(this, current_widget); }

        // vftable shift: +20
        virtual void __thiscall DrawWindow(bool update, int32_t iLowID, int32_t iHighID)
        { get_type_vftable(this)->DrawWindow(this, update, iLowID, iHighID); }

        // vftable shift: +24
        virtual void __thiscall DoModal(bool fadeIn)
        { get_type_vftable(this)->DoModal(this, fadeIn); }

        // vftable shift: +28
        virtual void __thiscall AddWidgetsToMessageStream()
        { get_type_vftable(this)->AddWidgetsToMessageStream(this); }

        // vftable shift: +32
        virtual void __thiscall sleep(bool arg)
        { get_type_vftable(this)->sleep(this, arg); }

    public:
        int32_t BroadcastMessage(message::ECommandType command,
                                 widget::ECommands subtype,
                                 int32_t itemID,
                                 uint32_t extra)
        { return THISCALL_5(int32_t, 0x5FF400, this, command, subtype, itemID, extra); }

        int32_t BroadcastMessage(message& msg)
        { return THISCALL_2(int32_t, 0x5FF3A0, this, &msg); }

        int32_t WidgetSetStatus(int32_t id, widget::EStatusFlags status)
        { return THISCALL_3(int32_t, 0x5FF490, this, id, status); }

        int32_t WidgetClearStatus(int32_t id, widget::EStatusFlags status)
        { return THISCALL_3(int32_t, 0x5FF520, this, id, status); }

        widget* GetWidget(int32_t widget_id)
        { return THISCALL_2(widget*, 0x5FF5B0, this, widget_id); }

        void CenterWindow(int32_t centerX, int32_t centerY)
        { THISCALL_3(void, 0x5FF800, this, centerX, centerY); }

        // Find widget ID by (mx, my) coordinates /
        // Найти ID виджета по координатам (mx, my).
        int32_t findWidget(int32_t mx, int32_t my)
        { return THISCALL_3(int32_t, 0x5FF970, this, mx, my); }

        // Find widget by (mx, my) coordinates /
        // Найти виджета по координатам (mx, my).
        widget* findWidgetPtr(int32_t mx, int32_t my)
        { return THISCALL_3(widget*, 0x5FF9A0, this, mx, my); }

        void SetFocus(int32_t id)
        { THISCALL_2(void, 0x5FFA50, this, id); }

        // Delete all window widgets /
        // Удалить все виджеты окна.
        void delete_widgets()
        { THISCALL_1(void, 0x5FFAD0, this); }

        // Add <newWidget> with <newPriority> /
        // Добавить виджет <newWidget> с приоритетом отрисовки <newPriority>.
        void AddWidget(widget* newWidget, int32_t newPriority)
        { THISCALL_3(void, 0x5FF270, this, newWidget, newPriority); }

        void RemoveWidget(widget* w)
        { THISCALL_2(void, 0x5FF320, this, w); }

        void RestoreBackground(bool update)
        { THISCALL_2(void, 0x5FF780, this, update); }

        int32_t SaveBackground()
        { return THISCALL_1(int32_t, 0x5FF6C0, this); }

        // Set widget position /
        // Установить позицию виджета.
        void WidgetSetPos(int32_t id, int32_t new_x, int32_t new_y)
        { WidgetSetPos(GetWidget(id), new_x, new_y); }

        // Set widget position /
        // Установить позицию виджета.
        void WidgetSetPos(widget* w, int32_t new_x, int32_t new_y)
        {
            if (w)
            {
                w->send_message(widget::WIDGET_SET_X, new_x);
                w->send_message(widget::WIDGET_SET_Y, new_y);
            }
        }

        // Make widget visible /
        // Сделать виджет видимым.
        void WidgetSetVisible(int32_t id, bool arg)
        { WidgetSetVisible(GetWidget(id), arg); }

        // Make widget visible /
        // Сделать виджет видимым.
        void WidgetSetVisible(widget* w, bool arg)
        {
            if (w)
                w->set_visible(arg);
        }

        // Set widget height /
        // Установить высоту виджета.
        void WidgetSetHeight(int32_t id, int32_t new_height)
        { WidgetSetHeight(GetWidget(id), new_height); }

        // Set widget height /
        // Установить высоту виджета.
        void WidgetSetHeight(widget* w, int32_t new_height)
        {
            if (w)
                w->send_message(widget::WIDGET_SET_HEIGHT, new_height);
        }

        // Set widget width /
        // Установить ширину виджета.
        void WidgetSetWidth(int32_t id, int32_t new_width)
        { WidgetSetHeight(GetWidget(id), new_width); }

        // Set widget width /
        // Установить ширину виджета.
        void WidgetSetWidth(widget* w, int32_t new_width)
        {
            if (w)
                w->send_message(widget::WIDGET_SET_HEIGHT, new_width);
        }

        // Enable all widgets belonging to the current window /
        // Перерисовать все виджеты, принадлежащие текущему окну.
        void EnableAllWidgets(bool enable)
        {
            for ( TWidgetVector::iterator it = Widgets.begin();
                it != Widgets.end();
                ++it )
            {
                (*it)->enable(enable);
            }
        }

        void SleepAllWidgets(bool put_to_sleep)
        { THISCALL_2(void, 0x5FFB70, this, put_to_sleep); }

        // Redraw(refresh) the whole window /
        // Перерисовать(обновить) окно полностью.
        void Refresh()
        { DrawWindow(true, MIN_WIDGET_ID, MAX_WIDGET_ID); }

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t priority;

        // offset: +0x8 = +8,  size = 0x4 = 4
        heroWindow* nextWindow;

        // offset: +0xC = +12,  size = 0x4 = 4
        heroWindow* prevWindow;

        // offset: +0x10 = +16,  size = 0x4 = 4
        TAttribute type;

        // offset: +0x14 = +20,  size = 0x4 = 4
        TStatus status;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t x;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t y;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t width;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t height;

        // offset: +0x28 = +40,  size = 0x4 = 4
        widget* headWidget;

        // offset: +0x2C = +44,  size = 0x4 = 4
        widget* tailWidget;

        // offset: +0x30 = +48,  size = 0x10 = 16
        TWidgetVector Widgets;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t focusId;

        // offset: +0x44 = +68,  size = 0x4 = 4
        Bitmap16Bit* background;

        // offset: +0x48 = +72,  size = 0x4 = 4
        int32_t sleepCount;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_HEROWINDOW
#define NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(static_cast<heroWindow*>(this), flag); }\
virtual int32_t __thiscall Open(int32_t newPriority, bool update) override\
{ return get_type_vftable(this)->Open(static_cast<heroWindow*>(this), newPriority, update); }\
virtual void __thiscall Close(bool update) override\
{ get_type_vftable(this)->Close(static_cast<heroWindow*>(this), update); }\
virtual int32_t __thiscall handle_message(message& msg) override\
{ return get_type_vftable(this)->handle_message(static_cast<heroWindow*>(this), &msg); }\
virtual void __thiscall handle_widget_hover(widget* current_widget) override\
{ get_type_vftable(this)->handle_widget_hover(static_cast<heroWindow*>(this), current_widget); }\
virtual void __thiscall DrawWindow(bool update, int32_t iLowID, int32_t iHighID) override\
{ get_type_vftable(this)->DrawWindow(static_cast<heroWindow*>(this), update, iLowID, iHighID); }\
virtual void __thiscall DoModal(bool fadeIn) override\
{ get_type_vftable(this)->DoModal(static_cast<heroWindow*>(this), fadeIn); }\
virtual void __thiscall AddWidgetsToMessageStream() override\
{ get_type_vftable(this)->AddWidgetsToMessageStream(static_cast<heroWindow*>(this)); }\
virtual void __thiscall sleep(bool arg) override\
{ get_type_vftable(this)->sleep(static_cast<heroWindow*>(this), arg); }
#endif

class textWidget;
#pragma pack(push, 4)
// size = 0x50 = 80, align = 4, baseclass: heroWindow
class CHeroWindowEx : public heroWindow
{
    public:
        struct vftable_t : public heroWindow::vftable_t
        {
            int32_t (__thiscall* WindowHandler)(CHeroWindowEx*, message*);
            bool (__thiscall* ProcessHover)(CHeroWindowEx*, int32_t, int32_t);
            bool (__thiscall* ProcessRightSelect)(CHeroWindowEx*, int32_t);
            int32_t (__thiscall* OnWidgetDeselect)(CHeroWindowEx*, int32_t, bool*);
            textWidget* (__thiscall* GetRolloverWidget)(CHeroWindowEx*);
        };

    protected:
        CHeroWindowEx() NH3API_DELETED_FUNCTION

    public:
        CHeroWindowEx(int32_t winX,
                      int32_t winY,
                      int32_t winWidth,
                      int32_t winHeight,
                      heroWindow::TAttribute winType) NH3API_NOEXCEPT
            : heroWindow(nh3api::dummy_tag)
        { THISCALL_6(void, 0x5FFC00, this, winX, winY, winWidth, winHeight, winType); }

        CHeroWindowEx(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : heroWindow(tag)
        {
            NH3API_IGNORE(m_lastIMHoverID);
        }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(CHeroWindowEx)

        // vftable shift: +36
        virtual int32_t __thiscall WindowHandler(message& msg)
        { return get_type_vftable(this)->WindowHandler(this, &msg); }

        // vftable shift: +40
        virtual bool __thiscall ProcessHover(int32_t mouseX, int32_t mouseY)
        { return get_type_vftable(this)->ProcessHover(this, mouseX, mouseY); }

        // vftable shift: +44
        virtual bool __thiscall ProcessRightSelect(int32_t id)
        { return get_type_vftable(this)->ProcessRightSelect(this, id); }

        // vftable shift: +48
        virtual int32_t __thiscall OnWidgetDeselect(int32_t id, bool& bExitFlag)
        { return get_type_vftable(this)->OnWidgetDeselect(this, id, &bExitFlag); }

        // vftable shift: +52
        virtual textWidget* __thiscall GetRolloverWidget()
        { return get_type_vftable(this)->GetRolloverWidget(this); }

    public:
        void SetHelpText(THelpText* pHelpText, int32_t start, int32_t stop, bool copyText)
        { THISCALL_5(void, 0x5FFEA0, this, pHelpText, start, stop, copyText); }

    public:
        // offset: +0x4C = +76,  size = 0x4 = 4
        int32_t m_lastIMHoverID;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_CHEROWINDOWEX
#define NH3API_VIRTUAL_OVERRIDE_CHEROWINDOWEX(CLASS_NAME)\
NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(CLASS_NAME)\
virtual int32_t __thiscall WindowHandler(message& msg) override\
{ return get_type_vftable(this)->WindowHandler(this, &msg); }\
virtual bool __thiscall ProcessHover(int32_t mouseX, int32_t mouseY) override\
{ return get_type_vftable(this)->ProcessHover(this, mouseX, mouseY); }\
virtual bool __thiscall ProcessRightSelect(int32_t id) override\
{ return get_type_vftable(this)->ProcessRightSelect(this, id); }\
virtual int32_t __thiscall OnWidgetDeselect(int32_t id, bool& bExitFlag) override\
{ return get_type_vftable(this)->OnWidgetDeselect(this, id, &bExitFlag); }\
virtual textWidget* __thiscall GetRolloverWidget() override\
{ return get_type_vftable(this)->GetRolloverWidget(this); }
#endif

#pragma pack(push, 4)
//
// size = 0x60 = 96, align = 4, baseclass: CHeroWindowEx
class CAdvPopup : public CHeroWindowEx
{
    public:
        struct vftable_t : CHeroWindowEx::vftable_t
        {
            int32_t (__thiscall* ExitDialog)(CAdvPopup*, message*);
        };

    protected:
        CAdvPopup() NH3API_DELETED_FUNCTION

    public:
        CAdvPopup(int32_t winX,
                  int32_t winY,
                  int32_t winWidth,
                  int32_t winHeight,
                  heroWindow::TAttribute winType) NH3API_NOEXCEPT
            : CHeroWindowEx(nh3api::dummy_tag)
        { THISCALL_6(void, 0x41AFA0, this, winX, winY, winWidth, winHeight, winType); }

        CAdvPopup(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : CHeroWindowEx(tag)
        {
            NH3API_IGNORE(exitId, exitCodeSubtype, exitCommand, netHandlerInPopup);
        }

    public:
        NH3API_VIRTUAL_OVERRIDE_CHEROWINDOWEX(CAdvPopup)

        // vftable shift: +56
        virtual int32_t __thiscall ExitDialog(message& msg)
        { return get_type_vftable(this)->ExitDialog(this, &msg); }

    public:
        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t exitId;

        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t exitCodeSubtype;

        // offset: +0x58 = +88,  size = 0x4 = 4
        int32_t exitCommand;

        // offset: +0x5C = +92,  size = 0x1 = 1
        bool netHandlerInPopup;

};
#pragma pack(pop)
#ifndef NH3API_VIRTUAL_OVERRIDE_CADVPOPUP
#define NH3API_VIRTUAL_OVERRIDE_CADVPOPUP(CLASS_NAME)\
NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(CLASS_NAME)\
NH3API_VIRTUAL_OVERRIDE_CHEROWINDOWEX(CLASS_NAME)\
virtual int32_t __thiscall ExitDialog(message& msg) override\
{ return get_type_vftable(this)->ExitDialog(this, &msg); }
#endif

#pragma pack(push, 4)
//
// size = 0x54 = 84, align = 4, baseclass: heroWindow
class TDialogBox : public heroWindow
{
    public:
        struct vftable_t : heroWindow::vftable_t
        {
            // CTextDialog also has virtual function called Setup, rename following the
            // convention: class_method
            bool (__thiscall* TDialogBox_Setup)(TDialogBox*, int32_t, int32_t, int32_t, int32_t);
        };

    protected:
        TDialogBox() NH3API_DELETED_FUNCTION

    public:
        TDialogBox(heroWindow::TAttribute winType)
            : heroWindow(nh3api::dummy_tag)
        { THISCALL_2(void, 0x48F9E0, this, winType); }

        TDialogBox(int32_t winX, int32_t winY, int32_t winWidth, int32_t winHeight, heroWindow::TAttribute winType)
            : heroWindow(nh3api::dummy_tag)
        { THISCALL_6(void, 0x48F940, this, winX, winY, winWidth, winHeight, winType); }

        TDialogBox(const nh3api::dummy_tag_t& tag)
            : heroWindow(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(TDialogBox)

        // vftable shift: +36
        virtual bool __thiscall Setup(int32_t winX, int32_t winY, int32_t winWidth, int32_t winHeight)
        { return get_type_vftable(this)->TDialogBox_Setup(this, winX, winY, winWidth, winHeight); }

    public:
        // offset: +0x4C = +76,  size = 0x4 = 4
        int32_t beginID;

        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t endID;
};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_TDIALOGBOX
#define NH3API_VIRTUAL_OVERRIDE_TDIALOGBOX(CLASS_NAME)\
NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(CLASS_NAME)\
virtual bool __thiscall Setup(int32_t winX, int32_t winY, int32_t winWidth, int32_t winHeight) override\
{ return get_type_vftable(this)->TDialogBox_Setup(this, winX, winY, winWidth, winHeight); }
#endif

#pragma pack(push, 4)
// size = 0x58 = 88, align = 4, baseclass: TDialogBox
class CTextDialog : public TDialogBox
{
    public:
        struct vftable_t : TDialogBox::vftable_t
        {
            bool (__thiscall *CTextDialog_Setup)(CTextDialog*, const char*, font*);
              void (__thiscall *UpdateText)(CTextDialog*, const char*);
              void (__thiscall *CalcDimensions)(CTextDialog*, const char*, font*, int32_t*, int32_t*, int32_t*, int32_t*);
        };

    protected:
        CTextDialog() NH3API_DELETED_FUNCTION

    public:
        CTextDialog(heroWindow::TAttribute winType)
            : TDialogBox(nh3api::dummy_tag)
        { THISCALL_2(void, 0x490360, this, winType); }

        CTextDialog(const nh3api::dummy_tag_t& tag)
            : TDialogBox(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TDIALOGBOX(CTextDialog)

        // vftable shift: +40
        virtual bool __thiscall Setup(const char* cText, font* pFont)
        { return get_type_vftable(this)->CTextDialog_Setup(this, cText, pFont); }

        // vftable shift: +44
        virtual void __thiscall UpdateText(const char* cNewText)
        { get_type_vftable(this)->UpdateText(this, cNewText); }

        // vftable shift: +48
        virtual void __thiscall CalcDimensions(const char* cText, font* pFont, int32_t& winX, int32_t& winY, int32_t& winWidth, int32_t& winHeight)
        { get_type_vftable(this)->CalcDimensions(this, cText, pFont, &winX, &winY, &winWidth, &winHeight); }

    public:
        int32_t ExitDialog(message& msg)
        { return THISCALL_2(int32_t, 0x4906A0, this, &msg); }

    public:
        // offset: +0x54 = +84,  size = 0x4 = 4
        textWidget* pTextWidget;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Modeless window /
// i.e. such window that is not a dialog,
// that is,
/////////////////////
// НЕ модальное окно:
// т.е. такое окно, не является диалогом, иными словами
// оно не блокирует другие окна. Примером может служить окно колонки ресурсов.
// Такое окно не может существовать само по себе, оно зависит от родительского окна.
// size = 0x34 = 52, align = 4
class TSubWindow
{
    public:
        struct vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(TSubWindow*, uint8_t);
        };

    public:
        TSubWindow() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x5AA650, this); }

        TSubWindow(int32_t x, int32_t y, int32_t w, int32_t h, heroWindow* parent_window) NH3API_NOEXCEPT
        { THISCALL_6(void, 0x5AA6D0, this, x, y, w, h, parent_window); }

        TSubWindow(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Widgets(tag)
        { NH3API_IGNORE(X, Y, Width, Height, Widgets, ParentWindow, FirstWidgetID, LastWidgetID, Background); }

        ~TSubWindow() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x5AA720, this); }

    public:
        void initialize(int32_t x, int32_t y, int32_t w, int32_t h, heroWindow* parent_window)
        { THISCALL_6(void, 0x5AA780, this, x, y, w, h, parent_window); }

        void AddWidget(widget* newWidget, int32_t newPriority)
        { THISCALL_3(void, 0x5AA7B0, this, newWidget, newPriority); }

        void Draw(bool update, int32_t iLowID, int32_t iHighID)
        { THISCALL_4(void, 0x5AA800, this, update, iLowID, iHighID); }

        void SaveBackground()
        { THISCALL_1(void, 0x5AA870, this); }

        void RestoreBackground()
        { THISCALL_1(void, 0x5AA920, this); }

        void Refresh()
        { Draw(true, heroWindow::MIN_WIDGET_ID, heroWindow::MAX_WIDGET_ID); }

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t X;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t Y;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t Width;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t Height;

        // offset: +0x14 = +20,  size = 0x10 = 16
        TWidgetVector Widgets;

        // offset: +0x24 = +36,  size = 0x4 = 4
        heroWindow* ParentWindow;

        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t FirstWidgetID;

        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t LastWidgetID;

        // offset: +0x30 = +48,  size = 0x4 = 4
        Bitmap16Bit* Background;

};
#pragma pack(pop)

#ifndef NH3API_VIRTUAL_OVERRIDE_TSUBWINDOW
#define NH3API_VIRTUAL_OVERRIDE_TSUBWINDOW(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }
#endif

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct type_dialog_resource
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    EGameResource resource;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t qualifier;
};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x4C = 76, align = 4
struct type_dialog_icon
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    EGameResource resource;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t qualifier;

    // offset: +0x8 = +8,  size = 0x10 = 16
    exe_string spriteName;

    // offset: +0x18 = +24,  size = 0x10 = 16
    exe_string text;

    // offset: +0x28 = +40,  size = 0x4 = 4
    int32_t spriteFrameIndex;

    // offset: +0x2C = +44,  size = 0x8 = 8
    POINT spritePos;

    // offset: +0x34 = +52,  size = 0x4 = 4
    int32_t spriteHeight;

    // offset: +0x38 = +56,  size = 0x4 = 4
    int32_t spriteWidth;

    // offset: +0x3C = +60,  size = 0x8 = 8
    POINT textPos;

    // offset: +0x44 = +68,  size = 0x4 = 4
    int32_t textHeight;

    // offset: +0x48 = +72,  size = 0x4 = 4
    int32_t textWidth;

};
#pragma pack(pop)

enum EMBType : int32_t;
#pragma pack(push, 4)
// size = 0x2A0 = 672, align = 4
struct TNormalDialogInfo
{
    // offset: +0x0 = +0,  size = 0x10 = 16
    exe_string dialog_text;

    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t x;

    // offset: +0x14 = +20,  size = 0x4 = 4
    int32_t y;

    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t width;

    // offset: +0x1C = +28,  size = 0x4 = 4
    int32_t height;

    // offset: +0x20 = +32,  size = 0x4 = 4
    int32_t text_widget_x;

    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t text_widget_y;

    // offset: +0x28 = +40,  size = 0x4 = 4
    int32_t text_widget_width;

    // offset: +0x2C = +44,  size = 0x4 = 4
    int32_t text_widget_height;

    // offset: +0x30 = +48,  size = 0x1 = 1
    bool text_expansion;

    // offset: +0x34 = +52,  size = 0x260 = 608
    std::array<type_dialog_icon, 8> icons;

    // offset: +0x294 = +660,  size = 0x4 = 4
    EMBType iMBType;

    // offset: +0x298 = +664,  size = 0x4 = 4
    int32_t iSpecial;

    // offset: +0x29C = +668,  size = 0x4 = 4
    int32_t timeout;

};
#pragma pack(pop)

// NormalDialog dialog interaction type /
// Вид диалога NormalDialog.
enum EMBType : int32_t
{
    // Accept button /
    // Кнопка принятия.
    NORMAL_DIALOG_DEFAULT = 1,
    // Accept/decline dialog /
    // Диалог, в котором нужно выбрать либо утвердительный ответ, либо отказ
    NORMAL_DIALOG_YESNO = 2,
    NORMAL_DIALOG_POPUP = 4,
    NORMAL_DIALOG_CHOOSE = 7,
    NORMAL_DIALOG_CHOOSE_OPTIONAL = 10,
};

NH3API_FORCEINLINE
// address: 0x4F6C00
// Do default dialog /
// Стандартный диалог.
void NormalDialog(const char* cText,
                  EMBType iMBType = NORMAL_DIALOG_DEFAULT,
                  int32_t x = -1,
                  int32_t y = -1,
                  EGameResource iResType1 = const_no_resource,
                  int32_t iResExtra1 = 0,
                  EGameResource iResType2 = const_no_resource,
                  int32_t iResExtra2 = 0,
                  int32_t iSpecial = -1,
                  int32_t iTimeout = 0,
                  EGameResource iResType3 = const_no_resource,
                  int32_t iResExtra3 = 0)
{ FASTCALL_12(void, 0x4F6C00, cText, iMBType, x, y, iResType1, iResExtra1, iResType2, iResExtra2, iSpecial, iTimeout, iResType3, iResExtra3); }

NH3API_FORCEINLINE
// address: 0x4F7D20
// Do extended dialog /
// Расширенный диалог.
void extended_dialog(const char* text,
                     exe_vector<type_dialog_resource>& resources,
                     int32_t x = -1,
                     int32_t y = -1,
                     int32_t timeout = 0)
{ FASTCALL_5(void, 0x4F7D20, text, &resources, x, y, timeout); }

NH3API_DISABLE_WARNING_END