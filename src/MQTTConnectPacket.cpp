/**
 * @brief MQTTConnectPacket
 * 
 * Implementation for MQTT Connect packet
 * 
 */

#include "JSONUtil.h"
#include "MQTTConnectPacket.h"

namespace afm {
    namespace communications {
        MQTTConnectPacket::MQTTConnectPacket()
            : MQTTPacket()
        {
            setType(MQTTPacketType::MQTT_Connect);
        }

        MQTTConnectPacket::~MQTTConnectPacket()
        {

        }

        bool MQTTConnectPacket::initialize(const MQTTOptions &options)
        {
            bool success = MQTTPacket::initialize(options);

            if (success == true) {
                // the client id is mandatory
                success = extractValue(options, sc_clientId, m_clientId);
                if (success == true) {
                    if (extractValue(options, sc_willFlag, m_willFlag) == true) {
                        if (m_willFlag == true) {
                            // Process will related options - qos, retain, topic, message
                            if (extractValue(options, sc_qosLevel, m_willQos) == false) {
                                m_willQos = MQTT_QOS::MQTT_QOS_0;
                            }
                            if (extractValue(options, sc_retainFlag, m_willRetain)== false) {
                                m_willRetain = false;
                            }
                            if (extractValue(options, sc_topic, m_willTopic) == false) {
                                success = false;
                            }
                            if (extractValue(options, sc_message, m_willMessage) == false) {
                                m_willMessage.clear();
                            }
                        }
                    }
                    if (extractValue(options, sc_usernameFlag, m_usernameFlag) == true) {
                        if (m_usernameFlag == true) {
                            if (extractValue(options, sc_username, m_username) == false) {
                                success = false;
                            }
                        }
                    }
                    if (extractValue(options, sc_passwordFlag, m_passwordFlag) == true) {
                        if (m_passwordFlag == true) {
                            if (extractValue(options, sc_password, m_password) == false) {
                                success = false;
                            }
                        }
                    }

                    extractValue(options, sc_protocolName, m_protocolName);
                    extractValue(options, sc_protocolLevel, m_protocolLevel);
                    extractValue(options, sc_cleanSession, m_cleanSession);
                    extractValue(options, sc_keepAlive, m_keepAlive);
                }
            }

            return success;
        }

        /**
         * Protected parts
         */
        uint32_t MQTTConnectPacket::getVariableLength()
        {
            return 0;
        }

        bool MQTTConnectPacket::encodePayload(MQTTBuffer &buffer)
        {
            return false;
        }

        bool MQTTConnectPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            return false;
        }
    }
}
