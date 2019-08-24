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

            MQTTOptions clientOptions = options;

            clientOptions[sc_processorType] = sc_mqttClient;

            if (m_pProcessor->initialize(clientOptions) == true) {
                m_pProcessor->addListener(shared_from_this());
                m_pListener = pListener;

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
                    m_currentState = MQTTState::eMQTTSubscription_Requested;
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
                    m_currentState = MQTTState::eMQTTUnSubscribe_Requested;
                    m_pProcessor->sendPacket(pPacket);
                    success = true;
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
                    MQTTSubscribeAckPacketSPtr pAckPacket = std::dynamic_pointer_cast<MQTTSubscribeAckPacket>(pPacket);

                    if (pAckPacket != nullptr) {
                        m_currentState = pAckPacket->subscriptionSuccess() == true
                            ? MQTTState::eMQTTSubscription_Success
                            : MQTTState::eMQTTSubscription_Failed;
                    } else {
                        m_currentState = MQTTState::eMQTTSubscription_Failed;
                    }

                    if (m_pListener != nullptr) {
                        m_pListener->onSubscriptionAdded(m_currentState == MQTTState::eMQTTSubscription_Success);
                    }
                }
                break;
                case MQTTPacketType::MQTT_UnSubscribeAck:
                {
                     m_currentState = MQTTState::eMQTTUnSubscribe_Success;

                    if (m_pListener != nullptr) {
                        m_pListener->onSubscriptionRemoved(m_currentState == MQTTState::eMQTTUnSubscribe_Success);
                    }
                }
                break;
                case MQTTPacketType::MQTT_Publish:
                {
                    IMQTTPacketSPtr pAckPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_PublishAck);

                    if (m_pListener != nullptr) {
                        m_pListener->onMessageReceived(pPacket);
                    }
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
                        } else {
                            m_currentState = MQTTState::eMQTTConnection_Active;
                            nextTimeOut = std::chrono::steady_clock::now() + keepAliveTime;
                        }

                        // start the keep alive time 
                    }
                    break;
                    case MQTTState::eMQTTSubscription_Requested:
                    {

                    }
                    break;
                    case MQTTState::eMQTTSubscription_Success:
                    {
                        // we succeeded
                        m_currentState = MQTTState::eMQTTConnection_Active;
                        nextTimeOut = std::chrono::steady_clock::now() + keepAliveTime;
                    }
                    break;
                    case MQTTState::eMQTTConnection_Active:
                    {
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