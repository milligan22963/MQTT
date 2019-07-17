/**
 * @brief MQTTConnectPacket
 * 
 * Implementation for MQTT Connect packet
 * 
 */

#ifndef _H_MQTT_CONNECT_PACKET
#define _H_MQTT_CONNECT_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTConnectPacket : public MQTTPacket
        {
            public:
                MQTTConnectPacket();
                virtual ~MQTTConnectPacket();

                virtual bool initialize(const MQTTOptions &options) override;

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
                std::string m_clientId;
                std::string m_protocolName = "MQTT";
                uint8_t     m_protocolLevel = 4;
                uint16_t    m_keepAlive = 30;
                bool        m_cleanSession = false;
                bool        m_willFlag = false;
                bool        m_willRetain = false;
                MQTT_QOS    m_willQos;
                MQTTBuffer  m_willTopic;
                MQTTBuffer  m_willMessage;
                bool        m_usernameFlag = false;
                MQTTBuffer  m_username;
                bool        m_passwordFlag = false;
                MQTTBuffer  m_password;
        };

        using MQTTConnectPacketSPtr = std::shared_ptr<MQTTConnectPacket>;
    }
}
#endif