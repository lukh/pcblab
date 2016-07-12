#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T13:11:28
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += no_keywords

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcblab
TEMPLATE = app


INCLUDEPATH += ../src \
    ../src/pcblab/gerber \
    ../src/pcblab/viewprocessor \
    ../src/gui \
    ../src/gui/opencvviewer \
    ../src/gui/cairogerberviewer


SOURCES += \
    ../src/main.cpp \
    ../src/gui/mainwindow.cpp \
    ../src/pcblab/gerber/gerberlayer.cpp \
    ../src/pcblab/gerber/syntaxparser.cpp \
    ../src/pcblab/gerber/graphicobject.cpp \
    #../src/tests/pcblab/gerber/gerberlayertest.cpp \
    ../src/pcblab/common.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_dcode.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_gcode.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_xcode.cpp \
    ../src/pcblab/gerber/aperture/aperture.cpp \
    ../src/pcblab/gerber/aperture/aperturetemplate.cpp \
    ../src/pcblab/gerber/aperture/apertureprimitive.cpp \
    ../src/gui/opencvviewer/qtopencvviewer.cpp \
    ../src/pcblab/viewprocessor/viewprocessor.cpp \
    ../src/gui/cairogerberviewer/cairogerberviewer.cpp

HEADERS  += \
    ../src/pcblab/gerber/aperture/aperture.h \
    ../src/pcblab/gerber/aperture/aperturetemplate.h \
    ../src/pcblab/gerber/gerberhandler.h \
    ../src/pcblab/gerber/gerberlayer.h \
    ../src/pcblab/gerber/graphicobject.h \
    ../src/pcblab/gerber/graphicstate.h \
    ../src/pcblab/gerber/igerberview.h \
    ../src/pcblab/gerber/syntaxparser.h \
    ../src/pcblab/common.h \
    ../src/gui/mainwindow.h \
    #../src/tests/pcblab/gerber/gerberlayertest.h \
    ../src/gui/q_debugstream.h \
    ../src/pcblab/gerber/aperture/aperturemodifier.h \
    ../src/pcblab/gerber/aperture/apertureprimitive.h \
    ../src/tools/exprparser.h \
    ../src/tools/stringsplitter.h \
    ../src/gui/opencvviewer/qtopencvviewer.h \
    ../src/pcblab/viewprocessor/viewprocessor.h \
    ../src/gui/cairogerberviewer/cairogerberviewer.h
    ../src/gui/opencvviewer/iopencvviewer.h





# ----------- opencv ----------
OPENCV_PATH = "D:\libs\opencv24\build\install" # Note: update with the correct OpenCV version

OCV_LIBS_PATH = "$$OPENCV_PATH/x86/mingw/lib" #project compiled using Visual C++ 2010 32bit compiler

LIBS     += -L$$OCV_LIBS_PATH \
-lopencv_calib3d2413 \
-lopencv_contrib2413 \
-lopencv_core2413 \
-lopencv_features2d2413 \
-lopencv_flann2413 \
-lopencv_gpu2413 \
-lopencv_imgproc2413 \
-lopencv_legacy2413 \
-lopencv_ml2413 \
-lopencv_objdetect2413 \
-lopencv_ts2413 \
-lopencv_video2413 \
-lopencv_highgui2413 \
-lopencv_nonfree2413 \
-lopencv_photo2413 \
-lopencv_stitching2413 \
-lopencv_videostab2413 \
-llibpng


#cairo
CAIRO_PATH="D:/libs/build_cairo/cairo/install"

CAIRO_LIBS_PATH="$$CAIRO_PATH/lib"


LIBS += -L$$CAIRO_LIBS_PATH \
   -lcairo -lpixman-1 -llibpng




INCLUDEPATH += \
    $$OPENCV_PATH/include/ \
    $$CAIRO_PATH/include/



FORMS += \
    ../src/gui/mainwindow.ui

DISTFILES += \
    ../doc/the_gerber_file_format_specification.pdf \
    ../data/apertures.grb \
    ../data/contours.grb



