/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#ifndef _H_MQTTCLIENT
#define _H_MQTTCLIENT

#include "IMQTTListener.h"
#include "IMQTTProcessorListener.h"
#include "MQTTProcessor.h"

namespace afm {
    namespace communications {
        enum MQTTState
        {
            eMQTTConnection_Requested,
            eMQTTConnection_Success,
            eMQTTConnection_Failed,
            eMQTTSubscription_Requested,
            eMQTTSubscription_Success,
            eMQTTSubscription_Failed,
            eMQTTDisconnect_Requested,
            eMQTTDisconnect_Success,
            eMQTTDisconnect_Failed,
            eEndMQTTStates
        };

        class MQTTClient : public IMQTTProcessListener, public std::enable_shared_from_this<MQTTClient>
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener);
                virtual bool sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos);
                virtual void shutdown();

                /**
                 * IMQTTProcessorListener interface implementation
                 */
                virtual void onConnected() override;
                virtual void onMessageReceived(const IMQTTPacketSPtr pPacket) override;
                virtual void onMessageDelivered(const IMQTTPacketSPtr pPacket) override;
                virtual void onDisconnected() override;
                virtual void onError() override;

            private:
                IMQTTListenerSPtr   m_pListener = nullptr;
                MQTTProcessorSPtr   m_pProcessor = nullptr;
                MQTTState           m_currentState = MQTTState::eEndMQTTStates;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif