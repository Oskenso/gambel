#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T17:43:01
#
#-------------------------------------------------

QT       -= core gui

TARGET = gambel

TEMPLATE = lib

DEFINES += LIBGAMBEL_LIBRARY

SOURCES += \
    libgambel.c \
    cpu.c

HEADERS += libgambel.h \
    defines.h \
    cpu.h \
    memory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CFLAGS += -march=native

CONFIG += staticlib
