cmake_minimum_required(VERSION 3.28)

project(echoserver)

# ==========
# BEGIN: Client
# ==========
add_executable(es_client)

target_compile_features(es_client
    PRIVATE
        cxx_std_20
)

target_sources(es_client
    PRIVATE
        src/client.cpp
        src/reader.cpp
        src/socket.cpp
        src/writer.cpp
)

# ==========
# BEGIN: Server
# ==========

add_executable(es_server)

target_compile_features(es_server
    PRIVATE
        cxx_std_20
)

target_sources(es_server
    PRIVATE
        src/reader.cpp
        src/server.cpp
        src/socket.cpp
        src/writer.cpp
)
