#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T13:11:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcblab
TEMPLATE = app


INCLUDEPATH += ../src


SOURCES += \
    ../src/main.cpp \
    ../src/gui/mainwindow.cpp \
    ../src/pcblab/gerber/gerberlayer.cpp \
    ../src/pcblab/gerber/syntaxparser.cpp \
    ../src/pcblab/gerber/graphicobject.cpp \
    ../src/tests/pcblab/gerber/gerberlayertest.cpp \
    ../src/pcblab/common.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_dcode.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_gcode.cpp \
    ../src/pcblab/gerber/syntaxparser/syntaxparser_xcode.cpp

HEADERS  += \
    ../src/pcblab/gerber/aperture/aperture.h \
    ../src/pcblab/gerber/aperture/apertureparam.h \
    ../src/pcblab/gerber/aperture/aperturetemplate.h \
    ../src/pcblab/gerber/gerberhandler.h \
    ../src/pcblab/gerber/gerberlayer.h \
    ../src/pcblab/gerber/graphicobject.h \
    ../src/pcblab/gerber/graphicstate.h \
    ../src/pcblab/gerber/igerberview.h \
    ../src/pcblab/gerber/syntaxparser.h \
    ../src/pcblab/common.h \
    ../src/gui/mainwindow.h \
    ../src/tests/pcblab/gerber/gerberlayertest.h \
    ../src/gui/q_debugstream.h

FORMS += \
    ../src/gui/mainwindow.ui

DISTFILES +=


