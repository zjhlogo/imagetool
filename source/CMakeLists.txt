cmake_minimum_required (VERSION 3.6.0)

project (imagetool)

include_directories(${PROJECT_SOURCE_DIR}/../external/libbaseutil/source)
include_directories(${PROJECT_SOURCE_DIR}/../external/libfreeimage/source)

set(ALL_FILES
ImageTool.cpp
ImageTool.h
main.cpp
MaxRectsBinPack.cpp
MaxRectsBinPack.h
PackingUtil.cpp
PackingUtil.h
Rect.cpp
Rect.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -DFREEIMAGE_LIB")
else ()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DFREEIMAGE_LIB")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DFREEIMAGE_LIB")
endif ()

add_executable(${PROJECT_NAME} ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()

target_link_libraries(${PROJECT_NAME} static_baseutil static_freeimage static_jpeg static_jxr static_openexr static_openjpeg static_png static_rawlite static_tiff4 static_webp static_zlib)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJECT_NAME}_D)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJECT_NAME})
