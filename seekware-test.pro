#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T15:33:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seekware-test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -pthread
QMAKE_CFLAGS += -std=c++11 -pthread
LIBS += -pthread

CONFIG += console

INCLUDEPATH += /usr/include/c++/4.9.2
INCLUDEPATH += /usr/local/include/seekware

QMAKE_LFLAGS += -L/usr/local/lib -lseekware -lstdc++ -lpthread `pkg-config --libs libusb-1.0`
