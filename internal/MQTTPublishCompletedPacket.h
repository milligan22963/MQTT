/**
 * @brief MQTTPublishCompletedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#ifndef _H_MQTT_PUBLISH_COMPLETED_PACKET
#define _H_MQTT_PUBLISH_COMPLETED_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTPublishCompletedPacket : public MQTTTrackedPacket
        {
            public:
                MQTTPublishCompletedPacket();
                virtual ~MQTTPublishCompletedPacket();

            private:
        };
    }
}
#endif