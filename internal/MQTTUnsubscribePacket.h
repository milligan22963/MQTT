/**
 * @brief MQTTUnsubscribePacket
 * 
 * Implementation for MQTT UnSubscribe packet
 * 
 */

#ifndef _H_MQTT_UNSUBSCRIBE_PACKET
#define _H_MQTT_UNSUBSCRIBE_PACKET

#include <vector>
#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {

        class MQTTUnSubscribePacket : public MQTTTrackedPacket
        {
            public:
                MQTTUnSubscribePacket();
                virtual ~MQTTUnSubscribePacket();

                virtual bool initialize(const MQTTOptions &options) override;

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
                std::vector<MQTTBuffer> m_subscriptions;
        };

        using MQTTUnSubscribePacketSPtr = std::shared_ptr<MQTTUnSubscribePacket>;
    }
}
#endif