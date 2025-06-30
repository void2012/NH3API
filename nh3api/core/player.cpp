//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "player.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
int32_t& giCurPlayer         = get_global_var_ref(0x69CCF4, int32_t);
uint8_t& giCurWatchPlayerBit = get_global_var_ref(0x69CD08, uint8_t);
uint8_t& giCurrentPlayerBit  = get_global_var_ref(0x69CD10, uint8_t);
#endif
