cmake_minimum_required(VERSION 2.8)
project(brute CXX)
set(TARGET brute)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")


########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../include)
include_directories(${INCLUDE_DIRS})


########################################################
# source files
file(GLOB Files_src *.cpp ../include/md5bruteforce/*.h)
set(SOURCES ${SOURCES} ${Files_src})
source_group("Sources" FILES ${SOURCES})



########################################################
add_executable(${TARGET} ${SOURCES})
