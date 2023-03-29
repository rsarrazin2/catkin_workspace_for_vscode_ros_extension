cmake_minimum_required(VERSION 3.0.2)
project(foo)

find_package(catkin REQUIRED)

add_executable(${PROJECT_NAME}_node src/foo_node.cpp)
