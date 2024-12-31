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
font*& gTinyFont = get_global_var_ref(0x698A54, font*);     
font*& gSmallFont = get_global_var_ref(0x698A58, font*);    
font*& gMedFont = get_global_var_ref(0x698A5C, font*);      
font*& gBigFont = get_global_var_ref(0x698A60, font*);     
font*& gCalli10RFont = get_global_var_ref(0x698A64, font*);
const TTextResource* const& GameText = get_global_var_ref(0x6A5DC4, const TTextResource* const);
const TTextResource* const& ArrayText = get_global_var_ref(0x6A60AC, const TTextResource* const);
#endif
