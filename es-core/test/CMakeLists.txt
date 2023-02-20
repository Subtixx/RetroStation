find_package(GTest CONFIG REQUIRED)

include(CTest)
include(GoogleTest)

FILE(GLOB_RECURSE ES_CORE_TEST_SOURCES LIST_DIRECTORIES false *.cpp)
FILE(GLOB_RECURSE ES_CORE_TEST_HEADERS LIST_DIRECTORIES false *.h)

add_executable(es_core_tests ${ES_CORE_TEST_SOURCES} ${ES_CORE_TEST_HEADERS})

gtest_discover_tests(es_core_tests)

target_link_libraries(es_core_tests PRIVATE GTest::gtest GTest::gtest_main es-core)