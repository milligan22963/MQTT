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
            : MQTTPacket()
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

        bool MQTTTrackedPacket::getPacketField(const std::string &field, uint16_t &value)
        {
            bool success = false;

            if (field == sc_optionMessageId) {
                value = m_messageId;
                success = true;
            }

            return success;
        }

        /**
         * Internal parts
         */

        uint32_t MQTTTrackedPacket::getVariableLength()
        {
            return sizeof(m_messageId);
        }

        bool MQTTTrackedPacket::encodePayload(MQTTBuffer &buffer)
        {
            bool success = true;

            buffer.push_back(HighByte(m_messageId));
            buffer.push_back(LowByte(m_messageId));

            return success;
        }

        bool MQTTTrackedPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            bool success = false;

            if ((buffer.size() - offset) >= sizeof(m_messageId)) {
                m_messageId = buffer[offset++] << 8;
                m_messageId |= buffer[offset++];

                success = true;
            }
            return success;
        }
    }
}
