//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "combat.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS

const std::array<const uint8_t, 12>& combatManager::moatCell
= get_global_var_ref(0x63BCE8, const std::array<const uint8_t, 12>);

const std::array<const uint8_t, 12>& combatManager::moat2Cell
= get_global_var_ref(0x63BCF4, const std::array<const uint8_t, 12>);

const std::array<const uint8_t, 12>& combatManager::wallCell
= get_global_var_ref(0x63BD00, const std::array<const uint8_t, 12>);

const std::array<const uint8_t, 12>& combatManager::leftCell
= get_global_var_ref(0x63BD0C, const std::array<const uint8_t, 12>);

const std::array<const uint32_t, MAX_TOWNS>& combatManager::moatDamage
= get_global_var_ref(0x63BD18, const std::array<const uint32_t, MAX_TOWNS>);

const std::array<const std::array<const typename combatManager::SCmbtHero, 2>, MAX_TOWNS>& combatManager::sCmbtHero
= get_global_var_ref(0x63BD40, const std::array<const std::array<const typename combatManager::SCmbtHero, 2>, MAX_TOWNS>);

const std::array<const typename combatManager::TWallTarget, 8>& combatManager::wallTargets
= get_global_var_ref(0x63BE60, const std::array<const typename combatManager::TWallTarget, 8>);

const std::array<const typename combatManager::SElevationOverlay, 34>& combatManager::sElevationOverlay
= get_global_var_ref(0x63BEC0, const std::array<const typename combatManager::SElevationOverlay, 34>);

const std::array<const typename combatManager::TObstacleInfo, 91>& combatManager::ObstacleInfo
= get_global_var_ref(0x63C7C8, const std::array<const typename combatManager::TObstacleInfo, 91>);

const typename combatManager::TObstacleInfo& combatManager::QuicksandInfo
= get_global_var_ref(0x63CEE8, const typename combatManager::TObstacleInfo);

const typename combatManager::TObstacleInfo& combatManager::LandMineInfo 
= get_global_var_ref(0x63CF00, const typename combatManager::TObstacleInfo);

const std::array<const typename combatManager::TObstacleInfo, 5>& combatManager::WallObstacleInfo
= get_global_var_ref(0x63CF18, const std::array<const typename combatManager::TObstacleInfo, 5>);

const std::array<const float, 3>& combatManager::CombatSpeedMod
= get_global_var_ref(0x63CF7C, const std::array<const float, 3>);

const std::array<const typename combatManager::TArcherTraits, MAX_TOWNS>& combatManager::ArcherTraits
= get_global_var_ref(0x63CF88, const std::array<const typename combatManager::TArcherTraits, MAX_TOWNS>);

const std::array<const std::array<const typename combatManager::TWallTraits, 18>, MAX_TOWNS>& combatManager::akWallTraits
= get_global_var_ref(0x66D848, const std::array<const std::array<const typename combatManager::TWallTraits, 18>, MAX_TOWNS>);

combatManager*& gpCombatManager 
= get_global_var_ref(0x699420, combatManager*);

std::array<int32_t, 2>& iCombatControlNetPos 
= get_global_var_ref(0x69778C, std::array<int32_t, 2>);

bool& gbRetreatWin   
= get_global_var_ref(0x6985F3, bool);

bool& gbSurrenderWin 
= get_global_var_ref(0x697794, bool);

bool& gbInCombat 
= get_global_var_ref(0x699590, bool);

#endif
