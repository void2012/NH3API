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

#include "nh3api_std/nh3api_std.hpp"
#include "nh3api_std/enum_limits.hpp"

// Heroes IDs /
// Герои.
enum THeroID : int32_t
{
    HERO_NONE            = -1, // Никакой.
    HERO_MOST_POWERFUL   = -3, // Самый сильный герой(лагерь героя).

    HERO_ORRIN           = 0,  // Оррин
    HERO_VALESKA         = 1,  // Валеска
    HERO_EDRIC           = 2,  // Эдрик
    HERO_SYLVIA          = 3,  // Сильвия
    HERO_LORD_HAART      = 4,  // Лорд Хаарт
    HERO_SORSHA          = 5,  // Сорша
    HERO_CHRISTIAN       = 6,  // Кристиан
    HERO_TYRIS           = 7,  // Тирис
    HERO_RION            = 8,  // Рион
    HERO_ADELA           = 9,  // Адель
    HERO_CUTHBERT        = 10, // Катберт
    HERO_ADELAIDE        = 11, // Аделаида
    HERO_INGHAM          = 12, // Инхам
    HERO_SANYA           = 13, // Соня
    HERO_LOYNIS          = 14, // Лонис
    HERO_CAITLIN         = 15, // Кейтлин

    HERO_MEPHALA         = 16, // Мепхала
    HERO_UFRETIN         = 17, // Уфретин
    HERO_JENOVA          = 18, // Енова
    HERO_RYLAND          = 19, // Риланд
    HERO_THORGRIM        = 20, // Торгрим
    HERO_IVOR            = 21, // Ивор
    HERO_CLANCY          = 22, // Кланси
    HERO_KYRRE           = 23, // Киррь
    HERO_CORONIUS        = 24, // Корониус
    HERO_ULAND           = 25, // Уланд
    HERO_ELLESHAR        = 26, // Эллезар
    HERO_GEM             = 27, // Джем
    HERO_MALCOM          = 28, // Малькольм
    HERO_MELODIA         = 29, // Мелоида
    HERO_ALAGAR          = 30, // Алагар
    HERO_AERIS           = 31, // Аерис

    HERO_PIQUEDRAM       = 32, // Пикедрам
    HERO_THANE           = 33, // Тан
    HERO_JOSEPHINE       = 34, // Жозефина
    HERO_NEELA           = 35, // Нелла
    HERO_TOROSAR         = 36, // Торосар
    HERO_FAFNER          = 37, // Фафнер
    HERO_RISSA           = 38, // Ризза
    HERO_IONA            = 39, // Иона
    HERO_ASTRAL          = 40, // Астрал
    HERO_HALON           = 41, // Халон
    HERO_SERENA          = 42, // Серена
    HERO_DAREMYTH        = 43, // Даремиф
    HERO_THEODORUS       = 44, // Теодор
    HERO_SOLMYR          = 45, // Солмир
    HERO_CYRA            = 46, // Кира
    HERO_AINE            = 47, // Аин

    HERO_FIONA           = 48, // Фиона
    HERO_RASHKA          = 49, // Рашка
    HERO_MARIUS          = 50, // Мариус
    HERO_IGNATIUS        = 51, // Игнат
    HERO_OCTAVIA         = 52, // Октавия
    HERO_CALH            = 53, // Калх
    HERO_PYRE            = 54, // Пир
    HERO_NYMUS           = 55, // Нимус
    HERO_AYDEN           = 56, // Айден
    HERO_XYRON           = 57, // Ксирон
    HERO_AXSIS           = 58, // Аксис
    HERO_OLEMA           = 59, // Олема
    HERO_CALID           = 60, // Калид
    HERO_ASH             = 61, // Аш
    HERO_ZYDAR           = 62, // Зидар
    HERO_XARFAX          = 63, // Ксарфакс

    HERO_STRAKER         = 64, // Стракер
    HERO_VOKIAL          = 65, // Вокиал
    HERO_MOANDOR         = 66, // Моандор
    HERO_CHARNA          = 67, // Чарна
    HERO_TAMIKA          = 68, // Тамика
    HERO_ISRA            = 69, // Исра
    HERO_CLAVIUS         = 70, // Клавиус
    HERO_GALTHRAN        = 71, // Гарлтран
    HERO_SEPTIENNA       = 72, // Септиенна
    HERO_AISLINN         = 73, // Аислин
    HERO_SANDRO          = 74, // Сандро
    HERO_NIMBUS          = 75, // Нимбус
    HERO_THANT           = 76, // Тант
    HERO_XSI             = 77, // Кси
    HERO_VIDOMINA        = 78, // Видомина
    HERO_NAGASH          = 79, // Нагаш

    HERO_LORELEI         = 80, // Лорелей
    HERO_ARLACH          = 81, // Арлаш
    HERO_DACE            = 82, // Дас
    HERO_AJIT            = 83, // Аджит
    HERO_DAMACON         = 84, // Дамакон
    HERO_GUNNAR          = 85, // Гуннар
    HERO_SYNCA           = 86, // Синка
    HERO_SHAKTI          = 87, // Шакти
    HERO_ALAMAR          = 88, // Аламар
    HERO_JAEGAR          = 89, // Жаегар
    HERO_MALEKITH        = 90, // Малекит
    HERO_JEDDITE         = 91, // Джеддит
    HERO_GEON            = 92, // Геон
    HERO_DEEMER          = 93, // Деемер
    HERO_SEPHINROTH      = 94, // Сепхинороф
    HERO_DARKSTORN       = 95, // Дарксторн

    HERO_YOG             = 96, // Йог
    HERO_GURNISSON       = 97, // Гарниссон
    HERO_JABARKAS        = 98, // Жабаркас
    HERO_SHIVA           = 99, // Шива
    HERO_GRETCHIN        = 100,// Гретчин
    HERO_KRELLION        = 101,// Креллион
    HERO_CRAG_HACK       = 102,// Крэг Хэк
    HERO_TYRAXOR         = 103,// Тираксор
    HERO_GIRD            = 104,// Гирд
    HERO_VEY             = 105,// Вей
    HERO_DESSA           = 106,// Десса
    HERO_TEREK           = 107,// Терек
    HERO_ZUBIN           = 108,// Зубин
    HERO_GUNDULA         = 109,// Гундула
    HERO_ORIS            = 110,// Орис
    HERO_SAURUG          = 111,// Сауруг

    HERO_BRON            = 112,// Брон
    HERO_DRAKON          = 113,// Дрэйкон
    HERO_WYSTAN          = 114,// Вистан
    HERO_TAZAR           = 115,// Тазар
    HERO_ALKIN           = 116,// Алкин
    HERO_KORBAC          = 117,// Корбак
    HERO_GERWULF         = 118,// Гервульф
    HERO_BROGHILD        = 119,// Бронхильд
    HERO_MIRLANDA        = 120,// Мирланда
    HERO_ROSIC           = 121,// Розик
    HERO_VOY             = 122,// Вой
    HERO_VERDISH         = 123,// Вердиш
    HERO_MERIST          = 124,// Мерист
    HERO_STYG            = 125,// Стиг
    HERO_ANDRA           = 126,// Андра
    HERO_TIVA            = 127,// Тива

    HERO_PASIS           = 128,// Пасис
    HERO_THUNAR          = 129,// Тунар
    HERO_IGNISSA         = 130,// Игнисса
    HERO_LACUS           = 131,// Лакус
    HERO_MONERE          = 132,// Монер
    HERO_ERDAMON         = 133,// Эрдамон
    HERO_FIUR            = 134,// Фьюр
    HERO_KALT            = 135,// Кальт
    HERO_LUNA            = 136,// Луна
    HERO_BRISSA          = 137,// Брисса
    HERO_CIELE           = 138,// Циель
    HERO_LABETHA         = 139,// Лабета
    HERO_INTEUS          = 140,// Интеус
    HERO_AENAIN          = 141,// Аэнаин
    HERO_GELARE          = 142,// Джелар
    HERO_GRINDAN         = 143,// Гриндан

    HERO_SIR_MULLICH     = 144,// Сэр Мюллих
    HERO_ADRIENNE        = 145,// Адриэн
    HERO_CATHERINE       = 146,// Катерина
    HERO_DRACON          = 147,// Дракон
    HERO_GELU            = 148,// Джелу
    HERO_KILGOR          = 149,// Килгор
    HERO_LORD_HAART_LICH = 150,// Лорд Хаарт
    HERO_MUTARE          = 151,// Мутаре
    HERO_ROLAND          = 152,// Роланд
    HERO_MUTARE_DRAKE    = 153,// Мутаре Дрэйк
    HERO_BORAGUS         = 154,// Борагус
    HERO_XERON           = 155,// Ксерон

    MAX_HEROES_SOD       = 156,// Кол-во героев в SoD
    MAX_HEROES_AB        = 146,// Кол-во героев в AB
    MAX_HEROES_ROE       = 128,// Кол-во героев в RoE
    MAX_HEROES           = MAX_HEROES_SOD
};

template<>
struct nh3api::enum_limits<THeroID>
    : nh3api::enum_limits_base<THeroID, HERO_ORRIN, HERO_XERON>
{ static inline constexpr bool is_specialized = true; };

// Hero classes /
// Классы героя
enum THeroClass : int32_t
{
    eClassKnight       = 0,  // Класс "Рыцарь"
    eClassCleric       = 1,  // Класс "Клерик"
    eClassRanger       = 2,  // Класс "Рейнджер"
    eClassDruid        = 3,  // Класс "Друид"
    eClassAlchemist    = 4,  // Класс "Алхимик"
    eClassWizard       = 5,  // Класс "Чародей"
    eClassPagan        = 6,  // Класс "Одержимый"
    eClassHeretic      = 7,  // Класс "Еретик"
    eClassDeathKnight  = 8,  // Класс "Рыцарь Смерти"
    eClassNecromancer  = 9,  // Класс "Некромант"
    eClassOverlord     = 10, // Класс "Верховный лорд"
    eClassWarlock      = 11, // Класс "Чернокнижник"
    eClassBarbarian    = 12, // Класс "Варвар"
    eClassBattleMage   = 13, // Класс "Боевой маг"
    eClassBeastmaster  = 14, // Класс "Зверолов"
    eClassWitch        = 15, // Класс "Ведьма"
    eClassPlanesWalker = 16, // Класс "Странник"
    eClassElementalist = 17, // Класс "Элементалист"
    kNumHeroClasses    = 18, // Количество классов героев в игре

    HERO_CLASS_KNIGHT       = 0,  // Класс "Рыцарь"
    HERO_CLASS_CLERIC       = 1,  // Класс "Клерик"
    HERO_CLASS_RANGER       = 2,  // Класс "Рейнджер"
    HERO_CLASS_DRUID        = 3,  // Класс "Друид"
    HERO_CLASS_ALCHEMIST    = 4,  // Класс "Алхимик"
    HERO_CLASS_WIZARD       = 5,  // Класс "Чародей"
    HERO_CLASS_PAGAN        = 6,  // Класс "Одержимый"
    HERO_CLASS_HERETIC      = 7,  // Класс "Еретик"
    HERO_CLASS_DEATHKNIGHT  = 8,  // Класс "Рыцарь Смерти"
    HERO_CLASS_NECROMANCER  = 9,  // Класс "Некромант"
    HERO_CLASS_OVERLORD     = 10, // Класс "Верховный лорд"
    HERO_CLASS_WARLOCK      = 11, // Класс "Чернокнижник"
    HERO_CLASS_BARBARIAN    = 12, // Класс "Варвар"
    HERO_CLASS_BATTLEMAGE   = 13, // Класс "Боевой маг"
    HERO_CLASS_BEASTMASTER  = 14, // Класс "Зверолов"
    HERO_CLASS_WITCH        = 15, // Класс "Ведьма"
    HERO_CLASS_PLANESWALKER = 16, // Класс "Странник"
    HERO_CLASS_ELEMENTALIST = 17, // Класс "Элементалист"
    MAX_HERO_CLASSES = 18 // Количество классов героев в игре
};

template<>
struct nh3api::enum_limits<THeroClass>
    : nh3api::enum_limits_base<THeroClass, HERO_CLASS_KNIGHT, HERO_CLASS_ELEMENTALIST>
{ static inline constexpr bool is_specialized = true; };

// Hero sex /
// Пол героя.
enum TSex : int32_t
{
    eSexMale   = 0, // Мужчина
    eSexFemale = 1 // Женщина
};

// Hero race /
// Раса героя.
enum TRace : int32_t
{
    eRaceDemon      = 0,  // Демон
    eRaceDwarf      = 1,  // Гном
    eRaceEfreet     = 2,  // Ифрит
    eRaceElf        = 3,  // Эльф
    eRaceGenie      = 4,  // Джин
    eRaceGnoll      = 5,  // Гнолл
    eRaceGoblin     = 6,  // Гоблин
    eRaceHuman      = 7,  // Человек
    eRaceLich       = 8,  // Лич
    eRaceLizardman  = 9,  // Ящер
    eRaceMinotaur   = 10, // Минотавр
    eRaceOgre       = 11, // Огр
    eRaceTroglodyte = 12, // Троглодит
    eRaceVampire    = 13 // Вампир
};

// Hero or boat direction frame /
// Кадр поворота героя или лодки.
enum hero_seqid : uint32_t
{
    hs_stand_n   = 0,  // Стоя, повернувшись на север
    hs_stand_ne  = 1,  // Стоя, повернувшись на северо-восток
    hs_stand_e   = 2,  // Стоя, повернувшись на восток
    hs_stand_se  = 3,  // Стоя, повернувшись на юго-восток
    hs_stand_s   = 4,  // Стоя, повернувшись на юг
    hs_walk_n    = 5,  // Двигаясь, повернувшись на север
    hs_walk_ne   = 6,  // Двигаясь, повернувшись на северо-восток
    hs_walk_e    = 7,  // Двигаясь, повернувшись на восток
    hs_walk_se   = 8,  // Двигаясь, повернувшись на юго-восток
    hs_walk_s    = 9,  // Двигаясь, повернувшись на юг
    hs_turn_n_ne = 10, // Поворачиваясь с севера на северо-восток
    hs_turn_ne_n = 11, // Поворачиваясь с северо-востока на север
    hs_turn_ne_e = 12, // Поворачиваясь с северо-востока на восток
    hs_turn_e_ne = 13, // Поворачиваясь с востока на северо-восток
    hs_turn_e_se = 14, // Поворачиваясь с востока на юго-восток
    hs_turn_se_e = 15, // Поворачиваясь с юго-востока на восток
    hs_turn_se_s = 16, // Поворачиваясь с юго-востока на юг
    hs_turn_s_se = 17, // Поворачиваясь с юга на юго-восток
    hs_max       = 18  // Кол-во кадров поворота героя или лодки
};

template<>
struct nh3api::enum_limits<hero_seqid>
    : nh3api::enum_limits_base<hero_seqid, hs_stand_n, hs_turn_s_se>
{ static inline constexpr bool is_specialized = true; };

// Hero state flags /
// Флаги состояния героя.
NH3API_FLAG_ENUM hero_flags : uint32_t
{
    HF_WELL                = 0b0000000000000000000000000000001,
    HF_STABLES             = 0b0000000000000000000000000000010,
    HF_BUOY                = 0b0000000000000000000000000000100,
    HF_SWANPOND            = 0b0000000000000000000000000001000,
    HF_IDOLFORTUNEMORALE   = 0b0000000000000000000000000010000,
    HF_FOUNTAINFORTUNE_N1  = 0b0000000000000000000000000100000,
    HF_WATERINGHOLE        = 0b0000000000000000000000001000000,
    HF_OASIS               = 0b0000000000000000000000010000000,
    HF_TEMPLE              = 0b0000000000000000000000100000000,
    HF_SHIPWRECK           = 0b0000000000000000000001000000000,
    HF_CRYPT               = 0b0000000000000000000010000000000,
    HF_DERELICTSHIPPENALTY = 0b0000000000000000000100000000000,
    HF_PYRAMID             = 0b0000000000000000001000000000000,
    HF_FAERIERING          = 0b0000000000000000010000000000000,
    HF_FOUNTAINOFYOUTH     = 0b0000000000000000100000000000000,
    HF_MERMAIDS            = 0b0000000000000001000000000000000,
    HF_RALLYFLAG           = 0b0000000000000010000000000000000,
    HF_ISINTAVERN          = 0b0000000000000100000000000000000,
    HF_ISINBOAT            = 0b0000000000001000000000000000000,
    HF_UNKNOWN             = 0b0000000000010000000000000000000,
    HF_SIRENS              = 0b0000000000100000000000000000000,
    HF_WARRIORTOMB         = 0b0000000001000000000000000000000,
    HF_LUCKCHEAT           = 0b0000000010000000000000000000000,
    HF_MORALECHEAT         = 0b0000000100000000000000000000000,
    HF_MOVEPOINTSCHEAT     = 0b0000001000000000000000000000000,
    HF_IDOLFORTUNELUCK     = 0b0000010000000000000000000000000,
    HF_TEMPLEDAY7          = 0b0000100000000000000000000000000,
    HF_FOUNTAINFORTUNE_1   = 0b0001000000000000000000000000000,
    HF_FOUNTAINFORTUNE_2   = 0b0010000000000000000000000000000,
    HF_FOUNTAINFORTUNE_3   = 0b0100000000000000000000000000000,
    HF_UNKNOWN2            = 0b1000000000000000000000000000000
};
