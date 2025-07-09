//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "era.hpp"

namespace Era 
{

int32_t*  const v = get_global_var_ptr(0x887664, int32_t); 
TErmZVar* const z = get_global_var_ptr(0x9271E8, TErmZVar); 
int32_t*  const y = get_global_var_ptr(0xA48D7C, int32_t); 
int32_t*  const x = get_global_var_ptr(0x91DA34, int32_t); 
bool*     const f = get_global_var_ptr(0x91F2DF, bool); 
float*    const e = get_global_var_ptr(0xA48F14, float); 
HINSTANCE hPlugin = nullptr;
TPlugin   plugin  = nullptr;

} // namespace Era

namespace EraMemory
{
volatile size_t* allocatedMemorySize = nullptr;
} // namespace EraMemory
