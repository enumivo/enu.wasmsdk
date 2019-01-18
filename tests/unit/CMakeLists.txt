list(APPEND CMAKE_MODULE_PATH ${ENUMIVO_CDT_BIN})
include(EnumivoCDTMacros)

add_native_executable(name_tests name_tests.cpp)
add_native_executable(system_tests system_tests.cpp)
add_native_executable(print_tests print_tests.cpp)
add_dependencies(name_tests EnumivoTools)
add_dependencies(system_tests EnumivoTools)
add_dependencies(print_tests EnumivoTools)
