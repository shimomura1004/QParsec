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
    ast/delay.cpp \
    parser/datum/parsersimpledatum.cpp \
    parser/datum/parserboolean.cpp \
    parser/datum/parsercharacter.cpp \
    parser/datum/parseridentifier.cpp \
    parser/datum/parsernumber.cpp \
    parser/datum/parserstring.cpp \
    parser/datum/parservariable.cpp \
    parser/datum/parserlist.cpp \
    parser/datum/parserdatum.cpp \
    parser/datum/parservector.cpp \
    parser/datum/parsercompounddatum.cpp \
    parser/expression/parserexpression.cpp \
    parser/expression/parserquotation.cpp \
    parser/expression/parserliteral.cpp \
    parser/expression/parserselfevaluating.cpp \
    parser/expression/parserprocedurecall.cpp \
    parser/expression/parsersequence.cpp \
    parser/expression/parserbody.cpp \
    parser/expression/parserlambda.cpp \
    parser/expression/parsercondition.cpp \
    parser/expression/parserassignment.cpp \
    parser/expression/parsercond.cpp \
    parser/expression/parsercase.cpp \
    parser/expression/parserand.cpp \
    parser/expression/parseror.cpp \
    parser/expression/parserbindingspec.cpp \
    parser/expression/parserlet.cpp \
    parser/expression/parserbegin.cpp \
    parser/expression/parserdo.cpp \
    parser/expression/parserdelay.cpp \
    parser/expression/parserderivedexpression.cpp

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
    ast/ast.h \
    parser/datum/parsersimpledatum.h \
    parser/datum/datum.h \
    parser/datum/parserboolean.h \
    parser/datum/parsercharacter.h \
    parser/datum/parseridentifier.h \
    parser/datum/parsernumber.h \
    parser/datum/parserstring.h \
    parser/datum/parservariable.h \
    parser/datum/parserlist.h \
    parser/datum/parserdatum.h \
    parser/datum/parservector.h \
    parser/datum/parsercompounddatum.h \
    parser/expression/parserexpression.h \
    parser/expression/parserquotation.h \
    parser/expression/parserliteral.h \
    parser/expression/parserselfevaluating.h \
    parser/expression/parserprocedurecall.h \
    parser/expression/parsersequence.h \
    parser/expression/parserbody.h \
    parser/expression/parserlambda.h \
    parser/expression/parsercondition.h \
    parser/expression/parserassignment.h \
    parser/expression/parsercond.h \
    parser/expression/parsercase.h \
    parser/expression/parserand.h \
    parser/expression/parseror.h \
    parser/expression/parserbindingspec.h \
    parser/expression/parserlet.h \
    parser/expression/parserbegin.h \
    parser/expression/parserdo.h \
    parser/expression/parserdelay.h \
    parser/expression/parserderivedexpression.h \
    parser/expression/expression.h

