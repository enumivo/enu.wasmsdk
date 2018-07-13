file(GLOB HEADERS "*.hpp"
                  "*.h")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp)

add_library(enumivo 
            enulib.cpp 
            ${HEADERS})

target_include_directories(enumivo PUBLIC 
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${SDK_INSTALL_PREFIX}/include/>)

target_link_libraries( enumivo c c++ )

install(TARGETS enumivo EXPORT EnuLib
   LIBRARY DESTINATION ${SDK_INSTALL_PREFIX}/lib/
   ARCHIVE DESTINATION ${SDK_INSTALL_PREFIX}/lib/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../enulib DESTINATION ${SDK_INSTALL_PREFIX}/include)
