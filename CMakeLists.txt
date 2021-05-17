cmake_minimum_required(VERSION 3.5)

project(TestImageProcessing LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")


add_executable(${PROJECT_NAME}
  main.cpp
  image.h
  TestTimer.h
  imagemath.h
  image.cpp)

