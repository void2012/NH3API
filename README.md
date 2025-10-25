## Welcome to the NH3API GitHub repository!
NH3API by void_17 is an open source C++17 library for modding Heroes of Might and Magic III. The library is primarly targeted at the latest version of the game(Complete edition) with [HD Mod by baratorch](https://sites.google.com/site/heroes3hd/ "HD Mod official page")

See our [wiki](https://github.com/void2012/NH3API/wiki) page for core guidelines on installing and usage of the library

## Rich game executable database covering most aspects of the game logic
The library is based on the void_17's IDA Database of the Windows x86 version of the Complete Edition executable.

More than 80% of the functions, variables and types names are from the original game code, [retrieved](https://github.com/void2012/HoMM3-Dreamcast-Dump) from the Dreamcast version of the game.

![database_screenshot](https://github.com/user-attachments/assets/450b394e-d511-4985-8939-62ed014d5683)

## Wide support of compilers

NH3API v1.2 supports three major compilers(including clang-cl in Visual Studio) and requires at least C++17 to work.
For C++98/11/14 support, see [v1.1 branch](https://github.com/void2012/NH3API/tree/v1.1)

<table>
    <tr>
        <td></td>
        <td>Minimum supported version</td>
        <td>Version compatible with Windows XP</td>
    </tr>
    <tr>
        <td>MSVC</td>
        <td>19.14, Visual Studio 2017 (C++17) or higher</td>
        <td>19.14, Visual Studio 2017 (C++17) with v141_xp build tools.</td>
    </tr>
    <tr>
        <td>MinGW GCC</td>
        <td>9.0 (C++17)</td>
        <td>Any version since 5.0 with posix threads, preferably with msvcrt runtime.<br>
            <a href="https://github.com/skeeto/w64devkit" target="_blank" rel="noopener noreferrer">w64devkit</a> is the best choice
        </td>
    </tr>
    <tr>
        <td>MinGW Clang</td>
        <td>9.0 (C++17)</td>
        <td>Requires custom hacky standard library build to support Windows XP. See <a href="https://github.com/void2012/llvm-mingw-winxp" target="_blank" rel="noopener noreferrer">llvm-mingw-winxp</a></td>
    </tr>
    <tr>
        <td>Clang-CL</td>
        <td>15.0.0, Visual Studio 2019 (C++17) or higher</td>
        <td>See Windows XP compatible build tools in <a href="https://github.com/zufuliu/llvm-utils" target="_blank" rel="noopener noreferrer">llvm-utils</a></td>
    </tr>
</table>

## Easy to use
A single header for almost everything you need to mod the game

```cpp
#include <nh3api/core.hpp>
```

## Easy CMake integration
NH3API can be used without CMake(except for Era III), however the CMake integration is very straightforward. It takes only two CMake commands to include NH3API in your project

```cmake
add_subdirectory(nh3api)
target_link_libraries(MyProject PRIVATE nh3api)
```

## Era III modding platform support
NH3API supports Era III modding platform(just include `<nh3api/era/era.hpp>`) but requires CMake support. The integration is still pretty easy:

```cmake
SET(NH3API_CMAKE_USE_ERA ON CACHE BOOL "Build ERA support module")
add_subdirectory(nh3api)
target_link_libraries(NH3APIHelloWorld PRIVATE nh3api)
```

## Build
Simple Hello, World in a single dllmain.cpp file:
```cpp
#include <nh3api/core.hpp> // The directory of dllmain.cpp also contains the copy of this repository

// Display a "Hello, World!" dialog before the 3DO intro
void __stdcall HelloWorld(HiHook* hook, void* _this)
{
    NormalDialog("Hello, World!");
    THISCALL_1(void, hook->GetOriginalFunc(), _this);
}

extern "C" NH3API_DLLEXPORT
BOOL APIENTRY DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
    if ( fdwReason == DLL_PROCESS_ATTACH  )
    {
        Patcher* patcher = GetPatcher();
        if ( patcher == nullptr )
            return false;
        PatcherInstance* instance = patcher->CreateInstance("HD.Plugin.NH3API.HelloWorld");
        if ( instance )
            instance->WriteHiHook(0x4EEE31, CALL_, EXTENDED_, THISCALL_, &HelloWorld);
        else
            return false;
    }
    return true;
}

```

Build the example:

MSVC:
```
cl /std:c++17 -I./nh3api /LD dllmain.cpp /Fe:hello-world.dll
```
GCC MinGW:
```
g++ -m32 -std=c++17 -I./nh3api -mdll -o hello-world.dll dllmain.cpp
```
Clang MinGW:
```
clang++ -m32 -std=c++17 -I./nh3api -mdll -o hello-world.dll dllmain.cpp
```

## Examples
See [Awesome-NH3API](https://github.com/void2012/Awesome-NH3API) for a curated list of plugins that use NH3API. Feel free to contribute and suggest your own plugin!

## Debugging
> [!NOTE]
> The NH3API contains a pretty printer for Visual Studio debugger. Add [nh3api_std.natvis](https://github.com/void2012/NH3API/blob/main/debugging/nh3api_std.natvis) to your C++ project in Visual Studio and enjoy the visualized debugging of NH3API containers which are fully binary compatible with the game.

## Contact the author
[Join our discord server!](https://discord.com/invite/BrbwGNFYgP)

## Acknowledgements
I want to thank AlexSpl and RoseKavalier for their enthusiasm and contribution to the HoMM III modding community.
This library also wouldn't be possible without the FatalistDC who published the Beta version of the Dreamcast port which contained the debugging information

## Legal info
Copyright © 2021-2025 void_17

NH3API is licensed under the Apache 2.0 Software license, see LICENSE for details.

This software library is provided for informational and educational purposes only. Said library is the result of reverse engineering and must not be used for commercial purposes. The author of this library does not claim any rights or ownership over the Heroes of Might and Magic trademark or intellectual property owned by Ubisoft Entertainment. Users are encouraged to support the official products by purchasing and playing the original Heroes of Might and Magic III: Complete Edition game through authorized channels.

Additionally, this library utilizes the freeware version of IDA (The Interactive Disassembler), which is a trademark of Hex-Rays. The author recommends using only the official freeware version of IDA and does not endorse or promote any unauthorized or paid versions.

By using this software, you agree to respect all applicable trademarks and intellectual property rights.
