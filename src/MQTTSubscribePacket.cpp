/**
 * @brief MQTTSubscribePacket
 * 
 * Implementation for MQTT Subscribe packet
 * 
 */

#include <set>

#include "JSONUtil.h"
#include "MQTTSubscribePacket.h"

namespace afm {
    namespace communications {
        MQTTSubscribePacket::MQTTSubscribePacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_Subscribe);
        }

        MQTTSubscribePacket::~MQTTSubscribePacket()
        {
            m_subscriptions.clear();
        }

        bool MQTTSubscribePacket::initialize(const MQTTOptions &options)
        {
            std::set<bool> results;

            if (MQTTTrackedPacket::initialize(options) == true) {
                MQTT_QOS qos = MQTT_QOS::MQTT_QOS_0;
                MQTTBuffer topic;

                for (auto subscription : options[sc_subscriptions]) {
                    results.insert(extractValue(subscription, sc_qosLevel, qos));
                    results.insert(extractValue(subscription, sc_topic, topic));

                    m_subscriptions.push_back({topic, qos});
                }
            } else {
                results.insert(false);
            }
            return results.find(false) == results.end() ? true : false;
        }

        uint32_t MQTTSubscribePacket::getVariableLength()
        {
            uint32_t length = MQTTTrackedPacket::getVariableLength();

            for (auto subscription : m_subscriptions) {
                length += calculateFieldLength(subscription.topic);
                length += sizeof(MQTT_QOS);
            }

            return length;
        }

        bool MQTTSubscribePacket::encodePayload(MQTTBuffer &buffer)
        {
            bool success = MQTTTrackedPacket::encodePayload(buffer);

            if (success == true) {
                for (auto subscription : m_subscriptions) {
                    encodeData(buffer, subscription.topic);
                    buffer.push_back(subscription.qos);
                }
            }

            return success;
        }

        bool MQTTSubscribePacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            bool success = true;

            // pull out the message_id
            success = MQTTTrackedPacket::decodePayload(buffer, offset, payloadLength);

            // get subscriptions
            if (success == true) {
                MQTTBuffer topic;

                while (decodeData(buffer, offset, topic) == true) {
                    MQTT_QOS qos;
                    qos = (MQTT_QOS)buffer[offset++];
                    m_subscriptions.push_back({topic, qos});
                }
            }

            return success;
        }
    }
}
