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

#include "resources/resources_include.hpp" // EGameResource
#include "skills.hpp" // TSkillMastery
#include "creatures.hpp" // TCreatureType
#include "spells.hpp" // SpellID

#pragma pack(push, 8)
// AI Player /
// Игрок ИИ.
// size = 0x98 = 152, align = 8
class type_AI_player
{
    public:
        // Player team /
        // Команда игрока.
        // offset: +0x0 = +0,  size = 0x2 = 2
        int16_t team;

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

        // offset: +0x5C = +92,  size = 0x38 = 56
        std::array<double, 7> resource_value;

};
#pragma pack(pop)

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
            int32_t (__thiscall* get_value)(const type_artifact_effect*, const hero*, bool, bool);
        };

    public:
        NH3API_FORCEINLINE
        type_artifact_effect() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_artifact_effect)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_artifact_effect(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        { }

    public:

        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall get_value(const hero* owner, bool equipped, bool exact) const = 0;

};

#ifndef NH3API_DECLARE_TYPE_ARTIFACT_EFFECT
#define NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(CLASS, BASE_CLASS, ...) \
NH3API_FORCEINLINE \
CLASS(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT \
    : BASE_CLASS(tag) \
{ NH3API_IGNORE(__VA_ARGS__); } \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); } \
virtual int32_t __thiscall get_value(const hero* owner, bool equipped, bool exact) const override \
{ return get_type_vftable(this)->get_value(this, owner, equipped, exact); }
#endif // NH3API_VIRTUAL_OVERRIDE_BASEMANAGER

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(CLASS, ...) \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
NH3API_DELEGATE_DUMMY(CLASS) \
{ NH3API_SET_VFTABLE(); }
#endif

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(CLASS, BASE_CLASS, TYPE1, ARG1, VALUE1) \
NH3API_FORCEINLINE \
CLASS(TYPE1 _##ARG1) NH3API_NOEXCEPT \
    : ARG1(_##ARG1) NH3API_DELEGATE_DUMMY_COMMA(BASE_CLASS) \
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
    : ARG1(_##ARG1), ARG2(_##ARG2), BASE_CLASS(nh3api::dummy_tag) \
{ NH3API_SET_VFTABLE(); } \
NH3API_FORCEINLINE \
CLASS() NH3API_NOEXCEPT \
    : CLASS(VALUE1, VALUE2) \
{}
#endif

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_scouting_artifact : public type_artifact_effect
{
    public:
    NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_scouting_artifact, type_artifact_effect, bonus)

    NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_scouting_artifact, type_artifact_effect, int32_t, bonus, 0)
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t bonus;
};

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_combat_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_combat_artifact, type_artifact_effect, bonus)
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
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_movement_artifact)
};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_spellcaster_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_spellcaster_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_spellcaster_artifact)
};

// size = 0xC = 12, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_tome_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_tome_artifact, type_combat_artifact, school)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(type_tome_artifact, type_combat_artifact, TSpellSchool, school, const_invalid_school)

    public:
        // offset: +0x8 = +8,  size = 0x4 = 4
        TSpellSchool school;

};

// size = 0x8 = 8, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_antimagic_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_antimagic_artifact, type_artifact_effect, max_level)
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
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_antimorale_artifact)
};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_antiluck_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_antiluck_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_antiluck_artifact)
};

// size = 0xC = 12, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_income_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_income_artifact, type_artifact_effect, amount, resource)
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
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_creature_growth_artifact, type_artifact_effect, level, bonus)
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
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_spell_artifact, type_artifact_effect, spell)
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
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_shooter_bonus_artifact)
};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_statue_of_legion_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_statue_of_legion_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_statue_of_legion_artifact)
};

// size = 0x4 = 4, align = 4, baseclass: type_artifact_effect
NH3API_VIRTUAL_CLASS type_elixir_of_life_artifact : public type_artifact_effect
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_elixir_of_life_artifact, type_artifact_effect)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_elixir_of_life_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_might_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_might_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_might_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_power_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_power_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_power_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_knowledge_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_knowledge_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_knowledge_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_morale_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_morale_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_morale_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_luck_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_luck_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_luck_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_base_necromancy_artifact : public type_combat_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_base_necromancy_artifact, type_combat_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_base_necromancy_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_base_necromancy_artifact
NH3API_VIRTUAL_CLASS type_necromancy_artifact : public type_base_necromancy_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_necromancy_artifact, type_base_necromancy_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_necromancy_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_power_artifact
NH3API_VIRTUAL_CLASS type_duration_artifact : public type_power_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_duration_artifact, type_power_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_duration_artifact)

};

// size = 0xC = 12, align = 4, baseclass: type_power_artifact
NH3API_VIRTUAL_CLASS type_school_artifact : public type_power_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_school_artifact, type_power_artifact, school)
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
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_angelic_alliance_artifact)
};

// size = 0x8 = 8, align = 4, baseclass: type_base_necromancy_artifact
NH3API_VIRTUAL_CLASS type_undead_king_cloak_artifact : public type_base_necromancy_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_undead_king_cloak_artifact, type_base_necromancy_artifact)
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_undead_king_cloak_artifact)

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

        type_creature_value(const nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        { NH3API_IGNORE(type, value, amount); }

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
        type_spellvalue(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : list(tag)
        { NH3API_IGNORE(our_hero, stack_value, power, duration, mana); }

    public:
        int32_t get_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable, int32_t combat_value) const
        { return THISCALL_5(int32_t, 0x5275C0, this, spell, mastery, times_castable, combat_value); }

        int32_t get_mass_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x5276A0, this, spell, mastery, times_castable); }

        int32_t get_enchantment_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x527770, this, spell, mastery, times_castable); }

        int32_t get_raw_spell_value(SpellID spell) const
        { return THISCALL_2(int32_t, 0x5278B0, this, spell); }

        int32_t get_best_spell_value(int32_t bits) const
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

        // offset: +0x14 = +20,  size = 0x10 = 16
        exe_vector<type_creature_value> list;

};
#pragma pack(pop)

NH3API_FORCEINLINE int32_t AI_get_spell_value(const hero* our_hero, SpellID spell)
{ return FASTCALL_2(int32_t, 0x527B20, our_hero, spell); }