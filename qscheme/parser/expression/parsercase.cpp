#include "parsercase.h"
#include "expression.h"
#include "parser/datum/datum.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "token/token.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;
using namespace token;

QSharedPointer<ast::Case::CaseClause> ParserCase::ParserCaseClause::parse(Input &input) {
    Lexeme(Char('('))->parse(input);
    auto data = Parens(Many(datum::Datum()))->parse(input);
    auto sequence = Sequence()->parse(input);
    Lexeme(Char(')'))->parse(input);

    return QSharedPointer<ast::Case::CaseClause>(new ast::Case::CaseClause(data, sequence));
}

Parser<QSharedPointer<ast::Case::CaseClause> > *ParserCase::CaseClause() { return new ParserCaseClause(); }

Parser<QList<ast::SharedVal> > *ParserCase::Else() {
    return Lexeme(Parens(Right(Lexeme(Str("else")), Sequence())));
}

ast::SharedVal ParserCase::parse(Input &input) {
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

Parser<ast::SharedVal> *Case() { return new ParserCase(); }

}
}
}
