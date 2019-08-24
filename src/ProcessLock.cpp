/**
 * @brief ProcessLock
 * 
 * Class for aquiring a lock and wait for event
 * 
 */

#include "ProcessLock.h"

namespace afm {
    namespace common {
        ProcessLock::ProcessLock()
        {

        }

        ProcessLock::~ProcessLock()
        {

        }

        void ProcessLock::lock()
        {
            m_processMutex.lock();
        }

        void ProcessLock::unLock()
        {
            m_processMutex.unlock();
        }

        void ProcessLock::wake()
        {
            m_processFlag.notify_one();
        }

        void ProcessLock::wait()
        {
            std::unique_lock<std::mutex> lk(m_processMutex);

            m_processFlag.wait(lk);
        }

        std::cv_status ProcessLock::waitFor(std::chrono::steady_clock::duration waitTime)
        {
            std::unique_lock<std::mutex> lk(m_processMutex);

            return m_processFlag.wait_for(lk, waitTime);
        }

        std::cv_status ProcessLock::waitUntil(std::chrono::system_clock::time_point waitTime)
        {
            std::unique_lock<std::mutex> lk(m_processMutex);

            return m_processFlag.wait_until(lk, waitTime);
        }

        std::cv_status ProcessLock::waitUntil(std::chrono::steady_clock::time_point waitTime)
        {
            std::unique_lock<std::mutex> lk(m_processMutex);

            return m_processFlag.wait_until(lk, waitTime);
        }
    }
}
