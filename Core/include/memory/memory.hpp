#pragma once

#include "memorySegment.hpp"
#include <common/common.hpp>

#ifdef TZ_PS3_EMU_WIN
#include <Windows.h>
#endif
#ifdef TZ_PS3_EMU_LINUX
#include <sys/mman.h>
#endif
#ifdef TZ_PS3_EMU_MACOS
#include <sys/mman.h>
#define MAP_ANONYMOUS MAP_ANON
#endif

constexpr unsigned long long FOUR_GB = 0x100000000ULL;
enum
{
    // Memory segments
    SEG_MAIN_MEMORY = 0,  // 0x00010000 to 0x2FFFFFFF
    SEG_PRX_MEMORY,       // 0x30000000 to 0x3FFFFFFF
    SEG_RSX_FIFO_MEMORY,  // 0x40000000 to 0x4FFFFFFF
    SEG_MMAPPER_MEMORY,   // 0xB0000000 to 0xBFFFFFFF
    SEG_RSX_LOCAL_MEMORY, // 0xC0000000 to 0xCFFFFFFF
    SEG_STACK,            // 0xD0000000 to 0xDFFFFFFF

    // Count of memory segments
    SEG_COUNT,
};

class Memory
{

    void *m_base;
    MemorySegment m_segment[SEG_COUNT];

public:
    void init();
    void close();

    void alloc();
    void check();
    void free();

    // read
    u8 read8(u32 addr);
    u16 read16(u32 addr);
    u32 read32(u32 addr);
    u64 read64(u32 addr);
    u128 read128(u32 addr);
    void readLeft(u8 *dst, u32 src, u32 size);
    void readRight(u8 *dst, u32 src, u32 size);

    // write
    void write8(u32 addr, u8 value);
    void write16(u32 addr, u16 value);
    void write32(u32 addr, u32 value);
    void write64(u32 addr, u64 value);
    void write128(u32 addr, u128 value);
    void writeLeft(u32 dst, u8 *src, u32 size);
    void writeRight(u32 dst, u8 *src, u32 size);

    void *getBaseMemory() { return m_base; }

    MemorySegment &operator()(size_t id) { return m_segment[id]; }
    void *operator[](u32 addr) { return (void *)((u64)m_base + addr); }
    void *operator+(u32 addr) { return (void *)((u64)m_base + addr); }
};