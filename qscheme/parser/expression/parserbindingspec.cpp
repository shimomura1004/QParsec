#include "parserbindingspec.h"
#include "expression.h"
#include "prim/prim.h"
#include "token/token.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace token;
using namespace language;

Parser<QPair<QString, ast::SharedVal> > *BindingSpec() {
    return Lexeme(Parens(Pair(Lexeme(scheme::Variable()), Lexeme(Expression()))));
}

}
}
}
