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

#include <array>                   // std::array
#include <memory>                  // std::destroy_at
#include <utility>                 // std::move

#include "artifact.hpp"            // type_artifact
#include "creatures.hpp"           // TCreatureType
#include "resources/resources.hpp" // EGameResource
#include "skills.hpp"              // TSkillMastery
#include "spells.hpp"              // SpellID

NH3API_WARNING(push)
NH3API_WARNING_MSVC_DISABLE(4582) // 'type': constructor is not implicitly called
NH3API_WARNING_MSVC_DISABLE(4583) // 'type': destructor is not implicitly called

#pragma pack(push, 8)
// AI Player /
// Игрок ИИ.
// size = 0x98 = 152, align = 8
class type_AI_player
{
    public:
        [[nodiscard]] int32_t get_magus_hut_value() const noexcept
        { return magus_hut_value; }

        void reset_magus_hut_value() noexcept
        { THISCALL_1(void, 0x429AB0, this); }

        [[nodiscard]] int32_t get_resource_value(const std::array<int32_t, 7>& cost) const noexcept
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

        [[nodiscard]] int32_t get_total_value(int32_t basic_value, const std::array<int32_t, 7>& cost) noexcept
        { return THISCALL_3(int32_t, 0x42A150, this, basic_value, &cost); }

        [[nodiscard]] static float get_attack_bonus(int16_t player)
        { return FASTCALL_1(float, 0x428710, player); }

        static void set_attack_bonuses(float computer_bonus, float human_bonus) noexcept
        {
            attack_computer_bonus = computer_bonus;
            attack_human_bonus    = human_bonus;
        }

    // static variables
    public:
        static inline float& attack_computer_bonus = get_global_var_ref(0x6604F8, float);
        static inline float& attack_human_bonus    = get_global_var_ref(0x6604FC, float);

    // member variables
    public:
        // Player team /
        // Команда игрока.
        // offset: +0x0 = +0,  size = 0x2 = 2
        int16_t team;

        unsigned char : 8;
        unsigned char : 8;

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

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x60 = +96,  size = 0x38 = 56
        std::array<double, 7> resource_value;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

inline std::array<type_AI_player, 8>& AI_player = get_global_var_ref(0x6929A0, std::array<type_AI_player, 8>);

enum EArtifactEffectType : int32_t
{
    ARTIFACT_EFFECT_MIGHT             = 0,
    ARTIFACT_EFFECT_POWER             = 1,
    ARTIFACT_EFFECT_KNOWLEDGE         = 2,
    ARTIFACT_EFFECT_MORALE            = 3,
    ARTIFACT_EFFECT_LUCK              = 4,
    ARTIFACT_EFFECT_SCOUTING          = 5,
    ARTIFACT_EFFECT_NECROMANCY        = 6,
    ARTIFACT_EFFECT_COMBAT            = 7,
    ARTIFACT_EFFECT_MOVEMENT          = 8,
    ARTIFACT_EFFECT_SPELLCASTER       = 9,
    ARTIFACT_EFFECT_DURATION          = 10,
    ARTIFACT_EFFECT_SCHOOL            = 11,
    ARTIFACT_EFFECT_TOME              = 12,
    ARTIFACT_EFFECT_ANTIMAGIC         = 13,
    ARTIFACT_EFFECT_ANTIMORALE        = 14,
    ARTIFACT_EFFECT_ANTILUCK          = 15,
    ARTIFACT_EFFECT_INCOME            = 16,
    ARTIFACT_EFFECT_CREATURE_GROWTH   = 17,
    ARTIFACT_EFFECT_SPELL             = 18,
    ARTIFACT_EFFECT_SHOOTER_BONUS     = 19,
    ARTIFACT_EFFECT_ANGELIC_ALLIANCE  = 20,
    ARTIFACT_EFFECT_UNDEAD_KING_CLOAK = 21,
    ARTIFACT_EFFECT_ELIXIR_OF_LIFE    = 22,
    ARTIFACT_EFFECT_STATUE_OF_LEGION  = 23
};

template<>
struct nh3api::enum_limits<EArtifactEffectType>
    : nh3api::enum_limits_base<EArtifactEffectType, ARTIFACT_EFFECT_MIGHT, ARTIFACT_EFFECT_STATUE_OF_LEGION>
{ static inline constexpr bool is_specialized = true; };

#pragma pack(push, 4)
// Artifact evaluation class for AI /
// Вспомогательный класс оценки значимости артефакта для ИИ.
// size = 0x4 = 4, align = 4
NH3API_VIRTUAL_CLASS type_artifact_effect
{
    public:
        struct vftable_t
        {
            void (__thiscall* scalar_deleting_destructor)(type_artifact_effect*, uint8_t);
            EArtifactEffectType (__thiscall* get_value)(const type_artifact_effect*, const hero*, bool, bool);
        };

    public:
        inline type_artifact_effect() noexcept
            : type_artifact_effect(nh3api::dummy_tag)
        { NH3API_SET_VFTABLE(); }

        inline type_artifact_effect(const nh3api::dummy_tag_t&) noexcept
        {}

        inline ~type_artifact_effect() noexcept
        { THISCALL_1(void, 0x432500, this); }

        type_artifact_effect(const type_artifact_effect&)            = delete;
        type_artifact_effect(type_artifact_effect&&)                 = delete;
        type_artifact_effect& operator=(const type_artifact_effect&) = delete;
        type_artifact_effect& operator=(type_artifact_effect&&)      = delete;

    public:

        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual EArtifactEffectType __thiscall get_value(const hero* owner, bool equipped, bool exact) const = 0;

};

#ifndef NH3API_DECLARE_TYPE_ARTIFACT_EFFECT
#define NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(CLASS_NAME, BASE_CLASS) \
inline ~CLASS_NAME() noexcept = default; \
inline CLASS_NAME(const nh3api::dummy_tag_t& tag) noexcept : BASE_CLASS(tag) {} \
CLASS_NAME(const CLASS_NAME&)            = delete; \
CLASS_NAME(CLASS_NAME&&)                 = delete; \
CLASS_NAME& operator=(const CLASS_NAME&) = delete; \
CLASS_NAME& operator=(CLASS_NAME&&)      = delete; \
void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); } \
EArtifactEffectType __thiscall get_value(const hero* owner, bool equipped, bool exact) const override \
{ return get_type_vftable(this)->get_value(this, owner, equipped, exact); }
#endif // NH3API_VIRTUAL_OVERRIDE_BASEMANAGER

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(CLASS_NAME) \
inline CLASS_NAME() noexcept : CLASS_NAME(nh3api::dummy_tag) { NH3API_SET_VFTABLE(); }
#endif

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1(CLASS_NAME, BASE_CLASS, TYPE1, ARG1, VALUE1) \
inline CLASS_NAME(TYPE1 _##ARG1) noexcept : BASE_CLASS(nh3api::dummy_tag), ARG1(_##ARG1) \
{ NH3API_SET_VFTABLE(); } \
inline CLASS_NAME() noexcept : CLASS_NAME(VALUE1) {}
#endif

#ifndef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2
#define NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2(CLASS_NAME, BASE_CLASS, TYPE1, ARG1, TYPE2, ARG2, VALUE1, VALUE2) \
inline CLASS_NAME(TYPE1 _##ARG1, TYPE2 _##ARG2) noexcept \
    : BASE_CLASS(nh3api::dummy_tag), ARG1(_##ARG1), ARG2(_##ARG2) \
{ NH3API_SET_VFTABLE(); } \
inline CLASS_NAME() noexcept : CLASS_NAME(VALUE1, VALUE2) \
{}
#endif

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

// there is no vftable for this class
// size = 0x8 = 8, align = 4, baseclass: type_combat_artifact
NH3API_VIRTUAL_CLASS type_base_necromancy_artifact : public type_combat_artifact
{
    public:
        type_base_necromancy_artifact(const nh3api::dummy_tag_t& tag) noexcept
            : type_combat_artifact(tag)
        {}

        inline type_base_necromancy_artifact(int32_t _bonus)
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
        NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT(type_angelic_alliance_artifact)

};

// size = 0x8 = 8, align = 4, baseclass: type_base_necromancy_artifact
NH3API_VIRTUAL_CLASS type_undead_king_cloak_artifact : public type_base_necromancy_artifact
{
    public:
        NH3API_DECLARE_TYPE_ARTIFACT_EFFECT(type_undead_king_cloak_artifact, type_base_necromancy_artifact)
        inline type_undead_king_cloak_artifact() noexcept
            :  type_base_necromancy_artifact(30)
        { NH3API_SET_VFTABLE(); }

};

#undef NH3API_DECLARE_TYPE_ARTIFACT_EFFECT
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT1
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT2
#undef NH3API_CONSTRUCTOR_TYPE_ARTIFACT_EFFECT_DEFAULT

// size = 0xC = 12, align = 4
struct type_creature_value
{
    public:
        inline constexpr type_creature_value() noexcept = default;

        inline type_creature_value(const nh3api::dummy_tag_t&) noexcept
        {}

    public:
        inline constexpr bool operator<(const type_creature_value& other) const noexcept
        { return this->value < other.value; }

        inline constexpr bool operator>(const type_creature_value& other) const noexcept
        { return this->value > other.value; }

    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        TCreatureType type {CREATURE_NONE};

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t value {0};

        // offset: +0x8 = +8,  size = 0x2 = 2
        int16_t amount {0};

        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;

// size = 0x24 = 36, align = 4
struct type_spellvalue
{
    public:
        inline type_spellvalue(const hero* new_hero) noexcept
            : type_spellvalue(nh3api::dummy_tag)
        { THISCALL_2(void, 0x527220, this, new_hero); }

        inline type_spellvalue(const nh3api::dummy_tag_t& tag) noexcept
            : list { tag }
        {}

        inline type_spellvalue(const type_spellvalue& other)
            : list { other.list }
        { std::memcpy(reinterpret_cast<void*>(this), &other, __builtin_offsetof(type_spellvalue, list)); }

        inline type_spellvalue& operator=(const type_spellvalue& other)
        {
            std::memcpy(reinterpret_cast<void*>(this), &other, __builtin_offsetof(type_spellvalue, list));
            this->list = other.list;
            return *this;
        }

        inline type_spellvalue(type_spellvalue&& other) noexcept
            : list(std::move(other.list))
        {
            std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
            std::memset(static_cast<void*>(&other), 0, sizeof(*this));
        }

        inline type_spellvalue& operator=(type_spellvalue&& other) noexcept
        {
            if ( this != &other )
            {
                std::destroy_at(&this->list);
                std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                std::memset(static_cast<void*>(&other), 0, sizeof(*this));
            }
            return *this;
        }

        inline ~type_spellvalue() noexcept
        { std::destroy_at(&list); } // note that we have to manually destroy 'list'

    public:
        [[nodiscard]] int32_t get_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable, int32_t combat_value) const
        { return THISCALL_5(int32_t, 0x5275C0, this, spell, mastery, times_castable, combat_value); }

        [[nodiscard]] int32_t get_mass_damage_spell_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x5276A0, this, spell, mastery, times_castable); }

        [[nodiscard]] int32_t get_enchantment_value(SpellID spell, TSkillMastery mastery, int32_t times_castable) const
        { return THISCALL_4(int32_t, 0x527770, this, spell, mastery, times_castable); }

        [[nodiscard]] int32_t get_raw_spell_value(SpellID spell) const
        { return THISCALL_2(int32_t, 0x5278B0, this, spell); }

        [[nodiscard]] int32_t get_best_spell_value(int32_t bits) const
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

// size = 0xC = 12, align = 4
struct HeroDestination
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t value;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t move_cost;

    // offset: +0x8 = +8,  size = 0x1 = 1
    bool is_nearby;

    // offset: +0x9 = +9,  size = 0x1 = 1
    bool is_critical;

    unsigned char : 8;
    unsigned char : 8;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop)

inline int32_t get_artifact_purchase_price(TArtifact artifact, int32_t market_count, EGameResource* best_resource)
{ return FASTCALL_3(int32_t, 0x524A30, artifact, market_count, best_resource); }

inline int32_t get_artifact_purchase_value(TArtifact artifact, int32_t market_count, int32_t* funds)
{ return FASTCALL_3(int32_t, 0x529C30, artifact, market_count, funds); }

inline int32_t AI_get_spell_value(const hero* our_hero, SpellID spell)
{ return FASTCALL_2(int32_t, 0x527B20, our_hero, spell); }

class NewmapCell;
inline int32_t AI_value_of_combat(const hero* attacking_hero, const hero* defending_hero, const armyGroup& defending_army, const town* defending_town, const NewmapCell* cell)
{
    if ( attacking_hero == nullptr || defending_hero == nullptr )
        return -1000000000;
    return FASTCALL_5(int32_t, 0x427330, attacking_hero, defending_hero, &defending_army, defending_town, cell);
}

inline int32_t AI_approximate_strength(const hero* current_hero)
{ return FASTCALL_1(int32_t, 0x427650, current_hero); }

inline int32_t AI_approximate_strength(const hero* current_hero, const armyGroup& current_army)
{ return FASTCALL_2(int32_t, 0x427690, current_hero, &current_army); }

inline bool can_take_town(const hero* attacking_hero, const town* defending_town)
{
    if ( attacking_hero == nullptr || defending_town == nullptr )
        return false;
    return FASTCALL_2(bool, 0x428410, attacking_hero, defending_town);
}

inline int32_t AI_get_value_of_artifact(type_artifact artifact, const hero* owner, bool equipped, bool exact)
{ return FASTCALL_4(int32_t, 0x4336C0, artifact, owner, equipped, exact); }

inline int32_t total_artifact_value(hero* candidate, int32_t player_id)
{
    if ( candidate == nullptr || (player_id >= 8 || player_id < 0) )
        return 0;
    return FASTCALL_2(int32_t, 0x4339E0, candidate, player_id);
}

inline int32_t AI_get_value_of_artifact(const type_artifact& artifact, int32_t player_id)
{
    if ( (player_id >= 8 || player_id < 0) )
        return 0;
    return FASTCALL_2(int32_t, 0x433AA0, &artifact, player_id);
}

inline double AI_value_of_morale(int32_t morale, int32_t change)
{ return FASTCALL_2(double, 0x435480, morale, change); }

inline double value_of_luck_and_morale(int32_t value, int32_t change, double good_value_multiplier, double bad_value_multiplier)
{ return FASTCALL_4(double, 0x4354A0, value, change, good_value_multiplier, bad_value_multiplier); }

inline double AI_value_of_luck(int32_t luck, int32_t change)
{ return FASTCALL_2(double, 0x4355B0, luck, change); }

class playerData;
inline int32_t AI_resource_cost(const playerData* player, const std::array<int32_t, 7>& resources)
{ return (player) ? FASTCALL_2(int32_t, 0x527150, player, resources.data()) : 0; }

inline int32_t AI_resource_cost(int32_t player_id, const std::array<int32_t, 7>& resources)
{
    if ( (player_id >= 8 || player_id < 0) )
        return 0;
    return FASTCALL_2(int32_t, 0x5271A0, player_id, resources.data());
}

inline TSecondarySkill AI_choose_secondary_skill(const hero* our_hero, TSecondarySkill first, TSecondarySkill second, bool complex_choice)
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

NH3API_WARNING(pop)
