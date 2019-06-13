/**
 * @brief MQTTClient
 * 
 * Class for MQTT Client imps
 * 
 */

#ifndef _H_MQTTCLIENT
#define _H_MQTTCLIENT

#include "IMQTTClient.h"

namespace afm {
    namespace communications {
        class MQTTClient
        {
            public:
                MQTTClient();
                virtual ~MQTTClient();

                virtual bool initialize(IMQTTClientSPtr pListener);
            private:
                IMQTTClientSPtr m_pListener = nullptr;                
        };

        using MQTTClientSPtr = std::shared_ptr<MQTTClient>;
    }
}
#endif