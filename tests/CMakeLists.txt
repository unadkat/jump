set(all_test_sources
    test.cpp
)

foreach(test_source ${all_test_sources})
    # Cut off the file extension and directory path
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_include_directories(${test_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    if(use_cblas)
        target_link_libraries(${test_name} PUBLIC blas)
    endif()
    if(use_lapacke)
        target_link_libraries(${test_name} PUBLIC lapacke)
    endif()
    add_dependencies(tests ${test_name})
endforeach(test_source ${all_test_sources})

