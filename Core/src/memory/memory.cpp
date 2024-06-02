#include <memory/memory.hpp>

void Memory::init()
{
#if defined(TZ_PS3_EMU_WIN32)
    m_base = VirtualAlloc(nullptr, FOUR_GB, MEM_RESERVE, PAGE_NOACCESS);
#elif defined(TZ_PS3_EMU_LINUX) || defined(TZ_PS3_EMU_MACOS)
    m_base = mmap(nullptr, FOUR_GB, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
#endif
    if (m_base == 0 || m_base == (void *)-1)
        // TODO : missing error handling if i am not wrong
        // great learning i geuss
        return;
    m_segment[SEG_MAIN_MEMORY].init(0x00010000, 0x2FFF0000);
    m_segment[SEG_PRX_MEMORY].init(0x30000000, 0x1000000);
    m_segment[SEG_RSX_FIFO_MEMORY].init(0x40000000, 0x1000000);
    m_segment[SEG_MMAPPER_MEMORY].init(0xB0000000, 0x10000000);
    m_segment[SEG_RSX_LOCAL_MEMORY].init(0xC0000000, 0x1000000);
    m_segment[SEG_STACK].init(0xD0000000, 0x1000000);
}
void Memory::close()
{
 // TODO unmap for win / linux & macos
}

// read
u8 Memory::read8(u32 addr)
{
    return *(u8 *)((u64)m_base + addr);
}
u16 Memory::read16(u32 addr)
{
    return re16(*(u16 *)((u64)m_base + addr));
}
u32 Memory::read32(u32 addr)
{
    return re32(*(u32 *)((u64)m_base + addr));
}
u64 Memory::read64(u32 addr)
{
    return re64(*(u64 *)((u64)m_base + addr));
}
u128 Memory::read128(u32 addr)
{
    return re128(*(u128 *)((u64)m_base + addr));
}
void Memory::readLeft(u8 *dst, u32 src, u32 size)
{
    for (u32 i = 0; i < size; i++)
    {
        dst[i] = read8(src + i);
    }
}
void Memory::readRight(u8 *dst, u32 src, u32 size)
{
    for (u32 i = size - 1; i >= 0; i--)
    {
        dst[i] = read8(src + i);
    }
}

// write
void Memory::write8(u32 addr, u8 value)
{
    *(u8 *)((u64)m_base + addr) = value;
}
void Memory::write16(u32 addr, u16 value)
{
    *(u16 *)((u64)m_base + addr) = value;
}
void Memory::write32(u32 addr, u32 value)
{
    *(u32 *)((u64)m_base + addr) = value;
}
void Memory::write64(u32 addr, u64 value)
{
    *(u64 *)((u64)m_base + addr) = value;
}
void Memory::write128(u32 addr, u128 value)
{
    *(u128 *)((u64)m_base + addr) = value;
}
void Memory::writeLeft(u32 dst, u8 *src, u32 size)
{
    for (u32 i = 0; i < size; i++)
    {
        write8(dst + i, src[size - 1 - i]);
    }
}
void Memory::writeRight(u32 dst, u8 *src, u32 size)
{
    for (u32 i = size - 1; i >= 0; i--)
    {
        write8(dst + i, src[size - 1 - i]);
    }
}


void Memory::alloc()
{
}
void Memory::check()
{
}
void Memory::free()
{
}