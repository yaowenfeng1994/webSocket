cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(webSocket)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(utils/json)

add_executable(webSocket main.cpp src/socket_interface.cpp src/debug_log.cpp src/web_socket_handler.cpp
        src/web_socket_request.cpp src/sha1.cpp src/base64.cpp src/web_socket_respond.cpp utils/lib_json/json_reader.cpp
        utils/lib_json/json_writer.cpp utils/lib_json/json_value.cpp)