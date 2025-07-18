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

#include "town.hpp" // town, TownExtra
#include "hero.hpp" // hero, HeroExtra
#include "campaign.hpp" // SCampaign
#include "map.hpp" // NewfullMap

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

enum type_action_type : int32_t
{
    const_initialization_action = 0, //
    const_normal_action         = 1, //
    const_remote_action         = 2, //
    const_recorded_action       = 3 //
};

#pragma pack(push, 1)
// size = 0x4E7D0 = 321488, align = 1
class game
{
    public:
        #pragma pack(push, 4)
        // size = 0x14 = 20, align = 4
        struct TRumour
        {
            // offset: +0x0 = +0,  size = 0x10 = 16
            exe_string RumourText;
            // offset: +0x10 = +16,  size = 0x1 = 1
            bool Unavailable;
        };
        #pragma pack(pop)

    public:
        NH3API_FORCEINLINE
        game() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4CDBE0, this); }

        NH3API_FORCEINLINE
        game(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : townExtraPool(tag),
              BlackMarkets(tag),
              sMapHeader(tag),
              worldMap(tag),
              townPool(tag),
              signPool(tag),
              minePool(tag),
              generatorPool(tag),
              garrisonPool(tag),
              boatPool(tag),
              university_pool(tag),
              creature_banks(tag),
              MapRumours(tag),
              whirlpools(tag),
              underground_gates(tag),
              underground_gate_exits(tag),
              recorded_events(tag),
              quest_monsters(tag)
        { THISCALL_1(void, 0x4CDBE0, this); }

        NH3API_FORCEINLINE
        ~game() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4CE270, this); }

    public:
        bool IsMultiplayer() const
        { return THISCALL_1(bool, 0x4CE950, this); }

        bool OnSameTeam(int32_t player1, int32_t player2) const
        { return THISCALL_3(bool, 0x529BB0, this, player1, player2); }

        bool IsHuman(int32_t gamePos) const
        { return THISCALL_2(bool, 0x4CE600, this, gamePos); }

        bool IsLocalHuman(int32_t cur_player) const
        { return THISCALL_2(bool, 0x4CE630, this, cur_player); }

        playerData* GetLocalPlayer()
        { return THISCALL_1(playerData*, 0x4CE670, this); }

        const playerData* GetLocalPlayer() const
        { return THISCALL_1(playerData*, 0x4CE670, this); }

        int32_t GetLocalPlayerGamePos() const
        { return THISCALL_1(int32_t, 0x4CE6E0, this); }

        bool is_human_ally(int32_t player_number) const
        { return THISCALL_2(bool, 0x42B9E0, this, player_number); }

        const char* GetPlayerName(int32_t player)
        { return THISCALL_2(char*, 0x4CE820, this, player); }

        int32_t GetTeam(int32_t player) const
        { return THISCALL_2(int32_t, 0x4A55D0, this, player); }

        town* GetTown(int32_t whichTown)
        { return THISCALL_2(town*, 0x42BA30, this, whichTown); }

        const town* GetTown(int32_t whichTown) const
        { return THISCALL_2(town*, 0x42BA30, this, whichTown); }

        NewmapCell* get_cell(type_point point)
        { return THISCALL_2(NewmapCell*, 0x42ED80, this, point); }

        const NewmapCell* get_cell(type_point point) const
        { return THISCALL_2(NewmapCell*, 0x42ED80, this, point); }

        NewmapCell* get_cell(uint8_t x, uint8_t y, uint8_t z)
        { return &this->worldMap.cellData[x + this->worldMap.Size * (y + z * this->worldMap.Size)]; }

        const NewmapCell* get_cell(uint8_t x, uint8_t y, uint8_t z) const
        { return &this->worldMap.cellData[x + this->worldMap.Size * (y + z * this->worldMap.Size)]; }

        hero* GetHero(int32_t which)
        { return THISCALL_2(hero*, 0x4317D0, this, which); }

        const hero* GetHero(int32_t which) const
        { return THISCALL_2(hero*, 0x4317D0, this, which); }

        int32_t get_new_boat_id()
        { return THISCALL_1(int32_t, 0x4BAE30, this); }

        int32_t CreateBoat(int32_t x, int32_t y, int32_t z, int32_t owner, bool bIsRemoteMove, int8_t type)
        { return THISCALL_7(int32_t, 0x4BAF10, this, x, y, z, owner, bIsRemoteMove, type); }

        boat* GetHeroBoat(int32_t id, bool occupied)
        { return THISCALL_3(boat*, 0x4CE5C0, this, id, occupied); }

        boat* GetBoat(int32_t id)
        { return id > 0 ? &boatPool[id] : nullptr; }

        const boat* GetBoat(int32_t id) const
        { return id > 0 ? &boatPool[id] : nullptr; }

        THeroID GetStartingHeroId(TTownType alignment, int32_t playerPos, type_point mapPosition)
        { return THISCALL_4(THeroID, 0x4BB0C0, this, alignment, playerPos, mapPosition); }

        THeroID GetNewHeroId(TTownType alignment, THeroClass excluded, bool prefer_alignment, THeroClass target)
        { return THISCALL_5(THeroID, 0x4BB2A0, this, alignment, excluded, prefer_alignment, target); }

        int32_t GetTownId(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(int32_t, 0x4BB530, this, x, y, z); }

        int32_t GetGeneratorId(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(int32_t, 0x4BB5C0, this, x, y, z); }

        void SetVisibility(int32_t startX, int32_t startY, int32_t z, int32_t whichPlayer, int32_t range, int32_t remote_move)
        { THISCALL_7(void, 0x49CDD0, this, startX, startY, z, whichPlayer, range, remote_move); }

        void ResetVisibility(int32_t start_x, int32_t start_y, int32_t z, int32_t whichPlayer, int32_t range)
        { THISCALL_6(void, 0x49D040, this, start_x, start_y, z, whichPlayer, range); }

        TCreatureType GetRandomMonster(int32_t minLevel, int32_t maxLevel)
        { return THISCALL_3(TCreatureType, 0x4C8F80, this, minLevel, maxLevel); }

        TArtifact GetRandomArtifactId(int32_t ArtifactClass)
        { return THISCALL_2(TArtifact, 0x4C9190, this, ArtifactClass); }

        SpellID GetRandomSpell(const exe_bitset<5>& spell_level)
        { return THISCALL_2(SpellID, 0x4C9260, this, &spell_level); }

        void InsertObject(int32_t x, int32_t y, int32_t z, TAdventureObjectType objType, int32_t objectIndex, int32_t extraInfo)
        { THISCALL_7(void, 0x4C9550, this, x, y, z, objType, objectIndex, extraInfo); }

        void MakeTerrainVisible(int32_t whichPlayer, uint16_t visMask)
        { THISCALL_3(void, 0x4C9F00, this, whichPlayer, visMask); }

        void ConvertObject(NewmapCell* tempCell)
        { THISCALL_2(void, 0x4C9650, this, tempCell); }

        void ClaimTown(int32_t townId, int32_t newPlayerOwner, bool bIsRemoteMove, bool check_end_game)
        { THISCALL_5(void, 0x4C5EA0, this, townId, newPlayerOwner, bIsRemoteMove, check_end_game); }

        void ClaimMine(int32_t mineId, int32_t newPlayerOwner, type_action_type action_type)
        { THISCALL_4(void, 0x4C63A0, this, mineId, newPlayerOwner, action_type); }

        void ClaimGenerator(int32_t generatorId, int32_t newPlayerOwner)
        { THISCALL_3(void, 0x4C6470, this, generatorId, newPlayerOwner); }

        void ClaimGarrison(int32_t garrisonId, int32_t newPlayerOwner)
        { THISCALL_3(void, 0x4C6620, this, garrisonId, newPlayerOwner); }

        void ClaimShipyard(type_point point, int32_t newPlayerOwner)
        { THISCALL_3(void, 0x4C66F0, this, point, newPlayerOwner); }

        bool get_random_lith(const exe_vector<type_point>& points, type_point& result, int32_t cell_type, int32_t excluded) const
        { return THISCALL_5(bool, 0x4CD840, this, &points, &result, cell_type, excluded); }

        bool get_random_lith_exit(int32_t color, type_point* result) const
        { return THISCALL_3(bool, 0x4CDA80, this, color, result); }

        bool get_random_lith(int32_t color, int32_t excluded, type_point& result) const
        { return THISCALL_4(bool, 0x4CDAB0, this, color, excluded, &result); }

        bool get_random_whirlpool(int32_t excluded, type_point& result) const
        { return THISCALL_3(bool, 0x4CDAE0, this, excluded, &result); }

        type_point get_underground_gate_exit(const NewmapCell* cell) const
        { return THISCALL_2(type_point, 0x4CDB00, this, cell); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        heroWindow* newGameWin;

        // offset: +0x4 = +4,  size = 0x46 = 70
        std::array<uint8_t, 70> spellAllocInfo;

        // offset: +0x4A = +74,  size = 0x46 = 70
        std::array<uint8_t, 70> spellDisabledInfo;

        // offset: +0x90 = +144,  size = 0x4 = 4
        LPCRITICAL_SECTION bink_critical_section;

        // offset: +0x94 = +148,  size = 0x10 = 16
        exe_vector<TownExtra> townExtraPool;

        // offset: +0xA4 = +164,  size = 0x1F3B0 = 127920
        std::array<HeroExtra, 156> heroExtraPool;

        // offset: +0x1F454 = +128084,  size = 0x4 = 4
        int32_t difficultyRating;

        // offset: +0x1F458 = +128088,  size = 0x7C = 124
        SCampaign sCampaign;

        // offset: +0x1F4D4 = +128212,  size = 0x1 = 1
        bool bNewCampaignStarted;

        // offset: +0x1F4D5 = +128213,  size = 0x15F = 351
        std::array<char, 351> cGameFilename;

        // offset: +0x1F634 = +128564,  size = 0x1 = 1
        int8_t numPlayers;

        // offset: +0x1F635 = +128565,  size = 0x1 = 1
        int8_t numDeadPlayers;

        // offset: +0x1F636 = +128566,  size = 0x8 = 8
        std::array<bool, 8> playerDead;

        // offset: +0x1F63E = +128574,  size = 0x2 = 2
        uint16_t day;

        // offset: +0x1F640 = +128576,  size = 0x2 = 2
        uint16_t week;

        // offset: +0x1F642 = +128578,  size = 0x2 = 2
        uint16_t month;

        // offset: +0x1F644 = +128580,  size = 0x20 = 32
        std::array<char, 32> cUniqueSystemID;

        // offset: +0x1F664 = +128612,  size = 0x1C = 28
        std::array<TArtifact, 7> marketArtifacts;

        // offset: +0x1F680 = +128640,  size = 0x10 = 16
        exe_vector<TBlackMarket> BlackMarkets;

        // offset: +0x1F690 = +128656,  size = 0x2 = 2
        int16_t ultimateX;

        // offset: +0x1F692 = +128658,  size = 0x2 = 2
        int16_t ultimateY;

        // offset: +0x1F694 = +128660,  size = 0x1 = 1
        uint8_t ultimateZ;

        // offset: +0x1F695 = +128661,  size = 0x1 = 1
        uint8_t ultimateRadius;

        // offset: +0x1F696 = +128662,  size = 0x1 = 1
        uint8_t ultimateValid;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1F697 = +128663,  size = 0x1 = 1
        byte_t gap_1F697[1];

    public:
        // offset: +0x1F698 = +128664,  size = 0x4 = 4
        int32_t iGameType;

        // offset: +0x1F69C = +128668,  size = 0x1 = 1
        bool bIsCheater;

        // offset: +0x1F69D = +128669,  size = 0x1 = 1
        bool is_tutorial;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1F69E = +128670,  size = 0x2 = 2
        byte_t gap_1F69E[2];

    public:
        // offset: +0x1F6A0 = +128672,  size = 0x1CC = 460
        SGameSetupOptions sSetup;

        // offset: +0x1F86C = +129132,  size = 0x304 = 772
        NewSMapHeader sMapHeader;

        // offset: +0x1FB70 = +129904,  size = 0xF5C = 3932
        NewfullMap worldMap;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x20ACC = +133836,  size = 0x4 = 4
        byte_t gap_20ACC[4];

    public:
        // offset: +0x20AD0 = +133840,  size = 0xB40 = 2880
        std::array<playerData, 8> player;

        // offset: +0x21610 = +136720,  size = 0x10 = 16
        exe_vector<town> townPool;

        // offset: +0x21620 = +136736,  size = 0x2C8F8 = 182520
        std::array<hero, MAX_HEROES> heroPool;

        // offset: +0x4DF18 = +319256,  size = 0x9C = 156
        std::array<uint8_t, MAX_HEROES> heroAllocInfo;

        // offset: +0x4DFB4 = +319412,  size = 0x270 = 624
        std::array<exe_bitset<8>, MAX_HEROES> heroAvailable;

        // offset: +0x4E224 = +320036,  size = 0x90 = 144
        std::array<uint8_t, MAX_ARTIFACTS> artifactAllocInfo;

        // offset: +0x4E2B4 = +320180,  size = 0x90 = 144
        std::array<uint8_t, MAX_ARTIFACTS> reservedArtifactInfo;

        // offset: +0x4E344 = +320324,  size = 0x20 = 32
        std::array<uint8_t, 32> InfoFlags;

        // offset: +0x4E364 = +320356,  size = 0x8 = 8
        std::array<uint8_t, 8> GuardFlags;

        // offset: +0x4E36C = +320364,  size = 0x6 = 6
        std::array<uint16_t, 3> cartographerMask;

        // offset: +0x4E372 = +320370,  size = 0x3 = 3
        std::array<uint8_t, 3> cartographerFlags;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_4E375[3];

    public:
        // offset: +0x4E378 = +320376,  size = 0x10 = 16
        exe_vector<Sign> signPool;

        // offset: +0x4E388 = +320392,  size = 0x10 = 16
        exe_vector<mine> minePool;

        // offset: +0x4E398 = +320408,  size = 0x10 = 16
        exe_vector<generator> generatorPool;

        // offset: +0x4E3A8 = +320424,  size = 0x10 = 16
        exe_vector<garrison> garrisonPool;

        // offset: +0x4E3B8 = +320440,  size = 0x10 = 16
        exe_vector<boat> boatPool;

        // offset: +0x4E3C8 = +320456,  size = 0x10 = 16
        exe_vector<type_university> university_pool;

        // offset: +0x4E3D8 = +320472,  size = 0x10 = 16
        exe_vector<type_creature_bank> creature_banks;

        // offset: +0x4E3E8 = +320488,  size = 0x1 = 1
        int8_t numObelisks;

        // offset: +0x4E3E9 = +320489,  size = 0x30 = 48
        std::array<int8_t, 48> obeliskPool;

        // offset: +0x4E419 = +320537,  size = 0x12C = 300
        std::array<char, 301> cCurRumour;

        // offset: +0x4E546 = +320838,  size = 0x100 = 256
        std::array<bool, 256> rumourAllocInfo;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x4E646 = +321094,  size = 0x2 = 2
        byte_t gap_4E646[2];

    public:
        // offset: +0x4E648 = +321096,  size = 0x10 = 16
        exe_vector<TRumour> MapRumours;

        // offset: +0x4E658 = +321112,  size = 0x1C = 28
        std::array<bool, MAX_SECONDARY_SKILLS> ss_disabled;

        // offset: +0x4E674 = +321140,  size = 0x4 = 4
        heroWindow* armyWindow;

    protected:
        // offset: +0x4E678 = +321144,  size = 0x4 = 4
        byte_t gap_4E678[4];

    public:
        // offset: +0x4E67C = +321148,  size = 0x80 = 128
        std::array<exe_vector<type_point>, 8> two_way_liths;

        // offset: +0x4E6FC = +321276,  size = 0x80 = 128
        std::array<exe_vector<type_point>, 8> lith_exits;

        // offset: +0x4E77C = +321404,  size = 0x10 = 16
        exe_vector<type_point> whirlpools;

        // offset: +0x4E78C = +321420,  size = 0x10 = 16
        exe_vector<type_point> underground_gates;

        // offset: +0x4E79C = +321436,  size = 0x10 = 16
        exe_vector<type_point> underground_gate_exits;

        // offset: +0x4E7AC = +321452,  size = 0x10 = 16
        exe_vector<void*> recorded_events;
        //exe_vector<type_event_record*> recorded_events;
        
        // offset: +0x4E7BC = +321468,  size = 0x10 = 16
        exe_vector<QuestMonster> quest_monsters;

    protected:
        // offset: +0x4E7CC = +321484,  size = 0x4 = 4
        byte_t gap_4E7CC[4];
};
#pragma pack(pop)

NH3API_SIZE_ASSERT(0x4E7D0, game);

NH3API_INLINE_OR_EXTERN
game*& gpGame
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699538, game*));

NH3API_DISABLE_WARNING_END