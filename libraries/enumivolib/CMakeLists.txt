file(GLOB HEADERS "*.hpp"
                  "*.h")

add_library(enumivo
            enumivolib.cpp
            crypto.cpp
            ${HEADERS})

add_library(enumivo_malloc
            malloc.cpp
            ${HEADERS})

add_library(enumivo_dsm
            simple_malloc.cpp
            ${HEADERS})

add_library(enumivo_cmem
            memory.cpp
            ${HEADERS})

add_native_library(native_enumivo
                   enumivolib.cpp
                   crypto.cpp
                   malloc.cpp
                   ${HEADERS})

set_target_properties(enumivo_malloc PROPERTIES LINKER_LANGUAGE C)

target_include_directories(enumivo PUBLIC
                                 ${CMAKE_SOURCE_DIR}/libc/musl/include
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/internal
                                 ${CMAKE_SOURCE_DIR}/libc/musl/src/crypt
                                 ${CMAKE_SOURCE_DIR}/libc/musl/arch/eos
                                 ${CMAKE_SOURCE_DIR}/libcxx/include
                                 ${CMAKE_SOURCE_DIR}
                                 ${CMAKE_SOURCE_DIR}/boost/include)

target_link_libraries( enumivo c c++ )
add_dependencies( native_enumivo enumivo )

add_custom_command( TARGET enumivo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET enumivo_malloc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo_malloc> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET enumivo_dsm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo_dsm> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET enumivo_cmem POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:enumivo_cmem> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_enumivo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_enumivo> ${BASE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../enumivolib DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
