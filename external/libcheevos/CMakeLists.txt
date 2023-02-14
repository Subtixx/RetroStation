project("libcheevos" C CXX)

#find_package(7zip CONFIG REQUIRED) # Since vcpkg is so stupid, we have to use the old way of finding 7zip
find_package(flac CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

# Use 7zip/C
set(7ZIP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/7zip/C)
FILE(GLOB_RECURSE 7ZIP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/7zip/C/*.c)
FILE(GLOB_RECURSE 7ZIP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/7zip/C/*.h)
# Remove Util/LzmaLib/*
FILE(GLOB_RECURSE 7ZIP_SPAM ${CMAKE_CURRENT_SOURCE_DIR}/7zip/C/Util/*.c)
LIST(REMOVE_ITEM 7ZIP_SOURCES ${7ZIP_SPAM})

set(CHEEVOS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/cheevos.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rcheevos.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_api_info.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_api_request.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_api_runtime.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_api_user.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_consoles.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_error.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_hash.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_runtime.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_runtime_types.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include/rc_url.h
)

set(CHEEVOS_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/cheevos.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rapi/rc_api_common.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rapi/rc_api_common.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rapi/rc_api_info.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rapi/rc_api_runtime.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rapi/rc_api_user.c	
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/alloc.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/compat.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/condition.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/condset.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/consoleinfo.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/format.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/lboard.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/memref.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/operand.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/rc_compat.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/rc_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/rc_libretro.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/rc_libretro.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/richpresence.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/runtime.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/runtime_progress.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/trigger.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rcheevos/value.c
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rhash/hash.c	
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rhash/cdreader.c	
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rhash/md5.h
	${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/src/rurl/url.c		
)

set(LIBRETRO_COMMON_HEADERS

	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_miscellaneous.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_endianness.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_common.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_common_api.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_environment.h		
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_inline.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_assert.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_math.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_timers.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/retro_dirent.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libretro.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/boolean.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/memalign.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/memmap.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/formats/cdfs.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/formats/m3u_file.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/chd_stream.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/file_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/file_stream_transforms.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/interface_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/memory_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/rzip_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/stdin_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/streams/trans_stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/bitstream.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/cdrom.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/chd.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/coretypes.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/huffman.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/libchdr_zlib.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/lzma.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/minmax.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/libchdr/flac.h		
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/file/file_path.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/string/stdstring.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/lists/dir_list.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/lists/string_list.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/cdrom/cdrom.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/time/rtime.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/vfs/vfs.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/vfs/vfs_implementation.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/vfs/vfs_implementation_cdrom.h	
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/compat/msvc.h		
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/encodings/crc32.h
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include/encodings/utf.h

	${7ZIP_HEADERS}
)

set(LIBRETRO_COMMON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/file/file_path.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/file/file_path_io.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/file/retro_dirent.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/compat/compat_strl.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/compat/compat_strcasestr.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/compat/compat_posix_string.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/compat/fopen_utf8.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/memmap/memalign.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/memmap/memmap.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/cdfs/cdfs.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_bitstream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_cdrom.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_chd.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_huffman.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_lzma.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_zlib.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_flac.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/libchdr/libchdr_flac_codec.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/formats/m3u/m3u_file.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/chd_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/file_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/file_stream_transforms.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/interface_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/memory_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/rzip_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/stdin_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/trans_stream.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/trans_stream_pipe.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/streams/trans_stream_zlib.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/string/stdstring.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/utils/md5.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/vfs/vfs_implementation.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/vfs/vfs_implementation_cdrom.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/lists/dir_list.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/lists/string_list.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/time/rtime.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/cdrom/cdrom.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/encodings/encoding_crc32.c
	${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/encodings/encoding_utf.c

	${7ZIP_SOURCES}
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

add_definitions("-DRC_DISABLE_LUA")
add_definitions("-DHAVE_CDROM -DHAVE_CHD -DWANT_SUBCODE -DWANT_RAW_DATA_SECTOR -DHAVE_ZLIB -DHAVE_7ZIP -DHAVE_FLAC -DFLAC__NO_DLL -DFLAC__HAS_OGG=0")
#add_definitions("-D")

if(NOT DEFINED MSVC)
	add_definitions("-DHAVE_LROUND")
endif()

include_directories(${COMMON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/rcheevos/include)
include_directories(${COMMON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/libretro-common/include ${7ZIP_INCLUDE_DIRS})
add_library(libcheevos STATIC ${CHEEVOS_SOURCES} ${CHEEVOS_HEADERS} ${LIBRETRO_COMMON_HEADERS} ${LIBRETRO_COMMON_SOURCES})