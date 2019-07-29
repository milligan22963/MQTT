/**
 * @brief MQTTPingPacket
 * 
 * Implementation for MQTT Ping Packets
 * 
 */

#ifndef _H_MQTT_PING_REQUEST_PACKET
#define _H_MQTT_PING_REQUEST_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTPingRequestPacket : public MQTTPacket
        {
            public:
                MQTTPingRequestPacket();
                virtual ~MQTTPingRequestPacket();

            protected:
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);
        };

        using MQTTPingRequestPacketSPtr = std::shared_ptr<MQTTPingRequestPacket>;
    }
}
#endif