/**
 * @brief MQTTPingResponsePacket
 * 
 * Implementation for MQTT Ping Response Packets
 * 
 */

#ifndef _H_MQTT_PING_RESPONSE_PACKET
#define _H_MQTT_PING_RESPONSE_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTPingResponsePacket : public MQTTPacket
        {
            public:
                MQTTPingResponsePacket();
                virtual ~MQTTPingResponsePacket();

            protected:
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);
        };

        using MQTTPingResponsePacketSPtr = std::shared_ptr<MQTTPingResponsePacket>;
    }
}
#endif