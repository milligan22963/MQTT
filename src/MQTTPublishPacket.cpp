/**
 * @brief MQTTPublishPacket
 * 
 * Base implementatio for MQTT Packets with tracking i.e. messages
 * 
 */

#include <set>
#include "JSONUtil.h"
#include "MQTTPublishPacket.h"

namespace afm {
    namespace communications {

        const uint8_t sc_duplicateFlagSet = 0x08;
        const uint8_t sc_retainFlagSet = 0x01;

        MQTTPublishPacket::MQTTPublishPacket()
        {

        }

        MQTTPublishPacket::~MQTTPublishPacket()
        {

        }

        bool MQTTPublishPacket::initialize(const MQTTOptions &options)
        {
            bool success = MQTTTrackedPacket::initialize(options);

            if (success == true) {
                std::set<bool> results;
                
                results.insert(extractValue(options, sc_duplicateFlag, m_duplicate));
                results.insert(extractValue(options, sc_qosLevel, m_qos));
                results.insert(extractValue(options, sc_retainFlag, m_retain));

                uint8_t flags = 0;

                // if qos > 0 then we can set the duplicate
                // and the qos level
                if (m_qos > MQTT_QOS::MQTT_QOS_0) {
                    if (m_duplicate == true) {
                        flags |= sc_duplicateFlagSet;
                    }
                    flags |= m_qos << 1;
                }

                if (m_retain == true) {
                    flags |= sc_retainFlagSet;
                }

                setFlags(flags);

                success = results.find(false) == results.end() ? true : false;
            }

            return success;
        }

        /**
         * Internal parts
         */

        uint32_t MQTTPublishPacket::getVariableLength()
        {
            uint32_t length = MQTTTrackedPacket::getVariableLength();

            length += m_topic.size();
            length += m_message.size();

            return length;
        }

        bool MQTTPublishPacket::encodePayload(MQTTBuffer &buffer)
        {
            std::set<bool> results;

            // encode the topic
            results.insert(encodeData(buffer, m_topic));

            // encode the base class message id
            results.insert(MQTTTrackedPacket::encodePayload(buffer));

            if (m_message.size() > 0) {
                buffer.insert(buffer.end(), m_message.begin(), m_message.end());
            }

            return results.find(false) == results.end() ? true : false;
        }

        bool MQTTPublishPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            bool success = false;

            return success;
        }
    }
}