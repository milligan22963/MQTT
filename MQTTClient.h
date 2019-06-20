/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#ifndef _H_MQTTCLIENT
#define _H_MQTTCLIENT

#include "IMQTTListener.h"
#include "MQTTProcessor.h"

namespace afm {
    namespace communications {
        class MQTTClient : public std::enable_shared_from_this<MQTTClient>
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener);
                virtual bool sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos);
                virtual void shutdown();

            private:
                IMQTTListenerSPtr m_pListener = nullptr;
                MQTTProcessorSPtr m_pProcessor = nullptr;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif