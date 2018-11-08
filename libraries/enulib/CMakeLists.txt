file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(enumivo
            enulib.cpp
            crypto.cpp
            ${HEADERS})

target_include_directories(enumivo PUBLIC
                                 "$<BUILD_INTERFACE:${STANDARD_INCLUDES}>"
                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

target_link_libraries( enumivo c c++ )

install(TARGETS enumivo EXPORT EnuLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../enulib DESTINATION ${CMAKE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
add_custom_command( TARGET enumivo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo> ${CMAKE_BINARY_DIR}/lib )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../enulib DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
