project(libretro C CXX)

set(LIBRETRO_HEADERS
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_miscellaneous.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_endianness.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_common.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_common_api.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_environment.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_inline.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_assert.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_math.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_timers.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/retro_dirent.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libretro.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/boolean.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/memalign.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/memmap.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/formats/cdfs.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/formats/m3u_file.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/chd_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/file_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/file_stream_transforms.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/interface_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/memory_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/rzip_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/stdin_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/streams/trans_stream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/bitstream.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/cdrom.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/chd.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/coretypes.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/huffman.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/libchdr_zlib.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/lzma.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/minmax.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/libchdr/flac.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/file/file_path.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/string/stdstring.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/lists/dir_list.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/lists/string_list.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/cdrom/cdrom.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/time/rtime.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/vfs/vfs.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/vfs/vfs_implementation.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/vfs/vfs_implementation_cdrom.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/compat/msvc.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/encodings/crc32.h
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include/encodings/utf.h
        )

set(LIBRETRO_SOURCES
        ${CMAKE_SOURCE_DIR}/external/libretro-common/file/file_path.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/file/file_path_io.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/file/retro_dirent.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/compat/compat_strl.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/compat/compat_strcasestr.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/compat/compat_posix_string.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/compat/fopen_utf8.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/memmap/memalign.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/memmap/memmap.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/cdfs/cdfs.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_bitstream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_cdrom.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_chd.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_huffman.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_lzma.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_zlib.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_flac.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/libchdr/libchdr_flac_codec.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/formats/m3u/m3u_file.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/chd_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/file_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/file_stream_transforms.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/interface_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/memory_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/rzip_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/stdin_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/trans_stream.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/trans_stream_pipe.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/streams/trans_stream_zlib.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/string/stdstring.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/utils/md5.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/vfs/vfs_implementation.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/vfs/vfs_implementation_cdrom.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/lists/dir_list.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/lists/string_list.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/time/rtime.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/cdrom/cdrom.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/encodings/encoding_crc32.c
        ${CMAKE_SOURCE_DIR}/external/libretro-common/encodings/encoding_utf.c
        )

add_library(libretro STATIC
        ${LIBRETRO_SOURCES}
        ${LIBRETRO_HEADERS}
        )
target_include_directories(libretro PUBLIC
        ${CMAKE_SOURCE_DIR}/external/libretro-common/include
        ${CMAKE_SOURCE_DIR}/external/libretro-common
        ${CMAKE_SOURCE_DIR}/external/7zip
        )
target_link_libraries(libretro PUBLIC 7ZIP)

# disable all warnings for just this project
if (MSVC)
    target_compile_options(libretro PRIVATE /W0)
else ()
    target_compile_options(libretro PRIVATE -w)
endif ()