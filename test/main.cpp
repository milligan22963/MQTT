/**
 * @brief
 * 
 * Main.cpp
 * 
 * AFM Software
 */

#include <iostream>

#include "MQTTPacketOptions.h"
#include "MQTTClient.h"

using namespace afm;
using namespace communications;

class TestClient : public IMQTTListener, std::enable_shared_from_this<TestClient>
{
    public:
        virtual void onConnected(bool success) override
        {

        }
        virtual void onSubscriptionAdded(bool success)  override
        {

        }

        virtual void onSubscriptionRemoved(bool success) override
        {

        }

        virtual void onDisconnected(bool success) override
        {

        }

        virtual void onMessageReceived(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos) override
        {

        }

        virtual void onMessageDelivered(const MQTTBuffer &topic, const MQTTBuffer &message, MQTT_QOS qos) override
        {

        }
};

int main(int argc, char *argv[])
{
    MQTTOptions mqttOptions = {
        {sc_brokerURL, "localhost"},
        {sc_brokerPort, 1883}
    };

    int returnCode = 0;
    std::shared_ptr<TestClient> pClient = std::make_shared<TestClient>();

    MQTTClientSPtr pMQTTClient = std::make_shared<MQTTClient>();

    if (pMQTTClient->initialize(mqttOptions, pClient) == true) {
        std::cout << "Initialized\n";
    } else {
        std::cout << "Failed to init\n";
    }

    std::cout << "Shutting down mqttclient\n";
    pMQTTClient->shutdown();

    std::cout << "Clearing mqttclient\n";

    pMQTTClient = nullptr;
    
    pClient = nullptr;

    return returnCode;
}