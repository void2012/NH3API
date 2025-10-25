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
/*
* The API was updated for ERA 3.9.16.
*/

#include <initializer_list>
#include "../core.hpp"

// Means that returned pointer memory was allocated in era.dll and must be freed via MemFree after using
#define ERA_MEM(var) var NH3API_DEALLOCATOR(::Era::MemFree, 1)

// Means that variable is stored in persisted memory, which will neber be allocated and MUST NOT be modified
#define ERA_STATIC(var) var

#define ERA_API extern "C" NH3API_DLLIMPORT

#define ERA_IMPORT_DUMMY(NAME) ERA_API void __cdecl NAME()

namespace Era_imports
{
    ERA_IMPORT_DUMMY(RegisterHandler);
    ERA_IMPORT_DUMMY(FireEvent);
    ERA_IMPORT_DUMMY(SetLanguage);
    ERA_IMPORT_DUMMY(ReloadLanguageData);
    ERA_IMPORT_DUMMY(tr);
    ERA_IMPORT_DUMMY(trTemp);
    ERA_IMPORT_DUMMY(trStatic);
    ERA_IMPORT_DUMMY(AllocErmFunc);
    ERA_IMPORT_DUMMY(FireErmEvent);
    ERA_IMPORT_DUMMY(ExecErmCmd);
    ERA_IMPORT_DUMMY(ExtractErm);
    ERA_IMPORT_DUMMY(ReloadErm);
    ERA_IMPORT_DUMMY(NameTrigger);
    ERA_IMPORT_DUMMY(ShowErmError);
    ERA_IMPORT_DUMMY(GetArgXVars);
    ERA_IMPORT_DUMMY(GetRetXVars);
    ERA_IMPORT_DUMMY(GetTriggerReadableName);
    ERA_IMPORT_DUMMY(GetAssocVarIntValue);
    ERA_IMPORT_DUMMY(GetAssocVarStrValue);
    ERA_IMPORT_DUMMY(SetAssocVarIntValue);
    ERA_IMPORT_DUMMY(SetAssocVarStrValue);
    ERA_IMPORT_DUMMY(GetEraRegistryIntValue);
    ERA_IMPORT_DUMMY(GetEraRegistryStrValue);
    ERA_IMPORT_DUMMY(SetEraRegistryIntValue);
    ERA_IMPORT_DUMMY(SetEraRegistryStrValue);
    ERA_IMPORT_DUMMY(SetIsCommanderIdFunc);
    ERA_IMPORT_DUMMY(SetIsElixirOfLifeStackFunc);
    ERA_IMPORT_DUMMY(SetRegenerationAbility);
    ERA_IMPORT_DUMMY(SetStdRegenerationEffect);
    ERA_IMPORT_DUMMY(IsCampaign);
    ERA_IMPORT_DUMMY(GetCampaignFileName);
    ERA_IMPORT_DUMMY(GetMapFileName);
    ERA_IMPORT_DUMMY(GetCampaignMapInd);
    ERA_IMPORT_DUMMY(FindNextObject);
    ERA_IMPORT_DUMMY(ClearAllIniCache);
    ERA_IMPORT_DUMMY(ClearIniCache);
    ERA_IMPORT_DUMMY(ReadStrFromIni);
    ERA_IMPORT_DUMMY(WriteStrToIni);
    ERA_IMPORT_DUMMY(SaveIni);
    ERA_IMPORT_DUMMY(_Hook); // renamed since v3.9.16
    ERA_IMPORT_DUMMY(_Splice); // renamed since v3.9.16
    ERA_IMPORT_DUMMY(WriteAtCode);
    // ERA_IMPORT_DUMMY(CalcHookPatchSize); // removed since v3.9.16
    ERA_IMPORT_DUMMY(RollbackAppliedPatch);
    ERA_IMPORT_DUMMY(FreeAppliedPatch);
    ERA_IMPORT_DUMMY(FatalError);
    ERA_IMPORT_DUMMY(NotifyError);
    ERA_IMPORT_DUMMY(GenerateDebugInfo);
    ERA_IMPORT_DUMMY(GetEraVersion);
    ERA_IMPORT_DUMMY(GetVersionNum);
    ERA_IMPORT_DUMMY(ReportPluginVersion);
    ERA_IMPORT_DUMMY(GetProcessGuid);
    ERA_IMPORT_DUMMY(GetGameState);
    ERA_IMPORT_DUMMY(WriteSavegameSection);
    ERA_IMPORT_DUMMY(ReadSavegameSection);
    ERA_IMPORT_DUMMY(GetButtonID);
    ERA_IMPORT_DUMMY(PatchExists);
    ERA_IMPORT_DUMMY(PcxPngExists);
    ERA_IMPORT_DUMMY(PluginExists);
    ERA_IMPORT_DUMMY(RedirectFile);
    ERA_IMPORT_DUMMY(GlobalRedirectFile);
    ERA_IMPORT_DUMMY(ToStaticStr);
    ERA_IMPORT_DUMMY(MemFree);
    ERA_IMPORT_DUMMY(RedirectMemoryBlock);
    ERA_IMPORT_DUMMY(GetRealAddr);
    ERA_IMPORT_DUMMY(Hash32);
    ERA_IMPORT_DUMMY(SplitMix32);
    ERA_IMPORT_DUMMY(LoadImageAsPcx16);
    ERA_IMPORT_DUMMY(NameColor);
    ERA_IMPORT_DUMMY(ShowMessage);
    ERA_IMPORT_DUMMY(FastQuitToGameMenu);
    ERA_IMPORT_DUMMY(FormatQuantity);
    ERA_IMPORT_DUMMY(DecorateInt);
    ERA_IMPORT_DUMMY(LogMemoryState); // since v3.9.16
    ERA_IMPORT_DUMMY(RegisterMemoryConsumer); // since v3.9.16
    ERA_IMPORT_DUMMY(_ClientMemAlloc); // since v3.9.16
    ERA_IMPORT_DUMMY(_ClientMemFree); // since v3.9.16
    ERA_IMPORT_DUMMY(_ClientMemRealloc); // since v3.9.16
    ERA_IMPORT_DUMMY(WriteLog); // since v3.9.16
    ERA_IMPORT_DUMMY(CreatePlugin); // since v3.9.16

} // namespace Era_imports

namespace Era
{

using TErmZVar = ::std::array<char, 512>;
using TXVars   = ::std::array<int32_t, 16>;

#pragma pack(push, 1)

const bool32_t EXEC_DEF_CODE = true;

inline int32_t* const  v = get_global_var_ptr(0x887664, int32_t);  // 1..10000
inline TErmZVar* const z = get_global_var_ptr(0x9271E8, TErmZVar); // 1..1000
inline int32_t* const  y = get_global_var_ptr(0xA48D7C, int32_t);  // 1..100
inline int32_t* const  x = get_global_var_ptr(0x91DA34, int32_t);  // 1..16
inline bool* const     f = get_global_var_ptr(0x91F2DF, bool);     // 1..1000
inline float* const    e = get_global_var_ptr(0xA48F14, float);    // 1..100

inline HINSTANCE hPlugin = nullptr;
using TPlugin            = void*;
inline TPlugin plugin    = nullptr;

enum ECallingConvention : int32_t
{
    // Right-to-left, caller clean-up
    CONV_CDECL = 0,

    // Right-to-left
    CONV_STDCALL = 1,

    // Right-to-left, first argument in ECX
    CONV_THISCALL = 2,

    // Right-to-left, first two arguments in ECX, EDX
    CONV_FASTCALL = 3,

    // Left-to-right, first three arguments in EAX, EDX, ECX
    CONV_REGISTER = 4,

    // Left-to-right
    CONV_PASCAL = 5

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

enum THookType : int32_t
{
    HOOKTYPE_BRIDGE = 0,
    HOOKTYPE_CALL   = 1,
    HOOKTYPE_JUMP   = 2
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

using THookHandler = bool32_t (__stdcall *)(THookContext* Context);

struct TEvent
{
    char*   Name;
    void*   Data;
    int32_t DataSize;
};

using TEventHandler = void (__stdcall*)(TEvent* Event);

struct TGameState
{
    int32_t RootDlgId;
    int32_t CurrentDlgId;
};

NH3API_FORCEINLINE
::exe_std_string era_GetModuleFileName (HINSTANCE hInstance)
{
    ::std::array<char, 256> buf{};
    DWORD filePathLen = ::GetModuleFileNameA(hInstance, buf.data(), sizeof(buf));
    if (filePathLen == 0)
        return "";

    ::exe_std_string filePath(buf.data(), ::std::min<DWORD>((DWORD)sizeof(buf) - 1, filePathLen));
    size_t lastSlashPos = filePath.find_last_of('\\');
    if (lastSlashPos != ::exe_std_string::npos)
    {
        filePath.erase(0, lastSlashPos + 1);
        return filePath;
    }
    else
    {
        return filePath;
    }
}

void __stdcall MemFree(const void* buf) noexcept;

NH3API_FORCEINLINE
// ======================= EVENTS ======================= //

/** Adds handler to be called when specific named event triggers, ex. "OnAfterWoG" or "OnSavegameWrite" */
void __stdcall RegisterHandler(TEventHandler Handler, const char* const EventName) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::RegisterHandler), Handler, EventName); }

/** Triggers named event, passing custom event data of specified length to its handlers */
NH3API_FORCEINLINE void __stdcall FireEvent(const char* const EventName, void* EventData, int32_t DataSize) noexcept
{ STDCALL_3(void, reinterpret_cast<uintptr_t>(&::Era_imports::FireEvent), EventName, EventData, DataSize); }

// ===================== END EVENTS ===================== //

// ======================= INTERNATIONALIZATION ======================= //

/** Changes current language code in memory without altering ini files or reloading already loaded data */
NH3API_FORCEINLINE int32_t __stdcall SetLanguage(const char* const NewLanguage) noexcept
{ return STDCALL_1(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::SetLanguage), NewLanguage); }

/** Reloads all json files from "Lang" directory and current language subdirectory */
NH3API_FORCEINLINE void __stdcall ReloadLanguageData() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::ReloadLanguageData)); }

/** Translates given key, using pairs of (key, value) params for translation. Returns temporary buffer address, which must be immediately copied */
NH3API_FORCEINLINE char* __stdcall trTemp(const char* const key, const char* const* const params, uint32_t LastParamsIndex) noexcept
{ return STDCALL_3(char*, reinterpret_cast<uintptr_t>(&::Era_imports::trTemp), key, params, LastParamsIndex); }

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
std::string tr(const char* key, std::initializer_list<const char*> params) noexcept
{
   constexpr int MAX_PARAMS = 64;
   const uint32_t numParams = (params.size() <= MAX_PARAMS ? params.size() : MAX_PARAMS) & ~1u;

   return trTemp(key, params.begin(), numParams - 1);
}

/** Translates given key, using pairs of (key, value) params for translation */
NH3API_FORCEINLINE ERA_MEM(char*) __stdcall tr(const char* const key, const char* const * const params, int32_t LastParamsIndex) noexcept
{ return STDCALL_3(char*, reinterpret_cast<uintptr_t>(&::Era_imports::tr), key, params, LastParamsIndex); }

/** Translates given key and returns persistent pointer to translation */
NH3API_FORCEINLINE ERA_STATIC(char*) __stdcall trStatic(const char* const key) noexcept
{ return STDCALL_1(char*, reinterpret_cast<uintptr_t>(&::Era_imports::trStatic), key); }

NH3API_FORCEINLINE
/** Returns persisted/static translation for a given key */
ERA_STATIC(char*) tr (const char* key) noexcept
{ return trStatic(key); }

// ===================== END INTERNATIONALIZATION ===================== //


// ======================= ERM ======================= //

NH3API_FORCEINLINE int32_t __stdcall AllocErmFunc(const char* const EventName, int32_t EventId) noexcept
{ return STDCALL_2(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::AllocErmFunc), EventName, EventId); }

NH3API_FORCEINLINE void __stdcall FireErmEvent(int32_t EventId) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::FireErmEvent), EventId); }

NH3API_FORCEINLINE void __stdcall ExecErmCmd(const char* const CmdStr) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::ExecErmCmd), CmdStr);  }

NH3API_FORCEINLINE void __stdcall ExtractErm() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::ExtractErm)); }

NH3API_FORCEINLINE void __stdcall ReloadErm() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::ReloadErm)); }

NH3API_FORCEINLINE void __stdcall NameTrigger(int32_t TriggerId, const char* const Name) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::NameTrigger), TriggerId, Name); }

/** Displays regular ERM error dialog and can be used for ERM scripts error reporting and debugging */
NH3API_FORCEINLINE void __stdcall ShowErmError(const char* const Error) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::ShowErmError), Error); }

/** Returns pointer to array of arguments, that will be passed to ERM trigger as x-vars on ERM event generation */
NH3API_FORCEINLINE TXVars* __stdcall GetArgXVars() noexcept
{ return STDCALL_0(TXVars*, reinterpret_cast<uintptr_t>(&::Era_imports::GetArgXVars)); }

/** Returns pointer to array of ERM x-vars copy after ERM has been handled. Some variables may be treated as result */
NH3API_FORCEINLINE TXVars* __stdcall GetRetXVars() noexcept
{ return STDCALL_0(TXVars*, reinterpret_cast<uintptr_t>(&::Era_imports::GetRetXVars)); }

/** Returns human readable string for ERM event ID. Usually it's ERM trigger human readable name or ERM function name. */
NH3API_FORCEINLINE ERA_MEM(char*) __stdcall GetTriggerReadableName(int32_t TriggerId) noexcept
{ return STDCALL_1(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetTriggerReadableName), TriggerId); }

NH3API_FORCEINLINE int32_t __stdcall GetAssocVarIntValue(const char* const VarName) noexcept
{ return STDCALL_1(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::GetAssocVarIntValue), VarName); }

NH3API_FORCEINLINE ERA_MEM(char*) __stdcall GetAssocVarStrValue(const char* const VarName) noexcept
{ return STDCALL_1(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetAssocVarStrValue), VarName); }

NH3API_FORCEINLINE void __stdcall SetAssocVarIntValue(const char* const VarName, int32_t NewValue) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetAssocVarIntValue), VarName, NewValue); }

NH3API_FORCEINLINE void __stdcall SetAssocVarStrValue(const char* const VarName, const char* const NewValue) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetAssocVarStrValue), VarName, NewValue); }
// ===================== END ERM ===================== //


// ======================= SHARED GLOBAL MEMORY ======================= //

NH3API_FORCEINLINE int32_t __stdcall GetEraRegistryIntValue(const char* const VarName) noexcept
{ return STDCALL_1(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::GetEraRegistryIntValue), VarName); }

NH3API_FORCEINLINE ERA_MEM(char*) __stdcall GetEraRegistryStrValue(const char* const VarName) noexcept
{ return STDCALL_1(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetEraRegistryStrValue), VarName); }

NH3API_FORCEINLINE void __stdcall SetEraRegistryIntValue(const char* const VarName, int32_t NewValue) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetEraRegistryIntValue), VarName, NewValue); }

NH3API_FORCEINLINE void __stdcall SetEraRegistryStrValue(const char* const VarName, const char* const NewValue) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetEraRegistryStrValue), VarName, NewValue); }

// ===================== END SHARED GLOBAL MEMORY ===================== //


// ======================= CUSTOMIZABLE API ======================= //

using TIsCommanderId = bool32_t (__stdcall *)(int32_t MonId);
NH3API_FORCEINLINE TIsCommanderId __stdcall SetIsCommanderIdFunc(TIsCommanderId NewImpl) noexcept
{ return STDCALL_1(TIsCommanderId, reinterpret_cast<uintptr_t>(&::Era_imports::SetIsCommanderIdFunc), NewImpl); }

using TIsElixirOfLifeStack = bool32_t (__stdcall *)(uint8_t* BattleStack);
NH3API_FORCEINLINE TIsElixirOfLifeStack __stdcall SetIsElixirOfLifeStackFunc(TIsElixirOfLifeStack NewImpl) noexcept
{ return STDCALL_1(TIsElixirOfLifeStack, reinterpret_cast<uintptr_t>(&::Era_imports::SetIsElixirOfLifeStackFunc), NewImpl); }

NH3API_FORCEINLINE void __stdcall SetRegenerationAbility(int32_t MonId, int32_t Chance, int32_t HitPoints, int32_t HpPercents) noexcept
{ STDCALL_4(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetRegenerationAbility), MonId, Chance, HitPoints, HpPercents); }

NH3API_FORCEINLINE void __stdcall SetStdRegenerationEffect(int32_t Level7Percents, int32_t HpPercents) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::SetStdRegenerationEffect), Level7Percents, HpPercents); }
// ===================== END CUSTOMIZABLE API ===================== //


// ======================= MAP RELATED FUNCTIONS ======================= //

NH3API_FORCEINLINE bool32_t __stdcall IsCampaign() noexcept
{ return STDCALL_0(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::IsCampaign)); }

NH3API_FORCEINLINE ERA_MEM(char*) __stdcall GetCampaignFileName() noexcept
{ return STDCALL_0(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetCampaignFileName)); }

NH3API_FORCEINLINE ERA_MEM(char*) __stdcall GetMapFileName() noexcept
{ return STDCALL_0(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetMapFileName)); }

NH3API_FORCEINLINE int32_t __stdcall GetCampaignMapInd() noexcept
{ return STDCALL_0(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::GetCampaignMapInd)); }

/** Works the same as UN:U with fast search syntax, but does not raise error on no more objects, returns success flag; Direction is -1 for FORWARD and -2 for BACKWARD. */
NH3API_FORCEINLINE bool32_t __stdcall FindNextObject(int32_t ObjType, int32_t ObjSubtype, int32_t* x, int32_t* y, int32_t* z, ESearchDirection Direction) noexcept
{ return STDCALL_6(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::FindNextObject), ObjType, ObjSubtype, x, y, z, Direction); }

// ===================== END MAP RELATED FUNCTIONS ===================== //


// ======================= INI FILES ======================= //

NH3API_FORCEINLINE void __stdcall ClearAllIniCache() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::ClearAllIniCache)); }

NH3API_FORCEINLINE void __stdcall ClearIniCache(const char* const FileName) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::ClearIniCache), FileName); }

/** Reads entry from ini file. The fill will be cached in memory for further fast reading */
NH3API_FORCEINLINE bool32_t __stdcall ReadStrFromIni(const char* const Key, const char* const SectionName, const char* const FilePath, char* ResultBuffer) noexcept
{ return STDCALL_4(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::ReadStrFromIni), Key, SectionName, FilePath, ResultBuffer); }

/** Writes new value to ini file cache in memory. Automatically loads ini to cache if necessary */
NH3API_FORCEINLINE bool32_t __stdcall WriteStrToIni(const char* const Key, const char* const Value, const char* const SectionName, const char* const FilePath) noexcept
{ return STDCALL_4(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::WriteStrToIni), Key, Value, SectionName, FilePath); }

/** Saves cached ini file data on disk */
NH3API_FORCEINLINE bool32_t __stdcall SaveIni(const char* const FilePath) noexcept
{ return STDCALL_1(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::SaveIni), FilePath); }

// ===================== END INI FILES ===================== //


// ======================= HOOKS AND PATCHES ======================= //

// Removed since v3.9.16, see Hook
NH3API_FORCEINLINE void* __stdcall HookCode(void* Addr, THookHandler HandlerFunc, void** AppliedPatch) NH3API_DELETED_FUNCTION

/**
 * Installs new hook at specified address. Returns pointer to bridge with original code if any. Optionally specify address of a pointer to write applied patch structure
 * pointer to. It will allow to rollback the patch later. MinCodeSize specifies original code size to be erased (nopped). Use 0 in most cases.
 *
 * In case of bridge hook type calls handler function, when execution reaches specified address. Handler receives THookContext pointer.
 * If it returns true, overwritten commands are executed. Otherwise overwritten commands are skipped.
 * Change Context.RetAddr field to return to specific address after handler finishes execution with FALSE result.
 * The hook bridge code is always thread safe.
 */
NH3API_FORCEINLINE void* __stdcall Hook(void* Addr, THookHandler HandlerFunc, void** AppliedPatch = nullptr, int32_t MinCodeSize = 0, THookType HookType = HOOKTYPE_BRIDGE) noexcept
{ return STDCALL_5(void*, reinterpret_cast<uintptr_t>(&::Era_imports::_Hook), Addr, HandlerFunc, AppliedPatch, MinCodeSize, HookType); }

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
NH3API_FORCEINLINE void* __stdcall Splice(void* OrigFunc, void* HandlerFunc, ECallingConvention CallingConv, int32_t NumArgs, int32_t* CustomParam = nullptr, void** AppliedPatch = nullptr) noexcept
{ return STDCALL_6(void*, reinterpret_cast<uintptr_t>(&::Era_imports::_Splice), OrigFunc, HandlerFunc, CallingConv, NumArgs, CustomParam, AppliedPatch); }

/** Writes Count bytes from Src buffer to Dst code block */
NH3API_FORCEINLINE void __stdcall WriteAtCode(int32_t Count, void* Src, void* Dst) noexcept
{ STDCALL_3(void, reinterpret_cast<uintptr_t>(&::Era_imports::WriteAtCode), Count, Src, Dst); }

// Removed since v3.9.16
NH3API_FORCEINLINE int32_t __stdcall CalcHookPatchSize(void* pointer) NH3API_DELETED_FUNCTION

/** Rollback patch and free its memory. Do not use it afterwards */
NH3API_FORCEINLINE void __stdcall RollbackAppliedPatch(void* pointer) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::RollbackAppliedPatch), pointer); }

/** Frees applied patch structure. Use it if you don't plan to rollback it at all */
NH3API_FORCEINLINE void __stdcall FreeAppliedPatch(void* pointer) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::FreeAppliedPatch), pointer); }
// ===================== END HOOKS AND PATCHES ===================== //


// ======================= DEBUG AND INFO ======================= //

/** Displays system message box with error and terminates application */
NH3API_FORCEINLINE void __stdcall FatalError(const char* const Err) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::FatalError), Err); }

/** Displays system message box with error. Terminates application only if Debug.AbortOnError = 1 in heroes3.ini */
NH3API_FORCEINLINE void __stdcall NotifyError(const char* const error) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::NotifyError), error); }

/** Generates full Era debug info (scripts, plugins, patches, context) and saves all the files in Debug\Era directory */
NH3API_FORCEINLINE void __stdcall GenerateDebugInfo() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::GenerateDebugInfo)); }

/** Returns Era version string */
NH3API_FORCEINLINE ERA_STATIC(char*) __stdcall GetEraVersion() noexcept
{ return STDCALL_0(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetEraVersion)); }

/** Returns Era version number in 'X.X.XX' format. 3915 means 3.9.15 */
NH3API_FORCEINLINE int32_t __stdcall GetVersionNum() noexcept
{ return STDCALL_0(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::GetVersionNum)); }

NH3API_FORCEINLINE
/** Addes line to plugin versions message, shown on RMB on Credits button. The proposed format is: "{plugin name} vx.x.x". The function must be called in OnReportVersion event */
void __stdcall ReportPluginVersion(const char* const VersionLine) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::ReportPluginVersion), VersionLine); }

/** Returns 32-character unique key for current game process. The ID will be unique between multiple game runs */
NH3API_FORCEINLINE ERA_STATIC(char*) __stdcall GetProcessGuid() noexcept
{ return STDCALL_0(char*, reinterpret_cast<uintptr_t>(&::Era_imports::GetProcessGuid)); }

/** Returns IDs of game root dialog and current dialog. The first item in dialog class VMT tables is used as ID */
NH3API_FORCEINLINE void __stdcall GetGameState(TGameState* GameState) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::GetGameState), GameState); }

/**
 * Appends entry to "log.txt" file in the following form: >> [EventSource]: [Operation] #13#10 [Description].
 * Example: WriteLog("SaveGame", "Save monsters section", "Failed to detect monster array size")
 * Available since v3.9.16
 */
NH3API_FORCEINLINE bool32_t __stdcall WriteLog(const char* EventSource, const char* Operation, const char* Description) noexcept
{ return STDCALL_3(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::WriteLog), EventSource, Operation, Description); }

// ===================== END DEBUG AND INFO ===================== //


// ======================= SAVEGAMES ======================= //

/** Appends data portion to a certain named savegame section during game saving */
NH3API_FORCEINLINE void __stdcall WriteSavegameSection(int32_t DataSize, void* Data, const char* SectionName) noexcept
{ STDCALL_3(void, reinterpret_cast<uintptr_t>(&::Era_imports::WriteSavegameSection), DataSize, Data, SectionName); }

/** Reads data portion from a certain named savegame section during game loading. Returns number of bytes actually loaded */
NH3API_FORCEINLINE int32_t __stdcall ReadSavegameSection(int32_t DataSize, void* Data, const char* SectionName) noexcept
{ return STDCALL_3(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::ReadSavegameSection), DataSize, Data, SectionName); }
// ===================== END SAVEGAMES ===================== //


// ======================= RESOURCES ======================= //

/** Returns ID of game button by its unique name, specified in *.btn file */
NH3API_FORCEINLINE int32_t __stdcall GetButtonID(const char* ButtonName) noexcept
{ return STDCALL_1(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::GetButtonID), ButtonName); }

/** Checks if binary patch exist. Provide full file name with extension like 'no prisons.bin' */
NH3API_FORCEINLINE bool32_t __stdcall PatchExists(const char* PatchName) noexcept
{ return STDCALL_1(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::PatchExists), PatchName); }

/** Tries to load PNG replacement for pcx file name and returns success flag */
NH3API_FORCEINLINE bool32_t __stdcall PcxPngExists(const char* PcxName) noexcept
{ return STDCALL_1(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::PcxPngExists), PcxName); }

/** Check if plugin with given name exists. Omit the extension. Era automatically searches for both deprecated 'dll' and modern 'era' extensions */
NH3API_FORCEINLINE bool32_t __stdcall PluginExists(const char* PluginName) noexcept
{ return STDCALL_1(bool32_t, reinterpret_cast<uintptr_t>(&::Era_imports::PluginExists), PluginName); }

/** Creates resource redirection for current savegame only. Whenever an attempt to load non-cached resource with OldFileName will be made, NewFileName will be used instead */
NH3API_FORCEINLINE void __stdcall RedirectFile(const char* OldFileName, const char* NewFileName) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::RedirectFile), OldFileName, NewFileName); }

/**
 * Creates global resource redirection. Whenever an attempt to load non-cached resource with OldFileName will be made, NewFileName will be used instead.
 * Can be overriden by RedirectFile.
 */
NH3API_FORCEINLINE void __stdcall GlobalRedirectFile(const char* OldFileName, const char* NewFileName) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::GlobalRedirectFile), OldFileName, NewFileName); }
// ===================== END RESOURCES ===================== //


// ======================= MEMORY ======================= //

/**
 * Stores string contents in persistent memory under unique address. Returns existing address if string with the same contents exists.
 * The functions serves two purposes: data deduplication (only one string buffer for particular contents will be kept)
 * and pointer persistence (the pointer buffer will never be freed and thus can be safely used in dialogs and game structures).
 */
NH3API_FORCEINLINE char* __stdcall ToStaticStr(const char* Str) noexcept
{ return STDCALL_1(char*, reinterpret_cast<uintptr_t>(&::Era_imports::ToStaticStr), Str); }

/** Releases memory, allocated by Era. Such memory blocks are marked with ERA_MEM() in API */
NH3API_FORCEINLINE void __stdcall MemFree(const void* buf) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::MemFree), buf); }

/** Informs Era about moved game array or structure into another location. Specify old array address, old array size and new address */
NH3API_FORCEINLINE void __stdcall RedirectMemoryBlock(void* OldAddr, int32_t BlockSize, void* NewAddr) noexcept
{ STDCALL_3(void, reinterpret_cast<uintptr_t>(&::Era_imports::RedirectMemoryBlock), OldAddr, BlockSize, NewAddr); }

/** Returns final address for old known game array or structure. Uses information from all RedirectMemoryBlock calls. If nothing is found, returns address itself */
NH3API_FORCEINLINE void* __stdcall GetRealAddr(void* Addr) noexcept
{ return STDCALL_1(void*, reinterpret_cast<uintptr_t>(&::Era_imports::GetRealAddr), Addr); }

NH3API_FORCEINLINE
/**
 * Assigns new string value to buffer.
 * @param Buf      Buffer to change contents of.
 * @param NewValue New string value.
 * @param BufSize  Maximal buffer size or -1 to ignore it (buffer overflow may occur).
 */
void SetPcharValue(char * __restrict Buf, const char * __restrict NewValue, int32_t BufSize)  noexcept
{
    if (BufSize < 0)
    {
        ::lstrcpyA(Buf, NewValue);
    }
    else if (BufSize > 0)
    {
        int32_t NumBytesToCopy = ::lstrlenA(NewValue);

        if (NumBytesToCopy >= BufSize)
            NumBytesToCopy = BufSize - 1;

        ::std::memcpy(Buf, NewValue, NumBytesToCopy);
        Buf[NumBytesToCopy] = '\0';
    }
}

/* Writes memory consumption info to main log file */
/* Available since v3.9.16 */
NH3API_FORCEINLINE void __stdcall LogMemoryState() noexcept
{ STDCALL_0(void, reinterpret_cast<uintptr_t>(&::Era_imports::LogMemoryState)); }

/**
 * Registers memory consumer (plugin with own memory manager) and returns address of allocated memory counter, which
 * consumer should atomically increase and decrease in malloc/calloc/realloc/free operations.
 * Available since v3.9.16
 */
NH3API_FORCEINLINE size_t* __stdcall RegisterMemoryConsumer(const char* ConsumerName) noexcept
{ return STDCALL_1(size_t*, reinterpret_cast<uintptr_t>(&::Era_imports::LogMemoryState), ConsumerName); }

// ===================== END MEMORY ===================== //


// ======================= CRYPTO ======================= //

/** Calculates data block hash */
NH3API_FORCEINLINE int32_t __stdcall Hash32(const char* data, int32_t DataSize) noexcept
{ return STDCALL_2(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::Hash32), data, DataSize); }

/** Generates new random value using SplitMix32 algorithm and modifies input seed value */
NH3API_FORCEINLINE int32_t __stdcall SplitMix32(int32_t* seed, int32_t MinValue, int32_t MaxValue) noexcept
{ return STDCALL_3(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::SplitMix32), seed, MinValue, MaxValue); }
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
NH3API_FORCEINLINE void* __stdcall LoadImageAsPcx16(const char* FilePath, const char* PcxName, int32_t Width, int32_t Height, int32_t MaxWidth, int32_t MaxHeight, EImageResizeAlg ResizeAlg) noexcept
{ return STDCALL_7(void*, reinterpret_cast<uintptr_t>(&::Era_imports::LoadImageAsPcx16), FilePath, PcxName, Width, Height, MaxWidth, MaxHeight, ResizeAlg); }

// ===================== END GRAPHICS ===================== //


// ======================= ERA RICH TEXT SUPPORT ======================= //

/** Gives a name to a certain ARGB int32 color to use in colored texts */
NH3API_FORCEINLINE void __stdcall NameColor(int32_t Color32, const char* Name) noexcept
{ STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::NameColor), Color32, Name); }

// ===================== END ERA RICH TEXT SUPPORT ===================== //


// ======================= DIALOGS AND MENUS ======================= //

/** Display in-game native message dialog with OK button */
NH3API_FORCEINLINE void __stdcall ShowMessage(const char *Message) noexcept
{ STDCALL_1(void, reinterpret_cast<uintptr_t>(&::Era_imports::ShowMessage), Message); }

/** Raises special exception, closing all dialogs and forcing the game to return to the main menu */
NH3API_FORCEINLINE int32_t __stdcall FastQuitToGameMenu(EGameMenuTarget TargetScreen) noexcept
{ return STDCALL_1(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::FastQuitToGameMenu), TargetScreen); }
// ===================== END DIALOGS AND MENUS ===================== //


NH3API_FORCEINLINE
// ======================= UTILITIES ======================= //

/** Converts integer to std::string */
std::string IntToStr(int32_t value)
{ return to_std_string(value); }

/** Formats given positive or negative quantity to human-readable string with desired constraints on length */
NH3API_FORCEINLINE int32_t __stdcall FormatQuantity(int32_t value, char* buffer, int32_t BufSize, int32_t MaxLen, int32_t MaxDigits) noexcept
{ return STDCALL_5(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::FormatQuantity), value, buffer, BufSize, MaxLen, MaxDigits); }

/** Converts integer to string, separating each three digit group by "era.locale.thousand_separator" character. */
NH3API_FORCEINLINE int32_t __stdcall DecorateInt(int32_t value, char* buffer, int32_t IgnoreSmallNumbers) noexcept
{ return STDCALL_3(int32_t, reinterpret_cast<uintptr_t>(&::Era_imports::DecorateInt), value, buffer, IgnoreSmallNumbers); }

// ===================== END UTILITIES ===================== //

/** Creates new plugin API instance for particular DLL plugin. Pass real dll name with extension. Returns plugin instance or NULL is plugin is already created */
NH3API_FORCEINLINE TPlugin __stdcall CreatePlugin(const char* Name) noexcept
{ return STDCALL_1(TPlugin, reinterpret_cast<uintptr_t>(&::Era_imports::CreatePlugin), Name); }

NH3API_FORCEINLINE
/** Loads Era library and imports necessary functions. Must be called as soon as possible. */
void ConnectEra(HINSTANCE PluginDllHandle, const char* PluginName)
{
    ::exe_std_string finalPluginName = era_GetModuleFileName(PluginDllHandle);

    if (PluginName && *PluginName)
        finalPluginName = PluginName;

    plugin = CreatePlugin(finalPluginName.c_str());

    if (!plugin)
        FatalError((::exe_std_string("Duplicate registered plugin: ") + era_GetModuleFileName(PluginDllHandle)).c_str());
}

#pragma pack(pop)
} // namespace Era

namespace EraMemory
{
    inline volatile size_t* allocatedMemorySize = nullptr;

    using ::Era::RegisterMemoryConsumer;

    NH3API_FORCEINLINE void* __stdcall _ClientMemAlloc(volatile size_t* allocatedSize, size_t Size) noexcept
    { return STDCALL_2(void*, reinterpret_cast<uintptr_t>(&::Era_imports::_ClientMemAlloc), allocatedSize, Size); }

    NH3API_FORCEINLINE void __stdcall _ClientMemFree(volatile size_t* allocatedSize, const void* Buf) noexcept
    { STDCALL_2(void, reinterpret_cast<uintptr_t>(&::Era_imports::_ClientMemFree), allocatedSize, Buf); }

    NH3API_FORCEINLINE void* __stdcall _ClientMemRealloc(volatile size_t* allocatedSize, const void* Buf, size_t NewSize) noexcept
    { return STDCALL_3(void*, reinterpret_cast<uintptr_t>(&::Era_imports::_ClientMemRealloc), allocatedSize, Buf, NewSize); }

    struct CurrentModuleHandleGetter
    {
        static NH3API_NOINLINE HMODULE Get() noexcept
        {
            HMODULE result = nullptr;
            ::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCSTR>(&CurrentModuleHandleGetter::Get), &result);
            return result;
        }
    };

    NH3API_FORCEINLINE
    void InitMemoryManager()
    {
        static bool _MemoryManagerInitialized = false;
        if (!_MemoryManagerInitialized)
        {
            ::std::array<char, MAX_PATH> pluginLibraryPath{};
            const size_t filePathLen = ::GetModuleFileNameA(CurrentModuleHandleGetter::Get(), pluginLibraryPath.data(), sizeof(pluginLibraryPath));

            if (filePathLen > 0)
            {
                const char* pluginLibraryName = ::strrchr(pluginLibraryPath.data(), '\\');
                pluginLibraryName = (pluginLibraryName) ? pluginLibraryName + 1 : pluginLibraryPath.data();
                allocatedMemorySize = RegisterMemoryConsumer(pluginLibraryName);
            }
            _MemoryManagerInitialized = true;
        }
    }
}
