//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "map.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
int32_t& MAP_WIDTH = get_global_var_ref(0x6783C8, int32_t);
int32_t& MAP_HEIGHT = get_global_var_ref(0x6783CC, int32_t);
std::array<type_creature_bank_traits, 11>& const_creature_bank_traits = get_global_var_ref(0x695088, std::array<type_creature_bank_traits, 11>);
uint16_t* const& mapExtra = get_global_var_ref(0x698A48, uint16_t*);
#endif
