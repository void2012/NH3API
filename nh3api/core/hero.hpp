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

#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "resources/resources.hpp" // EGameResource
#include "hero_enums.hpp" // THeroID, TSex, TRace, THeroClass, hero_seqid
#include "player.hpp" // playerData
#include "objects.hpp" // type_obscuring_object
#include "artifact.hpp" // TArtifact, type_artifact
#include "creatures.hpp" // TCreatureType
#include "skills.hpp" // TSkillMastery
#include "utils.hpp" // bstruct_t

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#define hero_hpp_gpGame reinterpret_cast<void*>(0x699538)
#define hero_hpp_gpGame_get(OFFSET, ...) get_bstruct(hero_hpp_gpGame).get<__VA_ARGS__>(OFFSET)

#pragma pack(push, 4)
// Hero basic info from the map editor /
// Базовая информация о настроенном в редакторе карт герое.
// size = 0x14 = 20, align = 4
struct HeroIdentity
{
public:
    // Portrait index /
    // Номер портрета.
    // offset: +0x0 = +0,  size = 0x1 = 1
    int8_t portrait;

protected:
    NH3API_MAYBE_UNUSED
    // offset: +0x1 = +1,  size = 0x3 = 3
    byte_t gap_1[3];

public:
    // Hero name /
    // Имя героя.
    // offset: +0x4 = +4,  size = 0x10 = 16
    exe_string name;

};
#pragma pack(pop)

#pragma pack(push, 4)
// HeroIdentity with hero availability for each of players /
// HeroIdentity с доступностью героя для каждого из игроков.
// size = 0xC = 12, align = 4, baseclass: HeroIdentity
struct HeroPlayerInfo : public HeroIdentity
{
    // Может ли игрок выкупить этого героя в тавернах?
    // offset: +0x8 = +8,  size = 0x4 = 4
    exe_bitset<8> players;

};
#pragma pack(pop)

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
        NH3API_FORCEINLINE
        hero() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(hero)
        { THISCALL_1(void, 0x4D8860, this); }

        NH3API_FORCEINLINE
        hero(const hero& other) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(hero)
        { THISCALL_2(void, 0x406410, this, &other); }

        NH3API_FORCEINLINE
        hero(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : TownSpecialGrantedMask(tag)
        {}

        NH3API_FORCEINLINE
        ~hero() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x45F870, this); }

        NH3API_FORCEINLINE
        hero& operator=(const hero& other) NH3API_NOEXCEPT
        {
            THISCALL_2(void, 0x406410, this, &other);
            return *this;
        }

    public:
        // This hero belongs to human player /
        // Герой принадлежит игроку-человеку.
        NH3API_NODISCARD bool belongs_to_human() const
        { return playerOwner >= 0 && THISCALL_2(bool, 0x4CE600, hero_hpp_gpGame, playerOwner); }

        // Count equipped artifacts /
        // Посчитать количество надетых на героя артефактов (countWarMachines = включая военные машины).
        NH3API_NODISCARD int32_t get_equipped_artifacts(bool countWarMachines) const
        { return THISCALL_2(int32_t, 0x4D92E0, this, countWarMachines); }

        // Count backpack artifacts /
        // Посчитать количество артефактов в рюкзаке (countWarMachines = включая военные машины).
        NH3API_NODISCARD int32_t get_number_in_backpack(bool countWarMachines) const
        { return THISCALL_2(int32_t, 0x4D9330, this, countWarMachines); }

        // Does hero have this artifact? /
        // Есть ли у героя такой артефакт <artifact>(в рюкзаке или на себе)?
        NH3API_NODISCARD bool HasArtifact(TArtifact artifact) const
        { return THISCALL_2(bool, 0x4D9420, this, artifact); }

        // This <artifact> is equipped on hero? /
        // Носит ли герой такой артефакт <artifact>?
        NH3API_NODISCARD bool IsWieldingArtifact(TArtifact artifact) const
        { return THISCALL_2(bool, 0x4D9460, this, artifact); }

        // Add <spell> to spellbook /
        // Добавить заклинание <spell> в книгу заклинаний.
        void AddSpell(SpellID spell)
        { 
            if ( spell > SPELL_NONE && spell < MAX_BOOK_SPELLS)
            {
                in_spellbook[static_cast<size_t>(spell)] = true; 
                available_spells[static_cast<size_t>(spell)] = true;  
            }
        }

        void Deallocate(bool bGameLoaded, bool remote_move)
        { THISCALL_3(void, 0x4DA130, this, bGameLoaded, remote_move); }

        // Get experience points for <iLevel> /
        // Получить соответствующие уровню <iLevel> очки опыта
        NH3API_NODISCARD static int32_t GetExperience(int32_t iLevel)
        { return FASTCALL_1(int32_t, 0x4DA610, iLevel); }

        // Get experience points for upgrading from <iLevel> to <iLevel>+1 /
        // Получить очки опыта для перехода от уровня <iLevel> героя до уровня <iLevel>+1.
        NH3API_NODISCARD static int32_t GetExperienceIncrement(int32_t level)
        { return FASTCALL_1(int32_t, 0x4DA690, level); }

        // First not empty backpack slot /
        // Первый непустой слот рюкзака
        NH3API_NODISCARD int32_t get_last_backpack_index() const
        {
            int32_t i = 64; /*backpack.size()*/
            while ( i-- != 0 )
                if ( backpack[static_cast<size_t>(i)].type != ARTIFACT_NONE )
                    return i;
            return -1;
        }

        NH3API_NODISCARD NH3API_FORCEINLINE
        exe_string get_morale_description() const
        {
            exe_string result(::nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4DC590, this, &result);
            return result;
        }

        NH3API_NODISCARD NH3API_FORCEINLINE
        exe_string get_luck_description() const
        {
            exe_string result(::nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4DCD30, this, &result);
            return result;
        }

        void SetSS(TSecondarySkill iWhichSS, TSkillMastery iLevelToSet)
        { THISCALL_3(void, 0x4E2480, this, iWhichSS, iLevelToSet); }

        int32_t TakeSS(TSecondarySkill iWhichSS, TSkillMastery iNumLevelsToTake)
        { return THISCALL_3(int32_t, 0x4E24C0, this, iWhichSS, iNumLevelsToTake); }

        int32_t GiveSS(TSecondarySkill iWhichSS, TSkillMastery iNumLevelsToGive)
        { return THISCALL_3(int32_t, 0x4E2540, this, iWhichSS, iNumLevelsToGive); }

        NH3API_NODISCARD int32_t CreatureTypeCount(TCreatureType creatureType) const
        { return THISCALL_2(int32_t, 0x4E25B0, this, creatureType); }

        void UpgradeCreatures(TCreatureType sourceCreatureType, TCreatureType destCreatureType)
        { THISCALL_3(void, 0x4E25E0, this, sourceCreatureType, destCreatureType); }

        NH3API_NODISCARD int32_t GetNthSS(int32_t nSS) const
        { return THISCALL_2(int32_t, 0x4E2610, this, nSS); }

        void TransferArtifacts(hero* src)
        { THISCALL_2(void, 0x4E2640, this, src); }

        NH3API_NODISCARD bool artifactAllowedInSlot(TArtifact artifact, TArtifactSlot slot)
        { return THISCALL_3(bool, 0x4E2AB0, this, artifact, slot); }

        bool EquipArtifact(type_artifact& artifact, TArtifactSlot slot)
        { return THISCALL_3(bool, 0x4E2C70, this, &artifact, slot); }

        void remove_artifact(TArtifactSlot slot)
        { THISCALL_2(void, 0x4E2E40, this, slot); }

        void remove_backpack_artifact(TArtifactSlot slot)
        { THISCALL_2(void, 0x4E2FC0, this, slot & 0xFFFF); }

        bool remove_artifact(TArtifact art)
        { return THISCALL_2(bool, 0x4E3040, this, art); }

        NH3API_NODISCARD NH3API_FORCEINLINE
        exe_string get_backpack_error() const
        {
            exe_string result(::nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x4E3140, this, &result);
            return result;
        }

        bool add_to_backpack(const type_artifact& artifact, TArtifactSlot slot)
        { return THISCALL_3(bool, 0x4E3200, this, &artifact, slot); }

        bool GiveArtifact(const type_artifact& artifact, bool assemble_combo, bool equip_it)
        { return THISCALL_4(bool, 0x4E32E0, this, &artifact, assemble_combo, equip_it); }

        int32_t GiveExperience(int32_t howMuch, bool bCheckLevel, bool show_cap_window)
        { return THISCALL_4(int32_t, 0x4E3620, this, howMuch, bCheckLevel, show_cap_window); }

        void GiveResource(EGameResource whichRes, int32_t howMuch)
        { THISCALL_3(void, 0x4E3870, this, whichRes, howMuch); }

        NH3API_NODISCARD int32_t GetLuck(const hero* otherHero, int32_t on_cursed_ground, int32_t apply_limits) const
        { return THISCALL_4(int32_t, 0x4E3930, this, otherHero, on_cursed_ground, apply_limits); }

        NH3API_NODISCARD int32_t GetMorale(const hero* other_hero, bool on_cursed_ground, int32_t apply_limits) const
        { return THISCALL_4(int32_t, 0x4E3C20, this, other_hero, on_cursed_ground, apply_limits); }

        NH3API_NODISCARD TCreatureType getNecromancyCreature() const
        { return THISCALL_1(TCreatureType, 0x4E3ED0, this); }

        NH3API_NODISCARD double GetNecromancyFactor(bool apply_limit) const
        { return THISCALL_2(double, 0x4E3F40, this, apply_limit); }

        NH3API_NODISCARD int32_t GetMysticismBonus() const
        { return THISCALL_1(int32_t, 0x4E41B0, this); }

        NH3API_NODISCARD int32_t GetVisibility() const
        { return THISCALL_1(int32_t, 0x4E42E0, this); }

        NH3API_NODISCARD double GetArcheryFactor() const
        { return THISCALL_1(double, 0x4E43D0, this); }

        NH3API_NODISCARD double GetOffenseFactor() const
        { return THISCALL_1(double, 0x4E4520, this); }

        NH3API_NODISCARD double GetDefenseFactor() const
        { return THISCALL_1(double, 0x4E4580, this); }

        NH3API_NODISCARD int32_t GetEstatesBonus() const
        { return THISCALL_1(int32_t, 0x4E4600, this); }

        NH3API_NODISCARD double GetEagleEyeChance() const
        { return THISCALL_1(double, 0x4E4690, this); }

        NH3API_NODISCARD double GetSurrenderCostFactor() const
        { return THISCALL_1(double, 0x4E47F0, this); }

        NH3API_NODISCARD double GetMagicResistanceFactor() const
        { return THISCALL_1(double, 0x4E4950, this); }

        NH3API_NODISCARD double GetExperienceBonusFactor() const
        { return THISCALL_1(double, 0x4E4AB0, this); }

        NH3API_NODISCARD double GetManaModifier() const
        { return THISCALL_1(double, 0x4E4B20, this); }

        NH3API_NODISCARD double GetFirstAidFactor() const
        { return THISCALL_1(double, 0x4E4B90, this); }

        NH3API_NODISCARD int32_t GetMobility(bool sea_movement) const
        { return THISCALL_2(int32_t, 0x4E4C00, this, sea_movement); }

        NH3API_NODISCARD int32_t GetMobility() const
        { return THISCALL_1(int32_t, 0x4E5000, this); }

        NH3API_NODISCARD int32_t GetSpellDurationBonus() const
        { return THISCALL_1(int32_t, 0x4E5020, this); }

        NH3API_NODISCARD TAdventureObjectType GetGroundModifier() const
        { return THISCALL_1(TAdventureObjectType, 0x4E5130, this); }

        NH3API_NODISCARD int32_t get_special_terrain() const
        { return THISCALL_1(int32_t, 0x4E5210, this); }

        NH3API_NODISCARD TSkillMastery get_spell_level(SpellID spell, int32_t magic_terrain_type) const
        { return THISCALL_3(TSkillMastery, 0x4E52F0, this, spell, magic_terrain_type); }

        NH3API_NODISCARD TSkillMastery GetSpellSchoolLevel(TSpellSchool school_mask, bool is_on_magic_plains) const
        { return THISCALL_3(TSkillMastery, 0x4E5370, this, school_mask, is_on_magic_plains); }

        NH3API_NODISCARD TSpellSchool GetHighestSchool(TSpellSchool school_mask) const
        { return THISCALL_2(TSpellSchool, 0x4E5430, this, school_mask); }

        int32_t GetManaCost(SpellID iWhichSpell, const armyGroup* enemy, int8_t magic_terrain) const
        { return THISCALL_4(int32_t, 0x4E54B0, this, iWhichSpell, enemy, magic_terrain); }

        NH3API_NODISCARD boat* find_summonable_boat()
        { return THISCALL_1(boat*, 0x4E5710, this); }

        NH3API_NODISCARD const boat* find_summonable_boat() const
        { return THISCALL_1(const boat*, 0x4E5710, this); }

        NH3API_NODISCARD bool can_summon_boat() const
        { return THISCALL_1(bool, 0x4E57C0, this); }

        playerData* get_player()
        { return ( this->playerOwner >= 0 ) ? &hero_hpp_gpGame_get(0x20AD0, std::array<playerData, 8>)[static_cast<size_t>(this->playerOwner)] : nullptr; }

        NH3API_NODISCARD const playerData* get_player() const
        { return ( this->playerOwner >= 0 ) ? &hero_hpp_gpGame_get(0x20AD0, std::array<playerData, 8>)[static_cast<size_t>(this->playerOwner)] : nullptr; }
        
        NH3API_NODISCARD bool is_in_patrol_radius(type_point point) const
        { return THISCALL_2(bool, 0x4E5950, this, point); }

        int32_t modify_spell_damage(SpellID spell, int32_t damage, army* target_army) const
        { return THISCALL_4(int32_t, 0x4E59D0, this, spell, damage, target_army); }

        NH3API_NODISCARD int32_t get_combat_speed_bonus() const
        { return THISCALL_1(int32_t, 0x4E5D10, this); }

        NH3API_NODISCARD int32_t get_hit_point_bonus(TCreatureType creature) const
        { return THISCALL_2(int32_t, 0x4E5DF0, this, creature); }

        NH3API_NODISCARD int32_t GetRoguePower() const
        { return THISCALL_1(int32_t, 0x4E6050, this); }

        NH3API_NODISCARD bool IsInIdentifyRange(const type_point& point) const
        { return THISCALL_2(bool, 0x4E6080, this, &point); }

        NH3API_NODISCARD bool IsMobile() const
        { return THISCALL_1(bool, 0x4E61A0, this); }

        NH3API_NODISCARD int32_t GetHeroSpellBonus(SpellID spell_id, int32_t target_level, int32_t value) const
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

        // AI target cell x coordinate /
        // X клетки цели пути ИИ.
        // offset: +0x35 = +53,  size = 0x4 = 4
        int32_t         targetX;

        // AI target cell y coordinate /
        // Y клетки цели пути ИИ.
        // offset: +0x39 = +57,  size = 0x4 = 4
        int32_t         targetY;

        // AI target cell z coordinate /
        // Z клетки цели пути ИИ.
        // offset: +0x3D = +61,  size = 0x2 = 2
        int16_t         targetZ;

        // Last level hero could learn any magic school /
        // Последний уровень героя, при получении которого появилась возможность изучить школу магии.
        // offset: +0x3F = +63,  size = 0x2 = 2
        int16_t         last_magic_school_level;

        // AI target cell distance in MP /
        // Расстояние до клетки цели пути ИИ(в MP).
        // offset: +0x41 = +65,  size = 0x2 = 2
        int16_t         target_distance;

        //
        //
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
        NH3API_MAYBE_UNUSED
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x7F = +127,  size = 0x4 = 4
        uint32_t UniversityFlags;

        NH3API_MAYBE_UNUSED
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x83 = +131,  size = 0x4 = 4
        uint32_t Shrine1Flags;

        NH3API_MAYBE_UNUSED
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x87 = +135,  size = 0x4 = 4
        uint32_t Shrine2Flags;

        NH3API_MAYBE_UNUSED
        // Unused, = 0 /
        // Не используется, = 0.
        // offset: +0x8B = +139,  size = 0x4 = 4
        uint32_t Shrine3Flags;

    public:
        // Hero level up tree seed /
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
        std::array<type_artifact, MAX_ARTIFACT_SLOTS_SOD> equipped;

        // blocked slots by combo artifacts /
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
        std::array<int8_t, kNumPrimarySkills> stats;

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
#pragma pack(pop)

#pragma pack(push, 1)
// Hero extra information(from map file) /
// Информация о герое(из файла карты).
// size = 0x334 = 820, align = 1
struct HeroExtra
{
    public:
        // Player owning this hero /
        // Игрок, обладающий этим героем.
        // offset: +0x0 = +0,  size = 0x1 = 1
        int8_t Owner;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1 = +1,  size = 0x3 = 3
        byte_t gap_1[3];

    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x1A = +26,  size = 0x2 = 2
        byte_t gap_1A[2];

    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x23 = +35,  size = 0x1 = 1
        byte_t gap_23[1];

    public:
        // Number of hero secondary skills /
        // Кол-во вторичных навыков героя.
        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t NumSecondarySkills;

        // Hero secondary skills /
        // Вторичные навыки героя.
        // offset: +0x28 = +40,  size = 0x8 = 8
        std::array<int8_t, 8> secondarySkill;

        // Hero secondary skills levels(masteries) /
        // Уровни вторичного навыка.
        // offset: +0x30 = +48,  size = 0x8 = 8
        std::array<int8_t, 8> secondarySkillLevel;

        // Does hero have custom army edited via map editor?
        // Армия героя настроена в редакторе карт?
        // offset: +0x38 = +56,  size = 0x1 = 1
        int8_t bCustomArmies;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x39 = +57,  size = 0x3 = 3
        byte_t gap_39[3];

    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x307 = +775,  size = 0x1 = 1
        byte_t gap_307[1];

    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x31D = +797,  size = 0x3 = 3
        byte_t gap_31D[3];

    public:
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
        std::array<int8_t, 4> primarySkills;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_32E[3];
};
#pragma pack(pop)

#pragma pack(push, 4)
// Boat /
// Лодка.
// size = 0x28 = 40, align = 4, baseclass: type_obscuring_object
class boat : public type_obscuring_object
{
    public:
        boat() NH3API_NOEXCEPT
            : type_obscuring_object(),
              allocated(false)
        {}

        NH3API_FORCEINLINE
        boat(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}

        void obscure_cell()
        { type_obscuring_object::obscure_cell(OBJECT_BOAT, id); }

    public:
        // Is boat allocated? /
        // Лодка выделена?
        // offset: +0x18 = +24,  size = 0x1 = 1
        bool allocated;

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

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_1D[3];

    public:
        // Hero occupying this boat /
        // Герой, сидящий в лодке.
        // offset: +0x20 = +32,  size = 0x4 = 4
        THeroID occupying_hero;

        // Has hero occupying this boat /
        // Есть герой, сидящий в лодке.
        // offset: +0x24 = +36,  size = 0x1 = 1
        bool occupied;

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_25[3];

};
#pragma pack(pop)

#pragma pack(push, 4)
// Hero placeholder /
// Лагерь героя.
// size = 0x10 = 16, align = 4
struct HeroPlaceholder
{
    public:
        // Map object /
        // Соответствующий объект карты
        // offset: +0x0 = +0,  size = 0x4 = 4
        CObject* object;

        // Player this hero belongs to /
        // Игрок, которому принадлежит этот герой
        // offset: +0x4 = +4,  size = 0x1 = 1
        int8_t player;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x5 = +5,  size = 0x3 = 3
        byte_t gap_5[3];

    public:
        // Hero ID /
        // ID героя.
        // offset: +0x8 = +8,  size = 0x4 = 4
        THeroID id;

        // Hero power /
        // Сила героя.
        // offset: +0xC = +12,  size = 0x1 = 1
        int8_t power;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0xD = +13,  size = 0x3 = 3
        byte_t gap_D[3];

};
#pragma pack(pop)

enum EHeroSpecificAbilityType : uint32_t
{
    SPECIALITY_SKILL = 0,
    SPECIALITY_CREATURE = 1,
    SPECIALITY_RESOURCE = 2,
    SPECIALITY_SPELL    = 3,
    SPECIALITY_CREATURE_UNIVERSAL = 4,
    SPECIALITY_SIR_MULLICH = 5,
    SPECIALITY_CREATURE_UPGRADE = 6,
    SPECIALITY_MUTARE = 7
};

#pragma pack(push, 4)
// Hero speciality traits /
// Специализация героя.
// size = 0x28 = 40, align = 4
struct THeroSpecificAbility
{
    public:
        // Speciality type /
        // Тип специализации.
        // offset: +0x0 = +0,  size = 0x4 = 4
        EHeroSpecificAbilityType specialityType;

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
        int32_t creature_attack_bonus;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t creature_defense_bonus;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t creature_damage_bonus;

        // offset: +0x14 = +20,  size = 0x4 = 4
        TCreatureType creature_grade;

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
        const char* nameFull;

        // Full speciality description /
        // Описание специализации героя.
        // offset: +0x24 = +36,  size = 0x4 = 4
        const char* description;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Hero class traits /
// Свойства класса героев.
// size = 0x40 = 64, align = 4
struct THeroClassTraits
{
    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_3D[3];

};
#pragma pack(pop)

#pragma pack(push, 4)
// Hero traits /
// Свойства героев.
// size = 0x5C = 92, align = 4
struct THeroTraits
{
    public:
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

    protected:
        NH3API_MAYBE_UNUSED
        byte_t gap_3B[1];
        // offset: +0x3C = +60,  size = 0x4 = 4
        uint32_t attributes;

    public:
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

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
// Heroes traits /
// Свойства героев.
std::array<THeroTraits, MAX_HEROES_SOD>& akHeroTraits
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x679DD0, std::array<THeroTraits, MAX_HEROES_SOD>));

NH3API_INLINE_OR_EXTERN
// Heroes classes traits /
// Свойства классов героев.
std::array<THeroClassTraits, kNumHeroClasses>& akHeroClassTraits
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x67D868, std::array<THeroClassTraits, kNumHeroClasses>));

NH3API_INLINE_OR_EXTERN
// Heroes specific abilities /
// Специализации героев.
std::array<THeroSpecificAbility, MAX_HEROES_SOD>& akHeroSpecificAbilities
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x678420, std::array<THeroSpecificAbility, MAX_HEROES_SOD>));

NH3API_INLINE_OR_EXTERN
std::array<int32_t, 20>& move_constants
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698AE8, std::array<int32_t, 20>));

NH3API_DISABLE_WARNING_END
