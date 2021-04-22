cmake_minimum_required(VERSION 3.5)

project(test_elektronNipk LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_executable(test_elektronNipk
  main.cpp
  eliimage.h
  eliimagehandler.cpp eliimagehandler.h
  eliimageprocessing.cpp eliimageprocessing.h
)
target_link_libraries(test_elektronNipk Qt5::Core)
