cmake_minimum_required (VERSION 3.6.0)

project (imagetool)

add_subdirectory(external/libbaseutil/source)
add_subdirectory(external/libfreeimage/source)
add_subdirectory(external/libjpeg/source)
add_subdirectory(external/libjxr/source)
add_subdirectory(external/libopenexr/source)
add_subdirectory(external/libopenjpeg/source)
add_subdirectory(external/libpng/source)
add_subdirectory(external/librawlite/source)
add_subdirectory(external/libtiff4/source)
add_subdirectory(external/libwebp/source)
add_subdirectory(external/libzlib/source)
add_subdirectory(source)

set_property(DIRECTORY source PROPERTY VS_STARTUP_PROJECT imagetool)
