/**
 * @brief MQTTPacketOptions
 * 
 * Collection of packet options
 * 
 */

#include "MQTTPacketOptions.h"

namespace afm {
    namespace communications {

        /**
         * Tracked packet options
         */
        const std::string sc_optionMessageId = "message_id";

        /**
         * MQTT Client options
         */
        const std::string sc_brokerURL = "url";
        const std::string sc_brokerPort = "port";
        const std::string sc_persistence = "persistence";
        const std::string sc_persistencePath = "path";
        const std::string sc_persistenceBacklog = "backlog";

        /**
         * MQTT Connect options
         */
        const std::string sc_protocolName = "protocol_name";
        const std::string sc_protocolLevel = "protocol_level";
        const std::string sc_cleanSession = "clean_session";
        const std::string sc_willFlag = "will_flag";
        const std::string sc_willQOS = "will_qos";
        const std::string sc_willRetain = "will_retain";
        const std::string sc_usernameFlag = "username_flag";
        const std::string sc_passwordFlag = "password_flag";
        const std::string sc_keepAlive = "keep_alive";
        const std::string sc_clientId = "client_id";
        const std::string sc_willTopic = "will_topic";
        const std::string sc_willMessage = "will_message";
        const std::string sc_username = "username";
        const std::string sc_password = "password";

        /**
         * MQTT Connection Ack options
         */
        const std::string sc_sessionPresent = "session_present";
        const std::string sc_connectionResponse = "connection_response";

        /**
         * MQTT Subscribe packet options
         */
        const std::string sc_subscriptions = "subscriptions";

        /**
         * MQTT Publish options
         */
        const std::string sc_duplicateFlag = "duplicate";
        const std::string sc_qosLevel = "qos";
        const std::string sc_retainFlag = "retain";
        const std::string sc_topic = "topic";
        const std::string sc_message = "message";
    }
}
