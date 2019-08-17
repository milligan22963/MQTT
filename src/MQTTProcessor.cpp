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
                    success = true;
                } else {
                    m_connection = nullptr;
                }
            }

            return success;
        }

        bool MQTTProcessor::sendPacket(const IMQTTPacketSPtr pPacket)
        {
            bool success = false;

            SocketBuffer data;

            if (pPacket->encodePacket(data) == true) {
                success = m_connection->write(data);
            }
            return success;
        }

        bool MQTTProcessor::shutdown()
        {
            m_listeners.clear();

            if (m_connection != nullptr) {
                m_connection->removeListener(shared_from_this());
                m_connection->shutdown();
                m_connection = nullptr;
            }

            return true;
        }

        void MQTTProcessor::addListener(IMQTTProcessListenerSPtr pListener)
        {
            m_listeners.push_back(pListener);
        }

        void MQTTProcessor::removeListener(IMQTTProcessListenerSPtr pListener)
        {
            m_listeners.remove(pListener);
        }

        void MQTTProcessor::onConnected()
        {
            // Inform of socket level connection
            for (auto listener : m_listeners) {
                listener->onConnected();
            }
        }

        void MQTTProcessor::onDataReceived(const SocketBuffer &socketBuffer)
        {
            // convert this buffer to a message and pass it to the listener(s)
            IMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(socketBuffer);

            if (pPacket != nullptr) {
                for (auto listener : m_listeners) {
                    listener->onMessageReceived(pPacket);
                }
            } else {
                // failed
            }
        }

        void MQTTProcessor::onDataWritten(const SocketBuffer &socketBuffer)
        {
            IMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(socketBuffer);

            if (pPacket != nullptr) {
                for (auto listener : m_listeners) {
                    listener->onMessageDelivered(pPacket);
                }
            } else {
                // failed
            }
        }

        void MQTTProcessor::onError(int socketError)
        {
            for (auto listener : m_listeners) {
                listener->onError();
            }
        }

        void MQTTProcessor::onDisconnected()
        {
            for (auto listener : m_listeners) {
                listener->onDisconnected();
            }
        }
    }
}
