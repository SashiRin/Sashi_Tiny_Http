cmake_minimum_required(VERSION 3.10)
project(Sashi_Tiny_Http)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.53.0 COMPONENTS system thread filesystem REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Sashi_Tiny_Http main.cpp mime_types.cpp server.cpp request.cpp response.cpp connection.cpp connection_manager.cpp)
    target_link_libraries(Sashi_Tiny_Http ${Boost_LIBRARIES})
endif()

