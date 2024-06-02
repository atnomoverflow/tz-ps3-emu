#include "thread.hpp"

void CellThread::task()
{

    while (true)
    {
        std::unique_lock lock(m_mutex);
        switch (m_event)
        {
        case TZPS3EMU_EVENT_PAUSE:
            m_status = TZPS3EMU_STATUS_PAUSED;
            m_cv.wait(lock, [&]
                      { return m_event == TZPS3EMU_EVENT_RUN });
            m_status = TZPS3EMU_STATUS_RUNNING;
            // fall over to the run event handler
        case TZPS3EMU_EVENT_RUN:
            m_translator->step();
            break;
        case TZPS3EMU_EVENT_STOP:
            return;
        default:
            m_event = TZPS3EMU_EVENT_NONE;
            break;
        }
    }
}

void CellThread::start()
{
    // if somone tryed to start the thread again when it's alread started
    if (m_thread)
    {
        stop();
    }

    m_thread = new std::thread([&]()
                               {
        m_event = TZPS3EMU_EVENT_PAUSE;
        task(); });
}

void CellThread::run()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_event = TZPS3EMU_EVENT_RUN;
    m_cv.notify_one();
}

void CellThread::pause()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_event = TZPS3EMU_EVENT_PAUSE;
}

void CellThread::stop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_event = TZPS3EMU_EVENT_STOP;
}
