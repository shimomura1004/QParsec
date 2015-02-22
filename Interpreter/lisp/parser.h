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

Parser<ast::SharedVal> *Identifier() { return Apply(scheme::Identifier(), ast::Symbol::create); }
Parser<ast::SharedVal> *Variable() { return Apply(Lexeme(scheme::Variable()), ast::Variable::create); }

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

Parser<QList<ast::SharedVal>> *Sequence() { return Many1(Expression()); }
Parser<QList<ast::SharedVal>> *Body() {
    // todo: support definition inside lambda
    // Many(Definition())
    return Sequence();
}

struct ParserLambda : Parser<ast::SharedVal> {
    struct ParserFormals : Parser<QPair<QStringList, QString>> {
        QPair<QStringList, QString> parse(Input &input) {
            try {
                auto vars = Try(Parens(Many(Lexeme(scheme::Variable()))))->parse(input);
                return QPair<QStringList, QString>(vars, "");
            } catch (const ParserException &) {}

            try {
                auto var = Lexeme(scheme::Variable())->parse(input);
                return QPair<QStringList, QString>(QStringList(), var);
            } catch (const ParserException &) {}

            Lexeme(Char('('))->parse(input);
            auto vars = Many1(Lexeme(scheme::Variable()))->parse(input);
            Lexeme(Char('.'))->parse(input);
            auto var = Lexeme(scheme::Variable())->parse(input);
            Lexeme(Char(')'))->parse(input);

            return QPair<QStringList, QString>(vars, var);
        }
    };
    Parser<QPair<QStringList, QString>> *Formals() { return new ParserFormals(); }

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
        auto var = Lexeme(scheme::Variable())->parse(input);
        auto exp = Lexeme(Expression())->parse(input);
        Lexeme(Char(')'))->parse(input);
        return ast::Set::create(var, exp);
    }
};
Parser<ast::SharedVal> *Assignment() { return new ParserAssignment(); }

struct ParserCond : Parser<ast::SharedVal> {
    struct ParserCondClause : Parser<QSharedPointer<ast::Cond::CondClause>> {
        QSharedPointer<ast::Cond::CondClause> parse(Input &input) {
            Lexeme(Char('('))->parse(input);
            auto exp = Try(Lazy(Expression))->parse(input);

            try {
                // e.g. ((= x 1) => f)
                Lexeme(Str("=>"))->parse(input);
                auto recipient = Expression()->parse(input);
                Lexeme(Char(')'))->parse(input);

                ast::Cond::CondClause *clause = new ast::Cond::TestArrow(exp, recipient);
                return QSharedPointer<ast::Cond::CondClause>(clause);
            } catch (const ParserException &) {}

            try {
                // e.g. ((= x 1))
                Lexeme(Char(')'))->parse(input);

                ast::Cond::CondClause *test = new ast::Cond::Test(exp);
                return QSharedPointer<ast::Cond::CondClause>(test);
            } catch (const ParserException &) {}

            // e.g. ((= x 1) (print 1) (print 2))
            auto sequence = Sequence()->parse(input);
            Lexeme(Char(')'))->parse(input);

            ast::Cond::CondClause *clause = new ast::Cond::TestSeq(exp, sequence);
            return QSharedPointer<ast::Cond::CondClause>(clause);
        }
    };
    Parser<QSharedPointer<ast::Cond::CondClause>> *CondClause() { return new ParserCondClause(); }

    Parser<QList<ast::SharedVal>> *Else() {
        return Lexeme(Parens(Right(Lexeme(Str("else")), Sequence())));
    }

    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("cond"))->parse(input);

        try {
            auto elseexp = Try(Left(Else(), Lexeme(Char(')'))))->parse(input);

            // e.g. (cond (else ...))
            return ast::Cond::create(elseexp);
        } catch (const ParserException &) {}

        QList<QSharedPointer<ast::Cond::CondClause>> clauses;
        clauses.push_back(CondClause()->parse(input));

        try {
            Q_FOREVER {
                try {
                    // if parse 'else' succeeds, that is the last condclause
                    auto elseexp = Try(Left(Else(), Lexeme(Char(')'))))->parse(input);

                    // e.g. (cond ((= x 1) 3) (else ...))
                    return ast::Cond::create(clauses, elseexp);
                } catch (const ParserException &){}

                clauses.push_back(CondClause()->parse(input));
            }
        } catch (const ParserException &) {}

        // if parse 'condclause' fails without 'else'
        // e.g. (cond ((= x 1) 3))
        Lexeme(Char(')'))->parse(input);
        return ast::Cond::create(clauses);
    }
};
Parser<ast::SharedVal> *Cond() { return new ParserCond(); }

struct ParserCase : Parser<ast::SharedVal> {
    struct ParserCaseClause : Parser<QSharedPointer<ast::Case::CaseClause>> {
        QSharedPointer<ast::Case::CaseClause> parse(Input &input) {
            Lexeme(Char('('))->parse(input);
            auto data = Parens(Many(datum::Datum()))->parse(input);
            auto sequence = Sequence()->parse(input);
            Lexeme(Char(')'))->parse(input);

            return QSharedPointer<ast::Case::CaseClause>(new ast::Case::CaseClause(data, sequence));
        }
    };
    Parser<QSharedPointer<ast::Case::CaseClause>> *CaseClause() { return new ParserCaseClause(); }

    Parser<QList<ast::SharedVal>> *Else() {
        return Lexeme(Parens(Right(Lexeme(Str("else")), Sequence())));
    }

    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("case"))->parse(input);

        auto exp = Expression()->parse(input);

        try {
            auto elseexp = Try(Left(Else(), Lexeme(Char(')'))))->parse(input);

            // e.g. (case (else ...))
            return ast::Case::create(exp, elseexp);
        } catch (const ParserException &) {}

        QList<QSharedPointer<ast::Case::CaseClause>> clauses;
        clauses.push_back(CaseClause()->parse(input));

        try {
            Q_FOREVER {
                try {
                    // if parse 'else' succeeds, that is the last condclause
                    auto elseexp = Try(Left(Else(), Lexeme(Char(')'))))->parse(input);

                    // e.g. (case (* 2 3) (else ...))
                    return ast::Case::create(exp, clauses, elseexp);
                } catch (const ParserException &){}

                clauses.push_back(CaseClause()->parse(input));
            }
        } catch (const ParserException &) {}

        // if parse 'condclause' fails without 'else'
        // e.g. (case (* 2 3) (...))
        Lexeme(Char(')'))->parse(input);
        return ast::Case::create(exp, clauses);
    }
};
Parser<ast::SharedVal> *Case() { return new ParserCase(); }

Parser<ast::SharedVal> *And() {
    return Apply(Parens(Right(Lexeme(Str("and")), Many(Lazy(Expression)))), ast::And::create);
}

Parser<ast::SharedVal> *Or() {
    return Apply(Parens(Right(Lexeme(Str("or")), Many(Lazy(Expression)))), ast::Or::create);
}

Parser<QPair<QString, ast::SharedVal>> *BindingSpec() {
    return Lexeme(Parens(Pair(Lexeme(scheme::Variable()), Lexeme(Expression()))));
}

struct ParserLet : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);

        ast::Let::Type type = Lexeme(Choice({ Try(Right(Str("letrec"), Return(ast::Let::Rec))),
                                              Try(Right(Str("let*"), Return(ast::Let::Star))),
                                              Try(Right(Str("let"), Return(ast::Let::Normal))),
                                            }))->parse(input);

        QString name;
        if (type == ast::Let::Normal) {
            name = Option(Lexeme(scheme::Variable()), QString(""))->parse(input);
        }

        QList<QPair<QString, ast::SharedVal>> bindingspec = Lexeme(Parens(Many(BindingSpec())))->parse(input);
        QList<ast::SharedVal> body = Lexeme(Body())->parse(input);
        Lexeme(Char(')'))->parse(input);

        return ast::Let::create(name, bindingspec, body, type);
    }
};
Parser<ast::SharedVal> *Let() { return new ParserLet(); }

Parser<ast::SharedVal> *Begin() {
    ast::SharedVal(*f)(QList<ast::SharedVal>)  = [](QList<ast::SharedVal> seqs){return ast::Begin::create(seqs);};
    return Apply(Parens(Right(Lexeme(Str("begin")), Sequence())), f);
}

struct ParserDo : Parser<ast::SharedVal> {
    struct ParserIterationSpec : Parser<QSharedPointer<ast::Do::IterationSpec>> {
        QSharedPointer<ast::Do::IterationSpec> parse(Input &input) {
            Lexeme(Char('('))->parse(input);
            auto var = Lexeme(scheme::Variable())->parse(input);
            auto init = Expression()->parse(input);

            try {
                auto step = Expression()->parse(input);
                Lexeme(Char(')'))->parse(input);
                return ast::Do::IterationSpecWithStep::create(var, init, step);
            }
            catch (const ParserException&) {
                Lexeme(Char(')'))->parse(input);
                return ast::Do::IterationSpec::create(var, init);
            }
        }
    };
    Parser<QSharedPointer<ast::Do::IterationSpec>> *IterationSpec() { return new ParserIterationSpec(); }

    ast::SharedVal parse(Input &input) {
        Lexeme(Char('('))->parse(input);
        Lexeme(Str("do"))->parse(input);
        auto iterationSpecs = Lexeme(Parens(Many(IterationSpec())))->parse(input);
        auto test_doresult = Lexeme(Parens(Pair(Expression(), Expression())))->parse(input);
        auto commands = Lexeme(Many(Expression()))->parse(input);
        Lexeme(Char(')'))->parse(input);

        return ast::Do::create(iterationSpecs, test_doresult.first, test_doresult.second, commands);
    }
};
Parser<ast::SharedVal> *Do() { return new ParserDo(); }

struct ParserDerivedExpression : Parser<ast::SharedVal> {
    ast::SharedVal parse(Input &input) {
        return Choice({ Try(Cond()),
                        Try(Case()),
                        Try(And()),
                        Try(Or()),
                        Try(Let()),
                        Try(Begin()),
                        Try(Do())
                      })->parse(input);

//            Lexeme(Str("delay"))->parse(input);
//        } catch (const ParserException &) {}

        // todo: support quasification
    }
};
Parser<ast::SharedVal> *DerivedExpression() { return new ParserDerivedExpression(); }

Parser<ast::SharedVal> *Expression() {
    // variable
    // derived expression
    // macro use
    // macro block

    return Lexeme(Choice({ Try(Variable()),
                           Try(Literal()),
                           Try(Lambda()),
                           Try(ProcedureCall()),
                           Try(Condition()),
                           Try(Assignment()),
                           DerivedExpression(),
                         }));
}


}

}

#endif // PARSER_H
