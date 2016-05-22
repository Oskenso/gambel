#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T17:43:01
#
#-------------------------------------------------

QT       -= core gui

TARGET = libgambel

TEMPLATE = lib

DEFINES += LIBGAMBEL_LIBRARY

SOURCES += \
    libgambel.c

HEADERS += libgambel.h \
    defines.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
