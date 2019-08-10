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
#include "MQTTFactory.h"
#include "JSONUtil.h"

namespace afm {
    namespace communications {

        const std::string sc_processorType = "type";
        const std::string sc_mqttClient = "client";
        const std::string sc_mqttServer = "server";

        MQTTProcessor::MQTTProcessor()
            : m_threadRunning(false)
            , m_disconnectExpected(false)
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
                    m_connection->addListener(shared_from_this());
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

            m_listeners.clear();

            if (m_connection != nullptr) {
                m_connection->removeListener(shared_from_this());
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

        void MQTTProcessor::addListener(IMQTTListenerSPtr &pListener)
        {
            m_listeners.push_back(pListener);
        }

        void MQTTProcessor::removeListener(IMQTTListenerSPtr &pListener)
        {
            m_listeners.remove(pListener);
        }

        void MQTTProcessor::onConnected()
        {
            for (auto listener : m_listeners)
            {
                listener->onConnected(true);
            }
        }

        void MQTTProcessor::onDataReceived(const SocketBuffer &socketBuffer)
        {
            // convert this buffer to a message and pass it to the listener(s)
            IMQTTPacketSPtr pPacket = nullptr;

            for (auto listener : m_listeners) {
                listener->onMessageReceived(pPacket);
            }
        }

        void MQTTProcessor::onDataWritten(const SocketBuffer &socketBuffer)
        {
            IMQTTPacketSPtr pPacket = nullptr;

            for (auto listener : m_listeners) {
                listener->onMessageDelivered(pPacket);
            }
        }

        void MQTTProcessor::onError(int socketError)
        {
            onDisconnected();
        }

        void MQTTProcessor::onDisconnected()
        {
            // disconnect is state for mqtt so
            // we want to differentiate between a requested
            // disconnect opposed to a unexpected one
            if (m_disconnectExpected == false) {
                for (auto listener : m_listeners) {
                    listener->onConnected(false);
                }
            } else {
                m_disconnectExpected = false; // shouldn't always expect it
                for (auto listener : m_listeners) {
                    listener->onDisconnected(true);
                }
            }
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
