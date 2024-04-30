# boost dependency
# @copyright See `LICENSE` in the root directory of this project.
# This can take a while to build, consider running cmake with `-DFETCHCONTENT_QUIET=Off`

# Options in case the user wants to use their own libraries.
option(FC_BOOST_SYSTEM "Use system provided boost libraries." Off)
option(FC_BOOST_DEFAULT "Use default fetch content for boost libraries." On)

# Input values - none of these are used if FC_BOOST_SYSTEM is set to On.
#
#  FC_BOOST_URL  - The url to acquire boost from. This is only used if FC_BOOST_DEFAULT is Off. You might use this if you want an
#                  older version of boost or if you want to cache a copy on your local filesystem.
#                  Examples:
#                  https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
#                  file:///home/bailey/recv/boost-1.81.0.tar.
#
#  FC_BOOST_HASH - The SHA256 hash for confirming FC_BOOST_URL. Only used if FC_BOOST_URL is set.
#
#  BOOST_INCLUDE_LIBRARIES - The libraries to build.

# Output values:
#  LIBSB_FCMA_LIBS is updated with boost.

set(FC_BOOST_URL_DEFAULT https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz)
set(FC_BOOST_HASH_DEFAULT 2e64e5d79a738d0fa6fb546c6e5c2bd28f88d268a2a080546f74e5ff98f29d0e)


if(FC_BOOST_SYSTEM)
  # Sanity check and error out or note the library being used.
  if(FC_BOOST_DEFAULT)
    message(FATAL_ERROR "Can't have both FC_BOOST_SYSTEM and FC_BOOST_DEFAULT True at the same time.")
  endif()
  message(STATUS "Using system provided boost libraries.")
else()
  set(BOOST_ENABLE_CMAKE ON)

  # Set the libraries we want to build.
  if(NOT BOOST_INCLUDE_LIBRARIES)
    set(BOOST_INCLUDE_LIBRARIES system container)  # Add additional libraries here, e.g. url
  endif()

  if(FC_BOOST_DEFAULT)
    set(FC_BOOST_URL  ${FC_BOOST_URL_DEFAULT})
    set(FC_BOOST_HASH ${FC_BOOST_HASH_DEFAULT})
  endif()
  if(NOT FC_BOOST_URL OR NOT FC_BOOST_HASH)
    message(FATAL_ERROR "Either set FC_BOOST_DEFAULT or set FC_BOOST_URL and FC_BOOST_HASH.")
  endif()

  FetchContent_Declare(
    Boost
    SYSTEM
    URL ${FC_BOOST_URL}
    URL_HASH SHA256=${FC_BOOST_HASH}
  )
endif()

set(LIBSB_FCMA_LIBS ${LIBSB_FCMA_LIBS} Boost)
