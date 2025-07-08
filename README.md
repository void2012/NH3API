## Welcome to the NH3API GitHub repository!
NH3API by void_17 is an open source C++ library for modding Heroes of Might and Magic III. The library is primarly targeted at the latest version of the game(Complete edition) with [HD Mod by baratorch](https://sites.google.com/site/heroes3hd/ "HD Mod official page")

See our [wiki](https://github.com/void2012/NH3API/wiki) page for core guidelines on installing and usage of the library

## Rich game executable database covering most aspects of the game logic
The library is based on the void_17's IDA Database of the Windows x86 version of the Complete Edition executable.

More than 80% of the functions, variables and types names are from the original game code, [retrieved](https://github.com/void2012/HoMM3-Dreamcast-Dump) from the Dreamcast version of the game.

![database_screenshot](https://github.com/user-attachments/assets/450b394e-d511-4985-8939-62ed014d5683)

## Wide support of compilers 

NH3API supports three most popular compilers and has C++98 support for older versions Visual Studio.

<table>
    <tr>
        <td></td>
        <td>Minimum supported version</td>
        <td>Recommended version</td>
        <td>Version compatible with Windows XP</td>
    </tr>
    <tr>
        <td>MSVC</td>
        <td>14.0, Visual Studio 2005 (C++98)</td>
        <td>19.14, Visual Studio 2017 (C++17) or higher</td>
        <td>19.14, Visual Studio 2017 (C++17) with v141_xp build tools.</td>
    </tr>
    <tr>
        <td>MinGW GCC</td>
        <td>5.0 (C++11)</td>
        <td>7.1 (C++17) or higher</td>
        <td>Any version since 5.0 with posix threads, preferably with msvcrt runtime.<br>
            <a href="https://github.com/skeeto/w64devkit" target="_blank" rel="noopener noreferrer">w64devkit</a> is the best choice
        </td>
    </tr>
    <tr>
        <td>MinGW Clang</td>
        <td>3.3 (C++11)</td>
        <td>5.0 (C++17) or higher</td>
        <td>Requires custom hacky standard library build to support Windows XP. See <a href="https://github.com/void2012/llvm-mingw-winxp" target="_blank" rel="noopener noreferrer">llvm-mingw-winxp</a></td>
    </tr>
    <tr>
        <td>Clang-CL&lt;br&gt;(MSVC)</td>
        <td>8.0, Visual Studio 2019 (C++17)</td>
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
NH3API can be very easily used in your CMake C++ project, it takes only two CMake commands to include NH3API in your project

```cmake
add_subdirectory(nh3api)
target_link_libraries(MyProject PRIVATE nh3api)
```

> [!TIP]
> If your compiler supports C++17, you should use inline mode of the library which requires no CMake integration and compiling the static library. 
> Define the NH3API_FLAG_INLINE_HEADERS globally and you can use NH3API without any build system! It makes things way faster.

## Build
Simple Hello, World in a single dllmain.cpp file. The directory with dllmain.cpp also contains the copy of this repository,
```cpp
#include <nh3api/core.hpp>

// Display a "Hello, World!" dialog before the 3DO intro
void __stdcall HelloWorld(HiHook* hook, void* _this)
{
    NormalDialog("Hello, World!");
    THISCALL_1(void, hook->GetOriginalFunc(), _this);
}

extern "C" NH3API_DLLEXPORT
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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

Build with CMake and Ninja:
```shell
cmake -G "Ninja" -B bin  && cmake --build bin
```

Build without CMake(requries C++17):

MSVC:
```shell
cl.exe /std:c++17 -I./nh3api -DNH3API_FLAG_INLINE_HEADERS /LD dllmain.cpp /Fe:hello-world-nh3api.dll
```
GCC MinGW:
```shell
g++ -m32 -std=c++17 -I./nh3api -DNH3API_FLAG_INLINE_HEADERS -mdll -o hello-world-nh3api.dll dllmain.cpp
```
Clang MinGW:
```
clang++ -m32 -std=c++17 -I./nh3api -DNH3API_FLAG_INLINE_HEADERS -mdll -o hello-world-nh3api.dll dllmain.cpp
```

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
