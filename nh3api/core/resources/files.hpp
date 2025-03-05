//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

// #include <stdexcept> // std::runtime_error
// #include <cstring>  // string functions
// #include <algorithm> // std::sort
// #include <cstdio> // std::FILE
#include "../nh3api_std/exe_vector.hpp"    // exe_vector<T>
#include "../nh3api_std/exe_streambuf_vftables.hpp" // exe_streambuf<C>

NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

#ifndef NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE
#define NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE(CLASS_NAME)\
virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
virtual int32_t __thiscall read(void* buf, size_t len) override\
{ return get_type_vftable(this)->read(this, buf, len); }\
virtual int32_t __thiscall write(const void* buf, size_t len) override\
{ return get_type_vftable(this)->write(this, buf, len); }
#endif

// Abstract file class
NH3API_VIRTUAL_CLASS TAbstractFile
{
    public:
        struct vftable_t
        {
            void (__thiscall *scalar_deleting_destructor)(TAbstractFile*, uint8_t);
            int32_t (__thiscall *read)(TAbstractFile*, void*, size_t);
            int32_t (__thiscall *write)(TAbstractFile*, const void*, size_t);
        };

    public:
        TAbstractFile()
        { NH3API_SET_VFTABLE(); }

        TAbstractFile(const nh3api::omit_base_vftable_tag_t& tag) NH3API_NOEXCEPT
        {}

        TAbstractFile(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {}

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall read(void* buf, size_t len) = 0;

        // vftable shift: +8
        virtual int32_t __thiscall write(const void* buf, size_t len) = 0;

};

NH3API_FORCEINLINE
// address: 0x607100
// Compress buffer <source> of length <sourceLen> and write result to <dest> of length <destLen> /
// Сжать область памяти <source> длиной <sourceLen> и записать результат в область памяти <dest> длиной <destLen>.
int32_t exe_compress(uint8_t* dest, uint32_t destLen, const uint8_t* source, uint32_t sourceLen) NH3API_NOEXCEPT
{ return FASTCALL_5(int32_t, 0x607100, dest, destLen, source, sourceLen, 6); }

NH3API_FORCEINLINE
// address: 0x6071A0
// Uncompress buffer <source> of length <sourceLen> and write result to <dest> of length <destLen> /
// Распаковать область памяти <source> длиной <sourceLen> и записать результат в область памяти <dest> длиной <destLen>.
int32_t exe_uncompress(uint8_t* dest, uint32_t destLen, const uint8_t* source, uint32_t sourceLen) NH3API_NOEXCEPT
{ return FASTCALL_4(int32_t, 0x6071A0, dest, destLen, source, sourceLen); }

#pragma pack(push, 4)
// zlib file /
// zlib файл.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS TGzFile : public TAbstractFile
{
    public:
        TGzFile(const char* path, const char* mode)
            : TAbstractFile(nh3api::omit_base_vftable_tag)
        { THISCALL_3(void, 0x4D6EB0, this, path, mode); }

        TGzFile(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : TAbstractFile(tag)
        {}

        ~TGzFile()
        { THISCALL_1(void, 0x4D6FC0, this); }

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE(TGzFile)

    // member variables
    public:
        // zlib file handle /
        // указатель на zlib файл.
        // offset: +0x4 = +4,  size = 0x4 = 4
        void* file;

};
#pragma pack(pop)

#pragma pack(push, 4)
// Streambuf file /
// Файл чтения с помощью streambuf.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS TStreamBufFile : public TAbstractFile
{
    public:
        TStreamBufFile(exe_streambuf* src_stream)
            : TAbstractFile(nh3api::omit_base_vftable_tag), stream(src_stream)
        { NH3API_SET_VFTABLE(); }

        TStreamBufFile(const nh3api::dummy_tag_t& tag)
            : TAbstractFile(tag)
        {}

        ~TStreamBufFile()
        {}

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE(TStreamBufFile)

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        exe_streambuf* stream;

};
#pragma pack(pop)

#pragma pack(push, 4)
// zlib stream buffer /
// Поток чтения gz-файла.
// size = 0x84 = 132, align = 4, baseclass: std::streambuf
NH3API_VIRTUAL_CLASS TGzInflateBuf : public exe_streambuf
{
    public:
        NH3API_FORCEINLINE
        TGzInflateBuf(exe_streambuf& src) NH3API_NOEXCEPT
            : exe_streambuf(nh3api::dummy_tag)
        { THISCALL_2(void, 0x4D6260, this, &src); }

        NH3API_FORCEINLINE
        TGzInflateBuf(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
            : exe_streambuf(tag)
        {}

    public:
        bool is_compressed() const
        { return m_is_compressed; }

        uint32_t crc() const
        { return m_crc; }

        bool is_open() const
        { return m_open && m_stream_is_open; }

    public:
        NH3API_VIRTUAL_OVERRIDE_STREAMBUF(TGzInflateBuf)

    protected:
        // Parent streambuf /
        // Родительский streambuf.
        // offset: +0x38 = +56,  size = 0x4 = 4
        exe_streambuf* m_source;

    protected:
        // zlib stream /
        // zlib поток.
        // offset: +0x3C = +60,  size = 0x38 = 56
        std::array<uint8_t, 0x38> m_zstream;

    public:
        // Input buffer /
        // Буффер ввода.
        // offset: +0x74 = +116,  size = 0x4 = 4
        uint8_t* m_input_buffer;

        // Output buffer /
        // Буффер вывода.
        // offset: +0x78 = +120,  size = 0x4 = 4
        uint8_t* m_output_buffer;

        // offset: +0x7C = +124,  size = 0x4 = 4
        uint32_t m_crc;

        // offset: +0x80 = +128,  size = 0x1 = 1
        bool m_is_compressed;

        // offset: +0x81 = +129,  size = 0x1 = 1
        bool m_end_of_file;

        // offset: +0x82 = +130,  size = 0x1 = 1
        bool m_stream_is_open;

        // offset: +0x83 = +131,  size = 0x1 = 1
        bool m_open;

};
#pragma pack(pop)

#pragma pack(push, 4)
// LOD file entry /
// Запись LOD-файла.
// size = 0x20 = 32, align = 4
struct LODEntry
{
public:
    NH3API_FORCEINLINE
    LODEntry() NH3API_NOEXCEPT
    { memset_0(this, sizeof(*this)); }

    NH3API_FORCEINLINE
    LODEntry(const nh3api::dummy_tag_t&) NH3API_NOEXCEPT
    {}

public:
    // Entry name /
    // Название записи.
    // offset: +0x0 = +0,  size = 0x10 = 16
    std::array<char, 16> name;

    // Entry offset /
    // Смещение записи
    // offset: +0x10 = +16,  size = 0x4 = 4
    int32_t offset;

    // Entry uncompressed size /
    // Размер несжатой записи.
    // offset: +0x14 = +20,  size = 0x4 = 4
    int32_t size;

    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t attrib;

    // Entry compressed size /
    // Размер сжатой записи.
    // offset: +0x1C = +28,  size = 0x4 = 4
    int32_t csize;

};
#pragma pack(pop)

#pragma pack(push, 4)
// LOD file header /
// Заголовок LOD файла.
// size = 0x5C = 92, align = 4
struct LODHeader
{
public:
    LODHeader()
    : version(500), numEntries(0)
    { LOD_ID.fill('\0'); }

public:
    // offset: +0x0 = +0,  size = 0x4 = 4
    std::array<char, 4> LOD_ID;

    // LOD File version /
    // Версия LOD-файла.
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t version;

    // Number of entries /
    // Количество записей.
    // offset: +0x8 = +8,  size = 0x4 = 4
    int32_t numEntries;

protected:
    // offset: +0xC = +12,  size = 0x50 = 80
    std::array<char, 80> reserved;

};
#pragma pack(pop)

#pragma pack(push, 4)
// LOD File /
// LOD Файл.
// size = 0x18C = 396, align = 4
class LODFile
{
    public:
        LODFile() NH3API_NOEXCEPT
        :
        fileptr(nullptr),
        opened(0),
        dataBuffer(nullptr),
        header(),
        subindex()
        {}

        ~LODFile() NH3API_NOEXCEPT
        { THISCALL_1(void, 0x4FAE90, this); }

    public:
        LODEntry* getItemIndex(char const* item_name) NH3API_NOEXCEPT
        { return THISCALL_2(LODEntry*, 0x4FACA0, this, item_name); }

        bool      exist(char const* item_name) NH3API_NOEXCEPT
        {
            Find( 0, numEntries, item_name );
            return matchindex >= 0;
        }

        void      Find(uint32_t begin, uint32_t end, const char* item_name) NH3API_NOEXCEPT
        { THISCALL_4(void, 0x4FACF0, this, begin, end, item_name); }

        int32_t   open(char const* filename, uint32_t flags) NH3API_NOEXCEPT
        { return THISCALL_3(int32_t, 0x4FAF30, this, filename, flags); }

        bool      pointAt(char const* itemName) NH3API_NOEXCEPT
        { return THISCALL_2(bool, 0x4FB100, this, itemName); }

        int32_t   read(void* dest, int32_t numBytes) NH3API_NOEXCEPT
        { return THISCALL_3(int32_t, 0x4FB1B0, this, dest, numBytes); }

    public:
        // File pointer /
        // Указатель файла.
        // offset: +0x0 = +0,  size = 0x4 = 4
        exe_FILE* fileptr;

        // Lod file name /
        // Название файла LOD.
        // offset: +0x4 = +4,  size = 0x100 = 256
        std::array<char, 256> LODFileName;

        // FILE stream is open /
        // Открыт поток файла.
        // offset: +0x104 = +260,  size = 0x4 = 4
        bool32_t opened;

        // offset: +0x108 = +264,  size = 0x4 = 4
        uint8_t* dataBuffer;

        // offset: +0x10C = +268,  size = 0x4 = 4
        uint32_t dataBufferSize;

        // offset: +0x110 = +272,  size = 0x4 = 4
        int32_t dataItemIndex;

        // offset: +0x114 = +276,  size = 0x4 = 4
        int32_t dataPos;

        // offset: +0x118 = +280,  size = 0x4 = 4
        int32_t matchindex;

        // offset: +0x11C = +284,  size = 0x5C = 92
        LODHeader header;

        // offset: +0x178 = +376,  size = 0x4 = 4
        int32_t numEntries;

        // offset: +0x17C = +380,  size = 0x10 = 16
        exe_vector<LODEntry> subindex;

};
#pragma pack(pop)

NH3API_FORCEINLINE
LODFile* GetLODFile(const char* name) NH3API_NOEXCEPT
{
    if ( !name )
        return nullptr;
    const size_t name_size = strlen(name);
    if ( name_size == 0 )
        return nullptr;

    typedef std::array<std::pair<const char*, LODFile>, 8> lod_files_array_t;
    lod_files_array_t& lodfiles = get_global_var_ref(0x69D8A8, lod_files_array_t);
    
    for (lod_files_array_t::iterator it = lodfiles.begin();
         it != lodfiles.end();
         ++it) 
    {
        if (std::memcmp(name, it->first, name_size) == 0)
            return &it->second;
    }

    return nullptr;
}

#pragma pack(push, 4)
// LOD File stream /
// Поток чтения из LOD.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS t_lod_file_adapter NH3API_FINAL : public TAbstractFile
{
    public:
        t_lod_file_adapter(LODFile* src) NH3API_NOEXCEPT
            : TAbstractFile(nh3api::dummy_tag), lod_file(src)
        { NH3API_SET_VFTABLE(); }

        t_lod_file_adapter(LODFile& src) NH3API_NOEXCEPT
            : TAbstractFile(nh3api::dummy_tag), lod_file(&src)
        { NH3API_SET_VFTABLE(); }

        t_lod_file_adapter(const char* name) NH3API_NOEXCEPT
            : TAbstractFile(nh3api::dummy_tag), lod_file(GetLODFile(name))
        { NH3API_SET_VFTABLE(); }

    public:
        void set_lod(LODFile* newfile) NH3API_NOEXCEPT
        { lod_file = newfile; }

        const LODFile* get_lod() const NH3API_NOEXCEPT
        { return lod_file; }

        LODFile* get_lod() NH3API_NOEXCEPT
        { return lod_file; }

        bool is_open() const  NH3API_NOEXCEPT
        { 
            if (lod_file == nullptr)
                return false;
            else 
                return lod_file->opened;
        }

        // get entry by name
        LODEntry* get_entry(const char* name) NH3API_NOEXCEPT
        {
            if (lod_file->pointAt(name))
                return &lod_file->subindex[lod_file->dataItemIndex];
            else 
                return nullptr;
        }

        // read LOD File entry into buffer
        exe_vector<uint8_t> read_entry(const char* name)
        {
            if (!lod_file->pointAt(name))
                return exe_vector<uint8_t>(); // not found, empty buffer

            LODEntry* entry = &lod_file->subindex[lod_file->dataItemIndex];
            const size_t entry_size = entry->size;
            exe_vector<uint8_t> buffer(entry_size, 0);
            read(buffer.data(), entry_size);
            return buffer;
        }

        void seek(int32_t offset) NH3API_NOEXCEPT
        { exe_fseek(lod_file->fileptr, offset, EXE_SEEK_SET); }

    // virtual functions
    public:
        virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override
        { get_type_vftable(this)->scalar_deleting_destructor(this, flag); }

        virtual int32_t __thiscall read(void *buf, size_t len) override
        { return get_type_vftable(this)->read(this, buf, len); }

    protected:
        virtual int32_t __thiscall write(const void *buf, size_t len) override 
        { return 0; }

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        LODFile* lod_file;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS t_stdio_file_adapter NH3API_FINAL : public TAbstractFile
{
    public:
        t_stdio_file_adapter(const char* filename, const char* mode) NH3API_NOEXCEPT
            : file(exe_fopen(filename, mode))
        {
            NH3API_SET_VFTABLE();
        }

        ~t_stdio_file_adapter() NH3API_NOEXCEPT
        { exe_fclose(file); }

        void seek(int32_t offset)
        { exe_fseek(file, offset, EXE_SEEK_SET); }

    public:
        virtual void __thiscall scalar_deleting_destructor(uint8_t flag) override
        { get_type_vftable(this)->scalar_deleting_destructor(this, flag); }

        virtual int32_t __thiscall read(void* buf, size_t len) override
        { return exe_fread(buf, len, 1, file); }

        virtual int32_t __thiscall write(const void* buf, size_t len) override
        { return exe_fwrite(buf, len, 1, file); }

    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        exe_FILE* file;

};
#pragma pack(pop)

NH3API_DISABLE_WARNING_END
