configure_file(${CMAKE_CURRENT_SOURCE_DIR}/enumivo-cc.cpp.in ${CMAKE_BINARY_DIR}/enumivo-cc.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/enumivo-cpp.cpp.in ${CMAKE_BINARY_DIR}/enumivo-cpp.cpp)

add_tool(enumivo-cc)
add_tool(enumivo-cpp)
