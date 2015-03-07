#include "parserquotation.h"
#include "expression.h"
#include "parser/datum/datum.h"
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

ast::SharedVal ParserQuotation::parse(qparsec::Input &input) {
    try {
        auto datum = Parens(Right(Lexeme(Str("quote")), datum::Datum()))->parse(input);
        return ast::Quote::create(datum);
    } catch (const ParserException &) {}

    Char('\'')->parse(input);
    auto datum = datum::Datum()->parse(input);
    return ast::Quote::create(datum);
}

qparsec::Parser<ast::SharedVal> *Quotation() { return new ParserQuotation(); }

}
}
}
