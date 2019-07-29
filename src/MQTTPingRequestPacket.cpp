/**
 * @brief MQTTPingPacket
 * 
 * Implementation for MQTT Ping Packets
 * 
 */

#include "MQTTPingRequestPacket.h"

namespace afm {
    namespace communications {
        MQTTPingRequestPacket::MQTTPingRequestPacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_PingRequest);
        }

        MQTTPingRequestPacket::~MQTTPingRequestPacket()
        {

        }

        bool MQTTPingRequestPacket::encodePayload(MQTTBuffer &buffer)
        {
            return true;
        }

        bool MQTTPingRequestPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            return true;
        }
    }
}
