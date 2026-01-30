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

#include <algorithm>                // std::clamp
#include <array>                    // std::array
#include <memory>                   // std::destroy_at

#include "nh3api_std/exe_deque.hpp" // exe_deque<T>
#include "creatures.hpp"            // TCreatureType, akCreatureTypeTraits, TCreatureTypeTraits
#include "hexcell.hpp"              // hexcell
#include "resources/sounds.hpp"     // gpSoundManager, sample
#include "skills.hpp"               // TSkillMastery
#include "spells.hpp"               // SpellID
#include "utils.hpp"                // bstruct_t, get_bstruct

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)
NH3API_WARNING_MSVC_DISABLE(4582)
NH3API_WARNING_MSVC_DISABLE(4583)

class army;
#define army_hpp_gpCombatManager (reinterpret_cast<void*>(0x699420))
#define army_hpp_gpCombatManager_get(OFFSET, ...) (get_bstruct(army_hpp_gpCombatManager).get<__VA_ARGS__>(OFFSET))

enum TWallTargetId : int32_t
{
    const_no_wall_target    = -1, //
    eTargetUpperTower       = 0,  //
    eTargetUpperWall        = 1,  //
    eTargetMidUpperWall     = 2,  //
    eTargetGate             = 3,  //
    eTargetMidLowerWall     = 4,  //
    eTargetLowerWall        = 5,  //
    eTargetLowerTower       = 6,  //
    eTargetMainBuilding     = 7,  //
    kNumWallTargets         = 8,  //
    const_first_wall_target = 0,  //
};

template<>
struct nh3api::enum_limits<TWallTargetId>
: nh3api::enum_limits_base<TWallTargetId, const_first_wall_target, kNumWallTargets>
{ static inline constexpr bool is_specialized = true; };

#pragma pack(push, 1)
// size = 0x8 = 8, align = 1
struct type_ballistics_traits
{
    // offset: +0x0 = +0,  size = 0x1 = 1
    int8_t chance_to_hit_main_building;

    // offset: +0x1 = +1,  size = 0x1 = 1
    int8_t chance_to_hit_tower;

    // offset: +0x2 = +2,  size = 0x1 = 1
    int8_t chance_to_hit_drawbridge;

    // offset: +0x3 = +3,  size = 0x1 = 1
    int8_t chance_to_hit_wall;

    // offset: +0x4 = +4,  size = 0x1 = 1
    int8_t shots;

    // offset: +0x5 = +5,  size = 0x3 = 3
    std::array<int8_t, 3> chance_to_inflict_damage;

};
#pragma pack(pop) // 1

#pragma pack(push, 4)
// army class: stores information for every troop in a combat. /
// класс army: содержит информацию о каждом существе на поле боя.
// size = 0x548 = 1352, align = 4
class army
{
    public:
        // Sample IDs /
        // ID звуковых эффектов.
        enum TSampleID : int32_t
        {
            WALK_SAMPLE      = 0, // Звук ходьбы
            ATTACK_SAMPLE    = 1, // Звук атаки
            WINCE_SAMPLE     = 2, // Звук кривляний
            SHOOT_SAMPLE     = 3, // Звук выстрела
            DIE_SAMPLE       = 4, // Звук смерти
            DEFEND_SAMPLE    = 5, // Звук получения урона в стойке защиты
            PRE_WALK_SAMPLE  = 6, // Звук начала ходьбы
            POST_WALK_SAMPLE = 7, // Звук окончания ходьбы
            MAX_SAMPLES      = 8  // Количество звуков
        };

        enum
        {
            OFFSET_X = 196,
            OFFSET_Y = 267,
        };

    public:
        inline army() noexcept
            : army(nh3api::dummy_tag)
        { THISCALL_1(void, 0x43CF70, this); }

        inline army(const army& other)
            : army(nh3api::dummy_tag)
        { THISCALL_2(void, 0x437650, this, &other); }

        inline army& operator=(const army& other)
        {
            if ( this != &other )
                THISCALL_2(void, 0x437650, this, &other);

            return *this;
        }

        army(army&& other) noexcept
        {
            std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
            std::memset(static_cast<void*>(&other), 0, sizeof(*this));
        }

        army& operator=(army&& other) noexcept
        {
            if ( this != &other )
            {
                std::destroy_at(&this->SpellInfluenceQueue);
                std::destroy_at(&this->bound_armies);
                std::destroy_at(&this->binders);
                std::destroy_at(&this->aura_clients);
                std::destroy_at(&this->aura_sources);
                std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                std::memset(static_cast<void*>(&other), 0, sizeof(*this));
            }

            return *this;
        }

        inline army(const nh3api::dummy_tag_t& tag) noexcept
            : SpellInfluenceQueue { tag },
              bound_armies { tag },
              binders { tag },
              aura_clients { tag },
              aura_sources { tag }
        {}

        inline ~army() noexcept
        { THISCALL_1(void, 0x43D120, this); }

    public:
        // offset to front(returns either 1 or -1) /
        // поворот вперед(возвращает 1 или -1)
        /// @param direction направление
        [[nodiscard]] int32_t OffsetToFront(int32_t direction) const noexcept
        {
            if ( (-1 < direction) && (direction < 3) )
                return 1;

            if ( (2 < direction) && (direction < 6) )
                return -1;

            return (this->facing != 0) ? 1 : -1;
        }

        // clears fields AI_expected_damage...AI_possible_targets /
        // очищает поля  AI_expected_damage...AI_possible_targets
        void       clear_AI_values() noexcept
        {
            AI_expected_damage  = 0;
            AI_target           = nullptr;
            AI_target_value     = 0;
            AI_target_distance  = 0;
            AI_possible_targets = 0;
        }

        // flag check(for example, this->Is(CF_DOUBLE_WIDE) will return true if the creature is double wide) /
        // проверка флага(напр. this->Is(CF_DOUBLE_WIDE) возвращает true, если существо является широким(2 клетки) )
        /// @param flag число-флаг(используйте | для проверки множества флагов одновременно)
        [[nodiscard]] bool Is(uint32_t flag) const noexcept
        { return this->sMonInfo.flags & flag; }

        [[nodiscard]] int32_t get_AI_expected_damage() const noexcept
        { return this->AI_expected_damage; }

        [[nodiscard]] army* get_AI_target() noexcept
        { return this->AI_target; }

        [[nodiscard]] const army* get_AI_target() const noexcept
        { return this->AI_target; }

        [[nodiscard]] int32_t get_AI_target_value() const noexcept
        { return this->AI_target_value; }

        [[nodiscard]] int32_t get_AI_target_time() const
        { return get_AI_target_time(GetSpeed()); }

        [[nodiscard]] int32_t get_AI_possible_targets() const noexcept
        { return this->AI_possible_targets; }

        // Get initial owning side
        // Сторона, которой принадлежит отряд с начала боя
        [[nodiscard]] int32_t get_owning_side() const noexcept
        { return this->group; }

        // Get current owning side(considering hypn. spell)
        // Сторона, который принадлежит отряд сейчас(учитывая закл. гипноз)
        [[nodiscard]] int32_t get_controlling_side() const noexcept
        { return ( spellInfluence[SPELL_HYPNOTIZE] ) ? 1 - group : group; }

        // Get a specific spell <spell> duration time (in rounds) /
        // Время, в течении которого работает заклинание <spell> (измеряется в раундах)
        [[nodiscard]] int32_t get_spell_time(SpellID spell) const noexcept
        { return (spell >= FIRST_COMBAT_SPELL && spell < MAX_SPELLS) ? spellInfluence[static_cast<size_t>(spell)] : 0; }

        // Is active?
        // Живо ли существо?
        [[nodiscard]] bool IsActive() const noexcept
        { return (armyType > -1) && (numTroops > 0); }

        // Is incapacitated? (Has at least one active spell: Blind, Stone Gaze, Paralyze) /
        // Обездвижено ли существо(Если есть хотя бы одно из этих закл.: Слепота, Окаменение, Паралич)
        [[nodiscard]] bool IsIncapacitated() const noexcept
        {
            return (spellInfluence[SPELL_BLIND] != 0)
                || (spellInfluence[SPELL_STONE] != 0)
                || (spellInfluence[SPELL_PARALYZE] != 0);
        }

        // Can Retaliate to <other> troop? /
        // Может ли существо ответить на атаку существа <other>?
        [[nodiscard]] bool can_retaliate(const army& other) const noexcept
        {
            return !other.Is(CF_FREE_ATTACK) &&
            this->spellInfluence[SPELL_PARALYZE] &&
            (this->retaliationCount > 0);
        }

        // Can't attack? /
        // НЕ может ли существо атаковать?
        [[nodiscard]] bool cannot_attack() const noexcept
        {
            return ( Is(CF_IMMOBILIZED)
                    || IsIncapacitated()
                    || IsWarMachine()
                    || armyType == CREATURE_ARROW_TOWER
                    );
        }

        // Is war machine? /
        // Является боевой машиной
        [[nodiscard]] bool IsWarMachine() const noexcept
        { return Is(CF_SIEGE_WEAPON); }

        // Get an adjacent hex from given <direction> /
        // Гекс, на который смотрит существо по <direction>
        //int32_t    get_adjacent_hex(int32_t direction) const;

        // Get Morale(check the limit[-3;3]: <apply_limits>) /
        // Мораль существа(проверить лимит[-3;3]: <apply_limits>)
        [[nodiscard]] int32_t GetMorale(bool apply_limits = true) const noexcept
        {	if ( !apply_limits ) return iMorale; return std::clamp(iMorale, -3, 3); }

        // Get Luck(check the limit[-3;3]: <apply_limits>) /
        // Удача существа(проверить лимит[-3;3]: <apply_limits>)
        [[nodiscard]] int32_t GetLuck(bool apply_limits = true) const noexcept
        { if ( !apply_limits ) return iLuck; return std::clamp(iLuck, -3, 3); }

        // Get a specific spell <spell> skill mastery level /
        // Получить мастерство определенного наложенного типа заклинания <spell>
        [[nodiscard]] TSkillMastery get_spell_level(SpellID spell) const noexcept
        { return (spell >= FIRST_COMBAT_SPELL && spell < MAX_SPELLS) ? spell_level[static_cast<size_t>(spell)] : eMasteryNone; }

        // Set retalation count in the beginning of battle /
        // Установить количество ответок на атаку в начале битвы
        void       set_retaliation_count() noexcept
        {
            this->retaliationCount = 1;
            if ( this->armyType == CREATURE_GRIFFIN )
                ++this->retaliationCount;
            if ( this->armyType == CREATURE_ROYAL_GRIFFIN )
                this->retaliationCount = 5000;
            if ( this->spellInfluence[SPELL_COUNTERSTRIKE] )
                this->retaliationCount += this->counterstrokeBonus;
            if ( Is(CF_SIEGE_WEAPON) != 0 )
                this->retaliationCount = 0;
        }

        // Play sample <smpl> /
        // Проиграть звуковой эффект <smpl>
        void       play_sample(TSampleID smpl) const
        { if ( smpl >= 0 && smpl < MAX_SAMPLES ) THISCALL_2(void, 0x43D260, this, smpl); }

        // Stop sample <smpl> /
        // Остановить звуковой эффект <smpl>
        void       stop_sample(TSampleID smpl) const
        { if ( smpl >= 0 && smpl < MAX_SAMPLES ) THISCALL_2(void, 0x43D2A0, this, smpl); }

        // Play/Stop/Wait sample <smpl> /
        // Проиграть/остановить/подождать звуковой эффект <smpl>
        void       WaitSample(TSampleID smpl) const
        {
            if ( smpl >= 0 && smpl < MAX_SAMPLES )
                if ( !THISCALL_1(bool, 0x46A080, army_hpp_gpCombatManager) )
                    THISCALL_3(void, 0x59A1C0, 0x699414 /*gpSoundManager*/, armySample[static_cast<size_t>(smpl)]->memSample.memHSample, -1);
        }

        // Luck set depending on battle context /
        // Установка Удачи в зависимости от контекста боя.
        void SetLuck(const hero*      owner_hero,
                     const armyGroup* owner_group,
                     const town*      owner_town,
                     const hero*      other_hero,
                     const armyGroup* other_group,
                     int8_t           ground_type)
        { THISCALL_7(void, 0x43DC40, this, owner_hero, owner_group, owner_town, other_hero, other_group, ground_type); }

        // Morale set depending on battle context /
        // Установка Морали в зависимости от контекста боя.
        void SetMorale(const hero*      owner_hero,
                       const armyGroup* owner_group,
                       const town*      owner_town,
                       const hero*      other_hero,
                       const armyGroup* other_group,
                       int8_t           ground_type,
                       bool             angelic_alliance)
        { THISCALL_8(void, 0x43DD20, this, owner_hero, owner_group, owner_town, other_hero, other_group, ground_type, angelic_alliance); }

        // Draw a creature at cords (<x>;<y>) /
        // Нарисовать существо по координатам (<x>;<y>)
        void       DrawToBuffer(int32_t x, int32_t y, bool32_t bNumBoxOnly)
        { THISCALL_4(void, 0x43DE60, this, x, y, bNumBoxOnly); }

        // Add unicorn aura /
        // Добавить ауру единорога
        void       add_aura()
        { THISCALL_1(void, 0x43E790, this); }

        // Remove unicorn aura /
        // Убрать ауру единорога
        void       remove_aura()
        { THISCALL_1(void, 0x43E970, this); }

        // Remove all dendroid bindings /
        // Убрать все опутывания дендроидов
        void       remove_binding()
        { THISCALL_1(void, 0x43EB30, this); }

        // Walk animation in direction <direction> (called in WalkTo() in a loop for every cell in path) /
        // Проиграть анимацию движения, смотря в сторону <direction> (вызывается в WalkTo() в цикле каждую клетку пути)
        /// @param direction направление
        /// @param end_walk (true/false) конец движения
        /// @param initial_walk (true/false) начало движения
        void       Walk(int32_t direction, bool end_walk, bool initial_walk)
        { THISCALL_4(void, 0x43EDD0, this, direction, end_walk, initial_walk); }

        // Animate shooter and missile from current shooter to <target> /
        // Анимировать стрелка, а также снаряд, летящий от стрелка до <target>
        void       animate_missile(army* target)
        { THISCALL_2(void, 0x43EFE0, this, target); }

        void       range_attack(army* target)
        { THISCALL_2(void, 0x43F620, this, target); }

        void       range_attack()
        { THISCALL_1(void, 0x43FE80, this); }

        [[nodiscard]] int32_t get_clockwise(int32_t direction) const
        {
            if ( direction < 0 || direction >= 8 )
                return -1;

            const uint8_t dir_array_1[8] = { 0, 1, 2, 7, 3, 4, 5, 6 };
            const uint8_t dir_array_2[8] = { 0, 1, 2, 4, 5, 6, 7, 3 };
            if ( Is(CF_DOUBLE_WIDE) )
                return dir_array_1[(dir_array_2[direction] + 1) % 8];
            return (direction + 1) % 6;
        }

        [[nodiscard]] int32_t get_counter_clockwise(int32_t direction) const
        {
            if ( direction < 0 || direction >= 8 )
                return -1;

            const uint8_t dir_array_1[8] = { 0, 1, 2, 7, 3, 4, 5, 6 };
            const uint8_t dir_array_2[8] = { 0, 1, 2, 4, 5, 6, 7, 3 };
            if ( Is(CF_DOUBLE_WIDE) )
                return dir_array_1[(dir_array_2[direction] + 7) % 8];
            return (direction + 5) % 6;
        }

    protected:
        // Multihead attack (called in army::do_attack(army*, int32_t)) /
        // Атака существом с более чем одной головой (вызывается только в army::do_attack(army*, int32_t))
        /// @param attackMask битовая маска атаки
        /// @param damage наносимый урон(заполняется функцией)
        /// @param killed количество убитых существ(заполняется функцией)
        /// @param fire_damage урон огненного щита(заполняется функцией)
        void       do_multi_head_attack(uint32_t attackMask, int32_t* damage, int32_t* killed, int32_t* fire_damage)
        { THISCALL_5(void, 0x440030, this, attackMask, damage, killed, fire_damage); }

        // Check special attack (Wyvern, Mummy, Basilisk, ...) (called in army::do_attack(army*, int32_t)) /
        // Проверка на особую атаку(Виверн, мумии, василиска и т.д.) (вызывается только в army::do_attack(army*, int32_t))
        /// @param target указатель на цель для атаки
        bool       check_special_attack(army* target)
        { return THISCALL_2(bool, 0x440220, this, target); }

        // Fire shield attack (called only in army::do_attack(army*, int32_t)) /
        // Атака огненным щитом (вызывается только в army::do_attack(army*, int32_t))
        /// @param damage урон
        void       do_fire_shield(int32_t damage)
        { THISCALL_2(void, 0x4406E0, this, damage); }

        // Post-attack effect (Vampire Lords, Thunderbirds, ...) (called only in army::do_attack(army*, int32_t)) /
        // Эффект после атаки (лорды вампиры, птицы грома, ...) (вызывается только в army::do_attack(army*, int32_t))
        /// @param target указатель на цель для атаки
        /// @param iDamage нанесённый урон
        /// @param iKilled количество убитых существ
        /// @param total_life сумма HP
        void       do_post_attack(army* target, int32_t iDamage, int32_t iKilled, int32_t total_life)
        { THISCALL_5(void, 0x4408E0, this, target, iDamage, iKilled, total_life); }

    public:
        // Attack <target> in direction <direction> /
        // Атаковать существо <target> в сторону <direction>
        bool       do_attack(army* target, int32_t direction)
        { return THISCALL_3(bool, 0x441330, this, target, direction); }

        void       do_attack(int32_t direction)
        { THISCALL_2(void, 0x4419D0, this, direction); }

        // Check obstacles on attack path /
        // Проверка на предмет препятствий на пути атаки
        /// @param is_walking в данный момент существо идёт
        bool       check_obstacle_attacks(bool is_walking)
        { return armyType != CREATURE_ARROW_TOWER && THISCALL_3(bool, 0x46A150, army_hpp_gpCombatManager, this, is_walking); }

        // Walk to hex <destIndex> /
        // Идти до гекса <destIndex>.
        // Use army::simple_move(hex, restore_facing) instead. / Используйте лучше army::simple_move(hex, restore_facing)
        /// @param destIndex гекс-цель(окончание пути)
        /// @param restore_facing восстановить изначальный поворот
        bool       WalkTo(int32_t destIndex, bool restore_facing)
        { return THISCALL_3(bool, 0x441CC0, this, destIndex, restore_facing); }

        // Get adjusted attack, considering all the effects(Precision, Bloodlust, Slayer, etc..) /
        // Вычисление итоговой атаки, учитывая все заклинания(Меткость, Жажда крови, Палач и др.)
        /// @param enemy указатель на цель для атаки
        /// @param ranged_attack (true/false) является выстрелом
        [[nodiscard]] int32_t get_adjusted_attack(const army* enemy, bool ranged_attack) const
        { return (armyType > CREATURE_NONE && armyType < MAX_CREATURES) ? THISCALL_3(int32_t, 0x442130, this, enemy, ranged_attack) : 0; }

        // AI Attack Modifier   = get_adjusted_attack() - initial attack value /
        // Модификатор атаки ИИ = get_adjusted_attack() - начальная атака существа
        /// @param enemy указатель на цель для атаки
        /// @param ranged_attack (true/false) является выстрелом
        [[nodiscard]] int32_t get_attack_modifier(const army* enemy, bool ranged_attack) const
        { return (armyType > CREATURE_NONE && armyType < MAX_CREATURES) ? (get_adjusted_attack( enemy, ranged_attack ) - akCreatureTypeTraits[static_cast<size_t>(armyType)].attackSkill) : 0; }

        // Get adjusted defense, considering all the effects(Frenzy, Behemoths etc..) /
        // Вычисление итоговой защиты, учитывая Бешенство, Чудищ, Древних Чудищ и др.
        /// @param enemy указатель на цель для атаки
        /// @param frenzy_included учитывать Бешенство
        [[nodiscard]] int32_t get_adjusted_defense(const army* enemy, bool frenzy_included) const
        { return (armyType > CREATURE_NONE && armyType < MAX_CREATURES) ? THISCALL_3(int32_t, 0x4422B0, this, enemy, frenzy_included) : 0; }

        // AI Defense Modifier  = get_adjusted_defense() - initial attack value /
        // Модификатор атаки ИИ = get_adjusted_attack() - начальная атака существа
        [[nodiscard]] int32_t get_defense_modifier() const
        { return (armyType > CREATURE_NONE && armyType < MAX_CREATURES) ? (get_adjusted_defense(nullptr, true) - akCreatureTypeTraits[static_cast<size_t>(armyType)].defenseSkill) : 0; }

        // Get current controller hero pointer (considering hypn. spell) /
        // Указатель на героя, который в данный момент контролирует это существо(учитывая гипноз)
        [[nodiscard]] const hero* get_controller() const noexcept
        {
            if ( group > 1 || group < 0 )
                return nullptr;

            return ( spellInfluence[SPELL_HYPNOTIZE] )
            ?
            army_hpp_gpCombatManager_get(0x53CC, std::array<hero*,2>)[static_cast<size_t>(1 - group)]
            : get_owner();
        }

        // Get current controller hero pointer (considering hypn. spell) /
        // Указатель на героя, который в данный момент контролирует это существо(учитывая гипноз)
        [[nodiscard]] hero* get_controller() noexcept
        {
            if ( group > 1 || group < 0 )
                return nullptr;

            return ( spellInfluence[SPELL_HYPNOTIZE] )
            ?
            army_hpp_gpCombatManager_get(0x53CC, std::array<hero*,2>)[static_cast<size_t>(1 - group)]
            : get_owner();
        }

        // Get initial hero pointer /
        // Указатель на героя, который контролирует существо с начала боя
        [[nodiscard]] hero* get_owner() noexcept
        { return ( group == 1 || group == 0 ) ? army_hpp_gpCombatManager_get(0x53CC, std::array<hero*,2>)[static_cast<size_t>(group)] : nullptr; }

        // Get initial hero pointer /
        // Указатель на героя, который контролирует существо с начала боя
        [[nodiscard]] const hero* get_owner() const noexcept
        { return ( group == 1 || group == 0 ) ? army_hpp_gpCombatManager_get(0x53CC, std::array<hero*,2>)[static_cast<size_t>(group)] : nullptr; }

        // Calculate average damage /
        // Вычислить средний урон
        [[nodiscard]] double get_average_damage() const
        { return THISCALL_1(double, 0x442410, this); }

        // Calculate average damage depending on context /
        // Вычислить средний урон в зависимости от параметров:
        /// @param enemy указатель на цель для атаки
        /// @param ranged_attack (true/false) является выстрелом
        /// @param amount выбранное количество существ
        /// @param limit_damage (true/false) ограничение урона(?) обычно true
        /// @param distance расстояние от текущего отряда до врага
        [[nodiscard]] int32_t get_average_damage(const army& enemy,
                                                 bool        ranged_attack,
                                                 int32_t     amount,
                                                 bool        limit_damage,
                                                 int32_t     distance) const
        { return THISCALL_6(int32_t, 0x4424A0, this, &enemy, ranged_attack, amount, limit_damage, distance); }

        // Is <other> an enemy? /
        // Является ли <other> врагом?
        [[nodiscard]] bool is_enemy(const army* other) const noexcept
        {
            if ( !other || this == other )
                return false;

            if ( this->spellInfluence[SPELL_BERSERK] || other->spellInfluence[SPELL_BERSERK] )
                return true;

            return get_controlling_side() != other->group;
        }

        // Can shoot <other>? If <other> == nullptr, checks if creature can shoot /
        // Можно ли выстрелить в <other>? Если <other> == nullptr, проверяет, может ли существо стрелять.
        [[nodiscard]] bool can_shoot(const army* other = nullptr) const
        { return THISCALL_2(bool, 0x442610, this, other); }

        // Is enemy's hex adjacent? Returns false if <other> is ally. /
        // Враг <other> стоит впритык? Возвращает false, если <other> это не враг
        [[nodiscard]] bool enemy_is_adjacent(const army* other) const
        { return THISCALL_2(bool, 0x442710, this, other); }

        // Get unit total combat value /
        // Посчитать эффективную боевую ценность существа из отряда
        [[nodiscard]] double get_unit_combat_value(int32_t     lowest_attack,
                                                   int32_t     lowest_defense,
                                                   bool        ranged,
                                                   const army* excluded) const
        { return THISCALL_5(double, 0x442770, this, lowest_attack, lowest_defense, ranged, excluded); }

        // Get total combat value /
        // Посчитать эффективную боевую ценность отряда
        [[nodiscard]] int32_t get_total_combat_value(int32_t lowest_attack, int32_t lowest_defense) const
        { return THISCALL_3(int32_t, 0x442B80, this, lowest_attack, lowest_defense); }

        // Get total combat value upon receiving damage /
        // Посчитать эффективную боевую ценность потерь при нанесении урона
        [[nodiscard]] int32_t get_loss_combat_value(int32_t lowest_attack,
                                                    int32_t lowest_defense,
                                                    bool    ranged,
                                                    int32_t damage,
                                                    bool    kills_only) const
        { return THISCALL_6(int32_t, 0x442CF0, this, lowest_attack, lowest_defense, ranged, damage, kills_only); }

        // Get total HP. <simulated> = true in AI's simulated combats /
        // Общее количество очков здоровья. <simulated> = true в боях-симуляциях ИИ
        [[nodiscard]] int32_t get_total_hit_points(bool simulated = false) const
        { return THISCALL_2(int32_t, 0x442DA0, this, simulated); }

        // AI_expected_damage = arg, (AI_expected_damage = 1 if current creature is a clone) /
        // AI_expected_damage = arg, (AI_expected_damage = 1 если существо - клон)
        void       set_AI_expected_damage(int32_t arg)
        { THISCALL_2(void, 0x442DF0, this, arg); }

        // Get Fire shield strength (= 0.2 for Efreet Sultan) /
        // Сила огненного щита (= 0.2 для Ифрита-Султана)
        [[nodiscard]] float get_fire_shield_strength() const
        { return THISCALL_1(float, 0x442E50, this); }

        // Compute base damage (<simulate_only> = true in AI's simulated combats) /
        // Посчитать начальный урон (<simulate_only> = true в боях-симуляциях ИИ)
        [[nodiscard]] int32_t ComputeBaseDamage(bool simulate_only = false) const
        { return THISCALL_2(int32_t, 0x442E80, this, simulate_only); }

        [[nodiscard]] int32_t ComputeAttackerDamageBonuses(int32_t base_damage,
                                                           bool    is_shooting,
                                                           army*   defender,
                                                           bool    simulate_only,
                                                           int32_t distance) const
        { return THISCALL_6(int32_t, 0x443560, this, base_damage, is_shooting, defender, simulate_only, distance); }

        // Compute Attacker <enemy> damage reduction coefficent
        // Вычислить коэффицент уменьшения урона для атакующего <enemy>
        /// @param enemy указатель на цель для атаки
        /// @param ranged_attack (true/false) является выстрелом
        [[nodiscard]] double ComputeAttackerDamageReduction(const army* enemy, bool ranged_attack) const
        { return THISCALL_3(double, 0x4438B0, this, enemy, ranged_attack); }

        // Compute Defender(this) damage reduction coefficent. <ranged_attack> is always passed as true
        // Вычислить коэффицент уменьшения урона для текущего отряда. <ranged_attack> всегда вызывается с true
        [[nodiscard]] double ComputeDefenderDamageReduction(bool ranged_attack) const
        { return THISCALL_2(double, 0x443AB0, this, ranged_attack); }

        // Calculate Adjusted Damage /
        // Посчитать итоговый урон
        /// @param enemy указатель на врага
        /// @param base_damage начальный урон
        /// @param bIsShot (true/false) является выстрелом
        /// @param simulated (true/false) бой-симуляция ИИ
        /// @param distance расстояние до врага в гексах
        /// @param fire_damage указатель на урон огненного щита(заполняется функцией)
        /// @return returns adjusted damage / возвращает итоговый урон
        [[nodiscard]] int32_t adjust_damage(army*    enemy,
                                            int32_t  base_damage,
                                            bool     bIsShot,
                                            bool     simulated,
                                            int32_t  distance,
                                            int32_t* fire_damage) const
        { return THISCALL_7(int32_t, 0x443C60, this, enemy, base_damage, bIsShot, simulated, distance, fire_damage); }

        // Damage enemy /
        // Нанести урон врагу
        /// @param enemy указатель на врага
        /// @param iDamage указатель для урона(заполняется функцией)
        /// @param iKilled количество убитых(заполняется функцией)
        /// @param bIsShot (true/false) является выстрелом
        /// @return returns amount of killed troops / возвращает количество убитых
        int32_t DamageEnemy(army*    enemy,
                            int32_t* iDamage,
                            int32_t* iKilled,
                            bool     bIsShot)
        {
            assert(iDamage && iKilled);
            if ( !enemy )
                return 0;

            int32_t fire_damage = 0;
            *iDamage = this->adjust_damage(enemy, this->ComputeBaseDamage(false), bIsShot, false, this->joustBonus, &fire_damage);
            *iKilled = enemy->Damage(*iDamage);
            this->iLuckStatus = 0;
            return fire_damage;
        }

        // Get damage <damage> /
        // Получить урон <damage>
        /// @param damage получаемый урон
        /// @return returns amount of killed troops / возвращает количество убитых
        int32_t    Damage(int32_t damage)
        { return THISCALL_2(int32_t, 0x443DB0, this, damage); }

        // Cancel Blind, Stone Gaze, Paralyze if <iSpellType> = 2 OR Cancel Berserk if <iSpellType> = 0 or 1 /
        // Отмена заклинаний Слепота, Окаменение, Паралич если <iSpellType> = 2 и отмена Берсерка если <iSpellType> = 0 или 1
        void       CancelSpellType(SpellID iSpellType)
        { THISCALL_2(void, 0x4441F0, this, iSpellType); }

        // Apply Wyvern's Poison damage /
        // Применить урон яда Виверн
        void       adjust_hitpoints()
        {
            sMonInfo.hitPoints = static_cast<int32_t>((spellInfluence[SPELL_AGE] ? static_cast<float>(origHitPoints) * poison_penalty * 0.5f : static_cast<float>(origHitPoints) * poison_penalty) + 0.95f);
            residualDamage     = std::min<int32_t>(residualDamage, sMonInfo.hitPoints - 1);
        }

        // Cancel spell <spell> /
        // Отменить заклинание <spell>
        void       CancelIndividualSpell(SpellID spell)
        { THISCALL_2(void, 0x444230, this, spell); }

        // Cancell all spells /
        // Отменить все заклинания, наложенные на текущий отряд
        void       CancelAllSpells()
        {
            for ( size_t i = FIRST_COMBAT_SPELL; i < MAX_SPELLS; ++i )
            {
                if ( spellInfluence[i] > 0 )
                    CancelIndividualSpell(static_cast<SpellID>(i));
            }
        }

        // Apply spell <spell> with <mastery> and <power> by <casting_hero> /
        // Наложить заклинание <spell> эффективностью <power> ступени <mastery> героем <casting_hero>
        /// @param spell ID заклинания
        /// @param power сила заклинания
        /// @param mastery ступень навыка заклинания
        /// @param casting_hero указатель на героя, который будет кастовать
        void SetSpellInfluence(SpellID       spell,
                               int32_t       power,
                               TSkillMastery mastery,
                               hero const*   casting_hero)
        { THISCALL_5(void, 0x444610, this, spell, power, mastery, casting_hero); }

        // Decrement each spell influence (by round) /
        // Уменьшить время каждого заклинания на один раунд
        // Called each round / Вызывается каждый раунд army::ResetRound()
        void       DecrementSpellRounds()
        {
            for ( size_t i = FIRST_COMBAT_SPELL; i < MAX_SPELLS; ++i )
            {
                if ( spellInfluence[i] > 0 && i != SPELL_FRENZY )
                {
                    if ( spellInfluence[i] == 1 )
                        CancelIndividualSpell(static_cast<SpellID>(i));
                    else
                        --spellInfluence[i];
                }
            }

            if ( iRoundsLeftBeforeVanish > 0 )
                --iRoundsLeftBeforeVanish;
        }

        // Fills vector <armies> with berserk targets /
        // Заполняет vector <armies> с целями берсерка
        /// @param armies vector указателей на существ, который будет заполнен целями берсерка
        void       get_berserk_targets(exe_vector<army*>& armies) const
        { THISCALL_2(void, 0x4451B0, this, &armies); }

        // Go berserk /
        // Стать берсерком
        void       GoBerserk()
        { THISCALL_1(void, 0x4453F0, this); }

        // Get attack direction /
        // Сторона атаки
        /// @param our_hex гекс текущего существа
        /// @param enemy указатель на врага
        /// @param enemy_hex гекс врага
        [[nodiscard]] int32_t get_attack_direction(int32_t our_hex, const army* enemy, int32_t enemy_hex) const
        { return THISCALL_4(int32_t, 0x445560, this, our_hex, enemy, enemy_hex); }

        // Get attack direction /
        // Сторона атаки
        /// @param hex гекс атаки
        /// @param enemy указатель на врага
        [[nodiscard]] int32_t get_attack_direction(int32_t hex, const army* enemy) const
        { return THISCALL_3(int32_t, 0x4455D0, this, hex, enemy); }

        // move to <hex> / идти до гекса <hex>
        /// @param hex гекс-цель(окончание пути)
        /// @param restore_facing восстановить изначальный поворот
        /// @returns returns true, if move is succeed / возвращает true, если движение произошло
        bool       simple_move(int32_t hex, bool restore_facing)
        { return THISCALL_3(bool, 0x445670, this, hex, restore_facing); }

        // Attack <hex> /
        // Атаковать гекс <hex>
        /// @param hex гекс-цель(на который производится атака)
        /// @param restore_facing восстановить изначальный поворот
        /// @returns returns true if attack is succeed / возвращает true если атака успешная
        bool       attack_hex(int32_t hex, bool restore_facing)
        { return THISCALL_3(bool, 0x445780, this, hex, restore_facing); }

        // Just calls simple_move(hex, restore_facing) /
        // Вызывает simple_move(hex, restore_facing)
        bool       move_to(int32_t hex, bool restore_facing)
        { return simple_move(hex, restore_facing); }

        // Cyclops' <iTargetGridIndex> wall attack (called once in combatManager) /
        // Атака стены <iTargetGridIndex> циклопами(вызывается один раз в combatManager)
        void       AttackWall(int32_t iTargetGridIndex)
        { THISCALL_2(void, 0x445A50, this, iTargetGridIndex); }

        // Apply full Cure spell effect /
        // Применить полный эффект заклинания "лечение"
        /// @param level уровень эффекта(ступень навыка Магия Воды)
        /// @param iSpellPower эффективность заклинания
        /// @param casting_hero герой, который накладывает заклинание
        void       Cure(int32_t level, int32_t iSpellPower, hero const * casting_hero);

        [[nodiscard]] int32_t MidY() const noexcept
        { return BottomY() - this->image_height / 2; }

        [[nodiscard]] int32_t TopY() const noexcept
        { return BottomY() - this->image_height; }

        [[nodiscard]] int32_t BottomY() const noexcept
        { return (this->gridIndex > 0 && this->gridIndex < 187) ? army_hpp_gpCombatManager_get(0x1C4, std::array<hexcell, 187>)[static_cast<size_t>(this->gridIndex)].refY : 0; }

        [[nodiscard]] int32_t MidX() const noexcept
        {
            int32_t result = (this->gridIndex > 0 && this->gridIndex < 187) ? army_hpp_gpCombatManager_get(0x1C4, std::array<hexcell, 187>)[static_cast<size_t>(this->gridIndex)].refX : 0;
            if ( Is(CF_DOUBLE_WIDE) )
            {
                result += (this->facing) ? 22 : -22;
            }
            return result;
        }

        [[nodiscard]] int32_t FrontX() const noexcept
        {
            CSpriteFrame* standingStillFrame = stdIcon->s[cs_walk]->f[1];
            assert(standingStillFrame);
            int32_t cropped = standingStillFrame->CroppedX + standingStillFrame->CroppedWidth - OFFSET_X;
            int32_t result = (this->gridIndex > 0 && this->gridIndex < 187) ? army_hpp_gpCombatManager_get(0x1C4, std::array<hexcell, 187>)[static_cast<size_t>(this->gridIndex)].refX : 0;
            result += ( this->facing ) ? cropped : -cropped;
            return result;
        }

        // Get second grid index (Double wide creatures occupy two hexes, functions finds the second one) /
        // Индекс второго гекса(для двойных существ, которые занимают две клетки)
        [[nodiscard]] int32_t get_second_grid_index() const noexcept
        { 	return ( !Is(CF_DOUBLE_WIDE) ) ? gridIndex : gridIndex + ( facing ? 1 : -1 ); }

        // Checks wether <arg> hex is adjacent(considering 8 adj. hexes for double-wide creatures ) /
        // Проверяет, лежит ли гекс <arg> прямо напротив существа(учитываются 8 гексов для двойных существа)
        [[nodiscard]] bool is_adjacent(int32_t arg) const
        { return THISCALL_2(bool, 0x4463E0, this, arg); }

        // Turn /
        // Повернуться
        /// @param play_animation проиграть анимацию
        void       Turn(bool play_animation)
        { THISCALL_2(void, 0x446440, this, play_animation); }

        // Setup animation /
        // Подготовить анимацию
        void       SetupAnimation()
        { THISCALL_1(void, 0x446550, this); }

        // Play animation /
        // Проиграть анимацию
        /// @param sequence Индекс анимации
        /// @param nframes Количество кадров( -1 все кадры ) / ( -1 all frames )
        /// @param start_frame Начальный кадр
        void       PlayAnimation(int32_t sequence, int32_t nframes, int32_t start_frame)
        { THISCALL_4(void, 0x446660, this, sequence, nframes, start_frame); }

        // Can creature fit in <destIndex>? /
        // Может ли существо поместиться в <destIndex>?
        /// @param destIndex гекс-цель
        /// @param bAllowShifting всегда = false / always = false
        /// @param iNewDestIndex указатель на новый гекс(заполняется функцией)
        [[nodiscard]] bool CanFit(int32_t destIndex, int32_t bAllowShifting, int32_t* iNewDestIndex) const
        { return THISCALL_4(bool32_t, 0x446960, this, destIndex, bAllowShifting, iNewDestIndex); }

        // Get resurrected from <target> army size /
        // Посчитать количество возрожденных существ из <target>
        [[nodiscard]] int32_t get_resurrection_size(const army* target) const
        { return THISCALL_2(int32_t, 0x447050, this, target); }

        // can cast resurrect? / can resurrect at least one dead ally? /
        // может ли существо восстанавливать других существ? / восстановит ли существо хотя бы один труп?
        [[nodiscard]] bool can_cast_resurrect(int32_t hex) const
        { return THISCALL_2(bool, 0x4470F0, this, hex); }

        // Cast Faerie dragon spell /
        // Колдование заклинания Сказочным драконом
        void       FaerieDragonSpell()
        { THISCALL_1(void, 0x447230, this); }

        // Can creature cast spell on <hex>?
        // Может ли существо колдовать на гекс <hex>?
        [[nodiscard]] bool can_cast_spell(int32_t hex) const
        { return THISCALL_2(bool, 0x4473E0, this, hex); }

        // Count valid Master Gene spells number that can be casted on <target> /
        // Посчитать количество заклинаний, которые могут быть наложены на <target> Мастер-Джином.
        [[nodiscard]] uint32_t get_valid_caliph_spells(const army* target) const
        {
            uint32_t result = 0;
            for ( size_t i = FIRST_COMBAT_SPELL; i < MAX_COMBAT_SPELLS; ++i )
                if ( FASTCALL_2(bool, 0x447BD0, i, target) )
                    ++result;

            return result;
        }

        // Cast creature's specific spell on <targetIndex> /
        // Наложить заклинание существа на <targetIndex>.
        void       cast_spell(int32_t targetIndex)
        { THISCALL_2(void, 0x447F80, this, targetIndex); }

        // Get Magic mirror effect(taking into account the Faerie Dragons) /
        // Эффект заклинания Магическое зеркало (учитывая Волшебных драконов).
        [[nodiscard]] int32_t get_mirror_effect() const
        { return THISCALL_1(int32_t, 0x448510, this); }

        [[nodiscard]] int32_t get_multi_head_directions(int32_t     our_hex,
                                                        const army* enemy,
                                                        int32_t     enemy_hex) const
        { return THISCALL_4(int32_t, 0x4487D0, this, our_hex, enemy, enemy_hex); }

        // Calculate turns left to reach the AI_target given <speed> /
        // Посчитать, сколько ходов нужно, чтобы достичь AI_target со скоростью <speed>.
        [[nodiscard]] int32_t get_AI_target_time(int32_t speed) const
        { return THISCALL_2(int32_t, 0x4488F0, this, speed); }

        // Calculate current creature speed /
        // Посчитать текущую скорость существа.
        [[nodiscard]] int32_t GetSpeed() const
        { return THISCALL_1(int32_t, 0x4489F0, this); }

        // Need to turn? /
        // Нужно ли развернуться?
        [[nodiscard]] bool NeedToTurn(int32_t direction) const noexcept
        { return direction < 6 && ((this->facing == 0) != (direction >= 3)); }

        // Get creature name /
        // Получить имя существа.
        [[nodiscard]] const char* GetName() const
        { return this->GetName(numTroops); }

        // Get army name given amount <count> /
        // Получить имя существа в зависимости от количества <count>.
        [[nodiscard]] const char* GetName(int32_t count) const
        { return FASTCALL_2(const char*, 0x43FE20, armyType, count); }

        // Get attack direction /
        // Сторона атаки.
        /// @param enemy указатель на врага
        int32_t    get_attack_direction(const army* enemy) const
        { return get_attack_direction( this->gridIndex, enemy ); }

        // Does the creature leaves no body after death? /
        // Существо не оставляет после после смерти труп?
        [[nodiscard]] bool LeavesNoBody() const noexcept
        { return Is(CF_SACRIFICED | CF_SUMMONED); }

        // Creature is within highlighted area /
        // Существо находится внутри площади выделения.
        [[nodiscard]] bool is_in_area_highlight() const noexcept
        { return is_area_effect_target; }

        // Is Flight to <destIndex> valid? /
        // Возможен ли полет на гекс <destIndex>?
        /// @param destIndex проверяемый гекс
        [[nodiscard]] bool ValidFlight(int32_t destIndex) const
        { return THISCALL_3(bool, 0x4B4420, this, destIndex, false); }

        // Fly to <destIndex> /
        // Лететь на гекс <destIndex>.
        /// @param destIndex гекс-цель
        /// @param restore_facing восстановить изначальный поворот
        /// @returns 0
        int32_t    FlyTo(int32_t destIndex, bool restore_facing)
        { return THISCALL_3(int32_t, 0x4B4720, this, destIndex, restore_facing); }

        // Teleport to <destIndex> /
        // Телепортироваться на гекс <destIndex>.
        /// @param destIndex гекс-цель
        /// @param restore_facing восстановить изначальный поворот
        void       TeleportTo(int32_t destIndex, bool restore_facing)
        { THISCALL_3(void, 0x4B4BF0, this, destIndex, restore_facing); }

        // Find path to <fpTargetCellIndex>, which will be written to gpSearchArray /
        // Найти путь до <fpTargetCellIndex>, который будет записан в gpSearchArray.
        /// @param fpTargetCellIndex гекс-цель
        /// @param maxMoves unused / не используется
        /// @param bMoveUnlimited бесконечное движение(= false)
        /// @param bLiteralTarget unused / не используется
        bool       FindPath(int32_t fpTargetCellIndex, int32_t maxMoves, bool bMoveUnlimited, bool bLiteralTarget)
        { return THISCALL_5(bool32_t, 0x523EC0, this, fpTargetCellIndex, maxMoves, bMoveUnlimited, bLiteralTarget); }

        // Is path to <fpTargetCellIndex> valid?
        // Возможен ли путь до <fpTargetCellIndex>?
        /// @param destIndex гекс-цель
        [[nodiscard]] bool ValidPath(int32_t destIndex)
        { return THISCALL_3(bool32_t, 0x523F60, this, destIndex, false); }

        // Get Attack mask
        // Маска атаки
        [[nodiscard]] uint32_t GetAttackMask(int32_t currIndex, int32_t criteria, int32_t iLiteralTargetIndex) const
        { return THISCALL_4(uint32_t, 0x524010, this, currIndex, criteria, iLiteralTargetIndex); }

        [[nodiscard]] bool ValidAttack(int32_t currIndex, int32_t direction, int32_t criteria, int32_t iLiteralIndex, int32_t* testCellIndex) const
        { return THISCALL_6(bool32_t, 0x5240A0, this, currIndex, direction, criteria, iLiteralIndex, testCellIndex); }

        // Call army::get_adjacent_hex( hex, direction ) instead /
        // Вызывайте army::get_adjacent_hex( hex, direction ).
        [[nodiscard]] int32_t GetAdjacentCellIndex(int32_t hex, int32_t direction) const
        { return THISCALL_3(int32_t, 0x524280, this, hex, direction); }

        // Finds adjacent <hex> in <direction> /
        // Находит гекс, лежащий напротив данного гекса <hex> в сторону <direction>.
        [[nodiscard]] int32_t get_adjacent_hex(int32_t hex, int32_t direction) const
        { return THISCALL_3(int32_t, 0x5242E0, this, hex, direction); }

        // Is in Unicorn aura? /
        // Действует ли аура единорога?
        [[nodiscard]] bool is_in_aura() const noexcept
        { return !aura_sources.empty(); }

    public:
        // offset: +0x0 = +0,  size = 0x1 = 1
        bool bShowAttackFrames;

        // offset: +0x1 = +1,  size = 0x1 = 1
        bool bShowRangeFrames;

        // offset: +0x2 = +2,  size = 0x1 = 1
        int8_t iShowAttackFrameType;

        // offset: +0x3 = +3,  size = 0x1 = 1
        int8_t iNextFrameType;

        // offset: +0x4 = +4,  size = 0x1 = 1
        int8_t iRemainingFrames;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x8 = +8,  size = 0x4 = 4
        int32_t iDrawPriority;

        // offset: +0xC = +12,  size = 0x1 = 1
        bool bShowTroopCount;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Combat side(group) to attack /
        // Сторона, которую атакует данное существо
        // offset: +0x10 = +16,  size = 0x4 = 4
        int32_t groupToAttack;

        // Troop index to attack /
        // Индекс существа, которое атакует данное существо
        // offset: +0x14 = +20,  size = 0x4 = 4
        int32_t indexToAttack;

        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t attackLimit;

        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t targetCellIndex;

        // offset: +0x20 = +32,  size = 0x1 = 1
        bool bShowPowEffect;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t iMirrorSourceIndex;

        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t iMirrorDestIndex;

        // Rounds left before creature vanishes /
        // Сколько раундов осталось, прежде чем существо исчезнет.
        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t iRoundsLeftBeforeVanish;

        // Creature is currently moving /
        // Существо двигается в данный момент.
        // offset: +0x30 = +48,  size = 0x1 = 1
        bool IsMoving;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Creature type /
        // Тип существа
        // offset: +0x34 = +52,  size = 0x4 = 4
        TCreatureType armyType;

        // Combat hex of creature(of creature head for double-wide creatures) /
        // Гекс, на котором стоит существо(= гекс головы для двойных существ).
        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t gridIndex;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t currFrameType;

        // Current sprite frame index /
        // Текущий кадр спрайта существа.
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t currFrameIndex;

        // Facing direction /
        // Направление, куда смотрит существо.
        // offset: +0x44 = +68,  size = 0x4 = 4
        int32_t facing;

        // Walk direction /
        // Направление, в которое движется существо.
        // offset: +0x48 = +72,  size = 0x4 = 4
        int32_t walkDirection;

        // offset: +0x4C = +76,  size = 0x4 = 4
        int32_t numTroops;

        // offset: +0x50 = +80,  size = 0x4 = 4
        int32_t numTroopsToShowOverride;

        // offset: +0x54 = +84,  size = 0x4 = 4
        int32_t numTroopsBattleResurrected;

        // offset: +0x58 = +88,  size = 0x4 = 4
        int32_t residualDamage;

        // offset: +0x5C = +92,  size = 0x4 = 4
        int32_t origPos;

        // offset: +0x60 = +96,  size = 0x4 = 4
        int32_t origNumTroops;

        // offset: +0x64 = +100,  size = 0x4 = 4
        int32_t origSpeed;

        // offset: +0x68 = +104,  size = 0x4 = 4
        int32_t origWalkCycleTime;

        // offset: +0x6C = +108,  size = 0x4 = 4
        int32_t origHitPoints;

        // offset: +0x70 = +112,  size = 0x4 = 4
        int32_t iLuckStatus;

        union {
        // Creature general information /
        // Информация о существе.
        // offset: +0x74 = +116,  size = 0x74 = 116
        TCreatureTypeTraits sMonInfo;
        };

        // offset: +0xE8 = +232,  size = 0x1 = 1
        bool show_fire_shield;

        // offset: +0xE9 = +233,  size = 0x1 = 1
        bool bSomeUnitsDamaged;

        // offset: +0xEA = +234,  size = 0x1 = 1
        bool bAllUnitsKilled;

        unsigned char : 8;

        // offset: +0xEC = +236,  size = 0x4 = 4
        SpellID iPostPowSpellToCast;

        // offset: +0xF0 = +240,  size = 0x1 = 1
        bool hitByCreature;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Creature side("group"), 0 or 1 /
        // Сторона("группа"), которой принадлежит существо(0 или 1)
        // offset: +0xF4 = +244,  size = 0x4 = 4
        int32_t group;

        // Creature index, [0..21] /
        // Индекс, который дан существу в "группе" (от 0 до 21).
        // offset: +0xF8 = +248,  size = 0x4 = 4
        int32_t index;

        // offset: +0xFC = +252,  size = 0x4 = 4
        uint32_t iLastFidgetTime;

        // offset: +0x100 = +256,  size = 0x4 = 4
        int32_t ySpecialMod;

        // offset: +0x104 = +260,  size = 0x4 = 4
        int32_t xSpecialMod;

        // offset: +0x108 = +264,  size = 0x4 = 4
        int32_t bPowSequenceComplete;

        // offset: +0x10C = +268,  size = 0x4 = 4
        int32_t yModify;

        union {
        // Creature sprite information /
        // Информация о спрайте существа.
        // offset: +0x110 = +272,  size = 0x54 = 84
        SMonFrameInfo sMonFrameInfo;
        };

        // Creature sprite /
        // Спрайт существа.
        // offset: +0x164 = +356,  size = 0x4 = 4
        CSprite* stdIcon;

        // Creature missile sprite(= nullptr if there are no missiles associated with creature) /
        // Спрайт снаряда существа (= nullptr, если нет ассоциированных снарядов).
        // offset: +0x168 = +360,  size = 0x4 = 4
        CSprite* missileIcon;

        // Creature sprite height /
        // Высота спрайта существа.
        // offset: +0x16C = +364,  size = 0x4 = 4
        int32_t image_height;

        // Creature samples. Use TSampleID for index /
        // Проигрываемые звуковые эффекты. Индексация через enum TSampleID
        // offset: +0x170 = +368,  size = 0x20 = 32
        std::array<sample*, 8> armySample;

        // Creature move order /
        // Порядок хода существа.
        // offset: +0x190 = +400,  size = 0x4 = 4
        uint32_t expected_move_order;

        // Number of casted spells /
        // Кол-во наложенных на существо заклинаний
        // offset: +0x194 = +404,  size = 0x4 = 4
        int32_t numSpellInfluences;

        // Each casted spell time(in rounds) /
        // Количество раундов для каждого наложенного заклинания
        // offset: +0x198 = +408,  size = 0x144 = 324
        std::array<int32_t, MAX_SPELLS> spellInfluence;

        // Each casted spell skill mastery /
        // Уровень навыка каждого наложенного заклинания
        // offset: +0x2DC = +732,  size = 0x144 = 324
        std::array<TSkillMastery, MAX_SPELLS> spell_level;

        union
        {
        // Each casted spell queue /
        // Наложенные на существо заклинания.
        // offset: +0x420 = +1056,  size = 0x30 = 48
        exe_deque<SpellID> SpellInfluenceQueue;
        };

        // offset: +0x450 = +1104,  size = 0x4 = 4
        float PaletteEffect;

        // Number of retaliations
        // Количество ответок(ответных ударов) существа. (=5000 у Королевских грифонов).
        // offset: +0x454 = +1108,  size = 0x4 = 4
        int32_t retaliationCount;

        // offset: +0x458 = +1112,  size = 0x4 = 4
        uint32_t blessFactor;

        // offset: +0x45C = +1116,  size = 0x4 = 4
        uint32_t curseFactor;

        // offset: +0x460 = +1120,  size = 0x4 = 4
        int32_t antiMagicSpellLevel;

        // offset: +0x464 = +1124,  size = 0x4 = 4
        int32_t bloodlustBonus;

        // offset: +0x468 = +1128,  size = 0x4 = 4
        int32_t precisionBonus;

        // offset: +0x46C = +1132,  size = 0x4 = 4
        int32_t weaknessPenalty;

        // offset: +0x470 = +1136,  size = 0x4 = 4
        int32_t toughskinBonus;

        // offset: +0x474 = +1140,  size = 0x4 = 4
        int32_t disruptiverayPenalty;

        // offset: +0x478 = +1144,  size = 0x4 = 4
        int32_t prayerBonus;

        // offset: +0x47C = +1148,  size = 0x4 = 4
        int32_t mirthBonus;

        // offset: +0x480 = +1152,  size = 0x4 = 4
        int32_t sorrowPenalty;

        // offset: +0x484 = +1156,  size = 0x4 = 4
        int32_t fortuneBonus;

        // offset: +0x488 = +1160,  size = 0x4 = 4
        int32_t misfortunePenalty;

        // offset: +0x48C = +1164,  size = 0x4 = 4
        int32_t slayerLevel;

        // offset: +0x490 = +1168,  size = 0x4 = 4
        int32_t joustBonus;

        // offset: +0x494 = +1172,  size = 0x4 = 4
        int32_t counterstrokeBonus;

        // offset: +0x498 = +1176,  size = 0x4 = 4
        float frenzyAdjust;

        // offset: +0x49C = +1180,  size = 0x4 = 4
        float blindFactor;

        // offset: +0x4A0 = +1184,  size = 0x4 = 4
        float fire_shield_strength;

        // offset: +0x4A4 = +1188,  size = 0x4 = 4
        float poison_penalty;

        // offset: +0x4A8 = +1192,  size = 0x4 = 4
        float protectionFromAirFactor;

        // offset: +0x4AC = +1196,  size = 0x4 = 4
        float protectionFromFireFactor;

        // offset: +0x4B0 = +1200,  size = 0x4 = 4
        float protectionFromWaterFactor;

        // offset: +0x4B4 = +1204,  size = 0x4 = 4
        float protectionFromEarthFactor;

        // offset: +0x4B8 = +1208,  size = 0x4 = 4
        float shieldDamageFactor;

        // offset: +0x4BC = +1212,  size = 0x4 = 4
        float airShieldDamageFactor;

        // offset: +0x4C0 = +1216,  size = 0x1 = 1
        bool residualBlindness;

        // offset: +0x4C1 = +1217,  size = 0x1 = 1
        bool residualParalyze;

        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x4C4 = +1220,  size = 0x4 = 4
        TSkillMastery forgetfulness_level;

        // offset: +0x4C8 = +1224,  size = 0x4 = 4
        float slowPenalty;

        // offset: +0x4CC = +1228,  size = 0x4 = 4
        int32_t tailwindBonus;

        // offset: +0x4D0 = +1232,  size = 0x4 = 4
        int32_t diseaseDefensePenalty;

        // offset: +0x4D4 = +1236,  size = 0x4 = 4
        int32_t diseaseAttackPenalty;

        // Creature is on its native terrain /
        // Существо находится на родной для себя земле.
        // offset: +0x4D8 = +1240,  size = 0x1 = 1
        bool OnNativeTerrain;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x4DC = +1244,  size = 0x4 = 4
        int32_t DefendBonus;

        // Randomly choosen faerie dragon spell /
        // Заклинание, случайно выбранное магическим драконом
        // offset: +0x4E0 = +1248,  size = 0x4 = 4
        SpellID faerieDragonSpell;

        // offset: +0x4E4 = +1252,  size = 0x4 = 4
        uint32_t backlash_chance;

        // Morale /
        // Боевой дух.
        // offset: +0x4E8 = +1256,  size = 0x4 = 4
        int32_t iMorale;

        // Luck /
        // Удача.
        // offset: +0x4EC = +1260,  size = 0x4 = 4
        int32_t iLuck;

        // offset: +0x4F0 = +1264,  size = 0x1 = 1
        bool reset_this_round;

        // offset: +0x4F1 = +1265,  size = 0x1 = 1
        bool is_area_effect_target;

        unsigned char : 8;
        unsigned char : 8;

        union {
        // Creatures binded by the current dendroid /
        // опутанные существа данным дендроидом.
        // offset: +0x4F4 = +1268,  size = 0x10 = 16
        exe_vector<army*> bound_armies;
        };

        union {
        // Dendroids that bind the current creature /
        // дендроиды, которые опутали данный отряд.
        // offset: +0x504 = +1284,  size = 0x10 = 16
        exe_vector<army*> binders;
        };

        union {
        // Troops influenced by the unicorn aura /
        // существа под действием ауры данного единорога.
        // offset: +0x514 = +1300,  size = 0x10 = 16
        exe_vector<army*> aura_clients;
        };

        union {
        // Unicorns that cover current troop under aura /
        // единороги, под действием ауры которых данный отряд
        // offset: +0x524 = +1316,  size: 0x10 = 16
        exe_vector<army*> aura_sources;
        };

        // AI expected damage /
        // Ожидаемый ИИ урон.
        // offset: +0x534 = +1332,  size = 0x4 = 4
        int32_t AI_expected_damage;

        // AI target /
        // Цель ИИ.
        // offset: +0x538 = +1336,  size = 0x4 = 4
        army* AI_target;

        // AI target value /
        // значение(value) цели, выбранной ИИ.
        // offset: +0x53C = +1340,  size = 0x4 = 4
        int32_t AI_target_value;

        // AI distance to target /
        // расстояние до цели, выбранной ИИ.
        // offset: +0x540 = +1344,  size = 0x4 = 4
        int32_t AI_target_distance;

        // AI possible targets /
        // возможные цели ИИ.
        // offset: +0x544 = +1348,  size = 0x4 = 4
        int32_t AI_possible_targets;

} NH3API_MSVC_LAYOUT; // class army
#pragma pack(pop) // 4

template<>
struct nh3api::enum_limits<army::TSampleID>
    : nh3api::enum_limits_base<army::TSampleID, army::TSampleID::WALK_SAMPLE, army::TSampleID::POST_WALK_SAMPLE>
{};

NH3API_SIZE_ASSERT(0x548, army);

#undef army_hpp_gpCombatManager
#undef army_hpp_gpCombatManager_get

NH3API_WARNING(pop)
