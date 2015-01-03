#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T11:24:18
#
#-------------------------------------------------

QT       -= gui

TARGET = QParsec
TEMPLATE = lib

DEFINES += QPARSEC_LIBRARY

SOURCES +=

HEADERS += \
    QParsec.h \
    QParsecChar.h \
    QParsecCombinator.h \
    token/lisp.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
