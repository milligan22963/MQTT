/**
 * @brief MQTTDisconnectPacket
 * 
 * Implementation for MQTT Disconnect Packets
 * 
 */

#include "MQTTDisconnectPacket.h"

namespace afm {
    namespace communications {
        MQTTDisconnectPacket::MQTTDisconnectPacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_Disconnect);
        }

        MQTTDisconnectPacket::~MQTTDisconnectPacket()
        {

        }

        bool MQTTDisconnectPacket::encodePayload(MQTTBuffer &buffer)
        {
            return true;
        }

        bool MQTTDisconnectPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            return true;
        }
    }
}
