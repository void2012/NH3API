//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once 

#include "player_enums.hpp" // EPlayerColor
#include "hero_enums.hpp" // THero
#include "terrain.hpp" // TTownType

// Game difficulty /
// Игровая сложность.
enum EGameDifficulty : int8_t 
{
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_HARD = 2,
    DIFFICULTY_EXPERT = 3,
    DIFFICULTY_IMPOSSIBLE = 4,

    DIFFICULTY_NONE = -1,
    DIFFICULTY_MAX = 5,
};

#pragma pack(push, 1)
// Game setup options /
// Начальные условия игры.
// size = 0x1CC = 460, align = 1
struct SGameSetupOptions
{
    // offset: +0x0 = +0,  size = 0x8 = 8
    std::array<int8_t, MAX_PLAYERS> color;

    // Each player handicap /
    // Штраф каждого игрока.
    // offset: +0x8 = +8,  size = 0x8 = 8
    std::array<int8_t, MAX_PLAYERS> handicap;

    // Players fractions(alignments) /
    // Фракции игроков.
    // offset: +0x10 = +16,  size = 0x20 = 32
    std::array<TTownType, MAX_PLAYERS> alignment;

    // offset: +0x30 = +48,  size = 0x8 = 8
    std::array<int8_t, MAX_PLAYERS> playerPos;

    // Game difficulty /
    // Игровая сложность.
    // offset: +0x38 = +56,  size = 0x1 = 1
    EGameDifficulty difficulty;

    // Map file name /
    // Название файла карты.
    // offset: +0x39 = +57,  size = 0xFB = 251
    std::array<char, 251> cFilename;

    // Map file path /
    // Полный путь к файлу карты.
    // offset: +0x134 = +308,  size = 0x64 = 100
    std::array<char, 100> cPath;

    // offset: +0x198 = +408,  size = 0x8 = 8
    std::array<bool, MAX_PLAYERS> canFlipFromToComputer;

protected:
    // offset: +0x1A0 = +416,  size = 0x1 = 1
    int8_t curSelectedPlayer;

public:
    // offset: +0x1A1 = +417,  size = 0x1 = 1
    bool bThisFileInitialized;

    // offset: +0x1A2 = +418,  size = 0x1 = 1
    int8_t initializationNumHumans;

    // Turn duration /
    // Продолжительность хода.
    // offset: +0x1A3 = +419,  size = 0x1 = 1
    int8_t turnDuration;

    // Starting heroes /
    // Стартовые герои.
    // offset: +0x1A4 = +420,  size = 0x20 = 32
    std::array<THeroID, MAX_PLAYERS> startingHero;

    // Starting bonuses /
    // Стартовые бонусы.
    // offset: +0x1C4 = +452,  size = 0x8 = 8
    std::array<int8_t, MAX_PLAYERS> startingBonus;

};
#pragma pack(pop)