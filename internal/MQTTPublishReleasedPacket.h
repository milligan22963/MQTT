/**
 * @brief MQTTPublishReleasedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#ifndef _H_MQTT_PUBLISH_RELEASED_PACKET
#define _H_MQTT_PUBLISH_RELEASED_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTPublishReleasedPacket : public MQTTTrackedPacket
        {
            public:
                MQTTPublishReleasedPacket();
                virtual ~MQTTPublishReleasedPacket();

            private:
        };
    }
}
#endif