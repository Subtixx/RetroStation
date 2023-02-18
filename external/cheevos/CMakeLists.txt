add_definitions("-DRC_DISABLE_LUA")
add_definitions("-DHAVE_CDROM -DHAVE_CHD -DWANT_SUBCODE -DWANT_RAW_DATA_SECTOR -DHAVE_ZLIB -DHAVE_7ZIP -DHAVE_FLAC -DFLAC__NO_DLL -DFLAC__HAS_OGG=0 -D__USE_MISC=1")

if (NOT DEFINED MSVC)
    add_definitions("-DHAVE_LROUND")
endif ()

find_package(flac CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

project(cheevos C CXX)

FILE(
        GLOB_RECURSE
        CHEEVOS_SOURCES
        ${CMAKE_SOURCE_DIR}/external/rcheevos/src/*.c
        ${CMAKE_SOURCE_DIR}/external/rcheevos/src/*.cpp
        ./src/*.cpp
)

FILE(
        GLOB_RECURSE
        CHEEVOS_HEADERS
        ${CMAKE_SOURCE_DIR}/external/rcheevos/include/*.h
        ${CMAKE_SOURCE_DIR}/external/rcheevos/include/*.hpp
        ./include/*.h
)

add_library(${PROJECT_NAME} STATIC ${CHEEVOS_SOURCES} ${CHEEVOS_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/external/rcheevos/include
        ./include/
        ./src/
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include
        )

target_link_libraries(${PROJECT_NAME} PUBLIC 7ZIP libretro loguru)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        -DRC_DISABLE_LUA
        -DHAVE_CDROM
        -DHAVE_CHD
        -DWANT_SUBCODE
        -DWANT_RAW_DATA_SECTOR
        -DHAVE_ZLIB
        -DHAVE_7ZIP
        -DHAVE_FLAC
        -DFLAC__NO_DLL
        -DFLAC__HAS_OGG=0
        -D__USE_MISC=1
        )

add_subdirectory(lib7zip)
add_subdirectory(libretro)