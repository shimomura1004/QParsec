include(../common.pri)

QT       -= gui

TARGET = qparsec
TEMPLATE = lib

DESTDIR = $${PROLIB}

DEFINES += QPARSEC_LIBRARY

SOURCES += \
    input.cpp \
    parserexception.cpp \
    character/parseroneof.cpp \
    character/parsernoneof.cpp \
    character/parserchar.cpp \
    character/parserstr.cpp \
    character/parseranychar.cpp \
    character/parserspace.cpp \
    character/parsersatisfy.cpp \
    language/scheme/scheme.cpp \
    language/scheme/parseridentifier.cpp \
    prim/parserfail.cpp \
    prim/parserempty.cpp \
    combinator/parsercount.cpp \
    combinator/parsereof.cpp \
    combinator/parsermany.cpp

HEADERS += \
    parser.h \
    input.h \
    parserexception.h \
    qparsec.h \
    character/parseroneof.h \
    character/parsernoneof.h \
    character/parserchar.h \
    character/parserstr.h \
    character/parseranychar.h \
    character/parserspace.h \
    character/parsersatisfy.h \
    combinator/parsermany.h \
    character/character.h \
    combinator/parserskipmany.h \
    combinator/parserchoice.h \
    combinator/parsersepby.h \
    combinator/parserendby.h \
    combinator/parsercount.h \
    combinator/parserbetween.h \
    combinator/parseroption.h \
    combinator/parsereof.h \
    combinator/parserchain.h \
    combinator/parsrelookahead.h \
    combinator/combinator.h \
    prim/parserlazy.h \
    prim/parserapply.h \
    prim/parserapply2.h \
    prim/parserfail.h \
    prim/parserhelp.h \
    prim/parserleft.h \
    prim/parserpair.h \
    prim/parserright.h \
    prim/parserreturn.h \
    prim/parsertry.h \
    prim/parserempty.h \
    prim/parserignore.h \
    prim/parserseq.h \
    prim/prim.h \
    language/scheme/scheme.h \
    language/scheme/parseridentifier.h
