//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#ifndef NH3API_FILES_VFTABLES_HPP
#define NH3API_FILES_VFTABLES_HPP

#include "files.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(TYPE, ADDRESS) \
template<> struct vftable_address<TYPE> \
{ static const uintptr_t address = ADDRESS; };
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(TAbstractFile,        0x63DAC0)
NH3API_GENERATE_VFTABLE_INSTANCE(TGzFile,              0x63E74C)
NH3API_GENERATE_VFTABLE_INSTANCE(TStreamBufFile,       0x63DACC)
NH3API_GENERATE_VFTABLE_INSTANCE(TGzInflateBuf,        0x63E710)
NH3API_GENERATE_VFTABLE_INSTANCE(t_lod_file_adapter,   0x641138)

#endif // NH3API_FILES_VFTABLES_HPP