/**
 * @brief MQTTPacket
 * 
 * Base implementatio for MQTT Packets
 * 
 */

#ifndef _H_MQTT_PACKET
#define _H_MQTT_PACKET

#include "IMQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTPacket : public IMQTTPacket
        {
            public:
                MQTTPacket();
                virtual ~MQTTPacket();

                virtual bool initialize(const MQTTOptions &options) override;
                virtual bool encodePacket(MQTTBuffer &buffer) override;
                virtual bool decodePacket(const MQTTBuffer &buffer) override;
                virtual MQTTPacketType getType() const override;

                static MQTTPacketType getType(const MQTTBuffer &buffer);

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer) = 0;
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t offset, uint32_t payloadLength) = 0;
                void setType(MQTTPacketType type) { m_type = type; }
                void setFlags(uint8_t flags) { m_flags = flags; }
                uint8_t decodeVariableLength(const MQTTBuffer &buffer, uint32_t &variableLength);

            private:
                MQTTPacketType m_type = MQTTPacketType::MQTT_Reserved;
                uint8_t m_flags = 0;
        };

        using IMQTTPacketSPtr = std::shared_ptr<IMQTTPacket>;
    }
}
#endif