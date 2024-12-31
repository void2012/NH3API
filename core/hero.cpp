//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "hero.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<THeroTraits, MAX_HEROES_SOD>& akHeroTraits 
= get_global_var_ref(0x679DD0, std::array<THeroTraits, MAX_HEROES_SOD>);

std::array<THeroClassTraits, kNumHeroClasses>& akHeroClassTraits 
= get_global_var_ref(0x67D868, std::array<THeroClassTraits, kNumHeroClasses>);

std::array<THeroSpecificAbility, MAX_HEROES_SOD>& akHeroSpecificAbilities 
= get_global_var_ref(0x678420, std::array<THeroSpecificAbility, MAX_HEROES_SOD>);
#endif