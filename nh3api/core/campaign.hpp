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

#include <array>                 // std::array

#include "interface/widgets.hpp" // widgets
#include "artifact.hpp"          // type_artifact
#include "creatures.hpp"         // TCreatureType
#include "map_header.hpp"        // NewSMapHeader

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// Campaign type /
// Тип кампании.
enum ECampaignType : int32_t
{
    ROE_LONG_LIVE_THE_QUEEN     = 0,
    ROE_LIBERATION              = 1,
    ROE_SPOILS_OF_WAR           = 2,
    ROE_SONG_FOR_THE_FATHER     = 3,
    ROE_DUNGEONS_AND_DEVILS     = 4,
    ROE_LONG_LIVE_THE_KING      = 5,
    ROE_SEEDS_OF_DISCONTENT     = 6,

    AB_ARMAGEDDONS_BLADE        = 7,
    AB_DRAGONS_BLOOD            = 8,
    AB_DRAGON_SLAYER            = 9,
    AB_FESTIVAL_OF_LIFE         = 10,
    AB_FOOLHARDY_WAYWARDNESS    = 11,
    AB_PLAYING_WITH_FIRE        = 12,

    SOD_HACK_AND_SLASH          = 13,
    SOD_BIRTH_OF_A_BARBARIAN    = 14,
    SOD_NEW_BEGINNING           = 15,
    SOD_ELIXIR_OF_LIFE          = 16,
    SOD_RISE_OF_THE_NECROMANCER = 17,
    SOD_UNHOLY_ALLIANCE         = 18,
    SOD_SPECTRE_OF_POWER        = 19,

    CUSTOM_CAMPAIGN             = 20
};

template<>
struct nh3api::enum_limits<ECampaignType>
    : nh3api::enum_limits_base<ECampaignType, ROE_LONG_LIVE_THE_QUEEN, SOD_SPECTRE_OF_POWER>
{ static inline constexpr bool is_specialized = true; };

#pragma pack(push, 4)
// Campaign scenario info in campaign scenario dialog /
// Информация о сценарии в диалоге кампании.
// size = 0x4D4 = 1236, align = 4, baseclass: NewSMapHeader
struct CampaignScenarioPreview : public NewSMapHeader
{
    public:
        CampaignScenarioPreview()
            : NewSMapHeader()
        {}

        CampaignScenarioPreview(const CampaignScenarioPreview&)            = delete;
        CampaignScenarioPreview(CampaignScenarioPreview&&)                 = delete;
        CampaignScenarioPreview& operator=(const CampaignScenarioPreview&) = delete;
        CampaignScenarioPreview& operator=(CampaignScenarioPreview&&)      = delete;
        inline ~CampaignScenarioPreview() noexcept                         = default;

    public:
        // Game setup options /
        // Начальные условия игры.
        // offset: +0x304 = +772,  size = 0x1CC = 460
        SGameSetupOptions game_setup;
        // ^^^ trivial ^^^

        // Scenario is available /
        // Сценарий доступен.
        // offset: +0x4D0 = +1232,  size = 0x1 = 1
        bool available;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;

NH3API_SIZE_ASSERT(0x4D4, CampaignScenarioPreview);

// Campaign scenario dialog /
// Диалог кампании.
// size = 0xB4 = 180, align = 4, baseclass: heroWindow
NH3API_VIRTUAL_CLASS TCampaignBrief : public heroWindow
{
    public:
        inline TCampaignBrief(bool newCampaign, bool bViewFromGame)
        noexcept(false) // TGzInflateBuf may throw TGzInflateBuf::TDataError, std::filebuf::_Initcvt may throw std::bad_cast
            : TCampaignBrief(nh3api::dummy_tag)
        { THISCALL_3(void, 0x458DA0, this, newCampaign, static_cast<bool32_t>(bViewFromGame)); }

        inline TCampaignBrief(const nh3api::dummy_tag_t& tag) noexcept
            : heroWindow(tag),
              scenarios { tag }
        {}

        TCampaignBrief(const TCampaignBrief&)            = delete;
        TCampaignBrief& operator=(const TCampaignBrief&) = delete;
        TCampaignBrief(TCampaignBrief&&)                 = delete;
        TCampaignBrief& operator=(TCampaignBrief&&)      = delete;

        inline ~TCampaignBrief() noexcept
        { THISCALL_1(void, 0x45AC90, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_HEROWINDOW(TCampaignBrief)

    public:
        // Scenario video information /
        // Информация о видеоролике сценария.
        // size = 0x18 = 24, align = 4
        struct MapTextStruct
        {
            public:
                // Video ID /
                // ID видеоролика.
                // offset: +0x0 = +0,  size = 0x4 = 4
                int32_t video;

                // Audio ID /
                // ID аудиофайла.
                // offset: +0x4 = +4,  size = 0x4 = 4
                int32_t audio;

                // Subtitles /
                // Субтитры.
                // offset: +0x8 = +8,  size = 0x10 = 16
                exe_string subtitles;
        };

        // Scenario information /
        // Информация о сценарии.
        // size = 0xA8 = 168, align = 4
        struct ScenarioStruct
        {
            // Scenario name /
            // Название сценария.
            // offset: +0x0 = +0,  size = 0x10 = 16
            exe_string name;

            // Scenario file offset in the campaign file /
            // Смещение файла сценария в файле кампании.
            // offset: +0x10 = +16,  size = 0x4 = 4
            ptrdiff_t offset;

            // Compressed scenario file size /
            // Сжатый размер сценария.
            // offset: +0x14 = +20,  size = 0x4 = 4
            size_t inflated_size;

            // Scenarios to be completed to make this scenario available /
            // Сценарии, необходимые для открытия данного сценария.
            // offset: +0x18 = +24,  size = 0x10 = 16
            exe_vector<bool> prerequisites;

            // Scenario region description /
            // Описание региона сценария.
            // offset: +0x28 = +40,  size = 0x10 = 16
            exe_string region_desc;

            // Scenario region color /
            // Цвет региона сценария.
            // offset: +0x38 = +56,  size = 0x1 = 1
            uint8_t region_color;

            // Scenario difficulty /
            // Сложность сценария.
            // offset: +0x39 = +57,  size = 0x1 = 1
            EGameDifficulty difficulty;

            unsigned char : 8;
            unsigned char : 8;

            // Scenario prologue /
            // Пролог сценария.
            // offset: +0x3C = +60,  size = 0x4 = 4
            MapTextStruct* prologue;

            // Scenario epilogue /
            // Эпилог сценария.
            // offset: +0x40 = +64,  size = 0x4 = 4
            MapTextStruct* epilogue;

            // Crossover heroes retain experience /
            // Опыт героя переходит в следующие сценарии.
            // offset: +0x44 = +68,  size = 0x1 = 1
            bool retain_xp;

            // Crossover heroes retain primary skills /
            // Первичные навыки героя переходят в следующие сценарии.
            // offset: +0x45 = +69,  size = 0x1 = 1
            bool retain_pskills;

            // Crossover heroes retain secondary skills /
            // Вторичные навыки героя переходят в следующие сценарии.
            // offset: +0x46 = +70,  size = 0x1 = 1
            bool retain_sskills;

            // Crossover heroes retain spellbooks /
            // Книга заклинаний героя переходит в следующие сценарии.
            // offset: +0x47 = +71,  size = 0x1 = 1
            bool retain_spellbook;

            // Crossover heroes retain artifacts /
            // Артефакты героя переходят в следующие сценарии.
            // offset: +0x48 = +72,  size = 0x1 = 1
            bool retain_artifacts;

            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;

            // offset: +0x4C = +76,  size = 0x20 = 32
            std::array<int32_t, 8> heroes_status;

            // Hero placeholders /
            // Переходящие герои сценария.
            // offset: +0x6C = +108,  size = 0x10 = 16
            exe_vector<int32_t> hero_placeholders;

            // Crossover creatures /
            // Существа, переходящие в следующие сценарии.
            // offset: +0x7C = +124,  size = 0x14 = 20
            exe_bitset<MAX_CREATURES> crossover_creatures;

            // Crossover artifacts /
            // Артефакты, переходящие в следующие сценарии.
            // offset: +0x90 = +144,  size = 0x14 = 20
            exe_bitset<MAX_ARTIFACTS> crossover_artifacts;

            // Scenario start options /
            // Начальные опции сценария.
            // offset: +0xA4 = +164,  size = 0x4 = 4
            void* options;
            // t_scenario_start_options* options;

        } NH3API_MSVC_LAYOUT;

        // size = 0x5C = 92, align = 4
        struct CampaignHeaderStruct
        {
            public:
                inline CampaignHeaderStruct()
                    noexcept(false) // TGzInflateBuf may throw TGzInflateBuf::TDataError, std::filebuf::_Initcvt may throw std::bad_cast
                    : CampaignHeaderStruct(nh3api::dummy_tag)
                { THISCALL_1(void, 0x4883C0, this); }

                inline CampaignHeaderStruct(const nh3api::dummy_tag_t& tag) noexcept
                    : file_name { tag },
                      campaign_name { tag },
                      campaign_desc { tag },
                      scenarios { tag }
                {}

                inline CampaignHeaderStruct(const CampaignHeaderStruct& other)
                    : file_error { other.file_error },
                      file_name { other.file_name },
                      campaign_version { other.campaign_version },
                      region_map { other.region_map },
                      campaign_name { other.campaign_name },
                      campaign_desc { other.campaign_desc },
                      scenarios { other.scenarios },
                      data { other.data },
                      stream { other.stream },
                      variable_difficulty { other.variable_difficulty },
                      campaign_music { other.campaign_music }
                {}

                inline CampaignHeaderStruct& operator=(const CampaignHeaderStruct& other)
                {
                    if ( this != &other )
                    {
                        file_error          = other.file_error;
                        file_name           = other.file_name;
                        campaign_version    = other.campaign_version;
                        region_map          = other.region_map;
                        campaign_name       = other.campaign_name;
                        campaign_desc       = other.campaign_desc;
                        scenarios           = other.scenarios;
                        data                = other.data;
                        stream              = other.stream;
                        variable_difficulty = other.variable_difficulty;
                        campaign_music      = other.campaign_music;
                    }

                    return *this;
                }

                inline CampaignHeaderStruct(CampaignHeaderStruct&& other) noexcept
                {
                    std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                    std::memset(static_cast<void*>(&other), 0, sizeof(*this));
                }

                inline CampaignHeaderStruct& operator=(CampaignHeaderStruct&& other) noexcept
                {
                    if ( this != &other )
                    {
                        std::destroy_at(&this->file_name);
                        std::destroy_at(&this->campaign_name);
                        std::destroy_at(&this->campaign_desc);
                        std::destroy_at(&this->scenarios);
                        std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                        std::memset(static_cast<void*>(&other), 0, sizeof(*this));
                    }
                    return *this;
                }

                inline ~CampaignHeaderStruct() noexcept
                { THISCALL_1(void, 0x4881E0, this); }

            public:
                // offset: +0x0 = +0,  size = 0x4 = 4
                int32_t file_error;

                union {
                // offset: +0x4 = +4,  size = 0x10 = 16
                exe_string file_name;
                };

                // offset: +0x14 = +20,  size = 0x4 = 4
                int32_t campaign_version;

                // offset: +0x18 = +24,  size = 0x4 = 4
                int32_t region_map;

                union {
                // offset: +0x1C = +28,  size = 0x10 = 16
                exe_string campaign_name;
                };

                union {
                // offset: +0x2C = +44,  size = 0x10 = 16
                exe_string campaign_desc;
                };

                union {
                // offset: +0x3C = +60,  size = 0x10 = 16
                exe_vector<ScenarioStruct*> scenarios;
                };

                // offset: +0x4C = +76,  size = 0x4 = 4
                void* data;

                // offset: +0x50 = +80,  size = 0x4 = 4
                exe_streambuf* stream;

                // offset: +0x54 = +84,  size = 0x1 = 1
                bool variable_difficulty;

                unsigned char : 8;
                unsigned char : 8;
                unsigned char : 8;

                // offset: +0x58 = +88,  size = 0x4 = 4
                int32_t campaign_music;
        } NH3API_MSVC_LAYOUT;

    public:
        // offset: +0x4C = +76,  size = 0x4 = 4
        uint16_t* zBuffer;

        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t oldVolume;

        union {
        // Campaign scenarios /
        // Сценарии кампании.
        // offset: +0x54 = +84,  size = 0x10 = 16
        exe_vector<CampaignScenarioPreview> scenarios;
        };

        // Campaign information /
        // Информация о кампании.
        // offset: +0x64 = +100,  size = 0x4 = 4
        CampaignHeaderStruct* campaign;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Selected scenario ID /
        // ID выбранного сценария.
        // offset: +0x6C = +108,  size = 0x4 = 4
        int32_t selected_scenario;

        // offset: +0x70 = +112,  size = 0xC = 12
        std::array<coloredBorderFrame*, 3> start_bonus_borders;

        // offset: +0x7C = +124,  size = 0xC = 12
        std::array<bitmapBorder*, 3> bitmap_bonus_images;

        // offset: +0x88 = +136,  size = 0xC = 12
        std::array<iconWidget*, 3> sprite_bonus_images;

        // offset: +0x94 = +148,  size = 0x14 = 20
        std::array<button*, 5> difficulty_buttons;

        // offset: +0xA8 = +168,  size = 0x4 = 4
        type_func_button* difficulty_decr_button;

        // offset: +0xAC = +172,  size = 0x4 = 4
        type_func_button* difficulty_incr_button;

        // offset: +0xB0 = +176,  size = 0x4 = 4
        type_text_scroller* scroller;

} NH3API_MSVC_LAYOUT;

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BC2C, TCampaignBrief)

// Campaign scenario progress information /
// Информация о ходе прохождения сценарии кампании.
// size = 0x14 = 20, align = 4
struct CampaignScenarioInfo
{
    // Scenario completed /
    // Сценарий завершён.
    // offset: +0x0 = +0,  size = 0x1 = 1
    bool completed;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Days completed /
    // Кол-во дней, за которые пройден сценарий.
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t days;

    // Score /
    // Очки.
    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t score;

    // Scenario index /
    // Индекс сценария.
    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t index;

    // Completion order /
    // Порядок прохождения.
    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t complete_order;

} NH3API_MSVC_LAYOUT;

// Campaign progress information /
// Информация о ходе прохождения кампании.
// size = 0x7C = 124, align = 4
struct SCampaign
{
    public:
        inline SCampaign() noexcept
            : SCampaign(nh3api::dummy_tag)
        { THISCALL_1(void, 0x489040, this); }

        inline SCampaign(const nh3api::dummy_tag_t& tag) noexcept
            : CampaignFilename { tag },
              carryover_pool { tag },
              carryover_artifact { tag },
              scenarios { tag },
              assigned_carryover { tag }
        {}

        SCampaign(const SCampaign&)                = default;
        SCampaign& operator=(const SCampaign&)     = default;
        SCampaign(SCampaign&&) noexcept            = default;
        SCampaign& operator=(SCampaign&&) noexcept = default;

        // we don't wrap non-trivial members in union because there is no destructor
        // available in the game as a standalone function, as it was inlined inside of game::game()
        inline ~SCampaign() noexcept = default;

    public:
        [[nodiscard]] bool CampaignComplete() const
        { return THISCALL_1(bool, 0x489310, this); }

        CampaignScenarioInfo* GetCurrentScenario()
        { return THISCALL_1(CampaignScenarioInfo*, 0x57CB00, this); }

        [[nodiscard]] CampaignScenarioInfo* GetCurrentScenario() const
        { return THISCALL_1(CampaignScenarioInfo*, 0x57CB00, this); }

    public:
        // Player is cheater /
        // Игрок вводил чит-коды.
        // offset: +0x0 = +0,  size = 0x1 = 1
        bool bIsCheater;

        // Score over 350 /
        // Набрано более 350 очков.
        // offset: +0x1 = +1,  size = 0x1 = 1
        bool bSecretActive;

        // Current campaign scenario index /
        // Текущий сценарий кампании.
        // offset: +0x2 = +2,  size = 0x1 = 1
        int8_t iCurMap;

        unsigned char : 8;

        // Current campaign /
        // Текущая кампания.
        // offset: +0x4 = +4,  size = 0x4 = 4
        ECampaignType iCurrentCampaign;

    protected:
        [[maybe_unused]]
        // Number of campaign regions(unused) /
        // Кол-во регионов кампании(не используется).
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t NumMapRegions;

    public:
        // offset: +0xC = +12,  size = 0x1 = 1
        int8_t iCrossoverArrayIndex;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Scenario chosen bonus /
        // Выбранный бонус сценария.
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t briefing_choice;

        // Campaign file name /
        // Название файла кампании.
        // offset: +0x14 = +20,  size = 0x10 = 16
        exe_string CampaignFilename;

        // Campaigns completion status /
        // Были ли пройдены те или иные кампании.
        // offset: +0x24 = +36,  size = 0x15 = 21
        std::array<bool, 21> bCampaignCompleted;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Crossover heroes /
        // Переходящие в другой сценарий герои.
        // offset: +0x3C = +60,  size = 0x10 = 16
        exe_vector<exe_vector<hero>> carryover_pool;

        // Crossover artifacts /
        // Переходящие в другой сценарий артефакты.
        // offset: +0x4C = +76,  size = 0x10 = 16
        exe_vector<exe_vector<type_artifact>> carryover_artifact;

        // Campaign scenarios /
        // Сценарии кампании.
        // offset: +0x5C = +92,  size = 0x10 = 16
        exe_vector<CampaignScenarioInfo> scenarios;

    protected:
        [[maybe_unused]]
        // offset: +0x6C = +108,  size = 0x10 = 16
        exe_vector<int32_t> assigned_carryover;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

NH3API_WARNING(pop)
