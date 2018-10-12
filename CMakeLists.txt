cmake_minimum_required(VERSION 3.5)
project(enumivo_cdt)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 3)
set(VERSION_PATCH 1)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   message(WARNING "CMAKE_INSTALL_PREFIX is set to default path of ${CMAKE_INSTALL_PREFIX}, resetting to ${CMAKE_INSTALL_PREFIX}/enumivo.cdt")
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/enumivo.cdt")
elseif ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
   message(WARNING "CMAKE_INSTALL_PREFIX is explicitly set to /usr/local. This is not recommended.")
endif()

include(GNUInstallDirs)

include(ClangExternalProject.txt)

add_subdirectory(external)

set(WASM_SDK_BUILD true)

### Configure the EnumivoWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt/enumivo.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt/EnumivoWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/enumivo.cdt)
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/enumivo.cdt)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EnumivoWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EnumivoWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/enumivo.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/enumivo.cdt-config.cmake.package @ONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/lib/cmake/enumivo.cdt")
include(EnumivoWasmToolchain)

add_subdirectory(libraries)

include(InstallClang.txt)

configure_file(${CMAKE_SOURCE_DIR}/enumivo.imports.in ${CMAKE_BINARY_DIR}/enumivo.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/enumivo.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardenu/ricardenu.py ${CMAKE_BINARY_DIR}/scripts/ricardenu.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardenu.py DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)

set(VENDOR "enumivo.org")
set(PROJECT_NAME "enumivo.cdt")
set(DESC "Toolchain and supporting tools for the Enumivo platform")
set(URL "https://github.com/enumivo/enumivo.cdt")
set(EMAIL "admin@enumivo.org")
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_BINARY_DIR}/packages/generate_package.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_deb.sh ${CMAKE_BINARY_DIR}/packages/generate_deb.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_bottle.sh ${CMAKE_BINARY_DIR}/packages/generate_bottle.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_rpm.sh ${CMAKE_BINARY_DIR}/packages/generate_rpm.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh COPYONLY)
