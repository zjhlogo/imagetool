cmake_minimum_required (VERSION 3.1.0)
project (static_jpeg)

set(ALL_FILES
jaricom.c
jcapimin.c
jcapistd.c
jcarith.c
jccoefct.c
jccolor.c
jcdctmgr.c
jchuff.c
jcinit.c
jcmainct.c
jcmarker.c
jcmaster.c
jcomapi.c
jconfig.h
jcparam.c
jcprepct.c
jcsample.c
jctrans.c
jdapimin.c
jdapistd.c
jdarith.c
jdatadst.c
jdatasrc.c
jdcoefct.c
jdcolor.c
jdct.h
jddctmgr.c
jdhuff.c
jdinput.c
jdmainct.c
jdmarker.c
jdmaster.c
jdmerge.c
jdpostct.c
jdsample.c
jdtrans.c
jerror.c
jerror.h
jfdctflt.c
jfdctfst.c
jfdctint.c
jidctflt.c
jidctfst.c
jidctint.c
jinclude.h
jmemmgr.c
jmemnobs.c
jmemsys.h
jmorecfg.h
jpegint.h
jpeglib.h
jpegtran.c
jquant1.c
jquant2.c
jutils.c
jversion.h
transupp.c
transupp.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(static_jpeg ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
