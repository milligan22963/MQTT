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

        bool MQTTClient::initialize(const MQTTOptions &options, IMQTTClientSPtr pListener)
        {
            bool success = false;

            if (pListener != nullptr) {
                m_pListener = pListener;
                m_pProcessor = std::make_shared<MQTTProcessor>();

                if (m_pProcessor->initialize(options) == true) {
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
    }
}