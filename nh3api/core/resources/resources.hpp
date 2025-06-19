//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "../nh3api_std/exe_vector.hpp" // exe_vector
#include "../nh3api_std/exe_string.hpp" // exe_string, nh3api::default_hash
#include "../nh3api_std/exe_map.hpp"    // exe_map

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

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
    RType_palette     = 96, //
};

// Game resources and NormalDialog sprites IDs /
// Игровые ресурсы и ID спрайтов в NormalDialog.
enum EGameResource : int32_t
{
    const_no_resource           = -1, //
    WOOD                        = 0,  // Дерево
    MERCURY                     = 1,  // Ртуть
    ORE                         = 2,  // Руда
    SULFUR                      = 3,  // Сера
    CRYSTAL                     = 4,  // Кристаллы
    GEMS                        = 5,  // Самоцветы
    GOLD                        = 6,  // Золото
    ABANDONED                   = 7,  // Заброшенная шахта
    RES_ARTIFACT                = 8,  // Спрайт диалога: артефакт
    RES_SPELL                   = 9,  // Спрайт диалога: заклинание
    RES_COLOR                   = 10, //
    RES_GOOD_LUCK               = 11, // Спрайт диалога: положительная удача
    RES_NEUTRAL_LUCK            = 12, // Спрайт диалога: нейтральная удача
    RES_BAD_LUCK                = 13, // Спрайт диалога: отрицательная удача
    RES_GOOD_MORALE             = 14, // Спрайт диалога: положительный боевой дух
    RES_NEUTRAL_MORALE          = 15, // Спрайт диалога: нейтральный боевой дух
    RES_BAD_MORALE              = 16, // Спрайт диалога: отрицательный боевой дух
    RES_EXPERIENCE              = 17, // Спрайт диалога: опыт
    RES_HERO                    = 18, // Спрайт диалога: портрет героя
    RES_ARTIFACT_W_TEXT         = 19, //
    RES_SECONDARY_SKILL         = 20, // Спрайт диалога: вторичный навык
    RES_MONSTER                 = 21, // Спрайт диалога: существо
    RES_BUILDING_TT_0           = 22, //
    RES_BUILDING_TT_1           = 23, //
    RES_BUILDING_TT_2           = 24, //
    RES_BUILDING_TT_3           = 25, //
    RES_BUILDING_TT_4           = 26, //
    RES_BUILDING_TT_5           = 27, //
    RES_BUILDING_TT_6           = 28, //
    RES_BUILDING_TT_7           = 29, //
    RES_PRIMARY_SKILL_ATTACK    = 30, // Спрайт диалога: первичный навык атака
    RES_PRIMARY_SKILL_DEFENSE   = 31, // Спрайт диалога: первичный навык защита
    RES_PRIMARY_SKILL_POWER     = 32, // Спрайт диалога: первичный навык сила магии
    RES_PRIMARY_SKILL_KNOWLEDGE = 33, // Спрайт диалога: первичный навык знание
    RES_MANA                    = 34, // Спрайт диалога: мана
    RES_SMALL_GOLD              = 35, //
};

#ifndef NH3API_VIRTUAL_OVERRIDE_RESOURCE
#define NH3API_VIRTUAL_OVERRIDE_RESOURCE(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
virtual void __thiscall Dispose() override\
{ return get_type_vftable(this)->Dispose(this); }\
virtual int32_t __thiscall GetSize() const override\
{ return get_type_vftable(this)->GetSize(this); }
#endif // NH3API_VIRTUAL_OVERRIDE_RESOURCE

#pragma pack(push, 4)
// Game resource abstract class /
// Ресурс игры(базовый класс).
// size = 0x1C = 28, align = 4
NH3API_VIRTUAL_STRUCT resource
{
    // virtual function table
    public:
        struct vftable_t
        {
            void    (__thiscall* scalar_deleting_destructor)(resource*, uint8_t);
            void    (__thiscall* Dispose)(resource*);
            int32_t (__thiscall* GetSize)(const resource*);
        };

    private:
        resource() NH3API_DELETED_FUNCTION

    // constructors and destructor
    public:
        NH3API_FORCEINLINE
        resource(const char* newName, EResourceType newType) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(resource)
        { THISCALL_3(void, 0x558970, this, newName, newType); }

        NH3API_FORCEINLINE
        // dummy constructor
        resource(const nh3api::dummy_tag_t&) NH3API_NOEXCEPT
        { NH3API_IGNORE(Name, resType, ReferenceCount); }

        NH3API_FORCEINLINE
        ~resource() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x5589F0, this); }

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual void __thiscall Dispose()
        { return get_vftable(this)->Dispose(this); }

        // vftable shift: +8
        virtual int32_t __thiscall GetSize() const
        { return get_vftable(this)->GetSize(this); }

    // member variables
    public:
        // Resource name /
        // Название ресурса.
        // offset: +0x4 = +4,  size = 0xD = 13
        std::array<char, 13> Name;

        // Resource type /
        // Тип ресурса
        // offset: +0x14 = +20,  size = 0x4 = 4
        EResourceType resType;

        // Reference count /
        // Количество ссылок.
        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t ReferenceCount;
};
#pragma pack(pop)

#pragma pack(push, 1)
// size = 0x3 = 3, align = 1
struct TRGB
{
    // Red bits /
    // Биты красного цвета.
    // offset: +0x0 = +0,  size = 0x1 = 1
    uint8_t Red;

    // Green bits /
    // Биты зелёного цвета.
    // offset: +0x1 = +1,  size = 0x1 = 1
    uint8_t Green;

    // Blue bits /
    // Биты голубого цвета.
    // offset: +0x2 = +2,  size = 0x1 = 1
    uint8_t Blue;

};
#pragma pack(pop)

#pragma pack(push, 1)
// size = 0x4 = 4, align = 1
struct TRGBA : public TRGB
{
    // Alpha channel /
    // Альфа-канал
    // offset: +0x3 = +3,  size = 0x1 = 1
    uint8_t Alpha;
};
#pragma pack(pop)

#pragma pack(push, 4)
// 24 Bit palette resource /
// Игровой ресурс - 24-х битная палитра.
// size = 0x31C = 796, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TPalette24 : public resource
{
public:
    NH3API_FORCEINLINE
    TPalette24() NH3API_NOEXCEPT
    NH3API_DELEGATE_DUMMY_OR_BASE(TPalette24, resource)
    { THISCALL_1(void, 0x523320, this); }

    NH3API_FORCEINLINE
    TPalette24(const uint8_t* data) NH3API_NOEXCEPT
    NH3API_DELEGATE_DUMMY_OR_BASE(TPalette24, resource)
    { THISCALL_2(void, 0x523370, this, data); }

    NH3API_FORCEINLINE
    TPalette24(const TRGBA* rgba) NH3API_NOEXCEPT
    NH3API_DELEGATE_DUMMY_OR_BASE(TPalette24, resource)
    { THISCALL_2(void, 0x5233A0, this, rgba); }

    NH3API_FORCEINLINE
    TPalette24(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        : resource(tag) // resource(nullptr, RType_misc)
    { NH3API_IGNORE(Palette); }

    ~TPalette24()
    { THISCALL_1(void, 0x523450, this); }

public:
    NH3API_VIRTUAL_OVERRIDE_RESOURCE(TPalette24)

public:

    // 24-bit Palette /
    // 24-битная палитра.
    // offset: +0x1C = +28,  size = 0x300 = 768
    std::array<TRGB, 256> Palette;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
TPalette24*& gPlayerPalette24
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6AAD14, TPalette24*));

#pragma pack(push, 4)
// 16 Bit palette resource /
// Игровой ресурс - 16-ти битная палитра.
// size = 0x21C = 540, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TPalette16 : public resource
{
    public:
        NH3API_FORCEINLINE
        TPalette16() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette16, resource)
        { THISCALL_1(void, 0x522B40, this); }

        NH3API_FORCEINLINE
        TPalette16(const TPalette24& p24) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette16, resource)
        { THISCALL_2(void, 0x522D00, this, &p24); }

        NH3API_FORCEINLINE
        TPalette16(const uint16_t* data) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette16, resource)
        { THISCALL_2(void, 0x522B90, this, data); }

        NH3API_FORCEINLINE
        TPalette16(const TPalette24& p24,
                   int32_t rbits,
                   int32_t rshift,
                   int32_t gbits,
                   int32_t gshift,
                   int32_t bbits,
                   int32_t bshift) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette16, resource)
        { THISCALL_8(void, 0x522BC0, this, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        TPalette16(const char* name,
                   const TPalette24& p24,
                   int32_t rbits,
                   int32_t rshift,
                   int32_t gbits,
                   int32_t gshift,
                   int32_t bbits,
                   int32_t bshift) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette16, resource)
        { THISCALL_9(void, 0x522C60, this, name, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        TPalette16(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag) // resource(nullptr, RType_misc)
        { NH3API_IGNORE(Palette); }

        NH3API_FORCEINLINE
        TPalette16& operator=(const TPalette16& other) NH3API_NOEXCEPT
        { return *THISCALL_2(TPalette16*, 0x522E00, this, other); }

    public:
        NH3API_FORCEINLINE
        void Convert24to16(const TRGB* p24, int32_t rbits, int32_t rshift, int32_t gbits, int32_t gshift, int32_t bbits, int32_t bshift)
        { for ( int32_t i = 0; i <= 0xFF; ++i ) Palette[i] = (p24[i].Blue >> (8 - bbits) << bshift) | (p24[i].Green >> (8 - gbits) << gshift) | (p24[i].Red >> (8 - rbits) << rshift); }

        NH3API_FORCEINLINE
        void Convert24to16(const TPalette24& p24, int32_t rbits, int32_t rshift, int32_t gbits, int32_t gshift, int32_t bbits, int32_t bshift)
        { Convert24to16(p24.Palette.data(), rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        void Cycle(int32_t begin, int32_t end, int32_t step)
        { THISCALL_4(void, 0x522E40, this, begin, end, step); }

        void AdjustSaturation(float amount)
        { THISCALL_2(void, 0x522F00, this, amount); }

        void AdjustHSV(float hue, float hue_adjust, float saturation_adjust, float value_adjust)
        { THISCALL_5(void, 0x523040, this, hue, hue_adjust, saturation_adjust, value_adjust); }

        void Gray()
        { THISCALL_1(void, 0x523240, this); }

        const uint16_t* data() const 
        { return Palette.data(); }

        uint16_t* data() 
        { return Palette.data(); }

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TPalette16)

    public:
        // 16-bit Palette /
        // 16-битная палитра.
        // offset: +0x1C = +28,  size = 0x200 = 512
        std::array<uint16_t, 256> Palette;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
TPalette16*& gPlayerPalette
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6AAD10, TPalette16*));

NH3API_INLINE_OR_EXTERN
TPalette16*& gGamePalette
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6AAD18, TPalette16*));

// .def file encoding method /
// Метод кодировки .def файла.
enum TEncodingMethod : uint32_t
{
    eEncodeRaw          = 0, // Без кодировки
    eEncodeGeneralRLE   = 1, // Обычная кодировка
    eEncodeTilesetRLE   = 2, // Кодировка тайлов
    eEncodeAdvObjRLE    = 3, // Кодировка остальных спрайтов
    kNumEncodingMethods = 4, // Кол-во методов кодировки
};

#pragma pack(push, 4)
// Sprite frame resource /
// Игровой ресурс - один кадр спрайта(.def).
// size = 0x48 = 72, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT CSpriteFrame : public resource
{
    public:
        NH3API_FORCEINLINE
        CSpriteFrame(const char* name,
                     int32_t w,
                     int32_t h,
                     uint16_t* data,
                     int32_t csize,
                     TEncodingMethod encoding) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(CSpriteFrame, resource)
        { THISCALL_7(void, 0x47BBD0, this, name, w, h, data, csize, encoding); }

        NH3API_FORCEINLINE
        CSpriteFrame(const char* name,
                     int32_t w,
                     int32_t h,
                     uint16_t* data,
                     int32_t csize,
                     TEncodingMethod encoding,
                     int32_t cw,
                     int32_t ch,
                     int32_t cx,
                     int32_t cy) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(CSpriteFrame, resource)
        { THISCALL_11(void, 0x47BC80, this, name, w, h, data, csize, encoding, cw, ch, cx, cy); }

        NH3API_FORCEINLINE
        CSpriteFrame(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag) // resource(nullptr, RType_misc)
        { NH3API_IGNORE(DataSize,
                        ImageSize,
                        EncodingMethod,
                        Width,
                        Height,
                        CroppedWidth,
                        CroppedHeight,
                        CroppedX,
                        CroppedY,
                        Pitch,
                        map); }

        NH3API_FORCEINLINE
        ~CSpriteFrame()
        { THISCALL_1(void, 0x47BD50, this); }

    public:
        NH3API_FORCEINLINE
        static void SetPixelFormat(uint32_t red_mask, uint32_t green_mask, uint32_t blue_mask)
        { FASTCALL_3(void, 0x47BD80, red_mask, green_mask, blue_mask); }

        NH3API_FORCEINLINE
        // Draw combat creature /
        // Нарисовать существо в бою.
        void DrawCreature(int32_t sx,
                          int32_t sy,
                          int32_t sw,
                          int32_t sh,
                          uint16_t* dst,
                          int32_t dx,
                          int32_t dy,
                          int32_t dw,
                          int32_t dh,
                          int32_t dpitch,
                          TPalette16& pal,
                          bool hflip,
                          uint16_t flagcolor)
        { THISCALL_14(void, 0x47C300, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, flagcolor); }

        NH3API_FORCEINLINE
        // Draw adventure map object /
        // Нарисовать объект на карте приключений.
        void DrawAdvObjImpl(int32_t sx,
                            int32_t sy,
                            int32_t sw,
                            int32_t sh,
                            uint16_t* dst,
                            int32_t dx,
                            int32_t dy,
                            int32_t dw,
                            int32_t dh,
                            int32_t dpitch,
                            TPalette16& pal,
                            bool hflip,
                            uint16_t flagcolor)
        { THISCALL_14(void, 0x47C9C0, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, flagcolor); }

        NH3API_FORCEINLINE
        void DrawHeroAlpha(int32_t sx,
                           int32_t sy,
                           int32_t sw,
                           int32_t sh,
                           uint16_t* dst,
                           int32_t dx,
                           int32_t dy,
                           int32_t dw,
                           int32_t dh,
                           int32_t dpitch,
                           TPalette16& pal,
                           bool hflip)
        { THISCALL_13(void, 0x47CE10, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip); }

        NH3API_FORCEINLINE
        void DrawAdvObjShadowImpl(int32_t sx,
                                  int32_t sy,
                                  int32_t sw,
                                  int32_t sh,
                                  uint16_t* dst,
                                  int32_t dx,
                                  int32_t dy,
                                  int32_t dw,
                                  int32_t dh,
                                  int32_t dpitch,
                                  TPalette16& pal,
                                  bool hflip)
        { THISCALL_13(void, 0x47D250, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip); }

        NH3API_FORCEINLINE
        // Draw terrain/river/road/route arrow tile /
        // Нарисовать клетку почвы/дорогу/реку/стрелки хода.
        void DrawTile(int32_t sx,
                      int32_t sy,
                      int32_t sw,
                      int32_t sh,
                      uint16_t* dst,
                      int32_t dx,
                      int32_t dy,
                      int32_t dw,
                      int32_t dh,
                      int32_t dpitch,
                      TPalette16& pal,
                      bool hflip,
                      bool vflip)
        { THISCALL_14(void, 0x47D660, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, vflip); }

        NH3API_FORCEINLINE
        void DrawTileShadow(int32_t sx,
                            int32_t sy,
                            int32_t sw,
                            int32_t sh,
                            uint16_t* dst,
                            int32_t dx,
                            int32_t dy,
                            int32_t dw,
                            int32_t dh,
                            int32_t dpitch,
                            TPalette16& pal,
                            bool hFlipped,
                            bool vFlipped)
        { THISCALL_14(void, 0x47E140, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hFlipped, vFlipped); }

        NH3API_FORCEINLINE
        void DrawSpellEffect(int32_t sx,
                             int32_t sy,
                             int32_t sw,
                             int32_t sh,
                             uint16_t* dst,
                             int32_t dx,
                             int32_t dy,
                             int32_t dw,
                             int32_t dh,
                             int32_t dpitch,
                             TPalette16& pal,
                             int32_t hflip,
                             bool alpha)
        { THISCALL_14(void, 0x47E880, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, alpha); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(CSpriteFrame)

    // member variables
    public:
        // Frame data size /
        // Размер данных кадра.
        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t DataSize;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t ImageSize;

        // Frame encoding method /
        // Метод кодировки кадра.
        // offset: +0x24 = +36,  size = 0x4 = 4
        TEncodingMethod EncodingMethod;

        // Frame width /
        // Ширина кадра.
        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t Width;

        // Frame height /
        // Высота кадра.
        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t Height;

        // offset: +0x30 = +48,  size = 0x4 = 4
        int32_t CroppedWidth;

        // offset: +0x34 = +52,  size = 0x4 = 4
        int32_t CroppedHeight;

        // offset: +0x38 = +56,  size = 0x4 = 4
        int32_t CroppedX;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t CroppedY;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t Pitch;

        // Frame raw data /
        // Данные спрайта.
        // offset: +0x44 = +68,  size = 0x4 = 4
        uint16_t* map;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Sprite frames sequence /
// Последовательность кадров спрайта.
// size = 0xC = 12, align = 4
struct CSequence
{
    public:
        NH3API_FORCEINLINE
        CSequence(int32_t num) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(CSequence)
        { THISCALL_2(void, 0x47B160, this, num); }

        NH3API_FORCEINLINE
        CSequence(const nh3api::dummy_tag_t& dummy) NH3API_NOEXCEPT
        { NH3API_IGNORE(numFrames, allocatedFrames, f); }

    public:

        // Number of frames /
        // Кол-во кадров
        // offset: +0x0 = +0,  size = 0x4 = 4
        int32_t numFrames;

        // offset: +0x4 = +4,  size = 0x4 = 4
        int32_t allocatedFrames;

        // Frames /
        // Кадры.
        // offset: +0x8 = +8,  size = 0x4 = 4
        CSpriteFrame** f;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Sprite resource(.def) /
// Игровой ресурс - спрайт(.def)
// size = 0x38 = 56, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT CSprite : public resource
{
    public:
        NH3API_FORCEINLINE
        CSprite(const char* name, EResourceType sprtype, int32_t w, int32_t h) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(CSprite, resource)
        { THISCALL_5(void, 0x47B240, this, name, sprtype, w, h); }

        NH3API_FORCEINLINE
        CSprite(const nh3api::dummy_tag_t& dummy) NH3API_NOEXCEPT
            : resource(dummy)
        { NH3API_IGNORE(s, p16, p24, numSequences, validSeqMask, Width, Height); }

    public:
        NH3API_FORCEINLINE
        int32_t GetWidth() const
        { return Width; }

        NH3API_FORCEINLINE
        int32_t GetHeight() const
        { return Height; }

        NH3API_FORCEINLINE
        void SetPalette(const TPalette16& pal)
        { THISCALL_2(void, 0x47B4A0, this, &pal); }

        NH3API_FORCEINLINE
        uint16_t* GetPalette()
        { return p16 ? p16->Palette.data() : nullptr; }

        NH3API_FORCEINLINE
        void ResetPalette()
        { THISCALL_1(void, 0x47B520, this); }

        NH3API_FORCEINLINE
        void ColorCycle(int32_t begin, int32_t end, int32_t step)
        { return p16->Cycle(begin, end, step); }

        NH3API_FORCEINLINE
        // Draw general function /
        // Общая функция отрисовки спрайта.
        void Draw(int32_t seqnum,
                  int32_t framenum,
                  int32_t sx,
                  int32_t sy,
                  int32_t sw,
                  int32_t sh,
                  uint16_t* dst,
                  int32_t dx,
                  int32_t dy,
                  int32_t dw,
                  int32_t dh,
                  int32_t dpitch,
                  bool hflip,
                  bool tblit)
        { THISCALL_15(void, 0x47B610, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, tblit); }

        NH3API_FORCEINLINE
        // Draw combat creature /
        // Нарисовать существо в бою.
        void DrawCreature(int32_t seqnum,
                          int32_t framenum,
                          int32_t sx,
                          int32_t sy,
                          int32_t sw,
                          int32_t sh,
                          uint16_t* dst,
                          int32_t dx,
                          int32_t dy,
                          int32_t dw,
                          int32_t dh,
                          int32_t dpitch,
                          bool hflip,
                          uint16_t outcolor)
        { THISCALL_15(void, 0x47B680, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, outcolor); }

        NH3API_FORCEINLINE
        // Draw adventure map object /
        // Нарисовать объект на карте приключений.
        void DrawAdvObj(int32_t frame,
                        int32_t sx,
                        int32_t sy,
                        int32_t sw,
                        int32_t sh,
                        uint16_t* dst,
                        int32_t dx,
                        int32_t dy,
                        int32_t dw,
                        int32_t dh,
                        int32_t dpitch,
                        bool hflip)
        { THISCALL_13(void, 0x47B6E0, this, frame, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw adventure map object with flag /
        // Нарисовать объект с флагом на карте приключений.
        void DrawAdvObjWithFlag(int32_t framenum,
                                int32_t sx,
                                int32_t sy,
                                int32_t sw,
                                int32_t sh,
                                uint16_t* dst,
                                int32_t dx,
                                int32_t dy,
                                int32_t dw,
                                int32_t dh,
                                int32_t dpitch,
                                uint16_t flagcolor,
                                bool hflip)
        { THISCALL_14(void, 0x47B730, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, flagcolor, hflip); }

        NH3API_FORCEINLINE
        // Draw adventure map object shadow /
        // Нарисовать тень объекта на карте приключений.
        void DrawAdvObjShadow(int32_t framenum,
                              int32_t sx,
                              int32_t sy,
                              int32_t sw,
                              int32_t sh,
                              uint16_t* dst,
                              int32_t dx,
                              int32_t dy,
                              int32_t dw,
                              int32_t dh,
                              int32_t dpitch,
                              bool hflip)
        { THISCALL_13(void, 0x47B780, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw mouse cursor /
        // Нарисовать курсор мыши.
        void DrawPointer(int32_t framenum,
                         uint16_t* dst,
                         int32_t dx,
                         int32_t dy,
                         int32_t dw,
                         int32_t dh,
                         int32_t dpitch,
                         bool hflip)
        { THISCALL_9(void, 0x47B7D0, this, framenum, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw interface object /
        // Нарисовать объект интерфейса.
        void DrawInterface(int32_t framenum,
                           int32_t sx,
                           int32_t sy,
                           int32_t sw,
                           int32_t sh,
                           uint16_t* dst,
                           int32_t dx,
                           int32_t dy,
                           int32_t dw,
                           int32_t dh,
                           int32_t dpitch,
                           bool hflip)
        { THISCALL_13(void, 0x47B820, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw terrain/river/road/route arrow tile /
        // Нарисовать клетку почвы/дорогу/реку/стрелки хода.
        void DrawTile(int32_t framenum,
                      int32_t sx,
                      int32_t sy,
                      int32_t sw,
                      int32_t sh,
                      uint16_t* dst,
                      int32_t dx,
                      int32_t dy,
                      int32_t dw,
                      int32_t dh,
                      int32_t dpitch,
                      bool hflip,
                      bool vflip)
        { THISCALL_14(void, 0x47B870, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        NH3API_FORCEINLINE
        // Draw route arrow shadow /
        // Нарисовать тень стрелки хода.
        void DrawTileShadow(int32_t framenum,
                            int32_t sx,
                            int32_t sy,
                            int32_t sw,
                            int32_t sh,
                            uint16_t* dst,
                            int32_t dx,
                            int32_t dy,
                            int32_t dw,
                            int32_t dh,
                            int32_t dpitch,
                            bool hflip,
                            bool vflip)
        { THISCALL_14(void, 0x47B8C0, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        NH3API_FORCEINLINE
        // Draw 'shroud'(unexplored land stellar sprite) tile /
        // Нарисовать тайл неразведанной территории.
        void DrawShroudTile(int32_t framenum,
                            int32_t sx,
                            int32_t sy,
                            int32_t sw,
                            int32_t sh,
                            uint16_t* dst,
                            int32_t dx,
                            int32_t dy,
                            int32_t dw,
                            int32_t dh,
                            int32_t dpitch,
                            bool hflip,
                            bool vflip)
        { THISCALL_14(void, 0x47B910, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        NH3API_FORCEINLINE
        // Draw adventure map hero /
        // Нарисовать героя на карте.
        void DrawHero(int32_t seqnum,
                      int32_t framenum,
                      int32_t sx,
                      int32_t sy,
                      int32_t sw,
                      int32_t sh,
                      uint16_t* dst,
                      int32_t dx,
                      int32_t dy,
                      int32_t dw,
                      int32_t dh,
                      int32_t dpitch,
                      bool hflip)
        { THISCALL_14(void, 0x47B9A0, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw adventure map hero shadow /
        // Нарисовать тень героя на карте.
        void DrawHeroShadow(int32_t seqnum,
                            int32_t framenum,
                            int32_t sx,
                            int32_t sy,
                            int32_t sw,
                            int32_t sh,
                            uint16_t* dst,
                            int32_t dx,
                            int32_t dy,
                            int32_t dw,
                            int32_t dh,
                            int32_t dpitch,
                            bool hflip)
        { THISCALL_14(void, 0x47B9F0, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        void DrawHeroAlpha(int32_t seqnum,
                           int32_t framenum,
                           int32_t sx,
                           int32_t sy,
                           int32_t sw,
                           int32_t sh,
                           uint16_t* dst,
                           int32_t dx,
                           int32_t dy,
                           int32_t dw,
                           int32_t dh,
                           int32_t dpitch,
                           bool hflip)
        { THISCALL_14(void, 0x47BA40, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        NH3API_FORCEINLINE
        // Draw combat spell effect /
        // Нарисовать эффект заклинания в битве.
        void DrawSpellEffect(int32_t seqnum,
                             int32_t framenum,
                             int32_t sx,
                             int32_t sy,
                             int32_t sw,
                             int32_t sh,
                             uint16_t* dst,
                             int32_t dx,
                             int32_t dy,
                             int32_t dw,
                             int32_t dh,
                             int32_t dpitch,
                             bool hflip,
                             bool alpha)
        { THISCALL_15(void, 0x47BA90, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, alpha); }

        NH3API_FORCEINLINE
        // Number of sequences of each sprite type /
        // Кол-во групп каждого типа спрайта.
        static int32_t GetNumSeqs(EResourceType type)
        { return FASTCALL_1(int32_t, 0x47BAF0, type); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(CSprite)

    public:
        // Sprite frame sequences /
        // Группы кадров спрайта.
        // offset: +0x1C = +28,  size = 0x4 = 4
        CSequence** s;

        // 16-bit palette /
        // 16 битная палитра.
        // offset: +0x20 = +32,  size = 0x4 = 4
        TPalette16* p16;

        // 24-bit palette /
        // 24 битная палитра.
        // offset: +0x24 = +36,  size = 0x4 = 4
        TPalette24* p24;

        // Sprite frame sequences number /
        // Кол-во групп кадров спрайта.
        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t numSequences;

        // Sequences that have frames /
        // Группы, в которых есть кадры.
        // offset: +0x2C = +44,  size = 0x4 = 4
        bool32_t* validSeqMask;

        // Sprite width /
        // Ширина спрайта.
        // offset: +0x30 = +48,  size = 0x4 = 4
        int32_t Width;

        // Sprite height /
        // Высота спрайта.
        // offset: +0x34 = +52,  size = 0x4 = 4
        int32_t Height;

};
#pragma pack(pop)

NH3API_FORCEINLINE uint16_t convert555to565(uint16_t arg)
{ return (arg & 31) | (2 * (arg & 0x7FE0)); }

NH3API_FORCEINLINE uint32_t color1555to8888(uint16_t arg)
{ return (8 * (arg & 0x1F)) | ((arg & 0x3E0) << 6) | ((arg & 0x7C00) << 9) | ((arg & 0x8000) << 16); }

NH3API_FORCEINLINE uint32_t color0565to8888(uint16_t arg)
{ return (8 * (arg & 0x1F)) | (32 * (arg & 0x7E0)) | ((arg & 0xF800) << 8); }

NH3API_FORCEINLINE uint16_t color8888to1555(uint32_t arg)
{ return ((arg & 0x80000000) >> 16) | ((arg & 0xF80000) >> 9) | ((uint16_t)(arg & 0xF800) >> 6) | ((uint8_t)(arg & 0xF8) >> 3); }

NH3API_FORCEINLINE uint16_t color8888to0565(uint32_t arg)
{ return ((arg & 0xF80000) >> 8) | ((uint16_t)(arg & 0xFC00) >> 5) | ((uint8_t)(arg & 0xF8) >> 3); }

struct Bitmap816;
#pragma pack(push, 4)
// 16 Bit bitmap(.pcx) resource /
// Игровой ресурс - 16-ти битное изображение(.pcx).
// size = 0x38 = 56, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap16Bit : public resource
{
    public:
        NH3API_FORCEINLINE
        Bitmap16Bit(int32_t w, int32_t h) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap16Bit, resource)
        { THISCALL_3(void, 0x44DC40, this, w, h); }

        NH3API_FORCEINLINE
        Bitmap16Bit(const char* name, int32_t w, int32_t h) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap16Bit, resource)
        { THISCALL_4(void, 0x44DD20, this, name, w, h); }

        NH3API_FORCEINLINE
        Bitmap16Bit(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag)
        { NH3API_IGNORE(DataSize, ImageSize, Width, Height, Pitch, map, keepData); }

        NH3API_FORCEINLINE
        ~Bitmap16Bit()
        { THISCALL_1(void, 0x5BBF30, this); }

    public:
        NH3API_FORCEINLINE
        uint16_t* GetMap()
        { return map; }

        NH3API_FORCEINLINE
        const uint16_t* GetMap() const
        { return map; }

        NH3API_FORCEINLINE
        int32_t GetWidth() const
        { return Width; }

        NH3API_FORCEINLINE
        int32_t GetHeight() const
        { return Width; }

        NH3API_FORCEINLINE
        int32_t GetPitch() const
        { return Pitch; }

        NH3API_FORCEINLINE
        void Grab(uint16_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
        { THISCALL_7(void, 0x44E0C0, this, src, sx, sy, sw, sh, spitch); }

        NH3API_FORCEINLINE
        void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, Bitmap16Bit* dst, int32_t dx, int32_t dy, bool alpha)
        { Draw(sx, sy, sw, sh, dst->GetMap(), dx, dy, dst->GetWidth(), dst->GetHeight(), dst->GetPitch(), alpha); }

        NH3API_FORCEINLINE
        void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, uint16_t* dst, int32_t dx, int32_t dy, int32_t dw, int32_t dh, int32_t dpitch, bool alpha)
        { THISCALL_12(void, 0x44DF80, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, alpha); }

        NH3API_FORCEINLINE
        void Grab(const uint16_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
        { THISCALL_7(void, 0x44E0C0, this, src, sx, sy, sw, sh, spitch); }

        NH3API_FORCEINLINE
        void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E190, this, x, y, w, h, color); }

        NH3API_FORCEINLINE
        void FrameRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E210, this, x, y, w, h, color); }

        NH3API_FORCEINLINE
        void Darken(int32_t x, int32_t y, int32_t w, int32_t h)
        { THISCALL_5(void, 0x44E2C0, this, x, y, w, h); }

        NH3API_FORCEINLINE
        void Darken(int32_t x, int32_t y, int32_t w, int32_t h, Bitmap816* mask, int32_t sx, int32_t sy)
        { THISCALL_8(void, 0x44E370, this, x, y, w, h, mask, sx, sy); }

        NH3API_FORCEINLINE
        void Colorize(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E450, this, x, y, w, h, color); }

        NH3API_FORCEINLINE
        void Colorize(int32_t x, int32_t y, int32_t w, int32_t h, float hue, float saturation)
        { THISCALL_7(void, 0x44E610, this, x, y, w, h, hue, saturation); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(Bitmap16Bit)

    // member variables
    public:
        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t DataSize;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t ImageSize;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t Width;

        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t Height;

        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t Pitch;

        // offset: +0x30 = +48,  size = 0x4 = 4
        uint16_t* map;

        // offset: +0x34 = +52,  size = 0x1 = 1
        bool keepData;

};
#pragma pack(pop)

#pragma pack(push, 4)
// 16 Bit bitmap(.pcx) resource /
// Игровой ресурс - 16-ти битное изображение(.pcx).
// size = 0x56C = 1388, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap816 : public resource
{
    public:
        NH3API_FORCEINLINE
        Bitmap816(const char* name,
                  int32_t w,
                  int32_t h,
                  uint8_t* data,
                  TPalette16* p16,
                  int32_t csize) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap816, resource)
        { THISCALL_7(void, 0x44F4D0, this, name, w, h, data, p16, csize); }

        NH3API_FORCEINLINE
        Bitmap816(const char* name,
                  const char* path,
                  int32_t rbits,
                  int32_t rshift,
                  int32_t gbits,
                  int32_t gshift,
                  int32_t bbits,
                  int32_t bshift) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap816, resource)
        { THISCALL_9(void, 0x44F5A0, this, name, path, rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        Bitmap816(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag), Palette(tag), Palette24(tag)
        { NH3API_IGNORE(DataSize, ImageSize, Width, Height, Pitch, map); }

        NH3API_FORCEINLINE
        ~Bitmap816() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x44F6A0, this); }

    public:
        NH3API_FORCEINLINE
        void Draw(int32_t sx,
                  int32_t sy,
                  int32_t sw,
                  int32_t sh,
                  Bitmap16Bit* dst,
                  int32_t dx,
                  int32_t dy,
                  bool tblit)
        { THISCALL_9(void, 0x44FA80, this, sx, sy, sw, sh, dst, dx, dy, tblit); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(Bitmap816)

    // member variables
    public:
        // offset: +0x1C = +28,  size = 0x4 = 4
        int32_t DataSize;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t ImageSize;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t Width;

        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t Height;

        // offset: +0x2C = +44,  size = 0x4 = 4
        int32_t Pitch;

        // offset: +0x30 = +48,  size = 0x4 = 4
        uint16_t* map;

        // offset: +0x34 = +52,  size = 0x21C = 540
        TPalette16 Palette;

        // offset: +0x250 = +592,  size = 0x31C = 796
        TPalette24 Palette24;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x3C = 60, align = 4, baseclass: Bitmap16Bit
NH3API_VIRTUAL_STRUCT CTextEntrySave : public Bitmap16Bit
{
    public:
        NH3API_FORCEINLINE
        CTextEntrySave(int32_t w, int32_t h) NH3API_NOEXCEPT
            : Bitmap16Bit(w, h), saved(false)
        { NH3API_SET_VFTABLE(); }

        NH3API_FORCEINLINE
        CTextEntrySave(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : Bitmap16Bit(tag)
        { NH3API_IGNORE(saved); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(CTextEntrySave)

    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x1 = 1
        bool saved;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Used only in ResourceManager::GetBitmap16 /
// Используется внутри ResourceManager::GetBitmap16.
// size = 0x30 = 48, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap24Bit : public resource
{
    public:
        NH3API_FORCEINLINE
        Bitmap24Bit(const char* name, int32_t w, int32_t h, const uint8_t* data, int32_t size)
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap24Bit, resource) // resource(name, RType_bitmap24)
        { THISCALL_6(void, 0x44EA20, this, name, w, h, data, size); }

        NH3API_FORCEINLINE
        Bitmap24Bit(const char* name, const char* path)
        NH3API_DELEGATE_DUMMY_OR_BASE(Bitmap24Bit, resource) // resource(name, RType_bitmap24)
        { THISCALL_3(void, 0x44EAD0, this, name, path); }

        NH3API_FORCEINLINE
        Bitmap24Bit(const nh3api::dummy_tag_t& tag)
            : resource(tag)
        {}

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(Bitmap24Bit)

    // member variables
    public:
        // offset: +0x1C = +28,  size = 0x4 = 4
        size_t DataSize;

        // offset: +0x20 = +32,  size = 0x4 = 4
        int32_t ImageSize;

        // offset: +0x24 = +36,  size = 0x4 = 4
        int32_t Width;

        // offset: +0x28 = +40,  size = 0x4 = 4
        int32_t Height;

        // offset: +0x2C = +44,  size = 0x4 = 4
        uint8_t* data;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Font resource /
// Игровой ресурс - шрифт.
// size = 0x1260 = 4704, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT font : public resource
{
    public:
        // Font settings /
        // Настройки шрифта.
        // size = 0x1020 = 4128, align = 4
        struct TFontSpec
        {
        public:
            // size = 0xC = 12, align = 4
            struct myABC
            {
                // Amount of pixels left of the character /
                // Кол-во пикселей перед символом.
                // offset: +0x0 = +0,  size = 0x4 = 4
                int32_t abcA;

                // Character width in pixels /
                // Ширина символа в пикселях.
                // offset: +0x4 = +4,  size = 0x4 = 4
                int32_t abcB;

                // Amount of pixels right of the character /
                // Кол-во пикселей после символа.
                // offset: +0x8 = +8,  size = 0x4 = 4
                int32_t abcC;
            };

        public:
            // offset: +0x0 = +0,  size = 0x1 = 1
            uint8_t first;

            // offset: +0x1 = +1,  size = 0x1 = 1
            uint8_t last;

            // offset: +0x2 = +2,  size = 0x1 = 1
            uint8_t depth;

            // offset: +0x3 = +3,  size = 0x1 = 1
            int8_t xspace;

            // offset: +0x4 = +4,  size = 0x1 = 1
            int8_t yspace;

            // offset: +0x5 = +5,  size = 0x1 = 1
            uint8_t height;

            // offset: +0x6 = +6,  size = 0x1 = 1
            int8_t baseyoffset;

            // offset: +0x7 = +7,  size = 0x1 = 1
            int8_t pad;

            // offset: +0x8 = +8,  size = 0x4 = 4
            int32_t numpal;

            // offset: +0xC = +12,  size = 0x14 = 20
            ::std::array<uint16_t, 10> pal;

            // offset: +0x20 = +32,  size = 0xC00 = 3072
            ::std::array<myABC, 256> abc;

            // offset: +0xC20 = +3104,  size = 0x400 = 1024
            ::std::array<uint32_t, 256> Offset;

        };

        // Font color /
        // Цвет шрифта.
        enum TColor : int32_t
        {
            PRIMARY                = 1,
            PRIMARY_HIGHLIGHT      = 2,
            PRIMARY_DIM            = 3,
            WHITE                  = 4,
            WHITE_HIGHLIGHT        = 5,
            WHITE_DIM              = 6,
            HEADING                = 7,
            HEADING_HIGHLIGHT      = 8,
            HEADING_DIM            = 9,
            WHITE_PLAYER           = 10,
            WHITE_PLAYER_HIGHLIGHT = 11,
            WHITE_PLAYER_DIM       = 12,
            CHAT                   = 13,
            CHAT_HIGHLIGHT         = 14,
            CHAT_DIM               = 15,
            LowestColor            = 1,
            HighestColor           = 14,
            CUSTOM_COLOR           = 256,
        };

        // Font justification /
        // Выравнивание шрифта.
        enum EJustify : int32_t
        {
            LEFT_JUSTIFIED         = 0, // Выравнивание по левому краю
            CENTER_JUSTIFIED       = 1, // Выравнивание по центру
            RIGHT_JUSTIFIED        = 2, // Выравнивание по правому краю
            VERT_CENTER_JUSTIFIED  = 4,
            BOTTOM_JUSTIFIED       = 8,
        };

    public:
        NH3API_FORCEINLINE
        font(const char* name, const TFontSpec& fontspec, int32_t dsize, bool* d) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(font, resource)
        { THISCALL_5(void, 0x4B4DD0, this, name, &fontspec, dsize, d); }

        NH3API_FORCEINLINE
        font(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag), p16(tag)
        { NH3API_IGNORE(Data, DataSize); }

        NH3API_FORCEINLINE
        ~font()
        { THISCALL_1(void, 0x4B4E70, this); }

    public:
        void SetPalette(const TPalette16& other)
        { THISCALL_2(void, 0x4B4EE0, this, &other); }

        // Draw one character /
        // Нарисовать один символ.
        void DrawCharacter(char c, Bitmap16Bit* bmp, int32_t x, int32_t y, int32_t color)
        { THISCALL_6(void, 0x4B4F00, this, c, bmp, x, y, color); }

        int32_t GetColor(TColor color_scheme, bool highlighted)
        {
            if ( (color_scheme & CUSTOM_COLOR) != 0 )
                return color_scheme & ~CUSTOM_COLOR;
            if ( highlighted && (color_scheme == PRIMARY || color_scheme == WHITE || color_scheme == HEADING) )
                return color_scheme + WHITE_PLAYER;
            else
                return color_scheme + HEADING_DIM;
        }

        // Draw one character. Use GetColor() for <color> /
        // Нарисовать символ курсора. Используйте GetColor() для <color>.
        void DrawCursor(Bitmap16Bit* bitmap, int32_t x, int32_t y, int32_t color, bool highlighted)
        { DrawCharacter('_', bitmap, x, y, color + highlighted); }

        // Draw string /
        // Нарисовать строку.
        void DrawBoundedString(const char* str, Bitmap16Bit* bitmap, int32_t x, int32_t y, int32_t boxWidth, int32_t boxHeight, TColor color_scheme, uint32_t justification, int32_t cursorPos)
        { THISCALL_10(void, 0x4B51F0, this, str, bitmap, x, y, boxWidth, boxHeight, color_scheme, justification, cursorPos); }

        // Character width in pixels /
        // Длина символа(в пикселях).
        int32_t GetCharacterWidth(char currChar) const
        { return this->fr.abc[currChar].abcA + this->fr.abc[currChar].abcB + this->fr.abc[currChar].abcC; }

        // String width in pixels /
        // Длина строки(в пикселях).
        int32_t get_string_width(const char* arg) const
        { int32_t sum = 0; char c; while(*arg) { c = *arg++; sum += GetCharacterWidth(c); } return sum; }

        int32_t LineLength(const char* str, int32_t boxWidth) const
        { return THISCALL_3(int32_t, 0x4B5580, this, str, boxWidth); }

        int32_t LineWidth(const char* text) const
        { return THISCALL_2(int32_t, 0x4B5680, this, text);}

        int32_t LongestLineWidth(const char* str) const
        { return THISCALL_2(int32_t, 0x4B56F0, this, str);}

        // Length of the longest word in pixels /
        // Длина самого длинного слова(в пикселях).
        int32_t longest_word_length(const char* str) const
        { return THISCALL_2(int32_t, 0x4B5770, this, str); }

        int32_t LongestWrappedLineWidth(const char* str, int32_t boxWidth) const
        { return THISCALL_3(int32_t, 0x4B57E0, this, str, boxWidth); }

        void FillLinesVector(const char* str, int32_t boxWidth, exe_vector<exe_string>& result) const
        { THISCALL_4(void, 0x4B58F0, this, str, boxWidth, &result); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(font)

    // member variables
    public:
        // Font resource settings /
        // Настройки шрифта.
        // offset: +0x1C = +28,  size = 0x1020 = 4128
        TFontSpec fr;

        // 16-bit palette /
        // 16-битная палитра.
        // offset: +0x103C = +4156,  size = 0x21C = 540
        TPalette16 p16;

        // Font raw data /
        // Данные шрифта.
        // offset: +0x1258 = +4696,  size = 0x4 = 4
        uint8_t* Data;

        // Font raw data size /
        // Размер данных шрифта.
        // offset: +0x125C = +4700,  size = 0x4 = 4
        size_t DataSize;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
font*& tinyFont // крошечный шрифт
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698A54, font*));

NH3API_INLINE_OR_EXTERN
font*& smallFont // маленький шрифт
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698A58, font*));

NH3API_INLINE_OR_EXTERN
font*& medFont // средний(основной для текста) шрифт
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698A5C, font*));

NH3API_INLINE_OR_EXTERN
font*& bigFont // большой шрифт
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698A60, font*));

NH3API_INLINE_OR_EXTERN
font*& caligFont // каллиграфический шрифт
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x698A64, font*));

#pragma pack(push, 4)
// Help text /
// Текст подсказки..
// size = 0x8 = 8, align = 4
struct THelpText
{
    // Cursor rollover text(drawn at the bottom of the window) /
    // Текст при наведении(отображается внизу).
    // offset: +0x0 = +0,  size = 0x4 = 4
    const char* Rollover;

    // Right click text(drawn in the modal window) /
    // Текст при ПКМ(отображается в модальном окошке).
    // offset: +0x4 = +4,  size = 0x4 = 4
    const char* RightClick;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Text resource /
// Игровой ресурс - текстовый файл.
// size = 0x30 = 48, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TTextResource : public resource
{
    public:
        NH3API_FORCEINLINE
        TTextResource(const char* name, int32_t size, const char* data) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TTextResource, resource)
        { THISCALL_4(void,  0x5BBF90, this, name, size, data); }

        TTextResource(const nh3api::dummy_tag_t& tag)
            : resource(tag), Text(tag)
        {}

        const char* GetText(size_t pos) const
        { return Text[pos]; }

        const char* operator[](size_t pos) const
        { return Text[pos]; }

        uint32_t GetNumberOfStrings() const
        { return Text.size(); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TTextResource)

    // member variables
    public:
        // Test rows /
        // Строки текста.
        // offset: +0x1C = +28,  size = 0x10 = 16
        exe_vector<char*> Text;

        // Source data /
        // Оригинальные данные.
        // offset: +0x2C = +44,  size = 0x4 = 4
        char* Data;

};
#pragma pack(pop)

NH3API_INLINE_OR_EXTERN
const TTextResource* const& GameText // genrltxt.txt, usage: GameText->GetText(42)
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6A5DC4, const TTextResource* const));

NH3API_INLINE_OR_EXTERN
const TTextResource* const& ArrayText // arraytxt.txt, usage: ArrayText->GetText(42)
NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ref(0x6A60AC, const TTextResource* const));

#pragma pack(push, 4)
// Spreadsheet resource /
// Игровой ресурс - текстовая таблица.
// size = 0x34 = 52, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TSpreadsheetResource : public resource
{
    public:
        TSpreadsheetResource(const char* name, int32_t size, const char* data)
        NH3API_DELEGATE_DUMMY_OR_BASE(TSpreadsheetResource, resource) // resource(name, RType_text)
        { THISCALL_4(void, 0x5BC260, this, name, size, data); }

        TSpreadsheetResource(const nh3api::dummy_tag_t& tag)
            : resource(tag), SpreadSheet(tag)
        {}

        ~TSpreadsheetResource()
        { THISCALL_1(void, 0x5BC560, this); }

        // Number Of columns /
        // Количество столбцов.
        int32_t GetNumberOfColumns(size_t r) const
        { return SpreadSheet[r]->size(); }

         // Number of rows /
        // Количество строк.
        int32_t GetNumberOfRows() const
        { return SpreadSheet.size(); }

        // Get spreadsheet cell (intersection of row <r> and column <c>) /
        // Колонка таблицы на пересечении строки <r> и столбца <c>
        char const* GetSpreadsheet(size_t r, size_t c) const
        { return SpreadSheet[r]->operator[](c); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TSpreadsheetResource)

    // member variables
    public:
        // Spreadsheet /
        // Таблица.
        // offset: +0x1C = +28,  size = 0x10 = 16
        exe_vector<exe_vector<char *>* > SpreadSheet;

        // Source data /
        // Оригинальные данные.
        // offset: +0x2C = +44,  size = 0x4 = 4
        char* Data;

        // Source data size /
        // Размер оригинальных данных.
        // offset: +0x30 = +48,  size = 0x4 = 4
        size_t DataSize;

};
#pragma pack(pop)

struct sample;
class LODFile;
namespace ResourceManager
{

#pragma pack(push, 1)
// Resource map cached resources key /
// Ключ-идентификатор ресурса в дереве кэшированных ресурсов.
// size = 0xD = 13, align = 1
struct TCacheMapKey
{
    public:
        TCacheMapKey(const char * _name) NH3API_NOEXCEPT
        { ::nh3api::constexpr_char_traits::copy(name.data(), _name, 12); name.back() = '\0'; }

        bool operator<(TCacheMapKey const& rhf) const NH3API_NOEXCEPT
        { return ::nh3api::case_insensitive_traits::compare(this->name.data(), rhf.name.data(), this->name.size()) < 0; }

    public:
        // Resource name /
        // Длина ресурса.
        // offset: +0x0 = +0,  size = 0xC = 12
        ::std::array<char, 13> name;
};
#pragma pack(pop)

typedef exe_map<TCacheMapKey, resource*, 0x69E604, 0x69E608> TResourceMap;
NH3API_FORCEINLINE TResourceMap& GetResourceMap() NH3API_NOEXCEPT
{ return get_global_var_ref(0x69E560, TResourceMap);}

NH3API_FORCEINLINE Bitmap816* GetBitmap816(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(Bitmap816*, 0x55AA10, name); }

NH3API_FORCEINLINE Bitmap16Bit* GetBitmap16(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(Bitmap16Bit*, 0x55AE50, name); }

NH3API_FORCEINLINE TPalette16* GetPalette(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(TPalette16*, 0x55B5F0, name); }

NH3API_FORCEINLINE TPalette24* GetPalette24(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(TPalette24*, 0x55B680, name); }

NH3API_FORCEINLINE font* GetFont(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(font*, 0x55BAE0, name); }

NH3API_FORCEINLINE TTextResource* GetText(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(TTextResource*, 0x55BFE0, name); }

NH3API_FORCEINLINE TSpreadsheetResource* GetSpreadsheet(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(TSpreadsheetResource*, 0x55C2B0, name); }

NH3API_FORCEINLINE sample* GetSample(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(sample*, 0x55C930, name); }

NH3API_FORCEINLINE CSprite* GetSprite(const char* name) NH3API_NOEXCEPT
{ return FASTCALL_1(CSprite*, 0x55C9C0, name); }

// templated version for convenience

template <typename T> NH3API_FORCEINLINE
T* GetResource(const char* name)
{
    NH3API_STATIC_ASSERT("ResourceManager::GetResource: Invalid resource", 0);
    return nullptr;
}

template <> NH3API_FORCEINLINE
Bitmap816* GetResource<Bitmap816>(const char* name)
{ return GetBitmap816(name); }

template <> NH3API_FORCEINLINE
Bitmap16Bit* GetResource<Bitmap16Bit>(const char* name)
{ return GetBitmap16(name); }

template <> NH3API_FORCEINLINE
TPalette16* GetResource<TPalette16>(const char* name)
{ return GetPalette(name); }

template <> NH3API_FORCEINLINE
TPalette24* GetResource<TPalette24>(const char* name)
{ return GetPalette24(name); }

template <> NH3API_FORCEINLINE
font* GetResource<font>(const char* name)
{ return GetFont(name); }

template <> NH3API_FORCEINLINE
TTextResource* GetResource<TTextResource>(const char* name)
{ return GetText(name); }

template <> NH3API_FORCEINLINE
TSpreadsheetResource* GetResource<TSpreadsheetResource>(const char* name)
{ return GetSpreadsheet(name); }

template <> NH3API_FORCEINLINE
sample* GetResource<sample>(const char* name)
{ return GetSample(name); }

template <> NH3API_FORCEINLINE
CSprite* GetResource<CSprite>(const char* name)
{ return GetSprite(name); }

NH3API_FORCEINLINE void GetBackdrop(const char* resName, Bitmap16Bit* destBmap)
{ FASTCALL_2(void, 0x55D110, resName, destBmap); }

NH3API_FORCEINLINE LODFile* PointToSpriteResource(const char* name)
{ return THISCALL_1(LODFile*, 0x55D160, name); }

NH3API_FORCEINLINE LODFile* PointToBitmapResource(const char* name)
{ return THISCALL_1(LODFile*, 0x55D1F0, name); }

NH3API_FORCEINLINE size_t GetBitmapResourceSize(const char* name)
{ return THISCALL_1(size_t, 0x55D280, name); }

NH3API_FORCEINLINE int32_t ReadFromBitmapResource(LODFile* lod, uint8_t* data, size_t numBytes)
{ return FASTCALL_3(int32_t, 0x55D2E0, lod, data, numBytes); }

NH3API_FORCEINLINE int32_t ReadFromSpriteResource(LODFile* lod, uint8_t* data, size_t numBytes)
{ return FASTCALL_3(int32_t, 0x55D2E0, lod, data, numBytes); }

NH3API_FORCEINLINE void Dispose(resource* arg)
{ arg->Dispose(); }

NH3API_FORCEINLINE exe_string& GetDataPath()
{ return get_global_var_ref(0x69E528, exe_string); }

} // namespace ResourceManager

#if NH3API_STD_HASH
// std::hash support for ResourceManager::TCacheMapKey
template<>
struct std::hash< ResourceManager::TCacheMapKey >
{
    public:
        size_t operator()(const ResourceManager::TCacheMapKey& key) NH3API_NOEXCEPT
        {
            return ::nh3api::hash_string(key.name.data(), ::nh3api::safe_strlen(key.name.data(), key.name.size()));
        }
};
#endif

NH3API_DISABLE_WARNING_END
