#pragma once

#include "memory/memory.hpp"
#include <string>
enum EmulatorStatus
{
    TZPS3EMU_STATUS_UNKOWN,
    TZPS3EMU_STATUS_RUNNING,
    TZPS3EMU_STATUS_PAUSED,
    TZPS3EMU_STATUS_STOPPED

};

enum EmulatorEvent
{
    TZPS3EMU_EVENT_NONE,
    TZPS3EMU_EVENT_RUN,
    TZPS3EMU_EVENT_PAUSE,
    TZPS3EMU_EVENT_STOP
};

class Emulator
{
public:
    Memory memory;
    bool load(const std::string &path);
    void run();
    void pause();
    void stop();
};

extern Emulator tzPs3Emu;