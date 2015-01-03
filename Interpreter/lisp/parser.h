#ifndef PARSER_H
#define PARSER_H

#include <QSharedPointer>
#include <QParsec.h>
#include <QParsecCombinator.h>
#include <token/lisp.h>
#include "ast.h"

using namespace qparsec;

namespace lisp {
namespace parser {
using namespace qparsec::combinators;
using namespace qparsec::tokens::lisp;

Parser<ast::SharedVal> *Val();

Parser<ast::SharedVal> *Int() {
    ast::SharedVal(*f)(int) = [](int n){ return ast::Int::create(n); };
    return Apply(Integer(), f);
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

struct ParserVar : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        auto c = Letter()->parse(input);
        auto cs = Many(Alphanum())->parse(input);
        return ast::Symbol::create(c + cs);
    }
};
Parser<ast::SharedVal> *Var() { return new ParserVar(); }

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
                Lexeme(Choice({ Int(),
                                Var(),
                                Try(List()),
                                Try(Apply()),
                                Try(Lambda()),
                                Parens(Val())
                              }))->parse(input);
        return val;
    }
};
Parser<ast::SharedVal> *Val() { return new ParserVal(); }

}
}

#endif // PARSER_H
