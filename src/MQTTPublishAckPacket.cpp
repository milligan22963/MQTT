/**
 * @brief MQTTPublishPacket
 * 
 * Base implementatio for MQTT Packets with tracking i.e. messages
 * 
 */

#include <set>
#include "JSONUtil.h"
#include "MQTTPublishAckPacket.h"

namespace afm {
    namespace communications {

        MQTTPublishAckPacket::MQTTPublishAckPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_PublishAck);
        }

        MQTTPublishAckPacket::~MQTTPublishAckPacket()
        {

        }

        /**
         * Internal parts
         */
    }
}