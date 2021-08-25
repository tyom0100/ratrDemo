QT       += core network sql xml
QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    db_manager.cpp \
    manager.cpp \
    server.cpp \
    user.cpp

HEADERS += \
    constants.h \
    db_manager.h \
    manager.h \
    server.h \
    user.h
