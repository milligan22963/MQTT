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

        }

        bool MQTTClient::initialize(IMQTTClientSPtr pListener)
        {
            bool success = false;

            m_pListener = pListener;

            return success;
        }
    }
}