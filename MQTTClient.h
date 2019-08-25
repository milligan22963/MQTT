/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#ifndef _H_MQTTCLIENT
#define _H_MQTTCLIENT

#include <atomic>
#include <map>
#include <thread>
#include "IMQTTListener.h"
#include "IMQTTProcessorListener.h"
#include "MQTTProcessor.h"
#include "ProcessLock.h"

namespace afm {
    namespace communications {
        enum MQTTState
        {
            eMQTTConnection_Requested,
            eMQTTConnection_Success,
            eMQTTConnection_Failed,
            eMQTTConnection_Active,
            eMQTTDisconnect_Requested,
            eMQTTDisconnect_Success,
            eMQTTDisconnect_Failed,
            eEndMQTTStates
        };

        using MQTTMessageQueue = std::map<uint16_t, IMQTTPacketSPtr>;

        class MQTTClient : public IMQTTProcessListener, public std::enable_shared_from_this<MQTTClient>
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener);

                virtual bool addSubscription(const MQTTSubscription &subscription);
                virtual bool subscribe(bool subscribeOnConnect);

                virtual bool removeSubscription(const MQTTSubscription &subscription);
                virtual bool unsubscribe();
                virtual bool sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message,
                                         MQTT_QOS qos, bool retain);
                virtual void shutdown();

                /**
                 * IMQTTProcessorListener interface implementation
                 */
                virtual void onConnected() override;
                virtual void onMessageReceived(const IMQTTPacketSPtr pPacket) override;
                virtual void onMessageDelivered(const IMQTTPacketSPtr pPacket) override;
                virtual void onDisconnected() override;
                virtual void onError() override;

            protected:
                void queueOutgoingMessage(IMQTTPacketSPtr pMessage);
                void queueIncomingMessage(IMQTTPacketSPtr pMessage);
                void sendConnect();
                void sendKeepAlive();
                void process();

                void persistMessage(IMQTTPacketSPtr pMessage);
                void restoreMessages();

            private:
                std::string                     m_clientId;
                std::string                     m_persistencePath;
                uint32_t                        m_maxBacklog = 1000;
                uint16_t                        m_keepAliveTime = 0;
                std::atomic<bool>               m_keepProcessing;
                std::thread                     m_stateProcessor;   
                uint64_t                        m_nextMessageId = 0;
                std::vector<MQTTSubscription>   m_subscriptionsToAdd;
                std::vector<MQTTSubscription>   m_subscriptionsToRemove;
                IMQTTListenerSPtr               m_pListener = nullptr;
                MQTTProcessorSPtr               m_pProcessor = nullptr;
                std::atomic<MQTTState>          m_currentState;
                std::atomic<bool>               m_isConnected;
                std::atomic<bool>               m_subscribeOnConnect;
                common::ProcessLock             m_lock;
                MQTTMessageQueue                m_outgoingMessageQueue;
                MQTTMessageQueue                m_incomingMessageQueue;
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif