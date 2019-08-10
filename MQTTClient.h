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
        class MQTTClient : public IMQTTListener, std::enable_shared_from_this<MQTTClient>
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener);
                virtual bool sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos);
                virtual void shutdown();

                /**
                 * IMQTTListener interface implementation
                 */
                virtual void onConnected(bool success) override;
                virtual void onSubscriptionAdded(bool success) override;
                virtual void onSubscriptionRemoved(bool success) override;
                virtual void onDisconnected(bool success) override;
                virtual void onMessageReceived(const IMQTTPacketSPtr pPacket) override;
                virtual void onMessageDelivered(const IMQTTPacketSPtr pPacket) override;

            private:
                IMQTTListenerSPtr m_pListener = nullptr;
                MQTTProcessorSPtr m_pProcessor = nullptr;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif