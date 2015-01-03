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
    token/scheme.h \
    token/clang.h \
    token/tokens.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
