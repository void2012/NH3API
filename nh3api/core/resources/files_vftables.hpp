//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once 

#include "files.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(Type, Address)\
template<>\
inline const typename Type::vftable_t*\
get_type_vftable<Type>()\
{ return get_global_var_ptr(Address, const typename Type::vftable_t); }
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(TAbstractFile,        0x63DAC0)
NH3API_GENERATE_VFTABLE_INSTANCE(TGzFile,              0x63E74C)
NH3API_GENERATE_VFTABLE_INSTANCE(TStreamBufFile,       0x63DACC)
NH3API_GENERATE_VFTABLE_INSTANCE(TGzInflateBuf,        0x63E710)
NH3API_GENERATE_VFTABLE_INSTANCE(t_lod_file_adapter,   0x641138)