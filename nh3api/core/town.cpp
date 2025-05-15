//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "town.hpp"

#if !NH3API_CHECK_CPP11
const std::array<uint64_t, 64>& bitNumber = get_global_var_ref(0x66CD98, const std::array<uint64_t, 64>);
#endif

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<uint64_t, kNumTowns>&
gTownEligibleBuildMask = get_global_var_ref(0x697740, std::array<uint64_t, kNumTowns>);

std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>&
gHierarchyMask = get_global_var_ref(0x6977E8, std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>);
#endif