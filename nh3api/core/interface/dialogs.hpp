//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "widgets.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

enum TCreatureType : int32_t;
enum SpellID : int32_t;
class armyGroup;
class army;
class town;
class hero;
#pragma pack(push, 4)
// size = 0x78 = 120, align = 4, baseclass: TSubWindow
NH3API_VIRTUAL_CLASS TResourceDisplay : public TSubWindow
{
    public:
        struct vftable_t : public TSubWindow::vftable_t {};

        TResourceDisplay() = delete;
        TResourceDisplay(const TResourceDisplay&)            = delete;
        TResourceDisplay(TResourceDisplay&&)                 = delete;
        TResourceDisplay& operator=(const TResourceDisplay&) = delete;
        TResourceDisplay& operator=(TResourceDisplay&&)      = delete;

    public:
        inline TResourceDisplay(heroWindow* parent, bool is_small) noexcept
            : TSubWindow(nh3api::dummy_tag)
        { THISCALL_3(void, 0x558DF0, this, parent, is_small); }

        inline TResourceDisplay(const nh3api::dummy_tag_t& tag) noexcept
            : TSubWindow(tag)
        {}

        inline ~TResourceDisplay() noexcept
        { THISCALL_1(void, 0x5590D0, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_TSUBWINDOW(TResourceDisplay)

    public:
        void Update(bool draw, bool update)
        { THISCALL_3(void, 0x559170, this, draw, update); }

        void Clear()
        { THISCALL_1(void, 0x559270, this); }

    public:
        // Small version of resource display /
        // Уменьшенная версия
        // offset: +0x34 = +52,  size = 0x1 = 1
        bool IsSmall;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x38 = +56,  size = 0x1C = 28
        std::array<textWidget*, 7> ResourceWidgets;

        // offset: +0x54 = +84,  size = 0x1C = 28
        std::array<border*, 7> ResourceIconWidgets;

        // offset: +0x70 = +112,  size = 0x4 = 4
        bitmapBorder* BackgroundWidget;

        // offset: +0x74 = +116,  size = 0x4 = 4
        textWidget* DayWidget;

} NH3API_MSVC_LAYOUT;

// size = 0x34 = 52, align = 4, baseclass: TSubWindow
NH3API_VIRTUAL_CLASS type_bottom_view_window : public TSubWindow
{
    public:
        struct vftable_t : public TSubWindow::vftable_t
        {
            void (__thiscall* animate)(type_bottom_view_window* );
        };

    public:
        inline type_bottom_view_window(heroWindow* parent_window) noexcept
        { THISCALL_2(void, 0x451EC0, this, parent_window); }

        inline type_bottom_view_window(const nh3api::dummy_tag_t&) noexcept
            : TSubWindow(nh3api::dummy_tag)
        {}

        inline ~type_bottom_view_window() noexcept
        { THISCALL_1(void, 0x450A20, this); }

        type_bottom_view_window() = delete;
        type_bottom_view_window(const type_bottom_view_window&)            = delete;
        type_bottom_view_window(type_bottom_view_window&&)                 = delete;
        type_bottom_view_window& operator=(const type_bottom_view_window&) = delete;
        type_bottom_view_window& operator=(type_bottom_view_window&&)      = delete;

    public:
        NH3API_VIRTUAL_OVERRIDE_TSUBWINDOW(type_bottom_view_window)

        // vftable shift: +4
        virtual void __thiscall animate()
        { get_vftable(this)->animate(this); }

};

// Adventure map window /
// Окно карты приключений.
// size = 0xA0 = 160, align = 4, baseclass: heroWindow
NH3API_VIRTUAL_CLASS TAdventureMapWindow : public heroWindow
{
    public:
        struct vftable_t : heroWindow::vftable_t {};

    public:
        inline TAdventureMapWindow() noexcept
            : heroWindow(nh3api::dummy_tag)
        { THISCALL_1(TAdventureMapWindow*, 0x401510, this); }

        inline TAdventureMapWindow(const nh3api::dummy_tag_t& tag) noexcept
            : heroWindow(tag)
        {}

        inline ~TAdventureMapWindow() noexcept
        { THISCALL_1(void, 0x402B10, this); }

        TAdventureMapWindow(const TAdventureMapWindow&)            = delete;
        TAdventureMapWindow(TAdventureMapWindow&&)                 = delete;
        TAdventureMapWindow& operator=(const TAdventureMapWindow&) = delete;
        TAdventureMapWindow& operator=(TAdventureMapWindow&&)      = delete;

    public:
        void animate_bottom_view(bool in_background)
        { THISCALL_2(void, 0x402B90, this, in_background); }

        void draw_bottom_view(bool update)
        { THISCALL_2(void, 0x402BC0, this, update); }

        void set_bottom_view(type_bottom_view_window* new_view)
        { THISCALL_2(void, 0x402C10, this, new_view); }

        int32_t convertID2HelpID(int32_t id)
        { return THISCALL_2(int32_t, 0x402C50, this, id); }

        bool ProcessRightSelect(const message& msg)
        { return THISCALL_2(bool, 0x402E70, this, &msg); }

        bool ProcessHover(int32_t hx, int32_t hy)
        { return THISCALL_3(bool, 0x403010, this, hx, hy); }

        void DoHeroKnob(bool update)
        { THISCALL_2(void, 0x403220, this, update); }

        void DoTownKnob(bool update)
        { THISCALL_2(void, 0x403280, this, update); }

        void UpdateHeroLocators(int32_t top, bool drawWin, bool updateFlag)
        { THISCALL_4(void, 0x4032E0, this, top, drawWin, updateFlag); }

        void UpdateTownLocators(int32_t top, bool drawWin, bool updateFlag)
        { THISCALL_4(void, 0x403420, this, top, drawWin, updateFlag); }

        void UpdateHeroLocator(int32_t iWhich, bool drawWinSect, bool updateFlag)
        { THISCALL_4(void, 0x403560, this, iWhich, drawWinSect, updateFlag); }

        void UpdateTownLocator(int32_t iWhich, bool drawWinSect, bool updateFlag)
        { THISCALL_4(void, 0x4037A0, this, iWhich, drawWinSect, updateFlag); }

        void HighlightLocators(bool update)
        { THISCALL_2(void, 0x4038C0, this, update); }

        void UpdateQuestLogButton(bool update)
        { THISCALL_2(void, 0x4039B0, this, update); }

        void UpdateSleepButton(const hero* this_hero)
        { THISCALL_2(void, 0x403BA0, this, this_hero); }

        void UpdateSpellButton(const hero* this_hero)
        { THISCALL_2(void, 0x403BF0, this, this_hero); }

        bool SetElevationToggleImage(int32_t level)
        { return THISCALL_2(bool, 0x403C40, this, level); }

        void SetSleepImage(int32_t image)
        { THISCALL_2(void, 0x403CC0, this, image); }

        void ClearBottomView()
        { THISCALL_1(void, 0x403EE0, this); }

        void UpdateResourceDisplay(bool draw, bool update)
        { THISCALL_3(void, 0x403F00, this, draw, update); }

        void DrawChatText(bool update)
        { THISCALL_2(void, 0x403F20, this, update); }

        void UpdateButtons(bool draw, bool update)
        { THISCALL_3(void, 0x403F60, this, draw, update); }

    public:
        NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(TAdventureMapWindow)

    public:
        // offset: +0x4C = +76,  size = 0x4 = 4
        const widget* RadarWidget;

        // offset: +0x50 = +80,  size = 0x4 = 4
        const widget* MapWidget;

        // offset: +0x54 = +84,  size = 0x4 = 4
        textWidget* ChatTextWidget;

        // offset: +0x58 = +88,  size = 0x4 = 4
        CChatEdit* chatEdit;

        // offset: +0x5C = +92,  size = 0x4 = 4
        TResourceDisplay* ResourceDisplay;

        // offset: +0x60 = +96,  size = 0x4 = 4
        int32_t topHero;

        // offset: +0x64 = +100,  size = 0x4 = 4
        int32_t topTown;

        // offset: +0x68 = +104,  size = 0x4 = 4
        bitmapBackedTextWidget* RolloverWidget;

        // offset: +0x6C = +108,  size = 0x1 = 1
        bool animate_in_background;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x70 = +112,  size = 0x14 = 20
        std::array<bitmapBorder*, 5> hero_borders;

        // offset: +0x84 = +132,  size = 0x14 = 20
        std::array<bitmapBorder*, 5> hero_highlight_borders;

        // offset: +0x98 = +152,  size = 0x4 = 4
        type_bottom_view_window* bottom_view;

    protected:
        // offset: +0x9C = +156,  size = 0x4 = 4
        void* immersion;

} NH3API_MSVC_LAYOUT;

// Army information view window /
// Окно просмотра информации о стеке армии.
// size = 0xB8 = 184, align = 4, baseclass: CAdvPopup
NH3API_VIRTUAL_CLASS TViewArmyWindow : public CAdvPopup
{
    public:
        struct vftable_t : CAdvPopup::vftable_t {};
    
    public:
        inline TViewArmyWindow(const army* this_army, int32_t x0, int32_t y0, bool show_ok)
            : CAdvPopup(nh3api::dummy_tag)
        { THISCALL_5(void, 0x5F3700, this, this_army, x0, y0, show_ok); }
    
        inline TViewArmyWindow(armyGroup const& group, int32_t iarmy, const hero* this_hero, const town* this_town, int32_t x0, int32_t y0, int32_t upgrade, bool show_dismiss, bool show_ok)
            : CAdvPopup(nh3api::dummy_tag)
        { THISCALL_10(void, 0x5F3EF0, this, &group, iarmy, this_hero, this_town, x0, y0, upgrade, show_dismiss, show_ok); }
    
        inline TViewArmyWindow(int32_t army_type, int32_t x0, int32_t y0, bool show_ok)
            : CAdvPopup(nh3api::dummy_tag)
        { THISCALL_5(void, 0x5F45B0, this, army_type, x0, y0, show_ok); }
    
        inline ~TViewArmyWindow() noexcept
        { THISCALL_1(void, 0x5F4980, this); }
    
        TViewArmyWindow() = delete;
        TViewArmyWindow(const TViewArmyWindow&)            = delete;
        TViewArmyWindow(TViewArmyWindow&&)                 = delete;
        TViewArmyWindow& operator=(const TViewArmyWindow&) = delete;
        TViewArmyWindow& operator=(TViewArmyWindow&&)      = delete;

    public:
        [[nodiscard]] int32_t convertID2HelpID(int32_t id) const
        { return THISCALL_2(int32_t, 0x5F4A60, this, id); }
    
        void DoModal()
        { THISCALL_1(void, 0x5F4BA0, this); }
    
    public:
        NH3API_VIRTUAL_OVERRIDE_CADVPOPUP(TViewArmyWindow)
    
    public:
        // offset: +0x60 = +96,  size = 0x4 = 4
        TCreatureType ArmyType;
    
        // offset: +0x64 = +100,  size = 0x4 = 4
        int32_t ArmySize;
    
        // offset: +0x68 = +104,  size = 0x4 = 4
        int32_t morale;
    
        // offset: +0x6C = +108,  size = 0x10 = 16
        exe_string morale_help;
    
        // offset: +0x7C = +124,  size = 0x4 = 4
        int32_t luck;
    
        // offset: +0x80 = +128,  size = 0x10 = 16
        exe_string luck_help;
    
        // offset: +0x90 = +144,  size = 0x4 = 4
        int32_t Upgrade;
    
        // offset: +0x94 = +148,  size = 0x1 = 1
        bool ShowingUpgradeButton;
    
        // offset: +0x95 = +149,  size = 0x1 = 1
        bool ShowingDismissButton;
    
        // offset: +0x96 = +150,  size = 0x1 = 1
        bool ShowingOkButton;
    
        // offset: +0x98 = +152,  size = 0xC = 12
        std::array<SpellID, 3> Influence;
    
        // offset: +0xA4 = +164,  size = 0xC = 12
        std::array<int32_t, 3> Duration;
    
        // offset: +0xB0 = +176,  size = 0x4 = 4
        bitmapBackedTextWidget* RolloverWidget;
    
        // offset: +0xB4 = +180,  size = 0x4 = 4
        iconWidget* SpriteWidget;

};
#pragma pack(pop)

NH3API_SPECIALIZE_TYPE_VFTABLE(0x643C24, TViewArmyWindow)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x641034, TResourceDisplay)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BB04, type_bottom_view_window)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63A5E4, TAdventureMapWindow)

NH3API_WARNING(pop)
