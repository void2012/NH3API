//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/resources/resources.hpp"

inline std::array<char, 512>& gText = (get_global_var_ref(0x697428, std::array<char, 512>));
// genrltxt.txt, usage: GameText->GetText(42)
inline const TTextResource* const& GameText = get_global_var_ref(0x6A5DC4, const TTextResource* const);
// arraytxt.txt, usage: ArrayText->GetText(42)
inline const TTextResource* const& ArrayText = get_global_var_ref(0x6A60AC, const TTextResource* const);
// advevent.txt, usage: AdventureEventText->GetText(42)
inline const TTextResource* const& AdventureEventText = get_global_var_ref(0x696A68, const TTextResource* const);

inline std::array<THelpText, 5>&   gMainMenuHelp           = get_global_var_ref(0x6A6C88, std::array<THelpText, 5>);
inline std::array<THelpText, 5>&   gNewGameHelp            = get_global_var_ref(0x6A6C60, std::array<THelpText, 5>);
inline std::array<THelpText, 245>& gSingleSelectionHelp    = get_global_var_ref(0x6A6CE8, std::array<THelpText, 245>);
inline std::array<THelpText, 25>&  gMultiSelectionHelp     = get_global_var_ref(0x6A65D8, std::array<THelpText, 25>);
inline std::array<THelpText, 27>&  gAdventureWindowHelp    = get_global_var_ref(0x6A5748, std::array<THelpText, 27>);
inline std::array<THelpText, 48>&  gSystemOptionsHelp      = get_global_var_ref(0x6A75E8, std::array<THelpText, 48>);
inline std::array<THelpText, 7>&   gAdventureOptionsHelp   = get_global_var_ref(0x6A6598, std::array<THelpText, 7>);
inline std::array<THelpText, 11>&  gCombatWindowHelp       = get_global_var_ref(0x6A69D0, std::array<THelpText, 11>);
inline std::array<THelpText, 39>&  gCombatOptionsHelp      = get_global_var_ref(0x6A5610, std::array<THelpText, 39>);
inline std::array<THelpText, 15>&  gViewArmyHelp           = get_global_var_ref(0x6A74C0, std::array<THelpText, 15>);
inline std::array<THelpText, 11>&  gSpellbookHelp          = get_global_var_ref(0x6A5338, std::array<THelpText, 11>);
inline std::array<THelpText, 62>&  gCampaignBriefHelp      = get_global_var_ref(0x6A5A30, std::array<THelpText, 62>);
inline std::array<THelpText, 24>&  gCampaignWindowHelp     = get_global_var_ref(0x6A5FE8, std::array<THelpText, 24>);
inline std::array<THelpText, 3>&   gRecruitHelp            = get_global_var_ref(0x6A75C0, std::array<THelpText, 3>);
inline std::array<THelpText, 8>&   gMPHelp                 = get_global_var_ref(0x6A77B8, std::array<THelpText, 8>);
inline std::array<THelpText, 20>&  gSacrificeWindowHelp    = get_global_var_ref(0x6A66A0, std::array<THelpText, 20>);
inline std::array<THelpText, 3>&   gTransformerWindowHelp  = get_global_var_ref(0x6A7838, std::array<THelpText, 3>);
inline std::array<THelpText, 6>&   gResourceWindowHelp     = get_global_var_ref(0x6A58D0, std::array<THelpText, 6>);
inline std::array<THelpText, 5>&   gGiveResourceWindowHelp = get_global_var_ref(0x6A5410, std::array<THelpText, 5>);
inline std::array<THelpText, 5>&   gBuyArtifactWindowHelp  = get_global_var_ref(0x6A7E10, std::array<THelpText, 5>);
inline std::array<THelpText, 5>&   gSellArtifactWindowHelp = get_global_var_ref(0x6A6CB8, std::array<THelpText, 5>);
inline std::array<THelpText, 5>&   gSellCreatureWindowHelp = get_global_var_ref(0x6A7F00, std::array<THelpText, 5>);
inline std::array<THelpText, 4>&   gUniversityWindowHelp   = get_global_var_ref(0x6A7E40, std::array<THelpText, 4>);
