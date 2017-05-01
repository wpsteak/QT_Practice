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
        mainwindow.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    optionsdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui

QMAKE_CXXFLAGS += -std=c++11 -pthread
QMAKE_CFLAGS += -std=c++11 -pthread
LIBS += -pthread
LIBS += -L/usr/local/lib
#LIBS += -L/usr/lib/arm-linux-gnueabihf/libusb-1.0.a
LIBS += `pkg-config --libs libusb-1.0`

CONFIG += console

INCLUDEPATH += /usr/include/c++/4.9.2
INCLUDEPATH += /usr/local/include/seekware
INCLUDEPATH += /usr/include/libusb-1.0

QMAKE_LFLAGS += -L/usr/local/lib -lseekware -lstdc++ -lpthread `pkg-config --libs libusb-1.0`
