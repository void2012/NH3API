//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "terrain.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<ObjectProps, MAX_OBJECTS>& object_properties
= get_global_var_ref(0x6916E8, std::array<ObjectProps, MAX_OBJECTS>);

const std::array<tilePoint, 8>& normalDirTable
= get_global_var_ref(0x678150, const std::array<tilePoint, 8>);
#endif