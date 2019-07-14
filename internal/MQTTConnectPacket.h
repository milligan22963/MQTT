/**
 * @brief MQTTConnectPacket
 * 
 * Implementation for MQTT Connect packet
 * 
 */

#ifndef _H_MQTT_CONNECT_PACKET
#define _H_MQTT_CONNECT_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTConnectPacket : public MQTTPacket
        {
            public:
                MQTTConnectPacket();
                virtual ~MQTTConnectPacket();

                virtual bool initialize(const MQTTOptions &options) override;

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
        };

        using MQTTConnectPacketSPtr = std::shared_ptr<MQTTConnectPacket>;
    }
}
#endif