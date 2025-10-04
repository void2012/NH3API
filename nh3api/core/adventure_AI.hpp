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

#include "resources/resources.hpp" // EGameResource
#include "artifact.hpp"  // type_artifact
#include "skills.hpp"    // TSkillMastery
#include "creatures.hpp" // TCreatureType
#include "spells.hpp"    // SpellID

#pragma pack(push, 8)
// AI Player /
// Игрок ИИ.
// size = 0x98 = 152, align = 8
class type_AI_player
{
    public: 
        NH3API_NODISCARD int32_t get_magus_hut_value() const NH3API_NOEXCEPT
        { return magus_hut_value; }

        void reset_magus_hut_value() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x429AB0, this); }

        NH3API_NODISCARD int32_t get_resource_value(const std::array<int32_t, 7>& cost) const NH3API_NOEXCEPT
        {
            return static_cast<int32_t>(  
                     (cost[WOOD] * this->resource_value[WOOD])
                   + (cost[MERCURY] * this->resource_value[MERCURY])
                   + (cost[ORE] * this->resource_value[ORE])
                   + (cost[SULFUR] * this->resource_value[SULFUR])
                   + (cost[CRYSTAL] * this->resource_value[CRYSTAL])
                   + (cost[GEMS] * this->resource_value[GEMS])
                   + (cost[GOLD] * this->resource_value[GOLD]));
        }

        NH3API_NODISCARD int32_t get_total_value(int32_t basic_value, const std::array<int32_t, 7>& cost) NH3API_NOEXCEPT
        { return THISCALL_3(int32_t, 0x42A150, this, basic_value, &cost); }

        NH3API_NODISCARD static float get_attack_bonus(int16_t player) NH3API_NOEXCEPT
        { return FASTCALL_1(float, 0x428710, player); }

        static void set_attack_bonuses(const float computer_bonus, const float human_bonus) NH3API_NOEXCEPT
        {
            attack_computer_bonus = computer_bonus;
            attack_human_bonus    = human_bonus;
        }

    // static variables
    public:
        NH3API_INLINE_STATIC_VARIABLE
        float& attack_computer_bonus
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x6604F8, float));

        NH3API_INLINE_STATIC_VARIABLE
        float& attack_human_bonus
        NH3API_INLINE_STATIC_VARIABLE_INIT(get_global_var_ref(0x6604FC, float));

    // member variables
    public:
        // Player team /
        // Команда игрока.
        // offset: +0x0 = +0,  size = 0x2 = 2
        int16_t team;

    protected:
        // offset: +0x2 = +2,  size = 0x2 = 2
        NH3API_MAYBE_UNUSED
        byte_t gap_2[2];
    
    public:
        // Seer hut value /
        // Ценность хижины провидца.
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t magus_hut_value;

        // offset: +0x8 = +8,  size = 0x1C = 28
        std::array<int32_t, 7> reserved_funds;

        // offset: +0x24 = +36,  size = 0x1C = 28
        std::array<int32_t, 7> resource_supply;

        // offset: +0x40 = +64,  size = 0x1C = 28
        std::array<int32_t, 7> resource_demand;

    protected:
        // offset: +0x5C = +92,  size = 0x4 = 4
        NH3API_MAYBE_UNUSED 
        byte_t gap_5C[4];

    public:
        // offset: +0x60 = +96,  size = 0x38 = 56
        std::array<double, 7> resource_value;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
std::array<type_AI_player, 8>& AI_player
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6929A0, std::array<type_AI_player, 8>));

enum EArtifactEffectType
{
    ARTIFACT_EFFECT_MIGHT = 0,
    ARTIFACT_EFFECT_POWER = 1,
    ARTIFACT_EFFECT_KNOWLEDGE = 2,
    ARTIFACT_EFFECT_MORALE = 3,
    ARTIFACT_EFFECT_LUCK   = 4,
    ARTIFACT_EFFECT_SCOUTING = 5,
    ARTIFACT_EFFECT_NECROMANCY = 6,
    ARTIFACT_EFFECT_COMBAT = 7,
    ARTIFACT_EFFECT_MOVEMENT = 8,
    ARTIFACT_EFFECT_SPELLCASTER = 9,
    ARTIFACT_EFFECT_DURATION = 10,
    ARTIFACT_EFFECT_SCHOOL   = 11,
    ARTIFACT_EFFECT_TOME     = 12,
    ARTIFACT_EFFECT_ANTIMAGIC = 13,
    ARTIFACT_EFFECT_ANTIMORALE = 14,
    ARTIFACT_EFFECT_ANTILUCK   = 15,
    ARTIFACT_EFFECT_INCOME     = 16,
    ARTIFACT_EFFECT_CREATURE_GROWTH = 17,
    ARTIFACT_EFFECT_SPELL = 18,
    ARTIFACT_EFFECT_SHOOTER_BONUS = 19,
    ARTIFACT_EFFECT_ANGELIC_ALLIANCE = 20,
    ARTIFACT_EFFECT_UNDEAD_KING_CLOAK = 21,
    ARTIFACT_EFFECT_ELIXIR_OF_LIFE = 22,
    ARTIFACT_EFFECT_STATUE_OF_LEGION = 23
};

#pragma pack(push, 4)
// Artifact evaluation class for AI /
// Вспомогательный класс оценки значимости артефакта для ИИ.
// size = 0x4 = 4, align = 4
NH3API_VIRTUAL_CLASS type_artifact_effect
{
    public:
        struct vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(type_artifact_effect*, uint8_t);
            EArtifactEffectType (__thiscall* get_value)(const type_artifact_effect*, const hero*, bool, bool);
        };

    public:
        NH3API_FORCEINLINE
        type_artifact_effect() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_artifact_effect)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_artifact_effect(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        { }

        NH3API_FORCEINLINE
        ~type_artifact_effect() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x432500, this); }

    public:

        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual EArtifactEffectType __thiscall get_value(const hero* owner, bool equipped, bool exact) const = 0;

};

#ifndef NH3API_DECLARE_TYPE_ARTIFACT_EFFECT
#define NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(CLASS, BASE_CLASS) \
NH3API_DEFAULT_DESTRUCTOR(CLASS) \
NH3API_FORCEINLINE \
CLASS(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT \
    : BASE_CLASS(tag) \
{} \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); } \
virtual EArtifactEffectType __thiscall get_value(const hero* owner, bool equipped, bool exact) const override \
{ return get_type_vftable(this)->get_value(this, owner, equipped, exact); }
#endif // NH3API_VIRTUAL_OVERRIDE_BASEMANAGER

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(CLASS, BASE) \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
NH3API_DELEGATE_DUMMY_OR_BASE(CLASS, BASE) \
{ NH3API_SET_VFTABLE(); }
#endif

#if NH3API_STD_DELEGATING_CONSTRUCTORS

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(CLASS, BASE_CLASS, TYPE1, ARG1, VALUE1) \
NH3API_FORCEINLINE \
CLASS(TYPE1 _##ARG1) NH3API_NOEXCEPT \
    :  BASE_CLASS(::nh3api::dummy_tag), ARG1(_##ARG1) \
{ NH3API_SET_VFTABLE(); } \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
    : CLASS(VALUE1) \
{}
#endif

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2(CLASS, BASE_CLASS, TYPE1, ARG1, TYPE2, ARG2, VALUE1, VALUE2) \
NH3API_FORCEINLINE \
CLASS(TYPE1 _##ARG1, TYPE2 _##ARG2) NH3API_NOEXCEPT \
    : BASE_CLASS(::nh3api::dummy_tag), ARG1(_##ARG1), ARG2(_##ARG2) \
{ NH3API_SET_VFTABLE(); } \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
    : CLASS(VALUE1, VALUE2) \
{}
#endif

#else // NH3API_STD_DELEGATING_CONSTRUCTORS

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(CLASS, BASE_CLASS, TYPE1, ARG1, VALUE1) \
NH3API_FORCEINLINE \
CLASS(TYPE1 _##ARG1) NH3API_NOEXCEPT \
    :  BASE_CLASS(::nh3api::dummy_tag), ARG1(_##ARG1) \
{ NH3API_SET_VFTABLE(); } \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
    :  BASE_CLASS(::nh3api::dummy_tag), ARG1(VALUE1) \
{ NH3API_SET_VFTABLE(); } 
#endif

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2(CLASS, BASE_CLASS, TYPE1, ARG1, TYPE2, ARG2, VALUE1, VALUE2) \
NH3API_FORCEINLINE \
CLASS(TYPE1 _##ARG1, TYPE2 _##ARG2) NH3API_NOEXCEPT \
    : BASE_CLASS(::nh3api::dummy_tag), ARG1(_##ARG1), ARG2(_##ARG2) \
{ NH3API_SET_VFTABLE(); } \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
    : BASE_CLASS(::nh3api::dummy_tag), ARG1(VALUE1), ARG2(VALUE2) \
{ NH3API_SET_VFTABLE(); }
#endif

#endif // NH3API_STD_DELEGATING_CONSTRUCTORS

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_scouting_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_scouting_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_scouting_artifact, type_artifact_effect, int32_t, bonus, 0)
        
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t bonus;
};

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_combat_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_combat_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_combat_artifact, type_artifact_effect, int32_t, bonus, 0)
    
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t bonus;

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_movement_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_movement_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_movement_artifact, type_combat_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_spellcaster_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_spellcaster_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_spellcaster_artifact, type_combat_artifact)

};

// size = 0xC = 12, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_tome_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_tome_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_tome_artifact, type_combat_artifact, TSpellSchool, school, const_invalid_school)

    public:
        // offset: +0x8 = +8,  size = 0x4 = 4
        TSpellSchool school;

};

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_antimagic_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_antimagic_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_antimagic_artifact, type_artifact_effect, int32_t, max_level, 0)

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t max_level;

};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_antimorale_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_antimorale_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_antimorale_artifact, type_artifact_effect)

};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_antiluck_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_antiluck_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_antiluck_artifact, type_artifact_effect)

};

// size = 0xC = 12, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_income_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_income_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2(type_income_artifact, type_artifact_effect, int32_t, amount, EGameResource, resource, 0, const_no_resource)

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t amount;

        // offset: +0x8 = +8,  size = 0x4 = 4
        EGameResource resource;

};

// size = 0xC = 12, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_creature_growth_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_creature_growth_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2(type_creature_growth_artifact, type_artifact_effect, int32_t, level, int32_t, bonus, 0, 0)

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t level;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t bonus;

};

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_spell_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_spell_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_spell_artifact, type_artifact_effect, SpellID, spell, SPELL_NONE)

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        SpellID spell;

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_shooter_bonus_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_shooter_bonus_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_shooter_bonus_artifact, type_combat_artifact)

};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_statue_of_legion_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_statue_of_legion_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_statue_of_legion_artifact, type_artifact_effect)

};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_elixir_of_life_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_elixir_of_life_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_elixir_of_life_artifact, type_artifact_effect)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_might_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_might_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_might_artifact, type_combat_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_power_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_power_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_power_artifact, type_combat_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_knowledge_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_knowledge_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_knowledge_artifact, type_combat_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_morale_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_morale_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_morale_artifact, type_combat_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_luck_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_luck_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_luck_artifact, type_combat_artifact)

};

// there is no vftable for this class
// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_base_necromancy_artifact : public type_combat_artifact
{
    public:
        type_base_necromancy_artifact(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT 
            : type_combat_artifact(tag) 
        {} 

        NH3API_FORCEINLINE type_base_necromancy_artifact(int32_t _bonus)
            : type_combat_artifact(_bonus)
        {}

    public:
        void __thiscall scalar_deleting_destructor(uint8_t flag) override 
        { type_combat_artifact::scalar_deleting_destructor(flag); } 

        EArtifactEffectType __thiscall get_value(const hero* owner, bool equipped, bool exact) const override 
        { return type_combat_artifact::get_value(owner, equipped, exact); }        

};

// size = 0x8 = 8, align = 4, baseclass: type_base_necromancy_artifact
NH3API_VIRTUAL_CLASS type_necromancy_artifact : public type_base_necromancy_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_necromancy_artifact, type_base_necromancy_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_necromancy_artifact, type_base_necromancy_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_power_artifact
NH3API_VIRTUAL_CLASS type_duration_artifact : public type_power_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_duration_artifact, type_power_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_duration_artifact, type_power_artifact)

};

// size = 0xC = 12, align = 4, baseclass: type_power_artifact
NH3API_VIRTUAL_CLASS type_school_artifact : public type_power_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_school_artifact, type_power_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_school_artifact, type_power_artifact, TSpellSchool, school, const_invalid_school)

    public:
        // offset: +0x8 = +8,  size = 0x4 = 4
        TSpellSchool school;

};

// size = 0x8 = 8, align = 4, baseclass: type_might_artifact
NH3API_VIRTUAL_CLASS type_angelic_alliance_artifact : public type_might_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_angelic_alliance_artifact, type_might_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_angelic_alliance_artifact, type_might_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_base_necromancy_artifact
NH3API_VIRTUAL_CLASS type_undead_king_cloak_artifact : public type_base_necromancy_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_undead_king_cloak_artifact, type_base_necromancy_artifact)
        NH3API_FORCEINLINE type_undead_king_cloak_artifact() NH3API_NOEXCEPT
            :  type_base_necromancy_artifact(30)
        { NH3API_SET_VFTABLE(); }

};
#pragma pack(pop)

#undef NH3API_DECLARE_TYPE_ARTIFACT_EFFECT
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT

#pragma pack(push, 4)
// size = 0xC = 12, align = 4
struct type_creature_value
{
    public:
        type_creature_value() NH3API_NOEXCEPT
            : type(CREATURE_NONE), value(0), amount(0)
        {}

        type_creature_value(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}

    public:
        bool operator<(const type_creature_value& other) const NH3API_NOEXCEPT
        { return this->value < other.value; }

        bool operator>(const type_creature_value& other) const NH3API_NOEXCEPT
        { return this->value > other.value; }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        TCreatureType type;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t value;

        // offset: +0x8 = +8,  size = 0x2 = 2
        int16_t amount;
        
    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0xA = +10,  size = 0x2 = 2
        byte_t gap_A[2];

};

// size = 0x24 = 36, align = 4
struct type_spellvalue
{
    public:
        NH3API_FORCEINLINE
        type_spellvalue(const hero* new_hero) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_spellvalue)
        { THISCALL_2(void, 0x527220, this, new_hero); }

        NH3API_FORCEINLINE
        type_spellvalue(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : list(tag)
        {}

        NH3API_FORCEINLINE
        ~type_spellvalue() NH3API_NOEXCEPT
        { nh3api::destroy_at(&list); } // note that we have to manually destroy 'list'

    public:
        NH3API_NODISCARD int32_t get_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable, int32_t combat_value) const
        { return THISCALL_5(int32_t, 0x5275C0, this, spell, mastery, times_castable, combat_value); }

        NH3API_NODISCARD int32_t get_mass_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x5276A0, this, spell, mastery, times_castable); }

        NH3API_NODISCARD int32_t get_enchantment_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x527770, this, spell, mastery, times_castable); }

        NH3API_NODISCARD int32_t get_raw_spell_value(SpellID spell) const
        { return THISCALL_2(int32_t, 0x5278B0, this, spell); }

        NH3API_NODISCARD int32_t get_best_spell_value(int32_t bits) const
        { return THISCALL_2(int32_t, 0x527A90, this, bits); }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        const hero* our_hero;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t stack_value;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t power;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t duration;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t mana;

        union {
        // offset: +0x14 = +20,  size = 0x10 = 16
        exe_vector<type_creature_value> list;
        };
        
};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0xC = 12, align = 4
struct HeroDestination
{
public:
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t value;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t move_cost;

    // offset: +0x8 = +8,  size = 0x1 = 1
    bool is_nearby;

    // offset: +0x9 = +9,  size = 0x1 = 1
    bool is_critical;

protected:
    NH3API_MAYBE_UNUSED
    // offset: +0xA = +10,  size = 0x2 = 2
    byte_t gap_A[2];

};
#pragma pack(pop)

NH3API_FORCEINLINE int32_t AI_get_spell_value(const hero* our_hero, SpellID spell)
{ return FASTCALL_2(int32_t, 0x527B20, our_hero, spell); }

class NewmapCell;
NH3API_FORCEINLINE int32_t AI_value_of_combat(const hero* attacking_hero, const hero* defending_hero, const armyGroup& defending_army, const town* defending_town, const NewmapCell* cell)
{ 
    if ( attacking_hero == nullptr || defending_hero == nullptr )
        return -1000000000;
    else
        return FASTCALL_5(int32_t, 0x427330, attacking_hero, defending_hero, &defending_army, defending_town, cell); 
}

NH3API_FORCEINLINE int32_t AI_approximate_strength(const hero* current_hero)
{ return FASTCALL_1(int32_t, 0x427650, current_hero); }

NH3API_FORCEINLINE int32_t AI_approximate_strength(const hero* current_hero, const armyGroup& current_army)
{ return FASTCALL_2(int32_t, 0x427690, current_hero, &current_army); }

NH3API_FORCEINLINE bool can_take_town(const hero* attacking_hero, const town* defending_town)
{ 
    if ( attacking_hero == nullptr || defending_town == nullptr )
        return false;
    else 
        return FASTCALL_2(bool, 0x428410, attacking_hero, defending_town); 
}

NH3API_FORCEINLINE int32_t AI_get_value_of_artifact(type_artifact artifact, const hero* owner, bool equipped, bool exact)
{ return FASTCALL_4(int32_t, 0x4336C0, artifact, owner, equipped, exact); }

NH3API_FORCEINLINE int32_t total_artifact_value(hero* candidate, int32_t player_id)
{ 
    if ( candidate == nullptr || (player_id >= 8 || player_id < 0) )
        return 0;
    else
        return FASTCALL_2(int32_t, 0x4339E0, candidate, player_id); 
}

NH3API_FORCEINLINE int32_t AI_get_value_of_artifact(const type_artifact& artifact, int32_t player_id)
{ 
    if ( (player_id >= 8 || player_id < 0) )
        return 0;
    else
        return FASTCALL_2(int32_t, 0x433AA0, &artifact, player_id); 
}

NH3API_FORCEINLINE double AI_value_of_morale(int32_t morale, int32_t change)
{ return FASTCALL_2(double, 0x435480, morale, change); }

NH3API_FORCEINLINE double value_of_luck_and_morale(int32_t value, int32_t change, double good_value_multiplier, double bad_value_multiplier)
{ return FASTCALL_4(double, 0x4354A0, value, change, good_value_multiplier, bad_value_multiplier); }

NH3API_FORCEINLINE double AI_value_of_luck(int32_t luck, int32_t change)
{ return FASTCALL_2(double, 0x4355B0, luck, change); }

class playerData;
NH3API_FORCEINLINE int32_t AI_resource_cost(const playerData* player, const std::array<int32_t, 7>& resources)
{ return (player) ? FASTCALL_2(int32_t, 0x527150, player, resources.data()) : 0; }

NH3API_FORCEINLINE int32_t AI_resource_cost(int32_t player_id, const std::array<int32_t, 7>& resources)
{ 
    if ( (player_id >= 8 || player_id < 0) )
        return 0;
    else
        return FASTCALL_2(int32_t, 0x5271A0, player_id, resources.data()); 
}

NH3API_FORCEINLINE TSecondarySkill AI_choose_secondary_skill(const hero* our_hero, TSecondarySkill first, TSecondarySkill second, bool complex_choice)
{ return (our_hero) ? FASTCALL_4(TSecondarySkill, 0x52C0B0, our_hero, first, second, complex_choice) : SKILL_NONE; }

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6B0, type_artifact_effect)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6B8, type_scouting_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6C0, type_combat_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6C8, type_might_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6D0, type_power_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6D8, type_knowledge_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6E4, type_necromancy_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6EC, type_movement_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6F4, type_spellcaster_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B6FC, type_morale_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B704, type_luck_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B70C, type_duration_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B714, type_school_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B71C, type_antimagic_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B724, type_antimorale_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B72C, type_antiluck_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B734, type_tome_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B73C, type_income_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B744, type_creature_growth_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B74C, type_spell_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B754, type_shooter_bonus_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B75C, type_angelic_alliance_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B764, type_undead_king_cloak_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B770, type_statue_of_legion_artifact)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B778, type_elixir_of_life_artifact)
