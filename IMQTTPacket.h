/**
 * @brief IMQTTPacket
 * 
 * Interface for MQTT Packets
 * 
 */

#ifndef _H_IMQTT_PACKET
#define _H_IMQTT_PACKET

#include <memory>

namespace afm {
    namespace communications {
        enum MQTTPacketType
        {
            MQTT_Reserved = 0,
            MQTT_Connect,
            MQTT_ConnectAck,
            MQTT_Publish,
            MQTT_PublishAck,
            MQTT_PublishReceive,
            MQTT_PublishRelease,
            MQTT_PublishComplete,
            MQTT_Subscribe,
            MQTT_SubscribeAck,
            MQTT_UnSubscribe,
            MQTT_UnSubscribeAck,
            MQTT_PingRequest,
            MQTT_PingResponse,
            MQTT_Disconnect,
            END_MQTT_PACKETS
        };

        class IMQTTPacket
        {
            public:
                virtual ~IMQTTPacket() {};

        };

        using IMQTTPacketSPtr = std::shared_ptr<IMQTTPacket>;
    }
}
#endif