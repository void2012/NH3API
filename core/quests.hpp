//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_std/exe_vector.hpp" // exe_vector<T>
#include "nh3api_std/exe_bitset.hpp" // exe_bitset<N>
#include "resources/resources_include.hpp" // TAbstractFile
#include "random.hpp" // exe_rand()
#include "artifact.hpp" // TArtifact
#include "creatures.hpp" // TCreatureType, type_point
#include "hero_enums.hpp" // THeroID
#include "skills.hpp" // TSkillMastery, TSecondarySkill
#include "player_enums.hpp" // EPlayerColor

enum EQuestType : int32_t 
{
    QUEST_NONE             = 0,
    QUEST_EXPERIENCE       = 1,
    QUEST_SKILL            = 2,
    QUEST_DEFEAT_HERO      = 3,
    QUEST_DEFEAT_MONSTER   = 4,
    QUEST_ARTIFACT         = 5,
    QUEST_CREATURE         = 6,
    QUEST_RESOURCE         = 7,
    QUEST_BE_HERO          = 8,
    QUEST_BELONG_TO_PLAYER = 9,
};

class hero;
// Quest abstract class /
// Абстрактный класс - квест.
// size = 0x40 = 64, align = 4
NH3API_VIRTUAL_CLASS type_quest
{
    public:
        struct vftable_t
        {
            public:
            void (__thiscall *scalar_deleting_destructor)(type_quest*, uint8_t);
            int32_t (__thiscall *ai_value)(const type_quest*, int32_t);
            bool (__thiscall *can_complete)(const type_quest*, hero*);
            void (__thiscall *complete)(type_quest*, hero*);
            void (__thiscall *do_progress_dialog)(type_quest*, hero*);
            void (__thiscall *do_proposal_dialog)(type_quest*);
            exe_string* (__thiscall *get_quest_text)(const type_quest*, exe_string*);
            exe_string* (__thiscall *get_help_text)(const type_quest*, exe_string*);
            EQuestType (__thiscall *get_type)(const type_quest*);
            void (__thiscall *hero_defeated)(type_quest*, THeroID, int32_t);
            void (__thiscall *monster_defeated)(type_quest*, type_point, int32_t);
            void (__thiscall *load)(type_quest*, TAbstractFile*, int32_t);
            void (__thiscall *read)(type_quest*, TAbstractFile*);
            void (__thiscall *save)(const type_quest*, TAbstractFile*);
            void (__thiscall *init)(type_quest*);
        };

    public:
        NH3API_FORCEINLINE
        type_quest() NH3API_NOEXCEPT
            : limit(-1), text_variant(exe_rand() % 3)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_quest(bool _seer_hut) NH3API_NOEXCEPT
            : limit(-1), text_variant(exe_rand() % 3), seer_hut(_seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_quest(const nh3api::omit_base_vftable_tag_t&) NH3API_NOEXCEPT
            : limit(-1), text_variant(exe_rand() % 3)
        {}

        NH3API_FORCEINLINE
        type_quest(const nh3api::omit_base_vftable_tag_t&, bool _seer_hut) NH3API_NOEXCEPT
            : limit(-1), text_variant(exe_rand() % 3), seer_hut(_seer_hut)
        {}

        NH3API_FORCEINLINE
        type_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : proposal_text(tag), progress_text(tag), completion_text(tag)
        { NH3API_IGNORE(seer_hut, text_variant, seer_hut); }

    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall ai_value(int32_t player) const
        { return get_type_vftable(this)->ai_value(this, player); }

        // vftable shift: +8
        virtual bool __thiscall can_complete(hero* h) const
        { return get_type_vftable(this)->can_complete(this, h); }

        // vftable shift: +12
        virtual void __thiscall complete(hero* h)
        { get_type_vftable(this)->complete(this, h); }

        // vftable shift: +16
        // Do progress dialog(quest is not yet completed) /
        // Диалог промежуточного выполнения квеста(квест еще не выполнен).
        virtual void __thiscall do_progress_dialog(hero* h)
        { get_type_vftable(this)->do_progress_dialog(this, h); }

        // vftable shift: +20
        // First dialog upon entering /
        // Диалог получения квеста(первое посещение).
        virtual void __thiscall do_proposal_dialog()
        { get_type_vftable(this)->do_proposal_dialog(this); }

        // vftable shift: +24
        // Quest task message /
        // Текст задания квеста.
        virtual exe_string* __thiscall get_quest_text(exe_string * out_str) const
        { return get_type_vftable(this)->get_quest_text(this, out_str); } // ABI compability

        NH3API_FORCEINLINE
        // Quest task message /
        // Текст задания квеста.
        exe_string __thiscall get_quest_text() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) get_quest_text(&result);
            return result;
        } // ABI compability

        // vftable shift: +28
        virtual exe_string* __thiscall get_help_text(exe_string * out_str) const
        { return get_type_vftable(this)->get_help_text(this, out_str); } // ABI compability

        NH3API_FORCEINLINE
        exe_string __thiscall get_help_text() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) get_help_text(&result);
            return result;
        } // ABI compability

        // vftable shift: +32
        virtual EQuestType __thiscall get_type() const
        { return get_type_vftable(this)->get_type(this); }

        // vftable shift: +36
        virtual void __thiscall hero_defeated(THeroID target, int32_t player)
        { get_type_vftable(this)->hero_defeated(this, target, player); }

        // vftable shift: +40
        virtual void __thiscall monster_defeated(type_point target, int32_t player)
        { get_type_vftable(this)->monster_defeated(this, target, player); }

        // vftable shift: +44
        // read from game save /
        // чтение из сохранения.
        virtual void __thiscall load(TAbstractFile* file, int32_t version)
        { get_type_vftable(this)->load(this, file, version); }

        // vftable shift: +48
        // read from map /
        // Чтение из карты
        virtual void __thiscall read(TAbstractFile* file)
        { get_type_vftable(this)->read(this, file); }

        // vftable shift: +52
        // write to game save /
        // запись в сохранение.
        virtual void __thiscall save(TAbstractFile* file) const
        { get_type_vftable(this)->save(this, file); }

        // vftable shift: +56
        // init quest /
        // Инициализировать квест.
        virtual void __thiscall init()
        { get_type_vftable(this)->init(this); }

        NH3API_FORCEINLINE
        exe_string get_completion_text() const 
        { 
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x572FF0, this, &result);
            return result;
        }

    public:
        // Is seer hut /
        // Хижина провидца
        // offset: +0x4 = +4,  size = 0x1 = 1
        bool seer_hut;

        // Initial proposal text /
        // Текст, всплывающий при первом получении игроком квеста
        // offset: +0x8 = +8,  size = 0x10 = 16
        exe_string proposal_text;

        // Progress text /
        // Текст, всплывающий при повторном посещении игроком текста(игрок еще не выполнил квест).
        // offset: +0x18 = +24,  size = 0x10 = 16
        exe_string progress_text;

        // Completion text /
        // Текст, всплывающий при завершении квеста(вознаграждение или пропуск).
        // offset: +0x28 = +40,  size = 0x10 = 16
        exe_string completion_text;

        // Random text variant(0,1,2) /
        // Случайная вариация текста(0,1,2).
        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t text_variant;

        // Quest duration limit(in days) /
        // Время, в течение которого необходимо выполнить квест(в днях).
        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t limit;

};

#ifndef NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST
#define NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(reinterpret_cast<CLASS_NAME*>(this), flag); } \
virtual int32_t __thiscall ai_value(int32_t player) const override \
{ return get_type_vftable(this)->ai_value(reinterpret_cast<const CLASS_NAME*>(this), player); } \
virtual bool __thiscall can_complete(hero* h) const override \
{ return get_type_vftable(this)->can_complete(reinterpret_cast<const CLASS_NAME*>(this), h); } \
virtual void __thiscall complete(hero* h) override \
{ get_type_vftable(this)->complete(reinterpret_cast<CLASS_NAME*>(this), h); } \
virtual void __thiscall do_progress_dialog(hero* h) override \
{ get_type_vftable(this)->do_progress_dialog(reinterpret_cast<CLASS_NAME*>(this), h); } \
virtual void __thiscall do_proposal_dialog() override \
{ get_type_vftable(this)->do_proposal_dialog(reinterpret_cast<CLASS_NAME*>(this)); } \
virtual exe_string* __thiscall get_quest_text(exe_string* out_str) const override \
{ return get_type_vftable(this)->get_quest_text(reinterpret_cast<const CLASS_NAME*>(this), out_str); } \
virtual exe_string* __thiscall get_help_text(exe_string* out_str) const override \
{ return get_type_vftable(this)->get_help_text(reinterpret_cast<const CLASS_NAME*>(this), out_str); } \
virtual EQuestType __thiscall get_type() const override \
{ return get_type_vftable(this)->get_type(reinterpret_cast<const CLASS_NAME*>(this)); } \
virtual void __thiscall hero_defeated(THeroID target, int32_t player) override \
{ get_type_vftable(this)->hero_defeated(reinterpret_cast<CLASS_NAME*>(this), target, player); } \
virtual void __thiscall monster_defeated(type_point target, int32_t player) override \
{ get_type_vftable(this)->monster_defeated(reinterpret_cast<CLASS_NAME*>(this), target, player); } \
virtual void __thiscall load(TAbstractFile* file, int32_t version) override \
{ get_type_vftable(this)->load(reinterpret_cast<CLASS_NAME*>(this), file, version); } \
virtual void __thiscall read(TAbstractFile* file) override \
{ get_type_vftable(this)->read(reinterpret_cast<CLASS_NAME*>(this), file); } \
virtual void __thiscall save(TAbstractFile* file) const override \
{ get_type_vftable(this)->save(reinterpret_cast<const CLASS_NAME*>(this), file); } \
virtual void __thiscall init() override \
{ get_type_vftable(this)->init(reinterpret_cast<CLASS_NAME*>(this)); }
#endif // NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST

#pragma pack(push, 4)
// Hero belonging to player requirement quest /
// Квест, для выполнения которого необходимо быть героем определённого игрока.
// size = 0x44 = 68, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_belong_to_player_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_belong_to_player_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_belong_to_player_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_belong_to_player_quest)

    public:
        // Player to complete the quest /
        // Игрок, которому должен принадлежать герой, чтобы выполнить квест.
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t player;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Specific hero requirement quest /
// Квест, для выполнения которого необходимо быть определённым героем.
// size = 0x44 = 68, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_be_hero_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_be_hero_quest(bool _seer_hut, THeroID _id) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut), id(_id)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_be_hero_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_be_hero_quest)

    public:
        // Hero ID to complete the quest /
        // Необходимый ID героя для выполнения квеста.
        // offset: +0x40 = +64,  size = 0x4 = 4
        THeroID id;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Resource pay quest /
// Квест, для выполнения которого необходимо заплатить ресурсами.
// size = 0x5C = 92, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_resource_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_resource_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_resource_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_resource_quest) 

    public:
        // Resource requirement /
        // Необходимые для выполнения квеста ресурсы.
        // offset: +0x40 = +64,  size = 0x1C = 28
        std::array<int32_t, 7> resources;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Creature sacrifice quest /
// Квест, для выполнения которого необходимо пожертвовать существами из армии.
// size = 0x60 = 96, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_creature_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_creature_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_creature_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag), amounts(tag), types(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_creature_quest)

    public:
        // Creature amounts needed to complete the quest /
        // Количества существ, необходимые для выполнения квеста.
        // offset: +0x40 = +64,  size = 0x10 = 16
        exe_vector<int32_t>       amounts;

        // Creature types needed to complete the quest /
        // Типы существ, необходимые для выполнения квеста.
        // offset: +0x50 = +80,  size = 0x10 = 16
        exe_vector<TCreatureType> types;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Artifact requirement quest /
// Квест, для выполнения которого необходимо отдать артефакт.
// size = 0x50 = 80, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_artifact_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_artifact_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_artifact_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag), artifacts(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_artifact_quest)

    public:
        // Artifacts needed to complete the quest /
        // Артефакты, необходимые для выполнения квеста.
        // offset: +0x40 = +64,  size = 0x10 = 16
        exe_vector<TArtifact> artifacts;
};
#pragma pack(pop)

enum TCreatureType : int32_t;

#pragma pack(push, 4)
// Defeat the monster quest /
// Квест, для выполнения которого необходимо убить определённого монстра.
// size = 0x50 = 80, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_monster_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_monster_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_monster_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_monster_quest)

    public:
        // Target monster object reference /
        // Ссылка на объект монстра на карте(определяется редактором карт автоматически).
        // offset: +0x40 = +64,  size = 0x4 = 4
        uint32_t objRef;

        // Target monster position /
        // Точка на карте, где находится монстр, которого необходимо убить для выполнения квеста.
        // offset: +0x44 = +68,  size = 0x4 = 4
        type_point pos;

        // Target monster type /
        // Тип монстра, которого необходимо убить для выполнения квеста.
        // offset: +0x48 = +72,  size = 0x4 = 4
        TCreatureType type;

        // Player that killed the target monster /
        // Игрок, убивший этого монстра.
        // offset: +0x4C = +76,  size = 0x4 = 4
        int32_t killer;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Defeat the hero quest /
// Квест, для выполнения которого необходимо убить определённого героя.
// size = 0x4C = 76, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_defeat_hero_quest : public type_quest
{
    public: 
        NH3API_FORCEINLINE
        type_defeat_hero_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_defeat_hero_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag), completed(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_defeat_hero_quest)

    public:
        // Target hero object reference /
        // Ссылка на объект героя на карте(определяется редактором карт автоматически).
        // offset: +0x40 = +64,  size = 0x4 = 4
        uint32_t objRef;

        // Hero ID to be killed /
        // ID героя, которого необходимо убить, чтобы выполнить квест.
        // offset: +0x44 = +68,  size = 0x4 = 4
        THeroID   id;

        // Players that killed that hero at least once /
        // Игроки, убивавшие хоть раз этого героя.
        // offset: +0x48 = +72,  size = 0x4 = 4
        exe_bitset<MAX_PLAYERS> completed;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Primary skills quest: guest hero must have the required set of primary skills /
// Квест первичного навыка: герой-гость должен обладать необходимым набором значений первичных навыков для выполнения квеста.
// size = 0x44 = 68, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_skill_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_skill_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_skill_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_skill_quest)

    public:
        // Primary skill requirements /
        // Минимальные значения первичных навыков, необходимые для выполнения квеста.
        // offset: +0x40 = +64,  size = 0x4 = 4
        std::array<int8_t, kNumPrimarySkills> skill;
};
#pragma pack(pop)

#pragma pack(push, 4)
// Experience requirement quest /
// Квест, для выполнения которого герой-гость должен достигнуть определенного уровня опыта.
// size = 0x44 = 68, align = 4, baseclass: type_quest
NH3API_VIRTUAL_CLASS type_experience_quest : public type_quest
{
    public:
        NH3API_FORCEINLINE
        type_experience_quest(bool _seer_hut) NH3API_NOEXCEPT
            : type_quest(nh3api::omit_base_vftable_tag, _seer_hut)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        type_experience_quest(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : type_quest(tag)
        {}

    public:
        NH3API_VIRTUAL_OVERRIDE_TYPE_QUEST(type_experience_quest)

    public:
        // Hero level /
        // Уровень героя, необходимый для выполнения квеста.
        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t level;
};
#pragma pack(pop)

#pragma pack(push, 1)
// Quest guard /
// Страж границы.
// size = 0x5 = 5, align = 1
class TQuestGuard
{
    public:
        NH3API_FORCEINLINE
        TQuestGuard() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(TQuestGuard)
        { THISCALL_1(void, 0x573950, this); }

        NH3API_FORCEINLINE
        TQuestGuard(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {}

    public:
        NH3API_FORCEINLINE
        exe_string getLogText() const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_2(exe_string*, 0x573130, this, &result);
            return result;
        }

        NH3API_FORCEINLINE
        exe_string getQuickViewText(int32_t player) const
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_3(exe_string*, 0x573210, this, &result, player);
            return result;
        }

        NH3API_FORCEINLINE
        exe_string getRolloverText(int32_t player) const 
        {
            exe_string result(nh3api::dummy_tag);
            (void) THISCALL_3(exe_string*, 0x573410, this, &result, player);
            return result;
        }

        void load(TAbstractFile* infile, int32_t version)
        { THISCALL_3(void, 0x573850, this, infile, version); }

        void read(TAbstractFile* infile)
        { THISCALL_2(void, 0x5738B0, this, infile); }

        int32_t save(TAbstractFile* outfile) const
        { return THISCALL_2(int32_t, 0x5738F0, this, outfile); }

    public:
        // Target quest /
        // Квест, который необходимо выполнить.
        // offset: +0x0 = +0,  size = 0x4 = 4
        type_quest* quest;

        // Players that visited this object /
        // Игроки, посетившие этот объект.
        // offset: +0x4 = +4,  size = 0x1 = 1
        uint8_t visitedPlayers;
};
#pragma pack(pop)

// Seer hut reward type /
// Тип вознаграждения хижины провидца.
enum TSeerRewardType : uint32_t
{
	eRewardNone           = 0,
	eRewardExperience     = 1,  // Очки опыта
	eRewardMana           = 2,  // Мана
	eRewardMorale         = 3,  // Бонус боевого духа
	eRewardLuck           = 4,  // Бонус удачи
	eRewardResource       = 5,  // Ресурсы
	eRewardPrimarySkill   = 6,  // Первичный навык
	eRewardSecondarySkill = 7,  // Вторичный навык
	eRewardArtifact       = 8,  // Артефакт
	eRewardSpell          = 9,  // Заклинание
    eRewardCreature       = 10, // Существо
};

#pragma pack(push, 4)
// Seerhut reward / 
// Информация о вознаграждении хижины провидца.
// size = 0xC = 12, align = 4
struct TSeerReward
{
    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TSeerReward(TSeerRewardType reward_type,
                    int32_t reward_value1,
                    int32_t reward_value2) NH3API_NOEXCEPT
            : rewardType(reward_type), 
              rewardValue1(reward_value1),
              rewardValue2(reward_value2)
        {}

        NH3API_FORCEINLINE NH3API_CONSTEXPR
        TSeerReward() NH3API_NOEXCEPT
            : rewardType(eRewardNone), rewardValue1(0), rewardValue2(0)
        {}

        NH3API_FORCEINLINE 
        TSeerReward(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        { NH3API_IGNORE(rewardType, rewardValue1, rewardValue2); }

    public: 
        int32_t getValue(hero* current_hero)
        { return THISCALL_2(int32_t, 0x573E40, this, current_hero); }

        int32_t getRewardExtra(hero* current_hero)
        { return THISCALL_2(int32_t, 0x5742E0, this, current_hero); }

        void giveReward(hero* current_hero, bool human_player)
        { THISCALL_3(void, 0x574050, this, current_hero, human_player); }
    
    public:
        // Reward type /
        // Тип вознаграждения хижины провидца.
        // offset: +0x0 = +5,  size = 0x4 = 4
        TSeerRewardType rewardType;

        // offset: +0x9 = +9,  size = 0x4 = 4
        int32_t rewardValue1;

        // offset: +0xD = +13,  size = 0x4 = 4
        int32_t rewardValue2;
};
#pragma pack(pop)

#pragma pack(push, 1)
// Seer hut /
// Хижина провидца
// size = 0x13 = 19, align = 1, baseclass: TQuestGuard
class TSeerHut : TQuestGuard
{
    public:
        NH3API_FORCEINLINE
        TSeerHut() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(TSeerHut)
        { THISCALL_1(void, 0x573950, this); }

        NH3API_FORCEINLINE
        TSeerHut(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : TQuestGuard(tag), reward(tag)
        {}

    public:
        // Reward info /
        // Информация о вознаграждении хижины провидца.
        // offset: +0x5 = +5,  size = 0xC = 12
        TSeerReward reward;

        // Random seerhut name index /
        // Индекс случайного имени провидца.
        // offset: +0x11 = +17,  size = 0x1 = 1
        int8_t      NameIndex;

    protected:
        NH3API_MAYBE_UNUSED
        // offset: +0x12 = +18,  size = 0x1 = 1
        int8_t unused;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct QuestMonster
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t objRef;

    // offset: +0x4 = +4,  size = 0x4 = 4
    type_point point;

};
#pragma pack(pop)

NH3API_FORCEINLINE
type_quest* create_quest(EQuestType quest_type, bool hut) NH3API_NOEXCEPT
{ return FASTCALL_2(type_quest*, 0x573610, quest_type, static_cast<bool32_t>(hut)); }