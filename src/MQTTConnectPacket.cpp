/**
 * @brief MQTTConnectPacket
 * 
 * Implementation for MQTT Connect packet
 * 
 */

#include <set>
#include "JSONUtil.h"
#include "MQTTConnectPacket.h"

namespace afm {
    namespace communications {

        enum ConnectionFlags {
            ConnectFlag_Reserved        = 0x01,
            ConnectFlag_cleanSession    = 0x02,
            ConnectFlag_willFlag        = 0x04,
            ConnectFlag_willRetain      = 0x20,
            ConnectFlag_passwordFlag    = 0x40,
            ConnectFlag_usernameFlag    = 0x80
        };

        inline uint8_t setConnectQOS(MQTT_QOS qos)
        {
            return qos << 3;
        }

        inline MQTT_QOS getConnectQOS(uint8_t qos)
        {
            return (MQTT_QOS)(qos >> 3);
        }

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
            uint32_t packetLength = calculateFieldLength(m_protocolName);

            packetLength += sizeof(m_protocolLevel);
            packetLength += sizeof(uint8_t); // connect flag
            packetLength += sizeof(m_keepAlive);

            packetLength += calculateFieldLength(m_clientId);
            if (m_willFlag == true) {
                packetLength += calculateFieldLength(m_willTopic);
                packetLength += calculateFieldLength(m_willMessage);
            }
            if (m_usernameFlag == true) {
                packetLength += calculateFieldLength(m_username);
            }
            if (m_passwordFlag == true) {
                packetLength += calculateFieldLength(m_password);
            }

            return packetLength;
        }

        bool MQTTConnectPacket::encodePayload(MQTTBuffer &buffer)
        {
            std::set<bool> results;

            if (encodeData(buffer, m_protocolName) == true) {
                buffer.push_back(m_protocolLevel);

                uint8_t connectFlag = 0;
                
                if (m_willFlag == true) {
                    connectFlag = setConnectQOS(m_willQos);
                    connectFlag |= ConnectionFlags::ConnectFlag_willFlag;

                    if (m_willRetain == true) {
                        connectFlag |= ConnectionFlags::ConnectFlag_willRetain;
                    }
                }

                if (m_cleanSession == true) {
                    connectFlag |= ConnectionFlags::ConnectFlag_cleanSession;
                }

                if (m_passwordFlag == true) {
                    connectFlag |= ConnectionFlags::ConnectFlag_passwordFlag;
                }

                if (m_usernameFlag == true) {
                    connectFlag |= ConnectionFlags::ConnectFlag_usernameFlag;
                }

                buffer.push_back(connectFlag);

                buffer.push_back(HighByte(m_keepAlive));
                buffer.push_back(LowByte(m_keepAlive));

                results.insert(encodeData(buffer, m_clientId));

                if (m_willFlag == true) {
                    results.insert(encodeData(buffer, m_willTopic));
                    results.insert(encodeData(buffer, m_willMessage));
                }

                if (m_usernameFlag == true) {
                    results.insert(encodeData(buffer, m_username));
                }

                if (m_passwordFlag == true) {
                    results.insert(encodeData(buffer, m_password));
                }
            }
            return results.find(false) == results.end() ? true : false;
        }

        bool MQTTConnectPacket::decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength)
        {
            std::set<bool> results;

            results.insert(decodeData(buffer, offset, m_protocolName));

            m_protocolLevel = buffer[offset++];

            uint8_t connectFlags = buffer[offset++];

            m_cleanSession = (connectFlags & ConnectionFlags::ConnectFlag_cleanSession) != 0 ? true : false;

            m_keepAlive = buffer[offset++] << 8;
            m_keepAlive |= buffer[offset++];

            results.insert(decodeData(buffer, offset, m_clientId));

            if (connectFlags & ConnectionFlags::ConnectFlag_willFlag) {
                m_willFlag = true;
                m_willRetain = (connectFlags & ConnectionFlags::ConnectFlag_willRetain) != 0 ? true : false;
                m_willQos = getConnectQOS(connectFlags);
                results.insert(decodeData(buffer, offset, m_willTopic));
                results.insert(decodeData(buffer, offset, m_willMessage));
            } else {
                m_willFlag = false;
                m_willRetain = false;
                m_willQos = MQTT_QOS::MQTT_QOS_0;
            }

            if (connectFlags & ConnectionFlags::ConnectFlag_usernameFlag) {
                m_usernameFlag = true;
                results.insert(decodeData(buffer, offset, m_username));
            }

            if (connectFlags & ConnectionFlags::ConnectFlag_passwordFlag) {
                m_passwordFlag = true;
                results.insert(decodeData(buffer, offset, m_password));
            }

            return results.find(false) == results.end() ? true : false;
        }
    }
}
