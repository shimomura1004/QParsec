#ifndef PARSER_H
#define PARSER_H

#include <QSharedPointer>
#include <QParsec.h>
#include <QParsecCombinator.h>
#include <token/scheme.h>
#include <token/clang.h>
#include "ast.h"

using namespace qparsec;

namespace lisp {
namespace parser {
using namespace qparsec::combinators;
using namespace qparsec::tokens;
using namespace qparsec::tokens::clang;
using namespace qparsec::tokens::scheme;

Parser<ast::SharedVal> *Val();

Parser<ast::SharedVal> *Int() {
    ast::SharedVal(*f)(int64_t) = [](int64_t n){ return ast::Int::create(n); };
    return Apply(scheme::Num(), f);
}

Parser<ast::SharedVal> *List() {
    ast::SharedVal(*f)(QList<ast::SharedVal>) = [](QList<ast::SharedVal> list){ return ast::List::create(list); };
    return Right(Char('\''), Apply(Lexeme(Parens(Many(Val()))), f));
}

/**
 * (lambda (x y) (+ x y))
 */
struct ParserLambda : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        Char('(')->parse(input);
        Symbol("lambda")->parse(input);
        auto vars = Parens(SepBy(Many1(OneOf("abcdefghijklmnopqrstuvwxyz")), SkipMany1(Space())))->parse(input);
        WhiteSpace()->parse(input);
        auto body = Val()->parse(input);
        Char(')')->parse(input);
        WhiteSpace()->parse(input);
        return ast::Lambda::create(vars, body, ast::Env());
    }
};
Parser<ast::SharedVal> *Lambda() { return new ParserLambda(); }

Parser<ast::SharedVal> *Variable() {
    return Apply(qparsec::tokens::scheme::Identifier(), ast::Var::create);
}

struct ParserApply : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        auto es = Lexeme(Parens(Many1(Val())))->parse(input);
        return ast::Apply::create(es.first(), es.mid(1));
    }
};
Parser<ast::SharedVal> *Apply() { return new ParserApply(); }

Parser<ast::SharedVal> *Val();
struct ParserVal : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        ast::SharedVal val =
                Lexeme(Choice({ Try(Int()),
                                Variable(),
                                Try(List()),
                                Try(Lambda()),
                                Try(Apply()),
                                Parens(Val())
                              }))->parse(input);
        return val;
    }
};
Parser<ast::SharedVal> *Val() { return new ParserVal(); }

}
}

#endif // PARSER_H
