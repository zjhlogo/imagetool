cmake_minimum_required (VERSION 3.1.0)
project (static_freeimage)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../../libjpeg/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libjxr/source)
include_directories(${PROJECT_SOURCE_DIR}/../../librawlite/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/Half)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/Iex)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/IexMath)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/IlmImf)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/IlmThread)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenexr/source/Imath)
include_directories(${PROJECT_SOURCE_DIR}/../../libopenjpeg/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libpng/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libtiff4/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libwebp/source)
include_directories(${PROJECT_SOURCE_DIR}/../../libzlib/source)

set(ALL_FILES
CacheFile.h
DeprecationManager/Deprecated.cpp
DeprecationManager/DeprecationMgr.cpp
DeprecationManager/DeprecationMgr.h
FreeImage.h
FreeImage/BitmapAccess.cpp
FreeImage/CacheFile.cpp
FreeImage/ColorLookup.cpp
FreeImage/Conversion.cpp
FreeImage/Conversion4.cpp
FreeImage/Conversion8.cpp
FreeImage/Conversion16_555.cpp
FreeImage/Conversion16_565.cpp
FreeImage/Conversion24.cpp
FreeImage/Conversion32.cpp
FreeImage/ConversionFloat.cpp
FreeImage/ConversionRGB16.cpp
FreeImage/ConversionRGBA16.cpp
FreeImage/ConversionRGBAF.cpp
FreeImage/ConversionRGBF.cpp
FreeImage/ConversionType.cpp
FreeImage/ConversionUINT16.cpp
FreeImage/FreeImage.cpp
FreeImage/FreeImageC.c
FreeImage/FreeImageIO.cpp
FreeImage/GetType.cpp
FreeImage/Halftoning.cpp
FreeImage/J2KHelper.cpp
FreeImage/J2KHelper.h
FreeImage/LFPQuantizer.cpp
FreeImage/MemoryIO.cpp
FreeImage/MNGHelper.cpp
FreeImage/MultiPage.cpp
FreeImage/NNQuantizer.cpp
FreeImage/PixelAccess.cpp
FreeImage/Plugin.cpp
FreeImage/PluginBMP.cpp
FreeImage/PluginCUT.cpp
FreeImage/PluginDDS.cpp
FreeImage/PluginEXR.cpp
FreeImage/PluginG3.cpp
FreeImage/PluginGIF.cpp
FreeImage/PluginHDR.cpp
FreeImage/PluginICO.cpp
FreeImage/PluginIFF.cpp
FreeImage/PluginJ2K.cpp
FreeImage/PluginJNG.cpp
FreeImage/PluginJP2.cpp
FreeImage/PluginJPEG.cpp
FreeImage/PluginJXR.cpp
FreeImage/PluginKOALA.cpp
FreeImage/PluginMNG.cpp
FreeImage/PluginPCD.cpp
FreeImage/PluginPCX.cpp
FreeImage/PluginPFM.cpp
FreeImage/PluginPICT.cpp
FreeImage/PluginPNG.cpp
FreeImage/PluginPNM.cpp
FreeImage/PluginPSD.cpp
FreeImage/PluginRAS.cpp
FreeImage/PluginRAW.cpp
FreeImage/PluginSGI.cpp
FreeImage/PluginTARGA.cpp
FreeImage/PluginTIFF.cpp
FreeImage/PluginWBMP.cpp
FreeImage/PluginWebP.cpp
FreeImage/PluginXBM.cpp
FreeImage/PluginXPM.cpp
FreeImage/PSDParser.cpp
FreeImage/PSDParser.h
FreeImage/TIFFLogLuv.cpp
FreeImage/tmoColorConvert.cpp
FreeImage/tmoDrago03.cpp
FreeImage/tmoFattal02.cpp
FreeImage/tmoReinhard05.cpp
FreeImage/ToneMapping.cpp
FreeImage/WuQuantizer.cpp
FreeImage/ZLibInterface.cpp
FreeImagePlus/fipImage.cpp
FreeImagePlus/fipMemoryIO.cpp
FreeImagePlus/fipMetadataFind.cpp
FreeImagePlus/fipMultiPage.cpp
FreeImagePlus/fipTag.cpp
FreeImagePlus/fipWinImage.cpp
FreeImagePlus/FreeImagePlus.cpp
FreeImagePlus/FreeImagePlus.h
FreeImageIO.h
FreeImageToolkit/Background.cpp
FreeImageToolkit/BSplineRotate.cpp
FreeImageToolkit/Channels.cpp
FreeImageToolkit/ClassicRotate.cpp
FreeImageToolkit/Colors.cpp
FreeImageToolkit/CopyPaste.cpp
FreeImageToolkit/Display.cpp
FreeImageToolkit/Filters.h
FreeImageToolkit/Flip.cpp
FreeImageToolkit/JPEGTransform.cpp
FreeImageToolkit/MultigridPoissonSolver.cpp
FreeImageToolkit/Rescale.cpp
FreeImageToolkit/Resize.cpp
FreeImageToolkit/Resize.h
MapIntrospector.h
Metadata/Exif.cpp
Metadata/FIRational.cpp
Metadata/FIRational.h
Metadata/FreeImageTag.cpp
Metadata/FreeImageTag.h
Metadata/IPTC.cpp
Metadata/TagConversion.cpp
Metadata/TagLib.cpp
Metadata/XTIFF.cpp
Plugin.h
Quantizers.h
ToneMapping.h
Utilities.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPJ_STATIC -DFREEIMAGE_LIB -DLIBRAW_NODLL")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_PERF_MEASUREMENT -D__ANSI__ -DOPJ_STATIC -DNO_LCMS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -fexceptions -fvisibility=hidden -Wno-ctor-dtor-privacy -D__ANSI__ -DOPJ_STATIC -DNO_LCMS")
endif ()

add_library(static_freeimage ${ALL_FILES})

foreach(FILE ${ALL_FILES}) 
    get_filename_component(PARENT_DIR "${FILE}" PATH)

    # skip source and changes /'s to \\'s
    string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
