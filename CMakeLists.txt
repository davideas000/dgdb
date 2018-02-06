# Last modified: date
cmake_minimum_required(VERSION 3.0)
project( projectName )
set( CMAKE_CXX_FLAGS "-g -Wall -std=c++14" )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -fcolor-diagnostics -Wno-c++98-compat" )

    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pedantic -Wno-unused-parameter" )

endif()

include_directories( "include" "../../git/Catch/include/" )
include_directories( "/media/windows/Users/HP/Desktop/programming/cpp/projects" )

# set( local_libs directory_operations )

add_subdirectory( src )

add_subdirectory( tests/others )
add_subdirectory( tests/catch )

add_subdirectory( extras )
