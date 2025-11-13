cmake_minimum_required(VERSION 3.16)
project(MyPocoProject)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(text_plugins SHARED
    include/base_plugin.hpp
    src/text_plugins.cpp
)

add_library(math_plugins SHARED
    include/base_plugin.hpp
    src/math_plugins.cpp
)

add_executable(POCO_app main.cpp)

target_link_libraries(POCO_app PRIVATE 
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::JSON
)

target_include_directories(text_plugins
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(math_plugins
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(text_plugins PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(math_plugins PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
