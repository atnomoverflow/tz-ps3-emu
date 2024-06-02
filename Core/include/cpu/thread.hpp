#pragma once

#include "translator.hpp"
#include "emulator.hpp"

#include <mutex>
#include <condition_variable>
#include <string>
#include <thread>
class CellThread
{
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::string name;
    std::thread *m_thread;
    EmulatorEvent m_event;
    EmulatorStatus m_status;
    void task();

protected:
    CellTranslator *m_translator;

public:
    void start();

    void run();
    void pause();
    void stop();
};
