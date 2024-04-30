# @copyright See `LICENSE` in the root directory of this project.
# third_party/CMakeLists.txt


# Default FetchContent to Verbose. But don't override a user's explicit setting.
if(NOT FETCHCONTENT_QUIET)
  set(FETCHCONTENT_QUIET Off)
endif()


# Download CPM.cmake
file(
  DOWNLOAD      https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)


# Boost libraries
#  This speaks for itself.
set(CMAKE_FOLDER "${BASE_FOLDER}/Boost")
CPMAddPackage(
  NAME     Boost
  URL      https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
  URL_HASH SHA256=2e64e5d79a738d0fa6fb546c6e5c2bd28f88d268a2a080546f74e5ff98f29d0e
  SYSTEM   True
  OPTIONS
    "BOOST_ENABLE_CMAKE ON"
    "BUILD_SHARED_LIBS Off"
    "BOOST_INCLUDE_LIBRARIES system\\\;container\\\;circular_buffer"  # Note the escapes!
)


# Google's benchmarking tool
set(CMAKE_FOLDER "${BASE_FOLDER}/benchmark")
CPMAddPackage(
  NAME     benchmark
  URL      https://github.com/google/benchmark/archive/refs/tags/v1.8.3.tar.gz
  URL_HASH SHA256=6bc180a57d23d4d9515519f92b0c83d61b05b5bab188961f36ac7b06b0d9e9ce
  SYSTEM   True
  OPTIONS
  "BENCHMARK_ENABLE_INSTALL Off"
  "BENCHMARK_ENABLE_TESTING Off"
  "BENCHMARK_INSTALL_DOCS Off"
  "BENCHMARK_ENABLE_GTEST_TESTS Off"
)
