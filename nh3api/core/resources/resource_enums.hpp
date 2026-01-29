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

#include "../nh3api_std/enum_limits.hpp" // nh3api::enum_limits

// Game resource(file) type /
// Тип игрового ресурса(файла).
enum EResourceType : int32_t
{
    RType_invalid     = -1, //
    RType_misc        =  0, //
    RType_null        =  0, //
    RType_data        =  1, //
    RType_text        =  2, //
    RType_bitmap      = 16, //
    RType_bitmap8     = 16, //
    RType_bitmap24    = 17, //
    RType_bitmap16    = 18, //
    RType_bitmap565   = 19, //
    RType_bitmap555   = 20, //
    RType_bitmap1555  = 21, //
    RType_sfx         = 32, //
    RType_midi        = 48, //
    RType_sprite      = 64, //
    RType_spritedef   = 65, //
    RType_creature    = 66, //
    RType_advobj      = 67, //
    RType_hero        = 68, //
    RType_tileset     = 69, //
    RType_pointer     = 70, //
    RType_interface   = 71, //
    RType_spriteframe = 72, //
    RType_combat_hero = 73, //
    RType_advmask     = 79, //
    RType_font        = 80, //
    RType_palette     = 96  //
};

// Game resources and NormalDialog sprites IDs /
// Игровые ресурсы и ID спрайтов в NormalDialog.
enum EGameResource : int32_t
{
    const_no_resource           = -1,
    WOOD                        = 0,  // Дерево
    MERCURY                     = 1,  // Ртуть
    ORE                         = 2,  // Руда
    SULFUR                      = 3,  // Сера
    CRYSTAL                     = 4,  // Кристаллы
    GEMS                        = 5,  // Самоцветы
    GOLD                        = 6,  // Золото

    GAME_RESOURCE_NONE          = -1,
    GAME_RESOURCE_WOOD          = 0,  // Дерево
    GAME_RESOURCE_MERCURY       = 1,  // Ртуть
    GAME_RESOURCE_ORE           = 2,  // Руда
    GAME_RESOURCE_SULFUR        = 3,  // Сера
    GAME_RESOURCE_CRYSTAL       = 4,  // Кристаллы
    GAME_RESOURCE_GEMS          = 5,  // Самоцветы
    GAME_RESOURCE_GOLD          = 6,  // Золото
    MAX_GAME_RESOURCES          = 7,
    FIRST_GAME_RESOURCE         = GAME_RESOURCE_WOOD,
    LAST_GAME_RESOURCE          = GAME_RESOURCE_GOLD,

    ABANDONED                   = 7,  // Заброшенная шахта
    RES_ARTIFACT                = 8,  // Спрайт диалога: артефакт
    RES_SPELL                   = 9,  // Спрайт диалога: заклинание
    RES_COLOR                   = 10, // Изображение флага игрока определенного цвета
    RES_GOOD_LUCK               = 11, // Спрайт диалога: положительная удача
    RES_NEUTRAL_LUCK            = 12, // Спрайт диалога: нейтральная удача
    RES_BAD_LUCK                = 13, // Спрайт диалога: отрицательная удача
    RES_GOOD_MORALE             = 14, // Спрайт диалога: положительный боевой дух
    RES_NEUTRAL_MORALE          = 15, // Спрайт диалога: нейтральный боевой дух
    RES_BAD_MORALE              = 16, // Спрайт диалога: отрицательный боевой дух
    RES_EXPERIENCE              = 17, // Спрайт диалога: опыт
    RES_HERO                    = 18, // Спрайт диалога: портрет героя
    RES_ARTIFACT_W_TEXT         = 19, // Unused / Не используется
    RES_SECONDARY_SKILL         = 20, // Спрайт диалога: вторичный навык
    RES_MONSTER                 = 21, // Спрайт диалога: существо
    RES_BUILDING_TT_0           = 22, // Здание фракции Замок
    RES_BUILDING_TT_1           = 23, // Здание фракции Оплот
    RES_BUILDING_TT_2           = 24, // Здание фракции Башня
    RES_BUILDING_TT_3           = 25, // Здание фракции Инферно
    RES_BUILDING_TT_4           = 26, // Здание фракции Некрополис
    RES_BUILDING_TT_5           = 27, // Здание фракции Темница
    RES_BUILDING_TT_6           = 28, // Здание фракции Цитадель
    RES_BUILDING_TT_7           = 29, // Здание фракции Крепость
    RES_BUILDING_TT_8           = 30, // Здание фракции Сопряжение
    RES_PRIMARY_SKILL_ATTACK    = 31, // Спрайт диалога: первичный навык атака
    RES_PRIMARY_SKILL_DEFENSE   = 32, // Спрайт диалога: первичный навык защита
    RES_PRIMARY_SKILL_POWER     = 33, // Спрайт диалога: первичный навык сила магии
    RES_PRIMARY_SKILL_KNOWLEDGE = 34, // Спрайт диалога: первичный навык знание
    RES_MANA                    = 35, // Спрайт диалога: мана
    RES_SMALL_GOLD              = 36  // Маленький спрайт золота
};

template<>
struct nh3api::enum_limits<EGameResource>
    : nh3api::enum_limits_base<EGameResource, FIRST_GAME_RESOURCE, LAST_GAME_RESOURCE>
{ static inline constexpr bool is_specialized = true; };
