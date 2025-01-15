cmake_minimum_required(VERSION 3.20)
project(Jump VERSION 0.2.6 LANGUAGES CXX)

option(USE_CBLAS "" OFF)
option(USE_LAPACKE "" ON)
option(BUILD_TESTING "" OFF)

include(CTest)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Defaulting build type to ${default_build_type}")
    set(CMAKE_BUILD_TYPE ${default_build_type})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include(sourcelist.cmake)
add_library(jump STATIC ${sources})
target_include_directories(jump PUBLIC include)
if(USE_CBLAS)
    add_compile_definitions(JUMP_USE_CBLAS)
    target_link_libraries(jump PUBLIC blas)
endif()
if(USE_LAPACKE)
    add_compile_definitions(JUMP_USE_LAPACKE)
    target_link_libraries(jump PUBLIC lapacke)
endif()

add_subdirectory(./tests)

