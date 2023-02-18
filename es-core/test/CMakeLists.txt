find_package(GTest CONFIG REQUIRED)

include(CTest)
include(GoogleTest)

FILE(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)
FILE(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false *.h)

add_executable(tests ${TEST_SOURCES} ${TEST_HEADERS})

gtest_discover_tests(tests)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main es-core)