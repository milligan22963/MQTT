/**
 * @brief MQTTPublishPacket
 * 
 * Base implementatio for MQTT Packets with tracking i.e. messages
 * 
 */

#ifndef _H_MQTT_PUBLISH_PACKET
#define _H_MQTT_PUBLISH_PACKET

#include "MQTTTrackedPacket.h"

namespace afm {
    namespace communications {
        class MQTTPublishPacket : public MQTTTrackedPacket
        {
            public:
                MQTTPublishPacket();
                virtual ~MQTTPublishPacket();

                virtual bool initialize(const MQTTOptions &options) override;

                virtual bool getPacketField(const std::string &field, MQTT_QOS &value) override;

            protected:
                virtual uint32_t getVariableLength() override;
                virtual bool encodePayload(MQTTBuffer &buffer) override;
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength) override;

            private:
                bool m_duplicate = false;
                bool m_retain = false;
                MQTT_QOS m_qos;
                MQTTBuffer m_topic;
                MQTTBuffer m_message;
        };
    }
}
#endif