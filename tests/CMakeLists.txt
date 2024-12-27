set(all_test_sources
    test.cpp
)

add_custom_target(tests)
foreach(test_source ${all_test_sources})
    # Cut off the file extension and directory path
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} PUBLIC jump)
    add_dependencies(tests ${test_name})
endforeach(test_source ${all_test_sources})

