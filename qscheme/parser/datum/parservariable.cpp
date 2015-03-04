#include "parservariable.h"
#include "prim/prim.h"
#include "token/token.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace datum {

using namespace qparsec;
using namespace prim;
using namespace token;
using namespace language;

Parser<ast::SharedVal> *Variable() {
    return Apply(Lexeme(scheme::Variable()), ast::Variable::create);
}

}
}
}
