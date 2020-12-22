project(task)
cmake_minimum_required(VERSION 3.3)
add_definitions(-std=c++14 -Wall -g)
find_package(nlohmann_json REQUIRED)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
#
add_executable(main src/main.cpp src/broker.cpp)
target_link_libraries(main -lsockpp -lpthread)