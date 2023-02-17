project(cheevos C CXX)

FILE(
        GLOB_RECURSE
        CHEEVOS_SOURCES
        ../rcheevos/src/*.c
        ../rcheevos/src/*.cpp
        ./*.cpp
)

FILE(
        GLOB_RECURSE
        CHEEVOS_HEADERS
        ../rcheevos/include/*.h
        ../rcheevos/include/*.hpp
)

add_library(${PROJECT_NAME} STATIC ${CHEEVOS_SOURCES} ${CHEEVOS_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ../rcheevos/include ./)

project(libcustomcheevos)

FILE(
        GLOB_RECURSE
        LIBCHEEVOS_SOURCES
        ./src/*.cpp
)

FILE(
        GLOB_RECURSE
        LIBCHEEVOS_HEADERS
        ./include/*.h
)

find_package(loguru REQUIRED)

add_library(${PROJECT_NAME} STATIC ${LIBCHEEVOS_SOURCES} ${LIBCHEEVOS_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ../rcheevos/include ../libretro-common/include ./include)
target_link_libraries(${PROJECT_NAME} PUBLIC rcheevos libretro-common loguru)