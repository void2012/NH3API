//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "../creatures.hpp"
#include "../base_manager.hpp"

#pragma pack(push, 4)
// size = 0xBC = 188, align = 4, baseclass: baseManager
class recruitUnit : public baseManager
{
    public:
        recruitUnit() NH3API_DELETED_FUNCTION

        recruitUnit(armyGroup* newGroup,
                    bool bGroupIsTownGarrison,
                    TCreatureType MonType1_,
                    int16_t* numMon1,
                    TCreatureType MonType2_,
                    int16_t* numMon2,
                    TCreatureType MonType3_,
                    int16_t* numMon3,
                    TCreatureType MonType4_,
                    int16_t* numMon4) NH3API_NOEXCEPT
            : recruitUnit(::nh3api::dummy_tag)
        { THISCALL_11(void, 0x551750, this, newGroup, bGroupIsTownGarrison, MonType1_, numMon1, MonType2_, numMon2, MonType3_, numMon3, MonType4_, numMon4); }

        recruitUnit(hero* thisHero_,
                    TCreatureType MonType1_,
                    int16_t* numMon1,
                    TCreatureType MonType2_,
                    int16_t* numMon2,
                    TCreatureType MonType3_,
                    int16_t* numMon3,
                    TCreatureType MonType4_,
                    int16_t* numMon4) NH3API_NOEXCEPT
            : recruitUnit(::nh3api::dummy_tag)
        { THISCALL_10(void, 0x551860, this, thisHero_, MonType1_, numMon1, MonType2_, numMon2, MonType3_, numMon3, MonType4_, numMon4); }

        recruitUnit(town* newTown,
                    int32_t newDwellingIndex,
                    int32_t bInInTownMainScreen) NH3API_NOEXCEPT
            : recruitUnit(::nh3api::dummy_tag)
        { THISCALL_4(void, 0x551960, this, newTown, newDwellingIndex, bInInTownMainScreen); }

        recruitUnit(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : baseManager(tag)
        {}

        NH3API_DEFAULT_DESTRUCTOR(recruitUnit)

    public:
        void Update(bool new_monster, int32_t slot) NH3API_NOEXCEPT
        { THISCALL_3(void, 0x5507A0, this, new_monster, slot); }

    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(recruitUnit)

    public:
        // offset: +0x38 = +56,  size = 0x10 = 16
        std::array<int32_t, 4> CurrentSpriteFrame;

        // offset: +0x48 = +72,  size = 0x4 = 4
        int32_t type;

        // offset: +0x4C = +76,  size = 0x1 = 1
        bool view_only;

        // offset: +0x50 = +80,  size = 0x4 = 4
        TCreatureType monsterType;

        // offset: +0x54 = +84,  size = 0x4 = 4
        int16_t* numAvail;

        // offset: +0x58 = +88,  size = 0x4 = 4
        int32_t selectedPosition;

        // offset: +0x5C = +92,  size = 0x4 = 4
        TCreatureType MonType1;

        // offset: +0x60 = +96,  size = 0x4 = 4
        TCreatureType MonType2;

        // offset: +0x64 = +100,  size = 0x4 = 4
        TCreatureType MonType3;

        // offset: +0x68 = +104,  size = 0x4 = 4
        TCreatureType MonType4;

        // offset: +0x6C = +108,  size = 0x10 = 16
        std::array<int16_t*, 4> available;

        // offset: +0x7C = +124,  size = 0x4 = 4
        hero* thisHero;

        // offset: +0x80 = +128,  size = 0x4 = 4
        int32_t* availSource;

        // offset: +0x84 = +132,  size = 0x4 = 4
        int32_t goldPerTroop;

        // offset: +0x88 = +136,  size = 0x4 = 4
        int32_t altResource;

        // offset: +0x8C = +140,  size = 0x4 = 4
        int32_t resourcesPerTroop;

        // offset: +0x90 = +144,  size = 0x4 = 4
        int32_t bInTownMainScreen;

        // offset: +0x94 = +148,  size = 0x4 = 4
        heroWindow* errorWin;

        // offset: +0x98 = +152,  size = 0x4 = 4
        armyGroup* currArmyGroup;

        // offset: +0x9C = +156,  size = 0x1 = 1
        bool bCurrArmyGroupIsTownGarrison;

        // offset: +0xA0 = +160,  size = 0x4 = 4
        int32_t addIndex;

        // offset: +0xA4 = +164,  size = 0x4 = 4
        int32_t updateNeeded;

        // offset: +0xA8 = +168,  size = 0x4 = 4
        int32_t errorExit;

        // offset: +0xAC = +172,  size = 0x4 = 4
        int32_t maxAvail;

        // offset: +0xB0 = +176,  size = 0x4 = 4
        int32_t totalGold;

        // offset: +0xB4 = +180,  size = 0x4 = 4
        int32_t totalResources;

        // offset: +0xB8 = +184,  size = 0x4 = 4
        int32_t numberToBuy;

};
#pragma pack(pop)

NH3API_SPECIALIZE_TYPE_VFTABLE(0x640C80, recruitUnit)
