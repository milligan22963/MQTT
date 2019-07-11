/**
 * @brief MQTTPublishAckPacket
 * 
 * Implementatio for MQTT publish ack messages
 * 
 */

#ifndef _H_MQTT_PUBLISH_ACK_PACKET
#define _H_MQTT_PUBLISH_ACK_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTPublishAckPacket : public MQTTTrackedPacket
        {
            public:
                MQTTPublishAckPacket();
                virtual ~MQTTPublishAckPacket();

            private:
        };
    }
}
#endif