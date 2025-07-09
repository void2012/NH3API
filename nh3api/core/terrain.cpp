//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "terrain.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<ExtraObjectProperties, MAX_OBJECTS>& gExtraObjectProperties
= get_global_var_ref(0x6916E8, std::array<ExtraObjectProperties, MAX_OBJECTS>);

const std::array<tilePoint, 8>& normalDirTable
= get_global_var_ref(0x678150, const std::array<tilePoint, 8>);
#endif
