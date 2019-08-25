/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#include <iostream>

#include "JSONUtil.h"
#include "MQTTClient.h"
#include "MQTTFactory.h"
#include "MQTTConnectAckPacket.h"
#include "MQTTSubscribeAckPacket.h"

namespace afm {
    namespace communications {
        MQTTClient::MQTTClient()
            : m_keepProcessing(false)
            , m_currentState(MQTTState::eEndMQTTStates)
            , m_isConnected(false)
            , m_subscribeOnConnect(false)
        {

        }

        MQTTClient::~MQTTClient()
        {
            shutdown();
        }

        bool MQTTClient::initialize(const MQTTOptions &options, IMQTTListenerSPtr pListener)
        {
            bool success = false;

            m_pProcessor = std::make_shared<MQTTProcessor>();

            extractValue(options, sc_keepAlive, m_keepAliveTime);
            extractValue(options, sc_clientId, m_clientId);

            // pull out persistence object and then path and backlog...
            MQTTOptions persistence;
            if (extractValue(options, sc_persistence, persistence) == true) {
                extractValue(options, sc_persistencePath, m_persistencePath);
                extractValue(options, sc_persistenceBacklog, m_maxBacklog);
            }

            MQTTOptions clientOptions = options;

            clientOptions[sc_processorType] = sc_mqttClient;

            if (m_pProcessor->initialize(clientOptions) == true) {
                m_pProcessor->addListener(shared_from_this());
                m_pListener = pListener;

                // restore packets 
                m_keepProcessing = true;
                m_stateProcessor = std::thread(&MQTTClient::process, shared_from_this());

                success = true;
            } else {
                m_pProcessor->shutdown();
                m_pProcessor = nullptr;
            }
            return success;
        }

        bool MQTTClient::addSubscription(const MQTTSubscription &subscription)
        {
            m_subscriptionsToAdd.push_back(subscription);

            return true;
        }

        bool MQTTClient::removeSubscription(const MQTTSubscription &subscription)
        {
            m_subscriptionsToRemove.push_back(subscription);

            return true;
        }

        bool MQTTClient::subscribe(bool subscribeOnConnect)
        {
            bool success = false;

            m_subscribeOnConnect = subscribeOnConnect;

            if (m_isConnected == true) {
                MQTTOptions subscribeOptions;

                subscribeOptions[sc_optionMessageId] = (uint16_t)m_nextMessageId++;
                subscribeOptions[sc_subscriptions] = MQTTOptions::array();
                for (auto subscription : m_subscriptionsToAdd) {
                    subscribeOptions[sc_subscriptions].push_back(
                        {
                            {sc_topic, subscription.topic},
                            {sc_qosLevel, subscription.qos}
                        }
                    );
                }

                IMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_Subscribe);

                if (pPacket->initialize(subscribeOptions) == true) {
                    m_pProcessor->sendPacket(pPacket);
                    success = true;
                }
            } else {
                success = subscribeOnConnect; // true if we are doing it later
            }
            return success;
        }

        bool MQTTClient::unsubscribe()
        {
            bool success = false;

            if (m_isConnected == true) {
                MQTTOptions unsubscribeOptions;

                unsubscribeOptions[sc_optionMessageId] = (uint16_t)m_nextMessageId++;
                unsubscribeOptions[sc_subscriptions] = MQTTOptions::array();
                for (auto subscription : m_subscriptionsToRemove) {
                    unsubscribeOptions[sc_subscriptions].push_back(
                        {
                            {sc_topic, subscription.topic}
                        }
                    );
                }

                IMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_UnSubscribe);

                if (pPacket->initialize(unsubscribeOptions) == true) {
                    m_pProcessor->sendPacket(pPacket);
                    success = true;
                }

            }
            return success;
        }

        bool MQTTClient::sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos, bool retain)
        {
            bool success = false;

            MQTTOptions messageOptions;

            messageOptions[sc_optionMessageId] = (uint16_t)m_nextMessageId++;
            messageOptions[sc_topic] = topic;
            messageOptions[sc_message] = message;
            messageOptions[sc_qosLevel] = qos;
            messageOptions[sc_retainFlag] = retain;

            IMQTTPacketSPtr pPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_Publish);

            if (pPacket->initialize(messageOptions) == true) {
                // Good packet if qos > 0 we need to persist it
                if (qos > MQTT_QOS::MQTT_QOS_0) {
                    queueOutgoingMessage(pPacket);
                }
                if (m_isConnected == true) {
                    m_pProcessor->sendPacket(pPacket);
                    success = true;
                }
            }
            return success;
        }

        void MQTTClient::shutdown()
        {
            if (m_keepProcessing == true) {
                m_keepProcessing = false;
                m_lock.wake();

                m_stateProcessor.join();
            }

            if (m_pProcessor != nullptr) {
                m_pProcessor->shutdown();
                m_pProcessor = nullptr;
            }

            m_subscriptionsToAdd.clear();
            m_subscriptionsToRemove.clear();
        }

        void MQTTClient::onConnected()
        {
            sendConnect();
            m_lock.wake();
        }

        void MQTTClient::onMessageReceived(IMQTTPacketSPtr pPacket)
        {
            // check state to see if we are waiting on something...such as a connection request etc
            switch (pPacket->getType()) {
                case MQTTPacketType::MQTT_ConnectAck:
                {
                    MQTTConnectAckPacketSPtr pAckPacket = std::dynamic_pointer_cast<MQTTConnectAckPacket>(pPacket);

                    MQTTState newState = MQTTState::eMQTTConnection_Failed;

                    if (m_currentState == MQTTState::eMQTTConnection_Requested) {
                        if (pAckPacket != nullptr) {
                            if (pAckPacket->getResponse() == ConnectionResponse::eConnectionResponse_Accepted) {
                                newState = MQTTState::eMQTTConnection_Success;
                                m_isConnected = true;
                            }
                        }
                    }

                    if (m_pListener != nullptr) {
                        m_pListener->onConnected(newState == MQTTState::eMQTTConnection_Success);
                    }
                    m_currentState = newState;
                }
                break;
                case MQTTPacketType::MQTT_SubscribeAck:
                {
                    bool subscribeSuccess = false;

                    MQTTSubscribeAckPacketSPtr pAckPacket = std::dynamic_pointer_cast<MQTTSubscribeAckPacket>(pPacket);

                    if (pAckPacket != nullptr) {
                        subscribeSuccess = pAckPacket->subscriptionSuccess();
                    }

                    if (m_pListener != nullptr) {
                        m_pListener->onSubscriptionAdded(subscribeSuccess);
                    }
                }
                break;
                case MQTTPacketType::MQTT_UnSubscribeAck:
                {
                    if (m_pListener != nullptr) {
                        m_pListener->onSubscriptionRemoved(true);
                    }
                }
                break;
                case MQTTPacketType::MQTT_Publish:
                {
                    IMQTTPacketSPtr pAckPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_PublishAck);

                    if (m_pListener != nullptr) {
                        m_pListener->onMessageReceived(pPacket);
                    }

                    // send ack if qos > 0
                }
                break;
                case MQTTPacketType::MQTT_PublishAck:
                {
                    // for QOS 1 they have it, we are done
                    queueOutgoingMessage(pPacket);
                }
                break;
                case MQTTPacketType::MQTT_PublishReceive:
                {
                    // for qos 2 they have it, we need to keep tracking
                    queueOutgoingMessage(pPacket);
                }
                break;
                case MQTTPacketType::MQTT_PublishRelease:
                {
                    queueIncomingMessage(pPacket);
                }
                break;
                case MQTTPacketType::MQTT_PublishComplete:
                {
                    queueOutgoingMessage(pPacket);
                }
                break;
                case MQTTPacketType::MQTT_PingResponse:
                {
                    // glad they got it
                }
                break;
                default:
                {
                    // unhandled at the moment
                }
                break;
            }
            m_lock.wake();
        }

        void MQTTClient::onMessageDelivered(IMQTTPacketSPtr pPacket)
        {

        }

        void MQTTClient::onDisconnected()
        {
            m_isConnected = false;

            m_lock.wake();
        }

        void MQTTClient::onError()
        {
            m_isConnected = false;

            m_lock.wake();
        }

        /**
         * Internal parts
         */
        void MQTTClient::queueOutgoingMessage(IMQTTPacketSPtr pMessage)
        {
            uint16_t messageId;

            if (pMessage->getPacketField(sc_optionMessageId, messageId) == true) {
                bool moreToDo = true;
                MQTTMessageQueue::iterator iter = m_outgoingMessageQueue.find(messageId);

                if (iter != m_outgoingMessageQueue.end()) {
                    // what qos was requested?
                    MQTT_QOS qos;

                    if (pMessage->getPacketField(sc_qosLevel, qos) == true) {
                        if (qos == MQTT_QOS::MQTT_QOS_1) {
                            // for messages I sent, we can now drop this one as it was received
                            moreToDo = false; // we are done
                            m_outgoingMessageQueue.erase(iter); // done
                        } else if (qos == MQTT_QOS::MQTT_QOS_2) {
                        } else {
                            // ?
                            moreToDo = false;
                        }
                    }
                }
                if (moreToDo == true) {
                    m_outgoingMessageQueue[messageId] = pMessage;
                }
            }
        }

        void MQTTClient::queueIncomingMessage(IMQTTPacketSPtr pMessage)
        {
            uint16_t messageId;

            if (pMessage->getPacketField(sc_optionMessageId, messageId) == true) {
                bool moreToDo = true;
                MQTTMessageQueue::iterator iter = m_incomingMessageQueue.find(messageId);

                if (iter != m_incomingMessageQueue.end()) {
                    // what qos was requested?
                    MQTT_QOS qos;

                    if (pMessage->getPacketField(sc_qosLevel, qos) == true) {
                        if (qos == MQTT_QOS::MQTT_QOS_1) {
                            // for messages I sent, we can now drop this one as it was received
                            moreToDo = false; // we are done
                            m_incomingMessageQueue.erase(iter); // done
                        } else if (qos == MQTT_QOS::MQTT_QOS_2) {
                        } else {
                            // ?
                            moreToDo = false;
                        }
                    }
                }
                if (moreToDo == true) {
                    m_incomingMessageQueue[messageId] = pMessage;
                }
            }
        }

        void MQTTClient::sendConnect()
        {
            IMQTTPacketSPtr pConnectPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_Connect);

            if (pConnectPacket != nullptr) {
                MQTTOptions connectOptions;

                connectOptions[sc_clientId] = m_clientId;
                connectOptions[sc_keepAlive] = m_keepAliveTime;
                if (pConnectPacket->initialize(connectOptions) == true) {
                    m_isConnected = false;
                    m_currentState = MQTTState::eMQTTConnection_Requested;
                    m_pProcessor->sendPacket(pConnectPacket);
                } else {
                    // error
                }
            }
        }

        void MQTTClient::sendKeepAlive()
        {
            if (m_isConnected == true) {
                IMQTTPacketSPtr pKeepAlivePacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_PingRequest);
                MQTTOptions options;

                if (pKeepAlivePacket->initialize(options) == true) {
                    m_pProcessor->sendPacket(pKeepAlivePacket);
                } else {
                    // error
                }
            }
        }

        void MQTTClient::process()
        {
            bool timeoutOccurred = false;
            bool waitingForConnection = false;
            bool hasKeepAlive = m_keepAliveTime == 0 ? false : true;
            std::chrono::seconds connectionTimeout = std::chrono::seconds(30);
            std::chrono::seconds keepAliveTime;

            if (hasKeepAlive == true) {
                keepAliveTime = std::chrono::seconds(m_keepAliveTime);
            } else {
                keepAliveTime = connectionTimeout; // sleep for a period regardless
            }
            std::chrono::steady_clock::time_point nextTimeOut;

            while (m_keepProcessing == true) {
                switch (m_currentState) {
                    case MQTTState::eMQTTConnection_Requested:
                    {
                        // start timer for connection timeout
                        if (waitingForConnection == false) {
                            nextTimeOut = std::chrono::steady_clock::now() + connectionTimeout;
                            waitingForConnection = true;
                        }
                    }
                    break;
                    case MQTTState::eMQTTConnection_Failed:
                    {
                        // reconnect time out?
                        // may just keep failing....
                    }
                    break;
                    case MQTTState::eMQTTConnection_Success:
                    {
                        waitingForConnection = false; // we have it

                        // if we have subscriptions then make it so
                        if (m_subscribeOnConnect == true) {
                            subscribe(m_subscribeOnConnect);
                        }

                        // start the keep alive time 
                        m_currentState = MQTTState::eMQTTConnection_Active;
                        nextTimeOut = std::chrono::steady_clock::now() + keepAliveTime;
                    }
                    break;
                    case MQTTState::eMQTTConnection_Active:
                    {
                        if (m_outgoingMessageQueue.empty() == false) {
                            //  Send what we can - do we want to limit?
                        }
                        if (m_incomingMessageQueue.empty() == false) {
                            // Send what we can - handle as needed i.e ack, received, etc
                        }
                        if (timeoutOccurred == true) {
                            nextTimeOut = std::chrono::steady_clock::now() + keepAliveTime;
                        }
                    }
                    break;
                    default:
                    {

                    }
                    break;
                }
                timeoutOccurred = false;
                if (m_lock.waitUntil(nextTimeOut) == std::cv_status::timeout) {
                    timeoutOccurred = true;
                    if (waitingForConnection == true) {
                        // try a reconnect
                        sendConnect();
                    } else if (hasKeepAlive == true) {
                        sendKeepAlive();
                    }
                }
            }
        }
    }
}