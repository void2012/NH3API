//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/base_manager.hpp"
#include "core/resources/sounds.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

// avoid heavy <winbase.h> include
extern "C"
{
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
} // extern "C"

#pragma pack(push, 4)
// Sound & music manager /
// Менеджер звуков и музыки.
// size = 0xD8 = 216, align = 4, baseclass: baseManager
NH3API_VIRTUAL_CLASS soundManager : public baseManager
{
    public:
        struct vftable_t : baseManager::vftable_t
        {
            void (__thiscall* scalar_deleting_destructor)(soundManager*, uint8_t);
        };

    public:
        inline soundManager() noexcept
            : baseManager(nh3api::dummy_tag)
        { THISCALL_1(void, 0x599A60, this); }

        inline soundManager(const nh3api::dummy_tag_t& tag) noexcept
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
        { if ( smpl ) StopSample(smpl->memSample.memHSample); }

        void WaitSample(HSAMPLE smpl, int32_t time = -1)
        { if ( smpl) THISCALL_3(void, 0x59A1C0, this, smpl, time); }

        void WaitSample(const sample* smpl, int32_t time = -1)
        { if ( smpl ) WaitSample(smpl->memSample.memHSample, time); }

        void ModifySample(HSAMPLE inSample, int16_t sFunction, int32_t value)
        { THISCALL_4(void, 0x59A240, this, inSample, sFunction, value); }

        void SetSampleVolume(HSAMPLE smpl, int value)
        { ModifySample(smpl, 1, value); }

        void SetSampleVolume(sample* smpl, int value)
        { if ( smpl ) ModifySample(smpl->memSample.memHSample, 1, value); }

        void AdjustSoundVolumes()
        { THISCALL_1(void, 0x59A3C0, this); }

        void AdjustMusicVolumes()
        { THISCALL_1(void, 0x59A4B0, this); }

        int32_t SwitchAmbientMusic(TTerrainType terrain)
        { return ( terrain > TERRAIN_TYPE_NONE && terrain < MAX_TERRAIN_TYPES ) ? THISCALL_2(int32_t, 0x59A4E0, this, giTerrainToMusicTrack[static_cast<size_t>(terrain)]) : 0; }

        // Play sample /
        // Проиграть звуковую дорожку.
        /// @param sPtr звуковая дорожка
        HSAMPLE MemorySample(sample* sPtr)
        { return sPtr ? THISCALL_2(HSAMPLE, 0x59A510, this, sPtr) : nullptr; }

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

inline soundManager* const& gpSoundManager = get_global_var_ref(0x699414, soundManager*);
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63FE68, soundManager)

NH3API_WARNING(pop)
