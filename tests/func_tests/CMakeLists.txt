cmake_minimum_required(VERSION 2.8)
project(bruteforce_functional_test CXX)
set(TARGET bruteforce_functional_test)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")

########################################################
# source files
file(GLOB Files_src *.cpp)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src} ../../src/md5bruteforce.cpp)
source_group("Sources" FILES ${SOURCES})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../../include)
include_directories(${INCLUDE_DIRS})

########################################################
add_executable(${TARGET} ${SOURCES})

