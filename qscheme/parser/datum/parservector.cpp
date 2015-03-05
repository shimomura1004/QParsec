#include "parservector.h"
#include "prim/prim.h"
#include "character/character.h"
#include "token/token.h"
#include "parserdatum.h"

namespace qscheme {
namespace parser {
namespace datum {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace token;

qparsec::Parser<ast::SharedVal> *Vector() {
    return Apply(Right(Char('#'), Parens(Many(Lazy(Datum)))), ast::Vector::create);
}

}
}
}
