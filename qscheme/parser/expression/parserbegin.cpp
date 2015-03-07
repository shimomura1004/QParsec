#include "parserbegin.h"
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

Parser<ast::SharedVal> *Begin() {
    ast::SharedVal(*f)(QList<ast::SharedVal>)  = [](QList<ast::SharedVal> seqs){return ast::Begin::create(seqs);};
    return Apply(Parens(Right(Lexeme(Str("begin")), Sequence())), f);
}

}
}
}
