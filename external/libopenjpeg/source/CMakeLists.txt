cmake_minimum_required (VERSION 3.1.0)
project (static_openjpeg)

set(ALL_FILES
bio.c
bio.h
cio.c
cio.h
dwt.c
dwt.h
event.c
event.h
function_list.c
function_list.h
image.c
image.h
indexbox_manager.h
invert.c
invert.h
j2k.c
j2k.h
jp2.c
jp2.h
mct.c
mct.h
mqc.c
mqc.h
openjpeg.c
openjpeg.h
opj_clock.c
opj_clock.h
opj_codec.h
opj_config.h
opj_config_private.h
opj_includes.h
opj_intmath.h
opj_inttypes.h
opj_malloc.h
opj_stdint.h
pi.c
pi.h
raw.c
raw.h
t1.c
t1.h
t1_luts.h
t2.c
t2.h
tcd.c
tcd.h
tgt.c
tgt.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPJ_STATIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPJ_STATIC -std=c++11")

add_library(static_openjpeg ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
