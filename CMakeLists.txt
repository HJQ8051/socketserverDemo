cmake_minimum_required(VERSION 3.15)
project(SocketServer)

aux_source_directory(. SRC_LIST)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
add_executable(test ${SRC_LIST})

find_package(Threads REQUIRED)
target_link_libraries(test PRIVATE ${CMAKE_THREAD_LIBS_INIT})