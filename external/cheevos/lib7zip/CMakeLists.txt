
project(7ZIP C CXX)

# Use 7zip/C
set(7ZIP_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/7zip/C)
FILE(GLOB_RECURSE 7ZIP_SOURCES ${CMAKE_SOURCE_DIR}/external/7zip/C/*.c)
FILE(GLOB_RECURSE 7ZIP_HEADERS ${CMAKE_SOURCE_DIR}/external/7zip/C/*.h)
# Remove Util/LzmaLib/*
FILE(GLOB_RECURSE 7ZIP_SPAM ${CMAKE_SOURCE_DIR}/external/7zip/C/Util/*.c)
LIST(REMOVE_ITEM 7ZIP_SOURCES ${7ZIP_SPAM})

add_library(7ZIP STATIC ${7ZIP_SOURCES} ${7ZIP_HEADERS})
