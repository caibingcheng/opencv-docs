cmake_minimum_required(VERSION 2.8)
project(demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

add_executable(demo ${PROJECT_SOURCE_DIR}/src/main.cpp)

