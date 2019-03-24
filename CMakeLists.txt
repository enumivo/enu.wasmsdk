cmake_minimum_required(VERSION 3.5)
project(enumivo_cdt)

find_program(SCCACHE_FOUND sccache)
if (SCCACHE_FOUND)
   message(STATUS "Using sccache")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK sccache)
else()
   find_program(CCACHE_FOUND ccache)
   if (CCACHE_FOUND)
      message(STATUS "Using ccache")
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
   endif()
endif()


set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)
set(VERSION_PATCH 1)

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   message(WARNING "CMAKE_INSTALL_PREFIX is set to default path of ${CMAKE_INSTALL_PREFIX}, resetting to ${CMAKE_INSTALL_PREFIX}/enumivo.cdt")
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/enumivo.cdt")
elseif ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
   message(WARNING "CMAKE_INSTALL_PREFIX is explicitly set to /usr/local. This is not recommended.")
endif()

include(GNUInstallDirs)

include(modules/ClangExternalProject.txt)
include(modules/ToolsExternalProject.txt)

set(WASM_SDK_BUILD true)

### Configure the EnumivoWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt/enumivo.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt/EnumivoCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt/EnumivoWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/enumivo.cdt)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/enumivo.cdt)
install(FILES ${CMAKE_BINARY_DIR}/modules/EnumivoCDTMacros.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/enumivo.cdt)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake.package @ONLY)

include(modules/LibrariesExternalProject.txt)

include(modules/InstallCDT.cmake)

configure_file(${CMAKE_SOURCE_DIR}/imports/enumivo.imports.in ${CMAKE_BINARY_DIR}/enumivo.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/enumivo.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardenu/ricardenu.py ${CMAKE_BINARY_DIR}/scripts/ricardenu.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardenu.py DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)

# section for package construction
set(VENDOR "enumivo.org")
set(PROJECT_NAME "enumivo.cdt")
set(DESC "Toolchain and supporting tools for the Enumivo platform")
set(URL "https://github.com/enumivo/enumivo.cdt")
set(EMAIL "admin@enumivo.org")

# add licenses
configure_file(${CMAKE_SOURCE_DIR}/enumivo_llvm/LICENSE.TXT ${CMAKE_BINARY_DIR}/licenses/llvm.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/external/wabt/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/jsoncons/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/enumivo.cdt.license COPYONLY)

include(modules/TestsExternalProject.txt)

include(CTest)
enable_testing()
add_subdirectory(tests)
