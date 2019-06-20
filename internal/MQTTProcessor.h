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
#include "IMQTTListener.h"

namespace afm {
    namespace communications {
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
                std::atomic<bool> m_threadRunning;
                std::thread m_processingThread;
        };

        using MQTTProcessorSPtr = std::shared_ptr<MQTTProcessor>;
    }
}
#endif