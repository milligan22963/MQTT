/**
 * @brief MQTTPacketOptions
 * 
 * Collection of packet options
 * 
 */

#ifndef _H_MQTT_PACKET_OPTIONS
#define _H_MQTT_PACKET_OPTIONS

#include <string>

namespace afm {
    namespace communications {
        /**
         * Tracked packet options
         */
        extern const std::string sc_optionMessageId;

        /**
         * MQTT Client options
         */
        extern const std::string sc_brokerURL;
        extern const std::string sc_brokerPort;
        extern const std::string sc_persistence;
        extern const std::string sc_persistencePath;
        extern const std::string sc_persistenceBacklog;

        /**
         * MQTT Connect options
         */
        extern const std::string sc_protocolName;
        extern const std::string sc_protocolLevel;
        extern const std::string sc_cleanSession;
        extern const std::string sc_willFlag;
        extern const std::string sc_willQOS;
        extern const std::string sc_willRetain;
        extern const std::string sc_usernameFlag;
        extern const std::string sc_passwordFlag;
        extern const std::string sc_keepAlive;
        extern const std::string sc_clientId;
        extern const std::string sc_willTopic;
        extern const std::string sc_willMessage;
        extern const std::string sc_username;
        extern const std::string sc_password;

        /**
         * MQTT Connection Ack options
         */
        extern const std::string sc_sessionPresent;
        extern const std::string sc_connectionResponse;

        /**
         * MQTT Subscribe packet options
         */
        extern const std::string sc_subscriptions;
        
        /**
         * MQTT Publish options
         */
        extern const std::string sc_duplicateFlag;
        extern const std::string sc_qosLevel;
        extern const std::string sc_retainFlag;
        extern const std::string sc_topic;
        extern const std::string sc_message;
    }
}
#endif