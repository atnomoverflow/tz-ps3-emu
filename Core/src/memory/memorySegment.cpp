#include "memory/memorySegment.hpp"
#include "emulator.hpp"
#if defined(TZ_PS3_EMU_WIN)
#include <Windows.h>
#elif defined(TZ_PS3_EMU_LINUX) || defined(TZ_PS3_EMU_MACOS)
#include <sys/mman.h>
#endif
#include <cstring>


MemoryBlock::MemoryBlock(u32 block_addr, u32 block_size)
{
    size = PAGE_4K(block_size);
    addr = block_addr;
    realaddr = (void *)((u64)tzPs3Emu.memory.getBaseMemory() + block_addr);

#if defined(TZ_PS3_EMU_WIN)
    if (VirtualAlloc(realaddr, size, MEM_COMMIT, PAGE_READWRITE) != realaddr)
    {
#elif defined(TZ_PS3_EMU_LINUX) || defined(TZ_PS3_EMU_MACOS)
    if (::mprotect(realaddr, block_size, PROT_READ | PROT_WRITE))
    {
#endif
        // Error
        realaddr = nullptr;
        return;
    }
    memset(realaddr,0,size);
}

MemorySegment::MemorySegment(/* args */)
{
}
MemorySegment::MemorySegment(u32 start, u32 size)
{
    init(start, size);
}
void MemorySegment::init(u32 start, u32 size)
{
    close();
    m_start = 0;
    m_size = 0;
}
u32 MemorySegment::alloc(u32 size, u32 align)
{
    size = PAGE_4K(size);
    u32 exsize;
    if (align <= 4096)
    {
        align = 0;
        exsize = 0;
    }
    else
    {
        align &= ~4095;
        exsize = size - align - 1;
    }
    std::lock_guard<std::mutex> lock(m_mutex);

    for (u32 addr = m_start; addr <= m_start + m_size - exsize;)
    {
        for (const auto block : m_allocated)
        {
            if ((block.addr <= addr && addr < block.addr + block.size) ||
                (addr <= block.addr && block.addr < addr + exsize))
            {
                addr = block.addr + block.size;
                continue;
            }
        }

        if (align)
        {
            addr = (addr + (align - 1)) & ~(align - 1);
        }
        m_allocated.emplace_back(addr, size);
        return addr;
    }
    return 0;
}
bool MemorySegment::allocFixed(u32 addr, u32 size)
{
    size = PAGE_4K(size + (addr + 4095));
    addr &= ~4095;
    if (!isValid(addr) || !isValid(addr + size - 1))
        return false;
    std::lock_guard<std::mutex> lock(m_mutex);

    for (const auto block : m_allocated)
    {
        if ((block.addr <= addr && addr < block.addr + block.size) || (addr <= block.addr && block.addr < addr + size))
            return false;
    }
    m_allocated.emplace_back(addr, size);
    return true;
}

bool MemorySegment::isValid(u32 addr)
{
    if (addr < m_start && m_start + m_size <= addr)
        return false;
    return true;
}

MemorySegment::~MemorySegment()
{
    close();
}

