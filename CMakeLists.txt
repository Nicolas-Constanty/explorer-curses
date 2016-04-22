cmake_minimum_required(VERSION 3.5)
project(explorateur)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Explorer.cpp Explorer.hpp Menu.cpp Menu.hpp IMenu.cpp IMenu.hpp)
add_executable(explorateur ${SOURCE_FILES})