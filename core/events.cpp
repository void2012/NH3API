//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "events.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
HMENU& gDefaultMenu             = get_global_var_ref(0x698A34, HMENU);
HMENU& gCombatMenu              = get_global_var_ref(0x698A38, HMENU);
HMENU& hmnuRecruitSave          = get_global_var_ref(0x69D644, HMENU);
bool&  gbProcessingCombatAction = get_global_var_ref(0x698A38, bool);
bool&  insideProcessMessage     = get_global_var_ref(0x699608, bool);
#endif