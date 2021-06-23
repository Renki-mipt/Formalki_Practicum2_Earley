cmake_minimum_required(VERSION 2.8)
project(mipt-flat-prac2)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR})

find_package(GTest REQUIRED)

if(NOT GTest_FOUND)
  message(SEND_ERROR "Failed to find gtest library")
  return()
else()
  include_directories(${GTEST_INCLUDE_DIRS})
endif()

add_library(EarleyLib STATIC Earley.h Earley.cpp)
add_executable(MainTest main_test.cpp)
add_executable(main main.cpp)
target_link_libraries(main EarleyLib)
target_link_libraries(MainTest EarleyLib)
target_link_libraries(MainTest ${GTEST_LIBRARIES} pthread)

enable_testing()
