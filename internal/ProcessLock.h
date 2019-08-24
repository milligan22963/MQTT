/**
 * @brief ProcessLock
 * 
 * Class for aquiring a lock and wait for event
 * 
 */

#ifndef _H_PROCESSLOCK
#define _H_PROCESSLOCK

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

namespace afm {
    namespace common {
        class ProcessLock
        {
            public:
                ProcessLock();
                virtual ~ProcessLock();

                /**
                 * @brief Locks the mutex to syncrhonize operation
                 */
                void lock();

                /**
                 * @brief Unlocks the mutex
                 */
                void unLock();

                void wake();

                /**
                 * @brief pauses the process while waiting for something to happen
                 */
                void wait();
                std::cv_status waitFor(std::chrono::steady_clock::duration waitTime);
                std::cv_status waitUntil(std::chrono::system_clock::time_point waitTime);
                std::cv_status waitUntil(std::chrono::steady_clock::time_point waitTime);

            private:
                std::mutex                      m_processMutex;
                std::condition_variable         m_processFlag;
        };
    }
}
#endif