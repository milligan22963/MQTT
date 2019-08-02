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
#include "afmSocketFactory.h"
#include "JSONUtil.h"

namespace afm {
    namespace communications {

        const std::string sc_processorType = "type";
        const std::string sc_mqttClient = "client";
        const std::string sc_mqttServer = "server";

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

            std::string processorType;
            if (extractValue(options, sc_processorType, processorType) == true) {
                if (processorType == sc_mqttClient) {
                    m_connection = AfmSocketFactory::getInstance()->createSocket(eSocketType::eClientSocket);
                } else {
                    m_connection = AfmSocketFactory::getInstance()->createSocket(eSocketType::eServerSocket);
                }
                if (m_connection->initialize(options) == true) {
                    m_threadRunning = true;
                    m_processingThread = std::thread(&MQTTProcessor::processing, this);
                    success = true;
                } else {
                    m_connection = nullptr;
                }
            }

            return success;
        }

        bool MQTTProcessor::shutdown()
        {
            bool success = false;

            if (m_connection != nullptr) {
                m_connection->shutdown();
                m_connection = nullptr;
            }

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
