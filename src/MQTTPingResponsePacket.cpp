/**
 * @brief MQTTPingPacket
 * 
 * Implementation for MQTT Ping Packets
 * 
 */

#include "MQTTPingResponsePacket.h"

namespace afm {
    namespace communications {
        MQTTPingResponsePacket::MQTTPingResponsePacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_PingResponse);
        }

        MQTTPingResponsePacket::~MQTTPingResponsePacket()
        {

        }

        bool MQTTPingResponsePacket::encodePayload(MQTTBuffer &buffer)
        {
            return true;
        }

        bool MQTTPingResponsePacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            return true;
        }
    }
}
