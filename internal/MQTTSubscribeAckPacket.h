/**
 * @brief MQTTSubscribeAckPacket
 * 
 * Implementation for MQTT subscribe ack messages
 * 
 */

#ifndef _H_MQTT_SUBSCRIBE_ACK_PACKET
#define _H_MQTT_SUBSCRIBE_ACK_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTSubscribeAckPacket : public MQTTTrackedPacket
        {
            public:
                MQTTSubscribeAckPacket();
                virtual ~MQTTSubscribeAckPacket();

                virtual bool initialize(const MQTTOptions &options) override;

                bool subscriptionSuccess() const;

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
                std::vector<MQTT_QOS> m_subscriptionStatus;
        };

        using MQTTSubscribeAckPacketSPtr = std::shared_ptr<MQTTSubscribeAckPacket>;
    }
}
#endif