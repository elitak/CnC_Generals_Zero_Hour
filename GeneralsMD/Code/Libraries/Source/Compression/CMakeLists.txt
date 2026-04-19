file(GLOB_RECURSE SOURCES_EAC ${CMAKE_CURRENT_SOURCE_DIR}/EAC/*.cpp)

if(SAGE_USE_LZHL)
  file(GLOB_RECURSE SOURCES_LZH ${CMAKE_CURRENT_SOURCE_DIR}/LZHCompress/*.cpp)
endif()

find_package(ZLIB REQUIRED)

add_library(Compression STATIC CompressionManager.cpp ${SOURCES_EAC} ${SOURCES_LZH})

target_include_directories(
  Compression PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/../
                     ${CMAKE_CURRENT_SOURCE_DIR}/../../Include)

target_link_libraries(Compression PRIVATE ZLIB::ZLIB)

if(SAGE_USE_LZHL)
  target_link_libraries(Compression PRIVATE LZHL::LZHL)
  target_compile_definitions(Compression PRIVATE SAGE_USE_LZHL)
endif()
