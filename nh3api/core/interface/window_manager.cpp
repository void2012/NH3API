//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "window_manager.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
heroWindowManager*& gpWindowManager = get_global_var_ref(0x6992D0, heroWindowManager*);
uint32_t& giDialogTimeout           = get_global_var_ref(0x6977D4, uint32_t);
#endif
