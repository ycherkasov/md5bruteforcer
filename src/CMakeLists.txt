cmake_minimum_required(VERSION 2.8)
project(brute CXX)
set(TARGET brute)
 
cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

if(UNIX)
message("Unix")
  set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
  set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")

elseif(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc")
  set(MY_BOOST_DIR "C:/boost/boost_1_55_0")

endif()


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