project(
		RetroStation-Core
		VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}
		LANGUAGES C CXX
)

FILE(
		GLOB_RECURSE
		CORE_SOURCES
		src/*.cpp
)

FILE(
		GLOB_RECURSE
		CORE_HEADERS
		src/*.h
		${HEDLEY_INCLUDE_DIRS}
)

# Keep Directory structure in Visual Studio
if(MSVC)
	file(
		 GLOB_RECURSE source_list RELATIVE
		 "${CMAKE_CURRENT_SOURCE_DIR}"
		 *.cpp *.c
		)
	foreach(source IN LISTS source_list)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "/" "\\" source_path_msvc "${source_path}")
		string(REPLACE "src" "" source_path_final "${source_path_msvc}")
		source_group("${source_path_final}" FILES "${source}")
	endforeach()
endif()

set(ESCORE_VERSION_MAJOR 1)
set(ESCORE_VERSION_MINOR 1)
set(ESCORE_VERSION_PATCH 0)
set(ESCORE_VERSION_TWEAK 0)
configure_file(include/core_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/core_version.h @ONLY)

include_directories(${COMMON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(es-core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(es-core ${COMMON_LIBRARIES})

if(FEATURE_TESTS)
	add_subdirectory("./test")
endif()