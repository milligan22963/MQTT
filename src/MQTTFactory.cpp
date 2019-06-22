/**
 * @brief MQTTFactory
 * 
 * Class for creating MQTT messages
 * 
 */

#include "MQTTFactory.h"
#include "MQTTPacket.h"

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

                }
                break;
                case MQTTPacketType::MQTT_ConnectAck:
                {

                }
                break;
                case MQTTPacketType::MQTT_Disconnect:
                {

                }
                break;
                case MQTTPacketType::MQTT_PingRequest:
                {

                }
                break;
                case MQTTPacketType::MQTT_PingResponse:
                {

                }
                break;
                case MQTTPacketType::MQTT_Publish:
                {

                }
                break;
                case MQTTPacketType::MQTT_PublishAck:
                {

                }
                break;
                case MQTTPacketType::MQTT_PublishComplete:
                {

                }
                break;
                case MQTTPacketType::MQTT_PublishReceive:
                {

                }
                break;
                case MQTTPacketType::MQTT_PublishRelease:
                {

                }
                break;
                case MQTTPacketType::MQTT_Subscribe:
                {

                }
                break;
                case MQTTPacketType::MQTT_SubscribeAck:
                {

                }
                break;
                case MQTTPacketType::MQTT_UnSubscribe:
                {

                }
                break;
                case MQTTPacketType::MQTT_UnSubscribeAck:
                {

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
