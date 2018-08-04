cmake_minimum_required(VERSION 3.5)
project(hello_example VERSION 1.0.0)

# if no wasm root is given use default path
if(ENUMIVO_WASMSDK_ROOT STREQUAL "" OR NOT ENUMIVO_WASMSDK_ROOT)
   set(ENUMIVO_WASMSDK_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

# append the path to the module to include
list(APPEND CMAKE_MODULE_PATH ${ENUMIVO_WASMSDK_ROOT}/lib/cmake)

#include the toolchain cmake
include(EnumivoWasmToolchain)

add_executable( hello hello.cpp )
