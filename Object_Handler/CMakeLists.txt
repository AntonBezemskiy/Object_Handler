cmake_minimum_required(VERSION 3.5)

project(Object_Processing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(inc)

set(HEADER_FILES inc/structures.h inc/list.h inc/test.h)
set(SOURCE_FILES src/main.cpp src/list.cpp src/test.cpp)

add_executable(Object_Processing ${HEADER_FILES} ${SOURCE_FILES})

install(TARGETS Object_Processing
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
