//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "events.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
HWND& hwndApp                   = get_global_var_ref(0x699650, HWND);
HMENU& hmnuApp                  = get_global_var_ref(0x699654, HMENU);
int32_t& gGameCommand           = get_global_var_ref(0x697728, int32_t);
HMENU& gDefaultMenu             = get_global_var_ref(0x698A34, HMENU);
HMENU& gCombatMenu              = get_global_var_ref(0x698A38, HMENU);
HMENU& hmnuRecruitSave          = get_global_var_ref(0x69D644, HMENU);
HMENU& hmnuCurrent              = get_global_var_ref(0x69960C, HMENU);
bool&  gbProcessingCombatAction = get_global_var_ref(0x698A3C, bool);
bool&  insideProcessMessage     = get_global_var_ref(0x699608, bool);
bool32_t& gbCheatMenus          = get_global_var_ref(0x698A84, bool32_t);
std::array<char, 60>& gcCommandLine = get_global_var_ref(0x699610, std::array<char, 60>);
std::array<char, 368>& gcRegAppPath = get_global_var_ref(0x698614, std::array<char, 368>);
#endif
