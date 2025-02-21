cmake_minimum_required(VERSION 3.20)
project(Jump VERSION 0.2.6 LANGUAGES CXX)

option(USE_LAPACKE "" ON)
option(BUILD_TESTING "" ON)
option(ENABLE_EXPERIMENTAL "" OFF)
option(ENABLE_EXPRESSION_TEMPLATES "" OFF)

include(CTest)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Defaulting build type to ${default_build_type}")
    set(CMAKE_BUILD_TYPE ${default_build_type})
else()
    message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

include(sourcelist.cmake)
add_library(jump STATIC ${sources})
target_include_directories(jump PUBLIC include)
target_compile_options(jump PRIVATE -Wall -Wextra -Wpedantic)

if(USE_LAPACKE)
    find_library(LAPACKE lapacke REQUIRED)
    target_compile_definitions(jump PRIVATE JUMP_USE_LAPACKE)
    target_link_libraries(jump PUBLIC ${LAPACKE})
endif()
if (ENABLE_EXPERIMENTAL)
    set(ENABLE_EXPRESSION_TEMPLATES ON)
endif()
if (ENABLE_EXPRESSION_TEMPLATES)
    target_compile_definitions(jump PUBLIC JUMP_ENABLE_VECTOR_EXPRESSION_TEMPLATES)
    target_compile_definitions(jump PUBLIC JUMP_ENABLE_MATRIX_EXPRESSION_TEMPLATES)
endif()

add_subdirectory(./tests)

