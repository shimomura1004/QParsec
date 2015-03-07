#include "parserexpression.h"
#include "expression.h"
#include "prim/prim.h"
#include "combinator/combinator.h"
#include "token/token.h"
#include "parser/datum/parservariable.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace combinator;
using namespace token;

Parser<ast::SharedVal> *Expression() {
    // macro use
    // macro block

    return Lexeme(Choice({ Try(datum::Variable()),
                           Try(Literal()),
                           Try(Lambda()),
                           Try(ProcedureCall()),
                           Try(Condition()),
                           Try(Assignment()),
                           DerivedExpression(),
                         }));
}

}
}
}
