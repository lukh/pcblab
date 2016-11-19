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
    ../src/gui/tools \
    ../src/gui/opencvwidget \
    ../src/gui/cairogerberrenderer \
    ../src/gui/cairowidget


SOURCES += \
    ../src/main.cpp \
    ../src/gui/mainwindow.cpp \
    ../src/pcblab/gerber/gerberlayer.cpp \
    ../src/pcblab/gerber/graphicobject.cpp \
    #../src/tests/pcblab/gerber/gerberlayertest.cpp \
    ../src/pcblab/common.cpp \
    ../src/pcblab/gerber/gerberparser/gerberparser_dcode.cpp \
    ../src/pcblab/gerber/gerberparser/gerberparser_gcode.cpp \
    ../src/pcblab/gerber/gerberparser/gerberparser_xcode.cpp \
    ../src/pcblab/gerber/aperture/aperture.cpp \
    ../src/pcblab/gerber/aperture/aperturetemplate.cpp \
    ../src/pcblab/gerber/aperture/apertureprimitive.cpp \
    ../src/pcblab/pcblab.cpp \
    ../src/pcblab/gerber/gerberhandler.cpp \
    #../src/gui/processorwrapper.cpp \
    ../src/tools/fio.cpp \
    ../src/gui/cairogerberrenderer/cgv_draws.cpp \
    ../src/gui/cairogerberrenderer/cgv_arcs.cpp \
    ../src/gui/cairogerberrenderer/cgb_regions.cpp \
    ../src/gui/cairogerberrenderer/cgv_flashs.cpp \
    ../src/gui/cairoviewport/cairoviewport.cpp \
    ../src/gui/cairogerberrenderer/cairogerberrenderer.cpp \
    ../src/gui/icairoviewer.cpp \
    ../src/pcblab/viewprocessor/displayviewprocessor.cpp \
    ../src/gui/tools/qlayerconfigwidget.cpp \
    ../src/pcblab/gerber/igerberview.cpp \
    ../src/gui/cairowidget/qcairowidget.cpp \
    ../src/gui/opencvwidget/qopencvwidget.cpp \
    ../src/gui/qpcblabdisplayerwidget.cpp \
    ../src/pcblab/viewprocessor/iviewprocessor.cpp \
    ../src/pcblab/excellon/excellonparser.cpp \
    ../src/pcblab/gerber/gerberparser.cpp \
    ../src/pcblab/excellon/excellonstate.cpp \
    ../src/pcblab/excellon/excellonhandler.cpp \

HEADERS  += \
    ../src/pcblab/gerber/aperture/aperture.h \
    ../src/pcblab/gerber/aperture/aperturetemplate.h \
    ../src/pcblab/gerber/gerberhandler.h \
    ../src/pcblab/gerber/gerberlayer.h \
    ../src/pcblab/gerber/graphicobject.h \
    ../src/pcblab/gerber/graphicstate.h \
    ../src/pcblab/gerber/igerberview.h \
    ../src/pcblab/common.h \
    ../src/gui/mainwindow.h \
    #../src/tests/pcblab/gerber/gerberlayertest.h \
    ../src/gui/q_debugstream.h \
    ../src/pcblab/gerber/aperture/aperturemodifier.h \
    ../src/pcblab/gerber/aperture/apertureprimitive.h \
    ../src/tools/exprparser.h \
    ../src/tools/stringsplitter.h \
    ../src/pcblab/pcblab.h \
    #../src/gui/processorwrapper.h \
    ../src/config.h \
    ../src/gui/cairoviewport/cairoviewport.h \
    ../src/gui/cairogerberrenderer/cairogerberrenderer.h \
    ../src/gui/icairoviewer.h \
    ../src/pcblab/viewprocessor/displayviewprocessor.h \
    ../src/gui/tools/qdisplayer.h \
    ../src/gui/tools/qlayerconfigwidget.h \
    ../src/gui/cairowidget/icairowidget.h \
    ../src/tools/fio.h \
    ../src/gui/cairowidget/qcairowidget.h \
    ../src/gui/opencvwidget/qopencvwidget.h \
    ../src/gui/opencvwidget/iopencvwidget.h \
    ../src/gui/qpcblabdisplayerwidget.h \
    ../src/pcblab/viewprocessor/iviewprocessor.h \
    ../src/pcblab/excellon/excellonparser.h \
    ../src/pcblab/gerber/gerberparser.h \
    ../src/pcblab/excellon/excellonstate.h \
    ../src/pcblab/excellon/excellonhandler.h \






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
    ../src/gui/mainwindow.ui \
    ../src/gui/qpcblabdisplayerwidget.ui

DISTFILES += \
    ../doc/the_gerber_file_format_specification.pdf \
    ../data/apertures.grb \
    ../data/contours.grb \
    ../data/firstdraws.grb \
    ../data/arcs.grb \
    ../TODO



