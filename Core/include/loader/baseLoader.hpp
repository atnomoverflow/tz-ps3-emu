#pragma once

#include "common/common.hpp"
#include "filesystem/vfsLocalFile.hpp"
#include <string>
#include <memory>
enum FileType
{

    FILETYPE_ELF,
    FILETYPE_SELF,

    FILETYPE_UNKOWN
};

typedef struct
{
    be_t<u32> magic;     // Must be "SCE\0".
    be_t<u32> version;   // 2 for PS3, 3 for PS Vita.
    be_t<u16> attribute; // Corresponds to the revision of the encryption key. The Certified File Key ID is derived from the attribute.
    be_t<u16> category;
    be_t<u32> ext_header_size;
    be_t<u64> file_offset; // Offset to encapsulated data.
    be_t<u64> file_size;   // Size of the encapsulated data.
} ELF_ext_hdr;

typedef struct
{
    /**
     * ELF identifier
     */
    be_t<u32> magic;
    be_t<u8> elf_class;
    be_t<u8> data;
    be_t<u8> cur_ver;
    be_t<u8> os_abi;
    be_t<u64> abi_ver;

    be_t<u16> type;      // Sony Computer entertaiment types
    be_t<u16> machine;   // machine type
    be_t<u32> version;   // file version
    be_t<u32> entry;     // entry point address for the program
    be_t<u32> phoff;     // program header offset
    be_t<u32> shoff;     // section header offset
    be_t<u32> flags;     // process specific flags  TO_BE_RESEARCHED:  dont know what's this yet !!!
    be_t<u16> ehsize;    // elf header size
    be_t<u16> phentsize; // size of programm header entry
    be_t<u16> phnum;     // number of programm header entries
    be_t<u16> shentsize; // size of section header entry
    be_t<u16> shentnum;  // number of section header entries
    be_t<u16> shstrndx;  // section header string table index  TO_BE_RESEARCHED: WTF is this
} ELF_Ehdr;
typedef struct
{
    be_t<u32> type;   /* Segment type */
    be_t<u32> flags;  /* Segment flags */
    be_t<u64> offset; /* Segment file offset */
    be_t<u64> vaddr;  /* Segment virtual address */
    be_t<u64> paddr;  /* Segment physical address */
    be_t<u64> filesz; /* Segment size in file */
    be_t<u64> memsz;  /* Segment size in memory */
    be_t<u64> align;  /* Segment alignment */
} Elf_Phdr;

enum ELF_TYPE
{
    ET_SCE_EXEC = 0xFE00,        /* SCE Executable - PRX2 */
    ET_SCE_RELEXEC = 0xFE04,     /* SCE Relocatable Executable - PRX2 */
    ET_SCE_STUBLIB = 0xFE0C,     /* SCE SDK Stubs */
    ET_SCE_DYNEXEC = 0xFE10,     /* SCE EXEC_ASLR (PS4 Executable with ASLR) */
    ET_SCE_DYNAMIC = 0xFE18,     /* ? */
    ET_SCE_IOPRELEXEC = 0xFF80,  /* SCE IOP Relocatable Executable */
    ET_SCE_IOPRELEXEC2 = 0xFF81, /* SCE IOP Relocatable Executable Version 2 */
    ET_SCE_EERELEXEC = 0xFF90,   /* SCE EE Relocatable Executable */
    ET_SCE_EERELEXEC2 = 0xFF91,  /* SCE EE Relocatable Executable Version 2 */
    ET_SCE_PSPRELEXEC = 0xFFA0,  /* SCE PSP Relocatable Executable */
    ET_SCE_PPURELEXEC = 0xFFA4,  /* SCE PPU Relocatable Executable */
    ET_SCE_ARMRELEXEC = 0xFFA5,  /* ?SCE ARM Relocatable Executable (PS Vita System Software earlier or equal 0.931.010) */
    ET_SCE_PSPOVERLAY = 0xFFA8   /* ? */
};

class BaseLoader
{
protected:
    char *m_content;
    size_t m_size;

public:
    virtual bool open(const std::string &path) = 0;
    virtual bool load() = 0;
    virtual void close() = 0;
};
