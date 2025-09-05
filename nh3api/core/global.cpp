//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#include "global.hpp"

#ifndef NH3API_FLAG_INLINE_HEADERS

game*& gpGame = get_global_var_ref(0x699538, game*);
configStruct& gConfig = get_global_var_ref(0x6987A8, configStruct);
int32_t& giThisGamePos = get_global_var_ref(0x6995A4, int32_t);
bool32_t& gbRemoteOn = get_global_var_ref(0x69959C, bool32_t);
bool32_t& gbThisNetGotAdventureControl = get_global_var_ref(0x6977D8, bool32_t);
bool32_t& gbInNewGameSetup = get_global_var_ref(0x698450, bool32_t);
bool32_t& gbInReplay = get_global_var_ref(0x696A54, bool32_t);
bool32_t& gbGameOver = get_global_var_ref(0x697308, bool32_t);
bool& bDefeatedAllPlayers = get_global_var_ref(0x69956C, bool);
int32_t& giMonthTypeExtra = get_global_var_ref(0x697798, int32_t);
int32_t& giMonthType = get_global_var_ref(0x698884, int32_t); 
int32_t& giWeekTypeExtra = get_global_var_ref(0x69844C, int32_t);
int32_t& giWeekType = get_global_var_ref(0x6977A0, int32_t);
exe_bitset<48>& puzzlePiecesRemoved = get_global_var_ref(0x697738, exe_bitset<48>);
eNetGameType& iMPNetProtocol = get_global_var_ref(0x698A40, eNetGameType);

#endif
