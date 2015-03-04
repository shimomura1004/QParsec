#include "parservariable.h"

namespace qparsec {
namespace language {
namespace scheme {

const QStringList ParserVariable::ExpressionKeyword = {
    "quote", "lambda", "if", "set!", "begin", "cond", "and", "or", "case",
    "let", "let*", "letrec", "do", "delay", "quasiquote"
};

const QStringList ParserVariable::SyntacticKeyword = {
    "else", "=>", "define", "unquote", "unquote-splicing"
};

QString ParserVariable::parse(Input &input) {
    input.preserve();

    auto ident = Identifier()->parse(input);
    if (SyntacticKeyword.contains(ident) || ExpressionKeyword.contains(ident)) {
        input.restore();
        throw ParserException(input.index(), QStringLiteral("%1 is reserved identifier").arg(ident));
    }
    return ident;
}

Parser<QString> *Variable() { return new ParserVariable(); }

}
}
}
