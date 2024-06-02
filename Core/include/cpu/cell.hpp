#pragma once

#include "thread.hpp"
#include <vector>
enum CellThreadType
{
    CELL_THREAD_PPU,
    CELL_THREAD_SPU,

}

class Cell
{
    std::vector<CellThread *> m_threads;
    std::mutex m_mutex;

public:
    CellThread &createThread(CellThreadType type);
    removeThread(u32 id);

    void run();
    void pause();
    void stop();
};
