#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:22:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtclient
TEMPLATE = app

LIBS += -lwsock32

INCLUDEPATH += ../common/

SOURCES += main.cpp\
        client.cpp

HEADERS  += client.h

FORMS    += client.ui
