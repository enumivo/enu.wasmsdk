file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(enumivo
            enulib.cpp
            crypto.cpp
            ${HEADERS})

add_native_library(native_enumivo
                   enulib.cpp
                   crypto.cpp
                   ${HEADERS})

target_include_directories(enumivo PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 {CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_include_directories(native_enumivo PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( enumivo c c++ )
target_link_libraries( native_enumivo native_c native_c++ )
add_dependencies( native_enumivo enumivo )


add_custom_command( TARGET enumivo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_enumivo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_enumivo> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../enulib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
