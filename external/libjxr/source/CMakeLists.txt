cmake_minimum_required (VERSION 3.1.0)
project (static_jxr)

set(ALL_FILES
common/include/guiddef.h
common/include/wmsal.h
common/include/wmspecstring.h
common/include/wmspecstrings_adt.h
common/include/wmspecstrings_strict.h
common/include/wmspecstrings_undef.h
image/decode/decode.c
image/decode/decode.h
image/decode/JXRTranscode.c
image/decode/postprocess.c
image/decode/segdec.c
image/decode/strdec.c
image/decode/strdec_x86.c
image/decode/strInvTransform.c
image/decode/strPredQuantDec.c
image/encode/encode.c
image/encode/encode.h
image/encode/segenc.c
image/encode/strenc.c
image/encode/strenc_x86.c
image/encode/strFwdTransform.c
image/encode/strPredQuantEnc.c
image/sys/adapthuff.c
image/sys/ansi.h
image/sys/common.h
image/sys/image.c
image/sys/perfTimer.h
image/sys/perfTimerANSI.c
image/sys/strcodec.c
image/sys/strcodec.h
image/sys/strPredQuant.c
image/sys/strTransform.c
image/sys/strTransform.h
image/sys/windowsmediaphoto.h
image/sys/xplatform_image.h
image/x86/x86.h
jxrgluelib/JXRGlue.c
jxrgluelib/JXRGlue.h
jxrgluelib/JXRGlueJxr.c
jxrgluelib/JXRGluePFC.c
jxrgluelib/JXRMeta.c
jxrgluelib/JXRMeta.h
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_PERF_MEASUREMENT -D__ANSI__")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -fexceptions -fvisibility=hidden -Wno-ctor-dtor-privacy -D__ANSI__")
endif ()

add_library(static_jxr ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
