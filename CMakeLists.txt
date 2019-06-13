cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Architecture
set (ARCH amd64)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

set(SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
)

link_directories (

)

add_executable (mqttClientTest ${SRC_FILES})

install (TARGETS mqttClientTest DESTINATION ${BIN_DIR})

