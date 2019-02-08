cmake_minimum_required(VERSION 3.13)
project(Sashi_Tiny_Http)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.53.0 COMPONENTS system thread filesystem REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Sashi_Tiny_Http main.cpp mime_types.cpp)
    target_link_libraries(Sashi_Tiny_Http ${Boost_LIBRARIES})
endif()

