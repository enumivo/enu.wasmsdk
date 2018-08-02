cmake_minimum_required(VERSION 3.5)
project(hello_example VERSION 1.0.0)

# if no wasm root is given use default path
if(ENU_WASMSDK_ROOT STREQUAL "" OR NOT ENU_WASMSDK_ROOT)
   set(ENU_WASMSDK_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

# append the path to the module to include
list(APPEND CMAKE_MODULE_PATH ${ENU_WASMSDK_ROOT}/lib/cmake)

#include the toolchain cmake
include(EnuWasmToolchain)

