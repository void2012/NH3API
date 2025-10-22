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

#include "game_setup.hpp" // SGameSetupOptions
#include "lossvictory.hpp" // VictoryConditionStruct, LossConditionStruct
#include "hero.hpp" // HeroIdentity, HeroPlayerInfo

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)
#if !NH3API_CHECK_MSVC
NH3API_DISABLE_WARNING("-Winvalid-offsetof")
#endif

#pragma pack(push, 4)
// Map header /
// Заголовок карты.
// size = 0x2C0 = 704, align = 4
struct CMapHeaderData
{
public:
    NH3API_FORCEINLINE
    CMapHeaderData() noexcept
        : iVersion(28),
          IsPlayable(true),
          iDifficulty(DIFFICULTY_NORMAL),
          numPlayers(MAX_PLAYERS),
          minNumHumanPlayers(1),
          maxNumHumanPlayers(MAX_PLAYERS),
          lastTownNameAssigned(0),
          mapHasNotBeenSaved(false),
          max_hero_level(-1),
          numTeams(0),
          teamInfo{{0, 0, 0, 0, 0, 0, 0, 0}},
          gap_E(),
          Size(0),
          HasTwoLayers(false),
          gap_1D()
    {
        nh3api::construct_at(&placeholders);
        std::uninitialized_default_construct_n(PlayerSlotAttributes.begin(), PlayerSlotAttributes.size());
    }

    NH3API_FORCEINLINE
    CMapHeaderData(const ::nh3api::dummy_tag_t& tag) noexcept
        : placeholders(tag),
          PlayerSlotAttributes{{tag, tag, tag, tag, tag, tag, tag, tag}}
    {}

    NH3API_FORCEINLINE
    ~CMapHeaderData() noexcept
    {
        std::destroy_at(&placeholders);
        std::destroy(PlayerSlotAttributes.begin(), PlayerSlotAttributes.end());
    }

public:
    // Player attributes /
    // Свойства игрока.
    // size = 0x44 = 68, align = 4
    struct TPlayerSlotAttributes
    {
        public:
            NH3API_FORCEINLINE
            TPlayerSlotAttributes() noexcept
            NH3API_DELEGATE_DUMMY(TPlayerSlotAttributes)
            { THISCALL_1(void, 0x45A630, this); }

            NH3API_FORCEINLINE
            TPlayerSlotAttributes(const ::nh3api::dummy_tag_t& tag) noexcept
                : heroes(tag)
            {}

            NH3API_FORCEINLINE
            ~TPlayerSlotAttributes() noexcept
            { THISCALL_1(void, 0x45A860, this); }

            TPlayerSlotAttributes& operator=(const TPlayerSlotAttributes& other)
            {
                if ( this == &other )
                    return *this;

                nh3api::trivial_copy<52/*__builtin_offsetof(TPlayerSlotAttributes, heroes)*/>(&other, this);

                this->heroes = other.heroes;
                return *this;
            }

            TPlayerSlotAttributes(const TPlayerSlotAttributes& other)
            { *this = other; }
            
            #if NH3API_STD_MOVE_SEMANTICS
            TPlayerSlotAttributes& operator=(TPlayerSlotAttributes&& other) noexcept
            {
                if ( this == &other )
                    return *this;

                nh3api::trivial_move<52/*__builtin_offsetof(TPlayerSlotAttributes, heroes)*/>(&other, this);
                
                this->heroes = std::move(other.heroes);
                return *this;
            } 

            TPlayerSlotAttributes(TPlayerSlotAttributes&& other) noexcept
            { *this = std::move(other); }
            #endif

        public:
            // Can be human /
            // Может быть человеком.
            // offset: +0x0 = +0,  size = 0x1 = 1
            bool CanBeHuman;

            // Can be computer /
            // Может быть компьютерным игроком.
            // offset: +0x1 = +1,  size = 0x1 = 1
            bool CanBeComputer;

        protected:
            [[maybe_unused]]
            byte_t gap_1D[2];

        public:
            // AI strategy type /
            // Тип ИИ игрока.
            // offset: +0x4 = +4,  size = 0x4 = 4
            int32_t AIStrategy;

            // Fractions(alignments) that the player can pick /
            // Фракции, которые может выбрать игрок.
            // offset: +0x8 = +8,  size = 0x2 = 2
            int16_t legal_alignments;

            // Player has random fraction(alignment) /
            // Игрок имеет случайную фракцию.
            // offset: +0xA = +10,  size = 0x1 = 1
            bool HasRandomAlignment;

            // Generate hero in the main town /
            // Генерировать героя в главном городе.
            // offset: +0xB = +11,  size = 0x1 = 1
            bool GenerateHero;

            // Has main town /
            // Есть главный город.
            // offset: +0xC = +12,  size = 0x1 = 1
            bool has_main_town;

        protected:
            [[maybe_unused]]
            byte_t gap_D[3];

        public:
            // Main town type /
            // Тип главного города.
            // offset: +0x10 = +16,  size = 0x4 = 4
            TTownType main_town_type;

            // Main castle location /
            // Координаты главного города игрока.
            // offset: +0x14 = +20,  size = 0x4 = 4
            type_point CastleLoc;

            // Has random hero /
            // Есть случайный герой.
            // offset: +0x18 = +24,  size = 0x1 = 1
            bool hasRandomHero;

        protected:
            [[maybe_unused]]
            byte_t gap_19[3];

        public:
            // Main hero ID /
            // ID главного героя.
            // offset: +0x1C = +28,  size = 0x4 = 4
            THeroID nonRandomHeroId;

            // Main hero portrait /
            // Портрет главного героя.
            // offset: +0x20 = +32,  size = 0x4 = 4
            int32_t nonRandomHeroCustomPortrait;

            // Main hero name /
            // Имя главного героя.
            // offset: +0x24 = +36,  size = 0xC = 12
            std::array<char, 12> nonRandomHeroCustomName;

            // offset: +0x30 = +48,  size = 0x4 = 4
            int32_t default_placeholders;

            union {
            // offset: +0x34 = +52,  size = 0x10 = 16
            exe_vector<HeroIdentity> heroes;
            };
    };

public:
    // Map version /
    // Версия карты.
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t iVersion;

    // Map has no map editor problems /
    // Карта не содержит ошибок редактора.
    // offset: +0x4 = +4,  size = 0x1 = 1
    bool IsPlayable;

    // Map difficulty /
    // Сложность карты.
    // offset: +0x5 = +5,  size = 0x1 = 1
    EGameDifficulty iDifficulty;

    // Number of players /
    // Кол-во игроков.
    // offset: +0x6 = +6,  size = 0x1 = 1
    int8_t numPlayers;

    // Minimal number of human players /
    // Минимальное кол-во игроков-людей.
    // offset: +0x7 = +7,  size = 0x1 = 1
    int8_t minNumHumanPlayers;

    // Maximum number of human players /
    // Максимальное кол-во игроков-людей.
    // offset: +0x8 = +8,  size = 0x1 = 1
    int8_t maxNumHumanPlayers;

protected:
    [[maybe_unused]]
    // offset: +0x9 = +9,  size = 0x1 = 1
    int8_t lastTownNameAssigned;

    [[maybe_unused]]
    // offset: +0xA = +10,  size = 0x1 = 1
    bool mapHasNotBeenSaved;

public:
    // Maximum hero level /
    // Предельный уровень героя.
    // offset: +0xB = +11,  size = 0x1 = 1
    int8_t max_hero_level;

    // Number of player teams /
    // Кол-во команд игроков.
    // offset: +0xC = +12,  size = 0x1 = 1
    int8_t numTeams;

    // Player teams /
    // Команды игроков.
    // offset: +0xD = +13,  size = 0x8 = 8
    std::array<int8_t, 8> teamInfo;

protected:
    [[maybe_unused]]
    byte_t gap_E[3];

public:
    // Map size(width and height are the same) /
    // Размер карты(ширина и высота совпадают).
    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t Size;

    // Has two layers /
    // Есть два уровня.
    // offset: +0x1C = +28,  size = 0x1 = 1
    bool HasTwoLayers;

protected:
    [[maybe_unused]]
    byte_t gap_1D[3];

public:
    union {
    // Hero placeholders /
    // Герои, переходящие в следующий сценарий кампании.
    // offset: +0x20 = +32,  size = 0x10 = 16
    exe_vector<int32_t> placeholders;
    };

    // Victory Condition /
    // Условие победы.
    // offset: +0x30 = +48,  size = 0x4C = 76
    VictoryConditionStruct victory_condition;
    // ^^^ trivial ^^^

    // Loss condition /
    // Условие поражения.
    // offset: +0x7C = +124,  size = 0x24 = 36
    LossConditionStruct loss_condition;
    // ^^^ trivial ^^^

    union {
    // Players attributes /
    // Свойства игроков.
    // offset: +0xA0 = +160,  size = 0x220 = 544
    std::array<TPlayerSlotAttributes, MAX_PLAYERS> PlayerSlotAttributes;
    };

};
#pragma pack(pop)

#pragma pack(push, 4)
// Map information /
// Информация о карте.
// size = 0x304 = 772, align = 4, baseclass: CMapHeaderData
struct NewSMapHeader : CMapHeaderData
{
    public:
        NH3API_FORCEINLINE
        NewSMapHeader() noexcept
        NH3API_DELEGATE_DUMMY(NewSMapHeader)
        { THISCALL_1(void, 0x45A670, this); }

        NH3API_FORCEINLINE
        NewSMapHeader(const ::nh3api::dummy_tag_t& tag) noexcept
            : heroPlayerSetups(tag),
              mapName(tag),
              mapDescription(tag),
              availableHeroes(tag)
        {}

        NH3API_FORCEINLINE
        ~NewSMapHeader() noexcept
        { THISCALL_1(void, 0x45A9E0, this); }

        NH3API_FORCEINLINE
        NewSMapHeader& operator=(const NewSMapHeader& other) noexcept
        { THISCALL_2(void, 0x457990, this, &other); return *this; }

    public:
        union {
        // Hero global setups /
        // Глобальные настройки героев.
        // offset: +0x2C0 = +704,  size = 0x10 = 16
        exe_map<int32_t, HeroPlayerInfo, 0x694E44, 0x694E40> heroPlayerSetups;
        };

        union {
        // Map name /
        // Название карты.
        // offset: +0x2D0 = +720,  size = 0x10 = 16
        exe_string mapName;
        };

        union {
        // Map Description /
        // Описание карты.
        // offset: +0x2E0 = +736,  size = 0x10 = 16
        exe_string mapDescription;
        };

        union {
        // Available heroes /
        // Доступные герои.
        // offset: +0x2F0 = +752,  size = 0x14 = 20
        exe_bitset<MAX_HEROES> availableHeroes;
        };

};
#pragma pack(pop)

#if !NH3API_CHECK_MSVC
NH3API_DISABLE_WARNING_END
#endif
NH3API_DISABLE_WARNING_END

