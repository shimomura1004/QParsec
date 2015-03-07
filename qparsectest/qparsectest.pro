include(../common.pri)
include(../qparsec/qparsec.pri)

QT       += core testlib
QT       -= gui

TARGET = qparsectest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tst_prim.cpp \
    tst_character.cpp \
    tst_combinator.cpp \
    tst_token.cpp

HEADERS += \
    autotest.h
