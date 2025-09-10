//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
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

#include "memory.hpp"     // get_type_vftable 
#include "exe_string.hpp" // exe_string

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

typedef int32_t exe_streamoff;
typedef int32_t exe_streamsize;
typedef int64_t exe_fpos_t;

#pragma pack(push, 4)
//
// size = 0x20 = 32, align = 4
struct exe_iobuf
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
#pragma pack(pop)
typedef struct exe_iobuf exe_FILE;

NH3API_FORCEINLINE
// address: 0x618A1A
// Heroes3.exe internal thread-safe fwrite /
// внутренний fwrite Heroes3.exe (thread-safe).
size_t __cdecl exe_fwrite(const void* buffer, size_t size, size_t count, exe_FILE* stream) NH3API_NOEXCEPT
{ return CDECL_4(size_t, 0x618A1A, buffer, size, count, stream); }

NH3API_FORCEINLINE
// address: 0x618B53
// Heroes3.exe internal thread-safe fputc /
// внутренний fputc Heroes3.exe (thread-safe).
int32_t __cdecl exe_fputc(int32_t character, exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_2(int32_t, 0x618B53, character, file); }

NH3API_FORCEINLINE
// address: 0x618B8E
// Heroes3.exe internal thread-safe fgetc /
// внутренний fgetc Heroes3.exe (thread-safe).
int32_t __cdecl exe_fgetc(exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x618B8E, file); }

NH3API_FORCEINLINE
// address: 0x618BBF
// Heroes3.exe internal thread-safe ungetc /
// внутренний ungetc Heroes3.exe (thread-safe).
int32_t __cdecl exe_ungetc(int32_t character, exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_2(int32_t, 0x618BBF, character, file); }

NH3API_FORCEINLINE
// address: 0x618C56
// Heroes3.exe internal thread-safe fgetpos /
// внутренний fgetpos Heroes3.exe (thread-safe).
int32_t __cdecl exe_fgetpos(exe_FILE* in_file, int64_t* out_pos) NH3API_NOEXCEPT
{ return CDECL_2(int32_t, 0x618C56, in_file, out_pos); }

NH3API_FORCEINLINE
// address: 0x618C78
// Heroes3.exe internal thread-safe fseek /
// внутренний fseek Heroes3.exe (thread-safe).
int32_t __cdecl exe_fseek(exe_FILE* file, ptrdiff_t offset, ptrdiff_t origin) NH3API_NOEXCEPT
{ return CDECL_3(int32_t, 0x618C78, file, offset, origin); }

NH3API_FORCEINLINE
// address: 0x618D31
// Heroes3.exe internal thread-safe fsetpos /
// внутренний fsetpos Heroes3.exe (thread-safe).
int32_t __cdecl exe_fsetpos(exe_FILE* in_file, int64_t* in_pos) NH3API_NOEXCEPT
{ return CDECL_2(int32_t, 0x618D31, in_file, in_pos); }

NH3API_FORCEINLINE
// address: 0x618D49
// Heroes3.exe internal thread-safe setvbuf /
// внутренний setvbuf Heroes3.exe (thread-safe).
int32_t __cdecl exe_setvbuf(exe_FILE* file, char* buffer, int32_t mode, size_t size) NH3API_NOEXCEPT
{ return CDECL_4(int32_t, 0x618D49, file, buffer, mode, size); }

NH3API_FORCEINLINE
// address: 0x618DF8
// Heroes3.exe internal thread-safe fflush /
// внутренний fflush Heroes3.exe (thread-safe).
int32_t __cdecl exe_fflush(exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x618DF8, file); }

NH3API_FORCEINLINE
// address: 0x618F5E
// Heroes3.exe internal thread-safe fclose /
// внутренний fclose Heroes3.exe (thread-safe).
int32_t __cdecl exe_fclose(exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x618F5E, file); }

NH3API_FORCEINLINE
// address: [0x61A06D..0x61A0CA],  size = 0x5D = 93
// Heroes3.exe internal thread-safe _close /
// внутренний _close Heroes3.exe (thread-safe).
int32_t __cdecl exe_close(int32_t fd) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x61A06D, fd); }

NH3API_FORCEINLINE
// address: 0x6194DD
// Heroes3.exe internal thread-safe ftell /
// внутренний ftell Heroes3.exe (thread-safe).
int32_t __cdecl exe_ftell(exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x6194DD, file); }

NH3API_FORCEINLINE
// address: 0x619691
// Heroes3.exe internal thread-safe fopen /
// внутренний fopen Heroes3.exe (thread-safe).
exe_FILE* __cdecl exe_fopen(const char* filename, const char* mode) NH3API_NOEXCEPT
{ return CDECL_2(exe_FILE*, 0x619691, filename, mode); }

NH3API_DEALLOCATOR(exe_close, 1)
NH3API_FORCEINLINE
// address: 0x61A14D
// Heroes3.exe internal thread-safe _open /
// внутренний _open Heroes3.exe (thread-safe).
int32_t __cdecl exe_open(const char* filename, int32_t oflag, int32_t pmode) NH3API_NOEXCEPT
{ return CDECL_3(int32_t, 0x61A14D, filename, oflag, pmode); }

NH3API_FORCEINLINE
// address: 0x6196A4
// Heroes3.exe internal thread-safe fread /
// внутренний fread Heroes3.exe (thread-safe).
size_t __cdecl exe_fread(void* destBuffer, size_t elementSize, size_t count, void* file) NH3API_NOEXCEPT
{ return CDECL_4(size_t, 0x6196A4, destBuffer, elementSize, count, file); }

NH3API_FORCEINLINE
// address: 0x6212C1
// Heroes3.exe internal thread-safe _read /
// внутренний _read Heroes3.exe (thread-safe).
int32_t __cdecl exe_read(int32_t fd, void* buffer, uint32_t buffer_size) NH3API_NOEXCEPT
{ return CDECL_3(int32_t, 0x6212C1, fd, buffer, buffer_size); }

NH3API_FORCEINLINE
// address: 0x62092F
// Heroes3.exe internal thread-safe _filbuf /
// внутренний _filbuf Heroes3.exe (thread-safe).
int32_t __cdecl exe_filbuf(exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_1(int32_t, 0x62092F, file); }

NH3API_FORCEINLINE
// address: 0x61DA81
// Heroes3.exe internal thread-safe _flsbuf /
// внутренний _flsbuf Heroes3.exe (thread-safe).
int32_t __cdecl exe_flsbuf(int32_t c, exe_FILE* file) NH3API_NOEXCEPT
{ return CDECL_2(int32_t, 0x61DA81, c, file); }

enum exe_io_error_codes : uint32_t
{
    EXE_IOEOF = 0x10,
    EXE_IOERR = 0x20
};

// feof
NH3API_FORCEINLINE
bool32_t exe_feof(exe_FILE* _stream) NH3API_NOEXCEPT
{ return _stream->_flag & EXE_IOEOF; }

// ferror
NH3API_FORCEINLINE
bool32_t exe_ferror(exe_FILE* _stream) NH3API_NOEXCEPT
{ return _stream->_flag & EXE_IOERR; }

// _fileno
NH3API_FORCEINLINE
int32_t exe_fileno(exe_FILE* _stream) NH3API_NOEXCEPT
{ return _stream->_file; }

// getc
NH3API_FORCEINLINE
int32_t exe_getc(exe_FILE* _stream) NH3API_NOEXCEPT
{ return --(_stream)->_cnt >= 0 ? 0xff & *(_stream)->_ptr++ : exe_filbuf(_stream); }

// putc
NH3API_FORCEINLINE
int32_t exe_putc(int32_t _c, exe_FILE* _stream) NH3API_NOEXCEPT
{ return --(_stream)->_cnt >= 0 ? 0xff & (*(_stream)->_ptr++ = static_cast<char>(_c)) : exe_flsbuf(_c,_stream); }

enum exe_seek_flags : int32_t
{
    EXE_SEEK_SET = 0,
    EXE_SEEK_CUR = 1,
    EXE_SEEK_END = 2,
};

struct exe_ios
{
    enum iostate : uint32_t
    { goodbit = 0x0, eofbit = 0x1, failbit = 0x2, badbit = 0x4, _Statmask = 0x7 };
    enum openmode : uint32_t
    { in = 0x01, out = 0x02, ate = 0x04, app = 0x08, trunc = 0x10, binary = 0x20 };
    enum seekdir : uint32_t
    { beg = 0, cur = 1, end = 2 };
};

NH3API_CONSTEXPR NH3API_FORCEINLINE exe_ios::iostate operator|(exe_ios::iostate lhs, exe_ios::iostate rhs) NH3API_NOEXCEPT
{ return static_cast<exe_ios::iostate>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE exe_ios::openmode operator|(exe_ios::openmode lhs, exe_ios::openmode rhs) NH3API_NOEXCEPT
{ return static_cast<exe_ios::openmode>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

NH3API_CONSTEXPR NH3API_FORCEINLINE exe_ios::seekdir operator|(exe_ios::seekdir lhs, exe_ios::seekdir rhs) NH3API_NOEXCEPT
{ return static_cast<exe_ios::seekdir>(static_cast<uint32_t>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs))); }

template<class _St>
class exe_fpos
{
    public:
        exe_fpos(exe_streamoff _O = -1)
        : _Off(_O), _Fpos(0), _State(_Stz)
        {}

        exe_fpos(_St _S, exe_fpos_t _F)
        : _Off(0), _Fpos(_F), _State(_S)
        {}

        exe_fpos(const nh3api::dummy_tag_t&)
        { NH3API_IGNORE(_Off, _Fpos, _State); }

        _St state() const
        { return _State; }

        exe_fpos_t get_fpos_t() const
        { return _Fpos; }

        operator exe_streamoff() const
        { return (_Off + static_cast<int32_t>(_Fpos)); }

        exe_streamoff operator-(const exe_fpos<_St>& _R) const
        { return (exe_streamoff)*this - (exe_streamoff)_R; }

        exe_fpos<_St>& operator+=(exe_streamoff _O)
        {
            _Off += _O;
            return *this;
        }

        exe_fpos<_St>& operator-=(exe_streamoff _O)
        {
            _Off -= _O;
            return *this;
        }

        exe_fpos<_St> operator+(exe_streamoff _O) const
        {
            exe_fpos<_St> _Tmp = *this;
            return _Tmp += _O;
        }

        exe_fpos<_St> operator-(exe_streamoff _O) const
        {
            exe_fpos<_St> _Tmp = *this;
            return _Tmp -= _O;
        }

        bool operator==(const exe_fpos<_St>& _R) const
        { return (exe_streamoff)*this == (exe_streamoff)_R; }

        bool operator!=(const exe_fpos<_St>& _R) const
        { return !(*this == _R); }

        private:
            static const _St _Stz;

        private:
            exe_streamoff _Off;
            exe_fpos_t    _Fpos;
            _St           _State;
};

template<class _St>
const _St exe_fpos<_St>::_Stz = _St();

typedef int32_t                 exe_mbstate_t;
typedef exe_fpos<exe_mbstate_t> exe_streampos;

#ifndef NH3API_VIRTUAL_OVERRIDE_STREAMBUF
#define NH3API_VIRTUAL_OVERRIDE_STREAMBUF(CLASS_NAME) \
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
virtual int_type __thiscall overflow(int_type ch = traits_type::eof()) override\
{ return get_type_vftable(this)->overflow(this, ch); }\
virtual int_type __thiscall pbackfail(int_type ch = traits_type::eof()) override\
{ return get_type_vftable(this)->pbackfail(this, ch); }\
virtual exe_streamsize __thiscall showmanyc() override\
{ return get_type_vftable(this)->showmanyc(this); }\
virtual int_type __thiscall underflow() override\
{ return get_type_vftable(this)->underflow(this); }\
virtual int_type __thiscall uflow() override\
{ return get_type_vftable(this)->uflow(this); }\
virtual exe_streamsize __thiscall xsgetn(char_type * _S, exe_streamsize _N) override\
{ return get_type_vftable(this)->xsgetn(this, _S, _N); }\
virtual exe_streamsize __thiscall xsputn(const char_type *_S, exe_streamsize _N) override\
{ return get_type_vftable(this)->xsputn(this, _S, _N); }\
virtual pos_type* __thiscall seekoff(pos_type* result, off_type off, exe_ios::seekdir seek, exe_ios::openmode mode) override\
{ return get_type_vftable(this)->seekoff(this, result, off, seek, mode); }\
virtual pos_type* __thiscall seekpos(pos_type* result, pos_type pos, exe_ios::openmode mode) override\
{ return get_type_vftable(this)->seekpos(this, result, pos, mode); }\
virtual CLASS_NAME* __thiscall setbuf(char_type* buf, exe_streamsize size) override\
{ return static_cast<CLASS_NAME*>(get_type_vftable(this)->setbuf(this, buf, size)); }\
virtual int32_t __thiscall sync() override\
{ return get_type_vftable(this)->sync(this); }\
virtual void __thiscall imbue(void* ptr) override\
{ get_type_vftable(this)->imbue(this, ptr); }
#endif

template<class CharT, class CharTraits = std::char_traits<CharT> >
NH3API_VIRTUAL_CLASS exe_basic_streambuf
{
private:
    typedef exe_basic_streambuf<CharT, CharTraits> this_type;

public:
    typedef CharT                    char_type;
    typedef CharTraits                   traits_type;
    typedef int32_t               int_type;
    typedef exe_streampos         pos_type;
    typedef int32_t               off_type;
public:
    struct vftable_t
    {
        friend class exe_basic_streambuf;
        void (__thiscall* scalar_deleting_destructor)(exe_basic_streambuf*, uint8_t); // +0
        int_type (__thiscall* overflow)(exe_basic_streambuf*, int_type); // +4
        int_type (__thiscall* pbackfail)(exe_basic_streambuf*, int_type); // +8
        exe_streamsize (__thiscall* showmanyc)(exe_basic_streambuf*); // +12
        int_type (__thiscall* underflow)(exe_basic_streambuf*); // +16
        int_type (__thiscall* uflow)(exe_basic_streambuf*); // +20
        exe_streamsize (__thiscall* xsgetn)(exe_basic_streambuf*, char_type*, exe_streamsize); // +24
        exe_streamsize (__thiscall* xsputn)(exe_basic_streambuf*, const char_type*, exe_streamsize); // +28
        pos_type* (__thiscall* seekoff)(exe_basic_streambuf*, pos_type*, off_type, exe_ios::seekdir, exe_ios::openmode); // +32
        pos_type* (__thiscall* seekpos)(exe_basic_streambuf*, pos_type*, pos_type, exe_ios::openmode); // +36
        exe_basic_streambuf* (__thiscall* setbuf)(exe_basic_streambuf*, char_type*, exe_streamsize); // +40
        int32_t (__thiscall* sync)(exe_basic_streambuf*); // +44
        void (__thiscall* imbue)(exe_basic_streambuf*, void*); // +48
    };

protected:
    exe_basic_streambuf()
    {
        NH3API_SET_VFTABLE();
        _Init();
    }

    exe_basic_streambuf(const nh3api::omit_base_vftable_tag_t& tag)
    {
        _Init();
    }

    exe_basic_streambuf& operator=(const this_type& other)
    { std::memcpy(this, &other, sizeof(*this)); }

public:
    exe_basic_streambuf(const nh3api::dummy_tag_t& tag)
    {}

public:

    // vftable shift: +0
    virtual void __thiscall scalar_deleting_destructor(uint8_t flag)
    { get_type_vftable(this)->scalar_deleting_destructor(this, flag); }

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

    exe_basic_streambuf<char_type, traits_type>* __thiscall pubsetbuf(char_type *_S, exe_streamsize _N)
    { return (setbuf(_S, _N)); }
    exe_streamsize __thiscall in_avail()
    {
        return (gptr() != 0 && gptr() < egptr()
            ? egptr() - gptr() : showmanyc());
    }
    int __thiscall pubsync()
    { return (sync()); }
    int_type __thiscall sbumpc()
    {
        return (gptr() != 0 && gptr() < egptr()
            ? traits_type::to_int_type(*_Gninc()) : uflow());
    }
    int_type __thiscall sgetc()
    {
        return (gptr() != 0 && gptr() < egptr()
            ? traits_type::to_int_type(*gptr()) : underflow());
    }
    exe_streamsize __thiscall sgetn(char_type *_S, exe_streamsize _N)
    { return (xsgetn(_S, _N)); }
    int_type __thiscall snextc()
    {
        return (traits_type::eq_int_type(traits_type::eof(), sbumpc())
            ? traits_type::eof() : sgetc());
    }
    int_type __thiscall sputbackc(char_type _C)
    {
        return (gptr() != 0 && eback() < gptr()
            && traits_type::eq(_C, gptr()[-1])
            ? traits_type::to_int_type(*_Gndec())
            : pbackfail(traits_type::to_int_type(_C)));
    }
    void __thiscall stossc()
    {
        if (gptr() != 0 && gptr() < egptr())
            _Gninc();
        else
            uflow();
    }
    int_type __thiscall sungetc()
    {
        return (gptr() != 0 && eback() < gptr()
            ? traits_type::to_int_type(*_Gndec()) : pbackfail());
    }
    int_type __thiscall sputc(char_type _C)
    {
        return (pptr() != 0 && pptr() < epptr()
            ? traits_type::to_int_type(*_Pninc() = _C)
            : overflow(traits_type::to_int_type(_C)));
    }
    exe_streamsize __thiscall sputn(const char_type *_S, exe_streamsize _N)
    { return (xsputn(_S, _N)); }

protected:
    char_type* __thiscall eback() const
    { return (*_IGbeg); }
    char_type* __thiscall gptr() const
    { return (*_IGnext); }
    char_type* __thiscall pbase() const
    { return (*_IPbeg); }
    char_type* __thiscall pptr() const
    { return (*_IPnext); }
    char_type* __thiscall egptr() const
    { return (*_IGnext + *_IGcnt); }
    void __thiscall gbump(int _N)
    {
        *_IGcnt -= _N;
        *_IGnext += _N;
    }
    void __thiscall setg(char_type *_B, char_type *_N, char_type *_L)
    { *_IGbeg = _B, *_IGnext = _N, *_IGcnt = _L - _N; }
    char_type* __thiscall epptr() const
    { return (*_IPnext + *_IPcnt); }
    char_type* __thiscall _Gndec()
    {
        ++*_IGcnt;
        return (--*_IGnext);
    }
    char_type* __thiscall _Gninc()
    {
        --*_IGcnt;
        return ((*_IGnext)++);
    }
    void __thiscall pbump(int _N)
    {
        *_IPcnt -= _N;
        *_IPnext += _N;
    }
    void __thiscall setp(char_type *_B, char_type *_L)
    { *_IPbeg = _B, *_IPnext = _B, *_IPcnt = _L - _B; }
    void __thiscall setp(char_type *_B, char_type *_N, char_type *_L)
    { *_IPbeg = _B, *_IPnext = _N, *_IPcnt = _L - _N; }
    char_type* __thiscall _Pninc()
    {
        --*_IPcnt;
        return ((*_IPnext)++);
    }
    void __thiscall _Init()
    {
        _IGbeg = &_Gbeg; _IPbeg = &_Pbeg;
        _IGnext = &_Gnext; _IPnext = &_Pnext;
        _IGcnt = &_Gcnt; _IPcnt = &_Pcnt;
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);
    }
    void __thiscall _Init(char_type **_Gb, char_type **_Gn, int *_Gc,
        char_type **_Pb, char_type **_Pn, int *_Pc)
    {
        _IGbeg = _Gb, _IPbeg = _Pb;
        _IGnext = _Gn, _IPnext = _Pn;
        _IGcnt = _Gc, _IPcnt = _Pc;
    }

    // vftable shift: +4
    virtual int_type __thiscall overflow(int_type ch = traits_type::eof())
    { return get_type_vftable(this)->overflow(this, ch); }
    //{ return (traits_type::eof()); }

    // vftable shift: +8
    virtual int_type __thiscall pbackfail(int_type ch = traits_type::eof())
    { return get_type_vftable(this)->pbackfail(this, ch); }
    // { return (traits_type::eof()); }

    // vftable shift: +12
    virtual exe_streamsize __thiscall showmanyc()
    { return get_type_vftable(this)->showmanyc(this); }
    // { return 0; }

    // vftable shift: +16
    virtual int_type __thiscall underflow()
    { return get_type_vftable(this)->underflow(this); }
    //{ return (traits_type::eof()); }

    // vftable shift: +20
    virtual int_type __thiscall uflow()
    { return get_type_vftable(this)->uflow(this); }
    /* { return (traits_type::eq_int_type(traits_type::eof(), underflow())
            ? traits_type::eof() : traits_type::to_int_type(*_Gninc())); } */

    // vftable shift: +24
    virtual exe_streamsize __thiscall xsgetn(char_type * _S, exe_streamsize _N)
    {  return get_type_vftable(this)->xsgetn(this, _S, _N); }
    /*
    {
        int_type _C;
        exe_streamsize _M, _Ns;
        for (_Ns = 0; 0 < _N; )
            if (gptr() != 0 && 0 < (_M = egptr() - gptr()))
                {if (_N < _M)
                    _M = _N;
                traits_type::copy(_S, gptr(), _M);
                _S += _M, _Ns += _M, _N -= _M, gbump(_M); }
            else if (traits_type::eq_int_type(traits_type::eof(), _C = uflow()))
                break;
            else
                *_S++ = traits_type::to_char_type(_C), ++_Ns, --_N;
        return (_Ns);
    }
    */

    // vftable shift: +28
    virtual exe_streamsize __thiscall xsputn(const char_type *_S, exe_streamsize _N)
    { return get_type_vftable(this)->xsputn(this, _S, _N); }
    /*
    {
        exe_streamsize _M, _Ns;
        for (_Ns = 0; 0 < _N; )
            if (pptr() != 0 && 0 < (_M = epptr() - pptr()))
                {if (_N < _M)
                    _M = _N;
                traits_type::copy(pptr(), _S, _M);
                _S += _M, _Ns += _M, _N -= _M, pbump(_M); }
            else if (traits_type::eq_int_type(traits_type::eof(),
                overflow(traits_type::to_int_type(*_S))))
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
    virtual exe_basic_streambuf* __thiscall setbuf(char_type* buf, exe_streamsize size)
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
    char_type* _Gbeg;
    // offset: +0x8 = +8,  size = 0x4 = 4
    char_type* _Pbeg;
    // offset: +0xC = +12,  size = 0x4 = 4
    char_type** _IGbeg;
    // offset: +0x10 = +16,  size = 0x4 = 4
    char_type** _IPbeg;
    // offset: +0x14 = +20,  size = 0x4 = 4
    char_type* _Gnext;
    // offset: +0x18 = +24,  size = 0x4 = 4
    char_type* _Pnext;
    // offset: +0x1C = +28,  size = 0x4 = 4
    char_type** _IGnext;
    // offset: +0x20 = +32,  size = 0x4 = 4
    char_type** _IPnext;
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

typedef exe_basic_streambuf<char, std::char_traits<char> > exe_streambuf;

#pragma pack(push, 4)
//
// size = 0x54 = 84, align = 4, baseclass: std::streambuf
template<class CharT, class CharTraits = std::char_traits<CharT> >
NH3API_VIRTUAL_CLASS exe_basic_filebuf : public exe_basic_streambuf<CharT, CharTraits>
{
    protected:
        typedef exe_basic_filebuf<CharT, CharTraits> this_type;
        typedef exe_basic_streambuf<CharT, CharTraits> base_type;

    public:
        typedef HANDLE native_handle_type;
        typedef typename base_type::char_type char_type;
        typedef typename base_type::int_type  int_type;
        typedef typename base_type::pos_type  pos_type;
        typedef typename base_type::off_type  off_type;
        typedef typename base_type::traits_type traits_type;
        enum _Initfl { _Newfl, _Openfl, _Closefl }; // init flag

    public:
        NH3API_FORCEINLINE
        exe_basic_filebuf(exe_FILE* _F = nullptr) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(exe_basic_filebuf)
        { THISCALL_2(void, 0x60D3CF, this, _F); }

        NH3API_FORCEINLINE
        exe_basic_filebuf(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : base_type(tag)
        {}
        
        NH3API_FORCEINLINE
        ~exe_basic_filebuf() NH3API_NOEXCEPT
        { get_type_vftable(this)->scalar_deleting_destructor(this, 0); }

    public:
        bool is_open() const NH3API_NOEXCEPT
        { return static_cast<bool>(_File); }

        this_type* open(const char *_S, exe_ios::openmode _M) NH3API_NOEXCEPT
        {
            exe_FILE* _Fp;
            if (_File != nullptr || (_Fp = CDECL_2(exe_FILE*, 0x60CA8E, _S, _M)) == nullptr)
                return nullptr;
            THISCALL_3(void, 0x48D170, this, _Fp, _Openfl);
            THISCALL_1(void, 0x48D260, this);
            return this;
        }

        this_type* close() NH3API_NOEXCEPT
        {
            if (_File != nullptr && exe_fclose(_File) == 0)
            {
                THISCALL_3(void, 0x48D170, this, nullptr, _Closefl);
                return this;
            }
            else
            {
                return nullptr;
            }
        }

        // C++23
        native_handle_type native_handle() const NH3API_NOEXCEPT
        {
            assert(is_open());
            return CDECL_1(HANDLE, 0x6222B3, exe_fileno(_File));
        }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_STREAMBUF(exe_basic_filebuf)

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
        void* _Loc;
        // offset: +0x50 = +80,  size = 0x4 = 4
        exe_FILE* _File;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x50 = 80, align = 4, baseclass: std::streambuf
NH3API_VIRTUAL_CLASS exe_strstreambuf : public exe_streambuf
{
    protected:
        typedef exe_strstreambuf this_type;
        typedef exe_streambuf    base_type;

    // note: 'typename' not required
    public:
        typedef base_type::char_type char_type;
        typedef base_type::int_type  int_type;
        typedef base_type::pos_type  pos_type;
        typedef base_type::off_type  off_type;
        typedef base_type::traits_type traits_type;

    public:
        NH3API_FORCEINLINE
        explicit exe_strstreambuf(exe_streamsize _N = 0) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(exe_strstreambuf)
        { THISCALL_2(void, 0x4512D0, this, _N); }

        NH3API_FORCEINLINE
        exe_strstreambuf(const char *_G, exe_streamsize _N) NH3API_NOEXCEPT
        NH3API_DELEGATE_DUMMY(exe_strstreambuf)
        { THISCALL_3(void, 0x5167C0, this, _G, _N); }

        NH3API_FORCEINLINE
        exe_strstreambuf(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : base_type(tag)
        {}
           
        NH3API_FORCEINLINE
        ~exe_strstreambuf() NH3API_NOEXCEPT
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

typedef exe_basic_filebuf<char, std::char_traits<char> > exe_filebuf;
NH3API_SPECIALIZE_TYPE_VFTABLE(0x6456B8, exe_streambuf)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x645820, exe_filebuf)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x645680, exe_strstreambuf)

NH3API_FORCEINLINE
exe_streamsize read(exe_streambuf& stream, void* buf, size_t len)
{ return stream.sgetn(static_cast<char*>(buf), len); }

NH3API_FORCEINLINE
exe_streamsize write(exe_streambuf& stream, const void* buf, size_t len)
{ return stream.sputn(static_cast<const char*>(buf), len); }

NH3API_DISABLE_WARNING_END
