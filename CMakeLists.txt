cmake_minimum_required(VERSION 3.16)

project(TaskManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(window_lib STATIC
    main_window.cpp
    main_window.h
)

target_link_libraries(window_lib PRIVATE Qt6::Widgets)

add_executable(TaskManager main.cpp)

target_link_libraries(TaskManager PRIVATE 
    window_lib 
    Qt6::Widgets
)