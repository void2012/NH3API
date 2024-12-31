//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "resources.hpp"
#include "sound.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(Type, Address) \
template<>\
inline const typename Type::vftable_t*\
get_type_vftable<Type>()\
{ return get_global_var_ptr(Address, const typename Type::vftable_t); }
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(resource, 0x64100C)
NH3API_GENERATE_VFTABLE_INSTANCE(Bitmap16Bit, 0x63B9C8)
NH3API_GENERATE_VFTABLE_INSTANCE(Bitmap24Bit, 0x63B9F4)
NH3API_GENERATE_VFTABLE_INSTANCE(Bitmap816, 0x63BA14)
NH3API_GENERATE_VFTABLE_INSTANCE(CSpriteFrame, 0x63D6BC)
NH3API_GENERATE_VFTABLE_INSTANCE(CSprite, 0x63D6B0)
NH3API_GENERATE_VFTABLE_INSTANCE(font, 0x63E5F4) 
NH3API_GENERATE_VFTABLE_INSTANCE(TPalette16, 0x640378)
NH3API_GENERATE_VFTABLE_INSTANCE(TPalette24, 0x640384)
NH3API_GENERATE_VFTABLE_INSTANCE(sample, 0x6416E0)
NH3API_GENERATE_VFTABLE_INSTANCE(CTextEntrySave, 0x642D9C)
NH3API_GENERATE_VFTABLE_INSTANCE(TTextResource, 0x642DA8)
NH3API_GENERATE_VFTABLE_INSTANCE(TSpreadsheetResource, 0x642DB4)
NH3API_GENERATE_VFTABLE_INSTANCE(soundManager, 0x63FE68)