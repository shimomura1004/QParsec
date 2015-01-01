#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T01:23:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Interpreter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += ../QParsec
CONFIG += c++11

HEADERS += \
    lisp/Ast.h \
    lisp/Parser.h
