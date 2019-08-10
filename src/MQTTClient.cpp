/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#include "MQTTClient.h"

namespace afm {
    namespace communications {
        MQTTClient::MQTTClient()
        {

        }

        MQTTClient::~MQTTClient()
        {
            shutdown();
        }

        bool MQTTClient::initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener)
        {
            bool success = false;

            if (pListener != nullptr) {
                m_pListener = pListener;
                m_pProcessor = std::make_shared<MQTTProcessor>();

                MQTTOptions clientOptions = options;

                clientOptions[sc_processorType] = sc_mqttClient;

                if (m_pProcessor->initialize(clientOptions) == true) {
                    success = true;
                } else {
                    m_pProcessor->shutdown();
                    m_pProcessor = nullptr;
                }
            }
            return success;
        }

        bool MQTTClient::sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos)
        {
            bool success = false;

            return success;
        }

        void MQTTClient::shutdown()
        {
            if (m_pProcessor != nullptr) {
                m_pProcessor->shutdown();
                m_pProcessor = nullptr;
            }
            m_pListener = nullptr;
        }

        void MQTTClient::onConnected(bool success)
        {
            if (success == true) {
                // send out any backlogged data
            }
        }

        void MQTTClient::onSubscriptionAdded(bool success)
        {

        }

        void MQTTClient::onSubscriptionRemoved(bool success)
        {

        }

        void MQTTClient::onDisconnected(bool success)
        {
            // start backlogging
        }

        void MQTTClient::onMessageReceived(IMQTTPacketSPtr pPacket)
        {

        }

        void MQTTClient::onMessageDelivered(IMQTTPacketSPtr pPacket)
        {

        }
    }
}