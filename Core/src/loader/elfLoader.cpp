#include "loader/elfLoader.hpp"
#include "emulator.hpp"
bool BaseLoader::open(const std::string &path)
{
    vfsLocalFile file;
    if (!file.Open(path, vfsRead))
    {
        return false;
    }
    m_size = file.GetSize();
    m_content = new char[m_size];
    file.Seek(0, vfsSeekSet);
    file.Read(m_content, m_size);
    return true;
}

u16 ElfLoader::getMachine()
{
    if (!m_content)
        return 0;
    const ELF_Ehdr &ehdr = (ELF_Ehdr &)m_content[0];
    return ehdr.machine;
}

u32 ElfLoader::getEntry()
{
    if (!m_content)
        return 0;
    const ELF_Ehdr &ehdr = (ELF_Ehdr &)m_content[0];

    return ehdr.entry;
}

ElfLoader::ElfLoader(const std::string &path)
{
    open(path);
}
void ElfLoader::close()
{
    delete[] m_content;
    m_size = 0;
}
ElfLoader::~ElfLoader()
{
    close();
}
bool ElfLoader::load()
{
    if (!m_content)
        return false;
    const ELF_Ehdr &ehdr = (ELF_Ehdr &)m_content[0];
    // for each program header
    for (u64 i = 0; i < ehdr.phnum; i++)
    {
        const Elf_Phdr &phdr = (Elf_Phdr &)m_content[ehdr.phoff + i * sizeof(Elf_Phdr)];

        switch (phdr.type.ToLE())
        {
        case 0x00000001:
            if (phdr.memsz == 0)
                break;
            // the memsz might be greater than filesz
            // because the program might store values there later on
            tzPs3Emu.memory(SEG_MAIN_MEMORY).allocFixed(phdr.vaddr, phdr.memsz);
            std::memcpy(tzPs3Emu.memory[phdr.vaddr], &m_content[phdr.offset], phdr.filesz);
            break;
        default:
            break;
        }
    }

    return true;
}