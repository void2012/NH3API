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

#include "combat.hpp"

NH3API_NODISCARD NH3API_FORCEINLINE int32_t AI_get_attack_damage(const army& current_army, int32_t our_hits, const army& enemy, bool ranged, int32_t distance)
{ return FASTCALL_5(int32_t, 0x4355D0, &current_army, our_hits, &enemy, ranged, distance); }

#pragma pack(push, 4)
// size = 0x28 = 40, align = 4
class type_AI_combat_parameters
{
    // constructors 
    public:
        NH3API_FORCEINLINE
        type_AI_combat_parameters(const combatManager* combatMgr, int32_t side) NH3API_NOEXCEPT
        { THISCALL_3(void, 0x435B10, this, combatMgr, side); }

        NH3API_FORCEINLINE
        type_AI_combat_parameters(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        { }

    // methods
    public:
    /*
        void simulate_single_attack(const army& current_army, int32_t& our_hits, const army& enemy, int32_t& enemy_hits, bool ranged, int32_t distance)
        {
            const int32_t damage = AI_get_attack_damage(current_army, our_hits, enemy, ranged, distance);

            if ( !ranged )
            {
                const int32_t fireshield_damage = gpCombatManager->compute_fire_shield_damage(damage, &current_army, &enemy, enemy_hits);
                if ( fireshield_damage > 0 )
                {
                    our_hits -= fireshield_damage;
                    if ( our_hits < 0 )
                        our_hits = 0;
                }
            }

            enemy_hits -= damage;
            if ( enemy_hits < 0 )
                enemy_hits = 0;
        }
    */

        void simulate_attack(const army& current_army, int32_t& our_hits, const army& enemy, int32_t& enemy_hits, bool ranged, int32_t distance)
        { THISCALL_7(void, 0x435600, this, &current_army, &our_hits, &enemy, &enemy_hits, ranged, distance); }

        NH3API_NODISCARD int32_t get_simple_attack_effect(const army& current_army, int32_t our_total, const army& enemy, int32_t enemy_total, bool ranged, int32_t distance)
        { return THISCALL_7(int32_t, 0x4357E0, this, &current_army, our_total, &enemy, enemy_total, ranged, distance); }

        NH3API_NODISCARD int32_t get_simple_attack_effect(const army& current_army, const army& enemy, bool ranged, int32_t distance)
        { return THISCALL_5(int32_t, 0x4358C0, this, &current_army, &enemy, ranged, distance); }

        NH3API_NODISCARD int32_t get_ranged_attack_value(const army& current_army, const army& enemy)
        { return THISCALL_3(int32_t, 0x435900, this, &current_army, &enemy); }

        NH3API_NODISCARD int32_t get_exchange_effect(const army& current_army, const army& enemy, int32_t distance)
        { return THISCALL_4(int32_t, 0x435A10, this, &current_army, &enemy, distance); }

    // member variables
    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t lowest_attack;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t lowest_defense;

        // offset: +0x8 = +8,  size = 0x1 = 1
        bool kills_only;

        // offset: +0x9 = +9,  size = 0x1 = 1
        bool simulated;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t friendly_combat_value;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t enemy_combat_value;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t awake_friendly_value;
        
        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t awake_enemy_value;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t rounds_left;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t our_group;
        
        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t enemy_group;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x10 = 16, align = 4
struct type_AI_enemy_data
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    const army* enemy;
    
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t damage;

    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t count;

    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t total_damage;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x14 = 20, align = 4
struct type_enchant_data
{
    // constructors
    public:
        NH3API_CONSTEXPR type_enchant_data(SpellID new_spell, TSkillMastery new_mastery, int32_t new_power, int32_t new_duration) NH3API_NOEXCEPT
            : spell(new_spell),
              mastery(new_mastery),
              power(new_power),
              duration(new_duration),
              check_resistance(true)
        {}

        type_enchant_data(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}
            
    // methods
    public:
        NH3API_NODISCARD int32_t get_mastery_value() const 
        { return THISCALL_1(int32_t, 0x436580, this); }

        NH3API_NODISCARD bool valid() const 
        { return !(spell == SPELL_NONE && mastery == eMasteryNone && power == 0); }

    // member variables
    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        SpellID spell;

        // offset: +0x4 = +4,  size = 0x4 = 4
        TSkillMastery mastery;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t power;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t duration;

        // offset: +0x10 = +16,  size = 0x4 = 4
        bool check_resistance;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x24 = 36, align = 4, baseclass: type_enchant_data
struct type_spell_choice : type_enchant_data
{
    public:
        NH3API_FORCEINLINE
        type_spell_choice() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(type_spell_choice, type_enchant_data)
        { THISCALL_1(void, 0x4365A0, this); }

        NH3API_FORCEINLINE
        type_spell_choice(SpellID new_spell, TSkillMastery new_mastery, int32_t new_power, int32_t new_duration) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(type_spell_choice, type_enchant_data)
        { THISCALL_5(void, 0x4365D0, this, new_spell, new_mastery, new_power, new_duration); }

        NH3API_FORCEINLINE
        type_spell_choice(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_enchant_data(tag)
        {}

    public:
        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t target_hex;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t second_target_hex;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t value;

        // offset: +0x20 = +32,  size = 0x1 = 1
        bool cast_now;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x410 = 1040, align = 4
NH3API_VIRTUAL_CLASS type_AI_spellcaster
{
    public:
        struct vftable_t 
        {
            void (__thiscall *scalar_deleting_destructor)(type_AI_spellcaster*, uint8_t );
        };

    // constructors and destructor
    public:
        NH3API_FORCEINLINE
        type_AI_spellcaster(combatManager* combatMgr, int32_t side, bool creature_spell) NH3API_NOEXCEPT
            : estimate(::nh3api::dummy_tag)
        { THISCALL_4(void, 0x436610, this, combatMgr, side, creature_spell); } 

        NH3API_FORCEINLINE
        type_AI_spellcaster(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : estimate(tag)
        {}

        NH3API_FORCEINLINE
        ~type_AI_spellcaster() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x436880, this); }

    // methods
    public:
        NH3API_NODISCARD bool should_attack_now(const army& enemy) const
        { return THISCALL_2(bool, 0x4368B0, this, &enemy); }

        NH3API_NODISCARD int32_t get_damage_value(SpellID spell, int32_t base_damage, const hero* target_hero, const army* target) const
        { return THISCALL_5(int32_t, 0x436A80, this, spell, base_damage, target_hero, target); }

        NH3API_NODISCARD int32_t get_damage_spell_value(const army* our_army, type_enchant_data caster) const
        { return THISCALL_3(int32_t, 0x436BB0, this, our_army, caster); }

        NH3API_NODISCARD int32_t get_mass_damage_effect(int32_t enemy_damage, int32_t friendly_damage) const
        { return THISCALL_3(int32_t, 0x436C00, this, enemy_damage, friendly_damage); }

        NH3API_NODISCARD int32_t get_area_effect_value(SpellID spell, int32_t base_damage, TSkillMastery mastery, int32_t hex) const
        { return THISCALL_5(int32_t, 0x436C90, this, spell, base_damage, mastery, hex); }

        NH3API_NODISCARD int32_t get_chain_lightning_value(int32_t power, TSkillMastery mastery, army* target) const
        { return THISCALL_4(int32_t, 0x436DE0, this, power, mastery, target); }

        NH3API_NODISCARD int32_t get_attack_skill_value(const army* our_army, const army* enemy, int32_t duration, int32_t bonus) const
        { return THISCALL_5(int32_t, 0x437450, this, our_army, enemy, duration, bonus); }

        NH3API_NODISCARD int32_t get_defense_boost_value(const army* our_army, const army* enemy, int32_t duration, double increase) const
        { return THISCALL_5(int32_t, 0x438410, this, our_army, enemy, duration, increase); }

        NH3API_NODISCARD int32_t get_defense_skill_value(const army* our_army, int32_t duration, int32_t bonus) const
        { return THISCALL_4(int32_t, 0x438560, this, our_army, duration, bonus); }

        NH3API_NODISCARD int32_t get_speed_value(const army* our_army, int32_t increase, int32_t duration) const
        { return THISCALL_4(int32_t, 0x4391A0, this, our_army, increase, duration); }

        NH3API_NODISCARD int32_t get_protection_value(const army* our_army, TSpellSchool school, int32_t level, int32_t duration, int32_t amount) const
        { return FASTCALL_6(int32_t, 0x439330, this, our_army, school, level, duration, amount); }
        
        NH3API_NODISCARD int32_t get_cancel_value(army& current_army, bool bad_spells_only) const
        { return THISCALL_3(int32_t, 0x4396D0, this, &current_army, bad_spells_only); }

        NH3API_NODISCARD int32_t get_traitor_value(const army* enemy, const army* target) const
        { return THISCALL_3(int32_t, 0x439F90, this, enemy, target); }

        void consider_single_enchantment(type_spell_choice& choice, int32_t group) const
        { THISCALL_3(void, 0x43A2C0, this, &choice, group); }

        void consider_enchantment(type_spell_choice& choice, int32_t group) const
        { THISCALL_3(void, 0x43A560, this, &choice, group); }

        void consider_spell(type_spell_choice& choice) const
        { THISCALL_2(void, 0x43B770, this, &choice); }

        void set_melee_enemies()
        { THISCALL_1(void, 0x43BB70, this); }

        void find_enemy_attacks()
        { THISCALL_1(void, 0x43BC90, this); }

        bool cast_spell(bool retreating)
        { return THISCALL_2(bool, 0x43C450, this, retreating); }

    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        hero* current_hero;

        // offset: +0x8 = +8,  size = 0x4 = 4
        hero* enemy_hero;

        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t our_group;

        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t enemy_group;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t enemy_can_attack;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t can_be_attacked;

        // offset: +0x1C = +28,  size = 0x1 = 1
        bool win_likely;

        // offset: +0x1D = +29,  size = 0x1 = 1
        bool is_creature_spell;
        
        // offset: +0x20 = +32,  size = 0x28 = 40
        type_AI_combat_parameters estimate;

        // offset: +0x48 = +72,  size = 0x4 = 4
        type_AI_spellcaster* enemy_caster;

        // offset: +0x4C = +76,  size = 0x1 = 1
        bool owns_enemy_caster;

        // offset: +0x50 = +80,  size = 0x140 = 320
        std::array<type_AI_enemy_data, 20> melee_enemies;

        // offset: +0x190 = +400,  size = 0x140 = 320
        std::array<type_AI_enemy_data, 20> ranged_enemies;

        // offset: +0x2D0 = +720,  size = 0x140 = 320
        std::array<type_AI_enemy_data, 20> worst_enemies;

};
#pragma pack(pop)

// AI combat simulation creature speed category /
// Категории скорости существа при симуляции ИИ.
enum type_speed_category : int32_t 
{
    const_ranged         = 0, // стрельба
    const_very_fast      = 1, // очень быстрая скорость
    const_fast           = 2, // быстрая скорость
    const_average        = 3, // средняя скорость
    const_slow           = 4, // медленная скорость
    const_max_catagories = 5  // кол-во возможных категорий

};

#pragma pack(push, 8)
// size = 0x48 = 72, align = 8
struct type_monster_data
{
    // methods
    public:
        NH3API_NODISCARD int32_t get_enchantment_value(type_spell_choice& choice, const hero* casting_hero, const hero* target_hero) const
        { return THISCALL_4(int32_t, 0x423C80, this, &choice, casting_hero, target_hero); }

        NH3API_NODISCARD int32_t get_resurrection_value(type_spell_choice& choice, const hero* casting_hero) const
        { return THISCALL_3(int32_t, 0x423D00, this, &choice, casting_hero); }

        void cast_resurrection(type_spell_choice& choice, const hero* casting_hero)
        { 
            const int32_t resurrection_value = get_resurrection_value(choice, casting_hero) / this->value;
            this->number += resurrection_value;
            this->total_value += resurrection_value * this->value;
        }

        NH3API_NODISCARD int32_t get_spell_damage(SpellID spell, const hero* casting_hero, const hero* target_hero, int32_t damage) const
        { return THISCALL_5(int32_t, 0x423DE0, this, spell, casting_hero, target_hero, damage); }

        int32_t take_damage(int32_t damage)
        { return THISCALL_2(int32_t, 0x423EA0, this, damage); }

    // comparison operators
    public:
        NH3API_NODISCARD NH3API_CONSTEXPR bool operator<(const type_monster_data& other) const NH3API_NOEXCEPT
        { return this->value < other.value; }

        NH3API_NODISCARD NH3API_CONSTEXPR bool operator>(const type_monster_data& other) const NH3API_NOEXCEPT
        { return this->value > other.value; }

        NH3API_NODISCARD NH3API_CONSTEXPR bool operator>=(const type_monster_data& other) const NH3API_NOEXCEPT
        { return !(*this < other); }

        NH3API_NODISCARD NH3API_CONSTEXPR bool operator<=(const type_monster_data& other) const NH3API_NOEXCEPT
        { return !(*this > other); }

    // member variables
    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t index;
        
        // offset: +0x4 = +4,  size = 0x4 = 4
        TCreatureType type;
        
        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t number;
        
        // offset: +0xC = +12,  size = 0x4 = 4
        int32_t original_number;
        
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t speed;
        
        // offset: +0x18 = +24,  size = 0x8 = 8
        double melee_modifier;
        
        // offset: +0x20 = +32,  size = 0x8 = 8
        double final_melee_modifier;
        
        // offset: +0x28 = +40,  size = 0x8 = 8
        double ranged_modifier;
        
        // offset: +0x30 = +48,  size = 0x8 = 8
        double combat_value_per_hit;
        
        // offset: +0x38 = +56,  size = 0x4 = 4
        type_speed_category category;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t value;
        
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t total_value;

};
#pragma pack(pop)

NH3API_FORCEINLINE bool AI_quick_combat(hero* attacking_hero, hero* defending_hero, armyGroup& defending_army, town* defending_town, NewmapCell* map_cell)
{ return FASTCALL_5(bool, 0x4270C0, attacking_hero, defending_hero, &defending_army, defending_town, map_cell); }

NH3API_FORCEINLINE void AI_auto_combat(hero* attacking_hero, hero* defending_hero, armyGroup& attacking_army, armyGroup& defending_army, const town* defending_town, NewmapCell* cell)
{ FASTCALL_6(void, 0x427210, attacking_hero, defending_hero, &attacking_army, &defending_army, defending_town, cell); }

#pragma pack(push, 4)
// size = 0x34 = 52, align = 4
class type_AI_combat_data
{
    // constructors and destructor
    public: 
        NH3API_FORCEINLINE
        type_AI_combat_data(const hero* new_hero, const armyGroup* new_army, double base_modifier, const hero* _enemy_hero, const town* enemy_town, NewmapCell* map_cell) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_AI_combat_data)
        { THISCALL_7(void, 0x423EE0, this, new_hero, new_army, base_modifier, _enemy_hero, enemy_town, map_cell); }

        NH3API_FORCEINLINE
        type_AI_combat_data(const type_AI_combat_data& that) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_AI_combat_data)
        { THISCALL_2(void, 0x4276C0, this, &that); }

        NH3API_FORCEINLINE
        type_AI_combat_data(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : creatures(tag)
        {}

        NH3API_FORCEINLINE
        ~type_AI_combat_data() NH3API_NOEXCEPT
        { nh3api::destroy_at(&creatures); } // manually destroy 'creatures'

    // methods
    public:
        void adjust_army(bool dismiss_hero)
        { THISCALL_2(void, 0x424880, this, dismiss_hero); }

        NH3API_NODISCARD int32_t get_fastest_speed() const 
        { return THISCALL_1(int32_t, 0x424960, this); }

        NH3API_NODISCARD type_speed_category get_category(TCreatureType creature, int32_t speed) const 
        {
            const TCreatureTypeTraits& traits = get_global_var_ref(0x6703B8, std::array<TCreatureTypeTraits, MAX_COMBAT_CREATURES>)[creature];

            if ( (traits.flags & CF_SHOOTING_ARMY) != 0 )
                return const_ranged;

            type_speed_category category = static_cast<type_speed_category>((14 - 2 * this->tactics_advantage + speed) / speed);
            if ( category > const_slow )
                category = const_slow;
            if ( wall_speed_limit > category && (traits.flags & CF_FLYING_ARMY) == 0 )
                category = static_cast<type_speed_category>(wall_speed_limit);
            return category;
        }

        NH3API_NODISCARD int32_t get_next_chain_lightning_target(int32_t excluded, const type_AI_combat_data& defender, int32_t start, int32_t damage) const
        { return THISCALL_5(int32_t, 0x4249D0, this, excluded, defender, start, damage); }

        void get_area_value(type_spell_choice& choice, const type_AI_combat_data& defender, int32_t damage, int32_t extra_targets) const
        { THISCALL_5(void, 0x424BF0, this, &choice, &defender, damage, extra_targets); }

        void get_damage_spell_value(type_spell_choice& choice, const type_AI_combat_data& defender) const
        { THISCALL_3(void, 0x424D20, this, &choice, defender); }

        void cast_chain_lightning(type_spell_choice& choice, type_AI_combat_data& defender, int32_t damage) const
        { THISCALL_4(void, 0x424FB0, this, &choice, &defender, damage); }

        void cast_area_effect(type_spell_choice& choice, type_AI_combat_data& defender, int32_t damage, int32_t extra_targets) const
        { THISCALL_5(void, 0x425100, this, &choice, &defender, damage, extra_targets); }

        void cast_damage_spell(type_spell_choice& choice, type_AI_combat_data& defender) const
        { THISCALL_3(void, 0x425260, this, &choice, &defender); }

        NH3API_NODISCARD int32_t get_mass_damage_value(type_spell_choice& choice, const hero* casting_hero) const
        { return THISCALL_3(int32_t, 0x4253E0, this, &choice, casting_hero); }

        void get_enchantment_value(type_spell_choice& choice) const
        { THISCALL_2(void, 0x425510, this, &choice); }

        void cast_enchantment(type_spell_choice& choice, const hero* casting_hero, bool increase)
        { THISCALL_4(void, 0x4258A0, this, &choice, casting_hero, increase); }

        void cast_enchantment(type_spell_choice& choice, type_AI_combat_data& defender)
        { THISCALL_3(void, 0x425B10, this, &choice, &defender); }

        void cast_spell(type_AI_combat_data& defender, type_speed_category round)
        { THISCALL_3(void, 0x425BD0, this, &defender, round); }

        void cast_spells(type_AI_combat_data& defender, type_speed_category round)
        {
            if ( this->get_fastest_speed() >= defender.get_fastest_speed() )
            {
                this->cast_spell(defender, round);
                defender.cast_spell(*this, round);
            }
            else  
            {
                defender.cast_spell(*this, round);
                this->cast_spell(defender, round);
            }
        }

        int32_t inflict_melee_damage(int32_t damage, int32_t start, int32_t speed_limit)
        { return THISCALL_4(int32_t, 0x426170, this, damage, start, speed_limit); }

        void kill()
        { THISCALL_1(void, 0x4262B0, this); }

        void inflict_damage(int32_t damage, int32_t blocker_speed)
        { THISCALL_3(void, 0x426300, this, damage, blocker_speed); }

        NH3API_NODISCARD int32_t get_attack(type_speed_category speed_limit, bool shooters_blocked) const
        { return THISCALL_3(int32_t, 0x426390, this, speed_limit, shooters_blocked); }

        NH3API_NODISCARD int32_t get_final_melee_value() const 
        { return THISCALL_1(int32_t, 0x426450, this); }

        void do_ranged_combat(type_AI_combat_data& defender)
        { 
            this->inflict_damage(defender.get_attack(const_ranged, false), const_ranged);
            defender.inflict_damage(this->get_attack(const_ranged, false), const_ranged);
        }

        void do_melee_combat(type_speed_category attacker_speed, type_AI_combat_data& defender)
        { 
            this->inflict_damage(defender.get_attack(const_slow, true), attacker_speed);
            defender.inflict_damage(this->get_attack(attacker_speed, false), const_ranged);
        }

        void do_melee_combat(type_AI_combat_data& defender)
        { 
            this->inflict_damage(defender.get_attack(const_slow, true), const_ranged);
            defender.inflict_damage(this->get_attack(const_slow, true), const_ranged);
        }

        void do_general_melee(type_AI_combat_data& defender)
        { THISCALL_2(void, 0x4264D0, this, &defender); }

        bool choose_melee(const type_AI_combat_data& enemy, type_speed_category current_round)
        { return THISCALL_3(bool, 0x4267C0, this, &enemy, current_round); }

        void simulate_combat(type_AI_combat_data& defender)
        { THISCALL_2(void, 0x426BC0, this, &defender); }

        void do_aftermath(type_AI_combat_data& defender, town* enemy_town)
        { THISCALL_3(void, 0x426EE0, this, &defender, enemy_town); }

        NH3API_NODISCARD int32_t get_total() const
        { return THISCALL_1(int32_t, 0x427750, this); }
    
    // member variables
    public:
        union {
        // offset: +0x0 = +0,  size = 0x10 = 16
        exe_vector<type_monster_data> creatures;
        };

        // offset: +0x10 = +16,  size = 0x4 = 4
        EMagicTerrain magic_terrain;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t mana;
        
        // offset: +0x18 = +24,  size = 0x1 = 1
        bool can_cast_spells;
        
        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t total_combat_value;
        
        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t tactics_advantage;
        
        // offset: +0x24 = +36,  size = 0x4 = 4
        hero* current_hero;
        
        // offset: +0x28 = +40,  size = 0x4 = 4
        const armyGroup* current_army;
        
        // offset: +0x2C = +44,  size = 0x4 = 4
        const hero* enemy_hero;
        
        // offset: +0x30 = +48,  size = 0x1 = 1
        uint8_t wall_archery_penalty;
        
        // offset: +0x32 = +50,  size = 0x2 = 2
        int16_t wall_speed_limit;

};
#pragma pack(pop)

class searchArray;
#pragma pack(push, 4)
// size = 0x2C = 44, align = 4
class type_AI_attack_hex_chooser
{
    // constructors
    public:
        NH3API_FORCEINLINE
        type_AI_attack_hex_chooser(const army* attack_army, const army* enemy_army, const int32_t* enemy_attack_array, searchArray* search_data, const type_AI_combat_parameters& data) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(type_AI_attack_hex_chooser)
        { THISCALL_6(void, 0x435D10, this, attack_army, enemy_army, enemy_attack_array, search_data, &data); }

        NH3API_FORCEINLINE
        type_AI_attack_hex_chooser(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        {}

        NH3API_DEFAULT_DESTRUCTOR(type_AI_attack_hex_chooser)
    
    // methods
    public:
        NH3API_NODISCARD int32_t get_attack_time() const 
        { return best_attack_time; }

        NH3API_NODISCARD int32_t get_best_hex() const 
        { return best_hex; }

        NH3API_NODISCARD int32_t get_hex_value() const 
        { return best_value; }

        NH3API_NODISCARD int32_t get_hex_attack_value(int32_t hex, int32_t& checked)
        { return THISCALL_3(int32_t, 0x435DD0, this, hex, &checked); }

        void check_adjacent_hexes(int32_t enemy_hex, int32_t start_direction, int32_t stop_direction)
        { THISCALL_4(void, 0x435F50, this, enemy_hex, start_direction, stop_direction); }

        bool find_attack_hex()
        { return THISCALL_1(bool, 0x436490, this); }

    // member variables
    public:
        // offset: +0x0 = +0,  size = 0x4 = 4
        const army* attacker;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t speed;

        // offset: +0x8 = +8,  size = 0x4 = 4
        const army* enemy;

        // offset: +0xC = +12,  size = 0x4 = 4
        searchArray* search_array;

        // offset: +0x10 = +16,  size = 0x4 = 4
        const int32_t* enemy_attacks;

        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t retaliation_strength;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t our_strength;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t best_value;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t best_hex;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t best_attack_time;

        // offset: +0x28 = +40,  size = 0x4 = 4
        type_AI_combat_parameters* estimate;

};
#pragma pack(pop)

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B7D8, type_AI_spellcaster)
