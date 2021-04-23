cmake_minimum_required(VERSION 3.5)

project(test_elektronNipk LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")


find_package(Qt5Core)
add_executable(test_elektronNipk
  main.cpp
  eliimage.h
  eliimagehandler.cpp eliimagehandler.h
  eliimageprocessing.cpp eliimageprocessing.h
)

target_link_libraries(test_elektronNipk Qt5::Core)
