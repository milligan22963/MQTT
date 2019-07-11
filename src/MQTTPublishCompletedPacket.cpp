/**
 * @brief MQTTPublishReceivedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#include "MQTTPublishCompletedPacket.h"

namespace afm {
    namespace communications {
        MQTTPublishCompletedPacket::MQTTPublishCompletedPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_PublishComplete);
        }

        MQTTPublishCompletedPacket::~MQTTPublishCompletedPacket()
        {

        }
    }
}
