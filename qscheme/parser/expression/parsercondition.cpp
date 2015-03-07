#include "parsercondition.h"
#include "expression.h"
#include "character/character.h"
#include "token/token.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace character;
using namespace token;

ast::SharedVal ParserCondition::parse(Input &input) {
    Lexeme(Char('('))->parse(input);
    Lexeme(Str("if"))->parse(input);
    auto condition = Expression()->parse(input);
    auto consequence = Expression()->parse(input);
    auto alternate = Option(Expression(), ast::Undef::create())->parse(input);
    Lexeme(Char(')'))->parse(input);
    return ast::If::create(condition, consequence, alternate);
}

Parser<ast::SharedVal> *Condition() {
    return new ParserCondition();
}

}
}
}
