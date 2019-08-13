/**
 * @brief MQTTProcessor
 * 
 * Class for MQTT basic MQTTProcessing
 * 
 */

#ifndef _H_MQTTPROCESSOR
#define _H_MQTTPROCESSOR

#include <atomic>
#include <list>
#include <thread>
#include "IAfmSocket.h"
#include "ISocketListener.h"
#include "IMQTTListener.h"

namespace afm {
    namespace communications {
        extern const std::string sc_processorType;
        extern const std::string sc_mqttClient;
        extern const std::string sc_mqttServer;
        
        class MQTTProcessor : public ISocketListener, public std::enable_shared_from_this<MQTTProcessor>
        {
            public:
                MQTTProcessor();
                virtual ~MQTTProcessor();

                virtual bool initialize(const MQTTOptions &options);

                virtual bool sendPacket(const IMQTTPacketSPtr pPacket);
                virtual void addListener(IMQTTListenerSPtr pListener);
                virtual void removeListener(IMQTTListenerSPtr pListener);

                /**
                 * ISocketListener interface implementation
                 */
                virtual void onConnected() override;
                virtual void onDataReceived(const SocketBuffer &socketBuffer) override;
                virtual void onDataWritten(const SocketBuffer &socketBuffer) override;
                virtual void onError(int socketError) override;
                virtual void onDisconnected() override;

                virtual bool shutdown();
    
            protected:
                void processing();

            private:
                std::atomic<bool>               m_disconnectExpected;
                std::atomic<bool>               m_threadRunning;
                std::thread                     m_processingThread;
                IAfmSocketSPtr                  m_connection = nullptr;
                std::list<IMQTTListenerSPtr>    m_listeners;
        };

        using MQTTProcessorSPtr = std::shared_ptr<MQTTProcessor>;
    }
}
#endif