project("emulationstation")

FILE(
			GLOB_RECURSE
	ES_SOURCES
	src/*.cpp
)

FILE(
	GLOB_RECURSE
	ES_HEADERS
	src/*.h
)

#-------------------------------------------------------------------------------
# define OS specific sources and headers
if(MSVC)
    LIST(APPEND ES_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/EmulationStation.rc
    )
endif()

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

find_package(Lua REQUIRED)

set(ADDITIONAL_LIBRARIES
		breakpad
		breakpad_common
		)
LIST(APPEND ADDITIONAL_LIBRARIES ${LUA_LIBRARIES})

#-------------------------------------------------------------------------------
# define target
include_directories(${COMMON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(emulationstation ${ES_SOURCES} ${ES_HEADERS})
target_link_libraries(emulationstation ${COMMON_LIBRARIES} es-core ${ADDITIONAL_LIBRARIES})

target_include_directories(emulationstation PUBLIC
		${CMAKE_SOURCE_DIR}/external/argagg/include
		${LUA_INCLUDE_DIR}
)

# special properties for Windows builds
if(MSVC)
    # Always compile with the "WINDOWS" subsystem to avoid console window flashing at startup
    # when --debug is not set (see es-core/src/main.cpp for explanation).
    # The console will still be shown if launched with --debug.
    # Note that up to CMake 2.8.10 this feature is broken: http://public.kitware.com/Bug/view.php?id=12566
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(emulationstation PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif()

# Copy directory if exists skip
add_custom_command(TARGET emulationstation POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/data/resources
		${CMAKE_SOURCE_DIR}/bin/resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/data/scripts
		${CMAKE_SOURCE_DIR}/bin/scripts
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/data/config
		${CMAKE_SOURCE_DIR}/bin/config
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/data/emulationstation
		${CMAKE_SOURCE_DIR}/bin/.emulationstation
)

#-------------------------------------------------------------------------------
# set up CPack install stuff so `make install` does something useful

install(TARGETS emulationstation
    RUNTIME
    DESTINATION bin)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A flexible graphical emulator front-end")
SET(CPACK_PACKAGE_DESCRIPTION "EmulationStation is a flexible, graphical front-end designed for keyboardless navigation of your multi-platform retro game collection.")

SET(CPACK_RESOURCE_FILE LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
SET(CPACK_RESOURCE_FILE README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alec Lofquist <allofquist@yahoo.com>")
SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libsdl2-2.0-0, libsdl2-mixer-2.0-0, libfreeimage3, libfreetype6, libcurl3, libasound2")
SET(CPACK_DEBIAN_PACKAGE_BUILDS_DEPENDS "debhelper (>= 8.0.0), cmake, g++ (>= 4.8), libsdl2-dev, libsdl2-mixer-dev, libfreeimage-dev, libfreetype6-dev, libcurl4-openssl-dev, libasound2-dev, libgl1-mesa-dev, rapidjson-dev, libgl1-mesa-dev")

SET(CPACK_PACKAGE_VENDOR "emulationstation.org")
SET(CPACK_PACKAGE_VERSION "2.0.0~rc1")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "emulationstation_${CMAKE_PACKAGE_VERSION}")
SET(CPACK_PACKAGE_EXECUTABLES "emulationstation" "emulationstation")

SET(CPACK_GENERATOR "TGZ;DEB")

INCLUDE(CPack)
