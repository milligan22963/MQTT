/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#include <iostream>
#include <unistd.h>

#include "MQTTClient.h"
#include "MQTTFactory.h"
#include "MQTTConnectAckPacket.h"
#include "MQTTSubscribeAckPacket.h"

namespace afm {
    namespace communications {
        MQTTClient::MQTTClient()
            : m_keepProcessing(false)
            , m_currentState(MQTTState::eEndMQTTStates)
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
            m_subscriptions.push_back(subscription);

            return true;
        }

        bool MQTTClient::removeSubscription(const MQTTSubscription &subscription)
        {
            m_subscriptions.push_back(subscription);

            return true;
        }

        bool MQTTClient::subscribe()
        {
            bool success = false;

            MQTTOptions subscribeOptions;

            subscribeOptions[sc_optionMessageId] = (uint16_t)m_nextMessageId++;
            subscribeOptions[sc_subscriptions] = MQTTOptions::array();
            for (auto subscription : m_subscriptions) {
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

            return success;
        }

        bool MQTTClient::unsubscribe()
        {
            return false;
        }

        bool MQTTClient::sendMessage(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos)
        {
            bool success = false;

            return success;
        }

        void MQTTClient::shutdown()
        {
            m_keepProcessing = false;
            m_stateProcessor.join();

            if (m_pProcessor != nullptr) {
                m_pProcessor->shutdown();
                m_pProcessor = nullptr;
            }
        }

        void MQTTClient::onConnected()
        {
            IMQTTPacketSPtr pConnectPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_Connect);

            if (pConnectPacket != nullptr) {
                MQTTOptions connectOptions;

                connectOptions[sc_clientId] = "afmTest";
                if (pConnectPacket->initialize(connectOptions) == true) {
                    m_currentState = MQTTState::eMQTTConnection_Requested;
                    m_pProcessor->sendPacket(pConnectPacket);
                } else {
                    // error
                }
            }
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
                case MQTTPacketType::MQTT_Publish:
                {
                    IMQTTPacketSPtr pAckPacket = MQTTFactory::getInstance()->createPacket(MQTTPacketType::MQTT_PublishAck);

                    if (m_pListener != nullptr) {
                        m_pListener->onMessageReceived(pPacket);
                    }
                }
                break;
                default:
                {
                    // unhandled at the moment
                }
                break;
            }
        }

        void MQTTClient::onMessageDelivered(IMQTTPacketSPtr pPacket)
        {

        }

        void MQTTClient::onDisconnected()
        {
        }

        void MQTTClient::onError()
        {
        }

        void MQTTClient::process()
        {
            while (m_keepProcessing == true) {
                switch (m_currentState)
                {
                    case MQTTState::eMQTTConnection_Requested:
                    case MQTTState::eMQTTConnection_Failed:
                    {
                        // reconnect time out?
                    }
                    break;
                    case MQTTState::eMQTTConnection_Success:
                    {
                        if (m_subscriptions.empty() == false) {
                            subscribe();
                        }
                    }
                    break;
                    case MQTTState::eMQTTSubscription_Requested:
                    {

                    }
                    break;
                    default:
                    {

                    }
                    break;
                }
                sleep(1);
            }
        }
    }
}