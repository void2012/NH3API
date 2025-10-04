//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "adventure_AI.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS

float& type_AI_player::attack_computer_bonus
= get_global_var_ref(0x6604F8, float);

float& type_AI_player::attack_human_bonus
= get_global_var_ref(0x6604FC, float);

std::array<type_AI_player, 8>& AI_player
= get_global_var_ref(0x6929A0, std::array<type_AI_player, 8>);

#endif 
