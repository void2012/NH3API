//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "town.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<uint64_t, kNumTowns>&
gTownEligibleBuildMask = get_global_var_ref(0x697740, std::array<uint64_t, kNumTowns>);

std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>&
gHierarchyMask = get_global_var_ref(0x6977E8, std::array<std::array<uint64_t, MAX_BUILDING_TYPE>, kNumTowns>);
#endif