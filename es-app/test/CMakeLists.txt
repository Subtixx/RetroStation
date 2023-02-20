find_package(GTest CONFIG REQUIRED)

include(CTest)
include(GoogleTest)

FILE(GLOB_RECURSE ES_APP_TEST_SOURCES LIST_DIRECTORIES false *.cpp)
FILE(GLOB_RECURSE ES_APP_TEST_HEADERS LIST_DIRECTORIES false *.h)

add_executable(es_app_tests ${ES_APP_TEST_SOURCES} ${ES_APP_TEST_HEADERS})

gtest_discover_tests(es_app_tests)

target_link_libraries(es_app_tests PRIVATE GTest::gtest GTest::gtest_main es-core)