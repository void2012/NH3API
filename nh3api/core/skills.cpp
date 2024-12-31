//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "skills.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS
std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>& akSSkillTraits =
get_global_var_ref(0x698D88, std::array<TSSkillTraits, MAX_SECONDARY_SKILLS>);

const std::array<int32_t, kNumMasteries>& luck_bonus = 
(get_global_var_ref(0x63E998, const std::array<int32_t, kNumMasteries>));

const std::array<int32_t, kNumMasteries>& leadership_bonus =
(get_global_var_ref(0x63E9A8, const std::array<int32_t, kNumMasteries>));

const std::array<float, kNumMasteries>& SSNecromancyFactor =
(get_global_var_ref(0x63E9B8, const std::array<float, kNumMasteries>));

const std::array<int32_t, kNumMasteries>& mysticism_bonus =
(get_global_var_ref(0x63E9C8, const std::array<int32_t, kNumMasteries>));

const std::array<int32_t, kNumMasteries>& SSScoutingRange =
(get_global_var_ref(0x63E9D8, const std::array<int32_t, kNumMasteries>));

const std::array<float, kNumMasteries>& SSArcheryMod =
(get_global_var_ref(0x63E9E8, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& offense_factor =
(get_global_var_ref(0x63E9F8, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& defense_factor =
(get_global_var_ref(0x63EA08, const std::array<float, kNumMasteries>));

const std::array<int32_t, kNumMasteries>& estates_bonus =
(get_global_var_ref(0x63EA18, const std::array<int32_t, kNumMasteries>));

const std::array<float, kNumMasteries>& SSEagleEyeChance =
(get_global_var_ref(0x63EA28, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& SSDiplomacyFactor =
(get_global_var_ref(0x63EA38, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& resistance_factor =
(get_global_var_ref(0x63EA48, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& learning_factor =
(get_global_var_ref(0x63EA58, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& logistics_factor =
(get_global_var_ref(0x63EA68, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& sorcery_factor =
(get_global_var_ref(0x63EA78, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& intelligence_factor =
(get_global_var_ref(0x63EA88, const std::array<float, kNumMasteries>));

const std::array<float, kNumMasteries>& firstaid_factor =
(get_global_var_ref(0x63EA98, const std::array<float, kNumMasteries>));
#endif