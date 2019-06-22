/**
 * @brief MQTTPacket
 * 
 * Base implementatio for MQTT Packets
 * 
 */

#include "MQTTPacket.h"

namespace afm {
    namespace communications {

        struct MQTTFixedHeader {
            union {
                struct {
                    uint8_t flags:4;
                    uint8_t packetType:4;
                } values;
                uint8_t raw;
            } header;
        };

        MQTTPacket::MQTTPacket()
        {

        }

        MQTTPacket::~MQTTPacket()
        {

        }

        bool MQTTPacket::initialize(const MQTTOptions &options)
        {
            return true;
        }

        bool MQTTPacket::encodePacket(MQTTBuffer &buffer)
        {
            MQTTFixedHeader fixedHeader;

            fixedHeader.header.values.flags = m_flags;
            fixedHeader.header.values.packetType = m_type;

            buffer.push_back(fixedHeader.header.raw);

            uint32_t variableLength = getVariableLength();
            uint8_t encodeByte = 0;
            do {
                encodeByte = variableLength % 128;
                variableLength /= 128;
                if (variableLength > 0) {
                    encodeByte |= 128;
                }
                buffer.push_back(encodeByte);
            } while (variableLength > 0);

            return encodePayload(buffer);
        }

        bool MQTTPacket::decodePacket(const MQTTBuffer &buffer)
        {
            MQTTFixedHeader fixedHeader;

            fixedHeader.header.raw = buffer.front();
            m_type = (MQTTPacketType)fixedHeader.header.values.packetType;
            m_flags = fixedHeader.header.values.flags;

            uint32_t offset = sizeof(MQTTFixedHeader) + 1;

            uint32_t length = 0;
            offset += decodeVariableLength(buffer, length);

            return decodePayload(buffer, offset, length);
        }

        MQTTPacketType MQTTPacket::getType() const
        {
            return m_type;
        }

        MQTTPacketType MQTTPacket::getType(const MQTTBuffer &buffer)
        {
            MQTTPacketType packetType = MQTTPacketType::MQTT_Reserved;
            MQTTFixedHeader fixedHeader;

            fixedHeader.header.raw = buffer.front();

            packetType = (MQTTPacketType)fixedHeader.header.values.packetType;

            return packetType;
        }

        uint32_t MQTTPacket::getVariableLength()
        {
            return 0;
        }

        uint8_t MQTTPacket::decodeVariableLength(const MQTTBuffer &buffer, uint32_t &variableLength)
        {
            uint8_t bytesConsumed = 0;
            variableLength = 0;
            uint8_t multiplier = 1;
            const uint8_t *pValue = buffer.data();
            do {
                pValue++;
                bytesConsumed++;
                variableLength += (*pValue & 127) * multiplier;
                multiplier *= 128;
                if (multiplier > (128 * 128 * 128)) {
                    break;
                }
            } while (*pValue & 128);

            return bytesConsumed;
        }
    }
}