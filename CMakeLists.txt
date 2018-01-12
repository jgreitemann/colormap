cmake_minimum_required(VERSION 2.8.12)
project(colormap CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

enable_testing()

include_directories(.)

add_subdirectory(test)
