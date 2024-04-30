cmake_minimum_required(VERSION 3.25)

set(target_name test_accum)


find_package(libsb REQUIRED)

if( NOT WIN32 AND NATIVE_SIZE EQUAL 64 )
  set( EXTRA_LIBS rt)
endif()

# The test include dir:
include_directories( src )

file(GLOB_RECURSE src_cpp *.cpp )
file(GLOB_RECURSE src_h   *.h)

add_executable(${target_name} ${src_cpp} ${src_h} )
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 17)

target_link_libraries(${target_name}
  libsb
  ${EXTRA_LIBS}
)

if(BUILD_USE_DEVECTOR)
  target_link_libraries(${target_name}
    Boost::container
  )
  target_compile_definitions(${target_name} PRIVATE
    -DUSE_BOOST_DEVECTOR=1
  )
endif()

add_test(NAME "${target_name}" COMMAND ${target_name})
