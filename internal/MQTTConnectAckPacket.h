/**
 * @brief MQTTConnectAckPacket
 * 
 * Implementation for MQTT Connect Ack packet
 * 
 */

#ifndef _H_MQTT_CONNECT_ACK_PACKET
#define _H_MQTT_CONNECT_ACK_PACKET

#include "MQTTPacket.h"

namespace afm {
    namespace communications {
        enum ConnectionResponse  : uint8_t {
            eConnectionResponse_Accepted = 0x00,
            eConnectionResponse_Refused_ProtocolVersion,
            eConnectionResponse_Refused_ClientId,
            eConnectionResponse_Refused_ServerUnavailable,
            eConnectionResponse_Refused_UserCredentials,
            eConnectionResponse_Refused_NotAuthorized,
            EndConnectionResponses
        };

        class MQTTConnectAckPacket : public MQTTPacket
        {
            public:
                MQTTConnectAckPacket();
                virtual ~MQTTConnectAckPacket();

                virtual bool initialize(const MQTTOptions &options) override;

                ConnectionResponse getResponse() const { return m_connectionResponse; }

            protected:
                virtual uint32_t getVariableLength();
                virtual bool encodePayload(MQTTBuffer &buffer);
                virtual bool decodePayload(const MQTTBuffer &buffer, uint32_t &offset, uint32_t payloadLength);

            private:
                bool                m_sessionPresent = false;
                ConnectionResponse  m_connectionResponse = ConnectionResponse::EndConnectionResponses;
        };

        using MQTTConnectAckPacketSPtr = std::shared_ptr<MQTTConnectAckPacket>;
    }
}
#endif