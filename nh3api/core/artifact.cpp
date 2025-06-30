//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "artifact.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<TArtifactTraits, MAX_ARTIFACTS>& akArtifactTraits
= get_global_var_ref(0x693A58, std::array<TArtifactTraits, MAX_ARTIFACTS>);

std::array<TArtifactSlotTraits, MAX_ARTIFACT_SLOTS>& akArtifactSlotTraits
= get_global_var_ref(0x694C58, std::array<TArtifactSlotTraits, MAX_ARTIFACT_SLOTS>);

std::array<CombinationArtifact, MAX_COMBO_ARTIFACTS>& gCombinationArtifacts
= get_global_var_ref(0x693938, std::array<CombinationArtifact, MAX_COMBO_ARTIFACTS>);
#endif