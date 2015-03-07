#include "parsercond.h"
#include "expression.h"
#include "prim/prim.h"
#include "character/character.h"
#include "token/token.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace token;

QSharedPointer<ast::Cond::CondClause> ParserCond::ParserCondClause::parse(Input &input) {
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

Parser<QSharedPointer<ast::Cond::CondClause> > *ParserCond::CondClause() {
    return new ParserCondClause();
}

Parser<QList<ast::SharedVal> > *ParserCond::Else() {
    return Lexeme(Parens(Right(Lexeme(Str("else")), Sequence())));
}

ast::SharedVal ParserCond::parse(Input &input) {
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

Parser<ast::SharedVal> *Cond() { return new ParserCond(); }

}
}
}
