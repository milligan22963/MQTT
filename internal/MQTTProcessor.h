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
#include "IAfmSocket.h"
#include "ISocketListener.h"
#include "IMQTTProcessorListener.h"

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
                virtual void addListener(IMQTTProcessListenerSPtr pListener);
                virtual void removeListener(IMQTTProcessListenerSPtr pListener);

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
                IAfmSocketSPtr                      m_connection = nullptr;
                std::list<IMQTTProcessListenerSPtr> m_listeners;
        };

        using MQTTProcessorSPtr = std::shared_ptr<MQTTProcessor>;
    }
}
#endif