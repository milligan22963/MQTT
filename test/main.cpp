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
        {sc_brokerURL, "ec2-18-191-138-192.us-east-2.compute.amazonaws.com"},
        {sc_brokerPort, 1883}
    };

    int returnCode = 0;

    MQTTClientSPtr pMQTTClient = std::make_shared<MQTTClient>();

    if (pMQTTClient->initialize(mqttOptions) == true) {
        std::cout << "Initialized\n";

        signal(SIGINT, programInterrupt);

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

    return returnCode;
}