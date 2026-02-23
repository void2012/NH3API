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

#include <algorithm>                 // std::clamp
#include <array>                     // std::array
#include <memory>                    // std::destroy_at

#include "artifact.hpp"              // TArtifact, type_artifact
#include "creatures.hpp"             // TCreatureType
#include "hero_enums.hpp"            // THeroID, TSex, TRace, THeroClass, hero_seqid
#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "objects.hpp"               // type_obscuring_object
#include "player.hpp"                // playerData
#include "resources/resources.hpp"   // EGameResource
#include "skills.hpp"                // TSkillMastery
#include "bstruct.hpp"               // bstruct_t

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#define hero_hpp_gpGame (reinterpret_cast<void*>(0x699538))
#define hero_hpp_gpGame_get(OFFSET, ...) (get_bstruct(hero_hpp_gpGame).get<__VA_ARGS__>(OFFSET))

#pragma pack(push, 4)
// Hero basic info from the map editor /
// Базовая информация о настроенном в редакторе карт герое.
// size = 0x14 = 20, align = 4
struct HeroIdentity
{
    // Portrait index /
    // Номер портрета.
    // offset: +0x0 = +0,  size = 0x1 = 1
    int8_t portrait;

    unsigned char : 8;

    // Hero name /
    // Имя героя.
    // offset: +0x4 = +4,  size = 0x10 = 16
    exe_string name;

} NH3API_MSVC_LAYOUT;

// HeroIdentity with hero availability for each of players /
// HeroIdentity с доступностью героя для каждого из игроков.
// size = 0xC = 12, align = 4, baseclass: HeroIdentity
struct HeroPlayerInfo : public HeroIdentity
{
    // Может ли игрок выкупить этого героя в тавернах?
    // offset: +0x8 = +8,  size = 0x4 = 4
    exe_bitset<8> players;

} NH3API_MSVC_LAYOUT;

// Hero placeholder /
// Лагерь героя.
// size = 0x10 = 16, align = 4
struct HeroPlaceholder
{
    // Map object /
    // Соответствующий объект карты
    // offset: +0x0 = +0,  size = 0x4 = 4
    CObject* object;

    // Player this hero belongs to /
    // Игрок, которому принадлежит этот герой
    // offset: +0x4 = +4,  size = 0x1 = 1
    int8_t player;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Hero ID /
    // ID героя.
    // offset: +0x8 = +8,  size = 0x4 = 4
    THeroID id;

    // Hero power /
    // Сила героя.
    // offset: +0xC = +12,  size = 0x1 = 1
    int8_t power;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;
} NH3API_MSVC_LAYOUT;

enum EHeroSpecificAbilityType : uint32_t
{
    SPECIALITY_SKILL              = 0,
    SPECIALITY_CREATURE           = 1,
    SPECIALITY_RESOURCE           = 2,
    SPECIALITY_SPELL              = 3,
    SPECIALITY_CREATURE_UNIVERSAL = 4,
    SPECIALITY_SPEED              = 5,
    SPECIALITY_CREATURE_UPGRADE   = 6,
    SPECIALITY_DRAGONS            = 7
};

template<>
struct nh3api::enum_limits<EHeroSpecificAbilityType>
    : nh3api::enum_limits_base<EHeroSpecificAbilityType, SPECIALITY_SKILL, SPECIALITY_DRAGONS>
{ static inline constexpr bool is_specialized = true; };

// Hero speciality traits /
// Специализация героя.
// size = 0x28 = 40, align = 4
struct THeroSpecificAbility
{
    public:
        // Speciality type /
        // Тип специализации.
        // offset: +0x0 = +0,  size = 0x4 = 4
        EHeroSpecificAbilityType type;

        // Speciality subtype /
        // Подтип специализации.
        // offset: +0x4 = +4,  size = 0x4 = 4
        union
        {
            TSecondarySkill skill;    // вторичный навык
            TCreatureType   creature; // существо
            EGameResource   resource; // ресурс
            SpellID         spell;    // заклинание
        } NH3API_MSVC_LAYOUT info;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t creatureAttackBonus;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t creatureDefenseBonus;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t creatureDamageBonus;

        // offset: +0x14 = +20,  size = 0x4 = 4
        TCreatureType creatureGrade;

    protected:
        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t unknown;

    public:
        // Short speciality name /
        // Короткое название специализации героя.
        // offset: +0x1C = +28,  size = 0x4 = 4
        const char* nameShort;

        // Full speciality name /
        // Полное названте специализации героя.
        // offset: +0x20 = +32,  size = 0x4 = 4
        const char* name;

        // Full speciality description /
        // Описание специализации героя.
        // offset: +0x24 = +36,  size = 0x4 = 4
        const char* description;

};

// Hero class traits /
// Свойства класса героев.
// size = 0x40 = 64, align = 4
struct THeroClassTraits
{
    // Town type /
    // Тип города.
    // offset: +0x0 = +0,  size = 0x4 = 4
    TTownType m_townType;

    // Class name /
    // Название класса.
    // offset: +0x4 = +4,  size = 0x4 = 4
    const char* m_name;

    // Aggression coefficient /
    // Коэффициент агрессии героя.
    // offset: +0x8 = +8,  size = 0x4 = 4
    float m_aggression;

    // Initial primary skills /
    // Начальные первичные навыки.
    // offset: +0xC = +12,  size = 0x4 = 4
    std::array<int8_t, 4> m_initialPrimarySkill;

    // Chance to gain primary each skill before level 10 /
    // Шанс получения каждого первичного навыка до 10 уровня.
    // offset: +0x10 = +16,  size = 0x4 = 4
    std::array<int8_t, 4> m_gainPrimarySkillChance;

    // Chance to gain primary each skill past level 10 /
    // Шанс получения каждого первичного навыка после 10 уровня.
    // offset: +0x14 = +20,  size = 0x4 = 4
    std::array<int8_t, 4> m_gainPrimarySkillChance10P;

    // Chance to gain each secondary skill /
    // Шанс получения каждого вторичного навыка.
    // offset: +0x18 = +24,  size = 0x1C = 28
    std::array<int8_t, 28> m_gainSecondarySkillChance;

    // Chance for heroes of this class to appear in each town /
    // Шанс появления героев этого класса в таверне города.
    // offset: +0x34 = +52,  size = 0x9 = 9
    std::array<int8_t, 9> m_foundInTownType;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;
} NH3API_MSVC_LAYOUT;

// Hero traits /
// Свойства героев.
// size = 0x5C = 92, align = 4
struct THeroTraits
{
    // Hero sex /
    // Пол героя.
    // offset: +0x0 = +0,  size = 0x4 = 4
    TSex m_sex;

    // Hero race /
    // Раса героя.
    // offset: +0x4 = +4,  size = 0x4 = 4
    TRace m_race;

    // Hero class /
    // Класс героя.
    // offset: +0x8 = +8,  size = 0x4 = 4
    THeroClass m_class;

    // First second skill /
    // Первый вторичный навык.
    // offset: +0xC = +12,  size = 0x4 = 4
    TSecondarySkill m_1stSkill;

    // First second skill mastery /
    // Уровень первого вторичного навыка.
    // offset: +0x10 = +16,  size = 0x4 = 4
    TSkillMastery m_1stSkillLevel;

    // Second second skill /
    // Второй вторичный навык.
    // offset: +0x14 = +20,  size = 0x4 = 4
    TSecondarySkill m_2ndSkill;

    // Second second skill mastery /
    // Уровень второго вторичного навыка.
    // offset: +0x18 = +24,  size = 0x4 = 4
    TSkillMastery m_2ndSkillLevel;

    // Hero starts with spell book /
    // Герой начинает с книгой заклинаний.
    // offset: +0x1C = +28,  size = 0x4 = 4
    bool32_t m_startsWithSpellbook;

    // Starting spell /
    // Заклинание, с которым начинает герой.
    // offset: +0x20 = +32,  size = 0x4 = 4
    SpellID m_startingSpell;

    // 1st creature type in hero default starting army /
    // Первый тип существа в начальной армии героя.
    // offset: +0x24 = +36,  size = 0x4 = 4
    TCreatureType m_1stStack;

    // 2nd creature type in hero default starting army /
    // Второй тип существа в начальной армии героя.
    // offset: +0x28 = +40,  size = 0x4 = 4
    TCreatureType m_2ndStack;

    // 3rd creature type in hero default starting army /
    // Третий тип существа в начальной армии героя.
    // offset: +0x2C = +44,  size = 0x4 = 4
    TCreatureType m_3rdStack;

    // Small portrait pcx name /
    // Название изображения малого портрета героя.
    // offset: +0x30 = +48,  size = 0x4 = 4
    const char* m_small_portrait_name;

    // Large portrait pcx name /
    // Название изображения большого портрета героя.
    // offset: +0x34 = +52,  size = 0x4 = 4
    const char* m_large_portrait_name;

    // Hero is allowed in RoE /
    // Герой разрешён в RoE.
    // offset: +0x38 = +56,  size = 0x1 = 1
    bool m_allowedInRoE;

    // Hero is allowed in AB and SoD /
    // Герой разрешён в AB и SoD.
    // offset: +0x39 = +57,  size = 0x1 = 1
    bool m_allowedInABSoD;

    // Is campaign hero /
    // Герои из кампании
    // offset: +0x3A = +58,  size = 0x1 = 1
    bool m_isCampaignHero;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Hero name
    // Имя героя.
    // offset: +0x40 = +64,  size = 0x4 = 4
    const char* m_name;

    // 1st army stack creature amount low /
    // Низкий предел количества существ в первом стеке.
    // offset: +0x44 = +68,  size = 0x4 = 4
    int32_t m_1stStackLow;

    // 1st army stack creature amount high /
    // Верхний предел количества существ в первом стеке.
    // offset: +0x48 = +72,  size = 0x4 = 4
    int32_t m_1stStackHigh;

    // 2nd army stack creature amount low /
    // Низкий предел количества существ во втором стеке.
    // offset: +0x4C = +76,  size = 0x4 = 4
    int32_t m_2ndStackLow;

    // 2nd army stack creature amount high /
    // Верхний предел количества существ во втором стеке.
    // offset: +0x50 = +80,  size = 0x4 = 4
    int32_t m_2ndStackHigh;

    // 3rd army stack creature amount low /
    // Низкий предел количества существ в третьем стеке.
    // offset: +0x54 = +84,  size = 0x4 = 4
    int32_t m_3rdStackLow;

    // 3rd army stack creature amount high /
    // Верхний предел количества существ в третьем стеке.
    // offset: +0x58 = +88,  size = 0x4 = 4
    int32_t m_3rdStackHigh;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

// Heroes traits /
// Свойства героев.
inline std::array<THeroTraits, MAX_HEROES_SOD>& akHeroTraits
= get_global_var_ref(0x679DD0, std::array<THeroTraits, MAX_HEROES_SOD>);

// Heroes classes traits /
// Свойства классов героев.
inline std::array<THeroClassTraits, kNumHeroClasses>& akHeroClassTraits
= get_global_var_ref(0x67D868, std::array<THeroClassTraits, kNumHeroClasses>);

// Heroes specific abilities /
// Специализации героев.
inline std::array<THeroSpecificAbility, MAX_HEROES_SOD>& akHeroSpecificAbilities
= get_global_var_ref(0x678420, std::array<THeroSpecificAbility, MAX_HEROES_SOD>);

inline std::array<int32_t, 20>& move_constants
= get_global_var_ref(0x698AE8, std::array<int32_t, 20>);

class army;
class boat;

#pragma pack(push, 1)
// hero class:
// stores information for hero on adventure map /
// класс hero:
// содержит информацию о герое на карте приключений.
// size = 0x492 = 1170, align = 2, baseclass: type_obscuring_object
class hero : public type_obscuring_object
{
    public:
        inline hero() noexcept
            : hero(nh3api::dummy_tag)
        { THISCALL_1(void, 0x4D8860, this); }

        inline hero(const hero& other) noexcept
            : hero(nh3api::dummy_tag)
        { THISCALL_2(void, 0x406410, this, &other); }

        inline hero(const nh3api::dummy_tag_t& tag) noexcept
            : TownSpecialGrantedMask { tag }, bio { tag }
        {}

        inline ~hero() noexcept
        { THISCALL_1(void, 0x45F870, this); }

        inline hero& operator=(const hero& other) noexcept
        {
            THISCALL_2(void, 0x406410, this, &other);
            return *this;
        }

        hero(hero&& other) noexcept
        {
            std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
            std::memset(static_cast<void*>(&other), 0, sizeof(*this));
        }

        hero& operator=(hero&& other) noexcept
        {
            if ( this != &other )
            {
                std::destroy_at(&this->bio);
                std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                std::memset(static_cast<void*>(&other), 0, sizeof(*this));
            }
            return *this;
        }

    public:
        // This hero belongs to human player /
        // Герой принадлежит игроку-человеку.
        [[nodiscard]] bool belongs_to_human() const
        { return playerOwner >= 0 && THISCALL_2(bool, 0x4CE600, 0x699538, playerOwner); }

        // Count equipped artifacts /
        // Посчитать количество надетых на героя артефактов (countWarMachines = включая военные машины).
        [[nodiscard]] int32_t get_equipped_artifacts(bool countWarMachines) const
        { return THISCALL_2(int32_t, 0x4D92E0, this, countWarMachines); }

        // Count backpack artifacts /
        // Посчитать количество артефактов в рюкзаке (countWarMachines = включая военные машины).
        [[nodiscard]] int32_t get_number_in_backpack(bool countWarMachines) const
        { return THISCALL_2(int32_t, 0x4D9330, this, countWarMachines); }

        // Does hero have this artifact? /
        // Есть ли у героя такой артефакт <artifact>(в рюкзаке или на себе)?
        [[nodiscard]] bool HasArtifact(TArtifact artifact) const
        { return THISCALL_2(bool, 0x4D9420, this, artifact); }

        // This <artifact> is equipped on hero? /
        // Носит ли герой такой артефакт <artifact>?
        [[nodiscard]] bool IsWieldingArtifact(TArtifact artifact) const
        { return THISCALL_2(bool, 0x4D9460, this, artifact); }

        // Add <spell> to spellbook /
        // Добавить заклинание <spell> в книгу заклинаний.
        void AddSpell(SpellID spell) noexcept
        {
            if ( spell > SPELL_NONE && spell < MAX_BOOK_SPELLS )
            {
                in_spellbook[static_cast<size_t>(spell)]     = true;
                available_spells[static_cast<size_t>(spell)] = true;
            }
        }

        void Deallocate(bool bGameLoaded = true, bool remote_move = false)
        { THISCALL_3(void, 0x4DA130, this, bGameLoaded, remote_move); }

        // Get experience points for <iLevel> /
        // Получить соответствующие уровню <iLevel> очки опыта
        [[nodiscard]] static int32_t GetExperience(int32_t iLevel)
        { return FASTCALL_1(int32_t, 0x4DA610, iLevel); }

        // Get experience points for upgrading from <iLevel> to <iLevel>+1 /
        // Получить очки опыта для перехода от уровня <iLevel> героя до уровня <iLevel>+1.
        [[nodiscard]] static int32_t GetExperienceIncrement(int32_t level)
        { return FASTCALL_1(int32_t, 0x4DA690, level); }

        [[nodiscard]] bool GetHflip() const noexcept
        { return facing > 4; }

        [[nodiscard]] hero_seqid GetStandSequence() const noexcept
        { return THISCALL_1(hero_seqid, 0x4D9380, this); }

        [[nodiscard]] bool IsFlying() const
        { return ((flags & HF_ISINBOAT) == 0) && ((flightLevel != eMasteryInvalid) || IsWieldingArtifact(ARTIFACT_ANGEL_WINGS)); }

        [[nodiscard]] bool CanWalkOnWater() const
        { return ((flags & HF_ISINBOAT) == 0) && ((waterWalkPower != eMasteryInvalid) || IsWieldingArtifact(ARTIFACT_BOOTS_OF_LEVITATION)); }

        void obscure_cell()
        { type_obscuring_object::obscure_cell(OBJECT_HERO, id); }

        // First not empty backpack slot /
        // Первый непустой слот рюкзака
        [[nodiscard]] int32_t get_last_backpack_index() const noexcept
        {
            int32_t i = 64; /* = backpack.size(); */
            while ( i-- != 0 )
                if ( backpack[static_cast<size_t>(i)].type != ARTIFACT_NONE )
                    return i;
            return -1;
        }

        [[nodiscard]] inline exe_string get_morale_description() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4DC590, this, &result);
            return result;
        }

        [[nodiscard]] inline exe_string get_luck_description() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4DCD30, this, &result);
            return result;
        }

        void SetSS(TSecondarySkill iWhichSS, TSkillMastery iLevelToSet)
        {
            assert(nh3api::in_enum_range(iWhichSS));
            assert(nh3api::in_enum_range(iLevelToSet));

            if ( nh3api::in_enum_range(iWhichSS) && nh3api::in_enum_range(iLevelToSet) )
                THISCALL_3(void, 0x4E2480, this, iWhichSS, iLevelToSet);
        }

        int32_t TakeSS(TSecondarySkill iWhichSS, TSkillMastery iNumLevelsToTake)
        {
            assert(nh3api::in_enum_range(iWhichSS));
            assert(nh3api::in_enum_range(iNumLevelsToTake));

            if ( nh3api::in_enum_range(iWhichSS) && nh3api::in_enum_range(iNumLevelsToTake) )
                return THISCALL_3(int32_t, 0x4E24C0, this, iWhichSS, iNumLevelsToTake);
            return 0;
        }

        int32_t GiveSS(TSecondarySkill iWhichSS, TSkillMastery iNumLevelsToGive)
        {
            assert(nh3api::in_enum_range(iWhichSS));
            assert(nh3api::in_enum_range(iNumLevelsToGive));

            if ( nh3api::in_enum_range(iWhichSS) && nh3api::in_enum_range(iNumLevelsToGive) )
                return THISCALL_3(int32_t, 0x4E2540, this, iWhichSS, iNumLevelsToGive);
            return 0;
        }

        [[nodiscard]] int32_t CreatureTypeCount(TCreatureType creatureType) const
        { return THISCALL_2(int32_t, 0x4E25B0, this, creatureType); }

        void UpgradeCreatures(TCreatureType sourceCreatureType, TCreatureType destCreatureType)
        {
            if ( nh3api::in_enum_range(sourceCreatureType) && nh3api::in_enum_range(destCreatureType) )
                THISCALL_3(void, 0x4E25E0, this, sourceCreatureType, destCreatureType);
        }

        [[nodiscard]] TSecondarySkill GetNthSS(uint32_t nSS) const
        {
            if ( nSS > 0 && nSS <= 8 )
                return THISCALL_2(TSecondarySkill, 0x4E2610, this, nSS);
            return SKILL_NONE;
        }

        [[nodiscard]] bool HasSecondarySkill(TSecondarySkill skill) const noexcept
        { return nh3api::in_enum_range(skill) ? (SSOrder[static_cast<size_t>(skill)] != 0) : false; }

        void TransferArtifacts(hero* src)
        { THISCALL_2(void, 0x4E2640, this, src); }

        [[nodiscard]] bool ArtifactAllowedInSlot(TArtifact artifact, TArtifactSlot slot)
        {
            if ( nh3api::in_enum_range(artifact) && nh3api::in_enum_range(slot) )
                return THISCALL_3(bool, 0x4E27C0, this, artifact, slot);
            return false;
        }

        [[nodiscard]] bool ArtifactEquippableInSlot(TArtifact artifact, TArtifactSlot slot)
        {
            if ( nh3api::in_enum_range(artifact) && nh3api::in_enum_range(slot) )
                return THISCALL_3(bool, 0x4E2AB0, this, artifact, slot);
            return false;
        }

        bool equip_artifact(const type_artifact& artifact, TArtifactSlot slot)
        {
            if ( nh3api::in_enum_range(slot) || slot == -1 )
                return THISCALL_3(bool, 0x4E2C70, this, &artifact, slot);
            return false;
        }

        void remove_artifact(TArtifactSlot slot)
        { THISCALL_2(void, 0x4E2E40, this, slot); }

        void remove_backpack_artifact(TArtifactSlot slot)
        { THISCALL_2(void, 0x4E2FC0, this, static_cast<uint16_t>(slot) & 0xFFFFU); }

        bool remove_artifact(TArtifact art)
        { return THISCALL_2(bool, 0x4E3040, this, art); }

        [[nodiscard]] inline exe_string get_backpack_error() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4E3140, this, &result);
            return result;
        }

        bool add_to_backpack(const type_artifact& artifact, TArtifactSlot slot)
        { return THISCALL_3(bool, 0x4E3200, this, &artifact, slot); }

        type_artifact& get_backpack(size_t slot) noexcept
        {
            assert(slot < 64);
            if ( slot < 64 )
                return backpack[slot];
            NH3API_UNREACHABLE();
        }

        [[nodiscard]] const type_artifact& get_backpack(size_t slot) const noexcept
        {
            assert(slot < 64);
            if ( slot < 64 )
                return backpack[slot];
            NH3API_UNREACHABLE();
        }

        type_artifact& get_artifact(TArtifactSlot slot) noexcept
        {
            assert(slot >= FIRST_ARTIFACT_SLOT && slot < MAX_ARTIFACT_SLOTS);
            if ( slot >= FIRST_ARTIFACT_SLOT && slot < MAX_ARTIFACT_SLOTS )
                return equipped[static_cast<size_t>(slot)];
            NH3API_UNREACHABLE();
        }

        [[nodiscard]] const type_artifact& get_artifact(TArtifactSlot slot) const noexcept
        {
            assert(slot >= FIRST_ARTIFACT_SLOT && slot < MAX_ARTIFACT_SLOTS);
            if ( slot >= FIRST_ARTIFACT_SLOT && slot < MAX_ARTIFACT_SLOTS )
                return equipped[static_cast<size_t>(slot)];
            NH3API_UNREACHABLE();
        }

        [[nodiscard]] int8_t GetPrimarySkill(TPrimarySkill skill) const noexcept
        {
            assert(skill >= FIRST_PRIMARY_SKILL && skill < MAX_PRIMARY_SKILLS);
            if ( skill >= FIRST_PRIMARY_SKILL && skill < MAX_PRIMARY_SKILLS )
                return std::clamp<int8_t>(stats[static_cast<size_t>(skill)], 0, 99);
            return 0;
        }

        void SetPrimarySkill(TPrimarySkill skill, int8_t amount) noexcept
        {
            assert(skill >= FIRST_PRIMARY_SKILL && skill < MAX_PRIMARY_SKILLS);
            if ( skill >= FIRST_PRIMARY_SKILL && skill < MAX_PRIMARY_SKILLS )
                stats[static_cast<size_t>(skill)] = std::clamp<int8_t>(amount, 0, 99);
        }

        [[nodiscard]] float get_aggression() const noexcept
        { return aggression; }

        [[nodiscard]] TSkillMastery get_secondary_skill(TSecondarySkill skill) const noexcept
        {
            assert(skill >= FIRST_SECONDARY_SKILL && skill < MAX_SECONDARY_SKILLS);
            if ( skill >= FIRST_SECONDARY_SKILL && skill < MAX_SECONDARY_SKILLS )
                return static_cast<TSkillMastery>(SSLevel[static_cast<size_t>(skill)]);
            return SKILL_MASTERY_NONE;
        }

        [[nodiscard]] int32_t get_value_of_duration() const noexcept
        { return value_of_duration; }

        [[nodiscard]] int32_t get_value_of_knowledge() const noexcept
        { return value_of_knowledge; }

        [[nodiscard]] int32_t get_value_of_power() const noexcept
        { return value_of_power; }

        [[nodiscard]] bool is_in_spellbook(SpellID spell) const noexcept
        {
            if ( spell >= FIRST_BOOK_SPELL && spell < MAX_BOOK_SPELLS )
                return in_spellbook[static_cast<size_t>(spell)];
            return false;
        }

        [[nodiscard]] int32_t GetMaxMana() const
        { return static_cast<int32_t>(std::clamp<int32_t>(stats[static_cast<size_t>(PRIMARY_SKILL_KNOWLEDGE)], 1, 99) * 10 * GetIntelligenceFactor()); }

        bool GiveArtifact(const type_artifact& artifact, bool assemble_combo, bool equip_it)
        { return THISCALL_4(bool, 0x4E32E0, this, &artifact, assemble_combo, equip_it); }

        int32_t GiveExperience(int32_t howMuch, bool bCheckLevel, bool show_cap_window)
        { return THISCALL_4(int32_t, 0x4E3620, this, howMuch, bCheckLevel, show_cap_window); }

        void GiveResource(EGameResource whichRes, int32_t howMuch)
        { THISCALL_3(void, 0x4E3870, this, whichRes, howMuch); }

        [[nodiscard]] const char* GetSpecificAbilityText() const noexcept
        { return akHeroSpecificAbilities[static_cast<size_t>(id)].name; }

        [[nodiscard]] const char* GetSpecificAbilityTextShort() const noexcept
        { return akHeroSpecificAbilities[static_cast<size_t>(id)].nameShort; }

        [[nodiscard]] int32_t GetLuck(const hero* otherHero, int32_t on_cursed_ground, int32_t apply_limits) const
        { return THISCALL_4(int32_t, 0x4E3930, this, otherHero, on_cursed_ground, apply_limits); }

        [[nodiscard]] int32_t GetMorale(const hero* other_hero, bool on_cursed_ground, int32_t apply_limits) const
        { return THISCALL_4(int32_t, 0x4E3C20, this, other_hero, on_cursed_ground, apply_limits); }

        [[nodiscard]] TCreatureType GetNecromancyCreature() const
        { return THISCALL_1(TCreatureType, 0x4E3ED0, this); }

        [[nodiscard]] double GetNecromancyFactor(bool apply_limit) const
        { return THISCALL_2(double, 0x4E3F40, this, apply_limit); }

        [[nodiscard]] int32_t GetMysticismBonus() const
        { return THISCALL_1(int32_t, 0x4E41B0, this); }

        [[nodiscard]] int32_t GetVisibility() const
        { return THISCALL_1(int32_t, 0x4E42E0, this); }

        [[nodiscard]] double GetArcheryFactor() const
        { return THISCALL_1(double, 0x4E43D0, this); }

        [[nodiscard]] double GetOffenseFactor() const
        { return THISCALL_1(double, 0x4E4520, this); }

        [[nodiscard]] double GetDefenseFactor() const
        { return THISCALL_1(double, 0x4E4580, this); }

        [[nodiscard]] int32_t GetEstatesBonus() const
        { return THISCALL_1(int32_t, 0x4E4600, this); }

        [[nodiscard]] double GetEagleEyeChance() const
        { return THISCALL_1(double, 0x4E4690, this); }

        [[nodiscard]] double GetSurrenderCostFactor() const
        { return THISCALL_1(double, 0x4E47F0, this); }

        [[nodiscard]] double GetMagicResistanceFactor() const
        { return THISCALL_1(double, 0x4E4950, this); }

        [[nodiscard]] double GetExperienceBonusFactor() const
        { return THISCALL_1(double, 0x4E4AB0, this); }

        [[nodiscard]] double GetIntelligenceFactor() const
        { return THISCALL_1(double, 0x4E4B20, this); }

        [[nodiscard]] double GetFirstAidFactor() const
        { return THISCALL_1(double, 0x4E4B90, this); }

        [[nodiscard]] int32_t GetMobility(bool sea_movement) const
        { return THISCALL_2(int32_t, 0x4E4C00, this, sea_movement); }

        [[nodiscard]] int32_t GetMobility() const
        { return THISCALL_1(int32_t, 0x4E5000, this); }

        [[nodiscard]] int32_t GetSpellDurationBonus() const
        { return THISCALL_1(int32_t, 0x4E5020, this); }

        [[nodiscard]] TAdventureObjectType GetGroundModifier() const
        { return THISCALL_1(TAdventureObjectType, 0x4E5130, this); }

        [[nodiscard]] int32_t get_special_terrain() const
        { return THISCALL_1(int32_t, 0x4E5210, this); }

        [[nodiscard]] TSkillMastery get_spell_level(SpellID spell, int32_t magic_terrain_type) const
        { return THISCALL_3(TSkillMastery, 0x4E52F0, this, spell, magic_terrain_type); }

        [[nodiscard]] TSkillMastery GetSpellSchoolLevel(TSpellSchool school_mask, bool is_on_magic_plains) const
        { return THISCALL_3(TSkillMastery, 0x4E5370, this, school_mask, is_on_magic_plains); }

        [[nodiscard]] TSpellSchool GetHighestSchool(TSpellSchool school_mask) const
        { return THISCALL_2(TSpellSchool, 0x4E5430, this, school_mask); }

        int32_t GetManaCost(SpellID iWhichSpell, const armyGroup* enemy, int8_t magic_terrain) const
        { return THISCALL_4(int32_t, 0x4E54B0, this, iWhichSpell, enemy, magic_terrain); }

        [[nodiscard]] boat* find_summonable_boat()
        { return THISCALL_1(boat*, 0x4E5710, this); }

        [[nodiscard]] const boat* find_summonable_boat() const
        { return THISCALL_1(const boat*, 0x4E5710, this); }

        [[nodiscard]] bool can_summon_boat() const
        { return THISCALL_1(bool, 0x4E57C0, this); }

        [[nodiscard]] playerData* get_player() noexcept
        { return ( playerOwner >= 0 && playerOwner < 8 ) ? &hero_hpp_gpGame_get(0x20AD0, std::array<playerData, 8>)[static_cast<size_t>(this->playerOwner)] : nullptr; }

        [[nodiscard]] const playerData* get_player() const noexcept
        { return ( playerOwner >= 0 && playerOwner < 8 ) ? &hero_hpp_gpGame_get(0x20AD0, std::array<playerData, 8>)[static_cast<size_t>(this->playerOwner)] : nullptr; }

        // Get town currently occupied by a hero /
        // Город, на данный момент занятый героем.
        [[nodiscard]] town* GetOccupiedTown() const noexcept
        { return type_obscuring_object::get_obscured_town(); }

        [[nodiscard]] type_point get_target() const noexcept
        { return { static_cast<int16_t>(targetX), static_cast<int16_t>(targetY), static_cast<int8_t>(targetZ) }; }

        [[nodiscard]] bool SpellIsAvailable(SpellID spell) const noexcept
        { return (spell >= FIRST_BOOK_SPELL && spell < MAX_BOOK_SPELLS) && available_spells[static_cast<size_t>(spell)]; }

        [[nodiscard]] bool is_in_patrol_radius(type_point point) const
        { return THISCALL_2(bool, 0x4E5950, this, point); }

        int32_t modify_spell_damage(SpellID spell, int32_t damage, army* target_army) const
        { return THISCALL_4(int32_t, 0x4E59D0, this, spell, damage, target_army); }

        [[nodiscard]] int32_t get_combat_speed_bonus() const
        { return THISCALL_1(int32_t, 0x4E5D10, this); }

        [[nodiscard]] int32_t get_hit_point_bonus(TCreatureType creature) const
        { return THISCALL_2(int32_t, 0x4E5DF0, this, creature); }

        [[nodiscard]] int32_t GetRoguePower() const
        { return THISCALL_1(int32_t, 0x4E6050, this); }

        [[nodiscard]] bool IsInIdentifyRange(const type_point& point) const
        { return THISCALL_2(bool, 0x4E6080, this, &point); }

        [[nodiscard]] bool IsMobile() const
        { return THISCALL_1(bool, 0x4E61A0, this); }

        [[nodiscard]] int32_t GetHeroSpellBonus(SpellID spell_id, int32_t target_level, int32_t value) const
        { return THISCALL_4(int32_t, 0x4E6260, this, spell_id, target_level, value); }

    public:
        // Hero's mana /
        // Мана героя.
        // offset: +0x18 = +24,  size = 0x2 = 2
        int16_t         mana;

        // Hero ID /
        // Тип героя.
        // offset: +0x1A = +26,  size = 0x4 = 4
        THeroID         id;

        // Hero relative index, 0..7 /
        // Порядковый номер(от 0 до 7) героя у игрока.
        // offset: +0x1E = +30,  size = 0x4 = 4
        int32_t         order;

        // Player this hero belongs to /
        // Игрок, которому принадлежит этот герой.
        // offset: +0x22 = +34,  size = 0x1 = 1
        int8_t          playerOwner;

        // Hero name /
        // Имя героя.
        // offset: +0x23 = +35,  size = 0xD = 13
        std::array<char, 13> name;

        // Hero class /
        // Класс героя.
        // offset: +0x30 = +48,  size = 0x4 = 4
        THeroClass      hero_class;

        // Portrait ID /
        // ID портрета.
        // offset: +0x34 = +52,  size = 0x1 = 1
        uint8_t         portrait;

        // Path target cell X coordinate /
        // X-координата клетки, на которую нацелен герой на карте.
        // offset: +0x35 = +53,  size = 0x4 = 4
        int32_t         targetX;

        // Path target cell Y coordinate /
        // Y-координата клетки, на которую нацелен герой на карте.
        // offset: +0x39 = +57,  size = 0x4 = 4
        int32_t         targetY;

        // Path target cell Z coordinate /
        // Z-координата клетки, на которую нацелен герой на карте.
        // offset: +0x3D = +61,  size = 0x2 = 2
        int16_t         targetZ;

        // Last level hero could learn any magic school /
        // Последний уровень героя, при получении которого появилась возможность изучить школу магии.
        // offset: +0x3F = +63,  size = 0x2 = 2
        int16_t         last_magic_school_level;

        // AI target cell distance in Move Points(MP) /
        // Расстояние до клетки цели пути ИИ(в Move Point-ах(MP)).
        // offset: +0x41 = +65,  size = 0x2 = 2
        int16_t         target_distance;

        // offset: +0x43 = +67,  size = 0x1 = 1
        bool            target_is_critical;

        // Hero's initial cell x coordinate(used to determine hero patrol zone) /
        // X клетки первоначальной позиции героя на карте(используется для определения зоны ограничения передвижения)
        // offset: +0x44 = +68,  size = 0x1 = 1
        uint8_t         patrolX;

        // Hero initial cell x coordinate(used to determine hero patrol zone) /
        // Y клетки первоначальной позиции героя на карте(используется для определения зоны ограничения передвижения)
        // offset: +0x45 = +69,  size = 0x1 = 1
        uint8_t         patrolY;

        // Hero patrol radius from the (patrolX, patrolY) cell /
        // Радиус ограничения движения героя относительно клетки (patrolX, patrolY).
        // offset: +0x46 = +70,  size = 0x1 = 1
        int8_t          patrolRadius;

        // Hero direction (0..7, clockwise) /
        // Направление героя (0..7, по часовой стрелке)
        // offset: +0x47 = +71,  size = 0x1 = 1
        uint8_t         facing;

        enum Formation : int8_t
        {
            GroupedFormation   = 1,
            PlacementFormation = 2,
        }
        // Hero army formation /
        // Расстановка в армии героя.
        // offset: +0x48 = +72,  size = 0x1 = 1
        formation;

        // Hero max MPs /
        // Максимальное количество MP героя.
        // offset: +0x49 = +73,  size = 0x4 = 4
        int32_t         maxMobility;

        // Hero current MPs /
        // Текущее количество MP героя.
        // offset: +0x4D = +77,  size = 0x4 = 4
        int32_t         currMobility;

        // Hero XP /
        // Очки опыта героя.
        // offset: +0x51 = +81,  size = 0x4 = 4
        int32_t         experience;

        // Hero level /
        // Уровень (опыта) героя.
        // offset: +0x55 = +85,  size = 0x2 = 2
        int16_t         Level;

        // original name: "TrainingGroundFlags"
        // Visited learning stones /
        // Посещённые камни знаний.
        // offset: +0x57 = +87,  size = 0x4 = 4
        uint32_t  LearningStoneFlags;

        // Visited marletto towers /
        // Посещённые башни марлетто.
        // offset: +0x5B = +91,  size = 0x4 = 4
        uint32_t DefenseTowerFlags;

        // Visited gardens of revelation /
        // Посещённые сады откровения.
        // offset: +0x5F = +95,  size = 0x4 = 4
        uint32_t GardenOfRevelationFlags;

        // Visited mercenary camps /
        // Посещённые лагеря наёмников.
        // offset: +0x63 = +99,  size = 0x4 = 4
        uint32_t MercCampFlags;

        // original name: "PowerSchoolFlags"
        // Visited star axises /
        // Посещённые звёздные оси.
        // offset: +0x67 = +103,  size = 0x4 = 4
        uint32_t StarAxisFlags;

        // Visited trees of knowledge /
        // Посещённые деревья знаний.
        // offset: +0x6B = +107,  size = 0x4 = 4
        uint32_t TreeOfKnowledgeFlags;

        // Visited libraries /
        // Посещённые библиотеки.
        // offset: +0x6F = +111,  size = 0x4 = 4
        uint32_t LibraryFlags;

        // Visited arenas /
        // Посещённые арены.
        // offset: +0x73 = +115,  size = 0x4 = 4
        uint32_t ArenaFlags;

        // Visited schools of magic /
        // Посещённые школы магии
        // offset: +0x77 = +119,  size = 0x4 = 4
        uint32_t MagicSchoolFlags;

        // Visited schools of war /
        // Посещённые школы войны.
        // offset: +0x7B = +123,  size = 0x4 = 4
        uint32_t WarSchoolFlags;

    protected:
        [[maybe_unused]]
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x7F = +127,  size = 0x4 = 4
        uint32_t UniversityFlags;

        [[maybe_unused]]
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x83 = +131,  size = 0x4 = 4
        uint32_t Shrine1Flags;

        [[maybe_unused]]
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x87 = +135,  size = 0x4 = 4
        uint32_t Shrine2Flags;

        [[maybe_unused]]
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x8B = +139,  size = 0x4 = 4
        uint32_t Shrine3Flags;

    public:
        // Hero level up tree random seed /
        // Случайный сид дерева улучшения уровня героя.
        // offset: +0x8F = +143,  size = 0x1 = 1
        uint8_t iLevelSeed;

        // Last level hero could learn any magic school /
        // Последний уровень героя, при получении которого появилась возможность изучить школу магии.
        // offset: +0x90 = +144,  size = 0x1 = 1
        uint8_t lastWisdom;

        // Hero army /
        // Армия героя.
        // offset: +0x91 = +145,  size = 0x38 = 56
        armyGroup heroArmy;

        // Hero secondary skills levels /
        // Ступени вторичных навыков героя.
        // offset: +0xC9 = +201,  size = 0x1C = 28
        std::array<int8_t, MAX_SECONDARY_SKILLS> SSLevel;

        // Hero secondary skills order(0..7) /
        // Порядки вторичных навыков героя(0..7).
        // offset: +0xE5 = +229,  size = 0x1C = 28
        std::array<uint8_t, MAX_SECONDARY_SKILLS> SSOrder;

        // Number of learned secondary skills /
        // Количество вторичных навыков героя.
        // offset: +0x101 = +257,  size = 0x4 = 4
        int32_t numSSs;

        // Hero flags /
        // Флаги героя.
        // offset: +0x105 = +261,  size = 0x4 = 4
        hero_flags flags;

        // offset: +0x109 = +265,  size = 0x4 = 4
        float turnExperienceToRVRatio;

        // Number of casts of Dimension Door(DD) this turn /
        // Количество исполненных заклинаний дверь измерения(DD) на этом ходу.
        // offset: +0x10D = +269,  size = 0x1 = 1
        int8_t dWalkSpellsCast;

        // Hero disguise spell level /
        // Уровень заклинания маскировки героя.
        // offset: +0x10E = +270,  size = 0x4 = 4
        TSkillMastery disguiseLevel;

        // Hero fly spell level /
        // Уровень заклинания полёта героя.
        // offset: +0x112 = +274,  size = 0x4 = 4
        TSkillMastery flightLevel;

        // Hero water walk spell level /
        // Уровень заклинания хождения по воде героя.
        // offset: +0x116 = +278,  size = 0x4 = 4
        TSkillMastery waterWalkPower;

        // Morale bonus /
        // Бонус боевого духа героя.
        // offset: +0x11A = +282,  size = 0x1 = 1
        int8_t moraleBonus;

        // Luck bonus /
        // Бонус удачи героя.
        // offset: +0x11B = +283,  size = 0x1 = 1
        int8_t luckBonus;

        // Hero is sleeping /
        // Герой "спит"(нажата соответствующая кнопка).
        // offset: +0x11C = +284,  size = 0x1 = 1
        bool IsSleeping;

        // Hero's killing AI value /
        // ценность убийства этого героя для ИИ.
        // offset: +0x11D = +285,  size = 0x4 = 4
        int32_t bounty;

        union {
        // Towns where special leveling up buildings are visited /
        // Города, в которых посещены специальные строения улучшения навыка героя.
        // offset: +0x121 = +289,  size = 0x8 = 8
        exe_bitset<48> TownSpecialGrantedMask;
        };

        // Hero visions spell level /
        // Уровень заклинания видения героя.
        // offset: +0x129 = +297,  size = 0x4 = 4
        int32_t visionsPower;

        // Equipped artifacts /
        // Надетые на героя артефакты.
        // offset: +0x12D = +301,  size = 0x98 = 152
        std::array<type_artifact, MAX_ARTIFACT_SLOTS> equipped;

        // blocked slots by combination artifacts /
        // Заблокированные слоты сборным артефактов.
        // offset: +0x1C5 = +453,  size = 0xF = 15
        std::array<int8_t, 15> blockedSlots;

        // Hero backpack /
        // Рюкзак героя.
        // offset: +0x1D4 = +468,  size = 0x200 = 512
        std::array<type_artifact, 64> backpack;

        // Number of artifacts in backpack /
        // Количество артефактов в рюкзаке.
        // offset: +0x3D4 = +980,  size = 0x1 = 1
        int8_t backpack_count;

        // Hero sex /
        // Пол героя.
        // offset: +0x3D5 = +981,  size = 0x4 = 4
        TSex sex;

        // Hero's biography is customized /
        // У героя настроена биография.
        // offset: +0x3D9 = +985,  size = 0x1 = 1
        bool bio_customized;

        union {
        // Hero's biography /
        // Биография героя.
        // offset: +0x3DA = +986,  size = 0x10 = 16
        exe_string bio;
        };

        // Learned spells /
        // Выученные героем заклинания.
        // offset: +0x3EA = +1002,  size = 0x46 = 70
        std::array<bool, MAX_BOOK_SPELLS> in_spellbook;

        // Available spells(including artifact spells) /
        // Доступные герою заклинания(учитывая артефакты).
        // offset: +0x430 = +1072,  size = 0x46 = 70
        std::array<bool, MAX_BOOK_SPELLS> available_spells;

        // Primary skills /
        // Первичные навыки.
        // offset: +0x476 = +1142,  size = 0x4 = 4
        std::array<int8_t, MAX_PRIMARY_SKILLS> stats;

        // Hero aggression /
        // Агрессия героя.
        // offset: +0x47A = +1146,  size = 0x4 = 4
        float aggression;

        // AI value of increasing of power /
        // Ценность ИИ увеличения силы магии.
        // offset: +0x47E = +1150,  size = 0x4 = 4
        int32_t value_of_power;

        // AI value of increasing duration of spells /
        // Ценность ИИ увеличения длительности заклинаний.
        // offset: +0x482 = +1154,  size = 0x4 = 4
        int32_t value_of_duration;

        // AI value of increasing of knowledge /
        // Ценность ИИ увеличения знаний.
        // offset: +0x486 = +1158,  size = 0x4 = 4
        int32_t value_of_knowledge;

        // Value of doubling of mana /
        // Ценность ИИ посещения объекта, удваивающего ману.
        // offset: +0x48A = +1162,  size = 0x4 = 4
        int32_t value_of_spring;

        // Value of refilling mana /
        // Ценность ИИ колодца.
        // offset: +0x48E = +1166,  size = 0x4 = 4
        int32_t value_of_well;
};

NH3API_SIZE_ASSERT(0x492, hero);

// Hero extra information(from map file) /
// Информация о герое(из файла карты).
// size = 0x334 = 820, align = 1
struct HeroExtra
{
    // Player owning this hero /
    // Игрок, обладающий этим героем.
    // offset: +0x0 = +0,  size = 0x1 = 1
    int8_t Owner;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Hero ID /
    // ID Героя.
    // offset: +0x4 = +4,  size = 0x4 = 4
    THeroID id;

    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t objRef;

    // Hero's name was set in map editor /
    // Имя героя настроено в редакторе.
    // offset: +0xC = +12,  size = 0x1 = 1
    bool bCustomName;

    // Hero's name. Max length is 12 /
    // Имя героя. Максимальное кол-во символов = 12.
    // offset: +0xD = +13,  size = 0xD = 13
    std::array<char, 13> Name;

    unsigned char : 8;
    unsigned char : 8;

    // Hero experience in XP /
    // Опыт героя в очках опыта.
    // offset: +0x1C = +28,  size = 0x4 = 4
    int32_t Experience;

    // Does hero have custom portrait number via map editor? /
    // Настроен ли номер портрета в редакторе карт?
    // offset: +0x20 = +32,  size = 0x1 = 1
    bool bCustomPortraitNumber;

    // Hero portrait number /
    // Номер портрета героя.
    // offset: +0x21 = +33,  size = 0x1 = 1
    uint8_t PortraitNumber;

    // Does hero have custom secondary skills via map editor?
    // Настроены ли вторичные навыки героя в редакторе карт?
    // offset: +0x22 = +34,  size = 0x1 = 1
    bool bCustomSecondarySkills;

    unsigned char : 8;

    // Number of hero secondary skills /
    // Кол-во вторичных навыков героя.
    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t NumSecondarySkills;

    // Hero secondary skills /
    // Вторичные навыки героя.
    // offset: +0x28 = +40,  size = 0x8 = 8
    std::array<int8_t, MAX_SKILLS_PER_HERO> secondarySkill;

    // Hero secondary skills levels(masteries) /
    // Уровни вторичного навыка.
    // offset: +0x30 = +48,  size = 0x8 = 8
    std::array<int8_t, MAX_SKILLS_PER_HERO> secondarySkillLevel;

    // Does hero have custom army edited via map editor?
    // Армия героя настроена в редакторе карт?
    // offset: +0x38 = +56,  size = 0x1 = 1
    bool bCustomArmies;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Creature types edited via map editor /
    // Типы существ, настроенные в редакторе карт.
    // offset: +0x3C = +60,  size = 0x1C = 28
    std::array<TCreatureType, 7> armies;

    // Creature numbers edited via map editor /
    // Кол-ва существ, настроенные в редакторе карт.
    // offset: +0x58 = +88,  size = 0xE = 14
    std::array<int16_t, 7> numTroops;

    // Hero army formation. (See enum hero::Formation) /
    // Строй в армии героя. (см. enum hero::Formation).
    // offset: +0x66 = +102,  size = 0x1 = 1
    int8_t GroupFormation;

    // Are hero artifacts edited(via map editor)? /
    // Артефакты героя настроены(в редакторе карт)?
    // offset: +0x67 = +103,  size = 0x1 = 1
    int8_t bCustomArtifacts;

    // Equipped artifacts /
    // Артефакты, надетые на героя.
    // offset: +0x68 = +104,  size = 0x98 = 152
    std::array<type_artifact, MAX_ARTIFACT_SLOTS_SOD> artifacts;

    // Hero backpack /
    // Рюкзак героя. Вмещает 64 артефакта.
    // offset: +0x100 = +256,  size = 0x200 = 512
    std::array<type_artifact, 64> backpack;

    // Number of artifacts in backpack /
    // Кол-во артефактов в рюкзаке героя.
    // offset: +0x300 = +768,  size = 0x1 = 1
    uint8_t numInBackpack;

    // Hero initial position /
    // Первоначальная клетка героя.
    // offset: +0x301 = +769,  size = 0x4 = 4
    type_point location;

    // Hero patrol radius from the (location.x, location.y) cell /
    // Радиус ограничения движения героя относительно клетки (location.x, location.y).
    // offset: +0x305 = +773,  size = 0x1 = 1
    int8_t PatrolRadius;

    // Customized biography /
    // Биография героя настроена в редакторе карт.
    // offset: +0x306 = +774,  size = 0x1 = 1
    bool bCustomBiography;

    unsigned char : 8;

    // Hero biography /
    // Биография героя.
    // offset: +0x308 = +776,  size = 0x10 = 16
    exe_string sBiography;

    // Hero sex(0 = man, 1 = woman) /
    // Пол героя(0 = мужчина, 1 = женщина).
    // offset: +0x318 = +792,  size = 0x4 = 4
    TSex sex;

    // Hero has custom spells? /
    // У героя настроены заклинания?
    // offset: +0x31C = +796,  size = 0x1 = 1
    int8_t bCustomSpells;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

    // Customized spells /
    // Настроенные заклинания.
    // offset: +0x320 = +800,  size = 0xC = 12
    exe_bitset<70> customSpells;

    // Hero has customized primary skills /
    // У героя настроены первичные навыки?
    // offset: +0x32C = +812,  size = 0x1 = 1
    int8_t bCustomPrimarySkills;

    // Customized hero primary skills /
    // Настроенные первичные навыки героя.
    // offset: +0x32D = +813,  size = 0x4 = 4
    std::array<int8_t, MAX_PRIMARY_SKILLS> primarySkills;

    unsigned char : 8;
    unsigned char : 8;
    unsigned char : 8;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 1

#pragma pack(push, 4)
// Boat /
// Лодка.
// size = 0x28 = 40, align = 4, baseclass: type_obscuring_object
class boat : public type_obscuring_object
{
    public:
        inline boat() noexcept = default;

        inline boat(const nh3api::dummy_tag_t&) noexcept
        {}

        void obscure_cell()
        { type_obscuring_object::obscure_cell(OBJECT_BOAT, id); }

    public:
        // Is boat allocated? /
        // Лодка выделена?
        // offset: +0x18 = +24,  size = 0x1 = 1
        bool allocated { false };

        // Boat ID /
        // ID лодки.
        // offset: +0x19 = +25,  size = 0x1 = 1
        uint8_t id;

        // Boat type(0..2) /
        // Тип лодки(0..2).
        // offset: +0x1A = +26,  size = 0x1 = 1
        int8_t boat_type;

        // Boat direction (0..7, clockwise) /
        // Направление лодки (0..7, по часовой стрелке)
        // offset: +0x1B = +27,  size = 0x1 = 1
        int8_t facing;

        // Player owning this boat /
        // Игрок, владеющий лодкой.
        // offset: +0x1C = +28,  size = 0x1 = 1
        int8_t playerOwner;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Hero occupying this boat /
        // Герой, сидящий в лодке.
        // offset: +0x20 = +32,  size = 0x4 = 4
        THeroID occupying_hero;

        // Has hero occupying this boat /
        // Есть герой, сидящий в лодке.
        // offset: +0x24 = +36,  size = 0x1 = 1
        bool occupied;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

inline void do_monster_join_dialog(hero* inHero, TCreatureType type, int32_t amount)
{ FASTCALL_3(void, 0x5D15D0, inHero, type, amount); }

inline void do_monster_join_dialog(hero* inHero, armyGroup& monsters, bool leave_guards)
{ FASTCALL_3(void, 0x5D16B0, inHero, &monsters, leave_guards); }

NH3API_WARNING(pop)
