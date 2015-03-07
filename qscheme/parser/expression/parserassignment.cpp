#include "parserassignment.h"
#include "expression.h"
#include "character/character.h"
#include "token/token.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace character;
using namespace token;
using namespace language;

Parser<QString> *ParserAssignment::Variable() {
    return scheme::Identifier();
}

ast::SharedVal ParserAssignment::parse(Input &input) {
    Lexeme(Char('('))->parse(input);
    Lexeme(Str("set!"))->parse(input);
    auto var = Lexeme(scheme::Variable())->parse(input);
    auto exp = Lexeme(Expression())->parse(input);
    Lexeme(Char(')'))->parse(input);
    return ast::Set::create(var, exp);
}

Parser<ast::SharedVal> *Assignment() {
    return new ParserAssignment();
}

}
}
}
