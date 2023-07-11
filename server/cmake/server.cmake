set(SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")


# Deps
set(LIBS
    "${JSON_LIBS}"
    "${FMT_LIBS}"
    "${SQLITE3_LIBS}"
    "${LIBHTTPSERVER_LIBS}"
    "${RANDOM_LIBS}"
)

set(INCLUDES
    "/usr/local/include/"
)


# Sources
set(SOURCES "")
include("${SOURCES_DIR}/.cmake")

list(APPEND INCLUDES SOURCES_DIR)


# Building
add_executable(mdl_scanner-server ${SOURCES})


target_link_libraries(mdl_scanner-server ${LIBS})
target_include_directories(mdl_scanner-server PRIVATE ${INCLUDES})


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(mdl_scanner-server PRIVATE "-Wall" "-Wextra" "-pedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(mdl_scanner-server PRIVATE "/MP16" "/WX" "/options:strict")
endif()


target_compile_definitions(mdl_scanner-server
    PUBLIC
        $<$<CONFIG:Debug>:MDL_SERVER_DEBUG>
)