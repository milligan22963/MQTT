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
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

set(LIB_FILES
    src/MQTTClient.cpp
    src/MQTTFactory.cpp
    src/MQTTPacket.cpp
    src/MQTTPacketOptions.cpp
    src/MQTTProcessor.cpp
    src/MQTTTrackedPacket.cpp
)

set(SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
    ${JSON_INCLUDE}
)

link_directories (
    ${LIB_DIR}
)

add_library (MQTT ${LIB_FILES})
add_executable (mqttClientTest ${SRC_FILES})

target_link_libraries(
    mqttClientTest
    MQTT
    pthread
)

install (TARGETS mqttClientTest DESTINATION ${BIN_DIR})

