/**
 * @brief MQTTPublishReceivedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#ifndef _H_MQTT_PUBLISH_RECEIVED_PACKET
#define _H_MQTT_PUBLISH_RECEIVED_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTPublishReceivedPacket : public MQTTTrackedPacket
        {
            public:
                MQTTPublishReceivedPacket();
                virtual ~MQTTPublishReceivedPacket();

            private:
        };
    }
}
#endif