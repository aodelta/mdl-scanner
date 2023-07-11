cmake_minimum_required(VERSION 3.22)
project(mdl_scanner CXX)


# --------------------- cmake settings ---------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/prefs.cmake")
    include("${PROJECT_SOURCE_DIR}/cmake/prefs.cmake")
endif()



# --------------------- Variables ---------------------
set(DEP_DIR "${PROJECT_SOURCE_DIR}/dep")


# --------------------- dependency building ---------------------
include("${PROJECT_SOURCE_DIR}/cmake/deps.cmake")


# ----------------------- Project setup -----------------------
include("${PROJECT_SOURCE_DIR}/cmake/server.cmake")