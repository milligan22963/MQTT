/**
 * @brief MQTTUnsubscribeAckPacket
 * 
 * Implementation for MQTT UnSubscribeACK packet
 * 
 */

#ifndef _H_MQTT_UNSUBSCRIBE_ACK_PACKET
#define _H_MQTT_UNSUBSCRIBE_ACK_PACKET

#include <vector>
#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {

        class MQTTUnSubscribeAckPacket : public MQTTTrackedPacket
        {
            public:
                MQTTUnSubscribeAckPacket();
                virtual ~MQTTUnSubscribeAckPacket();
        };

        using MQTTUnSubscribeAckPacketSPtr = std::shared_ptr<MQTTUnSubscribeAckPacket>;
    }
}
#endif