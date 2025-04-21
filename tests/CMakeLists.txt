set(all_test_sources
    dual.cpp
    linear_algebra.cpp
    matrix.cpp
    sandbox.cpp
    vector.cpp
)

foreach(test_source ${all_test_sources})
    # Cut off the file extension and directory path
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_compile_options(${test_name} PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(${test_name} PRIVATE jump)

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach(test_source ${all_test_sources})
