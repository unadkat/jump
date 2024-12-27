cmake_minimum_required(VERSION 3.20)

project(Jump VERSION 0.2.1 LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(use_cblas False)
set(use_lapacke True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(sourcelist.cmake)
add_library(jump STATIC ${sources})
target_include_directories(jump PUBLIC include)
if(use_cblas)
    add_compile_definitions(JUMP_USE_CBLAS)
    target_link_libraries(jump PUBLIC blas)
endif()
if(use_lapacke)
    add_compile_definitions(JUMP_USE_LAPACKE)
    target_link_libraries(jump PUBLIC lapacke)
endif()

add_subdirectory(./tests EXCLUDE_FROM_ALL)

