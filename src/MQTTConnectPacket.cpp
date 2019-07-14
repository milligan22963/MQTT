/**
 * @brief MQTTConnectPacket
 * 
 * Implementation for MQTT Connect packet
 * 
 */

#include "MQTTConnectPacket.h"

namespace afm {
    namespace communications {
        MQTTConnectPacket::MQTTConnectPacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_Connect);
        }

        MQTTConnectPacket::~MQTTConnectPacket()
        {

        }

        bool MQTTConnectPacket::initialize(const MQTTOptions &options)
        {
            bool success = MQTTPacket::initialize(options);

            return success;
        }

        /**
         * Protected parts
         */
        uint32_t MQTTConnectPacket::getVariableLength()
        {
            return 0;
        }

        bool MQTTConnectPacket::encodePayload(MQTTBuffer &buffer)
        {
            return false;
        }

        bool MQTTConnectPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            return false;
        }
    }
}
