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

// HD Mod API
// source: https://forum.df2.ru/index.php?showtopic=10421&st=2480&p=593378&#entry593378

#include "core/nh3api_std/exe_string.hpp"
#include "core/nh3api_std/exe_vector.hpp"
#include "core/nh3api_std/patcher_x86.hpp"
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
    HD_MOP  = 0x40
};

inline HANDLE getHDModHandle() noexcept
{ return GetModuleHandleA("_hd3_.dll"); }

inline bool isHDModPresent(const Patcher* patcher)
{
    if ( patcher )
        return static_cast<bool>(getHDModHandle()) && static_cast<bool>(patcher->GetInstance("HD"));
    else
        return false;
}

template<typename T, T DefaultValue = T {}>
inline T getHDModVariable(const Patcher* __restrict patcher, const char* __restrict name)
{ return patcher->VarGetValue(name, DefaultValue); }

inline uint32_t getHDModVersion(const Patcher* patcher)
{ return getHDModVariable<uint32_t>(patcher, "HD.Version.Dword"); }

inline HD_game_version getHDModEXEVersion(const Patcher* patcher)
{ return getHDModVariable<HD_game_version>(patcher, "HD.ExeVersion"); }

inline const char* getHDModVersionString(const Patcher* patcher)
{ return getHDModVariable<const char*>(patcher, "HD.Version.CStr"); }

// _HD3_Data
inline const char* getHDModDirectory(const Patcher* patcher)
{ return getHDModVariable<const char*>(patcher, "HD.Dir"); }

inline bool isHDPlusPresent(const Patcher* patcher)
{ return getHDModVariable<bool>(patcher, "HD+.On"); }

inline POINT getHDModResolution(const Patcher* patcher)
{ return { getHDModVariable<int32_t>(patcher, "HD.Option.RezX"), getHDModVariable<int32_t>(patcher, "HD.Option.RezY")}; }

inline int32_t getScreenWidth(const Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModVariable<int32_t>(patcher, "HD.Option.RezX");
    else
        return 800;
}

inline int32_t getScreenHeight(const Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModVariable<int32_t>(patcher, "HD.Option.RezY");
    else
        return 600;
}

inline POINT getGameScreenResolution(const Patcher* patcher)
{
    if ( isHDModPresent(patcher) )
        return getHDModResolution(patcher);
    else
        return { 800, 600 };
}

// Retrieve names of HD Mod plugins /
// Названия плагинов HD Mod.
inline exe_vector<exe_string> getHDModPlugins(const Patcher* patcher)
{
    exe_vector<exe_string>   result{};
    const char* __restrict const* __restrict const packDirs = getHDModVariable<const char* const* const>(patcher, "HD.PackDirs");
    const size_t packDirsCount        = getHDModVariable<const size_t>(patcher, "HD.PackDirs.Count");

    if ( packDirs && packDirsCount )
        for ( size_t i = 0; i < packDirsCount; ++i )
            result.emplace_back(packDirs[i]);

    return result;
}

namespace HD
{
struct FileTree
{
    public:
        struct vftable_t
        {
            const char* (__thiscall* FindPath)(FileTree*, const char*);
        };

        [[nodiscard]] const char* FindPath(const char* filename)
        { return vftable->FindPath(this, filename); }

    private:
        vftable_t* vftable;
};
} // namespace HD

[[nodiscard]] inline HD::FileTree* getHDModFileTree(const Patcher* patcher)
{
    return getHDModVariable<HD::FileTree*>(patcher, "HD.Files");
}

[[nodiscard]] inline const char* getHDModFilePath(const Patcher* const __restrict patcher, const char* const __restrict filename)
{
    HD::FileTree* const HDFileTree = getHDModFileTree(patcher);
    if ( HDFileTree == nullptr )
        return nullptr;
    else
        return HDFileTree->FindPath(filename);
}

[[nodiscard]] inline const char* getHDModFilePath(HD::FileTree* const __restrict HDFileTree, const char* const __restrict filename)
{
    if ( HDFileTree == nullptr )
        return nullptr;
    else
        return HDFileTree->FindPath(filename);
}

// Check for WINE /
// Проверить наличие WINE.
[[nodiscard]] inline bool isWinePresent()
{
    static bool    initialized = false;
    static HMODULE library     = nullptr;
    static FARPROC func        = nullptr;
    if ( !initialized )
    {
        library = GetModuleHandleA("ntdll.dll");
        if ( library ) // I *really* hope you're running it with windows or WINE
            func = GetProcAddress(library, "wine_get_version");
        initialized = true;
    }
    if ( library )
        return static_cast<bool>(func);
    else
        return false; // So you really run without the ntdll.dll.........
}

enum : int32_t
{
    RType_palette32 = 97 // 32-битная палитра в HD Mod
};

namespace HD
{

struct Bitmap : public ::resource
{
    public:
        inline Bitmap(int32_t w, int32_t h) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Bitmap)
        { THISCALL_3(void, 0x44DC40, this, w, h); }

        inline Bitmap(const char* name, int32_t w, int32_t h) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Bitmap)
        { THISCALL_4(void, 0x44DD20, this, name, w, h); }

        inline Bitmap(const ::nh3api::dummy_tag_t& tag) noexcept
            : ::resource(tag)
        {}

        inline ~Bitmap() noexcept
        { THISCALL_1(void, 0x5BBF30, this); }

    public:
        [[nodiscard]] inline uint16_t* GetMap() noexcept
        { return map; }

        [[nodiscard]] inline const uint16_t* GetMap() const noexcept
        { return map; }

        [[nodiscard]] inline uint32_t* GetMap32() noexcept
        { return map32; }

        [[nodiscard]] inline const uint32_t* GetMap32() const noexcept
        { return map32; }

        [[nodiscard]] inline int32_t GetWidth() const noexcept
        { return Width; }

        [[nodiscard]] inline int32_t GetHeight() const noexcept
        { return Width; }

        [[nodiscard]] inline int32_t GetPitch() const noexcept
        { return Pitch; }

        inline void Grab(uint16_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
        { THISCALL_7(void, 0x44E0C0, this, src, sx, sy, sw, sh, spitch); }

        inline void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, Bitmap* dst, int32_t dx, int32_t dy, bool alpha)
        { Draw(sx, sy, sw, sh, dst->GetMap(), dx, dy, dst->GetWidth(), dst->GetHeight(), dst->GetPitch(), alpha); }

        inline void Draw(int32_t sx, int32_t sy, int32_t sw, int32_t sh, void* dst, int32_t dx, int32_t dy, int32_t dw, int32_t dh, int32_t dpitch, bool alpha)
        { THISCALL_12(void, 0x44DF80, this, sx, sy, sw, sh, dst, dx, dy, dw, dh, dpitch, alpha); }

        inline void Grab(const void* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t spitch)
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
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(Bitmap)

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

        union {
        // offset: +0x30 = +48,  size = 0x4 = 4
        uint16_t* map;
        uint32_t* map32;
        };

        // offset: +0x34 = +52,  size = 0x1 = 1
        bool keepData;
};

// 16-bit or 32-bit palette
struct Palette : public ::resource
{
    public:
        inline Palette() noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_1(void, 0x522B40, this); }

        inline Palette(const ::TPalette24& p24) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_2(void, 0x522D00, this, &p24); }

        inline Palette(const void* data) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_2(void, 0x522B90, this, data); }

        inline Palette(const ::TPalette24& p24,
                        uint32_t            rbits,
                        uint32_t            rshift,
                        uint32_t            gbits,
                        uint32_t            gshift,
                        uint32_t            bbits,
                        uint32_t            bshift) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_8(void, 0x522BC0, this, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        inline Palette(const char*         name,
                        const ::TPalette24& p24,
                        uint32_t            rbits,
                        uint32_t            rshift,
                        uint32_t            gbits,
                        uint32_t            gshift,
                        uint32_t            bbits,
                        uint32_t            bshift) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_9(void, 0x522C60, this, name, &p24, rbits, rshift, gbits, gshift, bbits, bshift); }

        inline Palette(const ::nh3api::dummy_tag_t& tag) noexcept
            : resource(tag) // resource(nullptr, RType_misc)
        {}

        inline Palette& operator=(const Palette& other) noexcept
        {
            THISCALL_2(Palette*, 0x522E00, this, &other);
            return *this;
        }

        inline Palette(Palette const& other) noexcept
        NH3API_DELEGATE_DUMMY_BASE(Palette)
        { THISCALL_2(void, 0x522DD0, this, &other); }

        inline Palette& operator=(const ::TPalette16& other) noexcept
        {
            THISCALL_2(void, 0x522E00, this, &other);
            return *this;
        }

    public:
        NH3API_VIRTUAL_OVERRIDE_RESOURCE(Palette)

    public:
        [[nodiscard]] const uint16_t* data() const noexcept
        { return pal16.data(); }

        [[nodiscard]] uint16_t* data() noexcept
        { return pal16.data(); }

        [[nodiscard]] const uint32_t* data32() const noexcept
        { return pal32->data(); }

        [[nodiscard]] uint32_t* data32() noexcept
        { return pal32->data(); }

    union {
        struct
        {
            // 16-bit Palette /
            // 16-битная палитра.
            // offset: +0x1C = +28,  size = 0x200 = 512
            ::std::array<uint16_t, 256> pal16;
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
            ::std::array<uint32_t, 256>* pal32;
        };
    };
};

} // namespace HD

// virtual aliases for HD::Bitmap and HD::Palette
// I recommend using
// std::variant<Bitmap16Bit, HD::Bitmap> and
// std::variant<TPalette16, HD::Palette>

template<>
struct vftable_address<HD::Bitmap>
{
        static const uintptr_t address = vftable_address<Bitmap16Bit>::address;
};

template<>
struct vftable_address<HD::Palette>
{
        static const uintptr_t address = vftable_address<TPalette16>::address;
};

inline bool isHDMod32Bit(const TPalette16* pal /* = gGamePalette */)
{
    return pal->resType == static_cast<EResourceType>(RType_palette32);
}
