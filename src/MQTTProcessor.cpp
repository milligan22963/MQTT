/**
 * @brief MQTTProcessor
 * 
 * Class for MQTT basic MQTTProcessing
 * 
 */

#include <iostream>
#include <unistd.h>

#include "MQTTPacketOptions.h"
#include "MQTTProcessor.h"
#include "JSONUtil.h"

namespace afm {
    namespace communications {
        MQTTProcessor::MQTTProcessor()
            : m_threadRunning(false)
        {

        }

        MQTTProcessor::~MQTTProcessor()
        {
            shutdown();
        }

        bool MQTTProcessor::initialize(const MQTTOptions &options)
        {
            bool success = false;
            std::string brokerURL;
            uint16_t port;

            if (extractValue(options, sc_brokerURL, brokerURL) == true) {
                if (extractValue(options, sc_brokerPort, port) == true) {
                    m_threadRunning = true;
                    m_processingThread = std::thread(&MQTTProcessor::processing, this);
                    success = true;
                } else {
                    std::cout << "Failed to extract prt\n";
                }
            } else {
                std::cout << "Failed to extract url\n";
            }

            return success;
        }

        bool MQTTProcessor::shutdown()
        {
            bool success = false;

            if (m_threadRunning != false) {
                std::cout << "Shutting down thread\n";
                m_threadRunning = false;
                m_processingThread.join();
                success = true;
            }
            return success;
        }

        // internals
        void MQTTProcessor::processing()
        {
            while (m_threadRunning == true) {
                sleep(1); // for testing
            }
        }
    }
}
