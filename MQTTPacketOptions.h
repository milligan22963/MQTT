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

        /**
         * MQTT Publish options
         */
        extern const std::string sc_duplicateFlag;
        extern const std::string sc_qosLevel;
        extern const std::string sc_retainFlag;
    }
}
#endif