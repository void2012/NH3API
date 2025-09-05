//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "resources.hpp" // resource, sample
#include "../base_manager.hpp" // baseManager
#include "../terrain.hpp" // TTerrainType

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

// Adventure map object sounds /
// Звуки объектов карты приключений.
enum e_looping_sound_id : int32_t
{
    invalid_sound                   = -1, //
    minotaur_generator_sound        = 0, //
    gorgon_generator_sound          = 0, //
    crossbowman_generator_sound     = 1, //
    lizardman_generator_sound       = 1, //
    arena_sound                     = 2, //
    behemoth_generator_sound        = 3, //
    roc_generator_sound             = 4, //
    buoy_sound                      = 5, //
    campfire_sound                  = 6, //
    beholder_generator_sound        = 7, //
    cyclops_generator_sound         = 7, //
    demon_generator_sound           = 7, //
    troglodyte_generator_sound      = 7, //
    cyclops_bank_sound              = 7, //
    abandoned_mine_sound            = 7, //
    black_knight_generator_sound    = 8, //
    lich_generator_sound            = 8, //
    vampire_generator_sound         = 8, //
    wight_generator_sound           = 8, //
    sepulcher_sound                 = 8, //
    zombie_generator_sound          = 8, //
    devil_generator_sound           = 9, //
    hell_hound_generator_sound      = 10, //
    bone_dragon_generator_sound     = 11, //
    green_dragon_generator_sound    = 11, //
    red_dragon_generator_sound      = 11, //
    dragon_city_sound               = 11, //
    golem_factory_sound             = 12, //
    siege_weapon_factory_sound      = 12, //
    fountain_of_youth_sound         = 13, //
    fire_elemental_generator_sound  = 14, //
    imp_generator_sound             = 14, //
    pit_fiend_generator_sound       = 14, //
    imp_bank_sound                  = 14, //
    pillar_of_fire_sound            = 14, //
    rally_flag_sound                = 15, //
    fountain_of_fortune_sound       = 16, //
    water_elemental_generator_sound = 16, //
    magic_spring_sound              = 16, //
    gem_mine_sound                  = 17, //
    gremlin_generator_sound         = 18, //
    griffin_generator_sound         = 19, //
    gargoyle_generator_sound        = 19, //
    griffin_bank_sound              = 19, //
    harpy_generator_sound           = 20, //
    centaur_generator_sound         = 21, //
    stables_sound                   = 21, //
    cavalier_generator_sound        = 21, //
    hydra_generator_sound           = 22, //
    training_grounds_sound          = 23, //
    dragonfly_generator_sound       = 23, //
    dragonfly_bank_sound            = 23, //
    wood_mine_sound                 = 24, //
    shipyard_sound                  = 24, //
    magic_plains_sound              = 25, //
    genie_generator_sound           = 25, //
    mage_generator_sound            = 25, //
    anti_magic_garrison_sound       = 25, //
    magic_school_sound              = 25, //
    black_market_sound              = 26, //
    trading_post_sound              = 26, //
    merc_camp_sound                 = 27, //
    refugee_camp_sound              = 27, //
    water_wheel_sound               = 28, //
    gold_mine_sound                 = 29, //
    lith_one_way_sound              = 30, //
    lith_two_way_sound              = 31, //
    monk_generator_sound            = 32, //
    basilisk_generator_sound        = 33, //
    wyvern_generator_sound          = 33, //
    orc_generator_sound             = 34, //
    gnoll_generator_sound           = 34, //
    pegasus_generator_sound         = 35, //
    pikeman_generator_sound         = 36, //
    angel_generator_sound           = 37, //
    sanctuary_sound                 = 37, //
    temple_sound                    = 37, //
    shrine_sound                    = 38, //
    star_axis_sound                 = 39, //
    mercury_mine_sound              = 39, //
    sulfur_mine_sound               = 40, //
    ore_mine_sound                  = 40, //
    war_school_sound                = 41, //
    defense_tower_sound             = 41, //
    hill_fort_sound                 = 41, //
    garrison_sound                  = 41, //
    swordsman_generator_sound       = 42, //
    titan_generator_sound           = 43, //
    elemental_conflux_sound         = 43, //
    unicorn_generator_sound         = 44, //
    volcano_sound                   = 45, //
    air_elemental_generator_sound   = 46, //
    crystal_mine_sound              = 47, //
    cursed_ground_sound             = 48, //
    thieves_guild_sound             = 49, //
    dwarf_generator_sound           = 50, //
    dwarf_bank_sound                = 50, //
    earth_elemental_generator_sound = 51, //
    elf_generator_sound             = 52, //
    faerie_ring_sound               = 53, //
    garden_of_revelation_sound      = 54, //
    treefolk_generator_sound        = 54, //
    underground_gate_sound          = 55, //
    goblin_generator_sound          = 56, //
    mystical_garden_sound           = 57, //
    manticore_generator_sound       = 58, //
    medusa_generator_sound          = 59, //
    medusa_bank_sound               = 59, //
    naga_generator_sound            = 60, //
    naga_bank_sound                 = 60, //
    ogre_generator_sound            = 61, //
    siren_sound                     = 62, //
    skeleton_generator_sound        = 63, //
    tavern_sound                    = 64, //
    efreet_generator_sound          = 65, //
    gog_generator_sound             = 65, //
    windmill_sound                  = 66, //
    whirlpool_sound                 = 67, //
    wolf_rider_generator_sound      = 68, //
    shore_sound                     = 69, //
    max_environment_sounds          = 70, //
};

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct soundNode
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    e_looping_sound_id soundID;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t priority;

};
#pragma pack(pop)

typedef void* HSAMPLE;
#pragma pack(push, 4)
// Sound sample info /
// Информация о звуковой дорожке.
// size = 0x18 = 24, align = 4
struct MemorySampleStructure
{
    public:
        // Loaded in memory sample handle /
        // Идентификатор дорожки, загруженной в памяти.
        // offset: +0x0 = +0,  size = 0x4 = 4
        HANDLE memHSample;

        // Sample loaded data /
        // Загруженная информация о звуковой дорожке.
        // offset: +0x4 = +4,  size = 0x4 = 4
        void* data;

        // Sample loaded data size /
        // Размер загруженной информации о звуковой дорожке.
        // offset: +0x8 = +8,  size = 0x4 = 4
        size_t size;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t memCindex;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t memVolume;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t memLooping;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Звуковая дорожка /
// Sound sample.
// size = 0x34 = 52, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT sample : public resource
{
    public:
        sample() NH3API_DELETED_FUNCTION;

        NH3API_FORCEINLINE
        sample(const char* name, 
               const void* src, 
               size_t size, 
               int32_t channel = 0, 
               int32_t volume = 127, 
               bool32_t loop = true) NH3API_NOEXCEPT 
        NH3API_DELEGATE_DUMMY_OR_BASE(sample, resource)
        { THISCALL_7(void, 0x567050, this, name, src, size, channel, volume, loop); }
        
        NH3API_FORCEINLINE
        sample(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag) // resource(nullptr, RType_misc)
        {}

        NH3API_FORCEINLINE
        ~sample() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x567110, this); }

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(sample)

    public:
        // Sample information /
        // Информация о звуковой дорожке.
        // offset: +0x1C = +28,  size = 0x18 = 24
        MemorySampleStructure memSample;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct SAMPLE2
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    sample* resSample;
    // offset: +0x4 = +4,  size = 0x4 = 4
    HSAMPLE playSample;

};
#pragma pack(pop)

inline void ClearMemSample(SAMPLE2 sample2)
{ STDCALL_2(void, 0x59A710, sample2.resSample, sample2.playSample); }

inline SAMPLE2 LoadPlaySample(const char* cSampleName)
{ return FASTCALL_1(SAMPLE2, 0x59A770, cSampleName); }

inline void WaitEndSample(SAMPLE2 smpl, int32_t iMilliWait)
{ FASTCALL_2(void, 0x59A7C0, smpl, iMilliWait); }

inline void launch_sample(const char* sample_name, int32_t max_time = -1, int32_t channel = 3)
{ FASTCALL_3(void, 0x59A890, sample_name, max_time, channel); }

NH3API_INLINE_OR_EXTERN
const std::array<uint8_t, 10>& giTerrainToMusicTrack
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x678330, const std::array<uint8_t, 10>));

NH3API_INLINE_OR_EXTERN
// Sound is disabled /
// Звук в игре отключен.
bool32_t& gbNoSound
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6992E0, bool32_t));

#pragma pack(push, 4)
// Sound & music manager /
// Менеджер звуков и музыки.
// size = 0xD8 = 216, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS soundManager : public baseManager
{
    public:
        struct vftable_t : baseManager::vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(soundManager*, uint8_t);
        };

    public:
        soundManager() NH3API_NOEXCEPT
            : baseManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x599A60, this); }

        NH3API_FORCEINLINE
        soundManager(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : baseManager(tag)
        {}

    public:
        // Stop all samples /
        // Прекратить проигрывание звуков.
        /// @param bStopMusicToo остановить музыку в т.ч.
        void StopAllSamples(bool bStopMusicToo = true)
        { THISCALL_2(void, 0x59A090, this, bStopMusicToo); }

        // Stop certain sample /
        // Прекратить проигрывание определённого звука.
        /// @param smpl звук
        void StopSample(HSAMPLE smpl)
        { THISCALL_2(void, 0x59A180, this, smpl); }

        // Stop certain sample /
        // Прекратить проигрывание определённого звука.
        /// @param smpl звук
        void StopSample(sample* smpl)
        { StopSample(smpl->memSample.memHSample); }

        void WaitSample(const HSAMPLE smpl, int32_t time = -1)
        { THISCALL_3(void, 0x59A1C0, this, smpl, time); }

        void WaitSample(const sample* smpl, int32_t time = -1)
        { WaitSample(smpl->memSample.memHSample, time); }

        void ModifySample(HSAMPLE inSample, int16_t sFunction, int32_t value)
        { THISCALL_4(void, 0x59A240, this, inSample, sFunction, value); }

        void SetSampleVolume(HSAMPLE smpl, int value)
        { ModifySample(smpl, 1, value); }

        void SetSampleVolume(sample* smpl, int value)
        { ModifySample(smpl->memSample.memHSample, 1, value); }

        void AdjustSoundVolumes()
        { THISCALL_1(void, 0x59A3C0, this); }

        void AdjustMusicVolumes()
        { THISCALL_1(void, 0x59A4B0, this); }

        int32_t SwitchAmbientMusic(TTerrainType terrain)
        { return THISCALL_2(int32_t, 0x59A4E0, this, giTerrainToMusicTrack[terrain]); }

        // Play sample /
        // Проиграть звуковую дорожку.
        /// @param sPtr звуковая дорожка
        HSAMPLE MemorySample(sample* sPtr)
        { return THISCALL_2(HSAMPLE, 0x59A510, this, sPtr); }

        // Is music playing? /
        // Играет ли музыка?
        bool MusicPlaying()
        { return THISCALL_1(bool, 0x59A6F0, this); }

        void service_sounds()
        { THISCALL_1(void, 0x59AAD0, this); }

        void ResumeStream()
        { THISCALL_1(void, 0x59AF00, this); }

        void StartMP3(const char* filename, int32_t loopCount, bool bStopSamples)
        { THISCALL_4(void, 0x59AFB0, this, filename, loopCount, bStopSamples); }

        void StopMP3()
        { THISCALL_1(void, 0x59B310, this); }
    
    public:
        NH3API_VIRTUAL_OVERRIDE_BASEMANAGER(soundManager)

        NH3API_SCALAR_DELETING_DESTRUCTOR

    public:
        // offset: +0x38 = +56,  size = 0x4 = 4
        HANDLE mssHandle;

        // offset: +0x3C = +60,  size = 0x4 = 4
        HANDLE driver;

        // offset: +0x40 = +64,  size = 0x4 = 4
        void** samples;

        // offset: +0x44 = +68,  size = 0x38 = 56
        std::array<void**, 14> samples_array;

        // offset: +0x7C = +124,  size = 0x4 = 4
        int32_t sampleNum;

        // Currently played terrain music /
        // Проигрываемая музыка почвы.
        // offset: +0x80 = +128,  size = 0x4 = 4
        int32_t currentTerrainMusic;

        // offset: +0x84 = +132,  size = 0x4 = 4
        bool32_t playSounds;

        // offset: +0x88 = +136,  size = 0x4 = 4
        bool32_t bChangeSounds;

        // MP3 music is playing /
        // Проигрывается MP3 музыка?
        // offset: +0x8C = +140,  size = 0x4 = 4
        bool32_t MP3Playing;

        // offset: +0x90 = +144,  size = 0x18 = 24
        CRITICAL_SECTION section_sound_call;

        // offset: +0xA8 = +168,  size = 0x18 = 24
        CRITICAL_SECTION section_MP3_change;

        // offset: +0xC0 = +192,  size = 0x18 = 24
        CRITICAL_SECTION section_MP3_name_change;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
soundManager*& gpSoundManager
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x699414, soundManager*));

NH3API_DISABLE_WARNING_END
