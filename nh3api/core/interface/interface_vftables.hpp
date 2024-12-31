//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once 

#include "dialogs.hpp"
#include "recruit_unit.hpp"
#include "widgets.hpp"
#include "window_manager.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(Type, Address)\
template<>\
inline const typename Type::vftable_t*\
get_type_vftable<Type>()\
{ return get_global_var_ptr(Address, const typename Type::vftable_t); }
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(widget, 0x643CA0)
NH3API_GENERATE_VFTABLE_INSTANCE(heroWindow, 0x643CD4)
NH3API_GENERATE_VFTABLE_INSTANCE(CHeroWindowEx, 0x643CF8)
NH3API_GENERATE_VFTABLE_INSTANCE(CAdvPopup, 0x63A6A8)
NH3API_GENERATE_VFTABLE_INSTANCE(TDialogBox, 0x63DB40)
NH3API_GENERATE_VFTABLE_INSTANCE(CTextDialog, 0x63DB68)
NH3API_GENERATE_VFTABLE_INSTANCE(TSubWindow, 0x64235C)
NH3API_GENERATE_VFTABLE_INSTANCE(heroWindowManager, 0x643D5C)
NH3API_GENERATE_VFTABLE_INSTANCE(recruitUnit, 0x640C80)
NH3API_GENERATE_VFTABLE_INSTANCE(TResourceDisplay, 0x641034)
NH3API_GENERATE_VFTABLE_INSTANCE(type_bottom_view_window, 0x63BB04)
NH3API_GENERATE_VFTABLE_INSTANCE(TAdventureMapWindow, 0x63A5E4)
NH3API_GENERATE_VFTABLE_INSTANCE(iconWidget, 0x63EC48)
NH3API_GENERATE_VFTABLE_INSTANCE(textWidget, 0x642DC0)
NH3API_GENERATE_VFTABLE_INSTANCE(textEntryWidget, 0x642D50)
NH3API_GENERATE_VFTABLE_INSTANCE(border, 0x63BA24)
NH3API_GENERATE_VFTABLE_INSTANCE(bitmapBorder, 0x63BA94)
NH3API_GENERATE_VFTABLE_INSTANCE(coloredBorderFrame, 0x63BA5C)
NH3API_GENERATE_VFTABLE_INSTANCE(bitmapBackedTextWidget, 0x642DF8)
NH3API_GENERATE_VFTABLE_INSTANCE(button, 0x63BB54)
NH3API_GENERATE_VFTABLE_INSTANCE(textButton, 0x63BB88)
NH3API_GENERATE_VFTABLE_INSTANCE(type_func_button, 0x63BBBC)
NH3API_GENERATE_VFTABLE_INSTANCE(slider, 0x641D60)
NH3API_GENERATE_VFTABLE_INSTANCE(type_text_scroller, 0x642D1C)
NH3API_GENERATE_VFTABLE_INSTANCE(type_text_slider, 0x642CD8)
NH3API_GENERATE_VFTABLE_INSTANCE(CChatEdit, 0x640E40)