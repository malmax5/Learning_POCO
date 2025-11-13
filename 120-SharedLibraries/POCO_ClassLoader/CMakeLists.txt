cmake_minimum_required(VERSION 3.16)
project(MyPocoProject)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_executable(POCO_app main.cpp)

target_link_libraries(POCO_app PRIVATE 
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::JSON
)
