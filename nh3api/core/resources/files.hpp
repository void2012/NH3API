//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include <array>                          // std::array
#include <memory>                         // std::destroy_at

#include "../nh3api_std/exe_vector.hpp"   // exe_vector
#include "../nh3api_std/exe_streambuf.hpp"

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(26495)

#ifndef NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE
#define NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE(CLASS_NAME)\
void __thiscall scalar_deleting_destructor(uint8_t flag) override\
{ get_type_vftable(this)->scalar_deleting_destructor(this, flag); }\
int32_t __thiscall read(void* buf, size_t len) override\
{ return get_type_vftable(this)->read(this, buf, len); }\
int32_t __thiscall write(const void* buf, size_t len) override\
{ return get_type_vftable(this)->write(this, buf, len); }
#endif

#pragma pack(push, 4)
// Abstract file class
// size = 0x4 = 4, align = 4
NH3API_VIRTUAL_CLASS TAbstractFile
{
    public:
        struct vftable_t
        {
            void    (__thiscall* scalar_deleting_destructor)(TAbstractFile*, uint8_t);
            int32_t (__thiscall* read)(TAbstractFile*, void*, size_t);
            int32_t (__thiscall* write)(TAbstractFile*, const void*, size_t);
        };

    public:
        TAbstractFile() noexcept
        { NH3API_SET_VFTABLE(); }

        TAbstractFile(const TAbstractFile&)            noexcept = default;
        TAbstractFile(TAbstractFile&&)                 noexcept = default;
        TAbstractFile& operator=(const TAbstractFile&) noexcept = default;
        TAbstractFile& operator=(TAbstractFile&&)      noexcept = default;

        TAbstractFile(const nh3api::omit_base_vftable_tag_t&) noexcept
        {}

        TAbstractFile(const nh3api::dummy_tag_t&) noexcept
        {}

        ~TAbstractFile() noexcept = default;

    // virtual functions
    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual int32_t __thiscall read(void* buf, size_t len) = 0;

        // vftable shift: +8
        virtual int32_t __thiscall write(const void* buf, size_t len) = 0;

};

// Zlib: compress buffer <source> of length <sourceLen> and write result to <dest> of length <destLen> /
// Zlib: Сжать область памяти <source> длиной <sourceLen> и записать результат в область памяти <dest> длиной <destLen>.
inline int32_t exe_compress(void* dest, uint32_t destLen, const void* source, uint32_t sourceLen) noexcept
{ return FASTCALL_5(int32_t, 0x607100, dest, destLen, source, sourceLen, 6); }

// Zlib: Uncompress buffer <source> of length <sourceLen> and write result to <dest> of length <destLen> /
// Zlib: Распаковать область памяти <source> длиной <sourceLen> и записать результат в область памяти <dest> длиной <destLen>.
inline int32_t exe_uncompress(void* dest, uint32_t destLen, const void* source, uint32_t sourceLen) noexcept
{ return FASTCALL_4(int32_t, 0x6071A0, dest, destLen, source, sourceLen); }

// zlib file /
// zlib файл.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS TGzFile : public TAbstractFile
{
    public:
        TGzFile(const char* __restrict const path, const char* __restrict const mode = "rb") noexcept
            : TAbstractFile(nh3api::omit_base_vftable_tag)
        { THISCALL_3(void, 0x4D6EB0, this, path, mode); }

        TGzFile(const nh3api::dummy_tag_t& tag) noexcept
            : TAbstractFile(tag)
        {}

        ~TGzFile() noexcept
        { THISCALL_1(void, 0x4D6FC0, this); }

        TGzFile(const TGzFile&)            noexcept = default;
        TGzFile(TGzFile&&)                 noexcept = default;
        TGzFile& operator=(const TGzFile&) noexcept = default;
        TGzFile& operator=(TGzFile&&)      noexcept = default;

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

// Streambuf file /
// Файл чтения с помощью streambuf.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS TStreamBufFile : public TAbstractFile
{
    public:
        TStreamBufFile(const TStreamBufFile&)            noexcept = default;
        TStreamBufFile(TStreamBufFile&&)                 noexcept = default;
        TStreamBufFile& operator=(const TStreamBufFile&) noexcept = default;
        TStreamBufFile& operator=(TStreamBufFile&&)      noexcept = default;

        TStreamBufFile(exe_streambuf* src_stream) noexcept
            : TAbstractFile(nh3api::omit_base_vftable_tag), stream{src_stream}
        { NH3API_SET_VFTABLE(); }

        TStreamBufFile(const nh3api::dummy_tag_t& tag) noexcept
            : TAbstractFile(tag)
        {}

        ~TStreamBufFile() noexcept = default;

    // virtual functions
    public:
        NH3API_VIRTUAL_OVERRIDE_TABSTRACTFILE(TStreamBufFile)

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        exe_streambuf* stream;

};

// zlib stream buffer /
// Поток чтения gz-файла.
// size = 0x84 = 132, align = 4, baseclass: std::streambuf
NH3API_VIRTUAL_CLASS TGzInflateBuf : public exe_streambuf
{
    public:
        inline TGzInflateBuf(exe_streambuf& src) noexcept(false)
            : exe_streambuf(nh3api::dummy_tag)
        { THISCALL_2(void, 0x4D6260, this, &src); }

        inline TGzInflateBuf(const nh3api::dummy_tag_t& tag) noexcept
            : exe_streambuf(tag)
        {}

    public:
        [[nodiscard]] bool is_compressed() const noexcept
        { return m_is_compressed; }

        [[nodiscard]] uint32_t crc() const noexcept
        { return m_crc; }

        [[nodiscard]] bool is_open() const noexcept
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
        std::array<std::byte, 56> m_zstream;

    public:
        // Input buffer /
        // Буффер ввода.
        // offset: +0x74 = +116,  size = 0x4 = 4
        void* m_input_buffer;

        // Output buffer /
        // Буффер вывода.
        // offset: +0x78 = +120,  size = 0x4 = 4
        void* m_output_buffer;

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

// LOD file entry /
// Запись LOD-файла.
// size = 0x20 = 32, align = 4
struct LODEntry
{
    // Entry name /
    // Название записи.
    // offset: +0x0 = +0,  size = 0x10 = 16
    std::array<char, 16> name {};

    // Entry offset /
    // Смещение записи
    // offset: +0x10 = +16,  size = 0x4 = 4
    ptrdiff_t offset {0};

    // Entry uncompressed size /
    // Размер несжатой записи.
    // offset: +0x14 = +20,  size = 0x4 = 4
    size_t size {0};

    //
    // offset: +0x18 = +24,  size = 0x4 = 4
    int32_t attrib {0};

    // Entry compressed size /
    // Размер сжатой записи.
    // offset: +0x1C = +28,  size = 0x4 = 4
    size_t csize {0};
};

// LOD file header /
// Заголовок LOD файла.
// size = 0x5C = 92, align = 4
struct LODHeader
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    std::array<char, 4> LOD_ID {};

    // LOD File version /
    // Версия LOD-файла.
    // offset: +0x4 = +4,  size = 0x4 = 4
    int32_t version {500};

    // Number of entries /
    // Количество записей.
    // offset: +0x8 = +8,  size = 0x4 = 4
    size_t numEntries {0};

protected:
    // offset: +0xC = +12,  size = 0x50 = 80
    std::array<std::byte, 80> reserved {};

};

// LOD File /
// LOD Файл.
// size = 0x18C = 396, align = 4
class LODFile
{
    public:
        enum class openmode : int32_t
        {
            read_write = 0,
            read       = 1
        };

    public:
        inline LODFile() noexcept
            : subindex{}
        {}

        inline LODFile(const LODFile& other)
            : subindex { other.subindex }
        { std::memcpy(reinterpret_cast<void*>(this), &other, 380 /*__builtin_offsetof(LODFile, subindex)*/); }

        inline LODFile(LODFile&& other) noexcept
        {
            std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
            std::memset(static_cast<void*>(&other), 0, sizeof(*this));
        }

        inline LODFile& operator=(const LODFile& other)
        {
            if ( this != &other )
            {
                std::memcpy(reinterpret_cast<void*>(this), &other, 380 /*__builtin_offsetof(LODFile, subindex)*/);
                this->subindex = other.subindex;
            }

            return *this;
        }

        inline LODFile& operator=(LODFile&& other) noexcept
        {
            if ( this != &other )
            {
                std::destroy_at(&this->subindex);
                std::memcpy(static_cast<void*>(this), static_cast<void*>(&other), sizeof(*this));
                std::memset(static_cast<void*>(&other), 0, sizeof(*this));
            }

            return *this;
        }

        inline LODFile(const nh3api::dummy_tag_t& tag) noexcept
            : subindex { tag }
        {}

        inline ~LODFile() noexcept
        { THISCALL_1(void, 0x4FAE90, this); }

    public:
        LODEntry* getItemIndex(const char* const item_name) noexcept
        { return THISCALL_2(LODEntry*, 0x4FACA0, this, item_name); }

        bool      exist(const char* const item_name) noexcept
        {
            if ( item_name )
            {
                Find(0, static_cast<size_t>(numEntries), item_name);
                return matchindex >= 0;
            }

            return false;

        }

        void      Find(uint32_t begin, uint32_t end, const char* const item_name) noexcept
        { THISCALL_4(void, 0x4FACF0, this, begin, end, item_name); }

        int32_t   open(const char* const filename, openmode flags = openmode::read) noexcept
        { return THISCALL_3(int32_t, 0x4FAF30, this, filename, static_cast<int32_t>(flags)); }

        bool      pointAt(const char* const itemName) noexcept
        { return THISCALL_2(bool, 0x4FB100, this, itemName); }

        int32_t   read(void* dest, size_t numBytes) noexcept
        { return THISCALL_3(int32_t, 0x4FB1B0, this, dest, numBytes); }

    public:
        // File pointer /
        // Указатель файла.
        // offset: +0x0 = +0,  size = 0x4 = 4
        exe_FILE* fileptr {nullptr};

        // Lod file name /
        // Название файла LOD.
        // offset: +0x4 = +4,  size = 0x100 = 256
        std::array<char, 256> LODFileName {};

        // FILE stream is open /
        // Открыт поток файла.
        // offset: +0x104 = +260,  size = 0x4 = 4
        bool32_t opened {false};

        // offset: +0x108 = +264,  size = 0x4 = 4
        void* dataBuffer {nullptr};

        // offset: +0x10C = +268,  size = 0x4 = 4
        size_t dataBufferSize {0};

        // offset: +0x110 = +272,  size = 0x4 = 4
        int32_t dataItemIndex {0};

        // offset: +0x114 = +276,  size = 0x4 = 4
        ptrdiff_t dataPos {0};

        // offset: +0x118 = +280,  size = 0x4 = 4
        int32_t matchindex {0};

        // offset: +0x11C = +284,  size = 0x5C = 92
        LODHeader header {};

        // offset: +0x178 = +376,  size = 0x4 = 4
        size_t numEntries {0};

        union {
        // offset: +0x17C = +380,  size = 0x10 = 16
        exe_vector<LODEntry> subindex;
        };
};

inline LODFile* GetLODFile(const char* const name) noexcept
{
    if ( name == nullptr )
        return nullptr;

    auto& lodfiles = get_global_var_ref(0x69D8A8, std::array<std::pair<const char*, LODFile>, 5>);
    for ( auto& lodfile : lodfiles )
        if ( _stricmp(name, lodfile.first) == 0 )
            return &lodfile.second;

    return nullptr;
}

// LOD File stream /
// Поток чтения из LOD.
// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS t_lod_file_adapter final : public TAbstractFile
{
    public:
        t_lod_file_adapter(LODFile* src) noexcept
            : TAbstractFile(nh3api::dummy_tag), lod_file{src}
        { NH3API_SET_VFTABLE(); }

        t_lod_file_adapter(LODFile& src) noexcept
            : TAbstractFile(nh3api::dummy_tag), lod_file{&src}
        { NH3API_SET_VFTABLE(); }

        t_lod_file_adapter(const char* name) noexcept
            : TAbstractFile(nh3api::dummy_tag), lod_file{GetLODFile(name)}
        { NH3API_SET_VFTABLE(); }

    public:
        void set_lod(LODFile* newfile) noexcept
        { lod_file = newfile; }

        [[nodiscard]] const LODFile* get_lod() const noexcept
        { return lod_file; }

        [[nodiscard]] LODFile* get_lod() noexcept
        { return lod_file; }

        [[nodiscard]] bool is_open() const noexcept
        {
            if (lod_file == nullptr)
                return false;

            return lod_file->opened;
        }

        // get entry by name
        LODEntry* get_entry(const char* const name) noexcept
        {
            if ( name && lod_file->pointAt(name) && lod_file && lod_file->dataItemIndex >= 0 )
                return &lod_file->subindex[static_cast<size_t>(lod_file->dataItemIndex)];

            return nullptr;
        }

        // read LOD File entry into buffer
        [[nodiscard]] exe_vector<std::byte> read_entry(const char* const name)
        {
            exe_vector<std::byte> buffer;
            if ( lod_file == nullptr || name == nullptr || !lod_file->pointAt(name) )
                return buffer; // not found, empty buffer

            if ( lod_file->dataItemIndex < 0 )
                return buffer; // not found, empty buffer

            LODEntry* const entry = &lod_file->subindex[static_cast<size_t>(lod_file->dataItemIndex)];
            const size_t entry_size = static_cast<size_t>(entry->size);
            if ( entry_size == 0 )
                return buffer; // entry is empty

            buffer.resize(entry_size);
            read(buffer.data(), entry_size);
            return buffer;
        }

        void seek(ptrdiff_t offset) noexcept
        { exe_fseek(lod_file->fileptr, offset, SEEK_SET); }

    // virtual functions
    public:
        void __thiscall scalar_deleting_destructor(uint8_t flag) override
        { get_type_vftable(this)->scalar_deleting_destructor(this, flag); }

        int32_t __thiscall read(void *buf, size_t len) override
        { return get_type_vftable(this)->read(this, buf, len); }

    protected:
        int32_t __thiscall write(const void*, size_t) override
        { return 0; }

    // member variables
    public:
        // offset: +0x4 = +4,  size = 0x4 = 4
        LODFile* lod_file;

};

// size = 0x8 = 8, align = 4, baseclass: TAbstractFile
NH3API_VIRTUAL_CLASS t_stdio_file_adapter final : public TAbstractFile
{
    public:
        t_stdio_file_adapter(const char* const filename, const char* const mode) noexcept
            : file { exe_fopen(filename, mode), exe_fcloser {} }
        { NH3API_SET_VFTABLE(); }

        ~t_stdio_file_adapter() noexcept = default;
        t_stdio_file_adapter(const t_stdio_file_adapter&)            noexcept = delete;
        t_stdio_file_adapter(t_stdio_file_adapter&&)                 noexcept = default;
        t_stdio_file_adapter& operator=(const t_stdio_file_adapter&) noexcept = delete;
        t_stdio_file_adapter& operator=(t_stdio_file_adapter&&)      noexcept = default;

    public:
        void seek(ptrdiff_t offset)
        { exe_fseek(file.get(), offset, SEEK_SET); }

        exe_FILE* get_file() noexcept
        { return file.get(); }

        [[nodiscard]] const exe_FILE* get_file() const noexcept
        { return file.get(); }

        void reset_file(exe_FILE* ptr = nullptr) noexcept
        { file.reset(ptr); }

    public:
        void __thiscall scalar_deleting_destructor(uint8_t flag) override
        { get_type_vftable(this)->scalar_deleting_destructor(this, flag); }

        int32_t __thiscall read(void* buf, size_t len) override
        {
            if ( buf && len && file )
                return static_cast<int32_t>(exe_fread(buf, len, 1, file.get()));

            return 0;
        }

        int32_t __thiscall write(const void* buf, size_t len) override
        {
            if ( buf && len && file )
                return static_cast<int32_t>(exe_fwrite(buf, len, 1, file.get()));

            return 0;
        }

    protected:
        // offset: +0x4 = +4,  size = 0x4 = 4
        exe_unique_file file;

};
#pragma pack(pop) // 4

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63DAC0, TAbstractFile)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63E74C, TGzFile)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63DACC, TStreamBufFile)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x63E710, TGzInflateBuf)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x641138, t_lod_file_adapter)

NH3API_WARNING(pop)
