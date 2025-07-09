//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

#pragma once

#ifndef NH3API_STREAMBUF_VFTABLES_HPP
#define NH3API_STREAMBUF_VFTABLES_HPP

#include "exe_streambuf.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(TYPE, ADDRESS) \
template<> struct vftable_address<TYPE> \
{ static const uintptr_t address = ADDRESS; };
#endif

NH3API_GENERATE_VFTABLE_INSTANCE(exe_streambuf,    0x6456B8)
NH3API_GENERATE_VFTABLE_INSTANCE(exe_filebuf,      0x645820)
NH3API_GENERATE_VFTABLE_INSTANCE(exe_strstreambuf, 0x645680)

#endif // NH3API_STREAMBUF_VFTABLES_HPP
