/**
 * @brief MQTTDisconnectPacket
 * 
 * Implementation for MQTT Disconnect Packets
 * 
 */

#ifndef _H_MQTT_DISCONNECT_PACKET
#define _H_MQTT_DISCONNECT_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTDisconnectPacket : public MQTTPacket
        {
            public:
                MQTTDisconnectPacket();
                virtual ~MQTTDisconnectPacket();

            protected:
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);
        };

        using MQTTDisconnectPacketSPtr = std::shared_ptr<MQTTDisconnectPacket>;
    }
}
#endif