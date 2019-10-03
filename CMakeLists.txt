cmake_minimum_required(VERSION 2.8.12)
project(colormap CXX)
set(CMAKE_CXX_STANDARD 14)

enable_testing()

include_directories(include)

add_subdirectory(test)

install(DIRECTORY include/colormap DESTINATION include)