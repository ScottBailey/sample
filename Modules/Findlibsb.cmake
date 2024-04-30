#
# Find the include directory for libsb
#
# Copyright 2015-2024 Scott Bailey
# License: BSD 2.0
#
#
# Variables (CMake) that this module consumes:
#
#   none
#
# Functions/macros defined by this module:
#
#   none
#
# Variables defined by this module:
#
#   LIBSB_FOUND
#   LIBSB_INCLUDE_DIR                   The include directory.
#
#   LIBSB_H                             The header files.
#   LIBSB_ALL_H                         The header files including detal/*
#
# Variables modified by this moudule:
#
#    none


if( libsb_FOUND )
  return()
endif()


find_path(LIBSB_INCLUDE_DIR    sb/message/accumulator.h
  PATHS
    "${CMAKE_CURRENT_LIST_DIR}/.."
    ~/bailey
  PATH_SUFFIXES
    libsb/include
    libsb
    libsb/trunk
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libsb DEFAULT_MSG
  LIBSB_INCLUDE_DIR
)


if( libsb_FOUND )
  file(GLOB LIBSB_H ${LIBSB_INCLUDE_DIR}/sb/*.h)
  file(GLOB LIBSB_ALL_H ${LIBSB_INCLUDE_DIR}/sb/*.h ${LIBSB_INCLUDE_DIR}/sb/detail/*.ipp)

  add_library(libsb INTERFACE IMPORTED)
  target_include_directories(libsb INTERFACE "${LIBSB_INCLUDE_DIR}")
endif()
