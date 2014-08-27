cmake_minimum_required(VERSION 2.8)
project(brute CXX)
set(TARGET brute)

if( ("UNIX") AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") )

    # using Clang
    message("UNIX congiguration, Clang")
    set(CMAKE_CXX_FLAGS "-m32 -std=c++11")
    set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl")
elseif( ("UNIX") AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") )

# using GCC
    message("UNIX congiguration, GCC")
    message("WARNING: gcc-multilib is required on x64 systems for the cross-compilation!")
    set(CMAKE_CXX_FLAGS "-m32 -std=c++11 -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")

elseif("WIN32")

  message("Windows configuration")
  set(CMAKE_CXX_FLAGS "/EHsc")

endif()

add_subdirectory(src)
add_subdirectory(tests/func_tests)
add_subdirectory(tests/perf_tests)
