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

const std::array<int32_t, MAX_SKILL_MASTERIES>& luck_bonus =
(get_global_var_ref(0x63E998, const std::array<int32_t, MAX_SKILL_MASTERIES>));

const std::array<int32_t, MAX_SKILL_MASTERIES>& leadership_bonus =
(get_global_var_ref(0x63E9A8, const std::array<int32_t, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& SSNecromancyFactor =
(get_global_var_ref(0x63E9B8, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<int32_t, MAX_SKILL_MASTERIES>& mysticism_bonus =
(get_global_var_ref(0x63E9C8, const std::array<int32_t, MAX_SKILL_MASTERIES>));

const std::array<int32_t, MAX_SKILL_MASTERIES>& SSScoutingRange =
(get_global_var_ref(0x63E9D8, const std::array<int32_t, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& SSArcheryMod =
(get_global_var_ref(0x63E9E8, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& offense_factor =
(get_global_var_ref(0x63E9F8, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& defense_factor =
(get_global_var_ref(0x63EA08, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<int32_t, MAX_SKILL_MASTERIES>& estates_bonus =
(get_global_var_ref(0x63EA18, const std::array<int32_t, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& SSEagleEyeChance =
(get_global_var_ref(0x63EA28, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& SSDiplomacyFactor =
(get_global_var_ref(0x63EA38, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& resistance_factor =
(get_global_var_ref(0x63EA48, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& learning_factor =
(get_global_var_ref(0x63EA58, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& logistics_factor =
(get_global_var_ref(0x63EA68, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& sorcery_factor =
(get_global_var_ref(0x63EA78, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& intelligence_factor =
(get_global_var_ref(0x63EA88, const std::array<float, MAX_SKILL_MASTERIES>));

const std::array<float, MAX_SKILL_MASTERIES>& firstaid_factor =
(get_global_var_ref(0x63EA98, const std::array<float, MAX_SKILL_MASTERIES>));
#endif
