# common.cmake
#   use 'includecommon.cmake' to find this file
#
# This cmake files sets the following:
#
#   CMAKE_DEBUG_POSTFIX
#
#   CMAKE_CXX_FLAGS
#
# also adds a few defines to warn/error when building
#

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0011 NEW) # allow cmake_policy PUSH and POP for included scipts

add_definitions(-D_THREAD_SAFE -D_REENTRANT)

set(CMAKE_DEBUG_POSTFIX d)  # appends d to libraries.


if( APPLE OR UNIX OR MINGW )

  # go ahead and search for 64 bit libs
  set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)

  #set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall -Werror" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" )
  #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code" )  At this time, -Wunreachable-code is probably unstable.
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  #  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant")


elseif(MSVC)
        # level 4 warnings, as errors and then ignore a bunch of warnings:
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX " )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4005 /wd4800 /wd4244 /wd4815 /wd4100 /wd4127 /wd4065 /wd4996 /wd4267 /wd4511 /wd4512 /wd4706 /wd4290 /wd4190 /wd4251 /wd4250 /wd4407" )

  #Set compatibility to Windows XP SP2.
  add_definitions(-DWINVER=0x502)
  add_definitions(-D_WIN32_WINNT=0x502)
  #Prevent warnings in various libraries.
  add_definitions(-DCRT_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)  # allow boost::uuid
  add_definitions(-DNOMINMAX)  # keeps windows.h from defining min and max, which interfere with std::min and std::max from <algorithm>
  add_definitions(-DNO_EXTRA_HTON_FUNCTIONS)
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()


# Safety and sanity checks.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. This is bad practice.")
endif()

if( NOT MESSAGE_COMMON_TELL_BUILD_TYPE )
  set(MESSAGE_COMMON_TELL_BUILD_TYPE 1)
  if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE set to: <empty>")
  else()
    message(STATUS "CMAKE_BUILD_TYPE set to: ${CMAKE_BUILD_TYPE}")
  endif()
endif()


if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
  # Release
  add_definitions(-DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
  # Release with Debug info
  add_definitions(-DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
  #
else()
  # Neither debug nor release.

  if(NOT MSVC)
    message(FATAL_ERROR "\nCMAKE_BUILD_TYPE must be set to Release, Debug, RelWithDebInfo, or MinSizeRel\n   (i.e. cmake -DCMAKE_BUILD_TYPE=Debug )\n" )
  endif()
endif()


#
# Boost Libraries support

add_definitions(-D__STDC_LIMIT_MACROS) # ensure boost doesn't #include <stdint.h> without giving us our limit macros
add_definitions(-DBOOST_ALL_NO_LIB)  # disable autolinking of libraries
#
# Build with stack and or heap analyzer's available
option(BUILD_STACK_PROTECTOR "Enable or disable certain stack protection flags" OFF)
if( BUILD_STACK_PROTECTOR )
  message(WARNING "Building with stack protection")
  if( APPLE OR UNIX OR MINGW )
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fstack-protector-all ")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all ")
  elseif(MSVC)
    # not complete yet
  endif()
endif()


#
# Build with address sanitizer enabled
option(BUILD_SANITIZE_ADDRESS "Enable or disable Address Sanitizer" OFF)
if( BUILD_SANITIZE_ADDRESS )
  message(STATUS "Building with -fsanitizer=address. See https://code.google.com/p/address-sanitizer/wiki/AddressSanitizer")
  if( APPLE OR UNIX OR MINGW )
    # add compile options
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fsanitize=address -fno-omit-frame-pointer ")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer ")
    # add link options
    set( CMAKE_EXE_LINKER_FLAGS     "${CMAKE_EXE_LINKER_FLAGS}    -fsanitize=address ")
    set( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address ")
    set( CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=address ")
  elseif(MSVC)
    # not complete yet
  endif()
endif()


#
# Build with leak sanitizer enabled
option(BUILD_SANITIZE_LEAK "Enable or disable Leak Sanitizer" OFF)
if( BUILD_SANITIZE_LEAK )
  message(STATUS "Building with -fsanitizer=leak. See https://code.google.com/p/address-sanitizer/wiki/LeakSanitizer")
  if( APPLE OR UNIX OR MINGW)
    # add compile options
    set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fsanitize=leak -fno-omit-frame-pointer ")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fno-omit-frame-pointer ")
    # add link options
    set( CMAKE_EXE_LINKER_FLAGS     "${CMAKE_EXE_LINKER_FLAGS}    -fsanitize=leak ")
    set( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=leak ")
    set( CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=leak ")
  else()
    # not available yet
    message( SEND_ERROR "BUILD_SANITIZE_LEAK set to 1 but this platform doesn't support -fsanitize=leak" )
  endif()
endif()

# add colorized output for GNU compilers
if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
  if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9 )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always" )
  endif()
endif()
