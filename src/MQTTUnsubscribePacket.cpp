/**
 * @brief MQTTSubscribePacket
 * 
 * Implementation for MQTT UnSubscribe packet
 * 
 */

#include <set>

#include "JSONUtil.h"
#include "MQTTUnsubscribePacket.h"

namespace afm {
    namespace communications {
        MQTTUnSubscribePacket::MQTTUnSubscribePacket()
            : MQTTTrackedPacket()
        {
            setType(MQTTPacketType::MQTT_UnSubscribe);
            setFlags(0x02);
        }

        MQTTUnSubscribePacket::~MQTTUnSubscribePacket()
        {
            m_subscriptions.clear();
        }

        bool MQTTUnSubscribePacket::initialize(const MQTTOptions &options)
        {
            std::set<bool> results;

            if (MQTTTrackedPacket::initialize(options) == true) {
                MQTTBuffer topic;

                for (auto subscription : options[sc_subscriptions]) {
                    results.insert(extractValue(subscription, sc_topic, topic));

                    m_subscriptions.push_back(topic);
                }
            } else {
                results.insert(false);
            }
            return results.find(false) == results.end() ? true : false;
        }

        uint32_t MQTTUnSubscribePacket::getVariableLength()
        {
            uint32_t length = MQTTTrackedPacket::getVariableLength();

            for (auto subscription : m_subscriptions) {
                length += calculateFieldLength(subscription);
            }

            return length;
        }

        bool MQTTUnSubscribePacket::encodePayload(MQTTBuffer &buffer)
        {
            bool success = MQTTTrackedPacket::encodePayload(buffer);

            if (success == true) {
                for (auto subscription : m_subscriptions) {
                    encodeData(buffer, subscription);
                }
            }

            return success;
        }

        bool MQTTUnSubscribePacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            bool success = true;

            // pull out the message_id
            success = MQTTTrackedPacket::decodePayload(buffer, offset, payloadLength);

            // get subscriptions
            if (success == true) {
                MQTTBuffer topic;

                while (decodeData(buffer, offset, topic) == true) {
                    m_subscriptions.push_back(topic);
                }
            }

            return success;
        }
    }
}
