include(../common.pri)

QT       -= gui

TARGET = qparsec
TEMPLATE = lib

DESTDIR = $${PROLIB}

DEFINES += QPARSEC_LIBRARY

SOURCES += \
    input.cpp \
    parserexception.cpp \
    prim/parserfail.cpp \
    prim/parserempty.cpp \
    prim/parsertry.cpp \
    character/parseroneof.cpp \
    character/parsernoneof.cpp \
    character/parserchar.cpp \
    character/parserstr.cpp \
    character/parseranychar.cpp \
    character/parserspace.cpp \
    character/parsersatisfy.cpp \
    combinator/parsercount.cpp \
    combinator/parsereof.cpp \
    combinator/parsermany.cpp \
    token/parsersemi.cpp \
    token/parserwhitespace.cpp \
    token/parsersymbol.cpp \
    token/parsercomma.cpp \
    token/parsercolon.cpp \
    token/parserdot.cpp \
    language/scheme/parseridentifier.cpp \
    language/scheme/parservariable.cpp \
    language/scheme/parserboolean.cpp \
    language/scheme/parsernumber.cpp \
    language/scheme/parsercharacter.cpp \
    language/scheme/parserstring.cpp \
    number/parserdecimal.cpp \
    number/parserhexadecimal.cpp \
    number/parseroctal.cpp \
    number/parsernatural.cpp \
    number/parserinteger.cpp \
    number/parserdouble.cpp

HEADERS += \
    parser.h \
    input.h \
    parserexception.h \
    qparsec.h \
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
    character/parseroneof.h \
    character/parsernoneof.h \
    character/parserchar.h \
    character/parserstr.h \
    character/parseranychar.h \
    character/parserspace.h \
    character/parsersatisfy.h \
    character/character.h \
    combinator/parsermany.h \
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
    token/parsersemi.h \
    token/parserlexeme.h \
    token/parserwhitespace.h \
    token/token.h \
    token/parsersymbol.h \
    token/parserparens.h \
    token/parserbraces.h \
    token/parserbrackets.h \
    token/parsersquares.h \
    token/parsercomma.h \
    token/parsercolon.h \
    token/parserdot.h \
    token/parsersemisep.h \
    token/parsercommasep.h \
    language/scheme/parseridentifier.h \
    language/scheme/parservariable.h \
    language/scheme/scheme.h \
    language/scheme/parserboolean.h \
    language/scheme/parsernumber.h \
    language/scheme/parsercharacter.h \
    language/scheme/parserstring.h \
    number/parserdecimal.h \
    number/number.h \
    number/parserhexadecimal.h \
    number/parseroctal.h \
    number/parsernatural.h \
    number/parserinteger.h \
    number/parserdouble.h
