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
        extern const std::string sc_optionMessageId;
        extern const std::string sc_brokerURL;
        extern const std::string sc_brokerPort;
    }
}
#endif