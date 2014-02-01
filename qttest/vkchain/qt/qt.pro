#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:21:54
#
#-------------------------------------------------

QT       += core webkitwidgets network

QT       -= gui
TARGET = qt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    worker.cpp \
    queue.cpp \
    controller.cpp \
    json.cpp

HEADERS += \
    worker.h \
    queue.h \
    controller.h \
    json.h

