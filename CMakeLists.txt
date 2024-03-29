cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Architecture
set (ARCH amd64)

set (CMAKE_CXX_STANDARD 14)

set (JSON_INCLUDE /usr/local/include/nlohmann)

# Output for install
set(LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set(BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

set(AFM_SOCKET_INCLUDE ${CMAKE_SOURCE_DIR}/../afmSocket)
set(AFM_SOCKET_LIB ${CMAKE_SOURCE_DIR}/../afmSocket/cmake-release)

set(LIB_FILES
    src/ProcessLock.cpp
    src/MQTTClient.cpp
    src/MQTTConnectPacket.cpp
    src/MQTTConnectAckPacket.cpp
    src/MQTTDisconnectPacket.cpp
    src/MQTTFactory.cpp
    src/MQTTPacket.cpp
    src/MQTTPacketOptions.cpp
    src/MQTTPingRequestPacket.cpp
    src/MQTTPingResponsePacket.cpp
    src/MQTTProcessor.cpp
    src/MQTTPublishPacket.cpp
    src/MQTTPublishAckPacket.cpp
    src/MQTTPublishCompletedPacket.cpp
    src/MQTTPublishReceivedPacket.cpp
    src/MQTTPublishReleasedPacket.cpp
    src/MQTTSubscribePacket.cpp
    src/MQTTSubscribeAckPacket.cpp
    src/MQTTTrackedPacket.cpp
    src/MQTTUnsubscribePacket.cpp
    src/MQTTUnsubscribeAckPacket.cpp
)

set(SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
    ${JSON_INCLUDE}
    ${AFM_SOCKET_INCLUDE}
)

link_directories (
    ${LIB_DIR}
    ${AFM_SOCKET_LIB}
)

add_library (MQTT ${LIB_FILES})
add_executable (mqttClientTest ${SRC_FILES})

target_link_libraries(
    mqttClientTest
    MQTT
    AFMSocket
    pthread
)

install (TARGETS mqttClientTest DESTINATION ${BIN_DIR})

