include(../common.pri)
include(../qparsec/qparsec.pri)

QT       += core
QT       -= gui

TARGET = qscheme
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ast/symbol.cpp \
    ast/variable.cpp \
    ast/char.cpp \
    ast/string.cpp \
    ast/bool.cpp \
    ast/integer.cpp \
    ast/real.cpp \
    ast/rational.cpp \
    ast/complex.cpp \
    ast/undef.cpp \
    ast/quote.cpp \
    ast/set.cpp \
    ast/cond.cpp \
    ast/case.cpp \
    ast/and.cpp \
    ast/or.cpp \
    ast/list.cpp \
    ast/dlist.cpp \
    ast/vector.cpp \
    ast/apply.cpp \
    ast/lambda.cpp \
    ast/define.cpp \
    ast/if.cpp \
    ast/let.cpp \
    ast/begin.cpp \
    ast/do.cpp \
    ast/delay.cpp

HEADERS += \
    ast/val.h \
    ast/symbol.h \
    ast/variable.h \
    ast/char.h \
    ast/string.h \
    ast/bool.h \
    ast/integer.h \
    ast/real.h \
    ast/rational.h \
    ast/complex.h \
    ast/undef.h \
    ast/quote.h \
    ast/set.h \
    ast/cond.h \
    ast/case.h \
    ast/and.h \
    ast/or.h \
    ast/list.h \
    ast/dlist.h \
    ast/vector.h \
    ast/apply.h \
    ast/lambda.h \
    ast/define.h \
    ast/if.h \
    ast/let.h \
    ast/begin.h \
    ast/do.h \
    ast/delay.h \
    ast/ast.h

