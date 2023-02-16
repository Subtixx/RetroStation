cmake_minimum_required(VERSION 3.24.2)

# link_directories() treats paths relative to the source dir.
cmake_policy(SET CMP0015 NEW)

# Avoid warnings in CMake 3.0.2:
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0043 NEW)

# allow VERSION argument in project()
cmake_policy(SET CMP0048 NEW)

if (POLICY CMP0054)
    # do not interpret if() arguments as variables!
    cmake_policy(SET CMP0054 NEW)
endif()

if (POLICY CMP0091)
    # Allow selecting MSVC runtime library using CMAKE_MSVC_RUNTIME_LIBRARY.
    cmake_policy(SET CMP0091 NEW)
endif()

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.26.3.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

run_vcpkg()

project(emulationstation-all VERSION 1.0.0 LANGUAGES C CXX)

project_options(
        ENABLE_CACHE
        ${ENABLE_CPPCHECK}
        ${ENABLE_CLANG_TIDY}
        ENABLE_VS_ANALYSIS
        # ENABLE_CONAN
        # ENABLE_INTERPROCEDURAL_OPTIMIZATION
        # ENABLE_NATIVE_OPTIMIZATION
        ${ENABLE_DOXYGEN}
        ${ENABLE_COVERAGE}
        ${ENABLE_SANITIZER_ADDRESS}
        ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
        # ENABLE_PCH
        # PCH_HEADERS
        # WARNINGS_AS_ERRORS
        ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # LINKER "lld"
        # CONAN_PROFILE ${profile_path}  # passes a profile to conan: see https://docs.conan.io/en/latest/reference/profiles.html
)

option(GLES "Set to ON if targeting Embedded OpenGL" ${GLES})
option(GLES2 "Set to ON if targeting OpenGL ES 2.0" ${GLES2})
option(GL "Set to ON if targeting Desktop OpenGL" ${GL})
option(RPI "Set to ON to enable the Raspberry PI video player (omxplayer)" ${RPI})
option(CEC "CEC" ON)
option(BCM "BCM host" OFF)

option(BATOCERA "Set to ON to enable BATOCERA specific code" OFF)
option(RETROBAT "Set to ON to enable RETROBAT specific code" OFF)

option(ENABLE_FILEMANAGER "Set to ON to enable f1 shortcut for filesystem" OFF)
option(DISABLE_KODI "Set to ON to disable kodi in menu" OFF)
option(ENABLE_PULSE "Set to ON to enable pulse audio (versus alsa)" OFF)
option(ENABLE_TTS "Set to ON to enable text to speech" OFF)

#-------------------------------------------------------------------------------
#add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Packages
)

if(BATOCERA)
  MESSAGE("Batocera build")
  add_definitions(-DBATOCERA)
endif()

if(RETROBAT)
  MESSAGE("Retrobat build")
  add_definitions(-DRETROBAT)
endif()

# disable kodi menu
if(DISABLE_KODI)
  MESSAGE("kodi disabled")
else()
  add_definitions(-D_ENABLE_KODI_)
endif()

# batocera / file manager f1 button
# disable file manager
if(ENABLE_FILEMANAGER)
  MESSAGE("file manager enabled")
  add_definitions(-D_ENABLE_FILEMANAGER_)
endif()

if(CEC)
  MESSAGE("CEC enabled")
else()
  MESSAGE("CEC disabled")
endif()

if(BCM)
    set(BCMHOST found)
endif()

#-------------------------------------------------------------------------------
#set up OpenGL system variable
if(GLES)
    set(GLSystem "Embedded OpenGL" CACHE STRING "The OpenGL system to be used")
elseif(GLES2)
    set(GLSystem "OpenGL ES 2.0" CACHE STRING "The OpenGL system to be used")
elseif(GL)
    set(GLSystem "Desktop OpenGL" CACHE STRING "The OpenGL system to be used")
#-------------------------------------------------------------------------------
#check if we're running on Raspberry Pi
elseif(EXISTS "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/bcm_host.h")
    MESSAGE("bcm_host.h found")
    set(BCMHOST found)
    set(GLSystem "Embedded OpenGL" CACHE STRING "The OpenGL system to be used")
#-------------------------------------------------------------------------------
#check if we're running on OSMC Vero4K
elseif(EXISTS "${CMAKE_FIND_ROOT_PATH}/opt/vero3/lib/libMali.so")
    MESSAGE("libMali.so found")
    set(VERO4K found)
    set(GLSystem "Embedded OpenGL" CACHE STRING "The OpenGL system to be used")
#-------------------------------------------------------------------------------
#check if we're running on olinuxino / odroid / etc
elseif(EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/libMali.so" OR
    EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/libmali.so" OR # batocera
    EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/arm-linux-gnueabihf/libMali.so" OR
    EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/aarch64-linux-gnu/libMali.so" OR
    EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/arm-linux-gnueabihf/mali-egl/libmali.so" OR
    EXISTS "${CMAKE_FIND_ROOT_PATH}/usr/lib/arm-linux-gnueabihf/libmali.so")
    MESSAGE("libMali.so found")
    set(GLSystem "Embedded OpenGL" CACHE STRING "The OpenGL system to be used")
else()
    set(GLSystem "Desktop OpenGL" CACHE STRING "The OpenGL system to be used")
endif(GLES)

set_property(CACHE GLSystem PROPERTY STRINGS "Desktop OpenGL" "Embedded OpenGL")

#finding necessary packages
#-------------------------------------------------------------------------------
if(${GLSystem} MATCHES "Desktop OpenGL")
    find_package(OpenGL REQUIRED)
elseif(${GLSystem} MATCHES "OpenGL ES 2.0")
    find_package(OpenGLES2 REQUIRED)
else()
    find_package(OpenGLES REQUIRED)
endif()
find_package(Freetype REQUIRED)
find_package(FreeImage REQUIRED)
# batocera / background musics
find_package(SDL2MIXER REQUIRED)
# batocera / customisations
find_package(SDL2 REQUIRED)
find_package(CURL REQUIRED)
find_package(VLC REQUIRED)
find_package(RapidJSON REQUIRED)

if(ENABLE_PULSE)
  MESSAGE("pulse audio enabled")
  find_package(PulseAudio REQUIRED)
  add_definitions(-D_ENABLE_PULSE_)
else()
  MESSAGE("pulse audio disabled")
endif()

find_package(udev)
if(UDEV_FOUND)
  MESSAGE("udev found. guns enabled")
  add_definitions(-DHAVE_UDEV)
else()
  MESSAGE("udev not found. guns will be disabled")
endif()

if(ENABLE_TTS)
  MESSAGE("tts enabled")
  find_library(LIBESPEAK_LIBRARIES NAMES espeak REQUIRED)
  if("${LIBESPEAK_LIBRARIES}" STREQUAL "LIBESPEAK_LIBRARIES-NOTFOUND")
    MESSAGE(FATAL_ERROR "espeak not found")
  endif()
  add_definitions(-D_ENABLE_TTS_)
else()
  MESSAGE("tts disabled")
endif()

if(CEC)
  find_package(libCEC)
endif()

#add ALSA for Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(ALSA REQUIRED)
endif()

#-------------------------------------------------------------------------------
#set up compiler flags and excutable names
if(DEFINED BCMHOST OR RPI)
    add_definitions(-D_RPI_)
endif()

if(DEFINED VERO4K)
    add_definitions(-D_VERO4K_)
endif()

if(CEC)
  if(DEFINED libCEC_FOUND)
    add_definitions(-DHAVE_LIBCEC)
  endif()
endif()

if(DEFINED SCREENSCRAPER_SOFTNAME)
    add_definitions(-DSCREENSCRAPER_SOFTNAME="${SCREENSCRAPER_SOFTNAME}")
endif()

if(DEFINED SCREENSCRAPER_DEV_LOGIN)
    add_definitions(-DSCREENSCRAPER_DEV_LOGIN="${SCREENSCRAPER_DEV_LOGIN}")
endif()

if(DEFINED GAMESDB_APIKEY)
    add_definitions(-DGAMESDB_APIKEY="${GAMESDB_APIKEY}")
endif()

if(DEFINED CHEEVOS_DEV_LOGIN)
    add_definitions(-DCHEEVOS_DEV_LOGIN="${CHEEVOS_DEV_LOGIN}")
endif()

if(DEFINED HFS_DEV_LOGIN)
    add_definitions(-DHFS_DEV_LOGIN="${HFS_DEV_LOGIN}")
endif()

#-------------------------------------------------------------------------------

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-DNOMINMAX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    #check for G++ 4.7+
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE G++_VERSION)
    if (G++_VERSION VERSION_LESS 4.7)
            message(SEND_ERROR "You need at least G++ 4.7 to compile EmulationStation!")
    endif()

    #set up compiler flags for GCC
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-attributes -O0") #support C++14 for std::, optimize
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-attributes -O3") #support C++14 for std::, optimize
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3")  #-s = strip binary
endif()
endif()

if(${GLSystem} MATCHES "Desktop OpenGL")
    add_definitions(-DUSE_OPENGL_21)
elseif(${GLSystem} MATCHES "OpenGL ES 2.0")
    add_definitions(-DUSE_OPENGLES_20)
else()
    add_definitions(-DUSE_OPENGLES_10)
endif()

#-------------------------------------------------------------------------------
#add include directories
set(COMMON_INCLUDE_DIRS
    ${FREETYPE_INCLUDE_DIRS}
    ${FreeImage_INCLUDE_DIRS}
    # batocera / background musics
    ${SDLMIXER_INCLUDE_DIR}
    # batocera / customisations    
    ${SDL2_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
    ${VLC_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/external/id3v2lib/include
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/es-core/src
)

if(ENABLE_PULSE)
 LIST(APPEND COMMON_INCLUDE_DIRS
    ${PULSEAUDIO_INCLUDE_DIR}
      )
endif()

#add libCEC_INCLUDE_DIR
if(CEC)
  if(DEFINED libCEC_FOUND)
    LIST(APPEND COMMON_INCLUDE_DIRS
      ${libCEC_INCLUDE_DIR}
      )
  endif()
endif()

#add ALSA for Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    LIST(APPEND COMMON_INCLUDE_DIRS
        ${ALSA_INCLUDE_DIRS}
    )
endif()

if(DEFINED BCMHOST)
    LIST(APPEND COMMON_INCLUDE_DIRS
        "${CMAKE_FIND_ROOT_PATH}/opt/vc/include"
        "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos"
        "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vmcs_host/linux"
        "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos/pthreads"
    )
#add include directory for Vero4K
elseif(DEFINED VERO4K)
    LIST(APPEND COMMON_INCLUDE_DIRS
        "${CMAKE_FIND_ROOT_PATH}/opt/vero3/include"
    )
else()
    if(${GLSystem} MATCHES "Desktop OpenGL")
        LIST(APPEND COMMON_INCLUDE_DIRS
            ${OPENGL_INCLUDE_DIR}
        )
    elseif(${GLSystem} MATCHES "OpenGL ES 2.0")
        LIST(APPEND COMMON_INCLUDE_DIRS
            ${OPENGLES2_INCLUDE_DIR}
        )
    else()
        LIST(APPEND COMMON_INCLUDE_DIRS
            ${OPENGLES_INCLUDE_DIR}
        )
    endif()
endif()

#-------------------------------------------------------------------------------
#define libraries and directories
if(DEFINED BCMHOST)
    link_directories(
        # batocera / customisations
        "${CMAKE_FIND_ROOT_PATH}/opt/vc/lib"
    )
elseif(DEFINED VERO4K)
    link_directories(
        "${CMAKE_FIND_ROOT_PATH}/opt/vero3/lib"
    )
endif()
find_package(flac CONFIG REQUIRED)
find_package(ZLIB REQUIRED)


find_package(fmt CONFIG REQUIRED)
# Compile loguru with streams and fmtlib
add_definitions(-DLOGURU_WITH_STREAMS=1 -DLOGURU_WITH_FMTLIB=1)
find_package(loguru CONFIG REQUIRED)

find_package(Microsoft.GSL CONFIG REQUIRED)

find_path(HEDLEY_INCLUDE_DIRS "hedley.h")
LIST(APPEND COMMON_INCLUDE_DIRS ${HEDLEY_INCLUDE_DIRS})

set(COMMON_LIBRARIES
    ${FREETYPE_LIBRARIES}
    ${FreeImage_LIBRARIES}
    ${SDL2_LIBRARY}
    # batocera / background musics
    ${SDLMIXER_LIBRARY}
    # batocera / customisations    
    ${CURL_LIBRARIES}
    ${VLC_LIBRARIES}
    pugixml
    nanosvg
    id3v2lib
	libcheevos
    z
    FLAC::FLAC FLAC::FLAC++
    ZLIB::ZLIB
    loguru
    fmt::fmt
        Microsoft.GSL::GSL
)

if(UDEV_FOUND)
 LIST(APPEND COMMON_LIBRARIES
    ${UDEV_LIBRARY}
      )
endif()

if(ENABLE_PULSE)
 LIST(APPEND COMMON_LIBRARIES
    ${PULSEAUDIO_LIBRARY}
      )
endif()

if(ENABLE_TTS)
 LIST(APPEND COMMON_LIBRARIES
    ${LIBESPEAK_LIBRARIES}
      )
endif()

#add libCEC_LIBRARIES
if(CEC)
  if(DEFINED libCEC_FOUND)
    if(DEFINED BCMHOST)
      LIST(APPEND COMMON_LIBRARIES
        vchiq_arm
	vchostif
	)
    endif()
    LIST(APPEND COMMON_LIBRARIES
      dl
      ${libCEC_LIBRARIES}
      )
  endif()
endif()

#add ALSA for Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    LIST(APPEND COMMON_LIBRARIES
        ${ALSA_LIBRARY}
    )
endif()

if(DEFINED BCMHOST)
    LIST(APPEND COMMON_LIBRARIES
        bcm_host
        brcmEGL
	)
    if(GLES)
      LIST(APPEND COMMON_LIBRARIES ${OPENGLES_LIBRARIES})
    else()
      LIST(APPEND COMMON_LIBRARIES ${OPENGLES2_LIBRARIES})
    endif()
elseif(DEFINED VERO4K)
    LIST(APPEND COMMON_LIBRARIES
        EGL
        ${OPENGLES_LIBRARIES}
    )
else()
    if(MSVC)
        LIST(APPEND COMMON_LIBRARIES
            winmm
        )
    endif()
    if(${GLSystem} MATCHES "Desktop OpenGL")
        LIST(APPEND COMMON_LIBRARIES
            ${OPENGL_LIBRARIES}
        )
    elseif(${GLSystem} MATCHES "OpenGL ES 2.0")
        LIST(APPEND COMMON_LIBRARIES
            EGL
            ${OPENGLES2_LIBRARIES}
        )
    else()
        LIST(APPEND COMMON_LIBRARIES
            EGL
            ${OPENGLES_LIBRARIES}
        )
    endif()
endif()

# batocera / i18n
# i18n
find_program (MSGFMT_EXECUTABLE msgfmt)
find_program (MSGMERGE_EXECUTABLE msgmerge)
find_program (XGETTEXT_EXECUTABLE xgettext)
find_package(Intl)

if(MSGFMT_EXECUTABLE AND MSGMERGE_EXECUTABLE AND XGETTEXT_EXECUTABLE AND Intl_FOUND)
  message (STATUS "Native language support enabled.")
  add_definitions(-DHAVE_INTL)
else()
  message (STATUS "Native language support disabled.")
endif()

#-------------------------------------------------------------------------------
# set up build directories
set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${dir}/bin/ CACHE PATH "Build directory")
set(LIBRARY_OUTPUT_PATH ${dir}/bin/ CACHE PATH "Build directory")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/bin/ CACHE PATH "Build directory")

#-------------------------------------------------------------------------------
# add each component


# Load env variables
add_definitions(
        -DSCREENSCRAPER_DEV_LOGIN="devid=batoceralinux&devpassword=omvIrirRPuY"
        -DGAMESDB_APIKEY="b699c2e6f9f85730fc2e4aa831b24172e68701352310f4e4d301d9342ae39636"
        -DCHEEVOS_DEV_LOGIN="z=BatoceraLinux&y=u6LQHAtl0360ezfXLPuCbYRZfKeVkWas"
)

add_subdirectory("external")
add_subdirectory("es-core")
add_subdirectory("es-app")

if(MSGFMT_EXECUTABLE AND MSGMERGE_EXECUTABLE AND XGETTEXT_EXECUTABLE AND Intl_FOUND)
  add_subdirectory (locale)
endif()