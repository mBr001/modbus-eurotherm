#-------------------------------------------------
#
# Project created by QtCreator 2012-09-16T23:28:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = modbus-eurotherm
TEMPLATE = lib

CONFIG += staticlib

VERSION = 0.0.1

DEFINES += MODBUSEUROTHERM_LIBRARY

SOURCES += eurotherm.cpp

HEADERS += eurotherm.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
