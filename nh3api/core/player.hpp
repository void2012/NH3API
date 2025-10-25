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

#include "nh3api_std/exe_vector.hpp" // exe_vector<T>
#include "hero_enums.hpp" // THeroID
#include "artifact.hpp" // TArtifact

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// Текущий игрок.
inline int32_t& giCurPlayer = get_global_var_ref(0x69CCF4, int32_t);

inline int32_t& giCurWatchPlayer = get_global_var_ref(0x6977DC, int32_t);
inline uint8_t& giCurWatchPlayerBit = get_global_var_ref(0x69CD08, uint8_t);

// Текущий бит игрока.
inline uint8_t& giCurrentPlayerBit = get_global_var_ref(0x69CD10, uint8_t);

#pragma pack(push, 1)
// AI player resoruces info /
// Информация о ресурсах ИИ игрока.
// size = 0x7C = 124, align = 1
struct AI
{
    // offset: +0x0 = +0,  size = 0x1C = 28
    std::array<int32_t, 7> turnExpectedResource;

    // offset: +0x1C = +28,  size = 0x1C = 28
    std::array<int32_t, 7> turnProductionResource;

protected:
    [[maybe_unused]]
    std::byte gap_38[4];

public:
    // offset: +0x3C = +60,  size = 0x38 = 56
    std::array<double, 7> resource_value;

    // offset: +0x74 = +116,  size = 0x4 = 4
    int32_t average_resource_value;

    // offset: +0x78 = +120,  size = 0x4 = 4
    float turnValueOfAvgArtifact;

};
#pragma pack(pop)

class TAbstractFile;
class hero;
class town;
#pragma pack(push, 1)
// Player /
// Игрок.
// size = 0x168 = 360, align = 1
class playerData
{
    public:
        NH3API_FORCEINLINE
        playerData() noexcept
        NH3API_DELEGATE_DUMMY(playerData)
        { THISCALL_1(void, 0x4B9AB0, this); }

        NH3API_FORCEINLINE
        playerData(const ::nh3api::dummy_tag_t& tag) noexcept
            : shipyards(tag),
              assembledCombos(tag)
        {}

        NH3API_FORCEINLINE
        playerData& operator=(const playerData& other) noexcept
        { THISCALL_2(void, 0x58FA40, this, other); return *this; }

        NH3API_FORCEINLINE
        playerData(const playerData& other) noexcept
        { THISCALL_2(void, 0x58FA40, this, &other); }

        NH3API_FORCEINLINE
        ~playerData() noexcept
        { THISCALL_1(void, 0x4CE230, this); }

    public:
        // Does the player have capitol? /
        // Есть ли у игрока город с капитолием?
        [[nodiscard]] bool HasCapitol() const
        { return THISCALL_1(bool, 0x4B9C00, this); }

        // Move occupying hero to town's garrison /
        // Переместить героя-гостя в гарнизон города.
        /// @param our_town Город игрока.
        /// @return Successful move / Удалось переместить героя
        bool add_garrison_hero(town* our_town)
        { return THISCALL_2(bool, 0x4B9C80, this, our_town); }

        // Change player name [20] /
        // Изменить имя игрока [20].
        /// @param cNewName Новое имя.
        void SetName(char * cNewName)
        { THISCALL_2(void, 0x4B9DF0, this, cNewName); }

        void ClearNetInfo()
        { THISCALL_1(void, 0x4B9E30, this); }

        int32_t load(TAbstractFile* infile, int32_t version)
        { return THISCALL_3(int32_t, 0x4B9F20, this, infile, version); }

        int32_t save(TAbstractFile* outfile)
        { return THISCALL_2(int32_t, 0x4BA330, this, outfile); }

        [[nodiscard]] int32_t FindHero(int32_t id) const
        { return THISCALL_2(int32_t, 0x4BA6A0, this, id); }

        [[nodiscard]] int32_t FindTown(int32_t id) const
        { return THISCALL_2(int32_t, 0x4BA6D0, this, id); }

        [[nodiscard]] int32_t NextHero() const
        { return THISCALL_1(THeroID, 0x4BA700, this); }

        [[nodiscard]] int32_t NextTown() const
        { return THISCALL_1(int32_t, 0x4BA800, this); }

        [[nodiscard]] bool HasMobileHero() const
        { return THISCALL_1(bool, 0x4BA850, this); }

        [[nodiscard]] int32_t NumOfGivenArtifact(TArtifact iWhichArtifact) const
        { return THISCALL_2(int32_t, 0x4BA890, this, iWhichArtifact); }

        [[nodiscard]] bool hasGivenArtifact(TArtifact artifact) const
        { return THISCALL_2(bool, 0x4BA970, this, artifact); }

        [[nodiscard]] bool IsLocalHuman() const
        { return THISCALL_1(bool, 0x4BAA40, this); }

        [[nodiscard]] bool IsHuman() const
        { return THISCALL_1(bool, 0x4BAA60, this); }

        [[nodiscard]] const char* GetName() const
        { return THISCALL_1(char*, 0x4BAA70, this); }

        [[nodiscard]] bool hasAssembledCombos() const
        { return assembledCombos.any(); }

        [[nodiscard]] uint32_t numAssembledCombos() const
        { return assembledCombos.count(); }

    public:
        // Player color /
        // Цвет игрока.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t color;

        // Number of player active heroes /
        // Кол-во активных героев у игрока.
        // offset: +0x1 = +1,  size = 0x1 = 1
        int8_t numHeroes;

    protected:
        [[maybe_unused]]
        std::byte gap_2[2];

    public:
        // Current choosen hero /
        // Текущий выбранный герой.
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t currHero;

        // Player heroes /
        // Герои игрока.
        // offset: +0x8 = +8,  size = 0x20 = 32
        std::array<THeroID, 8> heroes;

        // Tavern heroes /
        // Герои таверны.
        // offset: +0x28 = +40,  size = 0x8 = 8
        std::array<THeroID, 2> recruits;

    protected:
        [[maybe_unused]]
        // unused / не используется
        // offset: +0x30 = +48,  size = 0x1 = 1
        uint8_t startingNumHeroes;

        [[maybe_unused]]
        std::byte gap_49[3];

    public:
        // AI player personality /
        // Темперамент игрока-ИИ.
        // offset: +0x34 = +52,  size = 0x4 = 4
        int32_t personality;

        // Number of
        // Число открытых пазлов на карте-загадке.
        // offset: +0x38 = +56,  size = 0x1 = 1
        int8_t extraPuzzlePieces;

        // AI guessed Grail position /
        // ИИ: предполагаемая клетка Грааля.
        // offset: +0x39 = +57,  size = 0x4 = 4
        type_point puzzle_guess;

        // Amount of days player left without towns /
        // Кол-во дней, которые остались до смерти игрока без городов.
        // offset: +0x3D = +61,  size = 0x1 = 1
        int8_t iDeathCountDown;

        // Amount of player towns /
        // Кол-во городов игрока.
        // offset: +0x3E = +62,  size = 0x1 = 1
        int8_t numTowns;

        // Current choosen town /
        // Текущий выбранный город
        // offset: +0x3F = +63,  size = 0x1 = 1
        int8_t currTown;

        // Player towns /
        // Города игрока.
        // offset: +0x40 = +64,  size = 0x48 = 72
        std::array<int8_t, 72> towns;

        // Show help at the beginning of the tactics phase in combat /
        // Показывать информацию о тактической фазе в начале боя.
        // offset: +0x88 = +136,  size = 0x1 = 1
        bool placement_help_enabled;

    protected:
        [[maybe_unused]]
        std::byte gap_89[3];

    public:
        union {
        // Player shipyards /
        // Верфи игрока.
        // offset: +0x8C = +140,  size = 0x10 = 16
        exe_vector<type_point> shipyards;
        };

        // Player resources /
        // Ресурсы игрока.
        // offset: +0x9C = +156,  size = 0x1C = 28
        std::array<int32_t, 7> resources;

        // Visited mystical gardens /
        // Посещённые магические сады.
        // offset: +0xB8 = +184,  size = 0x4 = 4
        uint32_t MysticalGardenFlags;

        // Visited magic springs /
        // Посещённые магические источники.
        // offset: +0xBC = +188,  size = 0x4 = 4
        uint32_t MagicSpringFlags;

        // Visited corpses /
        // Посещённые скелеты.
        // offset: +0xC0 = +192,  size = 0x4 = 4
        uint32_t DeadGuyFlags;

        // Visited lean-tos /
        // Посещённые навесы.
        // offset: +0xC4 = +196,  size = 0x4 = 4
        uint32_t LeanToFlags;

        // Network player id /
        // Идентификатор игрока при игре по сети.
        // offset: +0xC8 = +200,  size = 0x4 = 4
        uint32_t dpid;

        // Player name /
        // Имя игрока.
        // offset: +0xCC = +204,  size = 0x15 = 21
        std::array<char, 21> cName;

        // Player is local to this machine /
        // Игрок является локальным при игре по сети(текущая машина).
        // offset: +0xE1 = +225,  size = 0x1 = 1
        bool isLocal;

        // Player is human /
        // Игрок является человеком.
        // offset: +0xE2 = +226,  size = 0x1 = 1
        bool isHuman;

    protected:
        [[maybe_unused]]
        std::byte gap_E3[1];

    public:
        // Quick combat is on /
        // Включен режим быстрого боя.
        // offset: +0xE4 = +228,  size = 0x4 = 4
        bool32_t quickCombat;

        union {
        // Combo artifacts assembled by the player /
        // Собранные игроком сборные артефакты.
        // offset: +0xE8 = +232,  size = 0x4 = 4
        exe_bitset<MAX_COMBO_ARTIFACTS> assembledCombos;
        };

        // AI player resoruces info /
        // ИИ: Информация о ресурсах.
        // offset: +0xEC = +236,  size = 0x7C = 124
        AI ai;
};
#pragma pack(pop)

// Текущий игрок.
inline playerData* const& gpCurPlayer = get_global_var_ref(0x69CCFC, playerData*);

NH3API_SIZE_ASSERT(0x168, playerData);

NH3API_FORCEINLINE int32_t GetNumObelisks(int32_t whichPlayer)
{ return FASTCALL_1(int32_t, 0x4BA860, whichPlayer); }

NH3API_DISABLE_WARNING_END
