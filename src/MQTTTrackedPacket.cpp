/**
 * @brief MQTTTrackedPacket
 * 
 * Base implementatio for MQTT Packets
 * 
 */

#include "JSONUtil.h"
#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        MQTTTrackedPacket::MQTTTrackedPacket()
        {

        }

        MQTTTrackedPacket::~MQTTTrackedPacket()
        {

        }

        bool MQTTTrackedPacket::initialize(const MQTTOptions &options)
        {
            bool success = false;

            success = extractValue(options, sc_optionMessageId, m_messageId);

            return success;
        }

        uint32_t MQTTTrackedPacket::getVariableLength()
        {

        }

        /**
         * Internal parts
         */
        bool MQTTTrackedPacket::encodePayload(MQTTBuffer &buffer)
        {

        }

        bool MQTTTrackedPacket::decodePayload(const MQTTBuffer &buffer, uint32_t offset, uint32_t payloadLength)
        {

        }
    }
}
