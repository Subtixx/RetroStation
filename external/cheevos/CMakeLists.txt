project(cheevos C CXX)

FILE(
        GLOB_RECURSE
        CHEEVOS_SOURCES
        ../rcheevos/src/*.c
        ../rcheevos/src/*.cpp
)

FILE(
        GLOB_RECURSE
        CHEEVOS_HEADERS
        ../rcheevos/include/*.h
        ../rcheevos/include/*.hpp
)

add_library(${PROJECT_NAME} STATIC ${CHEEVOS_SOURCES} ${CHEEVOS_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ../rcheevos/include)
