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


const QStringList ExpressionKeyword = {
    "quote", "lambda", "if", "set!", "begin", "cond", "and", "or", "case",
    "let", "let*", "letrec", "do", "delay", "quasiquote"
};
const QStringList SyntacticKeyword = {
    "else", "=>", "define", "unquote", "unquote-splicing"
};

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

Parser<ast::SharedVal> *Identifier() {
    ast::SharedVal(*f)(QString) = [](QString ident){
        if (SyntacticKeyword.contains(ident) || ExpressionKeyword.contains(ident))
            return ast::Symbol::create(ident);
        return ast::Variable::create(ident);
    };
    return Apply(scheme::Identifier(), f);
}

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
                auto q = ast::Variable::create("quote");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            try {
                Char('`')->parse(input);
                auto q = ast::Variable::create("quasiquote");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            try {
                Str(",@")->parse(input);
                auto q = ast::Variable::create("unquote-splicing");
                auto datum = Datum()->parse(input);
                return ast::List::create(QList<ast::SharedVal>({q, datum}));
            } catch (const ParserException &) {}

            Char(',')->parse(input);
            auto q = ast::Variable::create("unquote");
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



namespace expression {
Parser<ast::SharedVal> *Expression();

struct ParserQuotation : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        try {
            auto datum = Parens(Right(Lexeme(Str("quote")), datum::Datum()))->parse(input);
            return ast::Quote::create(datum);
        } catch (const ParserException &) {}

        Char('\'')->parse(input);
        auto datum = datum::Datum()->parse(input);
        return ast::Quote::create(datum);
    }
};
Parser<ast::SharedVal> *Quotation() { return new ParserQuotation(); }
Parser<ast::SharedVal> *SelfEvaluating() {
    return Choice({ Try(Boolean()), Try(Number()), Try(Character()), String() });
}
Parser<ast::SharedVal> *Literal() {
    return Choice({ Try(Quotation()), SelfEvaluating()});
}

Parser<ast::SharedVal> *ProcedureCall() {
    ast::SharedVal(*f)(QList<ast::SharedVal>) = [](QList<ast::SharedVal> ops){
        return ast::Apply::create(ops.first(), ops.mid(1));
    };
    return Apply(Parens(Many1(Lazy(Expression))), f);
}

struct ParserLambda : Parser<ast::SharedVal> {
    struct ParserFormals : Parser<QPair<QStringList, QString>> {
        Parser<QString> *Variable() { return scheme::Identifier(); }

        QPair<QStringList, QString> parse(Input &input) {
            try {
                auto vars = Try(Parens(Many(Lexeme(Variable()))))->parse(input);
                return QPair<QStringList, QString>(vars, "");
            } catch (const ParserException &) {}

            try {
                auto var = Lexeme(Variable())->parse(input);
                return QPair<QStringList, QString>(QStringList(), var);
            } catch (const ParserException &) {}

            Lexeme(Char('('))->parse(input);
            auto vars = Many1(Lexeme(Variable()))->parse(input);
            Lexeme(Char('.'))->parse(input);
            auto var = Lexeme(Variable())->parse(input);
            Lexeme(Char(')'))->parse(input);

            return QPair<QStringList, QString>(vars, var);
        }
    };
    Parser<QPair<QStringList, QString>> *Formals() { return new ParserFormals(); }

    Parser<QList<ast::SharedVal>> *Sequence() { return Many1(Expression()); }
    Parser<QList<ast::SharedVal>> *Body() {
        // todo: support definition inside lambda
        // Many(Definition())
        return Sequence();
    }

    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("lambda"))->parse(input);
        QPair<QStringList, QString> formals = Lexeme(Formals())->parse(input);
        auto body = Lexeme(Body())->parse(input);
        Lexeme(Char(')'))->parse(input);

        return ast::Lambda::create(formals.first, formals.second, body, ast::Env());
    }
};
Parser<ast::SharedVal> *Lambda() { return new ParserLambda(); }

struct ParserCondition : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("if"))->parse(input);
        auto condition = Expression()->parse(input);
        auto consequence = Expression()->parse(input);
        auto alternate = Option(Expression(), ast::Undef::create())->parse(input);
        Lexeme(Char(')'))->parse(input);
        return ast::If::create(condition, consequence, alternate);
    }
};
Parser<ast::SharedVal> *Condition() { return new ParserCondition(); }

struct ParserAssignment : Parser<ast::SharedVal> {
    Parser<QString> *Variable() { return scheme::Identifier(); }

    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("set!"))->parse(input);
        auto var = Lexeme(Variable())->parse(input);
        auto exp = Lexeme(Expression())->parse(input);
        Lexeme(Char(')'))->parse(input);
        return ast::Set::create(var, exp);
    }
};
Parser<ast::SharedVal> *Assignment() { return new ParserAssignment(); }

Parser<ast::SharedVal> *Expression() {
    // variable
    // derived expression
    // macro use
    // macro block

    return Lexeme(Choice({ Try(Literal()),
                           Try(Lambda()),
                           Try(ProcedureCall()),
                           Try(Condition()),
                           Assignment(),
                         }));
}


}

}

#endif // PARSER_H
