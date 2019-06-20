/**
 * @brief MQTTFactory
 * 
 * Class for creating MQTT messages
 * 
 */

#include "MQTTFactory.h"

namespace afm {
    namespace communications {
        MQTTFactory::MQTTFactory()
        {

        }

        MQTTFactory::~MQTTFactory()
        {

        }

        MQTTFactorySPtr MQTTFactory::getInstance()
        {
            static MQTTFactorySPtr pFactory = nullptr;

            if (pFactory == nullptr) {
                pFactory = std::make_shared<MQTTFactory>();
            }
            return pFactory;
        }

        IMQTTPacketSPtr MQTTFactory::createPacket(MQTTPacketType packetType)
        {
            IMQTTPacketSPtr pPacket = nullptr;

            return pPacket;
        }

        IMQTTPacketSPtr MQTTFactory::createPacket(const MQTTBuffer &buffer)
        {
            IMQTTPacketSPtr pPacket = nullptr;

            return pPacket;
        }
    }
}
