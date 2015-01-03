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

Parser<ast::SharedVal> *Val();

Parser<ast::SharedVal> *Int() { return Apply(scheme::Num(), ast::Int::create); }
Parser<ast::SharedVal> *Boolean() { return Apply(scheme::Boolean(), ast::Bool::create); }
Parser<ast::SharedVal> *Character() { return Apply(scheme::Character(), ast::Char::create); }
Parser<ast::SharedVal> *List() { return Right(Char('\''), Apply(Lexeme(Parens(Many(Val()))), ast::List::create)); }

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

Parser<ast::SharedVal> *Variable() { return Apply(qparsec::tokens::scheme::Identifier(), ast::Var::create); }

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
                                Try(Boolean()),
                                Try(Character()),
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
