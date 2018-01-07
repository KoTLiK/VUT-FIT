cmake_minimum_required(VERSION 3.7.2)
project(isa)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -pedantic -Wall")

set(SOURCE_FILES src/isabot.cpp src/common/common.h src/common/functions.cpp src/config/Config.cpp src/config/Config.h src/common/functions.h src/client/Client.cpp src/client/Client.h src/protocol/Controller.cpp src/protocol/Controller.h src/client/ClientProtocol.h)
add_executable(isabot ${SOURCE_FILES})
