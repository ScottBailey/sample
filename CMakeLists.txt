# This is a simple CMake file to exercise some sample data

cmake_minimum_required(VERSION 3.25)
project(libsb CXX)

option(BUILD_TEST "Build the tests." On)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Modules" ${CMAKE_MODULE_PATH})

# Add third party dependencies BEFORE adding wall & werror.
add_subdirectory(third_party)


# Warnings as errors.
include(Modules/common.cmake)


if(BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
  add_subdirectory(benchmark)
else()
  message(WARNING "Tests are not being built.")
endif()
