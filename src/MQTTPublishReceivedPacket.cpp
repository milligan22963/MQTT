/**
 * @brief MQTTPublishReceivedPacket
 * 
 * Implementation for MQTT Publish received packets
 * 
 */

#include "MQTTPublishReceivedPacket.h"

namespace afm {
    namespace communications {
        MQTTPublishReceivedPacket::MQTTPublishReceivedPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_PublishReceive);
        }

        MQTTPublishReceivedPacket::~MQTTPublishReceivedPacket()
        {

        }
    }
}
