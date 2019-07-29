/**
 * @brief MQTTUnsubscribeAckPacket
 * 
 * Implementation for MQTT UnSubscribeACK packet
 * 
 */

#include "MQTTUnsubscribeAckPacket.h"

namespace afm {
    namespace communications {
        MQTTUnSubscribeAckPacket::MQTTUnSubscribeAckPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_UnSubscribeAck);
        }

        MQTTUnSubscribeAckPacket::~MQTTUnSubscribeAckPacket()
        {
            
        }
    }
}
