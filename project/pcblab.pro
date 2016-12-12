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
    ../src/pcblab/excellon \
    ../src/pcblab/bom \
    ../src/pcblab/viewprocessor \
    ../src/gui \
    ../src/gui/tools \
    ../src/gui/opencvwidget \
    ../src/gui/cairorenderer \
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
    ../src/gui/cairorenderer/cgv_draws.cpp \
    ../src/gui/cairorenderer/cgv_arcs.cpp \
    ../src/gui/cairorenderer/cgb_regions.cpp \
    ../src/gui/cairorenderer/cgv_flashs.cpp \
    ../src/gui/cairoviewport/cairoviewport.cpp \
    ../src/gui/cairorenderer/cairogerberrenderer.cpp \
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
    ../src/gui/cairorenderer/cairoexcellonrenderer.cpp \
    ../src/gui/cairorenderer/icairorenderer.cpp \
    ../src/pcblab/bom/bomparser.cpp \
    ../src/pcblab/bom/componenthandler.cpp \
    ../src/pcblab/bom/pnpparser.cpp \
    ../src/tools/csvparser.cpp \
    ../src/tools/stringoperations.cpp \
    ../src/gui/cairorenderer/cairocomponentrenderer.cpp \
    ../src/pcblab/bom/icomponentrenderer.cpp \
    ../src/gui/componentdisplayerwidget.cpp \
    ../src/pcblab/netlist/netlisthandler.cpp \
    ../src/pcblab/netlist/netlistparser.cpp

HEADERS  += \
    ../src/pcblab/gerber/aperture/aperture.h \
    ../src/pcblab/gerber/aperture/aperturetemplate.h \
    ../src/pcblab/gerber/gerberhandler.h \
    ../src/pcblab/gerber/gerberlayer.h \
    ../src/pcblab/gerber/graphicobject.h \
    ../src/pcblab/gerber/graphicstate.h \
    ../src/pcblab/common.h \
    ../src/gui/mainwindow.h \
    #../src/tests/pcblab/gerber/gerberlayertest.h \
    ../src/gui/q_debugstream.h \
    ../src/pcblab/gerber/aperture/aperturemodifier.h \
    ../src/pcblab/gerber/aperture/apertureprimitive.h \
    ../src/tools/exprparser.h \
    ../src/pcblab/pcblab.h \
    #../src/gui/processorwrapper.h \
    ../src/config.h \
    ../src/gui/cairoviewport/cairoviewport.h \
    ../src/gui/cairorenderer/cairogerberrenderer.h \
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
    ../src/gui/cairorenderer/cairoexcellonrenderer.h \
    ../src/pcblab/gerber/igerberrenderer.h \
    ../src/pcblab/excellon/iexcellonrenderer.h \
    ../src/gui/cairorenderer/icairorenderer.h \
    ../src/tools/csvparser.h \
    ../src/pcblab/bom/bomparser.h \
    ../src/pcblab/bom/componenthandler.h \
    ../src/pcblab/bom/component.h \
    ../src/pcblab/bom/pnpparser.h \
    ../src/tools/stringoperations.h \
    ../src/gui/cairorenderer/cairocomponentrenderer.h \
    ../src/pcblab/bom/icomponentrenderer.h \
    ../src/gui/componentdisplayerwidget.h \
    ../src/pcblab/netlist/netlisthandler.h \
    ../src/pcblab/netlist/netlistparser.h






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
    ../src/gui/qpcblabdisplayerwidget.ui \
    ../src/gui/componentdisplayerwidget.ui

DISTFILES += \
    ../doc/the_gerber_file_format_specification.pdf \
    ../data/apertures.grb \
    ../data/contours.grb \
    ../data/firstdraws.grb \
    ../data/arcs.grb \
    ../TODO



