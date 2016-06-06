#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T17:42:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gambel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libgambel/release/ -lgambel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libgambel/debug/ -lgambel
else:unix: LIBS += -L$$OUT_PWD/../libgambel/ -lgambel

INCLUDEPATH += $$PWD/../libgambel
DEPENDPATH += $$PWD/../libgambel


