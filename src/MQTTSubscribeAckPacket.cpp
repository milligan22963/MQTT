/**
 * @brief MQTTSubscribeAckPacket
 * 
 * Implementation for MQTT subscribe ack messages
 * 
 */

#include <set>
#include "JSONUtil.h"
#include "MQTTSubscribeAckPacket.h"

namespace afm {
    namespace communications {

        MQTTSubscribeAckPacket::MQTTSubscribeAckPacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_SubscribeAck);
        }

        MQTTSubscribeAckPacket::~MQTTSubscribeAckPacket()
        {
            m_subscriptionStatus.clear();
        }

        bool MQTTSubscribeAckPacket::initialize(const MQTTOptions &options)
        {
            std::set<bool> results;

            if (MQTTTrackedPacket::initialize(options) == true) {
                MQTT_QOS qos = MQTT_QOS::MQTT_QOS_0;

                for (auto subscription : options[sc_subscriptions]) {
                    results.insert(extractValue(subscription, sc_qosLevel, qos));
                    m_subscriptionStatus.push_back(qos);
                }
            } else {
                results.insert(false);
            }
            return results.find(false) == results.end() ? true : false;
        }

        bool MQTTSubscribeAckPacket::subscriptionSuccess() const
        {
            bool success = true;

            for (auto status : m_subscriptionStatus) {
                if (status == MQTT_QOS::MQTT_QOS_FAILURE) {
                    success = false;
                }
            }
            return success;
        }

        /**
         * Internal parts
         */
        uint32_t MQTTSubscribeAckPacket::getVariableLength()
        {
            uint32_t length = MQTTTrackedPacket::getVariableLength();

            length += (m_subscriptionStatus.size() * sizeof(MQTT_QOS));

            return length;
        }

        bool MQTTSubscribeAckPacket::encodePayload(MQTTBuffer &buffer)
        {
            bool success = MQTTTrackedPacket::encodePayload(buffer);

            if (success == true) {
                for (auto qos : m_subscriptionStatus) {
                    buffer.push_back(qos);
                }
            }

            return success;
        }

        bool MQTTSubscribeAckPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            bool success = true;

            // pull out the message_id
            success = MQTTTrackedPacket::decodePayload(buffer, offset, payloadLength);

            // get subscriptions
            if (success == true) {

                while (offset < payloadLength) {
                    m_subscriptionStatus.push_back((MQTT_QOS)buffer[offset++]);
                }
            }

            return success;
        }
    }
}