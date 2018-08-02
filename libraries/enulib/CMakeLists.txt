file(GLOB HEADERS "*.hpp"
                  "*.h")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/core_symbol.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/core_symbol.hpp)

add_library(enumivo 
            enulib.cpp 
            ${HEADERS})

target_include_directories(enumivo PUBLIC 
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_link_libraries( enumivo c c++ )

install(TARGETS enumivo EXPORT EnuLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../enulib DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/core_symbol.hpp DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/enulib)
