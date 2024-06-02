
#pragma once

#include "common/common.hpp"
#include <mutex>
#include <vector>

#define PAGE_4K(x) (((x) + 4095) & ~(4095))

struct MemoryBlock
{
    u32 addr;
    u32 size;
    void *realaddr;
    MemoryBlock(u32 block_addr, u32 block_size);
};

class MemorySegment
{
private:
    u32 m_start;
    u32 m_size;
    std::mutex m_mutex;
    std::vector<MemoryBlock> m_allocated;

public:
    MemorySegment();
    MemorySegment(u32 start, u32 size);
    ~MemorySegment();

    void init(u32 start, u32 size);
    void close();

    u32 alloc(u32 size, u32 align);
    bool allocFixed(u32 addr, u32 size);
    void free(u32 addr);

    bool isValid(u32 addr);
    u32 getUsedMemory();
};
