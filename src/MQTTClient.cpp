/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#include <iostream>
#include "MQTTClient.h"
#include "MQTTFactory.h"
#include "MQTTConnectAckPacket.h"

namespace afm {
    namespace communications {
        MQTTClient::MQTTClient()
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
                success = true;
            } else {
                m_pProcessor->shutdown();
                m_pProcessor = nullptr;
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

    }
}