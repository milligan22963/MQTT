/**
 * @brief MQTTFactory
 * 
 * Class for creating MQTT messages
 * 
 */

#ifndef _H_MQTT_FACTORY
#define _H_MQTT_FACTORY

#include <memory>
#include "IMQTTListener.h"
#include "IMQTTPacket.h"

namespace afm {
    namespace communications {
        class MQTTFactory;

        using MQTTFactorySPtr = std::shared_ptr<MQTTFactory>;

        class MQTTFactory
        {
            public:
                MQTTFactory();
                virtual ~MQTTFactory();
                
                static MQTTFactorySPtr getInstance();

                IMQTTPacketSPtr createPacket(MQTTPacketType packetType);
                IMQTTPacketSPtr createPacket(const MQTTBuffer &buffer);
        };
    }
}
#endif