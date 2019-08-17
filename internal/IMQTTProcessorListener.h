/**
 * @brief IMQTTProcessorListener
 * 
 * Internal interface for MQTT Client/Server implementations
 * 
 */

#ifndef _H_IMQTTPROCESSOR_LISTENER
#define _H_IMQTTPROCESSOR_LISTENER

#include <memory>

#include "IMQTTPacket.h"

namespace afm {
    namespace communications {
        class IMQTTProcessListener
        {
            public:
                virtual ~IMQTTProcessListener() {}

                virtual void onConnected() = 0;
                virtual void onMessageReceived(const IMQTTPacketSPtr pPacket) = 0;
                virtual void onMessageDelivered(const IMQTTPacketSPtr pPacket) = 0;
                virtual void onDisconnected() = 0;
                virtual void onError() = 0;
        };

        using IMQTTProcessListenerSPtr = std::shared_ptr<IMQTTProcessListener>;
    }
}
#endif