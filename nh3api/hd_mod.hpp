//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// HD Mod API
// source: https://forum.df2.ru/index.php?showtopic=10421&st=2480&p=593378&#entry593378
#pragma once

#include "core/nh3api_std/patcher_x86.hpp"
#include "core/nh3api_std/exe_vector.hpp"
#include "core/nh3api_std/exe_string.hpp"
#include "core/resources/resources.hpp"

// Game version of HD Mod /
// Версия игры с точки зрения HD Mod.
enum HD_game_version
{
    HD_NONE = 0x00,
    HD_SOD  = 0x01,
    HD_WOG  = 0x02,
    HD_TE   = 0x04,
    HD_HOTA = 0x08,
    HD_WT   = 0x10,
    HD_ERA  = 0x20,
    HD_MOP  = 0x40,
};

NH3API_FORCEINLINE
HANDLE getHDModHandle()
{ return GetModuleHandleA("_hd3_.dll"); }

NH3API_FORCEINLINE
bool isHDModPresent(Patcher* patcher)
{
    if ( patcher )
        return static_cast<bool>(getHDModHandle()) && static_cast<bool>(patcher->GetInstance("HD"));
    else
        return false;
}

// moved to struct and marked noinline to reduce the code bloat
struct get_hd_mod_varable_impl
{
    template<typename T, T DefaultValue>
    NH3API_NOINLINE
    static T get(Patcher* patcher, const char* name)
    {
        if ( patcher )
        {
            Variable* var = patcher->VarFind(name);
            if ( var )
            {
                // static_cast for integrals, reinterpret_cast for pointers...
                // use C-style as a compromise
                return (T)(var->GetValue());
            }
            else
            {
                return DefaultValue;
            }
        }
        else
        {
            return DefaultValue;
        }
    }
};

template<typename T, T DefaultValue>
NH3API_FORCEINLINE
T getHDModVariable(Patcher* patcher, const char* name)
{ return get_hd_mod_varable_impl::get<T, DefaultValue>(patcher, name); }

NH3API_FORCEINLINE
uint32_t getHDModVersion(Patcher* patcher)
{ return getHDModVariable<uint32_t, 0>(patcher, "HD.Version.Dword"); }

NH3API_FORCEINLINE
HD_game_version getHDModEXEVersion(Patcher* patcher)
{ return getHDModVariable<HD_game_version, HD_NONE>(patcher, "HD.ExeVersion"); }

NH3API_FORCEINLINE
const char* getHDModVersionString(Patcher* patcher)
{ return getHDModVariable<const char*, nullptr>(patcher, "HD.Version.CStr"); }

NH3API_FORCEINLINE
// _HD3_Data
const char* getHDModDirectory(Patcher* patcher)
{ return getHDModVariable<const char*, nullptr>(patcher, "HD.Dir"); }

NH3API_FORCEINLINE
bool isHDPlusPresent(Patcher* patcher)
{ return getHDModVariable<bool, false>(patcher, "HD+.On"); }

NH3API_FORCEINLINE
POINT getHDModResolution(Patcher* patcher)
{
    POINT result;
    result.x = getHDModVariable<int32_t, 0>(patcher, "HD.Option.RezX");
    result.y = getHDModVariable<int32_t, 0>(patcher, "HD.Option.RezY");
    return result;
}

NH3API_FORCEINLINE
int32_t getScreenWidth(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModResolution(patcher).x;
    else
        return 800;
}

NH3API_FORCEINLINE
int32_t getScreenHeight(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModResolution(patcher).x;
    else
        return 600;
}

NH3API_FORCEINLINE
POINT getScreenResolution(Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
    {
        return getHDModResolution(patcher);
    }
    else
    {
        POINT result;
        result.x = 800;
        result.y = 600;
        return result;
    }
}

NH3API_FORCEINLINE // this will not inline actually, just for the linker
// Retrieve names of HD Mod plugins /
// Названия плагинов HD Mod.
exe_vector<exe_string> getHDModPlugins(Patcher* patcher)
{
    exe_vector<exe_string> result(0);
    const char* const* const packDirs =
    getHDModVariable<const char* const* const, nullptr>(patcher, "HD.PackDirs");

    const size_t packDirsCount =
    getHDModVariable<const size_t, 0>(patcher, "HD.PackDirs.Count");

    if ( packDirs && packDirsCount )
    {
        for ( size_t i = 0; i < packDirsCount; ++i )
            result.push_back(packDirs[i]);
    }
    return result;
}

// static variables will be thread-safe according to the C++11 standard
// so we need to reduce the code bloat:
// move implementation to structure and mark noinline
struct is_wine_present_impl
{
    NH3API_NOINLINE
    static bool get() 
    {
        static bool initialized = false;
        static HMODULE library = nullptr;
        static FARPROC func = nullptr;
        if ( !initialized )
        {
            library = GetModuleHandleA("ntdll.dll");
            if ( library ) // I *really* hope you're running it with windows or WINE
                func = GetProcAddress(library, "wine_get_version");
            initialized = true;
        }
        if ( library )
        {
            return static_cast<bool>(func);
        }
        else
        {
            return false; // So you really run without the ntdll.dll.........
        }
    }
};

NH3API_FORCEINLINE
// Check for WINE /
// Проверить наличие WINE.
bool isWinePresent()
{ return is_wine_present_impl::get(); }

enum : int32_t 
{
    RType_palette32 = 97, // 32-битная палитра в HD Mod
};

namespace HD
{

using ::resource;
using ::TPalette16;
using ::TPalette16;

struct TBitmap : public resource 
{
    public:
        NH3API_FORCEINLINE
        TBitmap(int32_t w, int32_t h) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TBitmap, resource)
        { THISCALL_3(void, 0x44DC40, this, w, h); }

        NH3API_FORCEINLINE
        TBitmap(const char* name, int32_t w, int32_t h) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TBitmap, resource)
        { THISCALL_4(void, 0x44DD20, this, name, w, h); }

        NH3API_FORCEINLINE
        TBitmap(const ::nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag)
        { NH3API_IGNORE(DataSize, ImageSize, Width, Height, Pitch, map, keepData); }

        NH3API_FORCEINLINE
        ~TBitmap()
        { THISCALL_1(void, 0x5BBF30, this); }

    public:
        NH3API_FORCEINLINE
        uint16_t* GetMap()
        { return map; }

        NH3API_FORCEINLINE
        const uint16_t* GetMap() const
        { return map; }

        NH3API_FORCEINLINE
        uint32_t* GetMap32()
        { return map32; }

        NH3API_FORCEINLINE
        const uint32_t* GetMap32() const
        { return map32; }

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
        void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, TBitmap* dst, int32_t dx, int32_t dy, bool alpha)
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
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TBitmap)

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

        union 
        {
            // offset: +0x30 = +48,  size = 0x4 = 4
            uint16_t* map; 
            uint32_t* map32;   
        };

        // offset: +0x34 = +52,  size = 0x1 = 1
        bool keepData;
};

// 16-bit or 32-bit palette 
struct TPalette : public resource 
{
    public:
        NH3API_FORCEINLINE
        TPalette() NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette, resource)
        { THISCALL_1(void, 0x522B40, this); }

        NH3API_FORCEINLINE
        TPalette(const TPalette24& p24) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette, resource)
        { THISCALL_2(void, 0x522D00, this, &p24); }

        NH3API_FORCEINLINE
        TPalette(const uint16_t* data) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette, resource)
        { THISCALL_2(void, 0x522B90, this, data); }

        NH3API_FORCEINLINE
        TPalette(const TPalette24& p24,
                   int32_t rbits,
                   int32_t rshift,
                   int32_t gbits,
                   int32_t gshift,
                   int32_t bbits,
                   int32_t bshift) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette, resource)
        { THISCALL_8(void, 0x522BC0, this, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        TPalette(const char* name,
                   const TPalette24& p24,
                   int32_t rbits,
                   int32_t rshift,
                   int32_t gbits,
                   int32_t gshift,
                   int32_t bbits,
                   int32_t bshift) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY_OR_BASE(TPalette, resource)
        { THISCALL_9(void, 0x522C60, this, name, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        NH3API_FORCEINLINE
        TPalette(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : resource(tag) // resource(nullptr, RType_misc)
        { NH3API_IGNORE(Palette); }

        NH3API_FORCEINLINE
        TPalette& operator=(const TPalette& other) NH3API_NOEXCEPT
        { return *THISCALL_2(TPalette*, 0x522E00, this, other); }

        NH3API_FORCEINLINE
        TPalette& operator=(const TPalette16& other) NH3API_NOEXCEPT
        { return *THISCALL_2(TPalette*, 0x522E00, this, other); }

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(TPalette)

    public:
        const uint16_t* data() const 
        { return Palette.data(); }

        uint16_t* data() 
        { return Palette.data(); }

        const uint32_t* data32() const 
        { return Palette32->data(); }

        uint32_t* data32() 
        { return Palette32->data(); }

    union 
    {
        struct 
        {
            // 16-bit Palette /
            // 16-битная палитра.
            // offset: +0x1C = +28,  size = 0x200 = 512
            ::std::array<uint16_t, 256> Palette; 
        };
        
        struct 
        {
            // Unused in HD Mod 32-bit mode /
            // Не используется в режиме 32-бит HD Mod.
            // offset: +0x1C = +28,  size = 0x1FC = 508
            ::std::array<uint16_t, 254> unused;

            // 32-bit Palette /
            // 32-битная палитра.
            // offset: +0x218 = +536,  size = 0x4 = 4
            ::std::array<uint32_t, 256>* Palette32;
        };
    };
};

}

// virtual aliases for HD::TBitmap and HD::TPalette
// I recommend using 
// std::variant<Bitmap16Bit, HD::TBitmap> and 
// std::variant<TPalette16, HD::TPalette>

template<> 
struct vftable_address<HD::TBitmap>
{ static const uintptr_t address = vftable_address<Bitmap16Bit>::address; };

template<> 
struct vftable_address<HD::TPalette>
{ static const uintptr_t address = vftable_address<TPalette16>::address; };

NH3API_FORCEINLINE
bool isHDMod32Bit(const TPalette16* pal /* = gGamePalette */)
{ return pal->resType == RType_palette32; }
