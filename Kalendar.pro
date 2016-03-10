#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T17:50:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kalendar
TEMPLATE = app


SOURCES += main.cpp\
        kalendar.cpp \
    persistence/pmanager.cpp \
    test/test.cpp

HEADERS  += kalendar.h \
    event.h \
    persistence/pmanager.h \
    category.h \
    test/test.h

FORMS    += kalendar.ui

QMAKE_LIBS += -lsqlite3
