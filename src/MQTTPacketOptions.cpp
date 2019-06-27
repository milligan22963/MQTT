/**
 * @brief MQTTPacketOptions
 * 
 * Collection of packet options
 * 
 */

#include "MQTTPacketOptions.h"

namespace afm {
    namespace communications {
        const std::string sc_optionMessageId = "message_id";
        const std::string sc_brokerURL = "url";
        const std::string sc_brokerPort = "port";
        const std::string sc_duplicateFlag = "duplicate";
        const std::string sc_qosLevel = "qos";
        const std::string sc_retainFlag = "retain";
    }
}
