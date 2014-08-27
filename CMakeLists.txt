cmake_minimum_required(VERSION 2.8)
project(brute CXX)
set(TARGET brute)

if("UNIX")

  message("UNIX congiguration")
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
