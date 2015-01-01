#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T16:10:26
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_asttesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_asttesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

CONFIG += c++11
INCLUDEPATH += ../Interpreter
