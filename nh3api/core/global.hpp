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

#include "campaign.hpp"    // SCampaign
#include "hero.hpp"        // hero, HeroExtra
#include "map.hpp"         // NewfullMap
#include "multiplayer.hpp" // eNetGameType
#include "town.hpp"        // town, TownExtra

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

enum type_action_type : int32_t
{
    const_initialization_action = 0, //
    const_normal_action         = 1, //
    const_remote_action         = 2, //
    const_recorded_action       = 3  //
};

template<>
struct nh3api::enum_limits<type_action_type>
    : nh3api::enum_limits_base<type_action_type, const_initialization_action, const_recorded_action>
{ static inline constexpr bool is_specialized = true; };

enum type_event_record_type : uint32_t
{
    const_record_none         = 0, //
    const_record_move_hero    = 1, //
    const_record_teleport     = 2, //
    const_record_claim_mine   = 3, //
    const_record_claim_town   = 4, //
    const_record_hide_boat    = 5, //
    const_record_show_boat    = 6, //
    const_record_erase        = 7, //
    const_record_hide_hero    = 8, //
    const_record_show_hero    = 9, //
    const_record_player_death = 10, //
    const_record_shroud       = 11, //
};

template<>
struct nh3api::enum_limits<type_event_record_type>
    : nh3api::enum_limits_base<type_event_record_type, const_record_move_hero, const_record_shroud>
{ static inline constexpr bool is_specialized = true; };

#pragma pack(push, 4)
// Turn player actions for the replay /
// Действия игрока, воспроизводимые кнопкой "повторить ход".
// size = 0x8 = 8, align = 4
struct type_event_record
{
    public:
        struct vftable_t
        {
            void (__thiscall* scalar_deleting_destructor)(type_event_record*, uint8_t);
            type_event_record_type (__thiscall* get_type)(const type_event_record*);
            bool (__thiscall* load)(type_event_record*, TAbstractFile*, int32_t);
            bool (__thiscall* save)(const type_event_record*, TAbstractFile*, int32_t);
            void (__thiscall* replay)(type_event_record*, bool);
            void (__thiscall* undo)(type_event_record*);
        };

    public:
        [[nodiscard]] int8_t get_player_id() const noexcept
        { return player_id; }

    public:
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        [[nodiscard]] virtual type_event_record_type __thiscall get_type() const
        { return get_vftable(this)->get_type(this); }

        // vftable shift: +8
        virtual bool __thiscall load(TAbstractFile* file, int32_t version)
        { return get_vftable(this)->load(this, file, version); }

        // vftable shift: +12
        virtual bool __thiscall save(TAbstractFile* file, int32_t version) const
        { return get_vftable(this)->save(this, file, version); }

        // vftable shift: +16
        virtual void __thiscall replay(bool a1)
        { get_vftable(this)->replay(this, a1); }

        // vftable shift: +20
        virtual void __thiscall undo()
        { get_vftable(this)->undo(this); }

    public:
        // offset: +0x4 = +4,  size = 0x1 = 1
        int8_t player_id;

};

// size = 0x4E7D0 = 321488, align = 1
class game
{
    public:
        // size = 0x14 = 20, align = 4
        struct TRumour
        {
            // offset: +0x0 = +0,  size = 0x10 = 16
            exe_string RumourText;

            // offset: +0x10 = +16,  size = 0x1 = 1
            bool Unavailable;

            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } NH3API_MSVC_LAYOUT;

    public:
        inline game() noexcept
            : game(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4CDBE0, this); }

        inline game(const nh3api::dummy_tag_t& tag) noexcept
            : townExtraPool { tag },
              sCampaign { tag },
              BlackMarkets { tag },
              sMapHeader { tag },
              worldMap { tag },
              player { { tag, tag, tag, tag, tag, tag, tag, tag } },
              townPool { tag },
              signPool { tag },
              minePool { tag },
              generatorPool { tag },
              garrisonPool { tag },
              boatPool { tag },
              university_pool { tag },
              creature_banks { tag },
              MapRumours { tag },
              whirlpools { tag },
              underground_gates { tag },
              underground_gate_exits { tag },
              recorded_events { tag },
              quest_monsters { tag }
        { THISCALL_1(void, 0x4CDBE0, this); }

        game(const game& other)            = delete;
        game(game&& other)                 = delete;
        game& operator=(const game& other) = delete;
        game& operator=(game&& other)      = delete;

        inline ~game() noexcept
        { THISCALL_1(void, 0x4CE270, this); }

    public:
        [[nodiscard]] bool is_human_ally(int32_t player_number) const
        { return THISCALL_2(bool, 0x42B9E0, this, player_number); }

        [[nodiscard]] town* GetTown(int32_t whichTown)
        { return THISCALL_2(town*, 0x42BA30, this, whichTown); }

        [[nodiscard]] const town* GetTown(int32_t whichTown) const
        { return THISCALL_2(town*, 0x42BA30, this, whichTown); }

        [[nodiscard]] int32_t GetNumMapLevels() const noexcept
        { return worldMap.HasTwoLevels ? 2 : 1; }

        [[nodiscard]] bool OnSameTeam(int32_t player1, int32_t player2) const
        { return THISCALL_3(bool, 0x529BB0, this, player1, player2); }

        [[nodiscard]] NewmapCell* get_cell(type_point point)
        { return THISCALL_2(NewmapCell*, 0x42ED80, this, point); }

        [[nodiscard]] const NewmapCell* get_cell(type_point point) const
        { return THISCALL_2(NewmapCell*, 0x42ED80, this, point); }

        [[nodiscard]] NewmapCell* get_cell(uint8_t x, uint8_t y, uint8_t z)
        { return &this->worldMap.cellData[x + this->worldMap.Size * (y + z * this->worldMap.Size)]; }

        [[nodiscard]] const NewmapCell* get_cell(uint8_t x, uint8_t y, uint8_t z) const
        { return &this->worldMap.cellData[x + this->worldMap.Size * (y + z * this->worldMap.Size)]; }

        [[nodiscard]] bool IsMultiplayer() const
        { return THISCALL_1(bool, 0x4CE950, this); }

        [[nodiscard]] bool IsHuman(int32_t gamePos) const
        { return THISCALL_2(bool, 0x4CE600, this, gamePos); }

        [[nodiscard]] bool IsLocalHuman(int32_t cur_player) const
        { return THISCALL_2(bool, 0x4CE630, this, cur_player); }

        [[nodiscard]] playerData* GetLocalPlayer()
        { return THISCALL_1(playerData*, 0x4CE670, this); }

        [[nodiscard]] const playerData* GetLocalPlayer() const
        { return THISCALL_1(playerData*, 0x4CE670, this); }

        [[nodiscard]] int32_t GetLocalPlayerGamePos() const
        { return THISCALL_1(int32_t, 0x4CE6E0, this); }

        [[nodiscard]] const char* GetPlayerName(int32_t player_id)
        { return THISCALL_2(char*, 0x4CE820, this, player_id); }

        [[nodiscard]] int32_t GetTeam(int32_t player_id) const
        { return THISCALL_2(int32_t, 0x4A55D0, this, player_id); }

        [[nodiscard]] hero* GetHero(int32_t which)
        { return THISCALL_2(hero*, 0x4317D0, this, which); }

        [[nodiscard]] const hero* GetHero(int32_t which) const
        { return THISCALL_2(hero*, 0x4317D0, this, which); }

        [[nodiscard]] int32_t get_new_boat_id()
        { return THISCALL_1(int32_t, 0x4BAE30, this); }

        int32_t CreateBoat(int32_t x, int32_t y, int32_t z, int32_t owner, bool bIsRemoteMove, int8_t type)
        { return THISCALL_7(int32_t, 0x4BAF10, this, x, y, z, owner, bIsRemoteMove, type); }

        [[nodiscard]] boat* GetHeroBoat(int32_t id, bool occupied)
        { return THISCALL_3(boat*, 0x4CE5C0, this, id, occupied); }

        [[nodiscard]] boat* GetBoat(int32_t id)
        { return id > 0 ? &boatPool[static_cast<size_t>(id)] : nullptr; }

        [[nodiscard]] const boat* GetBoat(int32_t id) const
        { return id > 0 ? &boatPool[static_cast<size_t>(id)] : nullptr; }

        [[nodiscard]] THeroID GetStartingHeroId(TTownType alignment, int32_t playerPos, type_point mapPosition)
        { return THISCALL_4(THeroID, 0x4BB0C0, this, alignment, playerPos, mapPosition); }

        [[nodiscard]] THeroID GetNewHeroId(TTownType alignment, THeroClass excluded, bool prefer_alignment, THeroClass target)
        { return THISCALL_5(THeroID, 0x4BB2A0, this, alignment, excluded, prefer_alignment, target); }

        [[nodiscard]] int32_t GetTownId(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(int32_t, 0x4BB530, this, x, y, z); }

        [[nodiscard]] int32_t GetGeneratorId(int32_t x, int32_t y, int32_t z)
        { return THISCALL_4(int32_t, 0x4BB5C0, this, x, y, z); }

        void SetVisibility(int32_t startX, int32_t startY, int32_t z, int32_t whichPlayer, int32_t range, int32_t remote_move)
        { THISCALL_7(void, 0x49CDD0, this, startX, startY, z, whichPlayer, range, remote_move); }

        void ResetVisibility(int32_t start_x, int32_t start_y, int32_t z, int32_t whichPlayer, int32_t range)
        { THISCALL_6(void, 0x49D040, this, start_x, start_y, z, whichPlayer, range); }

        void clear_event_records()
        { THISCALL_1(void, 0x49D2A0, this); }

        void clear_event_records(int8_t player_id)
        { THISCALL_2(void, 0x49D330, this, player_id); }

        [[nodiscard]] TCreatureType GetRandomMonster(int32_t minLevel, int32_t maxLevel)
        { return THISCALL_3(TCreatureType, 0x4C8F80, this, minLevel, maxLevel); }

        [[nodiscard]] TArtifact GetRandomArtifactId(int32_t ArtifactClass)
        { return THISCALL_2(TArtifact, 0x4C9190, this, ArtifactClass); }

        [[nodiscard]] SpellID GetRandomSpell(const exe_bitset<5>& spell_level)
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

        [[nodiscard]] type_point get_underground_gate_exit(const NewmapCell* cell) const
        { return THISCALL_2(type_point, 0x4CDB00, this, cell); }

        [[nodiscard]] TCreatureType UpgradedCreatureType(TCreatureType creature) const
        { return THISCALL_2(TCreatureType, 0x529BF0, this, creature); }

    public:
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x4 = +4,  size = 0x46 = 70
        std::array<uint8_t, 70> spellAllocInfo;

        // offset: +0x4A = +74,  size = 0x46 = 70
        std::array<uint8_t, 70> spellDisabledInfo;

        // offset: +0x90 = +144,  size = 0x4 = 4
        PRTL_CRITICAL_SECTION bink_critical_section;

        union {
        // offset: +0x94 = +148,  size = 0x10 = 16
        exe_vector<TownExtra> townExtraPool;
        };

        union {
        // offset: +0xA4 = +164,  size = 0x1F3B0 = 127920
        std::array<HeroExtra, 156> heroExtraPool;
        };

        // offset: +0x1F454 = +128084,  size = 0x4 = 4
        int32_t difficultyRating;

        union {
        // offset: +0x1F458 = +128088,  size = 0x7C = 124
        SCampaign sCampaign;
        };

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

        union {
        // offset: +0x1F680 = +128640,  size = 0x10 = 16
        exe_vector<TBlackMarket> BlackMarkets;
        };

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

        unsigned char : 8;

        // offset: +0x1F698 = +128664,  size = 0x4 = 4
        int32_t iGameType;

        // offset: +0x1F69C = +128668,  size = 0x1 = 1
        bool bIsCheater;

        // offset: +0x1F69D = +128669,  size = 0x1 = 1
        bool is_tutorial;

        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x1F6A0 = +128672,  size = 0x1CC = 460
        SGameSetupOptions sSetup;
        // ^^^ trivial ^^^

        union {
        // offset: +0x1F86C = +129132,  size = 0x304 = 772
        NewSMapHeader sMapHeader;
        };

        union {
        // offset: +0x1FB70 = +129904,  size = 0xF5C = 3932
        NewfullMap worldMap;
        };

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        union {
        // offset: +0x20AD0 = +133840,  size = 0xB40 = 2880
        std::array<playerData, 8> player;
        };

        union {
        // offset: +0x21610 = +136720,  size = 0x10 = 16
        exe_vector<town> townPool;
        };

        union {
        // offset: +0x21620 = +136736,  size = 0x2C8F8 = 182520
        std::array<hero, MAX_HEROES> heroPool;
        };

        // offset: +0x4DF18 = +319256,  size = 0x9C = 156
        std::array<uint8_t, MAX_HEROES> heroAllocInfo;

        union {
        // offset: +0x4DFB4 = +319412,  size = 0x270 = 624
        std::array<exe_bitset<8>, MAX_HEROES> heroAvailable;
        };

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

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        union {
        // offset: +0x4E378 = +320376,  size = 0x10 = 16
        exe_vector<Sign> signPool;
        };

        union {
        // offset: +0x4E388 = +320392,  size = 0x10 = 16
        exe_vector<mine> minePool;
        };

        union {
        // offset: +0x4E398 = +320408,  size = 0x10 = 16
        exe_vector<generator> generatorPool;
        };

        union {
        // offset: +0x4E3A8 = +320424,  size = 0x10 = 16
        exe_vector<garrison> garrisonPool;
        };

        union {
        // offset: +0x4E3B8 = +320440,  size = 0x10 = 16
        exe_vector<boat> boatPool;
        };

        union {
        // offset: +0x4E3C8 = +320456,  size = 0x10 = 16
        exe_vector<type_university> university_pool;
        };

        union {
        // offset: +0x4E3D8 = +320472,  size = 0x10 = 16
        exe_vector<type_creature_bank> creature_banks;
        };

        // offset: +0x4E3E8 = +320488,  size = 0x1 = 1
        int8_t numObelisks;

        // offset: +0x4E3E9 = +320489,  size = 0x30 = 48
        std::array<int8_t, 48> obeliskPool;

        // offset: +0x4E419 = +320537,  size = 0x12C = 300
        std::array<char, 301> cCurRumour;

        // offset: +0x4E546 = +320838,  size = 0x100 = 256
        std::array<bool, 256> rumourAllocInfo;

        unsigned char : 8;
        unsigned char : 8;

        union {
        // offset: +0x4E648 = +321096,  size = 0x10 = 16
        exe_vector<TRumour> MapRumours;
        };

        // offset: +0x4E658 = +321112,  size = 0x1C = 28
        std::array<bool, MAX_SECONDARY_SKILLS> ss_disabled;

        // offset: +0x4E674 = +321140,  size = 0x4 = 4
        heroWindow* armyWindow;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        union {
        // offset: +0x4E67C = +321148,  size = 0x80 = 128
        std::array<exe_vector<type_point>, 8> two_way_liths;
        };

        union {
        // offset: +0x4E6FC = +321276,  size = 0x80 = 128
        std::array<exe_vector<type_point>, 8> lith_exits;
        };

        union {
        // offset: +0x4E77C = +321404,  size = 0x10 = 16
        exe_vector<type_point> whirlpools;
        };

        union {
        // offset: +0x4E78C = +321420,  size = 0x10 = 16
        exe_vector<type_point> underground_gates;
        };

        union {
        // offset: +0x4E79C = +321436,  size = 0x10 = 16
        exe_vector<type_point> underground_gate_exits;
        };

        union {
        // offset: +0x4E7AC = +321452,  size = 0x10 = 16
        exe_vector<type_event_record*> recorded_events;
        };

        union {
        // offset: +0x4E7BC = +321468,  size = 0x10 = 16
        exe_vector<QuestMonster> quest_monsters;
        };

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;

NH3API_SIZE_ASSERT(0x4E7D0, game);

// size = 0xD4 = 212, align = 4
struct configStruct
{
public:
    // offset: +0x0 = +0,  size = 0x8 = 8
    std::array<int32_t, 2> walkSpeed;

    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t musicVolume;

    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t soundVolume;

    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t lastMusicVolume;

    // offset: +0x14 = +20,  size = 0x4 = 4
    int32_t lastSoundVolume;

    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t AutoSave;

    // offset: +0x1C = +28,  size = 0x4 = 4
    int32_t ShowRoute;

    // offset: +0x20 = +32,  size = 0x4 = 4
    int32_t MoveReminder;

    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t QuickCombat;

    // offset: +0x28 = +40,  size = 0x4 = 4
    int32_t VideoSubtitles;

    // offset: +0x2C = +44,  size = 0x4 = 4
    int32_t TownOutlines;

    // offset: +0x30 = +48,  size = 0x4 = 4
    int32_t AnimateSpellBook;

    // offset: +0x34 = +52,  size = 0x4 = 4
    int32_t WindowScrollSpeed;

    // offset: +0x38 = +56,  size = 0x4 = 4
    int32_t BlackoutComputer;

    // offset: +0x3C = +60,  size = 0x4 = 4
    int32_t AutoCreatures;

    // offset: +0x40 = +64,  size = 0x4 = 4
    int32_t AutoSpells;

    // offset: +0x44 = +68,  size = 0x4 = 4
    int32_t AutoCatapult;

    // offset: +0x48 = +72,  size = 0x4 = 4
    int32_t AutoBallista;

    // offset: +0x4C = +76,  size = 0x4 = 4
    int32_t AutoFirstAidTent;

    // offset: +0x50 = +80,  size = 0x4 = 4
    int32_t PreferBink;

    // offset: +0x54 = +84,  size = 0x4 = 4
    int32_t MainGameShowMenu;

    // offset: +0x58 = +88,  size = 0x4 = 4
    int32_t ScreenX;

    // offset: +0x5C = +92,  size = 0x4 = 4
    int32_t ScreenY;

    // offset: +0x60 = +96,  size = 0x4 = 4
    int32_t FullScreen;

    // offset: +0x64 = +100,  size = 0x4 = 4
    int32_t bCombatShowEntireGrid;

    // offset: +0x68 = +104,  size = 0x4 = 4
    int32_t bCombatShowMouseHex;

    // offset: +0x6C = +108,  size = 0x4 = 4
    int32_t iCombatGridLevel;

    // offset: +0x70 = +112,  size = 0x1C = 28
    std::array<int32_t, 7> iCombatViewArmy;

    // offset: +0x8C = +140,  size = 0x1 = 1
    bool bDontTryRedbook;

    // offset: +0x8D = +141,  size = 0x1 = 1
    bool bFirstInstall;

    unsigned char : 8;
    unsigned char : 8;

    // offset: +0x90 = +144,  size = 0x4 = 4
    std::array<char, 4> cUniqueSystemID;

    // offset: +0x94 = +148,  size = 0x4 = 4
    int32_t iCombatSpeed;

    // offset: +0x98 = +152,  size = 0xD = 13
    std::array<char, 13> cCurRemoteReceive;

    // offset: +0xA5 = +165,  size = 0xD = 13
    std::array<char, 13> cRemoteReceiveDiff;

    // offset: +0xB2 = +178,  size = 0xD = 13
    std::array<char, 13> cCurRemoteSend;

    // offset: +0xBF = +191,  size = 0x15 = 21
    std::array<char, 21> cNetName;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

inline game* const& gpGame = get_global_var_ref(0x699538, game*);

inline bool OnMySide(int32_t iWhichPlayer)
{
    assert(iWhichPlayer < 8);
    assert(giCurPlayer < 8);
    if ( iWhichPlayer < 0 )
        return false;

    return ( giCurPlayer >= 0 ) ? gpGame->sMapHeader.teamInfo[static_cast<size_t>(giCurPlayer)] == gpGame->sMapHeader.teamInfo[static_cast<size_t>(iWhichPlayer)] : false;
}

// Game config /
// Игровые настройки.
inline configStruct& gConfig = get_global_var_ref(0x6987A8, configStruct);

inline int32_t&  giThisGamePos                = get_global_var_ref(0x6995A4, int32_t);
inline bool32_t& gbThisNetGotAdventureControl = get_global_var_ref(0x6977D8, bool32_t);
inline bool32_t& gbInNewGameSetup             = get_global_var_ref(0x698450, bool32_t);
inline bool32_t& gbInReplay                   = get_global_var_ref(0x696A54, bool32_t);
inline bool32_t& gbGameOver                   = get_global_var_ref(0x697308, bool32_t);
inline bool32_t& gbRemoteOn                   = get_global_var_ref(0x69959C, bool32_t);
inline bool&     bDefeatedAllPlayers          = get_global_var_ref(0x69956C, bool);

// Month creature type /
// Тип существа месяца.
inline int32_t& giMonthTypeExtra = get_global_var_ref(0x697798, int32_t);

// Month type /
// Тип месяца.
inline int32_t& giMonthType = get_global_var_ref(0x698884, int32_t);

// Week creature type /
// Тип существа недели.
inline int32_t& giWeekTypeExtra = get_global_var_ref(0x69844C, int32_t);

// Week type /
// Тип недели.
inline int32_t& giWeekType = get_global_var_ref(0x6977A0, int32_t);

inline exe_bitset<48>& puzzlePiecesRemoved = get_global_var_ref(0x697738, exe_bitset<48>);

// Current game type /
// Текущий тип игры.
inline eNetGameType& iMPNetProtocol = get_global_var_ref(0x698A40, eNetGameType);

NH3API_WARNING(pop)
