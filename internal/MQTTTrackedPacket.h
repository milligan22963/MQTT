/**
 * @brief MQTTTrackedPacket
 * 
 * Base implementatio for MQTT Packets with tracking i.e. messages
 * 
 */

#ifndef _H_MQTT_TRACKED_PACKET
#define _H_MQTT_TRACKED_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTTrackedPacket : public MQTTPacket
        {
            public:
                MQTTTrackedPacket();
                virtual ~MQTTTrackedPacket();

                virtual bool initialize(const MQTTOptions &options) override;

                virtual bool getPacketField(const std::string &field, uint16_t &value) override;

                uint16_t getMessageId() const { return m_messageId; }

            protected:
                virtual uint32_t getVariableLength() override;
                virtual bool encodePayload(MQTTBuffer &buffer) override;
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength) override;

            private:
                uint16_t m_messageId = 0;
        };

        using MQTTTrackedPacketSPtr = std::shared_ptr<MQTTTrackedPacket>;
    }
}
#endif