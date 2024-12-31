//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

#pragma once 

#include "exe_streambuf.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(Type, Address)\
template<>\
inline const typename Type::vftable_t*\
get_type_vftable<Type>()\
{ return get_global_var_ptr(Address, const typename Type::vftable_t); }
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(exe_streambuf,    0x6456B8)
NH3API_GENERATE_VFTABLE_INSTANCE(exe_filebuf,      0x645820) 
NH3API_GENERATE_VFTABLE_INSTANCE(exe_strstreambuf, 0x645680)