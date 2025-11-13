cmake_minimum_required(VERSION 3.16)
project(MyPocoProject)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(test_library SHARED
    test_library.hpp
    test_library.cpp
)

add_executable(POCO_app main.cpp)

target_link_libraries(POCO_app PRIVATE 
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::JSON
    test_library
)

target_include_directories(test_library
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(test_library PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
