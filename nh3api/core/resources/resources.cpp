//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "resources.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
TPalette24*& gPlayerPalette24 = get_global_var_ref(0x6AAD14, TPalette24*);
TPalette16*& gPlayerPalette = get_global_var_ref(0x6AAD10, TPalette16*);
TPalette16*& gGamePalette   = get_global_var_ref(0x6AAD18, TPalette16*);
font*& tinyFont = get_global_var_ref(0x698A54, font*);
font*& smallFont = get_global_var_ref(0x698A58, font*);
font*& medFont = get_global_var_ref(0x698A5C, font*);
font*& bigFont = get_global_var_ref(0x698A60, font*);
font*& caligFont = get_global_var_ref(0x698A64, font*);
std::array<char, 512>& gText = get_global_var_ref(0x697428, std::array<char, 512>);
const TTextResource* const& GameText = get_global_var_ref(0x6A5DC4, const TTextResource* const);
const TTextResource* const& ArrayText = get_global_var_ref(0x6A60AC, const TTextResource* const);
const TTextResource* const& AdventureEventText = get_global_var_ref(0x696A68, const TTextResource* const);
uint32_t& ResourceManager::RedBits = get_global_var_ref(0x69E5DC, uint32_t);
uint32_t& ResourceManager::RedShift = get_global_var_ref(0x69E5E4, uint32_t);
uint32_t& ResourceManager::RedMask = get_global_var_ref(0x69E5E8, uint32_t);
uint32_t& ResourceManager::GreenBits = get_global_var_ref(0x69E5D0, uint32_t);
uint32_t& ResourceManager::GreenShift = get_global_var_ref(0x69E5E0, uint32_t);
uint32_t& ResourceManager::GreenMask = get_global_var_ref(0x69E5EC, uint32_t);
uint32_t& ResourceManager::BlueBits = get_global_var_ref(0x69E5F0, uint32_t);
uint32_t& ResourceManager::BlueShift = get_global_var_ref(0x69E5D8, uint32_t);
uint32_t& ResourceManager::BlueMask = get_global_var_ref(0x69E5D4, uint32_t);
#endif
