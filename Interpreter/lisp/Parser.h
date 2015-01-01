#ifndef PARSER_H
#define PARSER_H

#include <QSharedPointer>
#include <QParsec.h>
#include "Ast.h"

namespace lisp {
namespace parser {
using namespace qparsec;

Parser<ast::SharedVal> *Val();

Parser<ast::SharedVal> *Int() {
    ast::SharedVal(*f)(int) = [](int n){ return ast::Int::create(n); };
    return Apply(Natural(), f);
}

struct ParserLambda : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        // (lambda (x y) (+ x y))
        Char('(')->parse(input);
        Symbol("lambda")->parse(input);
        auto vars = Parens(SepBy(Many1(OneOf("abcdefghijklmnopqrstuvwxyz")), SkipMany1(Space())))->parse(input);
        WhiteSpace()->parse(input);
        auto body = Val()->parse(input);
        Char(')')->parse(input);
        return ast::Lambda::create(vars, body, ast::Env());
    }
};
Parser<ast::SharedVal> *Lambda() { return new ParserLambda(); }

Parser<ast::SharedVal> *Val();
struct ParserVal : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        ast::SharedVal val =
                Choice({ Int(),
                         Lambda(),
                         Val()
                       })->parse(input);
        return val;
    }
};
Parser<ast::SharedVal> *Val() { return new ParserVal(); }

}
}

#endif // PARSER_H
