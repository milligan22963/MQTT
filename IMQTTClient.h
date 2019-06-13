/**
 * @brief IMQTTClient
 * 
 * Interface for MQTT Client receipients
 * 
 */

#ifndef _H_IMQTTCLIENT
#define _H_IMQTTCLIENT

#include <memory>

namespace afm {
    namespace communications {
        class IMQTTClient
        {
            public:
                virtual ~IMQTTClient() {};
        };

        using IMQTTClientSPtr = std::shared_ptr<IMQTTClient>;
    }
}
#endif