# FindFFMPEG.cmake
# Locate FFmpeg libraries using pkg-config with a manual fallback.
#
# Defines:
#   FFMPEG_FOUND
#   FFMPEG_INCLUDE_DIRS
#   FFMPEG_LIBRARIES

set(_ffmpeg_components avcodec avformat avutil swscale swresample)

# Prefer deterministic lookup from the vcpkg installation tree used by this
# build preset.
if(DEFINED VCPKG_TARGET_TRIPLET)
    set(_ffmpeg_vcpkg_roots
        "${VCPKG_INSTALLED_DIR}"
        "${CMAKE_BINARY_DIR}/vcpkg_installed"
        "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed"
    )

    foreach(_root IN LISTS _ffmpeg_vcpkg_roots)
        if(_root)
            set(_ffmpeg_base "${_root}/${VCPKG_TARGET_TRIPLET}")
            if(EXISTS "${_ffmpeg_base}/include/libavcodec/avcodec.h")
                set(FFMPEG_INCLUDE_DIRS "${_ffmpeg_base}/include")
                set(FFMPEG_LIBRARY_DIRS
                    "${_ffmpeg_base}/debug/lib"
                    "${_ffmpeg_base}/lib"
                )

                foreach(_comp ${_ffmpeg_components})
                    unset(_ffmpeg_lib_${_comp} CACHE)
                    find_library(_ffmpeg_lib_${_comp}
                        NAMES ${_comp}
                        PATHS ${FFMPEG_LIBRARY_DIRS}
                        NO_DEFAULT_PATH
                    )
                    if(_ffmpeg_lib_${_comp})
                        list(APPEND FFMPEG_LIBRARIES ${_ffmpeg_lib_${_comp}})
                    endif()
                    mark_as_advanced(_ffmpeg_lib_${_comp})
                endforeach()

                break()
            endif()
        endif()
    endforeach()
endif()

find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND AND NOT FFMPEG_LIBRARIES)
    foreach(_comp ${_ffmpeg_components})
        pkg_check_modules(_ffmpeg_${_comp} QUIET lib${_comp})
        if(_ffmpeg_${_comp}_FOUND)
            list(APPEND FFMPEG_INCLUDE_DIRS ${_ffmpeg_${_comp}_INCLUDE_DIRS})
            list(APPEND FFMPEG_LIBRARY_DIRS ${_ffmpeg_${_comp}_LIBRARY_DIRS})
            list(APPEND FFMPEG_LIBRARIES ${_ffmpeg_${_comp}_LIBRARIES})
        endif()
    endforeach()
endif()

# Resolve pkg-config library names to full paths when possible.
if(FFMPEG_LIBRARIES AND FFMPEG_LIBRARY_DIRS)
    set(_ffmpeg_resolved_libs)
    foreach(_lib IN LISTS FFMPEG_LIBRARIES)
        if(IS_ABSOLUTE "${_lib}" OR _lib MATCHES "^-l" OR _lib MATCHES "^-Wl")
            list(APPEND _ffmpeg_resolved_libs "${_lib}")
        else()
            set(_ffmpeg_resolved)
            find_library(_ffmpeg_resolved NAMES ${_lib} PATHS ${FFMPEG_LIBRARY_DIRS})
            if(_ffmpeg_resolved)
                list(APPEND _ffmpeg_resolved_libs "${_ffmpeg_resolved}")
            else()
                list(APPEND _ffmpeg_resolved_libs "${_lib}")
            endif()
        endif()
    endforeach()
    set(FFMPEG_LIBRARIES ${_ffmpeg_resolved_libs})
endif()

if(NOT FFMPEG_INCLUDE_DIRS)
    find_path(FFMPEG_INCLUDE_DIR libavcodec/avcodec.h)
    if(FFMPEG_INCLUDE_DIR)
        set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIR})
    endif()
endif()

if(NOT FFMPEG_LIBRARIES)
    foreach(_comp ${_ffmpeg_components})
        find_library(_ffmpeg_lib_${_comp} NAMES ${_comp} PATHS ${FFMPEG_LIBRARY_DIRS})
        if(_ffmpeg_lib_${_comp})
            list(APPEND FFMPEG_LIBRARIES ${_ffmpeg_lib_${_comp}})
        endif()
        mark_as_advanced(_ffmpeg_lib_${_comp})
    endforeach()
endif()

if(FFMPEG_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIRS)
endif()
if(FFMPEG_LIBRARY_DIRS)
    list(REMOVE_DUPLICATES FFMPEG_LIBRARY_DIRS)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFMPEG
    REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIRS
)

mark_as_advanced(FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES)
