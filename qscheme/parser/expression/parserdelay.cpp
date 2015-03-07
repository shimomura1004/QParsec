#include "parserdelay.h"
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

Parser<ast::SharedVal> *Delay() {
    return Apply(Lexeme(Parens(Right(Lexeme(Str("delay")), Expression()))), ast::Delay::create);
}

}
}
}
