//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once
/*
* The API was updated for ERA 3.9.15.
*/

#include <windows.h>
#include <cstdio>
#include <vector>

#include "../core.hpp"
#if NH3API_STD_MOVE_SEMANTICS
#include <initializer_list>
#else 
#include <string>	
#endif

// Means that returned pointer memory was allocated in era.dll and must be freed via MemFree after using
#define ERA_MEM(var) var NH3API_DEALLOCATOR(MemFree, 1)

// Means that variable is stored in persisted memory, which will neber be allocated and MUST NOT be modified
#define ERA_STATIC(var) var

#define ERA_API extern "C" NH3API_DLLIMPORT

namespace Era
{

typedef char TErmZVar[512];
typedef int32_t TXVars[16];

#pragma pack(push, 1)

const bool32_t EXEC_DEF_CODE = true;

NH3API_INLINE_OR_EXTERN
volatile int32_t*  const v NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0x887664, volatile int32_t)); // 1..10000
NH3API_INLINE_OR_EXTERN
volatile TErmZVar* const z NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0x9271E8, volatile TErmZVar)); // 1..1000
NH3API_INLINE_OR_EXTERN
volatile int32_t*  const  y NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0xA48D7C, volatile int32_t)); // 1..100
NH3API_INLINE_OR_EXTERN
volatile int32_t*  const x NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0x91DA34, volatile int32_t)); // 1..16
NH3API_INLINE_OR_EXTERN
volatile bool*     const f NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0x91F2DF, volatile bool)); // 1..1000
NH3API_INLINE_OR_EXTERN
volatile float*    const e NH3API_INLINE_OR_EXTERN_INIT(get_global_var_ptr(0xA48F14, volatile float)); // 1..100

enum ECallingConvention : int32_t
{
    CONV_LAST = -101,

    // Left-to-right
    CONV_PASCAL = CONV_LAST,

    // Left-to-right, first three arguments in EAX, EDX, ECX
    CONV_REGISTER = -102,

    // Right-to-left, caller clean-up
    CONV_CDECL = -103,

    // Right-to-left
    CONV_STDCALL = -104,

    // Right-to-left, first argument in ECX
    CONV_THISCALL = -105,

    // Right-to-left, first two arguments in ECX, EDX
    CONV_FASTCALL = -106
};

enum EGameMenuTarget : int32_t
{
    PAGE_DEFAULT    = -1,
    PAGE_NEW_GAME   = 101,
    PAGE_LOAD_GAME  = 102,
    PAGE_HIGH_SCORE = 103,
    PAGE_CREDITS    = 104,
    PAGE_QUIT       = 105,
    PAGE_RESTART    = 107,
    PAGE_MAIN       = 108,
};

enum ESearchDirection : int32_t
{
    SD_FORWARD  = -1,
    SD_BACKWARD = -2,
};

enum EImageResizeAlg : int32_t
{
    RESIZE_ALG_NO_RESIZE = 0, // Do not apply any constaints and do not change image dimensions
    RESIZE_ALG_STRETCH   = 1, // Assign image width and height to box width and height, scale unproportionally
    RESIZE_ALG_CONTAIN   = 2, // Resize image so, that at least one of its dimension become same as box dimension, and the other one less or equal to box dimension
    RESIZE_ALG_DOWNSCALE = 3, // Only downscale images proportionally, do not upscale small images
    RESIZE_ALG_UPSCALE   = 4, // Only upscale small images proportionally, leave big images as is
    RESIZE_ALG_COVER     = 5, // NOT IMPLEMENTED
    RESIZE_ALG_FILL      = 6, // Use image as a tile to fill the whole box
};

struct THookContext
{
    int32_t EDI; 
    int32_t ESI; 
    int32_t EBP; 
    int32_t ESP; 
    int32_t EBX; 
    int32_t EDX; 
    int32_t ECX; 
    int32_t EAX;
    int32_t RetAddr;
};

typedef bool32_t (__stdcall *THookHandler)(THookContext* Context);

struct TEvent
{
    char*   Name;
    void*   Data;
    int32_t DataSize;
};

typedef void __stdcall TEventHandler(TEvent* Event);

struct TGameState
{
    int32_t RootDlgId;
    int32_t CurrentDlgId;
};

NH3API_FORCEINLINE
/** Loads Era library and imports necessary functions. Must be called as soon as possible. */
void ConnectEra ()
{
    // May be extended in the future
}

// ======================= EVENTS ======================= //
/** Adds handler to be called when specific named event triggers, ex. "OnAfterWoG" or "OnSavegameWrite" */
ERA_API void __stdcall RegisterHandler (TEventHandler Handler, const char* EventName);

/** Triggers named event, passing custom event data of specified length to its handlers */
ERA_API void __stdcall FireEvent(const char* EventName, void* EventData, int32_t DataSize);
// ===================== END EVENTS ===================== //


// ======================= INTERNATIONALIZATION ======================= //
/** Changes current language code in memory without altering ini files or reloading already loaded data */
ERA_API int32_t __stdcall SetLanguage(const char* NewLanguage);

/** Reloads all json files from "Lang" directory and current language subdirectory */
ERA_API void __stdcall ReloadLanguageData();

/** Translates given key, using pairs of (key, value) params for translation. Returns temporary buffer address, which must be immediately copied */
ERA_API char* __stdcall trTemp(const char* key, const char* const* const params, int32_t LastParamsIndex);

#if NH3API_STD_MOVE_SEMANTICS
NH3API_FORCEINLINE
/**
 * Returns translation for given complex key ('xxx.yyy.zzz') with substituted parameters.
 * Pass vector of (parameter name, parameter value) pairs to substitute named parameters.
 * Example: Mod\Lang\*.json file: { "eqs": { "greeting": "Hello, @name@" } }
 * Example: ShowMessage(tr("eqs.greeting", { "name", "igrik" }).c_str());
 *
 * @param  key    Key to get translation for.
 * @param  params Vector of (parameter name, parameter value pairs).
 * @return        Translation string.
 */
std::string tr(const char* key, std::initializer_list<const char*> params)
{
   const int MAX_PARAMS = 64;
   const int numParams = ( params.size() <= MAX_PARAMS ? params.size() : MAX_PARAMS) & ~1;

   return trTemp(key, params.begin(), numParams - 1);
}
#else
NH3API_FORCEINLINE
std::string tr(const char* key, const std::vector<std::string>& params)
{
   const int MAX_PARAMS = 64;

   const char* _params[MAX_PARAMS]{};
   const int numParams = ( params.size() <= MAX_PARAMS ? params.size() : MAX_PARAMS) & ~1;

   for (int i = 0; i < numParams; i++) 
   {
     _params[i] = params[i].c_str();
   }

   return trTemp(key, _params, numParams - 1);
}

#endif

/** Translates given key, using pairs of (key, value) params for translation */
ERA_API ERA_MEM(char*) __stdcall _tr(const char* key, const char** params, int32_t LastParamsIndex);

/** Translates given key and returns persistent pointer to translation */
ERA_API ERA_STATIC(char*) __stdcall trStatic(const char* key);

NH3API_FORCEINLINE
/** Returns persisted/static translation for a given key */
ERA_STATIC(char*) tr (const char* key)
{ return trStatic(key); }

// ===================== END INTERNATIONALIZATION ===================== //


// ======================= ERM ======================= //


ERA_API int32_t __stdcall AllocErmFunc(const char* EventName, int32_t EventId);
ERA_API void __stdcall FireErmEvent(int32_t EventId);
ERA_API void __stdcall ExecErmCmd(const char* CmdStr);
ERA_API void __stdcall ExtractErm();
ERA_API void __stdcall ReloadErm();
ERA_API void __stdcall NameTrigger(int32_t TriggerId, const char *Name);

/** Displays regular ERM error dialog and can be used for ERM scripts error reporting and debugging */
ERA_API void __stdcall ShowErmError(const char *Error);

/** Returns pointer to array of arguments, that will be passed to ERM trigger as x-vars on ERM event generation */
ERA_API TXVars* __stdcall GetArgXVars();

/** Returns pointer to array of ERM x-vars copy after ERM has been handled. Some variables may be treated as result */
ERA_API TXVars* __stdcall GetRetXVars();

/** Returns human readable string for ERM event ID. Usually it's ERM trigger human readable name or ERM function name. */
ERA_API ERA_MEM(char*) __stdcall GetTriggerReadableName(int32_t TriggerId);

ERA_API int32_t __stdcall GetAssocVarIntValue(const char* VarName);
ERA_API ERA_MEM(char*) __stdcall GetAssocVarStrValue(const char* VarName);
ERA_API void __stdcall SetAssocVarIntValue(const char* VarName, int32_t NewValue);
ERA_API void __stdcall SetAssocVarStrValue(const char* VarName, const char* NewValue);
// ===================== END ERM ===================== //


// ======================= SHARED GLOBAL MEMORY ======================= //
ERA_API int32_t __stdcall GetEraRegistryIntValue(const char* VarName);
ERA_API ERA_MEM(char*) __stdcall GetEraRegistryStrValue(const char* VarName);
ERA_API void __stdcall SetEraRegistryIntValue(const char* VarName, int32_t NewValue);
ERA_API void __stdcall SetEraRegistryStrValue(const char* VarName, const char* NewValue);
// ===================== END SHARED GLOBAL MEMORY ===================== //


// ======================= CUSTOMIZABLE API ======================= //
typedef bool32_t (__stdcall *TIsCommanderId)(int32_t MonId);
ERA_API TIsCommanderId __stdcall SetIsCommanderIdFunc(TIsCommanderId NewImpl);

typedef bool32_t (__stdcall *TIsElixirOfLifeStack)(uint8_t* BattleStack);
ERA_API TIsElixirOfLifeStack __stdcall SetIsElixirOfLifeStackFunc(TIsElixirOfLifeStack NewImpl);

ERA_API void __stdcall SetRegenerationAbility(int32_t MonId, int32_t Chance, int32_t HitPoints, int32_t HpPercents);
ERA_API void __stdcall SetStdRegenerationEffect(int32_t Level7Percents, int32_t HpPercents);
// ===================== END CUSTOMIZABLE API ===================== //


// ======================= MAP RELATED FUNCTIONS ======================= //
ERA_API bool32_t __stdcall IsCampaign();
ERA_API ERA_MEM(char*) __stdcall GetCampaignFileName();
ERA_API ERA_MEM(char*) __stdcall GetMapFileName();
ERA_API int32_t __stdcall GetCampaignMapInd();

/** Works the same as UN:U with fast search syntax, but does not raise error on no more objects, returns success flag; Direction is -1 for FORWARD and -2 for BACKWARD. */
ERA_API bool32_t __stdcall FindNextObject(int32_t ObjType, int32_t ObjSubtype, int32_t* x, int32_t* y, int32_t* z, ESearchDirection Direction);
// ===================== END MAP RELATED FUNCTIONS ===================== //


// ======================= INI FILES ======================= //
ERA_API void __stdcall ClearAllIniCache();
ERA_API void __stdcall ClearIniCache(const char* FileName);

/** Reads entry from ini file. The fill will be cached in memory for further fast reading */
ERA_API bool32_t __stdcall ReadStrFromIni(const char* Key, const char* SectionName, const char* FilePath, char* ResultBuffer);

/** Writes new value to ini file cache in memory. Automatically loads ini to cache if necessary */
ERA_API bool32_t __stdcall WriteStrToIni(const char* Key, const char* Value, const char* SectionName, const char* FilePath);

/** Saves cached ini file data on disk */
ERA_API bool32_t __stdcall SaveIni(const char* FilePath);
// ===================== END INI FILES ===================== //


// ======================= HOOKS AND PATCHES ======================= //
/**
 * Calls handler function, when execution reaches specified address. Handler receives THookContext pointer.
 * If it returns true, overwritten commands are executed. Otherwise overwritten commands are skipped.
 * Change Context.RetAddr field to return to specific address after handler finishes execution with FALSE result.
 * The hook bridge code is always thread safe.
 */
ERA_API void* __stdcall HookCode(void* Addr, THookHandler HandlerFunc, void** AppliedPatch);

/**
 * Replaces original function with the new one with the same prototype and 1-2 extra arguments.
 * Calling convention is changed to STDCALL. The new argument is callable pointer, which can be used to
 * execute original function. The pointer is passed as THE FIRST argument. If custom parameter address
 * is given, the value of custom parameter will be passed to handler as THE SECOND argument. If AppliedPatch
 * pointer is given, it will be assigned an opaque pointer to applied patch data structure. This* pointer can
 * be used to rollback the patch (remove splicing).
 *
 * Returns address of the bridge to original function.
 *
 * Example:
 *   int32_t custom_param = 700;
 *   Splice((void*) 0x401000, (void*) MainProc, CONV_STDCALL, 2, &custom_param);
 *   int32_t __stdcall (void* orig_func, int32_t custom_param, int32_t arg1, int32_t arg2) MainProc {...}
 */
ERA_API void* __stdcall Splice(void* OrigFunc, void* HandlerFunc, ECallingConvention CallingConv, int32_t NumArgs, int32_t* CustomParam, void** AppliedPatch);

/** Writes Count bytes from Src buffer to Dst code block */
ERA_API void __stdcall WriteAtCode(int32_t Count, void* Src, void* Dst);

/** Calculates number of bytes to be overwritten during hook placement */
ERA_API int32_t __stdcall CalcHookPatchSize(void* pointer);

/** Rollback patch and free its memory. Do not use it afterwards */
ERA_API void __stdcall RollbackAppliedPatch(void* pointer);

/** Frees applied patch structure. Use it if you don't plan to rollback it at all */
ERA_API void __stdcall FreeAppliedPatch(void* pointer);// (*  *)
// ===================== END HOOKS AND PATCHES ===================== //


// ======================= DEBUG AND INFO ======================= //
/** Displays system message box with error and terminates application */
ERA_API void __stdcall FatalError(const char* Err);

/** Displays system message box with error. Terminates application only if Debug.AbortOnError = 1 in heroes3.ini */
ERA_API void __stdcall NotifyError(const char* error);

/** Generates full Era debug info (scripts, plugins, patches, context) and saves all the files in Debug\Era directory */
ERA_API void __stdcall GenerateDebugInfo();

/** Returns Era version string */
ERA_API ERA_STATIC(char*) __stdcall GetEraVersion();

/** Returns Era version number in 'X.X.XX' format. 3915 means 3.9.15 */
ERA_API int32_t __stdcall GetVersionNum();

/** Addes line to plugin versions message, shown on RMB on Credits button. The proposed format is: "{plugin name} vx.x.x". The function must be called in OnReportVersion event */
ERA_API void __stdcall ReportPluginVersion(const char* VersionLine);

/** Returns 32-character unique key for current game process. The ID will be unique between multiple game runs */
ERA_API ERA_STATIC(char*) __stdcall GetProcessGuid();

/** Returns IDs of game root dialog and current dialog. The first item in dialog class VMT tables is used as ID */
ERA_API void __stdcall GetGameState(TGameState* GameState);
// ===================== END DEBUG AND INFO ===================== //


// ======================= SAVEGAMES ======================= //
/** Appends data portion to a certain named savegame section during game saving */
ERA_API void __stdcall WriteSavegameSection(int32_t DataSize, void* Data, const char* SectionName);

/** Reads data portion from a certain named savegame section during game loading. Returns number of bytes actually loaded */
ERA_API int32_t __stdcall ReadSavegameSection(int32_t DataSize, void* Data, const char* SectionName);
// ===================== END SAVEGAMES ===================== //


// ======================= RESOURCES ======================= //
/** Returns ID of game button by its unique name, specified in *.btn file */
ERA_API int32_t __stdcall GetButtonID(const char* ButtonName);

/** Checks if binary patch exist. Provide full file name with extension like 'no prisons.bin' */
ERA_API bool32_t __stdcall PatchExists(const char* PatchName);

/** Tries to load PNG replacement for pcx file name and returns success flag */
ERA_API bool32_t __stdcall PcxPngExists(const char* PcxName);

/** Check if plugin with given name exists. Omit the extension. Era automatically searches for both deprecated 'dll' and modern 'era' extensions */
ERA_API bool32_t __stdcall PluginExists(const char* PluginName);

/** Creates resource redirection for current savegame only. Whenever an attempt to load non-cached resource with OldFileName will be made, NewFileName will be used instead */
ERA_API void __stdcall RedirectFile(const char* OldFileName, const char* NewFileName);

/**
 * Creates global resource redirection. Whenever an attempt to load non-cached resource with OldFileName will be made, NewFileName will be used instead.
 * Can be overriden by RedirectFile.
 */
ERA_API void __stdcall GlobalRedirectFile(const char* OldFileName, const char* NewFileName);
// ===================== END RESOURCES ===================== //


// ======================= MEMORY ======================= //
/**
 * Stores string contents in persistent memory under unique address. Returns existing address if string with the same contents exists.
 * The functions serves two purposes: data deduplication (only one string buffer for particular contents will be kept)
 * and pointer persistence (the pointer buffer will never be freed and thus can be safely used in dialogs and game structures).
 */
ERA_API char* __stdcall ToStaticStr(const char* Str);

/** Releases memory, allocated by Era. Such memory blocks are marked with ERA_MEM() in API */
ERA_API void __stdcall MemFree(const void* buf);

/** Informs Era about moved game array or structure into another location. Specify old array address, old array size and new address */
ERA_API void __stdcall RedirectMemoryBlock(void* OldAddr, int32_t BlockSize, void* NewAddr);

/** Returns final address for old known game array or structure. Uses information from all RedirectMemoryBlock calls. If nothing is found, returns address itself */
ERA_API void* __stdcall GetRealAddr(void* Addr);

NH3API_FORCEINLINE
/**
 * Assigns new string value to buffer.
 * @param Buf      Buffer to change contents of.
 * @param NewValue New string value.
 * @param BufSize  Maximal buffer size or -1 to ignore it (buffer overflow may occur).
 */
void SetPcharValue(char *Buf, const char *NewValue, int32_t BufSize) 
{
    if (BufSize < 0) 
    {
        lstrcpyA(Buf, NewValue);
    } 
    else if (BufSize > 0) 
    {
        int32_t NumBytesToCopy = lstrlenA(NewValue);

        if (NumBytesToCopy >= BufSize) 
            NumBytesToCopy = BufSize - 1;

        ::std::memcpy(Buf, NewValue, NumBytesToCopy);
        Buf[NumBytesToCopy] = '\0';
    }
}
// ===================== END MEMORY ===================== //


// ======================= CRYPTO ======================= //
/** Calculates data block hash */
ERA_API int32_t __stdcall Hash32(const char* data, int32_t DataSize);

/** Generates new random value using SplitMix32 algorithm and modifies input seed value */
ERA_API int32_t __stdcall SplitMix32(int32_t* seed, int32_t MinValue, int32_t MaxValue);
// ===================== END CRYPTO ===================== //


// ======================= GRAPHICS ======================= //
/**
 * Loads Pcx16 resource with rescaling support. Values <= 0 are considered 'auto'. If it's possible, images are scaled proportionally.
 * Resource name (name in binary resource tree) can be either fixed or automatic. Pass empty PcxName for automatic name.
 * If PcxName exceeds 12 characters, it's replaced with valid unique name. Check name field of result.
 * If resource is already registered and has proper format, it's returned with RefCount increased.
 * Result image dimensions may differ from requested if fixed PcxName is specified. Use automatic naming
 * to load image of desired size for sure.
 * Default image is returned in case of missing file and user is notified.
 */
ERA_API void* __stdcall LoadImageAsPcx16(const char* FilePath, const char* PcxName, int32_t Width, int32_t Height, int32_t MaxWidth, int32_t MaxHeight, EImageResizeAlg ResizeAlg);
// ===================== END GRAPHICS ===================== //


// ======================= ERA RICH TEXT SUPPORT ======================= //
/** Gives a name to a certain ARGB int32 color to use in colored texts */
ERA_API void __stdcall NameColor(int32_t Color32, const char* Name);
// ===================== END ERA RICH TEXT SUPPORT ===================== //


// ======================= DIALOGS AND MENUS ======================= //
/** Display in-game native message dialog with OK button */
ERA_API void __stdcall ShowMessage(const char *Message);

/** Raises special exception, closing all dialogs and forcing the game to return to the main menu */
ERA_API int32_t __stdcall FastQuitToGameMenu(EGameMenuTarget TargetScreen);
// ===================== END DIALOGS AND MENUS ===================== //


NH3API_FORCEINLINE
// ======================= UTILITIES ======================= //
/** Converts integer to std::string */
std::string IntToStr(int32_t value)
{ return to_std_string(value); }

/** Formats given positive or negative quantity to human-readable string with desired constraints on length */
ERA_API int32_t __stdcall FormatQuantity(int32_t value, char* buffer, int32_t BufSize, int32_t MaxLen, int32_t MaxDigits);

/** Converts integer to string, separating each three digit group by "era.locale.thousand_separator" character. */
ERA_API int32_t __stdcall DecorateInt(int32_t value, char* buffer, int32_t IgnoreSmallNumbers);
// ===================== END UTILITIES ===================== //

#pragma pack(pop)
} // namespace Era
