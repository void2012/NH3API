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

#include <string_view>

#include "../nh3api_std/exe_map.hpp"     // exe_map
#include "../nh3api_std/exe_string.hpp"  // exe_string, nh3api::default_hash
#include "../nh3api_std/exe_vector.hpp"  // exe_vector
#include "resource_enums.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#ifndef NH3API_VIRTUAL_OVERRIDE_RESOURCE
#define NH3API_VIRTUAL_OVERRIDE_RESOURCE(CLASS_NAME) \
void __thiscall scalar_deleting_destructor(uint8_t flag) override \
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
void __thiscall Dispose() override\
{ return get_type_vftable(this)->Dispose(this); }\
[[nodiscard]] size_t __thiscall GetSize() const override\
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
            void   (__thiscall* scalar_deleting_destructor)(resource*, uint8_t);
            void   (__thiscall* Dispose)(resource*);
            size_t (__thiscall* GetSize)(const resource*);
        };

    // constructors and destructor
    public:
        inline resource(const char* const newName, EResourceType newType) noexcept
            : resource(nh3api::dummy_tag)
        { THISCALL_3(void, 0x558970, this, newName, newType); }

        // dummy constructor
        inline resource(const nh3api::dummy_tag_t&) noexcept
        {}

        inline ~resource() noexcept
        { THISCALL_1(void, 0x5589F0, this); }

        resource()                                            = delete;
        resource(const resource&)                             = delete;
        resource& operator=(const resource&)                  = delete;
        inline resource(resource && other) noexcept           = default;
        inline resource& operator=(resource&& other) noexcept = default;

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual void __thiscall Dispose()
        { return get_vftable(this)->Dispose(this); }

        // vftable shift: +8
        [[nodiscard]] virtual size_t __thiscall GetSize() const
        { return get_vftable(this)->GetSize(this); }

    public:
        [[nodiscard]] EResourceType get_resType() const noexcept
        { return resType; }

        [[nodiscard]] const char* get_Name() const noexcept
        { return Name.data(); }

        [[nodiscard]] char* get_Name() noexcept
        { return Name.data(); }

        int32_t AddRef() noexcept
        { return ++ReferenceCount; }

        int32_t Release() noexcept
        {
            if ( ReferenceCount > 0 )
                --ReferenceCount;
            return ReferenceCount;
        }

        [[nodiscard]] int32_t GetReferenceCount() const noexcept
        { return ReferenceCount; }

    // member variables
    public:
        // Resource name /
        // Название ресурса.
        // offset: +0x4 = +4,  size = 0xD = 13
        std::array<char, 13> Name;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // Resource type /
        // Тип ресурса
        // offset: +0x14 = +20,  size = 0x4 = 4
        EResourceType resType;

        // Reference count /
        // Количество ссылок.
        // offset: +0x18 = +24,  size = 0x4 = 4
        int32_t ReferenceCount;
} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

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

// size = 0x4 = 4, align = 1
struct TRGBA : public TRGB
{
    // Alpha channel /
    // Альфа-канал
    // offset: +0x3 = +3,  size = 0x1 = 1
    uint8_t Alpha;
};
#pragma pack(pop) // 1

#pragma pack(push, 4)
// 24 Bit palette resource /
// Игровой ресурс - 24-х битная палитра.
// size = 0x31C = 796, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TPalette24 : public resource
{
    public:
        inline TPalette24() noexcept
            : TPalette24(nh3api::dummy_tag)
        { THISCALL_1(void, 0x523320, this); }

        inline TPalette24(const TRGB* data) noexcept
            : TPalette24(nh3api::dummy_tag)
        { THISCALL_2(void, 0x523370, this, data); }

        inline TPalette24(const TRGBA* rgba) noexcept
            : TPalette24(nh3api::dummy_tag)
        { THISCALL_2(void, 0x5233A0, this, rgba); }

        inline TPalette24(const TPalette24& other) noexcept
            : TPalette24(nh3api::dummy_tag)
        { THISCALL_2(void, 0x5233F0, this, &other); }

        inline TPalette24(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag) // resource(nullptr, RType_misc)
        {}

        inline TPalette24& operator=(TPalette24 const& other) noexcept
        {
            if ( this != &other )
                THISCALL_2(void, 0x523420, this, &other);

            return *this;
        }

        inline ~TPalette24() noexcept
        { THISCALL_1(void, 0x523450, this); }

        inline TPalette24(TPalette24&&)            noexcept = default;
        inline TPalette24& operator=(TPalette24&&) noexcept = default;

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TPalette24)

    public:
        void AdjustHSV(float hue, float hue_adjust, float saturation_adjust, float value_adjust)
        { THISCALL_5(void, 0x523470, this, hue, hue_adjust, saturation_adjust, value_adjust); }

        TRGB* data() noexcept
        { return Palette.data(); }

        [[nodiscard]] const TRGB* data() const noexcept
        { return Palette.data(); }

    public:
        // 24-bit Palette /
        // 24-битная палитра.
        // offset: +0x1C = +28,  size = 0x300 = 768
        std::array<TRGB, 256> Palette;

} NH3API_MSVC_LAYOUT;

// in: r, g, b
// out: h, s, v
inline void RGBToHSV(uint32_t r,
                     uint32_t g,
                     uint32_t b,
                     float& __restrict h,
                     float& __restrict s,
                     float& __restrict v) noexcept
{ FASTCALL_6(void, 0x523680, r, g, b, &h, &s, &v); }

// in: rgb
// out: h, s, v
inline void RGBToHSV(TRGB rgb,
                     float& __restrict h,
                     float& __restrict s,
                     float& __restrict v) noexcept
{ FASTCALL_6(void, 0x523680, rgb.Red, rgb.Green, rgb.Blue, &h, &s, &v); }

// in: h, s, v
// out: r, g, b
inline void HSVToRGB(float h,
                     float s,
                     float v,
                     uint32_t& __restrict r,
                     uint32_t& __restrict g,
                     uint32_t& __restrict b) noexcept
{ FASTCALL_6(void, 0x5237E0, h, s, v, &r, &g, &b); }

inline TPalette24* const& gPlayerPalette24 = get_global_var_ref(0x6AAD14, TPalette24*);

// 16 Bit palette resource /
// Игровой ресурс - 16-ти битная палитра.
// size = 0x21C = 540, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TPalette16 : public resource
{
    public:
        inline TPalette16() noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_1(void, 0x522B40, this); }

        inline TPalette16(const TPalette24& p24) noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_2(void, 0x522D00, this, &p24); }

        inline TPalette16(const TPalette16& other) noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_2(void, 0x522DD0, this, &other); }

        inline TPalette16(const uint16_t* data) noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_2(void, 0x522B90, this, data); }

        inline TPalette16(const TPalette24& p24,
                          uint32_t          rbits,
                          uint32_t          rshift,
                          uint32_t          gbits,
                          uint32_t          gshift,
                          uint32_t          bbits,
                          uint32_t          bshift) noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_8(void, 0x522BC0, this, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        inline TPalette16(const char*       name,
                          const TPalette24& p24,
                          uint32_t          rbits,
                          uint32_t          rshift,
                          uint32_t          gbits,
                          uint32_t          gshift,
                          uint32_t          bbits,
                          uint32_t          bshift) noexcept
            : TPalette16(nh3api::dummy_tag)
        { THISCALL_9(void, 0x522C60, this, name, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        inline TPalette16(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag) // resource(nullptr, RType_misc)
        {}

        inline TPalette16& operator=(const TPalette16& other) noexcept
        { THISCALL_2(void, 0x522E00, this, other); return *this; }

        inline ~TPalette16() noexcept
        { THISCALL_1(void, 0x522E30, this); }

        inline TPalette16(TPalette16&&)            noexcept = default;
        inline TPalette16& operator=(TPalette16&&) noexcept = default;

    public:
        inline void Convert24to16(const TRGB* p24, uint32_t rbits, uint32_t rshift, uint32_t gbits, uint32_t gshift, uint32_t bbits, uint32_t bshift) noexcept
        {
            for ( size_t i = 0; i < Palette.size(); ++i )
                Palette[i] = static_cast<uint16_t>(((p24[i].Blue >> (8u - bbits) << bshift) | (p24[i].Green >> (8u - gbits) << gshift) | (p24[i].Red >> (8u - rbits) << rshift)) & UINT16_MAX);
        }

        inline void Convert24to16(const TPalette24& p24, uint32_t rbits, uint32_t rshift, uint32_t gbits, uint32_t gshift, uint32_t bbits, uint32_t bshift) noexcept
        { Convert24to16(p24.Palette.data(), rbits, rshift, gbits, gshift, bbits, bshift); }

        inline void Cycle(uint32_t begin, uint32_t end, uint32_t step)
        { THISCALL_4(void, 0x522E40, this, begin, end, step); }

        void AdjustSaturation(float amount)
        { THISCALL_2(void, 0x522F00, this, amount); }

        void AdjustHSV(float hue, float hue_adjust, float saturation_adjust, float value_adjust)
        { THISCALL_5(void, 0x523040, this, hue, hue_adjust, saturation_adjust, value_adjust); }

        void Gray()
        { THISCALL_1(void, 0x523240, this); }

        [[nodiscard]] const uint16_t* data() const noexcept
        { return Palette.data(); }

        uint16_t* data() noexcept
        { return Palette.data(); }

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TPalette16)

    public:
        // 16-bit Palette /
        // 16-битная палитра.
        // offset: +0x1C = +28,  size = 0x200 = 512
        std::array<uint16_t, 256> Palette;

};

inline TPalette16* const& gPlayerPalette = get_global_var_ref(0x6AAD10, TPalette16*);
inline TPalette16* const& gGamePalette   = get_global_var_ref(0x6AAD18, TPalette16*);

// .def file (sprite) encoding method /
// Метод кодировки .def-файла (спрайта).
enum TEncodingMethod : uint32_t
{
    eEncodeRaw          = 0, // Без кодировки
    eEncodeGeneralRLE   = 1, // Обычная кодировка
    eEncodeTilesetRLE   = 2, // Кодировка тайлов
    eEncodeAdvObjRLE    = 3, // Кодировка остальных спрайтов
    kNumEncodingMethods = 4  // Кол-во методов кодировки
};

// Sprite frame resource /
// Игровой ресурс - один кадр спрайта(.def).
// size = 0x48 = 72, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT CSpriteFrame : public resource
{
    public:
        inline CSpriteFrame(const char* __restrict name,
                            int32_t w,
                            int32_t h,
                            void* __restrict data,
                            size_t          csize,
                            TEncodingMethod encoding) noexcept
            : CSpriteFrame(nh3api::dummy_tag)
        { THISCALL_7(void, 0x47BBD0, this, name, w, h, data, csize, encoding); }

        inline CSpriteFrame(const char* __restrict name,
                            int32_t w,
                            int32_t h,
                            void* __restrict data,
                            size_t          csize,
                            TEncodingMethod encoding,
                            int32_t         cw,
                            int32_t         ch,
                            int32_t         cx,
                            int32_t         cy) noexcept
            : CSpriteFrame(nh3api::dummy_tag)
        { THISCALL_11(void, 0x47BC80, this, name, w, h, data, csize, encoding, cw, ch, cx, cy); }

        inline CSpriteFrame(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag) // resource(nullptr, RType_misc)
        {}

        inline ~CSpriteFrame()
        { THISCALL_1(void, 0x47BD50, this); }

        CSpriteFrame(const CSpriteFrame&)                       = delete;
        CSpriteFrame& operator=(const CSpriteFrame&)            = delete;
        inline CSpriteFrame(CSpriteFrame&&) noexcept            = default;
        inline CSpriteFrame& operator=(CSpriteFrame&&) noexcept = default;

    public:
        void Clip(int32_t& sx,
                  int32_t& sy,
                  int32_t& sw,
                  int32_t& sh,
                  int32_t& dx,
                  int32_t& dy,
                  int32_t  dw,
                  int32_t  dh,
                  bool     bHFlip,
                  bool     bVFlip) noexcept
        {
            if ( bHFlip )
                sx = this->Width - (sw + sx);

            if ( bVFlip )
                sy = this->Height - (sh + sy);

            if ( dx < 0 )
            {
                if ( !bHFlip )
                    sx -= dx;

                sw += dx;
                dx  = 0;
            }

            if ( dy < 0 )
            {
                if ( !bVFlip )
                    sy -= dy;

                sh += dy;
                dy  = 0;
            }

            if ( sw + dx > dw )
            {
                if ( bHFlip )
                    sx += sw + dx - dw;

                sw = dw - dx;
            }

            if ( sh + dy > dh )
            {
                if ( bVFlip )
                    sy += sh + dy - dh;

                sh = dh - dy;
            }

            if ( sx < this->CroppedX )
            {
                if ( !bHFlip )
                    dx += this->CroppedX - sx;

                sw -= this->CroppedX - sx;
                sx  = this->CroppedX;
            }

            if ( sy < this->CroppedY )
            {
                if ( !bVFlip )
                    dy += this->CroppedY - sy;

                sh -= this->CroppedY - sy;
                sy  = this->CroppedY;
            }

            if ( sw + sx > this->CroppedWidth + this->CroppedX )
            {
                if ( bHFlip )
                    dx += sw + sx - this->CroppedWidth + this->CroppedX;

                sw = (this->CroppedWidth + this->CroppedX) - sx;
            }

            if ( sh + sy > this->CroppedHeight + this->CroppedY )
            {
                if ( bVFlip )
                    dy += sh + sy - this->CroppedHeight + this->CroppedY;

                sh = (this->CroppedHeight + this->CroppedY) - sy;
            }

            sx -= this->CroppedX;
            sy -= this->CroppedY;
        }

        inline static void SetPixelFormat(uint16_t red_mask, uint16_t green_mask, uint16_t blue_mask)
        { FASTCALL_3(void, 0x47BD80, red_mask, green_mask, blue_mask); }

        // Draw combat creature /
        // Нарисовать существо в бою.
        inline void DrawCreature(int32_t     sx,
                                 int32_t     sy,
                                 int32_t     sw,
                                 int32_t     sh,
                                 void*      dst,
                                 int32_t     dx,
                                 int32_t     dy,
                                 int32_t     dw,
                                 int32_t     dh,
                                 int32_t     dpitch,
                                 TPalette16& pal,
                                 bool        hflip,
                                 uint16_t    flagcolor)
        { THISCALL_14(void, 0x47C300, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, flagcolor); }

        // Draw adventure map object /
        // Нарисовать объект на карте приключений.
        inline void DrawAdvObjImpl(int32_t     sx,
                                   int32_t     sy,
                                   int32_t     sw,
                                   int32_t     sh,
                                   void*      dst,
                                   int32_t     dx,
                                   int32_t     dy,
                                   int32_t     dw,
                                   int32_t     dh,
                                   int32_t     dpitch,
                                   TPalette16& pal,
                                   bool        hflip,
                                   uint16_t    flagcolor)
        { THISCALL_14(void, 0x47C9C0, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, flagcolor); }

        inline void DrawHeroAlpha(int32_t     sx,
                                  int32_t     sy,
                                  int32_t     sw,
                                  int32_t     sh,
                                  void*      dst,
                                  int32_t     dx,
                                  int32_t     dy,
                                  int32_t     dw,
                                  int32_t     dh,
                                  int32_t     dpitch,
                                  TPalette16& pal,
                                  bool        hflip)
        { THISCALL_13(void, 0x47CE10, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip); }

        inline void DrawAdvObjShadowImpl(int32_t     sx,
                                         int32_t     sy,
                                         int32_t     sw,
                                         int32_t     sh,
                                         void*      dst,
                                         int32_t     dx,
                                         int32_t     dy,
                                         int32_t     dw,
                                         int32_t     dh,
                                         int32_t     dpitch,
                                         TPalette16& pal,
                                         bool        hflip)
        { THISCALL_13(void, 0x47D250, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip); }

        // Draw terrain/river/road/route arrow tile /
        // Нарисовать клетку почвы/дорогу/реку/стрелки хода.
        inline void DrawTile(int32_t     sx,
                             int32_t     sy,
                             int32_t     sw,
                             int32_t     sh,
                             void*      dst,
                             int32_t     dx,
                             int32_t     dy,
                             int32_t     dw,
                             int32_t     dh,
                             int32_t     dpitch,
                             TPalette16& pal,
                             bool        hflip,
                             bool        vflip)
        { THISCALL_14(void, 0x47D660, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, vflip); }

        inline void DrawTileShadow(int32_t     sx,
                                   int32_t     sy,
                                   int32_t     sw,
                                   int32_t     sh,
                                   void*      dst,
                                   int32_t     dx,
                                   int32_t     dy,
                                   int32_t     dw,
                                   int32_t     dh,
                                   int32_t     dpitch,
                                   TPalette16& pal,
                                   bool        hFlipped,
                                   bool        vFlipped)
        { THISCALL_14(void, 0x47E140, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hFlipped, vFlipped); }

        inline void DrawSpellEffect(int32_t     sx,
                                    int32_t     sy,
                                    int32_t     sw,
                                    int32_t     sh,
                                    void*      dst,
                                    int32_t     dx,
                                    int32_t     dy,
                                    int32_t     dw,
                                    int32_t     dh,
                                    int32_t     dpitch,
                                    TPalette16& pal,
                                    bool32_t    hflip,
                                    bool        alpha)
        { THISCALL_14(void, 0x47E880, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, &pal, hflip, alpha); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(CSpriteFrame)

    // static variables
    public:
        inline static uint16_t& div2mask = get_global_var_ref(0x6968F4, uint16_t);
        inline static uint16_t& div4mask = get_global_var_ref(0x6968FA, uint16_t);

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
        uint8_t* map;

};

// Sprite frames sequence /
// Последовательность кадров спрайта.
// size = 0xC = 12, align = 4
struct CSequence
{
    public:
        inline CSequence(int32_t num) noexcept
            : CSequence(nh3api::dummy_tag)
        { THISCALL_2(void, 0x47B160, this, num); }

        inline CSequence(const nh3api::dummy_tag_t&) noexcept
        {}

    public:
        int32_t AddFrame(CSpriteFrame* frame) noexcept
        { return THISCALL_2(int32_t, 0x47B1C0, this, frame); }

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

// Sprite resource(.def) /
// Игровой ресурс - спрайт(.def)
// size = 0x38 = 56, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT CSprite : public resource
{
    public:
        inline CSprite(const char* name, EResourceType sprtype, int32_t w, int32_t h) noexcept
            : CSprite(nh3api::dummy_tag)
        { THISCALL_5(void, 0x47B240, this, name, sprtype, w, h); }

        inline CSprite(const nh3api::dummy_tag_t& dummy) noexcept
            : resource(dummy)
        {}

    public:
        inline int32_t AddFrame(size_t seqnum, CSpriteFrame* frame)
        { return THISCALL_3(int32_t, 0x47B480, this, seqnum, frame); }

        inline void AllocateSeq(size_t seqnum, size_t numFrames)
        { THISCALL_3(void, 0x47B410, this, seqnum, numFrames); }

        [[nodiscard]] inline int32_t GetWidth() const noexcept
        { return Width; }

        [[nodiscard]] inline int32_t GetHeight() const noexcept
        { return Height; }

        inline void SetPalette(const TPalette16& pal)
        { THISCALL_2(void, 0x47B4A0, this, &pal); }

        [[nodiscard]] inline const uint16_t* GetPalette() const noexcept
        { return p16 ? p16->Palette.data() : nullptr; }

        inline uint16_t* GetPalette() noexcept
        { return p16 ? p16->Palette.data() : nullptr; }

        inline void ResetPalette()
        { THISCALL_1(void, 0x47B520, this); }

        inline void ColorCycle(uint32_t begin, uint32_t end, uint32_t step)
        { return p16->Cycle(begin, end, step); }

        // Draw general function /
        // Общая функция отрисовки спрайта.
        inline void Draw(int32_t seqnum,
                         int32_t framenum,
                         int32_t sx,
                         int32_t sy,
                         int32_t sw,
                         int32_t sh,
                         void* dst,
                         int32_t dx,
                         int32_t dy,
                         int32_t dw,
                         int32_t dh,
                         int32_t dpitch,
                         bool    hflip,
                         bool    tblit)
        { THISCALL_15(void, 0x47B610, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, tblit); }

        // Draw combat creature /
        // Нарисовать существо в бою.
        inline void DrawCreature(int32_t   seqnum,
                                 int32_t   framenum,
                                 int32_t   sx,
                                 int32_t   sy,
                                 int32_t   sw,
                                 int32_t   sh,
                                 void* dst,
                                 int32_t   dx,
                                 int32_t   dy,
                                 int32_t   dw,
                                 int32_t   dh,
                                 int32_t   dpitch,
                                 bool      hflip,
                                 uint16_t  outcolor)
        { THISCALL_15(void, 0x47B680, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, outcolor); }

        // Draw adventure map object /
        // Нарисовать объект на карте приключений.
        inline void DrawAdvObj(int32_t   frame,
                               int32_t   sx,
                               int32_t   sy,
                               int32_t   sw,
                               int32_t   sh,
                               void* dst,
                               int32_t   dx,
                               int32_t   dy,
                               int32_t   dw,
                               int32_t   dh,
                               int32_t   dpitch,
                               bool      hflip)
        { THISCALL_13(void, 0x47B6E0, this, frame, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw adventure map object with flag /
        // Нарисовать объект с флагом на карте приключений.
        inline void DrawAdvObjWithFlag(int32_t   framenum,
                                       int32_t   sx,
                                       int32_t   sy,
                                       int32_t   sw,
                                       int32_t   sh,
                                       void* dst,
                                       int32_t   dx,
                                       int32_t   dy,
                                       int32_t   dw,
                                       int32_t   dh,
                                       int32_t   dpitch,
                                       uint16_t  flagcolor,
                                       bool      hflip)
        { THISCALL_14(void, 0x47B730, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, flagcolor, hflip); }

        // Draw adventure map object shadow /
        // Нарисовать тень объекта на карте приключений.
        inline void DrawAdvObjShadow(int32_t   framenum,
                                     int32_t   sx,
                                     int32_t   sy,
                                     int32_t   sw,
                                     int32_t   sh,
                                     void* dst,
                                     int32_t   dx,
                                     int32_t   dy,
                                     int32_t   dw,
                                     int32_t   dh,
                                     int32_t   dpitch,
                                     bool      hflip)
        { THISCALL_13(void, 0x47B780, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw mouse cursor /
        // Нарисовать курсор мыши.
        inline void DrawPointer(int32_t   framenum,
                                void* dst,
                                int32_t   dx,
                                int32_t   dy,
                                int32_t   dw,
                                int32_t   dh,
                                int32_t   dpitch,
                                bool      hflip)
        { THISCALL_9(void, 0x47B7D0, this, framenum, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw interface object /
        // Нарисовать объект интерфейса.
        inline void DrawInterface(int32_t   framenum,
                                  int32_t   sx,
                                  int32_t   sy,
                                  int32_t   sw,
                                  int32_t   sh,
                                  void* dst,
                                  int32_t   dx,
                                  int32_t   dy,
                                  int32_t   dw,
                                  int32_t   dh,
                                  int32_t   dpitch,
                                  bool      hflip)
        { THISCALL_13(void, 0x47B820, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw terrain/river/road/route arrow tile /
        // Нарисовать клетку почвы/дорогу/реку/стрелки хода.
        inline void DrawTile(int32_t   framenum,
                             int32_t   sx,
                             int32_t   sy,
                             int32_t   sw,
                             int32_t   sh,
                             void* dst,
                             int32_t   dx,
                             int32_t   dy,
                             int32_t   dw,
                             int32_t   dh,
                             int32_t   dpitch,
                             bool      hflip,
                             bool      vflip)
        { THISCALL_14(void, 0x47B870, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        // Draw route arrow shadow /
        // Нарисовать тень стрелки хода.
        inline void DrawTileShadow(int32_t   framenum,
                                   int32_t   sx,
                                   int32_t   sy,
                                   int32_t   sw,
                                   int32_t   sh,
                                   void* dst,
                                   int32_t   dx,
                                   int32_t   dy,
                                   int32_t   dw,
                                   int32_t   dh,
                                   int32_t   dpitch,
                                   bool      hflip,
                                   bool      vflip)
        { THISCALL_14(void, 0x47B8C0, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        // Draw 'shroud'(unexplored land stellar sprite) tile /
        // Нарисовать тайл неразведанной территории.
        inline void DrawShroudTile(int32_t   framenum,
                                   int32_t   sx,
                                   int32_t   sy,
                                   int32_t   sw,
                                   int32_t   sh,
                                   void* dst,
                                   int32_t   dx,
                                   int32_t   dy,
                                   int32_t   dw,
                                   int32_t   dh,
                                   int32_t   dpitch,
                                   bool      hflip,
                                   bool      vflip)
        { THISCALL_14(void, 0x47B910, this, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, vflip); }

        // Draw adventure map hero /
        // Нарисовать героя на карте.
        inline void DrawHero(int32_t   seqnum,
                             int32_t   framenum,
                             int32_t   sx,
                             int32_t   sy,
                             int32_t   sw,
                             int32_t   sh,
                             void* dst,
                             int32_t   dx,
                             int32_t   dy,
                             int32_t   dw,
                             int32_t   dh,
                             int32_t   dpitch,
                             bool      hflip)
        { THISCALL_14(void, 0x47B9A0, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw adventure map hero shadow /
        // Нарисовать тень героя на карте.
        inline void DrawHeroShadow(int32_t   seqnum,
                                   int32_t   framenum,
                                   int32_t   sx,
                                   int32_t   sy,
                                   int32_t   sw,
                                   int32_t   sh,
                                   void* dst,
                                   int32_t   dx,
                                   int32_t   dy,
                                   int32_t   dw,
                                   int32_t   dh,
                                   int32_t   dpitch,
                                   bool      hflip)
        { THISCALL_14(void, 0x47B9F0, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        inline void DrawHeroAlpha(int32_t   seqnum,
                                  int32_t   framenum,
                                  int32_t   sx,
                                  int32_t   sy,
                                  int32_t   sw,
                                  int32_t   sh,
                                  void* dst,
                                  int32_t   dx,
                                  int32_t   dy,
                                  int32_t   dw,
                                  int32_t   dh,
                                  int32_t   dpitch,
                                  bool      hflip)
        { THISCALL_14(void, 0x47BA40, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip); }

        // Draw combat spell effect /
        // Нарисовать эффект заклинания в битве.
        inline void DrawSpellEffect(int32_t   seqnum,
                                    int32_t   framenum,
                                    int32_t   sx,
                                    int32_t   sy,
                                    int32_t   sw,
                                    int32_t   sh,
                                    void* dst,
                                    int32_t   dx,
                                    int32_t   dy,
                                    int32_t   dw,
                                    int32_t   dh,
                                    int32_t   dpitch,
                                    bool      hflip,
                                    bool      alpha)
        { THISCALL_15(void, 0x47BA90, this, seqnum, framenum, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, hflip, alpha); }

        // Number of sequences of each sprite type /
        // Кол-во групп каждого типа спрайта.
        [[nodiscard]] inline static int32_t GetNumSeqs(EResourceType type)
        { return FASTCALL_1(int32_t, 0x47BAF0, type); }

        [[nodiscard]] inline int32_t GetCroppedHeight(int32_t seq, int32_t frame) const noexcept
        {
            if ( CSpriteFrame* f = s[seq]->f[frame]; f )
                return f->CroppedHeight;

            return 0;
        }

        [[nodiscard]] inline int32_t GetCroppedWidth(int32_t seq, int32_t frame) const noexcept
        {
            if ( CSpriteFrame* f = s[seq]->f[frame]; f )
                return f->CroppedWidth;

            return 0;
        }

        [[nodiscard]] inline int32_t GetCroppedX(int32_t seq, int32_t frame) const noexcept
        {
            if ( CSpriteFrame* f = s[seq]->f[frame]; f )
                return f->CroppedX;

            return 0;
        }

        [[nodiscard]] inline int32_t GetCroppedY(int32_t seq, int32_t frame) const noexcept
        {
            if ( CSpriteFrame* f = s[seq]->f[frame]; f )
                return f->CroppedY;

            return 0;
        }

        [[nodiscard]] inline bool IsValidSeq(int32_t seqnum) const noexcept
        {
            if ( seqnum < this->numSequences )
            {
                if ( this->validSeqMask[seqnum] )
                    return true;
            }
            return false;
        }

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

[[nodiscard]] inline constexpr uint16_t convert555to565(uint16_t arg) noexcept
{ return (arg & 0x1FU) | (2U * (arg & 0x7FE0U)); }

[[nodiscard]] inline constexpr uint32_t color1555to8888(uint16_t arg) noexcept
{ return (8U * (arg & 0x1FU)) | ((arg & 0x3E0U) << 6U) | ((arg & 0x7C00U) << 9U) | ((arg & 0x8000U) << 16U); }

[[nodiscard]] inline constexpr uint32_t color0565to8888(uint16_t arg) noexcept
{ return (8U * (arg & 0x1FU)) | (32U * (arg & 0x7E0U)) | ((arg & 0xF800U) << 8U); }

[[nodiscard]] inline constexpr uint16_t color8888to1555(uint32_t arg) noexcept
{ return static_cast<uint16_t>(((arg & 0x80000000U) >> 16U) | ((arg & 0xF80000U) >> 9U) | (static_cast<uint16_t>(arg & 0xF800U) >> 6U) | (static_cast<uint8_t>(arg & 0xF8U) >> 3U)); }

[[nodiscard]] inline constexpr uint16_t color8888to0565(uint32_t arg) noexcept
{ return static_cast<uint16_t>(((arg & 0xF80000U) >> 8U) | (static_cast<uint16_t>(arg & 0xFC00U) >> 5U) | (static_cast<uint8_t>(arg & 0xF8U) >> 3U)); }

struct Bitmap816;
// 16 Bit bitmap(.pcx) resource /
// Игровой ресурс - 16-ти битное изображение(.pcx).
// size = 0x38 = 56, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap16Bit : public resource
{
    public:
        inline Bitmap16Bit(int32_t w, int32_t h) noexcept
            : Bitmap16Bit(nh3api::dummy_tag)
        { THISCALL_3(void, 0x44DC40, this, w, h); }

        inline Bitmap16Bit(const char* name, int32_t w, int32_t h) noexcept
            : Bitmap16Bit(nh3api::dummy_tag)
        { THISCALL_4(void, 0x44DD20, this, name, w, h); }

        inline Bitmap16Bit(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag)
        {}

        inline ~Bitmap16Bit() noexcept
        { THISCALL_1(void, 0x5BBF30, this); }

    public:
        inline uint16_t* GetMap() noexcept
        { return map; }

        [[nodiscard]] inline const uint16_t* GetMap() const noexcept
        { return map; }

        [[nodiscard]] inline int32_t GetWidth() const noexcept
        { return Width; }

        [[nodiscard]] inline int32_t GetHeight() const noexcept
        { return Height; }

        [[nodiscard]] inline int32_t GetPitch() const noexcept
        { return Pitch; }

        inline void Grab(uint16_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
        { THISCALL_7(void, 0x44E0C0, this, src, sx, sy, sw, sh, spitch); }

        inline void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, Bitmap16Bit* dst, int32_t dx, int32_t dy, bool alpha)
        { Draw(sx, sy, sw, sh, dst->GetMap(), dx, dy, dst->GetWidth(), dst->GetHeight(), dst->GetPitch(), alpha); }

        inline void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, void* dst, int32_t dx, int32_t dy, int32_t dw, int32_t dh, int32_t dpitch, bool alpha)
        { THISCALL_12(void, 0x44DF80, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, alpha); }

        inline void Grab(const uint16_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
        { THISCALL_7(void, 0x44E0C0, this, src, sx, sy, sw, sh, spitch); }

        inline void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E190, this, x, y, w, h, color); }

        inline void FrameRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E210, this, x, y, w, h, color); }

        inline void Darken(int32_t x, int32_t y, int32_t w, int32_t h)
        { THISCALL_5(void, 0x44E2C0, this, x, y, w, h); }

        inline void Darken(int32_t x, int32_t y, int32_t w, int32_t h, Bitmap816* mask, int32_t sx, int32_t sy)
        { THISCALL_8(void, 0x44E370, this, x, y, w, h, mask, sx, sy); }

        inline void Colorize(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
        { THISCALL_6(void, 0x44E450, this, x, y, w, h, color); }

        inline void Colorize(int32_t x, int32_t y, int32_t w, int32_t h, float hue, float saturation)
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

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;

// 16 Bit bitmap(.pcx) resource /
// Игровой ресурс - 16-ти битное изображение(.pcx).
// size = 0x56C = 1388, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap816 : public resource
{
    public:
        inline Bitmap816(const char* __restrict name,
                         int32_t w,
                         int32_t h,
                         uint8_t* __restrict data,
                         TPalette16* __restrict p16,
                         size_t csize) noexcept
            : Bitmap816(nh3api::dummy_tag)
        { THISCALL_7(void, 0x44F4D0, this, name, w, h, data, p16, csize); }

        inline Bitmap816(const char* __restrict name,
                         const char* __restrict path,
                         uint32_t rbits,
                         uint32_t rshift,
                         uint32_t gbits,
                         uint32_t gshift,
                         uint32_t bbits,
                         uint32_t bshift) noexcept
            : Bitmap816(nh3api::dummy_tag)
        { THISCALL_9(void, 0x44F5A0, this, name, path, rbits, rshift, gbits, gshift, bbits, bshift); }

        inline Bitmap816(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag), Palette { tag }, Palette24 { tag }
        {}

        inline ~Bitmap816() noexcept
        { THISCALL_1(void, 0x44F6A0, this); }

    public:
        void Draw(int32_t      sx,
                  int32_t      sy,
                  int32_t      sw,
                  int32_t      sh,
                  Bitmap16Bit* dst,
                  int32_t      dx,
                  int32_t      dy,
                  bool         tblit)
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

// size = 0x3C = 60, align = 4, baseclass: Bitmap16Bit
NH3API_VIRTUAL_STRUCT CTextEntrySave : public Bitmap16Bit
{
    public:
        inline CTextEntrySave(int32_t w, int32_t h) noexcept
            : Bitmap16Bit(w, h), saved{false}
        { NH3API_SET_VFTABLE(); }

        inline CTextEntrySave(const nh3api::dummy_tag_t& tag) noexcept
            : Bitmap16Bit(tag)
        {}

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(CTextEntrySave)

    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x1 = 1
        bool saved;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

} NH3API_MSVC_LAYOUT;

// Used only in ResourceManager::GetBitmap16 /
// Используется внутри ResourceManager::GetBitmap16.
// size = 0x30 = 48, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT Bitmap24Bit : public resource
{
    public:
        inline Bitmap24Bit(const char* __restrict name, int32_t w, int32_t h, const uint8_t* __restrict src, size_t size) noexcept
            : Bitmap24Bit(nh3api::dummy_tag) // resource(name, RType_bitmap24)
        { THISCALL_6(void, 0x44EA20, this, name, w, h, src, size); }

        inline Bitmap24Bit(const char* __restrict name, const char* __restrict path) noexcept
            : Bitmap24Bit(nh3api::dummy_tag) // resource(name, RType_bitmap24)
        { THISCALL_3(void, 0x44EAD0, this, name, path); }

        inline Bitmap24Bit(const nh3api::dummy_tag_t& tag) noexcept
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
        TRGB* data;

};

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
            std::array<uint16_t, 10> pal;

            // offset: +0x20 = +32,  size = 0xC00 = 3072
            std::array<myABC, 256> abc;

            // offset: +0xC20 = +3104,  size = 0x400 = 1024
            std::array<uint32_t, 256> Offset;

        };

        // Font color /
        // Цвет шрифта.
        enum TColor : uint32_t
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
            CUSTOM_COLOR           = 256
        };

        // Font justification /
        // Выравнивание шрифта.
        NH3API_FLAG_ENUM EJustify : int32_t
        {
            LEFT_JUSTIFIED         = 0b0000, // Выравнивание по левому краю
            CENTER_JUSTIFIED       = 0b0001, // Выравнивание по центру
            RIGHT_JUSTIFIED        = 0b0010, // Выравнивание по правому краю
            VERT_CENTER_JUSTIFIED  = 0b0100,
            BOTTOM_JUSTIFIED       = 0b1000
        };

    public:
        inline font(const char* __restrict name, const TFontSpec& __restrict fontspec, int32_t dsize, bool* __restrict d) noexcept
            : font(nh3api::dummy_tag)
        { THISCALL_5(void, 0x4B4DD0, this, name, &fontspec, dsize, d); }

        inline font(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag), p16 { tag }
        {}

        inline ~font() noexcept
        { THISCALL_1(void, 0x4B4E70, this); }

    public:
        void SetPalette(const TPalette16& other)
        { THISCALL_2(void, 0x4B4EE0, this, &other); }

        // Draw one character /
        // Нарисовать один символ.
        void DrawCharacter(char c, Bitmap16Bit* bmp, int32_t x, int32_t y, int32_t color)
        { THISCALL_6(void, 0x4B4F00, this, c, bmp, x, y, color); }

        [[nodiscard]] static constexpr TColor GetColor(TColor color_scheme, bool highlighted) noexcept
        {
            if ( (color_scheme & CUSTOM_COLOR) != 0 )
                return static_cast<TColor>(color_scheme & ~CUSTOM_COLOR);
            if ( highlighted && (color_scheme == PRIMARY || color_scheme == WHITE || color_scheme == HEADING) )
                return static_cast<TColor>(color_scheme + WHITE_PLAYER);

            return static_cast<TColor>(color_scheme + HEADING_DIM);
        }

        // Draw one character. Use GetColor() for <color> /
        // Нарисовать символ курсора. Используйте GetColor() для <color>.
        void DrawCursor(Bitmap16Bit* bitmap, int32_t x, int32_t y, int32_t color, bool highlighted)
        { DrawCharacter('_', bitmap, x, y, color + highlighted); }

        // Draw string /
        // Нарисовать строку.
        void DrawBoundedString(const char* str, Bitmap16Bit* bitmap, int32_t x, int32_t y, int32_t boxWidth, int32_t boxHeight, TColor color_scheme, EJustify justification = LEFT_JUSTIFIED, int32_t cursorPos = -1)
        { THISCALL_10(void, 0x4B51F0, this, str, bitmap, x, y, boxWidth, boxHeight, color_scheme, justification, cursorPos); }

        // Character width in pixels /
        // Длина символа(в пикселях).
        [[nodiscard]] int32_t GetCharacterWidth(char currChar) const noexcept
        { return currChar > 0 ? (this->fr.abc[static_cast<size_t>(static_cast<unsigned char>(currChar))].abcA + this->fr.abc[static_cast<size_t>(static_cast<unsigned char>(currChar))].abcB + this->fr.abc[static_cast<size_t>(currChar)].abcC) : 0; }

        // String width in pixels /
        // Длина строки(в пикселях).
        [[nodiscard]] size_t get_string_width(const char* arg) const noexcept
        {
            if ( arg == nullptr )
                return 0;

            size_t result = 0;
            char   c      = '\0';
            while ( *arg )
            {
                c       = *arg++;
                result += static_cast<size_t>(GetCharacterWidth(c));
            }
            return result;
        }

        [[nodiscard]] int32_t LineLength(const char* str, int32_t boxWidth) const
        { return THISCALL_3(int32_t, 0x4B5580, this, str, boxWidth); }

        [[nodiscard]] int32_t LineWidth(const char* text) const
        { return THISCALL_2(int32_t, 0x4B5680, this, text);}

        [[nodiscard]] int32_t LongestLineWidth(const char* str) const
        { return THISCALL_2(int32_t, 0x4B56F0, this, str);}

        // Length of the longest word in pixels /
        // Длина самого длинного слова(в пикселях).
        [[nodiscard]] int32_t longest_word_length(const char* str) const
        { return THISCALL_2(int32_t, 0x4B5770, this, str); }

        [[nodiscard]] int32_t LongestWrappedLineWidth(const char* str, int32_t boxWidth) const
        { return THISCALL_3(int32_t, 0x4B57E0, this, str, boxWidth); }

        void FillLinesVector(const char* __restrict str, int32_t boxWidth, exe_vector<exe_string>& __restrict result) const
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

template<>
struct nh3api::enum_limits<font::TColor>
    : nh3api::enum_limits_base<font::TColor, font::TColor::LowestColor, font::TColor::HighestColor>
{ static inline constexpr bool is_specialized = true; };

// крошечный шрифт
inline font* const& tinyFont = get_global_var_ref(0x698A54, font*);

// маленький шрифт
inline font* const& smallFont = get_global_var_ref(0x698A58, font*);

// средний(основной для текста) шрифт
inline font* const& medFont = get_global_var_ref(0x698A5C, font*);

// большой шрифт
inline font* const& bigFont = get_global_var_ref(0x698A60, font*);

// каллиграфический шрифт
inline font* const& caligFont = get_global_var_ref(0x698A64, font*);

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

// Text resource /
// Игровой ресурс - текстовый файл.
// size = 0x30 = 48, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TTextResource : public resource
{
    public:
        inline TTextResource(const char* name, int32_t size, const char* data) noexcept
            : TTextResource(nh3api::dummy_tag)
        { THISCALL_4(void,  0x5BBF90, this, name, size, data); }

        inline TTextResource(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag), Text{tag}
        {}

        TTextResource(TTextResource && other) noexcept
            : resource(std::move(other)),
              Text { std::move(other.Text) },
              Data { std::exchange(other.Data, nullptr) }
        {}

        TTextResource& operator=(TTextResource&& other) noexcept
        {
            if ( this != &other )
            {
                resource::operator=(static_cast<resource&&>(other));
                this->Text = std::move(other.Text);
                this->Data = std::exchange(other.Data, nullptr);
            }

            return *this;
        }

        inline ~TTextResource() noexcept
        { THISCALL_1(void, 0x5BC1C0, this); }

        [[nodiscard]] inline const char* GetText(size_t pos) const
        { return Text[pos]; }

        [[nodiscard]] inline char* GetText(size_t pos)
        { return Text[pos]; }

        [[nodiscard]] inline const char* const& operator[](size_t pos) const
        { return Text[pos]; }

        [[nodiscard]] inline char*& operator[](size_t pos)
        { return Text[pos]; }

        [[nodiscard]] inline size_t GetNumberOfStrings() const noexcept
        { return Text.size(); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TTextResource)

    // member variables
    public:
        union {
        // Test rows /
        // Строки текста.
        // offset: +0x1C = +28,  size = 0x10 = 16
        exe_vector<char*> Text;
        };

        // Source data /
        // Оригинальные данные.
        // offset: +0x2C = +44,  size = 0x4 = 4
        char* Data;

};

inline std::array<char, 512>& gText = (get_global_var_ref(0x697428, std::array<char, 512>));

// genrltxt.txt, usage: GameText->GetText(42)
inline const TTextResource* const& GameText = get_global_var_ref(0x6A5DC4, const TTextResource* const);

// arraytxt.txt, usage: ArrayText->GetText(42)
inline const TTextResource* const& ArrayText = get_global_var_ref(0x6A60AC, const TTextResource* const);

// advevent.txt, usage: AdventureEventText->GetText(42)
inline const TTextResource* const& AdventureEventText = get_global_var_ref(0x696A68, const TTextResource* const);

// Spreadsheet resource /
// Игровой ресурс - текстовая таблица.
// size = 0x34 = 52, align = 4, baseclass: resource
NH3API_VIRTUAL_STRUCT TSpreadsheetResource : public resource
{
    public:
        TSpreadsheetResource(const char* name, int32_t size, const char* data) noexcept
            : TSpreadsheetResource(nh3api::dummy_tag) // resource(name, RType_text)
        { THISCALL_4(void, 0x5BC260, this, name, size, data); }

        TSpreadsheetResource(const nh3api::dummy_tag_t& tag) noexcept
            : resource(tag), SpreadSheet { tag }
        {}

        TSpreadsheetResource(TSpreadsheetResource&& other) noexcept
            : resource{std::move(other)},
              SpreadSheet{std::move(other.SpreadSheet)},
              Data{std::exchange(other.Data, nullptr)},
              DataSize{std::exchange(other.DataSize, 0)}
        {}

        TSpreadsheetResource& operator=(TSpreadsheetResource&& other) noexcept
        {
            if ( this != &other )
            {
                resource::operator=(static_cast<resource&&>(other));
                this->SpreadSheet = std::move(other.SpreadSheet);
                this->Data        = std::exchange(other.Data, nullptr);
                this->DataSize    = std::exchange(other.DataSize, 0);
            }

            return *this;
        }

        ~TSpreadsheetResource() noexcept
        { THISCALL_1(void, 0x5BC560, this); }

        // Number Of columns /
        // Количество столбцов.
        [[nodiscard]] size_t GetNumberOfColumns(size_t r) const
        {
            if ( SpreadSheet[r] )
                return SpreadSheet[r]->size();

            return 0;
        }

         // Number of rows /
        // Количество строк.
        [[nodiscard]] size_t GetNumberOfRows() const noexcept
        { return SpreadSheet.size(); }

        // Get spreadsheet cell (intersection of row <r> and column <c>) /
        // Колонка таблицы на пересечении строки <r> и столбца <c>
        [[nodiscard]] char const* GetSpreadsheet(size_t r, size_t c) const
        {
            if ( SpreadSheet[r] )
            {
                auto& vec = SpreadSheet[r];
                if ( vec )
                    return vec->operator[](c);

                return nullptr;
            }

            return nullptr;
        }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TSpreadsheetResource)

    // member variables
    public:
        union {
        // Spreadsheet /
        // Таблица.
        // offset: +0x1C = +28,  size = 0x10 = 16
        exe_vector<exe_vector<char*>*> SpreadSheet;
        };

        // Source data /
        // Оригинальные данные.
        // offset: +0x2C = +44,  size = 0x4 = 4
        char* Data;

        // Source data size /
        // Размер оригинальных данных.
        // offset: +0x30 = +48,  size = 0x4 = 4
        size_t DataSize;

};
#pragma pack(pop) // 4

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
        constexpr TCacheMapKey(const TCacheMapKey&)            noexcept = default;
        constexpr TCacheMapKey(TCacheMapKey&&)                 noexcept = default;
        constexpr TCacheMapKey& operator=(const TCacheMapKey&) noexcept = default;
        constexpr TCacheMapKey& operator=(TCacheMapKey&&)      noexcept = default;
        ~TCacheMapKey() noexcept = default;

        constexpr TCacheMapKey(nh3api::dummy_tag_t&) noexcept
        {}

        constexpr TCacheMapKey() noexcept = default;

        constexpr TCacheMapKey(const char* name_) noexcept
        {
            if ( name_ )
            {
                const size_t length = std::min<size_t>(__builtin_strlen(name_), name.size() - 1);
                for ( size_t i = 0; i < length; ++i )
                    name[i] = name_[i];
            }
        }

        constexpr TCacheMapKey(const std::string_view name_) noexcept
        {
            if ( !name_.empty() )
            {
                const size_t length = std::min<size_t>(name_.size(), name.size() - 1);
                for ( size_t i = 0; i < length; ++i )
                    name[i] = name_[i];
                name.back() = '\0';
            }
        }

    public:
        // Resource name /
        // Длина ресурса.
        // offset: +0x0 = +0,  size = 0xC = 12
        ::std::array<char, 13> name{};
};
#pragma pack(pop) // 1

[[nodiscard]] inline constexpr bool operator<(TCacheMapKey const& lhs, TCacheMapKey const& rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs.name.data(), lhs.name.size()) < 0; }

[[nodiscard]] inline constexpr bool operator>(TCacheMapKey const& lhs, TCacheMapKey const& rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs.name.data(), lhs.name.size()) > 0; }

[[nodiscard]] inline constexpr bool operator<(TCacheMapKey const& lhs, ::std::string_view const& rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs.data(), lhs.name.size()) < 0; }

[[nodiscard]] inline constexpr bool operator>(TCacheMapKey const& lhs, ::std::string_view const& rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs.data(), lhs.name.size()) > 0; }

[[nodiscard]] inline constexpr bool operator<(TCacheMapKey const& lhs, const char* rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs, lhs.name.size()) < 0; }

[[nodiscard]] inline constexpr bool operator>(TCacheMapKey const& lhs, const char* rhs) noexcept
{ return ::std::char_traits<char>::compare(lhs.name.data(), rhs, lhs.name.size()) > 0; }

using TResourceMap = exe_map<TCacheMapKey, resource*, 0x69E604, 0x69E608>;

inline TResourceMap& GetResourceMap() noexcept
{ return get_global_var_ref(0x69E560, TResourceMap);}

inline Bitmap816* GetBitmap816(const char* name) noexcept
{ return FASTCALL_1(Bitmap816*, 0x55AA10, name); }

inline Bitmap16Bit* GetBitmap16(const char* name) noexcept
{ return FASTCALL_1(Bitmap16Bit*, 0x55AE50, name); }

inline TPalette16* GetPalette(const char* name) noexcept
{ return FASTCALL_1(TPalette16*, 0x55B5F0, name); }

inline TPalette24* GetPalette24(const char* name) noexcept
{ return FASTCALL_1(TPalette24*, 0x55B680, name); }

inline font* GetFont(const char* name) noexcept
{ return FASTCALL_1(font*, 0x55BAE0, name); }

inline TTextResource* GetText(const char* name) noexcept
{ return FASTCALL_1(TTextResource*, 0x55BFE0, name); }

inline TSpreadsheetResource* GetSpreadsheet(const char* name) noexcept
{ return FASTCALL_1(TSpreadsheetResource*, 0x55C2B0, name); }

inline sample* GetSample(const char* name) noexcept
{ return FASTCALL_1(sample*, 0x55C930, name); }

inline CSprite* GetSprite(const char* name) noexcept
{ return FASTCALL_1(CSprite*, 0x55C9C0, name); }

// templated version for convenience

template <typename T>
[[nodiscard]] inline T* GetResource(const char*) noexcept
{
    static_assert(false, "ResourceManager::GetResource: Invalid resource");
    return nullptr;
}

template <>
[[nodiscard]] inline Bitmap816* GetResource<Bitmap816>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetBitmap816(name);
}

template <>
[[nodiscard]] inline Bitmap16Bit* GetResource<Bitmap16Bit>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetBitmap16(name);
}

template <>
[[nodiscard]] inline TPalette16* GetResource<TPalette16>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetPalette(name);
}

template <>
[[nodiscard]] inline TPalette24* GetResource<TPalette24>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetPalette24(name);
}

template <>
[[nodiscard]] inline font* GetResource<font>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetFont(name);
}

template <>
[[nodiscard]] inline TTextResource* GetResource<TTextResource>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetText(name);
}

template <>
[[nodiscard]] inline TSpreadsheetResource* GetResource<TSpreadsheetResource>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetSpreadsheet(name);
}

template <>
[[nodiscard]] inline sample* GetResource<sample>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetSample(name);
}

template <>
[[nodiscard]] inline CSprite* GetResource<CSprite>(const char* name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    return GetSprite(name);
}

inline void GetBackdrop(const char* resName, Bitmap16Bit* destBmap) noexcept
{ FASTCALL_2(void, 0x55D110, resName, destBmap); }

inline LODFile* PointToSpriteResource(const char* name) noexcept
{ return THISCALL_1(LODFile*, 0x55D160, name); }

inline LODFile* PointToBitmapResource(const char* name) noexcept
{ return THISCALL_1(LODFile*, 0x55D1F0, name); }

[[nodiscard]] inline size_t GetBitmapResourceSize(const char* name) noexcept
{ return THISCALL_1(size_t, 0x55D280, name); }

inline int32_t ReadFromBitmapResource(LODFile* lod, void* data, size_t numBytes) noexcept
{ return FASTCALL_3(int32_t, 0x55D2E0, lod, data, numBytes); }

inline int32_t ReadFromSpriteResource(LODFile* lod, void* data, size_t numBytes) noexcept
{ return FASTCALL_3(int32_t, 0x55D2E0, lod, data, numBytes); }

inline void Dispose(resource* arg)
{
    if ( arg )
        arg->Dispose();
}

[[nodiscard]] inline exe_string& GetDataPath() noexcept
{ return get_global_var_ref(0x69E528, exe_string); }

[[nodiscard]] inline resource* GetFromCache(const char *name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    const TCacheMapKey key { name };
    auto it = GetResourceMap().find(key);

    if ( it == GetResourceMap().end() )
        return nullptr;

    it->second->AddRef();
    return it->second;

}

inline void AddToCache(resource* r)
{ FASTCALL_1(void, 0x5596F0, r); }

inline uint32_t& RedBits    = get_global_var_ref(0x69E5DC, uint32_t);
inline uint32_t& RedShift   = get_global_var_ref(0x69E5E4, uint32_t);
inline uint32_t& RedMask    = get_global_var_ref(0x69E5E8, uint32_t);
inline uint32_t& GreenBits  = get_global_var_ref(0x69E5D0, uint32_t);
inline uint32_t& GreenShift = get_global_var_ref(0x69E5E0, uint32_t);
inline uint32_t& GreenMask  = get_global_var_ref(0x69E5EC, uint32_t);
inline uint32_t& BlueBits   = get_global_var_ref(0x69E5F0, uint32_t);
inline uint32_t& BlueShift  = get_global_var_ref(0x69E5D8, uint32_t);
inline uint32_t& BlueMask   = get_global_var_ref(0x69E5D4, uint32_t);

} // namespace ResourceManager

// std::hash support for ResourceManager::TCacheMapKey
template<>
struct std::hash< ResourceManager::TCacheMapKey >
{
    public:
        [[nodiscard]]
    #ifdef __cpp_static_call_operator
        static
    #endif
        inline size_t operator()(const ResourceManager::TCacheMapKey& key)
    #ifndef __cpp_static_call_operator
        const
    #endif
        noexcept
        { return ::nh3api::hash_string(key.name.data(), ::strnlen(key.name.data(), key.name.size())); }
};

NH3API_SPECIALIZE_TYPE_VFTABLE(0x64100C, resource)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B9C8, Bitmap16Bit)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63B9F4, Bitmap24Bit)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63BA14, Bitmap816)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63D6BC, CSpriteFrame)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63D6B0, CSprite)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63E5F4, font)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x640378, TPalette16)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x640384, TPalette24)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642D9C, CTextEntrySave)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642DA8, TTextResource)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x642DB4, TSpreadsheetResource)

NH3API_WARNING(pop)
