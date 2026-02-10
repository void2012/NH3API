//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NH3API 2024
// Patcher_x86 header modified for the compability with the NH3API:
// Changes:
// 1. C99 Fixed width integer types
// 2. char* -> const char* parameters (const-correctness)
// 3. FASTCALL_1 macro is changed to FASTCALL1
// 4. Explicit calling convention call macros
// 5. Added JmpHook for CreateInstance
// 6. Added SafeLoHook for CreateInstance (v5.0)
// 7. Added GetPatcherVersion (v5.0)
// 8. LoHooks are deprecated
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// patcher_x86 library
// spread freely (free of charge)
// copyright: Barinov Alexander (baratorch), e-mail: baratorch@yandex.ru
// the form of implementation of low-level hooks (LoHook) is partially borrowed from Berserker (from ERA)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// библиотека patcher_x86
// распространяется свободно(бесплатно)
// авторское право: Баринов Александр (baratorch), e-mail: baratorch@yandex.ru
// форма реализации низкоуровневых хуков (LoHook) отчасти позаимствована у Berserker (из ERA)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// version 5.0 //
// версия 5.0  //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION.
//
//! The library provides:
// - convenient unified centralized
// tools for installing patches and hooks
// in the code of the target program.
// - additional tools: disassembler of the lengths of the opcodes and the function
// copy code with correct transfer of opcodes jmp and call c
// relative addressing
//! Library allows
// - set both simple and complex patches.
// - set high-level hooks, replacing the original functions in
// in the target code for their own, without worrying about the registers of the processor,
// stack, and return to the original code.
// - install high-level hooks one on another
// not excluding and adding functionality of hooks
// set before the last
// - install low-level hooks with high-level access to
// the registers of the processor, the stack, the erased code and the return address in the code
// - cancel any patch and hook installed with this library.
// - find out whether a particular mode is being used that uses the library
// - find out which mod (using the library) installed a specific patch / hook
// - get full access to all patches / hooks installed from other mods
// using this library
// - easily and quickly detect conflicting patches from different mods
// (using this library) 1) marking the log such conflicts as:
// - patches / hooks of different size are set to the same address
// - install patches / hooks overlapping each other with offset
// - patches are installed on top of the hooks and vice versa
// as well as 2) giving the opportunity to look at the dump (common listing) of all patches
// and hooks installed using this library at a particular time.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LOGGING.
//
// by default in patcher_x86.dll the logging is disabled to enable it,
// you must create the patcher_x86.ini file in the same folder with the only
// write: Logging = 1 (Logging = 0 - disables logging again)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// RULES OF USE.
//
// 1) each mod should call the GetPatcher () function 1 time, saving the result
// for example: Patcher * _P = GetPatcher ();
// 2) then using the Patcher::CreateInstance method, you need to create
// instance of PatcherInstance with its unique name
// for example: PatcherInstance * _PI = _P-> CreateInstance ("MyMod");
// 3) then use the methods of classes Patcher and PatcherInstance
// directly to work with patches and hooks
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ОПИСАНИЕ.
//
// ! библиотека предоставляет:
//  - удобные унифицированные централизованные
//    инструменты для установки патчей и хуков
//    в код целевой программы.
//  - дополнительные инструменты: дизассемблер длин опкодов и функцию
//    копирующую код с корректным переносом опкодов jmp и call c
//    относительной адресацией
// ! библиотека позволяет
//  - устанавливать как простые так и сложные патчи.
//  - устанавливать высокоуровневые хуки, замещая оригинальные функции в
//    в целевом коде на свои, не заботясь о регистрах процессора,
//    стэке, и возврате в оригинальный код.
//  - устанавливать высокоуровневые хуки один на другой
//    не исключая а дополняя при этом функциональность хуков
//    установленных раньше последнего
//  - устанавливать низкоуровневые хуки с высокоуровневым доступом к
//    регистрам процессора, стэку, затертому коду и адресу возврата в код
//  - отменять любой патч и хук, установленный с помощью этой библиотеки.
//  - узнать задействован ли определенный мод, использующий библиотеку
//  - узнать какой мод (использующий библиотеку) установил определенный патч/хук
//  - получить полный доступ ко всем патчам/хукам, установленным из других модов
//    с помощью этой библиотеки
//  - легко и быстро обнаружить конфликтующие патчи из разных модов
//    (использующих эту библиотеку) 1) отмечаяв логе такие конфликты как:
//        - устанавливаются патчи/хуки разного размера на один адрес
//        - устанавливаются патчи/хуки перекрывающие один другого со смещением
//        - устанавливаются патчи поверх хуков и наоборот
//    а так же 2) давая возможность посмотреть дамп (общий листинг) всех патчей
//    и хуков установленных с помощью этой библиотеки в конкретный момент времени.
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ЛОГИРОВАНИЕ.
//
// по умолчанию в patcher_x86.dll логирование отключено, чтобы включить его,
// необходимо в той же папке создать файл patcher_x86.ini c единственной
// записью: Logging = 1 (Logging = 0 - отключает логирование снова)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ПРАВИЛА ИСПОЛЬЗОВАНИЯ.
//
// 1) каждый мод должен 1 раз вызвать функцию GetPatcher(), сохранив результат
//  например: Patcher* _P = GetPatcher();
// 2) затем с помощью метода Pather::CreateInstance нужно создать
// экземпляр PatсherInstance со своим уникальным именем
//  например: PatсherInstance* _PI = _P->CreateInstance("MyMod");
// 3)  затем использовать методы классов Patсher и PatсherInstance
// непосредственно для работы с патчами и хуками
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>

#if __has_include(<libloaderapi.h>)
#include <libloaderapi.h>
#else
#include <winbase.h>
#endif

#include "intrin.hpp"
#include "call_macros.hpp"
#include "stl_extras.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wnon-virtual-dtor")
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_GNUC_DISABLE("-Wunused-parameter")
NH3API_WARNING_GNUC_DISABLE("-Wcast-function-type")
#ifdef __clang__
NH3API_WARNING_GNUC_DISABLE("-Wnested-anon-types")
NH3API_WARNING_GNUC_DISABLE("-Wcast-function-type-strict")
NH3API_WARNING_GNUC_DISABLE("-Wcast-function-type-mismatch")
NH3API_WARNING_GNUC_DISABLE("-Wgnu-anonymous-struct")
#endif
NH3API_WARNING_MSVC_DISABLE(4100)
NH3API_WARNING_MSVC_DISABLE(4191)
NH3API_WARNING_MSVC_DISABLE(4265)
NH3API_WARNING_MSVC_DISABLE(4714)

#pragma pack(push, 4)
// type "variable", is used for the values returned by Patcher::VarInit and Patcher::VarFind methods /
// тип "переменная", используется для возвращаемых методами Patcher::VarInit и Patcher::VarFind значений.
NH3API_VIRTUAL_STRUCT Variable
{
public:
    Variable()                           = delete;
    Variable(const Variable&)            = delete;
    Variable(Variable&&)                 = delete;
    Variable& operator=(const Variable&) = delete;
    Variable& operator=(Variable&&)      = delete;
    ~Variable()                          = delete;

    // returns the value of 'variable' (thread safe handling) /
    // возвращает значение 'переменной' (потокобезопасное обращение).
    [[nodiscard]] virtual uint32_t __stdcall GetValue() const = 0;

    // sets the value of the 'variable' (thread safe handling) /
    // устанавливает значение 'переменной' (потокобезопасное обращение).
    virtual void __stdcall SetValue(uint32_t value) = 0;

    // returns a pointer to a value (accessing the value through the pointer is not secure) /
    // возвращает указатель на значение (обращение к значению через указатель непотокобезопасно).
    [[nodiscard]] virtual uint32_t* __stdcall GetPValue() = 0;
};

// тип 'регистр флагов', размер - 32 бита
// используется в HookContext.
// type 'register of flags', size - 32 bits /
// used in HookContext.
struct FlagsRegister
{
    bool CF   : 1; // 0x00000001
    bool      : 1; // 0x00000002, reserved.
    bool PF   : 1; // 0x00000004
    bool      : 1; // 0x00000008, reserved.
    bool AF   : 1; // 0x00000010
    bool      : 1; // 0x00000020, reserved.
    bool ZF   : 1; // 0x00000040
    bool SF   : 1; // 0x00000080
    bool TF   : 1; // 0x00000100
    bool IF   : 1; // 0x00000200
    bool DF   : 1; // 0x00000400
    bool OF   : 1; // 0x00000800
    uint8_t IOPL : 2; // 0x00003000
    bool NT   : 1; // 0x00004000
    bool      : 1; // 0x00008000, reserved.
    bool RF   : 1; // 0x00010000
    bool VM   : 1; // 0x00020000
    bool AC   : 1; // 0x00040000
    bool VIF  : 1; // 0x00080000
    bool VIP  : 1; // 0x00100000
    bool ID   : 1; // 0x00200000
    bool      : 1; // 0x00400000, reserved.
    bool      : 1; // 0x00800000, reserved.
    bool      : 1; // 0x01000000, reserved.
    bool      : 1; // 0x02000000, reserved.
    bool      : 1; // 0x04000000, reserved.
    bool      : 1; // 0x08000000, reserved.
    bool      : 1; // 0x10000000, reserved.
    bool      : 1; // 0x20000000, reserved.
    bool      : 1; // 0x40000000, reserved.
    bool      : 1; // 0x80000000, reserved
} NH3API_MSVC_LAYOUT;

// HookContext structure
// used in functions of Lohook hook /
// Структура HookContext
// используется в функциях сработавших по LoHook хуку.
struct HookContext
{
    HookContext()                              = delete;
    HookContext(const HookContext&)            = delete;
    HookContext(HookContext&&)                 = delete;
    HookContext& operator=(const HookContext&) = delete;
    HookContext& operator=(HookContext&&)      = delete;
    ~HookContext()                             = delete;

    union
    {
        int32_t eax;
        union
        {
            int16_t ax;
            #pragma pack(push, 2)
            struct
            {
                int8_t al;
                int8_t ah;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;
    union
    {
        int32_t ecx;
        union
        {
            int16_t cx;
            #pragma pack(push, 2)
            struct
            {
                int8_t cl;
                int8_t ch;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;
    union
    {
        int32_t edx;
        union
        {
            int16_t dx;
            #pragma pack(push, 2)
            struct
            {
                int8_t dl;
                int8_t dh;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;
    union
    {
        int32_t ebx;
        union
        {
            int16_t bx;
            #pragma pack(push, 2)
            struct
            {
                int8_t bl;
                int8_t bh;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;

    union
    {
        uintptr_t esp;
        uint16_t  sp;
    } NH3API_MSVC_LAYOUT;

    union
    {
        uintptr_t ebp;
        uint16_t  bp;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t esi;
        int16_t si;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t edi;
        int16_t di;
    } NH3API_MSVC_LAYOUT;

    // return address, read/write.
    // will be jumped into after LoHook returned /
    // адрес возврата, чтение/изменение.
    // Определяет, куда будет совершен прыжок после возврата LoHook.
    uintptr_t return_address;

    // flags register, read/write /
    // регистр флагов, чтение/изменение
    FlagsRegister flags;

    // the Push function has a similar action to the PUSH processor command for the LoHook hook context
    // when used with the hook context set with WriteLoHook or CreateLoHook
    // the size of the memory that can be put on the stack with this function is limited to 128 bytes.
    // when used with the hook context set with WriteLoHookEx or CreateLoHookEx
    // this size is set arbitrarily by calling WriteLoHookEx or CreateLoHookEx.
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // функция Push имеет аналогичное действие команде процессора PUSH для контекста LoHook хука
    // при использовании с контекстом хука установленного с помощью WriteLoHook или CreateLoHook
    // размер памяти которая может быть помещена в стек с помощью этой функции ограничен 128 байтами.
    // при использовании с контекстом хука установленного с помощью WriteLoHookEx или CreateLoHookEx
    // этот размер устанавливается произвольно при вызове WriteLoHookEx или CreateLoHookEx.
    NH3API_NO_SANITIZE_ADDRESS inline void Push(int32_t v) noexcept
    {
        esp -= 4;
        *reinterpret_cast<int32_t*>(esp) = v;
    }

    // the Pop function has a similar action to the POP command for the LoHook hook context /
    // функция Pop имеет аналогичное действие команде процессора POP для контекста LoHook хука.
    NH3API_NO_SANITIZE_ADDRESS inline int32_t Pop() noexcept
    {
        int32_t r = *reinterpret_cast<int32_t*>(esp);
        esp += 4;
        return r;
    }

    // These getters are for the compability with the RoseKavailer's H3API...

    //
    inline constexpr int8_t& AL() noexcept { return al; }
    inline constexpr int8_t& AH() noexcept { return ah; }
    inline constexpr int16_t& AX() noexcept { return ax; }
    inline constexpr int8_t& CL() noexcept { return cl; }
    inline constexpr int8_t& CH() noexcept { return ch; }
    inline constexpr int16_t& CX() noexcept { return cx; }
    inline constexpr int8_t& DL() noexcept { return dl; }
    inline constexpr int8_t& DH() noexcept { return dh; }
    inline constexpr int16_t& DX() noexcept { return dx; }
    inline constexpr int8_t& BL() noexcept { return bl; }
    inline constexpr int8_t& BH() noexcept { return bh; }
    inline constexpr int16_t& BX() noexcept { return bx; }
    inline constexpr uint16_t& BP() noexcept { return bp; }
    inline constexpr int16_t& SI() noexcept { return si; }
    inline constexpr int16_t& DI() noexcept { return di; }

    [[nodiscard]] inline constexpr const int8_t&   AL() const noexcept { return al; }
    [[nodiscard]] inline constexpr const int8_t&   AH() const noexcept { return ah; }
    [[nodiscard]] inline constexpr const int16_t&  AX() const noexcept { return ax; }
    [[nodiscard]] inline constexpr const int8_t&   CL() const noexcept { return cl; }
    [[nodiscard]] inline constexpr const int8_t&   CH() const noexcept { return ch; }
    [[nodiscard]] inline constexpr const int16_t&  CX() const noexcept { return cx; }
    [[nodiscard]] inline constexpr const int8_t&   DL() const noexcept { return dl; }
    [[nodiscard]] inline constexpr const int8_t&   DH() const noexcept { return dh; }
    [[nodiscard]] inline constexpr const int16_t&  DX() const noexcept { return dx; }
    [[nodiscard]] inline constexpr const int8_t&   BL() const noexcept { return bl; }
    [[nodiscard]] inline constexpr const int8_t&   BH() const noexcept { return bh; }
    [[nodiscard]] inline constexpr const int16_t&  BX() const noexcept { return bx; }
    [[nodiscard]] inline constexpr const uint16_t& BP() const noexcept { return bp; }
    [[nodiscard]] inline constexpr const int16_t&  SI() const noexcept { return si; }
    [[nodiscard]] inline constexpr const int16_t&  DI() const noexcept { return di; }

} NH3API_MSVC_LAYOUT;


// Структура SafeLoHookContext
// используется в функциях сработавших по потокобезопасному SafeLoHook хуку
// см. PatcherInstance::CreateSafeLoHook().
///////////////////////////////////////////////////////////////////////////
// Структура SafeLoHookContext
// используется в функциях сработавших по потокобезопасному SafeLoHook хуку
// см. PatcherInstance::CreateSafeLoHook().
struct SafeLoHookContext
{
    SafeLoHookContext()                                    = delete;
    SafeLoHookContext(const SafeLoHookContext&)            = delete;
    SafeLoHookContext(SafeLoHookContext&&)                 = delete;
    SafeLoHookContext& operator=(const SafeLoHookContext&) = delete;
    SafeLoHookContext& operator=(SafeLoHookContext&&)      = delete;
    ~SafeLoHookContext()                                   = delete;

    uintptr_t return_address;

    FlagsRegister flags;

    union
    {
        int32_t edi;
        int16_t di;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t esi;
        int16_t si;
    } NH3API_MSVC_LAYOUT;

    union
    {
        uintptr_t ebp;
        uint16_t  bp;
    } NH3API_MSVC_LAYOUT;

    union
    {
        const uintptr_t esp; // immutable
        const uint16_t  sp;  // immutable
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t ebx;
        union
        {
            int16_t bx;
            #pragma pack(push, 2)
            struct
            {
                int8_t bl;
                int8_t bh;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t edx;
        union
        {
            int16_t dx;
            #pragma pack(push, 2)
            struct
            {
                int8_t dl;
                int8_t dh;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t ecx;
        union
        {
            int16_t cx;
            #pragma pack(push, 2)
            struct
            {
                int8_t cl;
                int8_t ch;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;

    union
    {
        int32_t eax;
        union
        {
            int16_t ax;
            #pragma pack(push, 2)
            struct
            {
                int8_t al;
                int8_t ah;
            };
            #pragma pack(pop)
        } NH3API_MSVC_LAYOUT;
    } NH3API_MSVC_LAYOUT;

    void    Push(int32_t v)  = delete;
    void    Push(uint32_t v) = delete;
    int32_t Pop()            = delete;

    inline constexpr int8_t&   AL() noexcept { return al; }
    inline constexpr int8_t&   AH() noexcept { return ah; }
    inline constexpr int16_t&  AX() noexcept { return ax; }
    inline constexpr int8_t&   CL() noexcept { return cl; }
    inline constexpr int8_t&   CH() noexcept { return ch; }
    inline constexpr int16_t&  CX() noexcept { return cx; }
    inline constexpr int8_t&   DL() noexcept { return dl; }
    inline constexpr int8_t&   DH() noexcept { return dh; }
    inline constexpr int16_t&  DX() noexcept { return dx; }
    inline constexpr int8_t&   BL() noexcept { return bl; }
    inline constexpr int8_t&   BH() noexcept { return bh; }
    inline constexpr int16_t&  BX() noexcept { return bx; }
    inline constexpr uint16_t& BP() noexcept { return bp; }
    inline constexpr int16_t&  SI() noexcept { return si; }
    inline constexpr int16_t&  DI() noexcept { return di; }

    [[nodiscard]] inline constexpr const int8_t&   AL() const noexcept { return al; }
    [[nodiscard]] inline constexpr const int8_t&   AH() const noexcept { return ah; }
    [[nodiscard]] inline constexpr const int16_t&  AX() const noexcept { return ax; }
    [[nodiscard]] inline constexpr const int8_t&   CL() const noexcept { return cl; }
    [[nodiscard]] inline constexpr const int8_t&   CH() const noexcept { return ch; }
    [[nodiscard]] inline constexpr const int16_t&  CX() const noexcept { return cx; }
    [[nodiscard]] inline constexpr const int8_t&   DL() const noexcept { return dl; }
    [[nodiscard]] inline constexpr const int8_t&   DH() const noexcept { return dh; }
    [[nodiscard]] inline constexpr const int16_t&  DX() const noexcept { return dx; }
    [[nodiscard]] inline constexpr const int8_t&   BL() const noexcept { return bl; }
    [[nodiscard]] inline constexpr const int8_t&   BH() const noexcept { return bh; }
    [[nodiscard]] inline constexpr const int16_t&  BX() const noexcept { return bx; }
    [[nodiscard]] inline constexpr const uint16_t& BP() const noexcept { return bp; }
    [[nodiscard]] inline constexpr const int16_t&  SI() const noexcept { return si; }
    [[nodiscard]] inline constexpr const int16_t&  DI() const noexcept { return di; }

} NH3API_MSVC_LAYOUT;

using SafeHookContext = SafeLoHookContext;

// values returned by a LoHook function /
// значения возвращаемые функцией срабатываемой по LoHook хуку.
enum ELoHookReturnPolicy : int32_t
{
    EXEC_DEFAULT    = 1,
    NO_EXEC_DEFAULT = 0,
    SKIP_DEFAULT    = NO_EXEC_DEFAULT // alias for NO_EXEC_DEFAULT
};

// values returned by Patch::GetType () /
// значения возвращаеемые Patch::GetType().
enum EPatchType : int32_t
{
    PATCH_ = 0,
    LOHOOK_ = 1,
    HIHOOK_ = 2
};

// values passed to PatcherInstance::Write () and PatcherInstance::CreatePatch () /
// значения передаваемые PatcherInstance::Write() и PatcherInstance::CreatePatch().
enum EPatcherInstanceWriteMode : int32_t
{
    DATA_ = 0,
    CODE_ = 1
};

// Abstract class Patch
// you can create an instance with
// using the methods of class PatcherInstance
/////////////////////////////////////////////
// Абстрактный класс Patch
// создать экземпляр можно с
// помощью методов класса PatcherInstance.
NH3API_VIRTUAL_CLASS Patch
{
public:
    Patch()                        = delete;
    Patch(const Patch&)            = delete;
    Patch(Patch&&)                 = delete;
    Patch& operator=(const Patch&) = delete;
    Patch& operator=(Patch&&)      = delete;
    ~Patch()                       = delete;

    // Returns the address on which to install the patch /
    // Возвращает адрес по которому устанавливается патч.
    [[nodiscard]] virtual uintptr_t __stdcall GetAddress() const = 0;

    // Returns the size of the patch /
    // Возвращает размер патча.
    [[nodiscard]] virtual uint32_t __stdcall GetSize() const = 0;

    // Returns the unique name of the PatcherInstance instance with which the patch was created /
    // Возвращает уникальное имя экземпляра PatcherInstance, с помощью которого был создан патч.
    [[nodiscard]] virtual const char* __stdcall GetOwner() const = 0;

    // Returns the type of the patch
    // for not hook always PATCH_
    // for LoHook always LOHOOK_
    // for HiHook always HIHOOK_
    ////////////////////////////////
    // Возвращает тип патча
    // для не хука всегда PATCH_
    // для LoHook всегда LOHOOK_
    // для HiHook всегда HIHOOK_
    [[nodiscard]] virtual EPatchType __stdcall GetType() const = 0;

    // returns true if the patch is applied and false, if not /
    // возвращает true, если патч применен и false, если нет.
    [[nodiscard]] virtual bool32_t __stdcall IsApplied() const = 0;

    // Apply the patch
    // returns> = 0 if the patch / hook is applied successfully
    // (the return value is the sequence number of the patch in the sequence
    // patches applied around the given address, the larger the number,
    // the later the patch was applied)
    // returns -2 if the patch is already applied
    // The result of executing the method is commonly written to the log
    ////////////////////////////////////////////////////////////////////////////////
    // Применяет патч
    // возвращает >= 0 , если патч/хук применился успешно
    // (возвращаемое значение является порядковым номером патча в последовательности
    // патчей, примененных по окрестности данного адреса, чем больше число,
    // тем позднее был применен патч)
    // возвращает -2, если патч уже применен
    // Результат выполнения метода распространенно пишется в лог.
    virtual int32_t __stdcall Apply() = 0;

    // ApplyInsert applies a patch specifying the sequence number in the
    // sequences of patches applied to this address.
    // the return values are the same as in Patch::Apply
    // the ApplyInsert function, you can pass an argument to the value returned
    // Undo function to apply the patch to the same place it was before the cancel.
    //////////////////////////////////////////////////////////////////////////////////////
    // ApplyInsert применяет патч с указанием порядкового номера в
    // последовательности патчей, примененных по этому адресу.
    // возвращаемые значения аналогичны соответсвующим в Patch::Apply
    // функции ApplyInsert можно аргументом передать значение, возвращаемое
    // функцией Undo, чтобы применить патч в то же место, на котором тот был до отменения.
    virtual int32_t __stdcall ApplyInsert(int32_t zorder) = 0;

    // Undo method
    // Undo the patch (hook) (in case the patch is applied last - restores the erased code)
    // Returns the number> = 0 if the patch (hook) was canceled successfully
    // (the return value is the patch number in the sequence
    // patches applied to this address, the larger the number,
    // the later the patch was applied)
    // Returns -2 if the patch has already been canceled (not applied)
    // The result of executing the method is commonly written to the log
    ////////////////////////////////////////////////////////////////////////////////////////////
    // Метод Undo
    // Отменяет патч(хук) (в случае если патч применен последним - восстанавливает затертый код)
    // Возвращает число >= 0, если патч(хук) был отменен успешно
    // (возвращаемое значение является номером патча в последовательности
    // патчей, примененных по данному адресу, чем больше число,
    // тем позднее был применен патч)
    // Возвращает -2, если патч и так уже был отменен (не был применен)
    // Результат выполнения метода распространенно пишется в лог.
    virtual bool32_t __stdcall Undo() = 0;

    // Destroy method
    // Destructor
    // Cancels and permanently destroys the patch / hook
    // returns always 1 (for compatibility with earlier versions of the library)
    // The result of the destruction is commonly written to the log
    ////////////////////////////////////////////////////////////////////////////
    // Метод Destroy
    // Деструктор
    // Отменяет и безвозвратно уничтожает патч/хук
    // возвращает всегда 1 (для совместимости с более ранними версиями библиотеки)
    // Результат уничтожения распространенно пишется в лог
    virtual bool32_t __stdcall Destroy() = 0;

protected:
    virtual Patch* __stdcall xGetAppliedBefore() = 0;
    virtual Patch* __stdcall xGetAppliedAfter() = 0;

public:
    // GetAppliedBefore method
    // returns the patch applied before the data
    // returns nullptr if this patch is applied first
    //////////////////////////////////////////////////////
    // Метод GetAppliedBefore
    // возвращает патч примененный перед данным
    // возвращает nullptr если данный патч применен первым
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] Patch* GetAppliedBefore()
    { return xGetAppliedBefore(); }

    // GetAppliedBefore method
    // returns the patch applied before the data
    // returns nullptr if this patch is applied first
    //////////////////////////////////////////////////////
    // Метод GetAppliedBefore
    // возвращает патч примененный перед данным
    // возвращает nullptr если данный патч применен первым
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] const Patch* GetAppliedBefore() const
    { return const_cast<Patch*>(this)->xGetAppliedBefore(); }

    // GetAppliedAfter method
    // returns the patch applied after the given
    // returns NULL if this patch is applied last
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод GetAppliedAfter
    // возвращает патч примененный после данного
    // возвращает nullptr если данный патч применен последним
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] Patch* GetAppliedAfter()
    { return xGetAppliedAfter(); }

    // GetAppliedAfter method
    // returns the patch applied after the given
    // returns NULL if this patch is applied last
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод GetAppliedAfter
    // возвращает патч примененный после данного
    // возвращает nullptr если данный патч применен последним
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] const Patch* GetAppliedAfter() const
    { return const_cast<Patch*>(this)->xGetAppliedAfter(); }

};

// Abstract class LoHook (inherited from Patch, that is, essentially low-hook is a patch)
// you can create an instance with
// using the methods of class PatcherInstance
// Note that LoHook takes 5 bytes on its address of the original instructions!
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Абстрактный класс LoHook (унаследован от Patch, т.е. по сути лоу-хук является патчем)
// создать экземпляр можно с
// помощью методов класса PatcherInstance
// Обратите внимание, что при установке лоухука он затирает 5 байт оригинальных инструкций на адресе установки!
NH3API_VIRTUAL_CLASS LoHook : public Patch
{
    public:
        LoHook()                         = delete;
        LoHook(const LoHook&)            = delete;
        LoHook(LoHook&&)                 = delete;
        LoHook& operator=(const LoHook&) = delete;
        LoHook& operator=(LoHook&&)      = delete;
        ~LoHook()                        = delete;
};

using SafeLoHook                = LoHook;
using _LoHookFunc_              = int32_t(__stdcall*)(LoHook*, HookContext*);
using _LoHookReferenceFunc_     = int32_t(__stdcall*)(LoHook&, HookContext&);
using _SafeLoHookFunc_          = int32_t(__stdcall*)(SafeLoHook*, SafeLoHookContext*);
using _SafeLoHookReferenceFunc_ = int32_t(__stdcall*)(SafeLoHook&, SafeLoHookContext&);
using lhfunc_t   = _LoHookFunc_;
using lhrfunc_t  = _LoHookReferenceFunc_;
using slhfunc_t  = _SafeLoHookFunc_;
using slhrfunc_t = _SafeLoHookReferenceFunc_;

// values passed as the hooktype argument in PatcherInstance::WriteHiHook and PatcherInstance::CreateHiHook /
// значения передаваемые как аргумент hooktype в PatcherInstance::WriteHiHook и PatcherInstance::CreateHiHook.
enum EHiHookSetupPolicy : int32_t
{
    CALL_    = 0,
    SPLICE_  = 1,
    FUNCPTR_ = 2
};

// values passed as the subtype argument in PatcherInstance::WriteHiHook and PatcherInstance::CreateHiHook /
// значения передаваемые как аргумент subtype в PatcherInstance::WriteHiHook и PatcherInstance::CreateHiHook.
enum EHiHookType : int32_t
{
    DIRECT_   = 0,
    EXTENDED_ = 1,
    SAFE_     = 2
};

// values passed as a calltype argument to PatcherInstance::WriteHiHook and PatcherInstance::CreateHiHook
// значения передаваемые как аргумент calltype в PatcherInstance::WriteHiHook и PatcherInstance::CreateHiHook.
enum EHiHookCallingConvention : int32_t
{
    STDCALL_  = 0,
    ANY_      = STDCALL_,
    THISCALL_ = 1,
    FASTCALL_ = 2,
    CDECL_    = 3,
    FASTCALL1 = THISCALL_
};

// Abstract class HiHook (inherited from Patch, that is, essentially hi-hook is a patch)
// you can create an instance using methods from the class PatcherInstance /
// Абстрактный класс HiHook (унаследован от Patch, т.е. по сути хай-хук является патчем)
// создать экземпляр можно с помощью методов класса PatcherInstance.
NH3API_VIRTUAL_CLASS HiHook : public Patch
{
public:
    HiHook()                         = delete;
    HiHook(const HiHook&)            = delete;
    HiHook(HiHook&&)                 = delete;
    HiHook& operator=(const HiHook&) = delete;
    HiHook& operator=(HiHook&&)      = delete;
    ~HiHook()                        = delete;

    // returns a pointer to the function (on the bridge to the function in the case of SPLICE_),
    // replaced by a hook
    // Attention! Calling a function for an unused hook, you can get
    // irrelevant (but working) value.
    ////////////////////////////////////////////////////////////////////////////////////////////
    // возвращает указатель на функцию (на мост к функции в случае SPLICE_),
    // замещенную хуком
    // Внимание! Вызывая функцию для не примененного хука, можно получить
    // неактуальное (но рабочее) значение.
    [[nodiscard]] virtual uintptr_t __stdcall GetDefaultFunc() const = 0;

    // returns a pointer to the original function (on the bridge to the function in the case of SPLICE_),
    // replaced by a hook (hooks) at this address
    // (that is, it returns GetDefaultFunc () for the first hook applied to this address)
    // Attention! Calling a function for an unused hook, you can get
    // irrelevant (but working) value.
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // возвращает указатель на оригинальную функцию (на мост к функции в случае SPLICE_),
    // замещенную хуком(хуками) по данному адресу
    // (т.е. возвращает GetDefaultFunc() для первого примененного хука по данному адресу)
    // Внимание! Вызывая функцию для не примененного хука, можно получить
    // неактуальное (но рабочее) значение.
    [[nodiscard]] virtual uintptr_t __stdcall GetOriginalFunc() const = 0;

    // returns the return address to the original code
    // can be used inside the hook function
    // SPLICE_ EXTENDED_ or SAFE_ hook to find out where it came from
    // for SPLICE_ DIRECT_ hook function always returns 0 (ie for DIRECT_ hook possibility to find the return address through it - no)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // возвращает адрес возврата в оригинальный код
    // можно использовать внутри хук-функции
    // SPLICE_ EXTENDED_ или SAFE_ хука, чтобы узнать откуда она была вызвана
    // для SPLICE_ DIRECT_ хука функция возвращает всегда 0 (т.е. для DIRECT_ хука возможности узнать адрес возврата через нее - нет)
    [[nodiscard]] virtual uintptr_t __stdcall GetReturnAddress() const = 0;

    // # ver 2.1
    // sets the value of user hook data /
    // устанавливает значение пользовательских данных хука
    virtual void __stdcall SetUserData(uint32_t data) = 0;

    // # ver 2.1
    // returns the value of the user hook data
    // if not specified by the user, then 0 /
    // возвращает значение пользовательских данных хука
    // если не задано пользователем, то равно 0
    [[nodiscard]] virtual uint32_t __stdcall GetUserData() = 0;

};

// Abstract class PatcherInstance
// create / get an instance using the CreateInstance and GetInstance methods of the Patcher class
// directly allows you to create / install patches and hooks in the code,
// adding them to the tree of all the patches / hooks created by the patcher_x86.dll library
/////////////////////////////////////////////////////////////////////////////////////////////////
// Абстрактный класс PatcherInstance
// создать/получить экземпляр можно с помощью методов CreateInstance и GetInstance класса Patcher
// непосредственно позволяет создавать/устанавливать патчи и хуки в код,
// добавляя их в дерево всех патчей/хуков, созданных библиотекой patcher_x86.dll
NH3API_VIRTUAL_CLASS PatcherInstance
{
public:
    PatcherInstance()                                  = delete;
    PatcherInstance(const PatcherInstance&)            = delete;
    PatcherInstance(PatcherInstance&&)                 = delete;
    PatcherInstance& operator=(const PatcherInstance&) = delete;
    PatcherInstance& operator=(PatcherInstance&&)      = delete;
    ~PatcherInstance()                                 = delete;

    // WriteByte method
    // write a one-byte number at address
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    ////////////////////////////////////////////
    // Метод WriteByte
    // пишет однобайтовое число по адресу address
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч.
    virtual Patch* __stdcall WriteByte(uintptr_t address, uint8_t value) = 0;

    // WriteWord method
    // write a two-byte number at address
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    ////////////////////////////////////////////////////////////
    // Метод WriteWord
    // пишет двухбайтовое число по адресу address
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч.
    virtual Patch* __stdcall WriteWord(uintptr_t address, uint16_t value) = 0;

    // WriteDword method
    // write a four-byte number at address
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    /////////////////////////////////////////////////
    // Метод WriteDword
    // пишет четырехбайтовое число по адресу address
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч.
    virtual Patch* __stdcall WriteDword(uintptr_t address, uint32_t value) = 0;

    // WriteAddressOf template
    // writes a pointer of data type (its address)
    // to the specified location
    // The data type can be anything
    //////////////////////////////////////////////
    // WriteAddressOf шаблон
    // пишет указатель на данные (адрес)
    // по определенному адресу программы
    // Тип может быть любым, даже функция.
    template<typename T> NH3API_NO_SANITIZE_ADDRESS inline Patch* WriteAddressOf(uintptr_t address, const T& data)
    { return WriteDword(address, *reinterpret_cast<uint32_t*>(__builtin_addressof(data))); }

    // WriteFloat method
    // write a four-byte floating point number at address
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    /////////////////////////////////////////////////
    // Метод WriteFloat
    // пишет четырехбайтовое число с плавающей точкой по адресу address
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч.
    NH3API_NO_SANITIZE_ADDRESS inline Patch* WriteFloat(uintptr_t address, float data)
    { return WriteDword(address, nh3api::bit_cast<uint32_t>(data)); }

    // WriteJmp method
    // writes jmp to opcode at address
    // (creates and applies a CODE_ patch)
    // Returns the pointer to the patch
    // the patch closes an integer number of opcodes,
    // i.e. The size of the patch> = 5, the difference is filled with NOPs.
    ////////////////////////////////////////////////////////////
    // Метод WriteJmp
    // пишет jmp to опкод по адресу address
    // (создает и применяет CODE_ патч)
    // Возвращает указатель на патч
    // патч закрывает целое количество опкодов,
    // т.е. размер патча >= 5, разница заполнятеся NOP'ами.
    virtual Patch* __stdcall WriteJmp(uintptr_t address, uintptr_t to) = 0;

    NH3API_NO_SANITIZE_ADDRESS inline Patch* WriteJmp(uintptr_t address, const void* to)
    { return WriteJmp(address, reinterpret_cast<uintptr_t>(to)); }

    // WriteHexPatch method
    // writes to the address address the byte sequence,
    // defined hex_str
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    // hex_str - c-string can contain hexadecimal digits
    // 0123456789ABCDEF (uppercase only!) Other characters
    // when reading by the method hex_str ignored (skipped)
    // convenient to use as an argument to this method
    // copied using Binary copy in OllyDbg
    ///////////////////////////////////////////////////////////////
    // Метод WriteHexPatch
    // пишет по адресу address позледовательность байт,
    // определенную hex_str
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч
    // hex_str - си-строка может содержать шестнадцатеричные цифры
    // 0123456789ABCDEF (только верхний регистр!) остальные символы
    // при чтении методом hex_str игнорируются(пропускаются)
    // удобно использовать в качестве аргумента этого метода
    // скопированное с помощью Binary copy в OllyDbg
    // пример:
    // patcherInstance->WriteHexPatch(0x57b521, "6A 01  6A 00");
    virtual Patch* __stdcall WriteHexPatch(uintptr_t address, const char* hex_str) = 0;

protected:
    // Method WriteCodePatchVA
    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteCodePatch
    ////////////////////////////////////////////////////////////
    // Метод WriteCodePatchVA
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteCodePatch.
    virtual Patch* __stdcall WriteCodePatchVA(uintptr_t address, const char* __restrict format, uint32_t* __restrict va_args) = 0;

public:
    [[deprecated("LoHooks are deprecated(unless you REALLY need to modify to esp). Use SafeLoHooks instead.")]]
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Method WriteLoHook
    // (upd. v5.0: It is not recommended to use WriteLoHook anymore. Use WriteSafeLoHook instead.)
    // creates an address low-level hook (CODE_ patch) and applies it
    // returns pointer to the hook
    // func - function called when the hook triggers
    // must have the form int __stdcall func (LoHook * h, HookContext * c);
    // in HookContext * c are passed for reading / changing
    // processor registers and return address
    // if func returns EXEC_DEFAULT, then
    // after the func is completed, the code is truncated.
    // if - SKIP_DEFAULT - the erased code is not executed
    //
    // ATTENTION!
    // the size of the memory that can be placed on the context stack
    // using c-> esp and c-> Push, is limited to 128 bytes.
    // if you need another restriction, use the WriteLoHookEx or CreateLoHookEx method.
    ////////////////////////////////////////////////////////////
    // Метод WriteLoHook
    // (upd. v5.0: Использование WriteLoHook больше не рекоммендуется. Используйте WriteSafeLoHook.)
    // создает по адресу address низкоуровневый хук (CODE_ патч) и применяет его
    // возвращает указатель на хук
    // func - функция вызываемая при срабатывании хука
    // должна иметь вид int __stdcall func(LoHook* h, HookContext* c);
    // в HookContext* c передаются для чтения/изменения
    // регистры процессора и адрес возврата
    // если func возвращает EXEC_DEFAULT, то
    // после завершения func выполняется затертый хуком код.
    // если - SKIP_DEFAULT - затертый код не выполняется
    //
    // ВНИМАНИЕ!
    // размер памяти, которая может быть помещена в стек контекста
    // с помощью использования c->esp и с->Push, ограничен 128 байтами.
    // если требуется иное ограничение используйте метод WriteLoHookEx или CreateLoHookEx.
    virtual LoHook* __stdcall WriteLoHook(uintptr_t address, lhfunc_t func) = 0;

    #ifndef DECLARE_LH
        // declare lohook function
        #define DECLARE_LH(NAME)   int32_t __stdcall NAME([[maybe_unused]] LoHook* __restrict h, [[maybe_unused]] HookContext* __restrict c)
        #define DECLARE_LHR(NAME)  int32_t __stdcall NAME([[maybe_unused]] LoHook* __restrict h, [[maybe_unused]] HookContext& __restrict c)
        // declare safelohook function
        #define DECLARE_SLH(NAME)  int32_t __stdcall NAME([[maybe_unused]] SafeLoHook* __restrict h, [[maybe_unused]] SafeHookContext* __restrict c)
        #define DECLARE_SLHR(NAME) int32_t __stdcall NAME([[maybe_unused]] SafeLoHook* __restrict h, [[maybe_unused]] SafeHookContext& __restrict c)

        // These define-s are for the compability with RoseKavalier's H3API

        #define _LHF_(NAME)   int32_t __stdcall NAME([[maybe_unused]] LoHook* __restrict h, [[maybe_unused]] HookContext* __restrict c)
        #define _LHREF_(NAME) int32_t __stdcall NAME([[maybe_unused]] LoHook& __restrict h, [[maybe_unused]] HookContext& __restrict c)
    #endif

protected:
    #if NH3API_CHECK_MSVC || NH3API_CHECK_MINGW
    template<typename R, typename... Args> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition_stdcall(R (*func)(Args...)) noexcept
    {
        (void) func;
        static_assert(false, "HiHook function must have the __stdcall calling convention "
                             "and have the first argument of type HiHook*");
    }

    template<typename R, typename Arg1, typename... Args> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition_argument(R (__stdcall* func)(Arg1, Args...)) noexcept
    {
        (void) func;
        static_assert(std::is_same_v<Arg1, HiHook*>, "HiHook function must have the first argument of type HiHook*");
    }

    template<typename R, typename... Args> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition_stdcall(R (__stdcall * func)(Args...)) noexcept
    {
        static_assert(sizeof...(Args) > 0, "HiHook function must have at least one argument of type HiHook*.");
        hihook_function_condition_argument(func);
    }

    template<typename F> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition(F* func) noexcept
    {
        static_assert(std::is_function_v<F>, "HiHook must be a function.");
        hihook_function_condition_stdcall(func);
    }

    #else // MSVC or MinGW GCC

    template<typename>
    struct is_stdcall_function_helper_t
    { using type = std::false_type; };

    template<typename... Args, typename R>
    struct is_stdcall_function_helper_t<R (__stdcall *)(HiHook*, Args...)>
    { using type = std::true_type; };

    template<typename... Args, typename R>
    struct is_stdcall_function_helper_t<R (__stdcall *)(HiHook&, Args...)>
    { using type = std::true_type; };

    template<typename... Args, typename R>
    struct is_stdcall_function_helper_t<R (__stdcall *)(const HiHook*, Args...)>
    { using type = std::true_type; };

    template<typename... Args, typename R>
    struct is_stdcall_function_helper_t<R (__stdcall *)(const HiHook&, Args...)>
    { using type = std::true_type; };

    template<typename F> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition(F*) noexcept
    {
        static_assert(std::is_function_v<F>, "HiHook must be a function.");

        constexpr bool is_stdcall = is_stdcall_function_helper_t<std::add_pointer_t<F>>::type::value;
        static_assert(is_stdcall, "HiHook function must have the __stdcall calling convention "
                                  "and have the first argument of type HiHook* or HiHook&");
    }

    template<typename C, typename R> NH3API_NO_SANITIZE_ADDRESS NH3API_FORCEINLINE static constexpr
    void hihook_function_condition(R C::*) noexcept
    { static_assert(false, "HiHook function must not be a member of some class"); }

    #endif // MSVC or MinGW GCC

    #if NH3API_HAS_BUILTIN_CONSTANT_P
    [[__gnu__::__error__("HiHooks do not support fastcall and thiscall calling conventions in SPLICE_ mode with DIRECT_, use EXTENDED_ or SAFE_ instead.")]]
    static void hihook_splice_error();
    #endif

    // protected
    virtual HiHook* __stdcall xWriteHiHook(uintptr_t address, EHiHookSetupPolicy hooktype, EHiHookType subtype, EHiHookCallingConvention calltype, const void* new_func) = 0;

public:
    // Method WriteHiHook
    // creates a high-level hook at address and applies it
    // returns pointer to the hook
    //
    // new_func - function replacing the original
    //
    // hooktype - hook type:
    // CALL_ - hook to the function call at address
    // the E8 and FF 15 opcodes are supported, otherwise the hook is not installed
    // and the information about this error is written to the log
    // SPLICE_ - hook directly to the FUNCTION itself at address
    // FUNCPTR_ - hook to the function in the pointer (rarely used, mostly for hooks in import tables)
    //
    // subtype - the subtype of the hook:
    // DIRECT_ - new_func has the same form as
    // original replaceable function
    // note: instead of __thiscall f (this)
    // you can use __fastcal f (this_)
    // instead of __thiscall f (this, ...) you can use
    // __fastcall f (this_, no_used_edx, ...)
    // EXTENDED_ - new_func function is passed the first stack argument
    // pointer to the HiHook instance and, in the case
    // conventions of the original __thiscall and __fastcall
    // register arguments are passed by stack second
    // So the function new_func should look like
    //? __stdcall new_func (HiHook * hook,?) For? ? Orig (?)
    //
    // ATTENTION! EXTENDED_ FASTCALL_ supports only functions with 2 or more arguments
    // for __fastcall with 1 argument, use EXTENDED_ FASTCALL_1 / EXTENDED_ THISCALL_
    //
    // SAFE_ is the same as EXTENDED_, but before calling (at the time of the call) GetDefaultFunc () is restored
    // The register values of the EAX, ECX(if not FASTCALL_ and not THISCALL_)
    // EDX (if not FASTCALL_), EBX, ESI, EDI, which were at the time of the call of the replaced function
    //
    //  In the vast majority of cases it is more convenient to use EXTENDED
    //  But DIRECT_ is executed faster because there is no bridge to the new replacement function
    //
    // Calltype - an agreement to call the original replacement f-tion:
    //  STDCALL_
    //  THISCALL_
    //  FASTCALL_
    //  CDECL_
    // need to specify the agreement correctly in order to EXTENDED_ hook correctly
    // built a bridge to a new replacement function
    //
    // CALL_, SPLICE_ hook is the CODE_ patch
    // FUNCPTR_ hook is a DATA_ patch
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод WriteHiHook
    // создает по адресу address высокоуровневый хук и применяет его
    // возвращает указатель на хук
    //
    // new_func - функция замещающая оригинальную
    //
    // hooktype - тип хука:
    //  CALL_ -  хук НА ВЫЗОВ функции по адресу address
    //     поддерживаются опкоды E8 и FF 15, в остальных случаях хук не устанавливается
    //     и в лог пишется информация об этой ошибке
    //  SPLICE_ - хук непосредственно НА САМУ ФУНКЦИЮ по адресу address
    //  FUNCPTR_ - хук на функцию в указателе (применяется редко, в основном для хуков в таблицах импорта)
    //
    // subtype - подтип хука:
    //  DIRECT_ - new_func имеет тот же вид что и
    //     оригинальная замещаемая функция
    //     примечание: вместо __thiscall f(this)
    //        можно использовать __fastcal f(this_)
    //        вместо __thiscall f(this, ...)  можно использовать
    //        __fastcall f(this_, no_used_edx, ...)
    //  EXTENDED_ - функции new_func первым стековым аргументом передается
    //     указатель на экземпляр HiHook и, в случае
    //     соглашений исходной ф-ии __thiscall и __fastcall
    //     регистровые аргументы передаются стековыми вторыми
    // Таким образом функция new_func должна иметь вид
    // ? __stdcall new_func(HiHook* hook, ?) для ? ? orig(?)
    //
    // ВНИМАНИЕ! EXTENDED_ FASTCALL_ поддерживает только функции с 2-мя и более аргументами
    // для __fastcall c 1 аргументом используйте EXTENDED_ FASTCALL1 / EXTENDED_ THISCALL_
    //
    //   SAFE_ - то же самое что и EXTENDED_, однако перед вызовом (на время вызова) GetDefaultFunc() восстанавливаются
    //    значения регистров процессора EAX, ECX (если не FASTCALL_ и не THISCALL_),
    //    EDX (если не FASTCALL_), EBX, ESI, EDI, бывшие на момент вызова замещенной функции
    //
    //  в подавляющем большинстве случаев удобнее использовать EXTENDED_
    //  но DIRECT_ выполняется быстрее из-за отсутвия моста к новой замещающей функции
    //
    // calltype - соглашение о вызове оригинальной замещаемой ф-ии:
    //  STDCALL_
    //  THISCALL_
    //  FASTCALL_
    //  CDECL_
    // необходимо верно указывать соглашение для того чтобы EXTENDED_ хук правильно
    // построил мост к новой замещающей функции
    //
    // CALL_, SPLICE_ хук является CODE_ патчем
    // FUNCPTR_ хук является DATA_ патчем
    //
    template<typename F> NH3API_NO_SANITIZE_ADDRESS
    HiHook* WriteHiHook(uintptr_t                address,
                        EHiHookSetupPolicy       hooktype,
                        EHiHookType              subtype,
                        EHiHookCallingConvention calltype,
                        F*                       new_func)
    {
        #if NH3API_HAS_BUILTIN_CONSTANT_P
        if ( __builtin_constant_p(hooktype) && __builtin_constant_p(subtype) && __builtin_constant_p(calltype) )
        {
            if ( hooktype == SPLICE_ && subtype == DIRECT_ && (calltype == THISCALL_ || calltype == FASTCALL_) )
                hihook_splice_error();
        }
        #endif
        hihook_function_condition(new_func);
        return xWriteHiHook(address, hooktype, subtype, calltype, static_cast<const void*>(new_func));
    }

    // Create Methods ...
    // create a patch / hook as well as the corresponding Write ... methods,
    // but do not use it
    // return pointer to patch / hook
    ///////////////////////////////////////////////////////////////////
    // Методы Create...
    // создают патч/хук так же как и соответствующие методы Write...,
    // но НЕ ПРИМЕНЯЮТ его
    // возвращают указатель на патч/хук.

    virtual Patch* __stdcall CreateBytePatch(uintptr_t address, int8_t value)      = 0;
    virtual Patch* __stdcall CreateWordPatch(uintptr_t address, int16_t value)      = 0;
    virtual Patch* __stdcall CreateDwordPatch(uintptr_t address, int32_t value)     = 0;
    virtual Patch* __stdcall CreateJmpPatch(uintptr_t address, uintptr_t to)        = 0;
    virtual Patch* __stdcall CreateHexPatch(uintptr_t address, const char* hex_str) = 0;

protected:
    virtual Patch* __stdcall CreateCodePatchVA(uintptr_t address, const char* __restrict format, uint32_t* __restrict va_args) = 0;
    virtual LoHook* __stdcall xCreateLoHook(uintptr_t address, lhfunc_t func) = 0;

public:
    [[deprecated("LoHooks are deprecated(unless you REALLY need to modify to esp). Use SafeLoHooks instead.")]] NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* CreateLoHook(uintptr_t address, lhfunc_t func)
    { return xCreateLoHook(address, func); }

    [[deprecated("LoHooks are deprecated(unless you REALLY need to modify to esp). Use SafeLoHooks instead.")]] NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* CreateLoHook(uintptr_t address, lhrfunc_t func)
    { return xCreateLoHook(address, reinterpret_cast<lhfunc_t>(func)); }

protected:
    virtual HiHook* __stdcall xCreateHiHook(uintptr_t address, EHiHookSetupPolicy hooktype, EHiHookType subtype, EHiHookCallingConvention calltype, const void* new_func) = 0;

public:
    template<typename F> NH3API_NO_SANITIZE_ADDRESS
    inline HiHook* CreateHiHook(uintptr_t                address,
                                EHiHookSetupPolicy       hooktype,
                                EHiHookType              subtype,
                                EHiHookCallingConvention calltype,
                                F*                       new_func)
    {
        #if NH3API_HAS_BUILTIN_CONSTANT_P
        if ( __builtin_constant_p(hooktype) && __builtin_constant_p(subtype) && __builtin_constant_p(calltype) )
        {
            if ( hooktype == SPLICE_ && subtype == DIRECT_ && (calltype == THISCALL_ || calltype == FASTCALL_) )
                hihook_splice_error();
        }
        #endif
        hihook_function_condition(new_func);
        return xCreateHiHook(address, hooktype, subtype, calltype, static_cast<const void*>(new_func));
    }

    // ApplyAll method
    // applies all patches / hooks created by this instance of PatcherInstance
    // always returns 1 (for compatibility with earlier versions of the library)
    // (see Patch::Apply)
    //////////////////////////////////////////////////////////////////////////////
    // Метод ApplyAll
    // применяет все патчи/хуки, созданные этим экземпляром PatcherInstance
    // всегда возвращает 1 (для совместимости с более ранними версиями библиотеки)
    // (см. Patch::Apply).
    virtual bool32_t __stdcall ApplyAll() = 0;

    // UndoAll Method
    // cancels all patches / hooks created by this instance of PatcherInstance
    // i.e. For each of the patches / hooks calls the Undo method
    // always returns 1 (for compatibility with earlier versions of the library)
    // (see Patch::Undo)
    ////////////////////////////////////////////////////////////
    // Метод UndoAll
    // отменяет все патчи/хуки, созданные этим экземпляром PatcherInstance
    // т.е. для каждого из патчей/хуков вызывает метод Undo
    // всегда возвращает 1 (для совместимости с более ранними версиями библиотеки)
    // (см. Patch::Undo).
    virtual bool32_t __stdcall UndoAll() = 0;

    // DestroyAll Method
    // cancels and permanently destroys all patches / hooks created by this instance of PatcherInstance
    // i.e. For each of the patches / hooks calls the Destroy method
    // always returns 1 (for compatibility with earlier versions of the library)
    // (see Patch::Destroy)
    ////////////////////////////////////////////////////////////
    // Метод DestroyAll
    // отменяет и безвозвратно уничтожает все патчи/хуки, созданные этим экземпляром PatcherInstance
    // т.е. для каждого из патчей/хуков вызывает метод Destroy
    // всегда возвращает 1 (для совместимости с более ранними версиями библиотеки)
    // (см. Patch::Destroy).
    virtual bool32_t __stdcall DestroyAll() = 0;

protected:
    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteDataPatch /
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteDataPatch
    virtual Patch* __stdcall WriteDataPatchVA(uintptr_t address, const char* __restrict format, const uint32_t* __restrict va_args) = 0;

    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteDataPatch /
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteDataPatch
    virtual Patch* __stdcall CreateDataPatchVA(uintptr_t address, const char* __restrict format, const uint32_t* __restrict va_args) = 0;

public:
    // GetLastPatchAt method
    // returns nullptr if no patch / hook has been applied in the vicinity of the address address,
    // created by this instance of PatcherInstance
    // otherwise returns the last applied patch / hook in the neighborhood of the address address,
    // created by this instance of PatcherInstance
    //////////////////////////////////////////////////////////////////////////////////////////
    // Метод GetLastPatchAt
    // возвращает nullptr, если в окрестности адреса address не был применен ни один патч/хук,
    // созданный данным экземпляром PatcherInstance
    // иначе возвращает последний примененый патч/хук в окрестности адреса address,
    // созданный данным экземпляром PatcherInstance.
    virtual Patch* __stdcall GetLastPatchAt(uintptr_t address) = 0;

    // UndoAllAt Method
    // cancels the patches applied by this instance of PatcherInstance
    // in the neighborhood of address
    // always returns 1 (for compatibility with earlier versions of the library)
    // (see Patch::Undo)
    // Метод UndoAllAt
    // отменяет патчи примененные данным экземпляром PatcherInstance
    // в окрестности адреса address
    // всегда возвращает 1 (для совместимости с более ранними версиями библиотеки)
    // (см. Patch::Undo).
    virtual bool32_t __stdcall UndoAllAt(uintptr_t address) = 0;

    // GetFirstPatchAt method
    // returns NULL if no patch / hook has been applied in the vicinity of the address address,
    // created by this instance of PatcherInstance
    // otherwise returns the first applied patch / hook in the neighborhood of the address address,
    // created by this instance of PatcherInstance
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Метод GetFirstPatchAt
    // возвращает nullptr, если в окрестности адреса address не был применен ни один патч/хук,
    // созданный данным экземпляром PatcherInstance
    // иначе возвращает первый примененый патч/хук в окрестности адреса address,
    // созданный данным экземпляром PatcherInstance
    virtual Patch* __stdcall GetFirstPatchAt(uintptr_t address) = 0;

    // Write Method
    // writes address data / code from memory to address data size size bytes
    // Returns the pointer to the patch
    ////////////////////////////////////////////////////////////
    // Метод Write
    // пишет по адресу address данные/код из памяти по адресу data размером size байт
    // Возвращает указатель на патч.
    virtual Patch* __stdcall Write(uintptr_t address, const void* data, size_t size, EPatcherInstanceWriteMode mode = DATA_) = 0;

    // CreatePatch method
    // creates a patch in the same manner Write method,
    // but does not apply it
    // return pointer to patch
    ///////////////////////////////////////////////////////////////////
    // Метод CreatePatch
    // создаёт патч так же как и метод Write,
    // но НЕ ПРИМЕНЯЕТ его
    // возвращают указатель на патч.
    virtual Patch* __stdcall CreatePatch(uintptr_t address, const void* data, size_t size, EPatcherInstanceWriteMode mode = DATA_) = 0;

    [[deprecated("LoHooks are deprecated(unless you REALLY need to modify to esp). Use SafeLoHooks instead.")]]
    // # ver 2.1, deprecated since version v5.0
    // Method WriteLoHookEx
    // is similar to the WriteLoHook method, but has an additional argument
    // stack_delta - the size of the memory that can be placed in the context stack
    // using HookContext::esp and HookContext::Push inside func.
    // Return a pointer to the LoHook hook
    ////////////////////////////////////////////////////////////
    // Метод WriteLoHookEx
    // аналогичен методу WriteLoHook, но имеет дополнительный аргумент
    // stack_delta - размер памяти который можно поместить в стэк контекста
    // используя HookContext::esp и HookContext::Push внутри func.
    // Возвращают указатель на LoHook хук.
    virtual LoHook* __stdcall WriteLoHookEx(uintptr_t address, const void* func, uint32_t stack_delta) = 0;

    [[deprecated("LoHooks are deprecated(unless you REALLY need to modify to esp). Use SafeLoHooks instead.")]]
    // # ver 2.1, deprecated since version v5.0
    // The CreateLoHookEx Method
    // creates a hook just like WriteLoHookEx,
    // but DO NOT apply it.
    // Return a pointer to the LoHook hook
    //////////////////////////////////////////
    // Метод CreateLoHookEx
    // создает хук так же как и WriteLoHookEx,
    // но НЕ ПРИМЕНЯЕТ его.
    // Возвращают указатель на LoHook хук.
    virtual LoHook* __stdcall CreateLoHookEx(uintptr_t address, const void* func, uint32_t stack_delta) = 0;

protected:
    // In the original form, the method is not supposed to be used,
    // see (below) the description of the WriteHexHook shell method /
    // В оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteHexHook.
    virtual LoHook* __stdcall WriteHexHookVA(uintptr_t address, bool32_t exec_default, const char* __restrict hex_str, uint32_t* __restrict va_args) = 0;

    // In the original form, the method is not supposed to be used,
    // see (below) the description of the CreateHexHook shell method /
    // В оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки CreateHexHook.
    virtual LoHook* __stdcall CreateHexHookVA(uintptr_t address, bool32_t exec_default, const char* __restrict hex_str, uint32_t* __restrict va_args) = 0;

public:
    // The BlockAt method sets the block to a specific address (to the specific address and not to the vicinity)
    // for this instance of PatcherInstance
    // after which this instance of PatcherInstance can not apply
    // patches at this address
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод BlockAt устанавливает блок на определенный адрес (именно на конкретный адрес а не на окрестность)
    // для данного экземпляра PatcherInstance
    // после чего данный экземпляр PatcherInstance не может применять
    // патчи по этому адресую
    virtual void __stdcall BlockAt(uintptr_t address) = 0;

protected:
    // # ver 2.6
    // the BlockAllExceptVA method sets the block to all addresses except those specified in va_args
    // as well as the BlockAt method operates with specific addresses and not neighborhoods
    // for this instance of PatcherInstance
    // after which this instance of PatcherInstance can not apply
    // patches for all addresses except those specified.
    // the list of addresses in va_args should end with 0 (zero)
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // метод BlockAllExceptVA устанавливает блок на все адреса кроме указанных в va_args
    // так же как и метод BlockAt оперирует конкретными адресами а не окрестностями
    // для данного экземпляра PatcherInstance
    // после чего данный экземпляр PatcherInstance не может применять
    // патчи по всем адресам кроме указанных.
    // список адресов в va_args должен заканчиваться 0 (нулем)
    virtual void __stdcall BlockAllExceptVA(uintptr_t* va_args) = 0;

    template <typename...Args>
    struct all_types_of_uintptr_t
        : std::bool_constant<(std::is_same_v<uintptr_t, Args> && ...)>
    {};

public:
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* BlockAllExcept(Args... args)
    {
        static_assert(all_types_of_uintptr_t<Args...>::value, "PatcherInstance::BlockAllExcept: arguments should be a list of uintptr_t-s");
        const uintptr_t va_args[] = { args... };
        return BlockAllExceptVA(&va_args[0]);
    }

    // # ver 4.0

    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteAsmPatch.
    virtual Patch* __stdcall WriteAsmPatchVA(uintptr_t address, uint32_t* va_args) = 0;

    // # ver 4.0
    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteAsmPatch /
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки CreateAsmPatch.
    virtual Patch* __stdcall CreateAsmPatchVA(uintptr_t address, uint32_t* va_args) = 0;

    // # ver 4.0
    // in the original form, the method is not supposed to be used,
    // see (below) the description of the CreateAsmPatch shell method /
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteAsmHook.
    virtual LoHook* __stdcall WriteAsmHookVA(uintptr_t address, uint32_t* va_args) = 0;

    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteAsmHook /
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки CreateAsmHook
    virtual LoHook* __stdcall CreateAsmHookVA(uintptr_t address, uint32_t* va_args) = 0;

public:
    // # ver 5.0
    // Applies thread-safe lohook, see CreateSafeLoHook() /
    // Применяет потокобезопасный лоухук, см. CreateSafeLoHook().
    virtual SafeLoHook* __stdcall WriteSafeLoHook(uintptr_t address, slhfunc_t func, uint32_t = 0) = 0;
    inline SafeLoHook* WriteSafeLoHook(uintptr_t address, slhrfunc_t func)
    { return WriteSafeLoHook(address, reinterpret_cast<slhfunc_t>(func)); }

    // # ver 5.0
    // Creates thread-safe lohook. The interface difference is:
    // 1. hook context has the reverse order, look SafeLoHookContext;
    // 2. esp register is read-only
    // 3. return_address is ignored if EXEC_DEFAULT is returned
    // This version makes no use of any locking mechanisms and actually performs better
    //////////////////////////////////////////////////////////////
    // Создает потокобезопасный лоухук. Разница со старым лоухуком:
    // 1. контекст имеет обратную структуру, см. SafeLoHookContext;
    // 2. регистр esp только для чтения;
    // 3. return_address игнорируется если лоухук вернул EXEC_DEFAULT
    // Эта версия лоухука не используют механизмы реализации потокобезопасности
    // и работает даже быстрее, чем старые лоухуки.
    virtual SafeLoHook* __stdcall CreateSafeLoHook(uintptr_t address, slhfunc_t func, uint32_t = 0) = 0;

    NH3API_NO_SANITIZE_ADDRESS inline SafeLoHook* CreateSafeLoHook(uintptr_t address, slhrfunc_t func)
    { return CreateSafeLoHook(address, reinterpret_cast<slhfunc_t>(func)); }

    // WriteAsmPatch writes a patch to the address address
    // returns pointer to patch
    // arguments ... are strings containing code in assembler
    // all the instructions recognized by OllyDbg 1.04 (up to and including MMX and amd 3DNow!) Are recognized
    // ATTENTION! Unlike OllyDbg, integers that do not have the prefix 0x or postfix h are read as decimal!
    // so do not forget to write the hex numbers explicitly
    // In one line there can be several instructions separated by a ';'
    // The assembler code can contain labels;
    // declaration of the label - label_name: (label name, colon),
    // use label_name (a name without a colon);
    // the maximum length of the label name is 39 characters, the name can contain letters, numbers, symbols '_' and '.';
    // the code can contain the pseudoinstruction times (writes the specified number of times the next instruction)
    // for example, the result "times 3 nop" will be the code 90 90 90
    // code can contain a pseudo-instruction _ExecCopy <address>, <size> (writes code from memory to <address> size <size>)
    // the code can contain pseudoinstructions db <number>, dw <number>, dd <number or label>.
    // The string can contain the character format% d. In this case, the line should be followed by the corresponding number of four-byte numbers (signed / unsigned / addresses / ...)
    // ATTENTION! The last argument (string) must be '\0'!
    // abstract example: WriteAsmPatch (0x112233, "begin: call% d", MyFunc, "jmp begin", "jne dword [% d]", 0xAABBCC, 0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // WriteAsmPatch пишет патч по адресу address
    // возвращаeт указатель на патч
    // аргументы ... - это строки, содержащие код на ассемблере
    // распознаются все инструкции, распознаваемые OllyDbg 1.04 (вплоть до MMX и amd 3DNow! включительно)
    // ВНИМАНИЕ! в отличие от OllyDbg, целые числа не имеющие префикса 0x или постфикса h читаются как ДЕСЯТЕРИЧНЫЕ!
    // поэтому не забывайте писать шеснадцатеричные числа явно
    // В одной строке может быть несколько инструкций, разделенных символом ';'
    // Код на ассемблере может содержать метки;
    // объявление метки - label_name: (имя метки, двоеточие),
    // использование label_name (имя без двоеточия);
    // максимальная длина имени метки - 39 символов, имя может содержать буквы, цифры, символы '_' и '.';
    // код может содержать псевдоинструкцию times (пишет заданное количество раз следующую инструкцию)
    // например, результатом "times 3 nop" будет код 90 90 90
    // код может содержать псевдоинструкцию _ExecCopy <адрес>, <размер> (пишет код из памяти по адресу <адрес> размером <размер>)
    // код может содержать псевдоинструкции db <число>, dw <число>, dd <число или метка>.
    // Строка может содержать формат-символы %d. В этом случае за строкой должно следовать соответствующее количество четырехбайтовых чисел (знаковые/беззнаковые/адреса/...)
    // ВНИМАНИЕ! последним аргументом (строкой) должен обязательно быть '\0'!
    // абстрактный пример: WriteAsmPatch(0x112233, "begin: call %d", MyFunc, "jmp begin", "jne dword [%d]", 0xAABBCC, '\0');
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* WriteAsmPatch(uintptr_t address, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteAsmPatchVA(address, &va_args[0]);
    }

    // CreateAsmPatch method
    // creates a patch as well as the WriteCodePatch method,
    // but does not apply it
    // returns pointer to patch
    //////////////////////////////////////////////////
    // Метод CreateAsmPatch
    // создает патч так же как и метод WriteCodePatch,
    // но не применяет его
    // возвращаeт указатель на патч.
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* CreateAsmPatch(uintptr_t address, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return CreateAsmPatchVA(address, &va_args[0]);
    }

    // WriteAsmHook writes to address address primitive hook
    // namely LoHook without calling a high-level function.
    // the body of the hook is written directly in the call to CreateHexHook or WriteHexHook
    // in the same way as the patch is written using WriteAsmPatch (see WriteAsmPatch)
    // Unlike the WriteAsmPatch method, the code here may or may not contain the _ExecDefault pseudo-command,
    // the code executes the code
    // ATTENTION! In the code there can not be more than one pseudo-command _ExecDefault
    // returns pointer to LoHook hook
    // ATTENTION! The last argument (string) must be '\0'!
    // abstract example: WriteAsmHook (0x112233, "cmp eax, 0; jne SkipDefault; _ExecDefault; jmp End; SkipDefault: mov ecx, 2; End: retn", '\0');
    ////////////////////////////////////////////////////////////
    // WriteAsmHook пишет по адресу address примитивный хук
    // а именно LoHook без вызова высокоуровневой функции.
    // тело хука пишется прямо в вызове CreateHexHook или WriteHexHook
    // таким же образом как пишется патч с помощью WriteAsmPatch (см. WriteAsmPatch)
    // В отличие метода WriteAsmPatch здесь код может содержать (а может не содержать) псевдокоманду _ExecDefault,
    // котроая выполняет затертый хуком код
    // ВНИМАНИЕ! в коде не может быть больше одной псевдокоманды _ExecDefault
    // возвращает указатель на LoHook хук
    // ВНИМАНИЕ! последним аргументом (строкой) должен обязательно быть '\0'!
    // абстрактный пример: WriteAsmHook(0x112233, "cmp eax, 0; jne SkipDefault; _ExecDefault; jmp End; SkipDefault: mov ecx, 2; End: retn", '\0');
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* WriteAsmHook(uintptr_t address, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteAsmHookVA(address, &va_args[0]);
    }

    // Method CreateAsmHook
    // creates a hook just like WriteAsmHook,
    // but DO NOT apply it.
    // Return a pointer to the LoHook hook
    ////////////////////////////////////////////////////////////
    // Метод CreateAsmHook
    // создает хук так же как и WriteAsmHook,
    // но НЕ ПРИМЕНЯЕТ его.
    // Возвращают указатель на LoHook хук.
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* CreateAsmHook(uintptr_t address, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return CreateAsmHookVA(address, &va_args[0]);
    }

    // WriteHexHook writes at address the most primitive hook
    // namely LoHook without calling a high-level function.
    // the body of the hook is written directly in the call to CreateHexHook or WriteHexHook
    // in the same way as the patch is written using WriteCodePatch (see WriteCodePatch)
    // exec_default - whether to execute the hooked code after executing the hook body
    // returns the pointer to the LoHook hook
    ////////////////////////////////////////////////////////////////////////////////////////
    // WriteHexHook пишет по адресу address самый примитивный хук
    // а именно LoHook без вызова высокоуровневой функции.
    // тело хука пишется прямо в вызове CreateHexHook или WriteHexHook
    // таким же образом как пишется патч с помощью WriteCodePatch (см. WriteCodePatch)
    // exec_default - выполнять ли затертый хуком код после выполнения тела хука
    // возвращает указатель на LoHook хук.
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* WriteHexHook(uintptr_t address, bool exec_default, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteHexHookVA(address, exec_default, format, &va_args[0]);
    }

    // CreateHexHook method
    // creates a hook as well as WriteHexHook,
    // but DO NOT apply it.
    // Return a pointer to the LoHook hook
    //////////////////////////////////////////
    // Метод CreateHexHook
    // создает хук так же как и WriteHexHook,
    // но НЕ ПРИМЕНЯЕТ его.
    // Возвращают указатель на LoHook хук.
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline LoHook* CreateHexHook(uintptr_t address, bool exec_default, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return CreateHexHookVA(address, exec_default, format, &va_args[0]);
    }

    // Method WriteCodePatch
    // writes to the address address the byte sequence,
    // defined format and ...
    // (creates and applies a CODE_ patch)
    // Returns the pointer to the patch
    // format - c-string can contain hexadecimal digits
    // 0123456789ABCDEF (uppercase only!),
    // as well as special format-characters (lower case!):
    // %b - (byte) writes a single-byte number from ...
    // %w - (word) writes a two-byte number from ...
    // %d - (dword) writes a four-byte number from ...
    // %j - writes jmp to the address from ...
    // %c - writes сall ...
    // %m - copies the code to the address ... the size ... (ie reads 2 arguments from ...)
    // copying occurs via MemCopyCodeEx (see description)
    // %% - writes a string with format characters from ...
    // %o - (offset) places the address offset in the argument from the argument
    // Complex code, relative to the beginning of the Complex code.
    // %n - write nop opcode, number equal to ...
    // #0: - #9: - set the label (from 0 to 9) to which you can go with # 0 - # 9
    // #0 - #9 - write the address after the opcodes EB, 70 - 7F, E8, E9, 0F80 - 0F8F
    // corresponding label; After other opcodes nothing writes
    // ~b - takes from ... the absolute address and writes the relative offset before it
    // 1 byte in size (used for EB, 70 - 7F opcodes)
    // ~d - takes from ... the absolute address and writes the relative offset before it
    // 4 bytes in size (used for the E8, E9, 0F 80 - 0F 8F opcodes)
    // %. - does nothing (like any other character not declared above after%)
    // abstract example:
    // Patch * p = pi-> WriteCodePatch (address,
    // "#0: %%",
    // "B9 %d %%", this, // mov ecx, this //
    // "BA %d %%", this-> context, // mov edx, context //
    // "%c %%", func, // call func //
    // "83 F8 01 %%", // cmp eax, 1
    // "0F 85 #7 %%", // jne long to label 7 (if func returns 0)
    // "83 F8 02 %%", // cmp eax, 2
    // "0F 85 ~d %%", 0x445544, // jne long to 0x445544 (if func returns 0)
    // "EB #0 %%", // jmp short to label 0
    // "%m %%", address2, size, // exec code copy from address2
    // "#7: FF 25 %d %.", & Return_address); // jmp [& return_address]
    ////////////////////////////////////////////////////////////
    // Метод WriteCodePatch
    // пишет по адресу address позледовательность байт,
    // определенную format и ...
    // (создает и применяет CODE_ патч)
    // Возвращает указатель на патч
    // format - си-строка может содержать шестнадцатеричные цифры
    // 0123456789ABCDEF (только верхний регистр!),
    // а так же специальные формат-символы (нижний регистр!):
    // %b - (byte) пишет однобайтовое число из ...
    // %w - (word) пишет двухбайтовое число из ...
    // %d - (dword) пишет четырехбайтовое число из ...
    // %j - пишет jmp на адрес из ...
    // %с - пишет сall ...
    // %m - копирует код по адресу ... размером ... (т.е. читает 2 аргумента из ...)
    //      копирование происходит посредством MemCopyCodeEx (см. описание)
    // %% - пишет строку с формат-символами из ...
    // %o - (offset) помещает по адресу из аргумента смещение позиции в
    //      Complex коде,  относительно начала Complex кода.
    // %n - пишет nop опкоды, количеством равным ...
    // #0: - #9: -устанавливает метку (от 0 до 9) к которой можно перейти с помощью #0 - #9
    // #0 -  #9  -пишет отностельный адрес после опкодов EB, 70 - 7F, E8, E9, 0F80 - 0F8F
    //      соответствующей метки; после других опкодов ничего не пишет
    // ~b - берет из ... абсолютный адрес и пишет относительное смещение до него
    //      размером в 1 байт (используется для опкодов EB, 70 - 7F)
    // ~d - берет из ... абсолютный адрес и пишет относительное смещение до него
    //      размером в 4 байта (используется для опкодов E8, E9, 0F 80 - 0F 8F)
    // %. - ничего не делает ( как и любой другой не объявленный выше символ после % )
    // абстрактный пример:
    // Patch* p = pi->WriteCodePatch(address,
    //  "#0: %%",
    //  "B9 %d %%", this,     // mov ecx, this  //
    //  "BA %d %%", this->context,   // mov edx, context  //
    //  "%c %%", func,      // call func  //
    //  "83 F8 01 %%",      // cmp eax, 1
    //  "0F 85 #7 %%",       // jne long to label 7 (if func returns 0)
    //  "83 F8 02 %%",      // cmp eax, 2
    //  "0F 85 ~d %%", 0x445544,   // jne long to 0x445544 (if func returns 0)
    //  "EB #0 %%",       // jmp short to label 0
    //  "%m %%", address2, size,   // exec  code copy from address2
    //  "#7: FF 25 %d %.", &return_address); // jmp [&return_address].
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* WriteCodePatch(uintptr_t address, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteCodePatchVA(address, format, &va_args[0]);
    }

    // The CreateCodePatch method
    // creates a patch as well as the WriteCodePatch method,
    // but does not apply it
    // returns pointer to patch
    //////////////////////////////////////////////////
    // Метод CreateCodePatch
    // создает патч так же как и метод WriteCodePatch,
    // но не применяет его
    // возвращаeт указатель на патч
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* CreateCodePatch(uintptr_t address, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return CreateCodePatchVA(address, format, &va_args[0]);
    }

    // WriteDataPatch Method
    // writes to the address address the byte sequence,
    // defined format and ...
    // (creates and applies DATA_ patch)
    // Returns the pointer to the patch
    // format - a c-string can contain hexadecimal digits
    // 0123456789ABCDEF (uppercase only!),
    // as well as special format-characters (lower case!):
    // %b - (byte) writes a single-byte number from ...
    // %w - (word) writes a two-byte number from ...
    // %d - (dword) writes a four-byte number from ...
    // %m - copies data to an address ... size ... (ie reads 2 arguments from ...)
    // %% - writes a string with format characters from ...
    // %o - (offset) places the address offset in the argument from the argument
    // Complex code, relative to the beginning of the Complex code.
    // %. - does nothing (like any other character not declared above after%)
    // abstract example:
    // Patch * p = pi-> WriteDataPatch (address,
    // "FF FF FF %d %%", var,
    // "%m %%", address2, size,
    // "AE %.");
    ////////////////////////////////////////////////////////////
    // Метод WriteDataPatch
    // пишет по адресу address позледовательность байт,
    // определенную format и ...
    // (создает и применяет DATA_ патч)
    // Возвращает указатель на патч
    // format - си-строка может содержать шестнадцатеричные цифры
    // 0123456789ABCDEF (только верхний регистр!),
    // а так же специальные формат-символы (нижний регистр!):
    // %b - (byte) пишет однобайтовое число из ...
    // %w - (word) пишет двухбайтовое число из ...
    // %d - (dword) пишет четырехбайтовое число из ...
    // %m - копирует данные по адресу ... размером ... (т.е. читает 2 аргумента из ...)
    // %% - пишет строку с формат-символами из ...
    // %o - (offset) помещает по адресу из аргумента смещение позиции в
    //      Complex коде,  относительно начала Complex кода.
    // %. - ничего не делает ( как и любой другой не объявленный выше символ после % )
    // абстрактный пример:
    // Patch* p = pi->WriteDataPatch(address,
    //  "FF FF FF %d %%", var,
    //  "%m %%", address2, size,
    //  "AE %.");
    template<typename ...Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* WriteDataPatch(uintptr_t address, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteDataPatchVA(address, format, &va_args[0]);
    }

    // CreateDataPatch method
    // creates the patch as well as the WriteDataPatch method,
    // but does not apply it
    // returns pointer to patch
    //////////////////////////////////////////////////////////
    // Метод CreateDataPatch
    // создает патч так же как и метод WriteDataPatch,
    // но не применяет его
    // возвращаeт указатель на патч
    template<typename... Args> NH3API_NO_SANITIZE_ADDRESS
    inline Patch* CreateDataPatch(uintptr_t address, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return CreateDataPatchVA(address, format, &va_args[0]);
    }
};

// class Patcher /
// Класс Patcher.
NH3API_VIRTUAL_CLASS Patcher
{
public:
    Patcher()                          = delete;
    Patcher(const Patcher&)            = delete;
    Patcher(Patcher&&)                 = delete;
    Patcher& operator=(const Patcher&) = delete;
    Patcher& operator=(Patcher&&)      = delete;
    ~Patcher()                         = delete;

    // CreateInstance method
    // Creates an instance of the PatcherInstance class, which
    // directly allows you to create patches and hooks and
    // returns a pointer to this instance.
    // owner - the unique name of the PatcherInstance instance
    // the method returns nullptr if an instance with the name owner is already created
    // if owner == nullptr or owner == "" then
    // the PatcherInstance instance nullptr be created with the module name from
    // the function was called.
    ///////////////////////////////////////////////////////////////////////////////////
    // Метод CreateInstance
    // создает экземпляр класса PatcherInstance, который
    // непосредственно позволяет создавать патчи и хуки и
    // возвращает указатель на этот экземпляр.
    // owner - уникальное имя экземпляра PatcherInstance
    // метод возвращает nullptr, если экземпляр с именем owner уже создан
    // если owner == nullptr или owner == "" то
    // экземпляр PatcherInstance будет создан с именем модуля из
    // которого была вызвана функция.
    virtual PatcherInstance* __stdcall CreateInstance(const char* owner) = 0;

protected:
    virtual PatcherInstance* __stdcall xGetInstance(const char* owner) = 0;

public:
    // GetInstance method
    // Returns a pointer to an instance of PatcherInstance
    // with the name owner.
    // the method returns NULL if
    // the instance named owner does not exist (was not created)
    // the module name can be passed as an argument.
    // Is used for :
    // - check if some mod is active, using patcher_x86.dll
    // - get access to all patches and hooks of some mod,
    // using patcher_x86.dll
    ///////////////////////////////////////////////////
    // Метод GetInstance
    // Возвращает указатель на экземпляр PatcherInstance
    // с именем owner.
    // метод возвращает nullptr в случае, если
    // экземпляр с именем owner не существует (не был создан)
    // в качестве аргумента можно передавать имя модуля.
    // Используется для :
    // - проверки активен ли некоторый мод, использующий patcher_x86.dll
    // - получения доступа ко всем патчам и хукам некоторого мода,
    //   использующего patcher_x86.dll
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] PatcherInstance* GetInstance(const char* owner)
    { return xGetInstance(owner); }

    // GetInstance method
    // Returns a pointer to an instance of PatcherInstance
    // with the name owner.
    // the method returns NULL if
    // the instance named owner does not exist (was not created)
    // the module name can be passed as an argument.
    // Is used for :
    // - check if some mod is active, using patcher_x86.dll
    // - get access to all patches and hooks of some mod,
    // using patcher_x86.dll
    ///////////////////////////////////////////////////
    // Метод GetInstance
    // Возвращает указатель на экземпляр PatcherInstance
    // с именем owner.
    // метод возвращает nullptr в случае, если
    // экземпляр с именем owner не существует (не был создан)
    // в качестве аргумента можно передавать имя модуля.
    // Используется для :
    // - проверки активен ли некоторый мод, использующий patcher_x86.dll
    // - получения доступа ко всем патчам и хукам некоторого мода,
    //   использующего patcher_x86.dll
    NH3API_NO_SANITIZE_ADDRESS [[nodiscard]] const PatcherInstance* GetInstance(const char* owner) const
    { return const_cast<Patcher*>(this)->xGetInstance(owner); }

    // GetLastPatchAt method
    // returns nullptr if no patch / hook is applied in the vicinity of the address address
    // otherwise returns the last applied patch / hook in the neighborhood of address
    // consistently walk through all the patches in a given neighborhood
    // using this method and Patch::GetAppliedBefore
    ///////////////////////////////////////////////////
    // Метод GetLastPatchAt
    // возвращает nullptr, если в окрестности адреса address не был применен ни один патч/хук
    // иначе возвращает последний примененый патч/хук в окрестности адреса address
    // последовательно пройтись по всем патчам в заданной окрестности
    // используя этот метод и Patch::GetAppliedBefore
    virtual Patch* __stdcall GetLastPatchAt(uintptr_t address) = 0;

    // UndoAllAt Method
    // cancels all patches / hooks in the neighborhood of address
    // always returns 1 (for compatibility with earlier versions of the library)
    ///////////////////////////////////////////////////
    // Метод UndoAllAt
    // отменяет все патчи/хуки в окрестности адреса address
    // всегда возвращает 1 (для совместимости с более ранними версиями библиотеки).
    virtual Patch* __stdcall UndoAllAt(uintptr_t address) = 0;

    // SaveDump Method
    // saves to file named file_name
    // - number and names of all instances of PatcherInstance
    // - the number of all applied patches / hooks
    // - list of all applied patches and hooks
    // with their installation addresses, sizes, global order of use, proprietors (PatcherInstance names)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод SaveDump
    // сохраняет в файл с именем file_name
    // - количество и имена всех экземпляров PatcherInstance
    // - количество всех примененных патчей/хуков
    // - список всех примененных патчей и хуков
    // с их адресами установки, размерами, глобальной очередностью применения, собственниками (именами PatcherInstance)
    virtual void __stdcall SaveDump(const char* file_name) = 0;

    // SaveLog method
    // saves to the file named file_name a log
    // if logging is disabled in the log, there will be 0 entries.
    // enable logging by creating in the library directory
    // the text file patcher_x86.ini with the contents: Logging = 1
    ///////////////////////////////////////////////////////////////
    // Метод SaveLog
    // сохраняет в файл с именем file_name лог
    // если логирование выключено в логе будет 0 записей.
    // включить логирование можно создав в директории библиотеки
    // текстовый файл patcher_x86.ini c cодержимым: Logging = 1
    virtual void __stdcall SaveLog(const char* file_name) = 0;

    // GetMaxPatchSize Method
    // The patcher_x86.dll library imposes some restrictions
    // to the maximum size of the patch,
    // which one can be recognized using the GetMaxPatchSize method
    // (at the moment this is 262144 bytes, which is enough)
    ///////////////////////////////////////////////////////////////
    // Метод GetMaxPatchSize
    // Библиотека patcher_x86.dll накладывает некоторые ограничения
    // на максимальный размер патча,
    // какой - можно узнать с помощью метода GetMaxPatchSize
    // (на данный момент это 262144 байт, чего вполне достаточно).
    [[nodiscard]] virtual int32_t __stdcall GetMaxPatchSize() const = 0;

protected:
    // Method WriteComplexDataVA
    // in the original form, the method is not supposed to be used,
    // see (below) the description of the wrapper method WriteComplexString
    ///////////////////////////////////////////////////////////////////////
    // Метод WriteComplexDataVA
    // в оригинальном виде применение метода не предполагается,
    // смотрите (ниже) описание метода-оболочки WriteComplexString.
    virtual int32_t __stdcall WriteComplexDataVA(uintptr_t address, const char* __restrict format, uint32_t* __restrict args) = 0;

public:
    // GetOpcodeLength method
    // A so-called Disassembler of the lengths of the opcodes
    // returns the length in bytes of the opcode at address
    // returns 0 if opcode is unknown
    ///////////////////////////////////////////////////
    // GetOpcodeLength метод
    // т.н. дизассемблер длин опкодов
    // возвращает длину в байтах опкода по адресу address
    // возвращает 0, если опкод неизвестен.
    [[nodiscard]] virtual int32_t __stdcall GetOpcodeLength(uintptr_t address) const = 0;

    // Method MemCopyCode
    // copies the code from memory to src address in memory at dst
    // MemCopyCode always copies an integer number of opcodes with the size> = size. Be careful!
    // returns the size of the copied code.
    // differs by the action from a simple copy of the memory,
    // that correctly copies the ops E8 (call), E9 (jmp long), 0F80 - 0F8F (j ** long)
    // c relative addressing without knocking down addresses in them, if instructions
    // Forward outside the copied blocking
    ///////////////////////////////////////////////////
    // Метод MemCopyCode
    // копирует код из памяти по адресу src в память по адресу dst
    // MemCopyCode копирует всегда целое количество опкодов размером >= size. Будьте внимательны!
    // возвращает размер скопированного кода.
    // отличается действием от простого копирования памяти тем,
    // что корректно копирует опкоды E8 (call), E9 (jmp long), 0F80 - 0F8F (j** long)
    // c относительной адресацией не сбивая в них адреса, если инструкции
    // направляют за пределы копируемого блокая.
    virtual int32_t __stdcall MemCopyCode(uintptr_t dst, uintptr_t src, size_t size) = 0;

    // GetFirstPatchAt method
    // returns nullptr if no patch / hook is applied in the vicinity of the <address>
    // otherwise returns the first applied patch / hook in the vicinity of <address>
    // consistently walk through all the patches in a given vicinity
    // using this method and Patch::GetAppliedAfter
    /////////////////////////////////////////////////////////////////////////////////////////
    // Метод GetFirstPatchAt
    // возвращает nullptr, если в окрестности адреса address не был применен ни один патч/хук
    // иначе возвращает первый примененый патч/хук в окрестности адреса <address>
    // последовательно пройтись по всем патчам в заданной окрестности
    // используя этот метод и Patch::GetAppliedAfter.
    [[nodiscard]] virtual Patch* __stdcall GetFirstPatchAt(uintptr_t address) = 0;

    // MemCopyCodeEx method
    // copies the code from memory to src address in memory at dst
    // returns the size of the copied code.
    // differs from MemCopyCode in that,
    // that correctly copies the opcodes EB (jmp short), 70 - 7F (j ** short)
    // c relative addressing without knocking down addresses in them, if instructions
    // send outside the copied block (in this case they are replaced by
    // corresponding to E9 (jmp long), 0F80 - 0F8F (j ** long) opcodes.
    // Attention! Because of this, the size of the copied code can be significantly
    // more than copied.
    /////////////////////////////////////////////////////////////////////////////////
    // Метод MemCopyCodeEx
    // копирует код из памяти по адресу src в память по адресу dst
    // возвращает размер скопированного кода.
    // отличается от MemCopyCode тем,
    // что корректно копирует опкоды EB (jmp short), 70 - 7F (j** short)
    // c относительной адресацией не сбивая в них адреса, если инструкции
    // направляют за пределы копируемого блокая (в этом случае они заменяются на
    // соответствующие E9 (jmp long), 0F80 - 0F8F (j** long) опкоды.
    // Внимание! Из-за этого размер скопированного кода может оказаться значительно
    // больше копируемого.
    virtual int32_t __stdcall MemCopyCodeEx(uintptr_t dst, uintptr_t src, size_t size) = 0;

protected:
    virtual Variable* __stdcall xVarInit(const char* __restrict name, uint32_t value) = 0;
    virtual Variable* __stdcall xVarFind(const char* name) = 0;

public:
    // # ver 2.3
    // VarInit Method
    // initializes a "variable" named name and sets the value of "variable" to value
    // if a "variable" with this name already exists, then simply sets its value to value
    // returns a pointer to the "variable" if successful and nullptr otherwise /
    // Метод VarInit
    // инициализирует "переменную" c именем name и устанавливает значение "переменной" равным value
    // если "переменная" с таким именем уже существует, то просто устанавливает ее значение равным value
    // возвращает указатель на "переменную" в случае успеха и nullptr в противном случае.
    template<typename ValueType> NH3API_NO_SANITIZE_ADDRESS
    inline Variable* __stdcall VarInit(const char* __restrict name, ValueType value)
#ifdef __cpp_concepts
    requires ( ::std::is_trivially_default_constructible_v<ValueType> && ::std::is_trivially_copyable_v<ValueType> && sizeof(ValueType) <= 4 )
#endif
    {
        #ifndef __cpp_concepts
        static_assert(::std::is_trivially_default_constructible_v<ValueType> && ::std::is_trivially_copyable_v<ValueType> && sizeof(ValueType) <= 4, "ValueType must be trivial");
        #endif

        if constexpr ( std::is_integral_v<ValueType> || std::is_enum_v<ValueType> )
            return xVarInit(name, static_cast<uint32_t>(value));
        else if constexpr ( std::is_pointer_v<ValueType> )
            return xVarInit(name, reinterpret_cast<uint32_t>(value));
        else
            return xVarInit(name, nh3api::bit_cast<uint32_t>(value));
    }

    // # ver 2.3
    // VarFind method
    // returns a pointer to a "variable" named name, if such was initialized
    // if not, it returns nullptr
    ////////////////////////////////////////////////////////////////////////////////////////
    // Метод VarFind
    // возвращает указатель на "переменную" с именем name, если такая была инициализированна
    // если нет, возвращает nullptr.
    NH3API_NO_SANITIZE_ADDRESS const Variable* VarFind(const char* name) const
    { return const_cast<Patcher*>(this)->xVarFind(name); }

    // # ver 2.3
    // VarFind method
    // returns a pointer to a "variable" named name, if such was initialized
    // if not, it returns nullptr
    ////////////////////////////////////////////////////////////////////////////////////////
    // Метод VarFind
    // возвращает указатель на "переменную" с именем name, если такая была инициализированна
    // если нет, возвращает nullptr.
    NH3API_NO_SANITIZE_ADDRESS Variable* VarFind(const char* name)
    { return xVarFind(name); }

    // # ver 2.6
    // Method PreCreateInstance
    // Creates a defective PatcherInstance with the specified name.
    // PatcherInstance created in this way can not create patches.
    // This incomplete instance is used to apply the PatcherInstance::BlockAt and PatcherInstance::BlockAllExceptVA methods
    // so that you can block addresses before this PatcherInstance is fully created using CreateInstance
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Метод PreCreateInstance
    // Создает неполноценный экземпляр PatcherInstance с указанным именем.
    // PatcherInstance созданный таким образом не может создавать патчи.
    // Этот неполноценный экземпляр используется для применения методов PatcherInstance::BlockAt и PatcherInstance::BlockAllExceptVA
    // чтобы можно было заблокировать адреса до того как данный PatcherInstance будет полноценно создан с помощью CreateInstance
    virtual PatcherInstance* __stdcall PreCreateInstance(const char* name) = 0;

    // # ver 4.1

    virtual int32_t __stdcall WriteAsmCodeVA(uintptr_t address, uint32_t* args) = 0;
    virtual uintptr_t __stdcall CreateCodeBlockVA(uint32_t* args) = 0;

    // The VarGetValue method returns the value of a "variable" named name
    // if the "variable" with this name was not initialized, it returns default_value /
    // Метод VarGetValue возвращает значение "переменной" c именем name
    // если "переменная" с таким именем не была инициализированна, возвращает default_value.
    template<typename ValueType> NH3API_NO_SANITIZE_ADDRESS
    inline ValueType VarGetValue(const char* __restrict name, NH3API_LIFETIMEBOUND const ValueType default_value = ValueType{}) const
    {
        if constexpr ( sizeof(ValueType) > 4 )
            return default_value;
        const Variable* const v = VarFind(name);
        if ( v == nullptr )
            return default_value;

        if constexpr ( std::is_integral_v<ValueType> || std::is_enum_v<ValueType> )
            return static_cast<ValueType>(v->GetValue());
        else if constexpr ( std::is_pointer_v<ValueType> )
            return reinterpret_cast<ValueType>(v->GetValue());
        else
            return nh3api::bit_cast<ValueType>(v->GetValue());
    }

    // The VarValue method returns a reference to the value of the "variable" named name
    // if the "variable" with this name was not initialized, initializes it and sets the value to 0
    // attention, accessing the value of a variable by reference is not thread-safe /
    // Метод VarValue возвращает ссылку на значение "переменной" c именем name
    // если "переменная" с таким именем не была инициализированна, инициализирует ее и устанавливает значение равным 0
    // внимание, обращение к значению переменной по ссылке непотокобезопасно.
    template<typename ValueType> NH3API_NO_SANITIZE_ADDRESS
    inline ValueType& VarValue(const char* name)
    {
        static_assert(sizeof(ValueType) <= 4, "Patcher_x86's variables cannot be larger than 4 bytes.");
        Variable* v = VarFind(name);
        if ( v == nullptr )
            v = VarInit(name, 0);

        if ( v == nullptr )
            __debugbreak();

        return *reinterpret_cast<ValueType*>(v->GetPValue());
    }

    // method WriteComplexData
    // is a more convenient interface
    // method WriteComplexDataVA
    // this method is defined here and not in the library, because Its appearance
    // differs in C and Delphi
    // The method's functionality is almost the same as that of PatcherInstance::WriteCodePatch
    // (see the description of this method)
    // that is, the method writes to the address address, the sequence of bytes,
    // defined by the arguments format and ...,
    // BUT! DOES NOT create an instance of the Patch class, with all that is implied (i.e., not allowing to undo, access to edit from another mode, etc.)
    // ATTENTION!
    // Use this method only for dynamically creating blocks
    // code, i.e. Write this method only in your memory,
    // a in the code of the program to be modified only with the help of
    // PatcherInstance::WriteCodePatch
    ///////////////////////////////////////////////////////////////////////////////////////
    // метод WriteComplexData
    // является более удобным интерфейсом
    // метода WriteComplexDataVA
    // этот метод определен здесь а не в библиотеке, т.к. его вид
    // отличается в Си и Делфи
    // Функционал метода почти тот же что и у PatcherInstance::WriteCodePatch
    // (см. описание этого метода)
    // то есть метод пишет по адресу address, последовательность байт,
    // определенную аргументами format и ...,
    // НО! НЕ создает экземпляр класса Patch, со всеми вытекающими
    // (т.е. не позволяя отменить правку, получить доступ к правке из другого мода и т.д.)
    // ВНИМАНИЕ!
    // Используйте этот метод только для динамического создания блоков
    // кода, т.е. пишите этим методом только в свою память,
    // а в код модифицируемой программы только с помощью
    // PatcherInstance::WriteCodePatch.
    template<typename... Args> NH3API_NO_SANITIZE_ADDRESS
    inline int32_t WriteComplexData(uintptr_t address, const char* __restrict format, Args... args)
    {
        const uint32_t va_args[] = { nh3api::cast<uint32_t>(args)... };
        return WriteComplexDataVA(address, format, &va_args[0]);
    }

    NH3API_NO_SANITIZE_ADDRESS inline HiHook* GetFirstHiHookAt(uintptr_t address)
    {
        Patch* p = GetFirstPatchAt(address);
        while ( true )
        {
            if ( p == nullptr )
                return nullptr;
            if ( p->GetType() == HIHOOK_ )
                return static_cast<HiHook*>(p);
            p = p->GetAppliedAfter();
        }
    }

    NH3API_NO_SANITIZE_ADDRESS inline HiHook* GetLastHiHookAt(uintptr_t address)
    {
        Patch* p = GetLastPatchAt(address);
        while ( true )
        {
            if ( p == nullptr )
                return nullptr;
            if ( p->GetType() == HIHOOK_ )
                return static_cast<HiHook*>(p);
            p = p->GetAppliedBefore();
        }
    }

};

// restore the alignment of members of structures and classes /
// восстанавливаем выравнивание членов структур и классов.
#pragma pack(pop)

//////////////////////////////////////////////////////////////////
NH3API_NO_SANITIZE_ADDRESS inline HMODULE GetPatcherHandle() noexcept
{ return LoadLibraryW(L"patcher_x86.dll"); }

// Function GetPatcher
// loads the library and, by calling the only exported
// function _GetPatcherX86@0, returns a pointer to the Patcher object,
// through which the entire functionality of the library patcher_x86.dll is available
// return NULL on failure
// function is called only once, which is obvious from its definition
/////////////////////////////////////////////////////////////////////////////////////
// Функция GetPatcher
// загружает библиотеку и, с помощью вызова единственной экспортируемой
// функции _GetPatcherX86@0, возвращает указатель на объект Patcher,
// посредством которого доступен весь функционал библиотеки patcher_x86.dll
// возвращает nullptr при неудаче
// функцию вызывать 1 раз, что очевидно из ее определения
NH3API_NO_SANITIZE_ADDRESS inline Patcher* GetPatcher() noexcept
{
    using GetPatcher_f    = Patcher*(__stdcall*)();
    HMODULE patcherHandle = GetPatcherHandle();
    if ( patcherHandle )
    {
        GetPatcher_f func = reinterpret_cast<GetPatcher_f>(GetProcAddress(patcherHandle, "_GetPatcherX86@0"));
        if ( func )
            return func();

        return nullptr;
    }

    return nullptr;
}

// Функция GetPatcherVersion возвращает версию патчера /
// GetPatcherVersion function returns the version of patcher_x86.
NH3API_NO_SANITIZE_ADDRESS inline uint32_t GetPatcherVersion() noexcept
{
    using GetPatcherX86Version_f = uint32_t(__stdcall*)();
    HMODULE patcherHandle        = GetPatcherHandle();
    if ( patcherHandle )
    {
        GetPatcherX86Version_f func = reinterpret_cast<GetPatcherX86Version_f>(GetProcAddress(patcherHandle, "_GetPatcherX86Version@0"));
        if ( func )
            return func();

        return 0;
    }
    return 0;
}

NH3API_WARNING(pop)
