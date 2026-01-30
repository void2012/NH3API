//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */

#pragma once

#include <cstdio>         // EOF

#include "memory.hpp"     // get_type_vftable
#include "exe_string.hpp" // exe_string

NH3API_WARNING(push)
NH3API_WARNING_MSVC_DISABLE(26495)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")

#pragma pack(push, 4)
extern "C"
{
// size = 0x20 = 32, align = 4
struct exe_FILE
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    char* _ptr;

    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t _cnt;

    // offset: +0x8 = +8,  size = 0x4 = 4
    char* _base;

    // offset: +0xC = +12,  size = 0x4 = 4
    int32_t _flag;

    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t _file;

    // offset: +0x14 = +20,  size = 0x4 = 4
    int32_t _charbuf;

    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t _bufsiz;

    // offset: +0x1C = +28,  size = 0x4 = 4
    char* _tmpfname;
};
} // extern "C"

// address: 0x618A1A
// Heroes3.exe internal thread-safe fwrite /
// внутренний fwrite Heroes3.exe (thread-safe).
inline size_t exe_fwrite(const void* __restrict buffer, size_t size, size_t count, exe_FILE* __restrict file) noexcept
{ return CDECL_4(size_t, 0x618A1A, buffer, size, count, file); }

// address: 0x618B53
// Heroes3.exe internal thread-safe fputc /
// внутренний fputc Heroes3.exe (thread-safe).
inline int32_t exe_fputc(int32_t character, exe_FILE* file) noexcept
{ return CDECL_2(int32_t, 0x618B53, character, file); }

// address: 0x618B8E
// Heroes3.exe internal thread-safe fgetc /
// внутренний fgetc Heroes3.exe (thread-safe).
inline int32_t exe_fgetc(exe_FILE* file) noexcept
{ return CDECL_1(int32_t, 0x618B8E, file); }

// address: 0x618BBF
// Heroes3.exe internal thread-safe ungetc /
// внутренний ungetc Heroes3.exe (thread-safe).
inline int32_t exe_ungetc(int32_t character, exe_FILE* file) noexcept
{ return CDECL_2(int32_t, 0x618BBF, character, file); }

// address: 0x618C56
// Heroes3.exe internal thread-safe fgetpos /
// внутренний fgetpos Heroes3.exe (thread-safe).
inline int32_t exe_fgetpos(exe_FILE* __restrict in_file, fpos_t* __restrict out_pos) noexcept
{ return CDECL_2(int32_t, 0x618C56, in_file, out_pos); }

// address: 0x618C78
// Heroes3.exe internal thread-safe fseek /
// внутренний fseek Heroes3.exe (thread-safe).
inline int32_t exe_fseek(exe_FILE* file, ptrdiff_t offset, ptrdiff_t origin) noexcept
{ return CDECL_3(int32_t, 0x618C78, file, offset, origin); }

// address: 0x618D31
// Heroes3.exe internal thread-safe fsetpos /
// внутренний fsetpos Heroes3.exe (thread-safe).
inline int32_t exe_fsetpos(exe_FILE* __restrict in_file, fpos_t* __restrict in_pos) noexcept
{ return CDECL_2(int32_t, 0x618D31, in_file, in_pos); }

// address: 0x618D49
// Heroes3.exe internal thread-safe setvbuf /
// внутренний setvbuf Heroes3.exe (thread-safe).
inline int32_t exe_setvbuf(exe_FILE* __restrict file, char* __restrict buffer, int32_t mode, size_t size) noexcept
{ return CDECL_4(int32_t, 0x618D49, file, buffer, mode, size); }

// address: 0x618DF8
// Heroes3.exe internal thread-safe fflush /
// внутренний fflush Heroes3.exe (thread-safe).
inline int32_t exe_fflush(exe_FILE* file) noexcept
{ return CDECL_1(int32_t, 0x618DF8, file); }

// address: 0x618F5E
// Heroes3.exe internal thread-safe fclose /
// внутренний fclose Heroes3.exe (thread-safe).
inline int32_t exe_fclose(exe_FILE* file) noexcept
{ return CDECL_1(int32_t, 0x618F5E, file); }

// address: 0x61A06D
// Heroes3.exe internal thread-safe _close /
// внутренний _close Heroes3.exe (thread-safe).
inline int32_t exe_close(int32_t fd) noexcept
{ return CDECL_1(int32_t, 0x61A06D, fd); }

// address: 0x6194DD
// Heroes3.exe internal thread-safe ftell /
// внутренний ftell Heroes3.exe (thread-safe).
inline int32_t exe_ftell(exe_FILE* file) noexcept
{ return CDECL_1(int32_t, 0x6194DD, file); }

// address: 0x619691
// Heroes3.exe internal thread-safe fopen /
// внутренний fopen Heroes3.exe (thread-safe).
inline exe_FILE* exe_fopen(const char* __restrict filename, const char* __restrict mode) noexcept
{ return CDECL_2(exe_FILE*, 0x619691, filename, mode); }

// address: 0x61A14D
// Heroes3.exe internal thread-safe _open /
// внутренний _open Heroes3.exe (thread-safe).
inline int32_t exe_open(const char* filename, int32_t oflag, int32_t pmode) noexcept
{ return CDECL_3(int32_t, 0x61A14D, filename, oflag, pmode); }

// address: 0x6196A4
// Heroes3.exe internal thread-safe fread /
// внутренний fread Heroes3.exe (thread-safe).
inline size_t exe_fread(void* __restrict buffer, size_t element_size, size_t count, exe_FILE* __restrict file) noexcept
{ return CDECL_4(size_t, 0x6196A4, buffer, element_size, count, file); }

// address: 0x6212C1
// Heroes3.exe internal thread-safe _read /
// внутренний _read Heroes3.exe (thread-safe).
inline int32_t exe_read(int32_t fd, void* buffer, size_t buffer_size) noexcept
{ return CDECL_3(int32_t, 0x6212C1, fd, buffer, buffer_size); }

// address: 0x62092F
// Heroes3.exe internal thread-safe _filbuf /
// внутренний _filbuf Heroes3.exe (thread-safe).
inline int32_t exe_filbuf(exe_FILE* file) noexcept
{ return CDECL_1(int32_t, 0x62092F, file); }

// address: 0x61DA81
// Heroes3.exe internal thread-safe _flsbuf /
// внутренний _flsbuf Heroes3.exe (thread-safe).
inline int32_t exe_flsbuf(int32_t c, exe_FILE* file) noexcept
{ return CDECL_2(int32_t, 0x61DA81, c, file); }

struct exe_fcloser
{
    inline void operator()(exe_FILE* ptr) const noexcept
    { exe_fclose(ptr); }
};

using exe_unique_file = std::unique_ptr<exe_FILE, exe_fcloser>;

inline exe_unique_file exe_make_unique_file(const char* __restrict filename, const char* __restrict mode)
{ return exe_unique_file { exe_fopen(filename, mode), exe_fcloser{} }; }

// feof
inline bool32_t exe_feof(exe_FILE* _stream) noexcept
{ return static_cast<uint32_t>(_stream->_flag) & 0x10U; }

// ferror
inline bool32_t exe_ferror(exe_FILE* _stream) noexcept
{ return static_cast<uint32_t>(_stream->_flag) & 0x20U; }

// _fileno
inline int32_t exe_fileno(exe_FILE* _stream) noexcept
{ return _stream->_file; }

// getc
inline int32_t exe_getc(exe_FILE* _stream) noexcept
{ return --(_stream)->_cnt >= 0 ? 0xFFU & *(_stream)->_ptr++ : exe_filbuf(_stream); }

// putc
inline int32_t exe_putc(int32_t _c, exe_FILE* _stream) noexcept
{ return --(_stream)->_cnt >= 0 ? 0xFFU & (*(_stream)->_ptr++ = static_cast<char>(_c)) : exe_flsbuf(_c,_stream); }

struct exe_ios
{
    enum event { erase_event = 0, imbue_event = 1, copyfmt_event = 2 };

    static constexpr unsigned int skipws     = 0x0001;
    static constexpr unsigned int unitbuf    = 0x0002;
    static constexpr unsigned int uppercase  = 0x0004;
    static constexpr unsigned int showbase   = 0x0008;
    static constexpr unsigned int showpoint  = 0x0010;
    static constexpr unsigned int showpos    = 0x0020;
    static constexpr unsigned int left       = 0x0040;
    static constexpr unsigned int right      = 0x0080;
    static constexpr unsigned int internal   = 0x0100;
    static constexpr unsigned int dec        = 0x0200;
    static constexpr unsigned int oct        = 0x0400;
    static constexpr unsigned int hex        = 0x0800;
    static constexpr unsigned int scientific = 0x1000;
    static constexpr unsigned int fixed      = 0x2000;

    static constexpr unsigned int boolalpha   = 0x4000;
    static constexpr unsigned int adjustfield = left | right | internal;
    static constexpr unsigned int basefield   = dec | oct | hex;
    static constexpr unsigned int floatfield  = scientific | fixed;

    enum iostate : uint32_t
    { goodbit = 0x0, eofbit = 0x1, failbit = 0x2, badbit = 0x4, _Statmask = 0x7 };

    enum openmode : uint32_t
    { in = 0x01, out = 0x02, ate = 0x04, app = 0x08, trunc = 0x10, binary = 0x20 };

    enum seekdir : uint32_t
    { beg = 0, cur = 1, end = 2 };
};

inline constexpr exe_ios::iostate operator|(exe_ios::iostate lhs, exe_ios::iostate rhs) noexcept
{ return static_cast<exe_ios::iostate>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

inline constexpr exe_ios::openmode operator|(exe_ios::openmode lhs, exe_ios::openmode rhs) noexcept
{ return static_cast<exe_ios::openmode>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

inline constexpr exe_ios::seekdir operator|(exe_ios::seekdir lhs, exe_ios::seekdir rhs) noexcept
{ return static_cast<exe_ios::seekdir>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

template<class _St>
class exe_fpos
{
    static_assert(sizeof(_St) <= 4, "exe_fpos<_St>: _St must be at maximum 4 bytes long");

    public:
        explicit exe_fpos(int32_t _O = -1) noexcept
            : _Off { _O }, _Fpos { 0 }, _State { _Stz }
        {}

        exe_fpos(_St _S, fpos_t _F) noexcept
            : _Off { 0 }, _Fpos { _F }, _State { _S }
        {}

        exe_fpos(const nh3api::dummy_tag_t&) noexcept
        {}

        [[nodiscard]] _St state() const noexcept
        { return _State; }

        [[nodiscard]] fpos_t get_fpos_t() const noexcept
        { return _Fpos; }

        [[nodiscard]] explicit operator int32_t() const noexcept
        { return _Off + static_cast<int32_t>(_Fpos); }

        [[nodiscard]] int32_t operator-(const exe_fpos<_St>& _R) const noexcept
        { return static_cast<int32_t>(*this) - static_cast<int32_t>(_R); }

        exe_fpos<_St>& operator+=(int32_t _O) noexcept
        {
            _Off += _O;
            return *this;
        }

        exe_fpos<_St>& operator-=(int32_t _O) noexcept
        {
            _Off -= _O;
            return *this;
        }

        exe_fpos<_St> operator+(int32_t _O) const noexcept
        {
            exe_fpos<_St> _Tmp = *this;
            return _Tmp += _O;
        }

        exe_fpos<_St> operator-(int32_t _O) const noexcept
        {
            exe_fpos<_St> _Tmp = *this;
            return _Tmp -= _O;
        }

        [[nodiscard]] bool operator==(const exe_fpos<_St>& _R) const noexcept
        { return static_cast<int32_t>(*this) == static_cast<int32_t>(_R); }

    #ifndef __cpp_impl_three_way_comparison
        [[nodiscard]] bool operator!=(const exe_fpos<_St>& _R) const noexcept
        { return !(*this == _R); }
    #endif

    private:
        inline static constexpr _St _Stz = _St{};

    private:
        int32_t _Off;
        uint32_t : 32;
        fpos_t _Fpos;
        _St    _State;
        uint32_t : 32;
} NH3API_MSVC_LAYOUT;

using exe_mbstate_t = int32_t;
using exe_streampos = exe_fpos<exe_mbstate_t>;

#ifndef NH3API_VIRTUAL_OVERRIDE_STREAMBUF
#define NH3API_VIRTUAL_OVERRIDE_STREAMBUF(CLASS_NAME) \
void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
int32_t __thiscall overflow(int32_t ch = static_cast<int32_t>(EOF)) override\
{ return get_type_vftable(this)->overflow(this, ch); }\
int32_t __thiscall pbackfail(int32_t ch = static_cast<int32_t>(EOF)) override\
{ return get_type_vftable(this)->pbackfail(this, ch); }\
int32_t __thiscall showmanyc() override\
{ return get_type_vftable(this)->showmanyc(this); }\
int32_t __thiscall underflow() override\
{ return get_type_vftable(this)->underflow(this); }\
int32_t __thiscall uflow() override\
{ return get_type_vftable(this)->uflow(this); }\
int32_t __thiscall xsgetn(char* _S, int32_t _N) override\
{ return get_type_vftable(this)->xsgetn(this, _S, _N); }\
int32_t __thiscall xsputn(const char *_S, int32_t _N) override\
{ return get_type_vftable(this)->xsputn(this, _S, _N); }\
pos_type* __thiscall seekoff(pos_type* result, off_type off, exe_ios::seekdir seek, exe_ios::openmode mode) override\
{ return get_type_vftable(this)->seekoff(this, result, off, seek, mode); }\
pos_type* __thiscall seekpos(pos_type* result, pos_type pos, exe_ios::openmode mode) override\
{ return get_type_vftable(this)->seekpos(this, result, pos, mode); } \
CLASS_NAME* __thiscall setbuf(char* buf, int32_t size) override \
{ return static_cast<CLASS_NAME*>(get_type_vftable(this)->setbuf(this, buf, size)); }\
int32_t __thiscall sync() override\
{ return get_type_vftable(this)->sync(this); }\
void __thiscall imbue(void* ptr) override\
{ get_type_vftable(this)->imbue(this, ptr); }
#endif

NH3API_VIRTUAL_CLASS exe_streambuf
{
public:
    using char_type = char;
    using traits_type = ::std::char_traits<char>;
    using int_type = int32_t;
    using pos_type = exe_streampos;
    using off_type = int32_t;

public:
    struct vftable_t
    {
        void           (__thiscall* scalar_deleting_destructor)(exe_streambuf*, uint8_t); // +0
        int32_t        (__thiscall* overflow)(exe_streambuf*, int32_t); // +4
        int32_t        (__thiscall* pbackfail)(exe_streambuf*, int32_t); // +8
        int32_t        (__thiscall* showmanyc)(exe_streambuf*); // +12
        int32_t        (__thiscall* underflow)(exe_streambuf*); // +16
        int32_t        (__thiscall* uflow)(exe_streambuf*); // +20
        int32_t        (__thiscall* xsgetn)(exe_streambuf*, char*, int32_t); // +24
        int32_t        (__thiscall* xsputn)(exe_streambuf*, const char*, int32_t); // +28
        pos_type*      (__thiscall* seekoff)(exe_streambuf*, pos_type*, off_type, exe_ios::seekdir, exe_ios::openmode); // +32
        pos_type*      (__thiscall* seekpos)(exe_streambuf*, pos_type*, pos_type, exe_ios::openmode); // +36
        exe_streambuf* (__thiscall* setbuf)(exe_streambuf*, char*, int32_t); // +40
        int32_t        (__thiscall* sync)(exe_streambuf*); // +44
        void           (__thiscall* imbue)(exe_streambuf*, void*); // +48
    };

protected:
    exe_streambuf() noexcept
    {
        NH3API_SET_VFTABLE();
        _Init();
    }

    exe_streambuf& operator=(const exe_streambuf& other) noexcept = default;

    exe_streambuf(const nh3api::omit_base_vftable_tag_t&) noexcept
    { _Init(); }

    exe_streambuf(const nh3api::dummy_tag_t&) noexcept
    {}

public:
    // vftable shift: +0
    NH3API_SCALAR_DELETING_DESTRUCTOR

    pos_type __thiscall pubseekoff(off_type _O, exe_ios::seekdir _W,
        exe_ios::openmode _M /*= exe_ios::in | exe_ios::out*/)
    {
        pos_type result(nh3api::dummy_tag);
        result = *seekoff(&result, _O, _W, _M);
        return result;
    } // ABI compability

    pos_type __thiscall pubseekpos(pos_type _P,
        exe_ios::openmode _M /*= exe_ios::in | exe_ios::out*/)
    {
        pos_type result(nh3api::dummy_tag);
        result = *seekpos(&result, _P, _M);
        return result;
    } // ABI compability

    exe_streambuf* __thiscall pubsetbuf(char *_S, int32_t _N)
    { return setbuf(_S, _N); }

    int32_t __thiscall in_avail()
    {
        return gptr() != nullptr && gptr() < egptr()
                     ? egptr() - gptr()
                     : showmanyc();
    }

    int __thiscall pubsync()
    { return sync(); }

    int32_t __thiscall sbumpc()
    {
        return gptr() != nullptr && gptr() < egptr()
                     ? static_cast<int32_t>(*_Gninc())
                     : uflow();
    }

    int32_t __thiscall sgetc()
    {
        return gptr() != nullptr && gptr() < egptr()
                     ? static_cast<int32_t>(*gptr())
                     : underflow();
    }

    int32_t __thiscall sgetn(char *_S, int32_t _N)
    { return xsgetn(_S, _N); }

    int32_t __thiscall snextc()
    {
        return (static_cast<int32_t>(EOF) == sbumpc())
                     ? static_cast<int32_t>(EOF)
                     : sgetc();
    }

    int32_t __thiscall sputbackc(char _C)
    {
        return gptr() != nullptr && eback() < gptr()
                            && (_C == gptr()[-1])
                     ? static_cast<int32_t>(*_Gndec())
                     : pbackfail(static_cast<int32_t>(_C));
    }

    void __thiscall stossc()
    {
        if ( gptr() != nullptr && gptr() < egptr() )
            _Gninc();
        else
            uflow();
    }

    int32_t __thiscall sungetc()
    {
        return gptr() != nullptr && eback() < gptr()
                     ? static_cast<int32_t>(*_Gndec())
                     : pbackfail();
    }

    int32_t __thiscall sputc(char _C)
    {
        return pptr() != nullptr && pptr() < epptr()
                     ? static_cast<int32_t>(*_Pninc() = _C)
                     : overflow(static_cast<int32_t>(_C));
    }

    int32_t __thiscall sputn(const char *_S, int32_t _N)
    { return xsputn(_S, _N); }

protected:
    [[nodiscard]] char* __thiscall eback() const noexcept
    { return *_IGbeg; }

    [[nodiscard]] char* __thiscall gptr() const noexcept
    { return *_IGnext; }

    [[nodiscard]] char* __thiscall pbase() const noexcept
    { return *_IPbeg; }

    [[nodiscard]] char* __thiscall pptr() const noexcept
    { return *_IPnext; }

    [[nodiscard]] char* __thiscall egptr() const noexcept
    { return *_IGnext + *_IGcnt; }

    void __thiscall gbump(int _N)
    {
        *_IGcnt -= _N;
        *_IGnext += _N;
    }

    void __thiscall setg(char* _B, char* _N, char* _L) noexcept
    {
        *_IGbeg  = _B;
        *_IGnext = _N;
        *_IGcnt  = _L - _N;
    }

    [[nodiscard]] char* __thiscall epptr() const noexcept
    { return *_IPnext + *_IPcnt; }

private:
    char* __thiscall _Gndec() noexcept
    {
        ++*_IGcnt;
        return --(*_IGnext);
    }

    char* __thiscall _Gninc() noexcept
    {
        --*_IGcnt;
        return (*_IGnext)++;
    }

public:
    void __thiscall pbump(int _N) noexcept
    {
        *_IPcnt -= _N;
        *_IPnext += _N;
    }

    void __thiscall setp(char* _B, char* _L) noexcept
    {
        *_IPbeg  = _B;
        *_IPnext = _B;
        *_IPcnt  = _L - _B;
    }

    void __thiscall setp(char* _B, char* _N, char* _L) noexcept
    {
        *_IPbeg  = _B;
        *_IPnext = _N;
        *_IPcnt  = _L - _N;
    }

private:
    char* __thiscall _Pninc()
    {
        --*_IPcnt;
        return (*_IPnext)++;
    }

    void __thiscall _Init() noexcept
    {
        _IGbeg = &_Gbeg;
        _IPbeg = &_Pbeg;
        _IGnext = &_Gnext;
        _IPnext = &_Pnext;
        _IGcnt = &_Gcnt;
        _IPcnt = &_Pcnt;
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);
    }

    void __thiscall _Init(char** _Gb, char** _Gn, int* _Gc,
                          char** _Pb, char** _Pn, int* _Pc) noexcept
    {
        _IGbeg  = _Gb;
        _IPbeg  = _Pb;
        _IGnext = _Gn;
        _IPnext = _Pn;
        _IGcnt  = _Gc;
        _IPcnt  = _Pc;
    }

public:
    // vftable shift: +4
    virtual int32_t __thiscall overflow(int32_t ch = static_cast<int32_t>(EOF))
    { return get_type_vftable(this)->overflow(this, ch); }
    //{ return (static_cast<int32_t>(EOF)); }

    // vftable shift: +8
    virtual int32_t __thiscall pbackfail(int32_t ch = static_cast<int32_t>(EOF))
    { return get_type_vftable(this)->pbackfail(this, ch); }
    // { return (static_cast<int32_t>(EOF)); }

    // vftable shift: +12
    virtual int32_t __thiscall showmanyc()
    { return get_type_vftable(this)->showmanyc(this); }
    // { return 0; }

    // vftable shift: +16
    virtual int32_t __thiscall underflow()
    { return get_type_vftable(this)->underflow(this); }
    //{ return (static_cast<int32_t>(EOF)); }

    // vftable shift: +20
    virtual int32_t __thiscall uflow()
    { return get_type_vftable(this)->uflow(this); }
    /* { return (traits_type::eq_int_type(static_cast<int32_t>(EOF), underflow())
            ? static_cast<int32_t>(EOF) : static_cast<int32_t>(*_Gninc())); } */

    // vftable shift: +24
    virtual int32_t __thiscall xsgetn(char * _S, int32_t _N)
    {  return get_type_vftable(this)->xsgetn(this, _S, _N); }
    /*
    {
        int32_t _C;
        int32_t _M, _Ns;
        for (_Ns = 0; 0 < _N; )
            if (gptr() != 0 && 0 < (_M = egptr() - gptr()))
                {if (_N < _M)
                    _M = _N;
                traits_type::copy(_S, gptr(), _M);
                _S += _M, _Ns += _M, _N -= _M, gbump(_M); }
            else if (traits_type::eq_int_type(static_cast<int32_t>(EOF), _C = uflow()))
                break;
            else
                *_S++ = traits_type::to_char_type(_C), ++_Ns, --_N;
        return (_Ns);
    }
    */

    // vftable shift: +28
    virtual int32_t __thiscall xsputn(const char *_S, int32_t _N)
    { return get_type_vftable(this)->xsputn(this, _S, _N); }
    /*
    {
        int32_t _M, _Ns;
        for (_Ns = 0; 0 < _N; )
            if (pptr() != 0 && 0 < (_M = epptr() - pptr()))
                {if (_N < _M)
                    _M = _N;
                traits_type::copy(pptr(), _S, _M);
                _S += _M, _Ns += _M, _N -= _M, pbump(_M); }
            else if (traits_type::eq_int_type(static_cast<int32_t>(EOF),
                overflow(static_cast<int32_t>(*_S))))
                break;
            else
                ++_S, ++_Ns, --_N;
        return (_Ns);
    }
    */

    // vftable shift: +32
    virtual pos_type* __thiscall seekoff(pos_type* result,
                                         off_type off,
                                         exe_ios::seekdir seek,
                                         exe_ios::openmode mode /*= exe_ios::in | exe_ios::out*/)
    { return get_type_vftable(this)->seekoff(this, result, off, seek, mode); }
    //{ return new (result) exe_streampos(-1); } // ABI compability

    // vftable shift: +36
    virtual pos_type* __thiscall seekpos(pos_type* result,
    pos_type pos, exe_ios::openmode mode /*= exe_ios::in | exe_ios::out*/)
    { return get_type_vftable(this)->seekpos(this, result, pos, mode); }
    //{ return new (result) exe_streampos(-1); } // ABI compability

    // vftable shift: +40
    virtual exe_streambuf* __thiscall setbuf(char* buf, int32_t size)
    { return get_type_vftable(this)->setbuf(this, buf, size); }
    //{ return this; }

    // vftable shift: +44
    virtual int32_t __thiscall sync()
    { return get_type_vftable(this)->sync(this); }
    //{ return 0; }

    // vftable shift: +48
    virtual void __thiscall imbue(void* ptr)
    { get_type_vftable(this)->imbue(this, ptr); }
    //{}

public:
    // offset: +0x4 = +4,  size = 0x4 = 4
    char* _Gbeg;

    // offset: +0x8 = +8,  size = 0x4 = 4
    char* _Pbeg;

    // offset: +0xC = +12,  size = 0x4 = 4
    char** _IGbeg;

    // offset: +0x10 = +16,  size = 0x4 = 4
    char** _IPbeg;

    // offset: +0x14 = +20,  size = 0x4 = 4
    char* _Gnext;

    // offset: +0x18 = +24,  size = 0x4 = 4
    char* _Pnext;

    // offset: +0x1C = +28,  size = 0x4 = 4
    char** _IGnext;

    // offset: +0x20 = +32,  size = 0x4 = 4
    char** _IPnext;

    // offset: +0x24 = +36,  size = 0x4 = 4
    int32_t _Gcnt;

    // offset: +0x28 = +40,  size = 0x4 = 4
    int32_t _Pcnt;

    // offset: +0x2C = +44,  size = 0x4 = 4
    int32_t* _IGcnt;

    // offset: +0x30 = +48,  size = 0x4 = 4
    int32_t* _IPcnt;

    // offset: +0x34 = +52,  size = 0x4 = 4
    void* _Loc;
};

// std::filebuf
// size = 0x54 = 84, align = 4, baseclass: std::streambuf
NH3API_VIRTUAL_CLASS exe_filebuf : public exe_streambuf
{
    public:
        using native_handle_type = HANDLE;
        enum _Initfl { _Newfl, _Openfl, _Closefl }; // init flag

    public:
        inline explicit exe_filebuf(exe_FILE* _Fileptr = nullptr) noexcept
            : exe_filebuf(nh3api::dummy_tag)
        { THISCALL_2(void, 0x60D3CF, this, _Fileptr); }

        inline exe_filebuf(const nh3api::dummy_tag_t& tag) noexcept
            : exe_streambuf(tag)
        {}

        inline ~exe_filebuf() noexcept
        { get_type_vftable(this)->scalar_deleting_destructor(this, 0); }

    public:
        [[nodiscard]] bool is_open() const noexcept
        { return static_cast<bool>(_File); }

        exe_filebuf* open(const char* _S, exe_ios::openmode _M) noexcept
        {
            exe_FILE* _Fp = CDECL_2(exe_FILE*, 0x60CA8E, _S, _M);
            if ( _File != nullptr || _Fp == nullptr )
                return nullptr;
            THISCALL_3(void, 0x48D170, this, _Fp, _Openfl);
            THISCALL_1(void, 0x48D260, this);
            return this;
        }

        exe_filebuf* close() noexcept
        {
            if ( _File != nullptr && exe_fclose(_File) == 0 )
            {
                THISCALL_3(void, 0x48D170, this, nullptr, _Closefl);
                return this;
            }

            return nullptr;
        }

        [[nodiscard]] native_handle_type native_handle() const noexcept
        { return CDECL_1(HANDLE, 0x6222B3, exe_fileno(_File)); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_STREAMBUF(exe_filebuf)

    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x4 = 4
        void* _Pcvt;

        // offset: +0x3C = +60,  size = 0x4 = 4
        int32_t _State0;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t _State;

        // offset: +0x44 = +68,  size = 0x4 = 4
        exe_string* _Str;

        // offset: +0x48 = +72,  size = 0x1 = 1
        bool _Closef;

        // offset: +0x4C = +76,  size = 0x4 = 4
        void* _Loc2;

        // offset: +0x50 = +80,  size = 0x4 = 4
        exe_FILE* _File;

};

// std::strstreambuf
// size = 0x50 = 80, align = 4, baseclass: std::streambuf
NH3API_VIRTUAL_CLASS exe_strstreambuf : public exe_streambuf
{
    public:
        inline explicit exe_strstreambuf(int32_t _N = 0) noexcept
            : exe_strstreambuf(nh3api::dummy_tag)
        { THISCALL_2(void, 0x4512D0, this, _N); }

        inline exe_strstreambuf(const char* const _String, int32_t _N) noexcept
            : exe_strstreambuf(nh3api::dummy_tag)
        { THISCALL_3(void, 0x5167C0, this, _String, _N); }

        inline exe_strstreambuf(const nh3api::dummy_tag_t& tag) noexcept
            : exe_streambuf(tag)
        {}

        inline ~exe_strstreambuf() noexcept
        { get_type_vftable(this)->scalar_deleting_destructor(this, 0); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_STREAMBUF(exe_strstreambuf)

    // member variables
    public:
        // offset: +0x38 = +56,  size = 0x4 = 4
        char* _Pendsave;

        // offset: +0x3C = +60,  size = 0x4 = 4
        char* _Seekhigh;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t _Alsize;

        // offset: +0x44 = +68,  size = 0x4 = 4
        int32_t _Strmode;

        // offset: +0x48 = +72,  size = 0x4 = 4
        void* (__cdecl* _Palloc)(size_t);

        // offset: +0x4C = +76,  size = 0x4 = 4
        void (__cdecl* _Pfree)(void*);

};
#pragma pack(pop)

NH3API_SPECIALIZE_TYPE_VFTABLE(0x6456B8, exe_streambuf)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x645820, exe_filebuf)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x645680, exe_strstreambuf)

inline int32_t read(exe_streambuf& stream, void* buf, size_t len)
{ return stream.sgetn(static_cast<char*>(buf), static_cast<int32_t>(len)); }

inline int32_t write(exe_streambuf& stream, const void* buf, size_t len)
{ return stream.sputn(static_cast<const char*>(buf), static_cast<int32_t>(len)); }

NH3API_WARNING(pop)
