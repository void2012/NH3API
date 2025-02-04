//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "creatures.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<TCreatureTypeTraits, MAX_COMBAT_CREATURES>& akCreatureTypeTraits =
get_global_var_ref(0x6703B8, std::array<TCreatureTypeTraits, MAX_COMBAT_CREATURES>);

std::array<std::array<std::array<TCreatureType, 7>, 2>, kNumTowns>& gDwellingType =
get_global_var_ref(0x6747B4, std::array<std::array<std::array<TCreatureType, 7>, 2>, kNumTowns>);
#endif