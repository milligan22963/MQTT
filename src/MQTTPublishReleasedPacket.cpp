/**
 * @brief MQTTPublishReceivedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#include "MQTTPublishReleasedPacket.h"

namespace afm {
    namespace communications {
        MQTTPublishReleasedPacket::MQTTPublishReleasedPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_PublishRelease);
        }

        MQTTPublishReleasedPacket::~MQTTPublishReleasedPacket()
        {

        }
    }
}
