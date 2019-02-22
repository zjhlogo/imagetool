cmake_minimum_required (VERSION 3.1.0)
project (static_png)

include_directories(${PROJECT_SOURCE_DIR}/../../libzlib/source)

set(ALL_FILES
png.c
png.h
pngconf.h
pngdebug.h
pngerror.c
pngget.c
pnginfo.h
pnglibconf.h
pngmem.c
pngpread.c
pngpriv.h
pngread.c
pngrio.c
pngrtran.c
pngrutil.c
pngset.c
pngstruct.h
pngtrans.c
pngwio.c
pngwrite.c
pngwtran.c
pngwutil.c
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(static_png ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
