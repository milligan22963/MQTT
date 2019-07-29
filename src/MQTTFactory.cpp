/**
 * @brief MQTTFactory
 * 
 * Class for creating MQTT messages
 * 
 */

#include "MQTTFactory.h"
#include "MQTTConnectPacket.h"
#include "MQTTConnectAckPacket.h"
#include "MQTTDisconnectPacket.h"
#include "MQTTPacket.h"
#include "MQTTPingRequestPacket.h"
#include "MQTTPingResponsePacket.h"
#include "MQTTPublishPacket.h"
#include "MQTTPublishAckPacket.h"
#include "MQTTPublishCompletedPacket.h"
#include "MQTTPublishReceivedPacket.h"
#include "MQTTPublishReleasedPacket.h"
#include "MQTTSubscribePacket.h"
#include "MQTTSubscribeAckPacket.h"
#include "MQTTUnsubscribePacket.h"
#include "MQTTUnsubscribeAckPacket.h"

namespace afm {
    namespace communications {
        MQTTFactory::MQTTFactory()
        {

        }

        MQTTFactory::~MQTTFactory()
        {

        }

        MQTTFactorySPtr MQTTFactory::getInstance()
        {
            static MQTTFactorySPtr pFactory = nullptr;

            if (pFactory == nullptr) {
                pFactory = std::make_shared<MQTTFactory>();
            }
            return pFactory;
        }

        IMQTTPacketSPtr MQTTFactory::createPacket(MQTTPacketType packetType)
        {
            IMQTTPacketSPtr pPacket = nullptr;

            switch (packetType) {
                case MQTTPacketType::MQTT_Connect:
                {
                    pPacket = std::make_shared<MQTTConnectPacket>();
                }
                break;
                case MQTTPacketType::MQTT_ConnectAck:
                {
                    pPacket = std::make_shared<MQTTConnectAckPacket>();
                }
                break;
                case MQTTPacketType::MQTT_Disconnect:
                {
                    pPacket = std::make_shared<MQTTDisconnectPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PingRequest:
                {
                    pPacket = std::make_shared<MQTTPingRequestPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PingResponse:
                {
                    pPacket = std::make_shared<MQTTPingResponsePacket>();
                }
                break;
                case MQTTPacketType::MQTT_Publish:
                {
                    pPacket = std::make_shared<MQTTPublishPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PublishAck:
                {
                    pPacket = std::make_shared<MQTTPublishAckPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PublishComplete:
                {
                    pPacket = std::make_shared<MQTTPublishCompletedPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PublishReceive:
                {
                    pPacket = std::make_shared<MQTTPublishReceivedPacket>();
                }
                break;
                case MQTTPacketType::MQTT_PublishRelease:
                {
                    pPacket = std::make_shared<MQTTPublishReleasedPacket>();
                }
                break;
                case MQTTPacketType::MQTT_Subscribe:
                {
                    pPacket = std::make_shared<MQTTSubscribePacket>();
                }
                break;
                case MQTTPacketType::MQTT_SubscribeAck:
                {
                    pPacket = std::make_shared<MQTTSubscribeAckPacket>();
                }
                break;
                case MQTTPacketType::MQTT_UnSubscribe:
                {
                    pPacket = std::make_shared<MQTTUnSubscribePacket>();
                }
                break;
                case MQTTPacketType::MQTT_UnSubscribeAck:
                {
                    pPacket = std::make_shared<MQTTUnSubscribeAckPacket>();
                }
                break;
                default:
                {
                    // ignore
                }
                break;
            }
            return pPacket;
        }

        IMQTTPacketSPtr MQTTFactory::createPacket(const MQTTBuffer &buffer)
        {
            IMQTTPacketSPtr pPacket = createPacket(MQTTPacket::getType(buffer));

            if (pPacket != nullptr) {
                pPacket->decodePacket(buffer);
            }

            return pPacket;
        }
    }
}
