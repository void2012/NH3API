//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "resources/resources.hpp" // EGameResource
#include "hero_enums.hpp" // THeroID
#include "terrain.hpp" // type_point

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#if !NH3API_CHECK_CPP11
NH3API_DISABLE_MSVC_WARNING_BEGIN(4341)
#endif
// Victory condition type /
// Тип условия победы.
enum EVictoryConditionType : int8_t
{
    VICTORY_NONE = -1, // победить всех врагов
    VICTORY_AQUIRE_ARTIFACT = 0, // заполучить артефакт
    VICTORY_ACCUMULATE_CREATURES = 1, // накопить существ
    VICTORY_ACCUMULATE_RESOURCES = 2, // накопить ресурсы
    VICTORY_UPGRADE_TOWN = 3, // улучшить город
    VICTORY_BUILD_HOLY_GRAIL_STRUCT = 4, // построить в городе Грааль
    VICTORY_DEFEAT_HERO = 5, // сразить героя
    VICTORY_CAPTURE_TOWN = 6, // захватить город
    VICTORY_DEFEAT_MONSTER = 7, // сразить монстра
    VICTORY_FLAG_ALL_CREATURE_GENERATORS = 8, // захватить все внешние жилища
    VICTORY_FLAG_ALL_MINES = 9, // захватить все шахты и маяки
    VICTORY_TRANSPORT_ARTIFACT = 10, // переместить артефакт в город
    VICTORY_DEFEAT_ALL_MONSTERS = 11, // уничтожить всех монстров
    VICTORY_SURVIVE_UNTIL_TIME_EXPIRES = 12, // выжить определенное время

    MAX_VICTORY_CONDITIONS = 13
};
#if !NH3API_CHECK_CPP11
NH3API_DISABLE_MSVC_WARNING_END
#endif

#if !NH3API_CHECK_CPP11
NH3API_DISABLE_MSVC_WARNING_BEGIN(4341)
#endif
// Loss condition type /
// Тип условия поражения.
enum ELossConditionType : int8_t
{
    LOSS_NONE = -1, // потерять все города и всех героев
    LOSS_LOSE_TOWN = 0, // потерять город
    LOSS_LOSE_HERO = 1, // потерять героя
    LOSS_TIME_EXPIRES = 2, // время вышло

    MAX_LOSS_CONDITIONS = 3
};
#if !NH3API_CHECK_CPP11
NH3API_DISABLE_MSVC_WARNING_END
#endif

class town;
class hero;
#pragma pack(push, 4)
// Victory condition /
// Условие победы.
// size = 0x4C = 76, align = 4
struct VictoryConditionStruct
{
    public:
        NH3API_FORCEINLINE
        VictoryConditionStruct() noexcept
        NH3API_DELEGATE_DUMMY(VictoryConditionStruct)
        { THISCALL_1(void, 0x4BC000, this); }

        NH3API_FORCEINLINE
        VictoryConditionStruct(const ::nh3api::dummy_tag_t&) noexcept
        {}

    public:
        [[nodiscard]] bool applies_to_player(int32_t player) const
        { return THISCALL_2(bool, 0x5F1940, this, player); }

        bool CheckForArtifactWin()
        { return THISCALL_1(bool, 0x5F19B0, this); }

        bool CheckForTotalCreatures()
        { return THISCALL_1(bool, 0x5F1EB0, this); }

        bool CheckForTotalResources()
        { return THISCALL_1(bool, 0x5F2020, this); }

        bool CheckForUpgradedTown()
        { return THISCALL_1(bool, 0x5F20E0, this); }

        bool CheckForGrailBuildingWin()
        { return THISCALL_1(bool, 0x5F2290, this); }

        bool CheckForHeroDefeatWin(int32_t winning_player, const hero* loser)
        { return THISCALL_3(bool, 0x5F24A0, this, winning_player, loser); }

        bool IsGrailTarget(town* thisTown)
        { return THISCALL_2(bool, 0x5F2500, this, thisTown); }

        bool IsTownCaptureTarget(town* thisTown)
        { return THISCALL_2(bool, 0x5F2600, this, thisTown); }

        bool CheckForTownCaptureWin()
        { return THISCALL_1(bool, 0x5F2640, this); }

        bool CheckForDefeatedMonsterWin(const hero* thisHero, type_point monster_loc)
        { return THISCALL_3(bool, 0x5F2730, this, thisHero, monster_loc); }

        bool CheckForFlaggedGeneratorWin()
        { return THISCALL_1(bool, 0x5F29A0, this); }

        bool CheckForFlaggedMineWin()
        { return THISCALL_1(bool, 0x5F2AC0, this); }

        bool CheckForTimeSurvival()
        { return THISCALL_1(bool, 0x5F2BB0, this); }

        bool CheckForArtifactTransportWin(const hero* thisHero, type_point town_loc)
        { return THISCALL_3(bool, 0x5F2C00, this, thisHero, town_loc); }

    public:
        // offset: +0x0 = +0,  size = 0x1 = 1
        EVictoryConditionType Type;

        // offset: +0x1 = +1,  size = 0x1 = 1
        bool AllowNormalVictory;

        // offset: +0x2 = +2,  size = 0x1 = 1
        bool AppliesToComputer;

    protected:
        [[maybe_unused]]
        byte_t gap_3[1];

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t ArtifactNum;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t CreatureType;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t NumCreatures;

        // offset: +0x10 = +16,  size = 0x4 = 4
        EGameResource ResourceType;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t NumResources;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t TownX;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t TownY;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t TownZ;

        // offset: +0x24 = +36,  size = 0x1 = 1
        int8_t HallLevel;

        // offset: +0x25 = +37,  size = 0x1 = 1
        int8_t CastleLevel;

    protected:
        [[maybe_unused]]
        byte_t gap_38[2];

    public:
        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t HeroX;

        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t HeroY;

        // offset: +0x30 = +48,  size = 0x4 = 4
        int32_t HeroZ;

        // offset: +0x34 = +52,  size = 0x4 = 4
        THeroID HeroID;

        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t MonsterX;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t MonsterY;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t MonsterZ;

        // offset: +0x44 = +68,  size = 0x4 = 4
        int32_t time_to_survive;

        // offset: +0x48 = +72,  size = 0x1 = 1
        bool GameWon;

        // offset: +0x49 = +73,  size = 0x1 = 1
        int8_t playerWinner;
    private:
        [[maybe_unused]]
        byte_t gap_74[2];

};
#pragma pack(pop)

#pragma pack(push, 4)
// Loss condition /
// Условие поражения.
// size = 0x24 = 36, align = 4
struct LossConditionStruct
{
    public:
        NH3API_FORCEINLINE
        LossConditionStruct() noexcept
        NH3API_DELEGATE_DUMMY(LossConditionStruct)
        { THISCALL_1(void, 0x45B7A0, this); }

        NH3API_FORCEINLINE
        LossConditionStruct(const ::nh3api::dummy_tag_t&) noexcept
        {}

    public:
        bool CheckForDefeatedHeroLoss(const hero* loser)
        { return THISCALL_2(bool, 0x5F2DE0, this, loser); }

        bool HeroKilled(const hero* loser)
        { return THISCALL_2(bool, 0x5F31B0, this, loser); }

        bool CheckForDefeatedTownLoss(int32_t old_owner, const town* lost_town)
        { return THISCALL_3(bool, 0x5F31E0, this, old_owner, lost_town); }

        bool CheckForTimeLimitExpired()
        { return THISCALL_1(bool, 0x5F32C0, this); }

    public:
        // offset: +0x0 = +0,  size = 0x1 = 1
        ELossConditionType Type;

    protected:
        [[maybe_unused]]
        byte_t gap_2[3];

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t TownX;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t TownY;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t TownZ;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t HeroX;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t HeroY;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t HeroZ;

        // offset: +0x1C = +28,  size = 0x4 = 4
        THeroID HeroID;

        // offset: +0x20 = +32,  size = 0x2 = 2
        int16_t NumDays;

        // offset: +0x22 = +34,  size = 0x1 = 1
        uint8_t GameLost;

        // offset: +0x23 = +35,  size = 0x1 = 1
        int8_t  playerLoser;

};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END
