cmake_minimum_required(VERSION 3.16)
project(AMPLE-Lib VERSION "0.2.2")

set(CMAKE_C_STANDARD 99)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)