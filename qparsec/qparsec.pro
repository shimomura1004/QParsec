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
    character/parsersatisfy.cpp

HEADERS += \
    parser.h \
    input.h \
    parserexception.h \
    parserempty.h \
    parserreturn.h \
    parserignore.h \
    parserseq.h \
    parserfail.h \
    parsertry.h \
    parserleft.h \
    parserright.h \
    parserpair.h \
    parserhelp.h \
    parserapply.h \
    parserapply2.h \
    parserlazy.h \
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
    combinator/combinator.h
