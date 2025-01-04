//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "game_setup.hpp" // SGameSetupOptions
#include "lossvictory.hpp" // VictoryConditionStruct, LossConditionStruct
#include "hero.hpp" // HeroIdentity, HeroPlayerInfo

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#pragma pack(push, 4)
// Map header /
// Заголовок карты.
// size = 0x2C0 = 704, align = 4
struct CMapHeaderData
{
public:
    // Player attributes /
    // Свойства игрока.
    // size = 0x44 = 68, align = 4
    struct TPlayerSlotAttributes
    {
        public:
            NH3API_FORCEINLINE
            TPlayerSlotAttributes() NH3API_NOEXCEPT
            NH3API_DELEGATE_DUMMY(TPlayerSlotAttributes)
            { THISCALL_1(void, 0x45A630, this); }

            NH3API_FORCEINLINE
            TPlayerSlotAttributes(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
                : heroes(tag)
            {}

            NH3API_FORCEINLINE
            ~TPlayerSlotAttributes() NH3API_NOEXCEPT
            { THISCALL_1(void, 0x45A860, this); }

        public:
            // Can be human /
            // Может быть человеком.
            // offset: +0x0 = +0,  size = 0x1 = 1
            bool CanBeHuman;

            // Can be computer /
            // Может быть компьютерным игроком.
            // offset: +0x1 = +1,  size = 0x1 = 1
            bool CanBeComputer;

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

            // offset: +0x34 = +52,  size = 0x10 = 16
            exe_vector<HeroIdentity> heroes;

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
    bool minNumHumanPlayers;

    // Maximum number of human players /
    // Максимальное кол-во игроков-людей.
    // offset: +0x8 = +8,  size = 0x1 = 1
    bool maxNumHumanPlayers;

protected:
    // offset: +0x9 = +9,  size = 0x1 = 1
    bool lastTownNameAssigned;

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

    // Map size(width and height are the same) /
    // Размер карты(ширина и высота совпадают).
    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t Size;

    // Has two layers /
    // Есть два уровня.
    // offset: +0x1C = +28,  size = 0x1 = 1
    bool HasTwoLayers;

    // Hero placeholders /
    // Герои, переходящие в следующий сценарий кампании.
    // offset: +0x20 = +32,  size = 0x10 = 16
    exe_vector<int32_t> placeholders;

    // Victory Condition /
    // Условие победы.
    // offset: +0x30 = +48,  size = 0x4C = 76
    VictoryConditionStruct victory_condition;

    // Loss condition /
    // Условие поражения.
    // offset: +0x7C = +124,  size = 0x24 = 36
    LossConditionStruct loss_condition;

    // Players attributes /
    // Свойства игроков.
    // offset: +0xA0 = +160,  size = 0x220 = 544
    std::array<TPlayerSlotAttributes, 8> PlayerSlotAttributes;

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
        NewSMapHeader() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(NewSMapHeader)
        { THISCALL_1(void, 0x45A670, this); }

        NH3API_FORCEINLINE
        NewSMapHeader(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : heroPlayerSetups(tag),
              mapName(tag),
              mapDescription(tag),
              availableHeroes(tag)
        {}

        NH3API_FORCEINLINE
        ~NewSMapHeader() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x45A9E0, this); }

        NH3API_FORCEINLINE
        NewSMapHeader& operator=(const NewSMapHeader& other) NH3API_NOEXCEPT
        { return *THISCALL_2(NewSMapHeader*, 0x457990, this, &other); }

    public:
        // Hero global setups /
        // Глобальные настройки героев.
        // offset: +0x2C0 = +704,  size = 0x10 = 16
        exe_map<int32_t, HeroPlayerInfo, 0x694E44, 0x694E40> heroPlayerSetups;

        // Map name /
        // Название карты.
        // offset: +0x2D0 = +720,  size = 0x10 = 16
        exe_string mapName;

        // Map Description /
        // Описание карты.
        // offset: +0x2E0 = +736,  size = 0x10 = 16
        exe_string mapDescription;

        // Available heroes /
        // Доступные герои.
        // offset: +0x2F0 = +752,  size = 0x14 = 20
        exe_bitset<MAX_HEROES> availableHeroes;

};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END
