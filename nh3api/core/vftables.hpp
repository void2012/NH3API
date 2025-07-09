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

#include "adventure.hpp"
#include "base_manager.hpp"
#include "combat.hpp"
#include "mouse.hpp"
#include "multiplayer.hpp"
#include "quests.hpp"

#ifndef NH3API_GENERATE_VFTABLE_INSTANCE
#define NH3API_GENERATE_VFTABLE_INSTANCE(TYPE, ADDRESS) \
template<> struct vftable_address<TYPE> \
{ static const uintptr_t address = ADDRESS; };
#endif

// managers
NH3API_GENERATE_VFTABLE_INSTANCE(advManager, 0x63A678)
NH3API_GENERATE_VFTABLE_INSTANCE(baseManager, 0x63B9BC)
NH3API_GENERATE_VFTABLE_INSTANCE(combatManager, 0x63D3E8)
NH3API_GENERATE_VFTABLE_INSTANCE(inputManager, 0x63FE10)
NH3API_GENERATE_VFTABLE_INSTANCE(mouseManager, 0x640038)
// managers

// multiplayer.hpp //
NH3API_GENERATE_VFTABLE_INSTANCE(t_complex_net_message, 0x640280)
NH3API_GENERATE_VFTABLE_INSTANCE(CNetMsgHandler, 0x640F24)
// multiplayer.hpp //

// quests.hpp //
NH3API_GENERATE_VFTABLE_INSTANCE(type_quest,                  0x64175C)
NH3API_GENERATE_VFTABLE_INSTANCE(type_experience_quest,       0x641798)
NH3API_GENERATE_VFTABLE_INSTANCE(type_skill_quest,            0x6417D4)
NH3API_GENERATE_VFTABLE_INSTANCE(type_defeat_hero_quest,      0x641810)
NH3API_GENERATE_VFTABLE_INSTANCE(type_monster_quest,          0x64184C)
NH3API_GENERATE_VFTABLE_INSTANCE(type_artifact_quest,         0x641888)
NH3API_GENERATE_VFTABLE_INSTANCE(type_creature_quest,         0x6418C4)
NH3API_GENERATE_VFTABLE_INSTANCE(type_resource_quest,         0x641900)
NH3API_GENERATE_VFTABLE_INSTANCE(type_be_hero_quest,          0x64193C)
NH3API_GENERATE_VFTABLE_INSTANCE(type_belong_to_player_quest, 0x641978)

#undef NH3API_GENERATE_VFTABLE_INSTANCE
