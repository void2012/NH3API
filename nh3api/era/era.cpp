//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "era.hpp"

namespace Era 
{

volatile int32_t*  const v = get_global_var_ptr(0x887664, volatile int32_t); 
volatile TErmZVar* const z = get_global_var_ptr(0x9271E8, volatile TErmZVar); 
volatile int32_t*  const y = get_global_var_ptr(0xA48D7C, volatile int32_t); 
volatile int32_t*  const x = get_global_var_ptr(0x91DA34, volatile int32_t); 
volatile bool*     const f = get_global_var_ptr(0x91F2DF, volatile bool); 
volatile float*    const e = get_global_var_ptr(0xA48F14, volatile float); 

} // namespace Era
