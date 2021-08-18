#-------------------------------------------------
#
# Project created by QtCreator 2017-01-29T07:42:54
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = chat_server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    user.cpp

HEADERS += \
    server.h \
    user.h
