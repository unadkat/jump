cmake_minimum_required(VERSION 3.20)
project(Jump VERSION 0.3.1 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL)
    set(JUMP_STANDALONE TRUE)
endif()

option(JUMP_USE_LAPACKE "Use liblapacke as linear algebra backend" ON)
option(JUMP_ENABLE_EXPERIMENTAL "Enable all experimental features" OFF)
option(JUMP_ENABLE_SMALL_VEC "Enable small vectors (experimental)" OFF)

if (JUMP_STANDALONE)
    option(JUMP_BUILD_TESTING "Build test drivers" ${BUILD_TESTING})
else()
    option(JUMP_BUILD_TESTING "Build test drivers" FALSE)
endif()

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Defaulting ${PROJECT_NAME} build type to ${default_build_type}")
    set(CMAKE_BUILD_TYPE ${default_build_type})
else()
    message(STATUS "${PROJECT_NAME} build type is ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

include(sourcelist.cmake)
add_library(jump STATIC ${sources})
target_include_directories(jump PUBLIC include)
target_compile_options(jump PRIVATE -Wall -Wextra -Wpedantic)

if(JUMP_USE_LAPACKE)
    find_library(LAPACKE lapacke REQUIRED)
    target_compile_definitions(jump PRIVATE JUMP_USE_LAPACKE)
    target_link_libraries(jump PUBLIC ${LAPACKE})
endif()
if (JUMP_ENABLE_EXPERIMENTAL)
    set(JUMP_ENABLE_SMALL_VEC ON)
endif()
if (JUMP_ENABLE_SMALL_VEC)
    target_compile_definitions(jump PUBLIC JUMP_ENABLE_SMALL_VEC)
endif()

if (JUMP_BUILD_TESTING)
    include(CTest)
    add_subdirectory(./tests)
endif()

