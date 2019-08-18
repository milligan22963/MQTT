/**
 * @brief MQTTTypes.h
 * 
 * Types for MQTT
 * 
 */

#ifndef _H_MQTTTYPES
#define _H_MQTTTYPES

#include <cstdint>
#include <vector>

#include <json.hpp>

namespace afm {
    namespace communications {
        enum MQTT_QOS : uint8_t
        {
            MQTT_QOS_0 = 0,
            MQTT_QOS_1,
            MQTT_QOS_2,
            MQTT_QOS_FAILURE = 0x80,
            END_MQTT_QOS
        };

        using MQTTBuffer = std::vector<uint8_t>;
        using MQTTOptions = nlohmann::json;

        struct MQTTSubscription {
            MQTTBuffer topic;
            MQTT_QOS qos;
        };
    }
}
#endif