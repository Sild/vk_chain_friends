#-------------------------------------------------
#
# Project created by QtCreator 2014-01-09T23:35:40
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network
QT       += widgets

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += link_pkgconfig
CONFIG += c++11

PKGCONFIG += vreen \
            vreenoauth

TEMPLATE = app


SOURCES += main.cpp \
    phonefetcher.cpp

HEADERS += \
    phonefetcher.h
