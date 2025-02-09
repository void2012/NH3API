This is the optional ERA 3 module of NH3API for modding based on Era/WoG platform

You need to link statically to era.lib (MSVC/Clang-CL) or libera.a (MinGW/Clang GNU) for it to work.

You can generate your own static library files using the following commands:

MSVC binutils:

`lib /def:era.def /out:era.lib /machine:x86`
GNU binutils:

`dlltool --dllname era.dll --input-def era.def --machine i386 --output-lib libera.a`