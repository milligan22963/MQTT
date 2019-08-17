/**
 * @brief MQTTConnectAckPacket
 * 
 * Implementation for MQTT Connect Ack packet
 * 
 */

#include <iostream>
#include <set>

#include "JSONUtil.h"
#include "MQTTConnectAckPacket.h"

namespace afm {
    namespace communications {
        MQTTConnectAckPacket::MQTTConnectAckPacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_ConnectAck);
        }

        MQTTConnectAckPacket::~MQTTConnectAckPacket()
        {

        }

        bool MQTTConnectAckPacket::initialize(const MQTTOptions &options)
        {
            std::set<bool> results;
            
            if (MQTTPacket::initialize(options) == true) {
                results.insert(extractValue(options, sc_sessionPresent, m_sessionPresent));
                results.insert(extractValue(options, sc_connectionResponse, m_connectionResponse));
            }

            return results.find(false) == results.end() ? true : false;
        }

        uint32_t MQTTConnectAckPacket::getVariableLength()
        {
            uint32_t length = sizeof(m_connectionResponse);

            length += sizeof(uint8_t);

            return length;
        }

        bool MQTTConnectAckPacket::encodePayload(MQTTBuffer &buffer)
        {
            bool success = true;

            buffer.push_back(m_sessionPresent == true ? 1 : 0);
            buffer.push_back(m_connectionResponse);

            return success;
        }

        bool MQTTConnectAckPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            uint8_t sessionPresent = buffer[offset++];

            m_sessionPresent = sessionPresent == 1 ? true : false;

            m_connectionResponse = (ConnectionResponse)buffer[offset++];

            return true;
        }
    }
}
