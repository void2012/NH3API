//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "combat.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
combatManager*& gpCombatManager = get_global_var_ref(0x699420, combatManager*);
#endif