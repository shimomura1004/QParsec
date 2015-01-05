#ifndef PARSER_H
#define PARSER_H

#include <QSharedPointer>
#include <QParsec.h>
#include <QParsecCombinator.h>
#include <token/tokens.h>
#include <token/scheme.h>

#include "ast.h"

namespace parser {
using namespace qparsec;
using namespace qparsec::chars;
using namespace qparsec::combinators;
using namespace qparsec::tokens;

Parser<ast::SharedVal> *Boolean() { return Apply(scheme::Boolean(), ast::Bool::create); }
Parser<ast::SharedVal> *Character() { return Apply(scheme::Character(), ast::Char::create); }
Parser<ast::SharedVal> *String() { return Apply(scheme::String(), ast::String::create); }
Parser<ast::SharedVal> *Number() {
    ast::SharedVal(*f)(scheme::SchemeNumber) = [](scheme::SchemeNumber num){
        switch(num.numtype) {
        case scheme::SchemeNumber::INTEGER:
            return ast::Integer::create(num.integer);
        case scheme::SchemeNumber::REAL:
            return ast::Real::create(num.real);
        case scheme::SchemeNumber::RATIONAL:
            return ast::Rational::create(num.rational.first, num.rational.second);
        case scheme::SchemeNumber::COMPLEX:
            return ast::Complex::create(num.complex.first, num.complex.second);
        }
    };

    return Apply(scheme::Number(), f);
}
// todo: return syntactic keyword or variable
Parser<ast::SharedVal> *Identifier() { return Apply(scheme::Identifier(), ast::Var::create); }

namespace datum {
Parser<ast::SharedVal> *Datum();

Parser<ast::SharedVal> *SimpleDatum() {
    return Choice({ Try(Boolean()),
                    Try(Number()),
                    Character(),
                    String(),
                    Identifier()
                  });
}

struct ParserList : Parser<ast::SharedVal> {
    struct ParserAbbriviation : Parser<ast::SharedVal> {
        ast::SharedVal parse(Input &input) {
            try {
                Char('\'')->parse(input);
                auto q = ast::Var::create("quote");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            try {
                Char('`')->parse(input);
                auto q = ast::Var::create("quasiquote");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            try {
                Str(",@")->parse(input);
                auto q = ast::Var::create("unquote-splicing");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            Char(',')->parse(input);
            auto q = ast::Var::create("unquote");
            auto datum = Datum()->parse(input);
            return ast::List::create(QList<ast::SharedVal>({q, datum}));
        }
    };
    Parser<ast::SharedVal> *Abbriviation() { return new ParserAbbriviation(); }

    ast::SharedVal parse(Input &input) {
        try {
            return Abbriviation()->parse(input);
        } catch (const ParserException &) {}

        Char('(')->parse(input);

        try {
            auto data = Many1(Datum())->parse(input);

            try {
                Lexeme(Char('.'))->parse(input);
                auto datum = Datum()->parse(input);
                Lexeme(Char(')'))->parse(input);
                return ast::DList::create(QList<ast::SharedVal>(data), datum);
            } catch (const ParserException &) {
                Lexeme(Char(')'))->parse(input);
                return ast::List::create(QList<ast::SharedVal>(data));
            }

        } catch (const ParserException &) {
            Lexeme(Char(')'))->parse(input);
            return ast::List::create(QList<ast::SharedVal>());
        }
    }
};
Parser<ast::SharedVal> *List() { return new ParserList(); }

Parser<ast::SharedVal> *Vector() {
 return Apply(Right(Char('#'), Parens(Many(Lazy(Datum)))), ast::Vector::create);
}

Parser<ast::SharedVal> *CompoundDatum() { return Choice({ List(), Vector() }); }

struct ParserDatum : Parser<ast::SharedVal> {
      ast::SharedVal parse(Input &input) {
          return Lexeme(Choice({Try(CompoundDatum()), SimpleDatum()}))->parse(input);
      }
};
Parser<ast::SharedVal> *Datum() { return new ParserDatum(); }

}





Parser<ast::SharedVal> *Val();

Parser<ast::SharedVal> *List() { return Right(Char('\''), Apply(Lexeme(Parens(Many(Val()))), ast::List::create)); }


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
                Lexeme(Choice({ Try(Number()),
                                Try(Boolean()),
                                Try(Character()),
                                String(),
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


Parser<ast::SharedVal> *Expression() {
    // variable
    // literal
    // procedure call
    // lambda expression
    // conditional
    // assignment
    // derived expression
    // macro use
    // macro block

    return Choice({

                      Try(Number()),
                    Try(Boolean()),
                    Try(Character()),
                    String(),
                    Variable(),
                    Try(List()),
                    Try(Lambda()),
                    Try(Apply()),
                  });
}




}

#endif // PARSER_H
