/**
 * @brief
 * 
 * Main.cpp
 * 
 * AFM Software
 */

#include <atomic>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "MQTTPacketOptions.h"
#include "MQTTClient.h"

std::atomic<bool> g_done;

using namespace afm;
using namespace communications;

class TestClient : public IMQTTListener, std::enable_shared_from_this<TestClient>
{
    public:
        virtual void onConnected(bool success) override
        {
            std::cout << "Connected: ";
            if (success == true) {
                std::cout << " true";
            } else {
                std::cout << " false";
            }

            std::cout << "\n";
        }
        virtual void onSubscriptionAdded(bool success)  override
        {
            if (success == true) {
                std::cout << "Successfully added subscription.\n";
            } else {
                std::cout << "Failed to add subscription\n";
            }
        }

        virtual void onSubscriptionRemoved(bool success) override
        {

        }

        virtual void onDisconnected(bool success) override
        {

        }

        virtual void onMessageReceived(const IMQTTPacketSPtr pPacket) override
        {
            std::cout << "Message for you sir.\n";
        }

        virtual void onMessageDelivered(const IMQTTPacketSPtr pPacket) override
        {
            std::cout << "Checks in the mail\n";
        }
};

void programInterrupt(int signalNumber)
{
    if (signalNumber == SIGINT) {
        //
        g_done = true;
    }
}

int main(int argc, char *argv[])
{
    MQTTOptions mqttOptions = {
        {sc_keepAlive, 30},
        {sc_brokerURL, "ec2-18-191-138-192.us-east-2.compute.amazonaws.com"},
        {sc_clientId, "TestClient"},
        {sc_brokerPort, 1883}
    };

    int returnCode = 0;

    MQTTClientSPtr pMQTTClient = std::make_shared<MQTTClient>();
    std::shared_ptr<TestClient> pClient = std::make_shared<TestClient>();
    
    if (pMQTTClient->initialize(mqttOptions, pClient) == true) {
        std::cout << "Initialized\n";

        signal(SIGINT, programInterrupt);

        std::cout << "Creating subscriptions\n";
        // Add subscriptions
        MQTTSubscription subscription;

        std::string myTopic = "test/123";
        subscription.topic = MQTTBuffer(myTopic.begin(), myTopic.end());
        subscription.qos = MQTT_QOS::MQTT_QOS_0;

        pMQTTClient->addSubscription(subscription);

        // Now to wait until something tells us to stop waiting
        while (g_done == false) {
            // sleep
            sleep(1);
        }
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