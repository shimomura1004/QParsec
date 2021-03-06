#include "parserand.h"
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

Parser<ast::SharedVal> *And() {
    return Apply(Parens(Right(Lexeme(Str("and")), Many(Lazy(Expression)))), ast::And::create);
}

}
}
}
