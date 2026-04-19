# FindFFMPEG.cmake
# Locate FFmpeg libraries using pkg-config with a manual fallback.
#
# Defines:
#   FFMPEG_FOUND
#   FFMPEG_INCLUDE_DIRS
#   FFMPEG_LIBRARIES

find_package(PkgConfig QUIET)

set(_ffmpeg_components avcodec avformat avutil swscale swresample)

if(PKG_CONFIG_FOUND)
    foreach(_comp ${_ffmpeg_components})
        pkg_check_modules(_ffmpeg_${_comp} QUIET lib${_comp})
        if(_ffmpeg_${_comp}_FOUND)
            list(APPEND FFMPEG_INCLUDE_DIRS ${_ffmpeg_${_comp}_INCLUDE_DIRS})
            list(APPEND FFMPEG_LIBRARIES ${_ffmpeg_${_comp}_LIBRARIES})
        endif()
    endforeach()
endif()

if(NOT FFMPEG_INCLUDE_DIRS)
    find_path(FFMPEG_INCLUDE_DIR libavcodec/avcodec.h)
    if(FFMPEG_INCLUDE_DIR)
        set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIR})
    endif()
endif()

if(NOT FFMPEG_LIBRARIES)
    foreach(_comp ${_ffmpeg_components})
        find_library(_ffmpeg_lib_${_comp} ${_comp})
        if(_ffmpeg_lib_${_comp})
            list(APPEND FFMPEG_LIBRARIES ${_ffmpeg_lib_${_comp}})
        endif()
        mark_as_advanced(_ffmpeg_lib_${_comp})
    endforeach()
endif()

if(FFMPEG_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIRS)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFMPEG
    REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIRS
)

mark_as_advanced(FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES)
