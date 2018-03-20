#-------------------------------------------------
#
# Project created by QtCreator 2015-12-21T23:16:04
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GC2
TEMPLATE = app


SOURCES += \
    main.cpp \
    histogramm.cpp \
    fleximage.cpp \
    redcross.cpp \
    pictureinfo.cpp \
    histogrammausgleich.cpp \
    filterlinear.cpp \
    filter.cpp \
    geradendetektor.cpp \
    histogrammtab.cpp \
    controller.cpp \
    imageviewer.cpp \
    functionbox.cpp \
    filtertest.cpp \
    edge_detection_USM.cpp \
    infotab.cpp \
    stdslider.cpp \
    utils.cpp \
    miscoperations.cpp

HEADERS  += \
    histogramm.h \
    fleximage.h \
    redcross.h \
    pictureinfo.h \
    histogrammausgleich.h \
    filterlinear.h \
    filter.h \
    geradendetektor.h \
    histogrammtab.h \
    controller.h \
    imageviewer.h \
    functionbox.h \
    filtertest.h \
    edge_detection_USM.h \
    infotab.h \
    stdslider.h \
    utils.h \
    miscoperations.h
