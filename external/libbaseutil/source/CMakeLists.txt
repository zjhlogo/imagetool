cmake_minimum_required (VERSION 3.1.0)
project (static_baseutil)

include_directories(${PROJECT_SOURCE_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
include_directories(${PROJECT_SOURCE_DIR}/platform/win32)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
include_directories(${PROJECT_SOURCE_DIR}/platform/linux)
endif ()

set(ALL_FILES
serialize/FieldAttributes.cpp
serialize/FieldAttributes.h
serialize/Serialize.cpp
serialize/Serialize.h
serialize/SerializeField.cpp
serialize/SerializeField.h
serialize/SerializeOp.cpp
serialize/SerializeOp.h
utils/BufferStream.cpp
utils/BufferStream.h
BaseType.cpp
BaseType.h
baseutil.h
Config.h
ConvertUTF.c
ConvertUTF.h
Cpp11EventSystem.h
File.h
FileUtil.cpp
FileUtil.h
format.cc
format.h
LogUtil.h
md5.cpp
md5.h
Rtti.cpp
Rtti.h
StringUtil.cpp
StringUtil.h
ThreadPool.h
TimeUtil.cpp
TimeUtil.h
tinyxml2.cpp
tinyxml2.h
XmlUtil.cpp
XmlUtil.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
set(ALL_FILES ${ALL_FILES}
platform/win32/dirent_win32.h
platform/win32/File_Win32.cpp
platform/win32/LogUtil_Win32.cpp
platform/win32/TimeUtil_Win32.cpp
platform/win32/TimeUtil_Win32.h
)
elseif (CMAKE_SYSTEM_NAME MATCHES "Android")
set(ALL_FILES ${ALL_FILES}
platform/android/File_Android.cpp
platform/android/LogUtil_Android.cpp
platform/android/TimeUtil_Android.cpp
platform/android/TimeUtil_Android.h
)
elseif (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
set(ALL_FILES ${ALL_FILES}
platform/emscripten/LogUtil_Emscripten.cpp
platform/emscripten/TimeUtil_Emscripten.cpp
platform/emscripten/TimeUtil_Emscripten.h
)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
set(ALL_FILES ${ALL_FILES}
platform/linux/File_Linux.cpp
platform/linux/LogUtil_Linux.cpp
platform/linux/TimeUtil_Linux.cpp
platform/linux/TimeUtil_Linux.h
)
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
else ()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif ()

add_library(static_baseutil ${ALL_FILES})

if (CMAKE_SYSTEM_NAME MATCHES "Android")
    target_link_libraries(static_baseutil log)
endif ()

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
