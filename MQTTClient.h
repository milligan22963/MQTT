/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#ifndef _H_MQTTCLIENT
#define _H_MQTTCLIENT

#include "IMQTTClient.h"
#include "MQTTProcessor.h"

namespace afm {
    namespace communications {
        class MQTTClient : public std::enable_shared_from_this<MQTTClient>
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options, IMQTTClientSPtr pListener);
                virtual bool sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos);
                virtual void shutdown();

            private:
                IMQTTClientSPtr m_pListener = nullptr;
                MQTTProcessorSPtr m_pProcessor = nullptr;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif