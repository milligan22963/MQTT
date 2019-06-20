/**
 * @brief IMQTTListener
 * 
 * Interface for MQTT Client receipients
 * 
 */

#ifndef _H_IMQTTLISTENER
#define _H_IMQTTLISTENER

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <json.hpp>

namespace afm {
    namespace communications {
        enum MQTT_QOS
        {
            MQTT_QOS_0 = 0,
            MQTT_QOS_1,
            MQTT_QOS_2,
            END_MQTT_QOS
        };

        using MQTTBuffer = std::vector<uint8_t>;
        using MQTTOptions = nlohmann::json;

        class IMQTTListener
        {
            public:
                virtual ~IMQTTListener() {};

                virtual void onConnected(bool success) = 0;
                virtual void onSubscriptionAdded(bool success) = 0;
                virtual void onSubscriptionRemoved(bool success) = 0;
                virtual void onDisconnected(bool success) = 0;
                virtual void onMessageReceived(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos) = 0;
                virtual void onMessageDelivered(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos) = 0;
        };

        using IMQTTListenerSPtr = std::shared_ptr<IMQTTListener>;
    }
}
#endif