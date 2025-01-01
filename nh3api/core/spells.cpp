//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "spells.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<TSpellTraits, MAX_SPELLS>& akSpellTraits
= get_global_var_ref(0x6854A0, std::array<TSpellTraits, MAX_SPELLS>);
#endif