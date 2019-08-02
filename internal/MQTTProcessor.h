/**
 * @brief MQTTProcessor
 * 
 * Class for MQTT basic MQTTProcessing
 * 
 */

#ifndef _H_MQTTPROCESSOR
#define _H_MQTTPROCESSOR

#include <atomic>
#include <thread>
#include "IAfmSocket.h"
#include "IMQTTListener.h"

namespace afm {
    namespace communications {
        extern const std::string sc_processorType;
        extern const std::string sc_mqttClient;
        extern const std::string sc_mqttServer;
        
        class MQTTProcessor : public std::enable_shared_from_this<MQTTProcessor>
        {
            public:
                MQTTProcessor();
                virtual ~MQTTProcessor();

                virtual bool initialize(const MQTTOptions &options);

                virtual bool shutdown();
    
            protected:
                void processing();

            private:
                std::atomic<bool>   m_threadRunning;
                std::thread         m_processingThread;
                IAfmSocketSPtr      m_connection = nullptr;
        };

        using MQTTProcessorSPtr = std::shared_ptr<MQTTProcessor>;
    }
}
#endif