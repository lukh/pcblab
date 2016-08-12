#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T13:11:28
#
#-------------------------------------------------


TARGET = apertureprimitive
TEMPLATE = app



INCLUDEPATH += ../../../src/pcblab/gerber/


SOURCES += \
    test_apertureprimitive.cpp \
    ../../../src/pcblab/gerber/aperture/apertureprimitive.cpp \
    ../../../src/pcblab/common.cpp

HEADERS  += \
    ../../../src/pcblab/gerber/aperture/apertureprimitive.h \
    test_apertureprimitive.h



LIBS += -lcppunit


#cairo
CAIRO_PATH="D:/libs/build_cairo/cairo/install"

CAIRO_LIBS_PATH="$$CAIRO_PATH/lib"


LIBS += -L$$CAIRO_LIBS_PATH \
   -lcairo -lpixman-1 -llibpng




INCLUDEPATH += \
    $$CAIRO_PATH/include/

