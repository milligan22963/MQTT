/**
 * @brief MQTTSubscribePacket
 * 
 * Implementation for MQTT Subscribe packet
 * 
 */

#ifndef _H_MQTT_SUBSCRIBE_PACKET
#define _H_MQTT_SUBSCRIBE_PACKET

#include <vector>
#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTSubscribePacket : public MQTTTrackedPacket
        {
            public:
                MQTTSubscribePacket();
                virtual ~MQTTSubscribePacket();

                virtual bool initialize(const MQTTOptions &options) override;

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
                std::vector<MQTTSubscription> m_subscriptions;
        };

        using MQTTSubscribePacketSPtr = std::shared_ptr<MQTTSubscribePacket>;
    }
}
#endif