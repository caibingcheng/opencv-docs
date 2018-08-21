cmake_minimum_required(VERSION 2.8)
project(demo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

#add_executable(demo ${PROJECT_SOURCE_DIR}/src/MovingAeraSeg.cpp)
add_executable(demo ${PROJECT_SOURCE_DIR}/src/ToneMap.cpp)
#add_executable(demo ${PROJECT_SOURCE_DIR}/src/Retina.cpp)


target_link_libraries(demo ${OpenCV_LIBS})