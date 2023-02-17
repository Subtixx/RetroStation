# test dependencies
set(tests_DEPENDENCIES_CONFIGURED Catch2)

foreach(DEPENDENCY ${tests_DEPENDENCIES_CONFIGURED})
    find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach()

include(CTest)
include(Catch)

# calling es-core executable directly
add_test(
        NAME es-core_test
        COMMAND es-core_exe ""
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# testing helpers
add_executable(es-core_helpers_tests "./tests.cpp")

# because es-core helpers and dependencies are private, we should link them here again
target_include_directories(es-core_helpers_tests PRIVATE "${es-core_INCLUDE_DIR}")

target_link_libraries(
        es-core_helpers_tests
        PRIVATE project_warnings
        project_options
        Catch2::Catch2
        ${es-core_LINKED_LIBRARIES})

# generate a main function for the test executable
target_compile_definitions(es-core_helpers_tests PRIVATE CATCH_CONFIG_MAIN DO_NOT_USE_WMAIN)

# use xml reporter if coverage is enabled
if(${ENABLE_COVERAGE})
    set(COVERAGE_ARGS REPORTER xml)
endif()